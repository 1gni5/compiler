#include <queue>
#include <variant>
#include "Generator.h"
#include "BB.h"

using namespace std;

void x86_64_Generator::generate(BB* start)
{
    // Insert prolog before entering the graph
    os  << ".globl main\n"
        << "main:\n"
        << "\tpushq\t%rbp\n"
        << "\tmovq\t%rsp, %rbp\n"
        << endl;

    queue<BB*> waiting;
    waiting.push(start);

    while (!waiting.empty())
    {
        BB* current = waiting.front();

        for (auto& instr : current->instructions)
        {
            
        }

        waiting.push(current->next);
        waiting.pop();
    }
    
    // If no more blocks are available, add epilog
    os  << "\tpopq\t%rbp\n"
        << "\tret\n"
        << endl;
}

void x86_64_Generator::translate(Store& intr)
{
    size_t offset = st[intr.destination];
    os  << "\tmovl\t$" << intr.value << ", -" << offset << "(%rbp)" << endl;
}

void x86_64_Generator::translate(Ret& intr)
{
    size_t offset = st[intr.name];
    os << "\tmovl\t-" << offset << "(%rbp), %eax" << endl;
}
