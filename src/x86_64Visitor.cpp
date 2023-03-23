#include <iostream>
#include <sstream>

#include "antlr4-runtime.h"
#include "x86_64Visitor.h"

using namespace std;

any x86_64Visitor::visitAxiom(ccParser::AxiomContext *ctx)
{
    visit(ctx->program());
    return 0;
}

any x86_64Visitor::visitProgram(ccParser::ProgramContext *ctx)
{
    cout << ".globl main\n"
         << "main:\n"
         << "\tpushq\t%rbp\n"
         << "\tmovq\t%rsp, %rbp\n" << endl;
    visit(ctx->returnStmt());
    return 0;
}

any x86_64Visitor::visitReturnStmt(ccParser::ReturnStmtContext *ctx)
{
    string name = any_cast<string>(visit(ctx->expr()));
    cout << "\tmovl\t-" << symbols[name] << "(%rbp), %eax\n"
         << "\tpopq\t%rbp\n"
         << "\tret" << endl;
    return 0;
}

any x86_64Visitor::visitConstExpression(ccParser::ConstExpressionContext *ctx)
{
    string name = "$tmp" + to_string(symbols.size());
    symbols[name] = (symbols.size() + 1) * 4;
    cout << "\tmovl\t$" << ctx->CONST()->getText()
         << ", -" << symbols[name] << "(%rbp)\n" << endl;

    return name;
}

any x86_64Visitor::visitAddition(ccParser::AdditionContext *ctx)
{
    string left = any_cast<string>(visit(ctx->expr(0)));
    string right = any_cast<string>(visit(ctx->expr(1)));

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

any x86_64Visitor::visitMultiplication(ccParser::MultiplicationContext *ctx)
{
    string left = any_cast<string>(visit(ctx->expr(0)));
    string right = any_cast<string>(visit(ctx->expr(1)));

    string name = "$tmp" + to_string(symbols.size());
    symbols[name] = (symbols.size() + 1) * 4;

    if (ctx->op->getText() == "*")
    {
        cout << "\tmovl\t-" << symbols[left] << "(%rbp), %eax\n"
            << "\timul\t-" << symbols[right] << "(%rbp), %eax\n"
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

any x86_64Visitor::visitParenthesis(ccParser::ParenthesisContext *ctx)
{
    return visit(ctx->expr());
}