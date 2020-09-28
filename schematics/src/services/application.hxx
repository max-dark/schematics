#pragma once

namespace Schematics::Service
{
struct Storage;
struct Application
{
    virtual Storage* storage() = 0;
protected:
    virtual ~Application();
};

}
