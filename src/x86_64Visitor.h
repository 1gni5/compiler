#pragma once

#include <map>
#include <string>

#include "antlr4-runtime.h"
#include "generated/ccBaseVisitor.h"

using namespace std;

class x86_64Visitor : public ccBaseVisitor
{
public:
  // Base of the program
  virtual std::any visitAxiom(ccParser::AxiomContext *ctx);
  virtual std::any visitProgram(ccParser::ProgramContext *ctx);

  // Control flow
  virtual std::any visitCompound(ccParser::CompoundContext *ctx);

  // Statements
  virtual std::any visitReturnStmt(ccParser::ReturnStmtContext *ctx);
  virtual std::any visitDeclaration(ccParser::DeclarationContext *ctx);
  virtual std::any visitAssignement(ccParser::AssignementContext *ctx);

  // Expressions
  // Const and variables
  virtual std::any visitVarExpr(ccParser::VarExprContext *ctx);
  virtual std::any visitCstExpr(ccParser::CstExprContext *ctx);

  // Basic arithmetics
  virtual std::any visitAddExpr(ccParser::AddExprContext *ctx);
  virtual std::any visitMultExpr(ccParser::MultExprContext *ctx);

  // Bit operators
  virtual std::any visitBitAndExpr(ccParser::BitAndExprContext *ctx);
  virtual std::any visitBitOrExpr(ccParser::BitOrExprContext *ctx);
  virtual std::any visitBitXorExpr(ccParser::BitXorExprContext *ctx);

  // Comparisons, Equalities
  virtual std::any visitCmpExpr(ccParser::CmpExprContext *ctx);
  virtual std::any visitEqExpr(ccParser::EqExprContext *ctx);
  
  // Misc.
  virtual std::any visitUnaryExpr(ccParser::UnaryExprContext *ctx);
  virtual std::any visitParenthesis(ccParser::ParenthesisContext *ctx);

private:
  string registerTemporary();
  map<string, size_t> symbols;
};