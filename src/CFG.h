#pragma once

#include <vector>
#include <ostream>

#include "BB.h"

using namespace std;

class CFG {
    private:
    vector<BB*> bbs;

    friend class IRVisitor;
    friend ostream& operator<<(ostream& os, const CFG& cfg);
};