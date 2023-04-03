#include <string>
#include "SymbolTable.h"

using namespace std;

SymbolTable& SymbolTable::getInstance()
{
  static SymbolTable instance;
  return instance;
}

size_t& SymbolTable::operator[](string key)
{
  return symbols[key];
}

string SymbolTable::createTemporary()
{
  string name = "%" + to_string(symbols.size());
  symbols[name] = (symbols.size() + 1) * 4;
  return name;
}