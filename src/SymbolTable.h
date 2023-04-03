#pragma once

#include <map>
#include <string>

using namespace std;


class SymbolTable {
  public:
  static SymbolTable& getInstance();
  size_t& operator[](string key);
  string createTemporary();

  private:
  SymbolTable() {};
  map<string, size_t> symbols;

  friend class x86_64_Generator;
};
