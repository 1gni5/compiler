#pragma once

#include "antlr4-runtime.h"
#include "generated/ccBaseVisitor.h"
#include "SymbolTable.h"
#include "CFG.h"

using namespace std;
using namespace antlr4;

class IRVisitor : public ccBaseVisitor
{
    public:
    IRVisitor() :st(SymbolTable::getInstance()) {}
    CFG build(tree::ParseTree *tree);
    virtual any visitAxiom(ccParser::AxiomContext *ctx);
    virtual any visitProgram(ccParser::ProgramContext *ctx);
    virtual any visitCompound(ccParser::CompoundContext *ctx);
    virtual any visitReturnStmt(ccParser::ReturnStmtContext *ctx);
    virtual any visitCstExpr(ccParser::CstExprContext *ctx);

    private:
    SymbolTable& st;
    CFG cfg;
};