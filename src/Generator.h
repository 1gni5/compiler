# pragma once

#include <ostream>

#include "SymbolTable.h"
#include "IRInstruction.h"
#include "BB.h"

using namespace std;

class Generator
{
    public:
    Generator(ostream& _os) :st(SymbolTable::getInstance()), os(_os) {};
    virtual void generate(BB* start) = 0;

    
    protected:
    SymbolTable& st;
    ostream& os;
};

class x86_64_Generator : public Generator 
{
    public:
    void generate(BB* start);

    private:
    virtual void translate(Store& instr);
    virtual void translate(Ret& instr);
};
