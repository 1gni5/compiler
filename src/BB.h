#pragma once

#include <vector>

class BB;
#include "IRInstruction.h"

using namespace std;


class BB
{
    public:
    vector<IRInstruction> instructions;
    BB* next;

    friend class IRVisitor;
};
