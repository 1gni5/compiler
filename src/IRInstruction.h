#pragma once

#include <variant>
#include <string>
using namespace std;



class IRInstruction {
    virtual void accept(Generator&) = 0;
};

class Store : public IRInstruction
{
    public:
    Store(string v, string d) :value(v), destination(d) {};
    void accept(Generator& Generator) = {}

    private:
    string value;
    string destination;

    friend class x86_64_Generator;
};

class Ret : public IRInstruction
{
    public:
    Ret(string n) :name(n) {};

    private:
    string name;

    friend class x86_64_Generator;
};

using Instr = variant<Store, Ret>;