#include <iostream>
#include <exception>

#include "antlr4-runtime.h"
#include "x86_64Visitor.h"

using antlrcpp::Any;

Any x86_64Visitor::visitAxiom(ccParser::AxiomContext *ctx)
{
    visit(ctx->program());
    return 0;
}

Any x86_64Visitor::visitProgram(ccParser::ProgramContext *ctx)
{
    cout << ".globl main\n"
         << "main:\n"
         << "\tpushq\t%rbp\n"
         << "\tmovq\t%rsp, %rbp\n" << endl;
    visit(ctx->compound());
    return 0;
}

Any x86_64Visitor::visitCompound(ccParser::CompoundContext *ctx)
{
    for (auto& stmt : ctx->statement())
    {
        visit(stmt);
    }
    return 0;
}


Any x86_64Visitor::visitReturnStmt(ccParser::ReturnStmtContext *ctx)
{
    string name = (visit(ctx->expr())).as<string>();
    cout << "\tmovl\t-" << symbols[name] << "(%rbp), %eax\n"
         << "\tpopq\t%rbp\n"
         << "\tret" << endl;
    return 0;
}

Any x86_64Visitor::visitDeclaration(ccParser::DeclarationContext *ctx) 
{
    // Register in table of characters
    for (auto& id : ctx->IDENTIFIER())
    {
        string name = id->getText();
        if (symbols.count(name)>0) {throw runtime_error("Already declared.");}
        symbols[name] = (symbols.size() + 1) * 4;
    }

    if (ctx->expr() != nullptr)
    {
        size_t index = ctx->IDENTIFIER().size();
        string left = ctx->IDENTIFIER(index - 1)->getText();
        string right = (visit(ctx->expr())).as<string>();

        cout << "\tmovl\t-" << symbols[right] << "(%rbp)" << ", %eax\n"
             << "\tmovl\t %eax, -" << symbols[left] << "(%rbp)" << endl;
        
        return left;
    }

    return 0;
}

Any x86_64Visitor::visitAssignement(ccParser::AssignementContext *ctx)
{
    string left = ctx->IDENTIFIER()->getText();
    string right = (visit(ctx->expr())).as<string>();

    cout << "\tmovl\t-" << symbols[right] << "(%rbp)" << ", %eax\n"
         << "\tmovl\t %eax, -" << symbols[left] << "(%rbp)" << endl;
    
    return left;
}

Any x86_64Visitor::visitConstExpression(ccParser::ConstExpressionContext *ctx)
{
    string name = "$tmp" + to_string(symbols.size());
    symbols[name] = (symbols.size() + 1) * 4;
    cout << "\tmovl\t$" << ctx->CONST()->getText()
         << ", -" << symbols[name] << "(%rbp)\n" << endl;

    return name;
}

Any x86_64Visitor::visitVarExpression(ccParser::VarExpressionContext *ctx)
{
    string name = ctx->IDENTIFIER()->getText();
    if (!symbols.count(name)>0) {throw runtime_error("Not declared.");}
    return name;
}

Any x86_64Visitor::visitAddition(ccParser::AdditionContext *ctx)
{
    string left = (visit(ctx->expr(0))).as<string>();
    string right = (visit(ctx->expr(1))).as<string>();

    string name = "$tmp" + to_string(symbols.size());
    symbols[name] = (symbols.size() + 1) * 4;
    

    if (ctx->op->getText() == "+")
    {
    cout << "\tmovl\t-" << symbols[left] << "(%rbp), %edx\n"
         << "\tmovl\t-" << symbols[right] << "(%rbp), %eax\n"
         << "\taddl\t%edx, %eax\n"
         << "\tmovl\t%eax, -" << symbols[name] << "(%rbp)\n" << endl;
    } else
    {
    cout << "\tmovl\t-" << symbols[left] << "(%rbp), %eax\n"
         << "\tsubl\t-" << symbols[right] << "(%rbp), %eax\n"
         << "\tmovl\t%eax, -" << symbols[name] << "(%rbp)\n" << endl;
    }

    return name;
}

Any x86_64Visitor::visitMultiplication(ccParser::MultiplicationContext *ctx)
{
    string left = (visit(ctx->expr(0))).as<string>();
    string right = (visit(ctx->expr(1))).as<string>();

    string name = "$tmp" + to_string(symbols.size());
    symbols[name] = (symbols.size() + 1) * 4;

    if (ctx->op->getText() == "*")
    {
        cout << "\tmovl\t-" << symbols[left] << "(%rbp), %eax\n"
            << "\timull\t-" << symbols[right] << "(%rbp), %eax\n"
            << "\tmovl\t%eax, -" << symbols[name] << "(%rbp)\n" << endl;
    } else 
    {
        cout << "\tmovl\t-" << symbols[left] << "(%rbp), %eax\n"
            << "\tcltd\n"
            << "\tidivl\t-" << symbols[right] << "(%rbp)\n"
            << "\tmovl\t%eax, -" << symbols[name] << "(%rbp)\n" << endl;

    }

    return name;
}

Any x86_64Visitor::visitParenthesis(ccParser::ParenthesisContext *ctx)
{
    return visit(ctx->expr());
}


Any x86_64Visitor::visitComparison(ccParser::ComparisonContext *ctx)
{
    string left = (visit(ctx->expr(0))).as<string>();
    string right = (visit(ctx->expr(1))).as<string>();

    string name = "$tmp" + to_string(symbols.size());
    symbols[name] = (symbols.size() + 1) * 4;


    string op = ctx->op->getText();
    map<string, string> verbs = {
        {"==", "sete"},
        {"!=", "setne"},
        {">=", "setge"},
        {"<=", "setle"},
        {">", "setg"},
        {"<", "setl"},
    };

    cout << "\tmovl\t-" << symbols[left] << "(%rbp), %eax\n"
         << "\tcmpl\t-" << symbols[right] << "(%rbp), %eax\n"
         << "\t" << verbs[op] << "\t%al\n"
         << "\tandb\t$1, %al\n"
         << "\tmovzbl\t%al, %eax\n" 
         << "\tmovl\t%eax, -" << symbols[name] << "(%rbp)" << endl;
    
    return name;
}