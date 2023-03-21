#pragma once

#include "antlr4-runtime.h"
#include "generated/ccBaseVisitor.h"

class arm64Visitor : public ccBaseVisitor
{
public:
  virtual std::any visitAxiom(ccParser::AxiomContext *ctx);
  virtual std::any visitProgram(ccParser::ProgramContext *ctx);
  virtual std::any visitReturnStmt(ccParser::ReturnStmtContext *ctx);
};