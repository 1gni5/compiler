#pragma once

#include <vector>
#include <ostream>

class CFG;
#include "BB.h"

using namespace std;

class CFG {
    private:
    vector<BB*> bbs;

    friend class IRVisitor;
};