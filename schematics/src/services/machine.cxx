#include "machine.hxx"

#include <s7_micro_client.h>

namespace Schematics::Service
{

struct Machine::Connection: public TSnap7MicroClient
{};

Machine::Machine(QObject *parent)
        : QObject(parent)
        , self{new Connection{}}
{}

Machine::~Machine()
{
    delete self;
}
} // namespace Schematics::Service
