#pragma once

#include <QString>

namespace Schematics::Service
{
struct Storage;
struct Application
{
    virtual Storage* storage() = 0;
    virtual bool getConnectionParams(const QString &name, QString &address, int &interval) = 0;
protected:
    virtual ~Application();
};

}
