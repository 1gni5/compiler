#include <iostream>

#include "antlr4-runtime.h"
#include "arm64Visitor.h"

using namespace std;

any arm64Visitor::visitAxiom(ccParser::AxiomContext *ctx)
{
    cout << "Visit Axiom" << endl;
    visit(ctx->program());
    return 0;
}

any arm64Visitor::visitProgram(ccParser::ProgramContext *ctx)
{
    cout << "Visit Program" << endl;
    visit(ctx->returnStmt());
    return 0;
}

any arm64Visitor::visitReturnStmt(ccParser::ReturnStmtContext *ctx)
{
    cout << "Visit Return Statement" << endl;
    cout << "CONST: " << ctx->CONST()->getText() << endl;
    return 0;
}