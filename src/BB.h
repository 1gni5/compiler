#pragma once

#include <vector>
#include "IRInstruction.h"

using namespace std;

class BB
{
    public:
    vector<IRInstruction> instructions;
    BB* next;

    friend class IRVisitor;
};
