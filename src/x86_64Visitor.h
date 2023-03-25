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
  virtual std::any visitParenthesis(ccParser::ParenthesisContext *ctx);
  virtual std::any visitAddition(ccParser::AdditionContext *ctx);
  virtual std::any visitMultiplication(ccParser::MultiplicationContext *ctx);
  virtual std::any visitConstExpression(ccParser::ConstExpressionContext *ctx);
  virtual std::any visitVarExpression(ccParser::VarExpressionContext *ctx);
  virtual std::any visitBitExpression(ccParser::BitExpressionContext *ctx);

  // Misc.
  virtual std::any visitComparison(ccParser::ComparisonContext *ctx);
  virtual std::any visitUnary(ccParser::UnaryContext *ctx);

private:
  string registerTemporary();
  map<string, size_t> symbols;
};