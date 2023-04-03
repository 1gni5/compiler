#pragma once

#include <variant>
#include <string>

#include "IRInstruction.h"

using namespace std;


class IRInstruction {};

class Store : public IRInstruction
{
    public:
    Store(string v, string d) :value(v), destination(d) {};

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