#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "antlr4-runtime.h"
#include "generated/ccLexer.h"
#include "generated/ccParser.h"
#include "generated/ccBaseVisitor.h"

#include "x86_64Visitor.h"
#include "IRVisitor.h"
#include "CFG.h"

using namespace antlr4;
using namespace std;

int main(int argn, const char **argv)
{
  stringstream in;
  if (argn==2)
  {
     ifstream lecture(argv[1]);
     in << lecture.rdbuf();
  }
  else
  {
      cerr << "usage: ifcc path/to/file.c" << endl ;
      exit(1);
  }
  
  ANTLRInputStream input(in.str());

  ccLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  tokens.fill();

  ccParser parser(&tokens);
  tree::ParseTree* tree = parser.axiom();

  if(parser.getNumberOfSyntaxErrors() != 0)
  {
      cerr << "error: syntax error during parsing" << endl;
      exit(1);
  }

  IRVisitor v;
  CFG cfg = v.build(tree);

  return 0;
}
