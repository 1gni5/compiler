#include <iostream>
#include <exception>

#include "antlr4-runtime.h"
#include "x86_64Visitor.h"

using namespace std;

string x86_64Visitor::registerTemporary()
{
    string name = "$tmp" + to_string(symbols.size());
    symbols[name] = (symbols.size() + 1) * 4;
    return name;
}

any x86_64Visitor::visitAxiom(ccParser::AxiomContext *ctx)
{
    visit(ctx->program());
    return 0;
}

any x86_64Visitor::visitProgram(ccParser::ProgramContext *ctx)
{
    cout << ".globl main\n" // Add program entrypoint
         << "main:\n"
         << "\tpushq\t%rbp\n" // Initialize the stack
         << "\tmovq\t%rsp, %rbp\n" << endl;

    // Visit main body
    visit(ctx->compound());
    return 0; // Dummy return
}

any x86_64Visitor::visitCompound(ccParser::CompoundContext *ctx)
{
    // Visit every statement (decl, assign, return)
    for (auto& stmt : ctx->statement())
    {
        visit(stmt);
    }

    return 0; // Dummy return
}

any x86_64Visitor::visitReturnStmt(ccParser::ReturnStmtContext *ctx)
{
    // Retreive name of temporary variable with expression result
    string name = any_cast<string>(visit(ctx->expr()));
    cout << "\tmovl\t-" << symbols[name] << "(%rbp), %eax\n"
         << "\tpopq\t%rbp\n" // Clean the stack
         << "\tret" << endl;
    return 0; // Dummy return
}

any x86_64Visitor::visitDeclaration(ccParser::DeclarationContext *ctx) 
{
    // Register the new name in table of symbols
    for (auto& id : ctx->IDENTIFIER())
    {
        string name = id->getText();
        if (symbols.contains(name)) {throw runtime_error("Already declared.");}

        // As we only support int type, every element is of size 4 bytes
        symbols[name] = (symbols.size() + 1) * 4;
    }

    // If an expression is present, assign it's value to last name declared
    if (ctx->expr() != nullptr)
    {
        // Get the last name declared
        size_t index = ctx->IDENTIFIER().size();
        string left = ctx->IDENTIFIER(index - 1)->getText();

        // Retreive name of temporary variable with expression result
        string right = any_cast<string>(visit(ctx->expr()));

        // Simply move: right -> reg(eax) -> left
        cout << "\tmovl\t-" << symbols[right] << "(%rbp)" << ", %eax\n"
             << "\tmovl\t %eax, -" << symbols[left] << "(%rbp)" << endl;
        
        // Assignement should return value in C
        // NOTE: Not yet fully implemented
        return left;
    }

    // Dummy return
    return 0;
}

any x86_64Visitor::visitAssignement(ccParser::AssignementContext *ctx)
{
    // Retreive the name of the variable
    string left = ctx->IDENTIFIER()->getText();

    // Retreive name of temporary variable with expression result
    string right = any_cast<string>(visit(ctx->expr()));

    // Simply move: rigth -> reg(eax) -> left
    cout << "\tmovl\t-" << symbols[right] << "(%rbp)" << ", %eax\n"
         << "\tmovl\t %eax, -" << symbols[left] << "(%rbp)" << endl;
    
    return left;
}

any x86_64Visitor::visitCstExpr(ccParser::CstExprContext *ctx)
{
    // Register a temporary variable to store the value of expression
    // NOTE: Not necessary to store every constante in memory, can be improved
    string name = registerTemporary();

    // Move const value to memory
    cout << "\tmovl\t$" << ctx->CONST()->getText()
         << ", -" << symbols[name] << "(%rbp)\n" << endl;

    return name;
}

any x86_64Visitor::visitVarExpr(ccParser::VarExprContext *ctx)
{
    // When expression is only a variable, just need to return the name
    string name = ctx->IDENTIFIER()->getText();
    if (!symbols.contains(name)) {throw runtime_error("Not declared.");}
    return name;
}

any x86_64Visitor::visitAddExpr(ccParser::AddExprContext *ctx)
{
    // Retreive both left and right expression results
    string left = any_cast<string>(visit(ctx->expr(0)));
    string right = any_cast<string>(visit(ctx->expr(1)));

    // Register a temporary variable to store the result of the expression
    string name = registerTemporary();

    // For operator priority both + and - are in the same function
    if (ctx->op->getText() == "+")
    {
    cout << "\tmovl\t-" << symbols[left] << "(%rbp), %edx\n"
         << "\tmovl\t-" << symbols[right] << "(%rbp), %eax\n"
         << "\taddl\t%edx, %eax\n" // TODO: Check if possible to only use 1 register
         << "\tmovl\t%eax, -" << symbols[name] << "(%rbp)\n" << endl;
    } else // - operator case
    {
    cout << "\tmovl\t-" << symbols[left] << "(%rbp), %eax\n"
         << "\tsubl\t-" << symbols[right] << "(%rbp), %eax\n"
         << "\tmovl\t%eax, -" << symbols[name] << "(%rbp)\n" << endl;
    }

    return name;
}

any x86_64Visitor::visitMultExpr(ccParser::MultExprContext *ctx)
{
    // Retreive both left and right expression results
    string left = any_cast<string>(visit(ctx->expr(0)));
    string right = any_cast<string>(visit(ctx->expr(1)));

    // Register a temporary variable to store the result of the expression
    string name = registerTemporary();

    // For operator priority both * and / are in the same function
    if (ctx->op->getText() == "*")
    {
        cout << "\tmovl\t-" << symbols[left] << "(%rbp), %eax\n"
             << "\timull\t-" << symbols[right] << "(%rbp), %eax\n"
             << "\tmovl\t%eax, -" << symbols[name] << "(%rbp)\n" << endl;
    } else // / operator case
    {
        cout << "\tmovl\t-" << symbols[left] << "(%rbp), %eax\n"
             << "\tcltd\n" // Setup recovery point in case of / 0
             << "\tidivl\t-" << symbols[right] << "(%rbp)\n"
             << "\tmovl\t%eax, -" << symbols[name] << "(%rbp)\n" << endl;
    }

    return name;
}

any x86_64Visitor::visitCmpExpr(ccParser::CmpExprContext *ctx)
{
    // Retreive both left and right expression results
    string left = any_cast<string>(visit(ctx->expr(0)));
    string right = any_cast<string>(visit(ctx->expr(1)));

    // Register a temporary variable to store the result of the expression
    string name = registerTemporary();


    // Assembly language resulting is very similar, so
    // we only need to change one instruction
    string op = ctx->op->getText();
    map<string, string> verbs = {
        {">=", "setge"},
        {"<=", "setle"},
        {">", "setg"},
        {"<", "setl"}
    };

    cout << "\tmovl\t-" << symbols[left] << "(%rbp), %eax\n"
         << "\tcmpl\t-" << symbols[right] << "(%rbp), %eax\n"
         << "\t" << verbs[op] << "\t%al\n" // Insert the matching instruction
         << "\tandb\t$1, %al\n"
         << "\tmovzbl\t%al, %eax\n" 
         << "\tmovl\t%eax, -" << symbols[name] << "(%rbp)" << endl;
    
    return name;
}

any x86_64Visitor::visitEqExpr(ccParser::EqExprContext *ctx)
{
    // Retreive both left and right expression results
    string left = any_cast<string>(visit(ctx->expr(0)));
    string right = any_cast<string>(visit(ctx->expr(1)));

    // Register a temporary variable to store the result of the expression
    string name = registerTemporary();


    // Assembly language resulting is very similar, so
    // we only need to change one instruction
    string op = ctx->op->getText();
    map<string, string> verbs = {
        {"==", "sete"},
        {"!=", "setne"}
    };

    cout << "\tmovl\t-" << symbols[left] << "(%rbp), %eax\n"
         << "\tcmpl\t-" << symbols[right] << "(%rbp), %eax\n"
         << "\t" << verbs[op] << "\t%al\n" // Insert the matching instruction
         << "\tandb\t$1, %al\n"
         << "\tmovzbl\t%al, %eax\n" 
         << "\tmovl\t%eax, -" << symbols[name] << "(%rbp)" << endl;
    
    return name;
}

any x86_64Visitor::visitUnaryExpr(ccParser::UnaryExprContext *ctx)
{
    // Retreive temporary variable name with expression results
    string right = any_cast<string>(visit(ctx->expr()));

    // Register a temporary variable to store the result of the expression
    string name = registerTemporary(); 

    // For operator priority both - and ! are in the same function
    if (ctx->op->getText() == "-")
    {
        cout << "\tmovl\t-" << symbols[right] << "(%rbp), %eax\n"
             << "\txorl\t%eax, %eax\n" 
             << "\tsubl\t-" << symbols[right] << "(%rbp), %eax" << endl;
    } else { // case of ! operator
        cout << "\tcmpl\t$0,-" << symbols[right] << "(%rbp)\n"
             << "\tsetne\t%al\n"
             << "\txorb\t$-1, %al\n"
             << "\tandb\t$1, %al\n"
             << "\tmovzbl\t%al, %eax" << endl;
    }

    // Move the result of the expression in temporary variable
    cout << "\tmovl\t%eax, -" << symbols[name] << "(%rbp)" << endl;

    return name;
}

any x86_64Visitor::visitBitAndExpr(ccParser::BitAndExprContext *ctx)
{
    // Retreive both left and right expression results
    string left = any_cast<string>(visit(ctx->expr(0)));
    string right = any_cast<string>(visit(ctx->expr(1)));

    string name = registerTemporary();

    cout << "\tmovl\t-" << symbols[right] << "(%rbp), %eax\n"
         << "\tandl\t-" << symbols[left] << "(%rbp), %eax\n"
         << "\tmovl\t%eax, -" << symbols[name] << "(%rbp)" << endl; // Move result to temporary variable

    return name;
}

any x86_64Visitor::visitBitXorExpr(ccParser::BitXorExprContext *ctx)
{
    // Retreive both left and right expression results
    string left = any_cast<string>(visit(ctx->expr(0)));
    string right = any_cast<string>(visit(ctx->expr(1)));

    string name = registerTemporary();

    cout << "\tmovl\t-" << symbols[right] << "(%rbp), %eax\n"
         << "\txorl\t-" << symbols[left] << "(%rbp), %eax\n"
         << "\tmovl\t%eax, -" << symbols[name] << "(%rbp)" << endl; // Move result to temporary variable

    return name;
}

any x86_64Visitor::visitBitOrExpr(ccParser::BitOrExprContext *ctx)
{
    // Retreive both left and right expression results
    string left = any_cast<string>(visit(ctx->expr(0)));
    string right = any_cast<string>(visit(ctx->expr(1)));

    string name = registerTemporary();

    cout << "\tmovl\t-" << symbols[right] << "(%rbp), %eax\n"
         << "\torl\t-" << symbols[left] << "(%rbp), %eax\n"
         << "\tmovl\t%eax, -" << symbols[name] << "(%rbp)" << endl; // Move result to temporary variable

    return name;
}

any x86_64Visitor::visitParenthesis(ccParser::ParenthesisContext *ctx)
{
    // Return result of contained expression
    return visit(ctx->expr());
}