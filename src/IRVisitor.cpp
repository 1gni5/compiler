#include "IRVisitor.h"

CFG IRVisitor::build(tree::ParseTree *tree)
{
    visit(tree);
    return cfg;
}

any IRVisitor::visitAxiom(ccParser::AxiomContext *ctx)
{
    visit(ctx->program());
    return 0;
}

any IRVisitor::visitProgram(ccParser::ProgramContext *ctx)
{
    visit(ctx->compound());
    return 0;
}

any IRVisitor::visitCompound(ccParser::CompoundContext *ctx)
{
    cfg.bbs.push_back(new BB());
    for (auto& stmt : ctx->statement())
    {
        visit(stmt);
    }
    return 0;
}

any IRVisitor::visitReturnStmt(ccParser::ReturnStmtContext *ctx)
{
    string name = any_cast<string>(ctx->expr());

    BB* current = cfg.bbs.back();
    current->instructions.push_back(Ret(name));
    return 0;
}

any IRVisitor::visitCstExpr(ccParser::CstExprContext *ctx)
{
    string name = st.createTemporary();
    string value = ctx->CONST()->getText();

    BB* current = cfg.bbs.back();
    current->instructions.push_back(Store(value, name));
    return name;
}

