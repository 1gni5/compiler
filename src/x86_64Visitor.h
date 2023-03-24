#pragma once

#include <map>
#include <string>

#include "antlr4-runtime.h"
#include "generated/ccBaseVisitor.h"

using namespace std;

class x86_64Visitor : public ccBaseVisitor
{
public:
  virtual antlrcpp::Any visitAxiom(ccParser::AxiomContext *ctx);
  virtual antlrcpp::Any visitProgram(ccParser::ProgramContext *ctx);
  virtual antlrcpp::Any visitCompound(ccParser::CompoundContext *ctx);

  virtual antlrcpp::Any visitReturnStmt(ccParser::ReturnStmtContext *ctx);
  virtual antlrcpp::Any visitDeclaration(ccParser::DeclarationContext *ctx);
  virtual antlrcpp::Any visitAssignement(ccParser::AssignementContext *ctx);

  virtual antlrcpp::Any visitParenthesis(ccParser::ParenthesisContext *ctx);
  virtual antlrcpp::Any visitAddition(ccParser::AdditionContext *ctx);
  virtual antlrcpp::Any visitMultiplication(ccParser::MultiplicationContext *ctx);
  virtual antlrcpp::Any visitConstExpression(ccParser::ConstExpressionContext *ctx);
  virtual antlrcpp::Any visitVarExpression(ccParser::VarExpressionContext *ctx);

  virtual antlrcpp::Any visitComparison(ccParser::ComparisonContext *ctx);

private:
  map<string, size_t> symbols;
};