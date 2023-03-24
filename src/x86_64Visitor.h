#pragma once

#include <map>
#include <string>

#include "antlr4-runtime.h"
#include "generated/ccBaseVisitor.h"

using namespace std;

class x86_64Visitor : public ccBaseVisitor
{
public:
  virtual std::any visitAxiom(ccParser::AxiomContext *ctx);
  virtual std::any visitProgram(ccParser::ProgramContext *ctx);
  virtual std::any visitCompound(ccParser::CompoundContext *ctx);

  virtual std::any visitReturnStmt(ccParser::ReturnStmtContext *ctx);
  virtual std::any visitDeclaration(ccParser::DeclarationContext *ctx);
  virtual std::any visitAssignement(ccParser::AssignementContext *ctx);

  virtual std::any visitParenthesis(ccParser::ParenthesisContext *ctx);
  virtual std::any visitAddition(ccParser::AdditionContext *ctx);
  virtual std::any visitMultiplication(ccParser::MultiplicationContext *ctx);
  virtual std::any visitConstExpression(ccParser::ConstExpressionContext *ctx);
  virtual std::any visitVarExpression(ccParser::VarExpressionContext *ctx);

  virtual std::any visitComparison(ccParser::ComparisonContext *ctx);

private:
  map<string, size_t> symbols;
};