#pragma once

#include <QString>

#include <coords/offset.hxx>

namespace Schematics::Service
{
struct Storage;
struct Application
{
    virtual Storage* storage() = 0;
    virtual Coords::OffsetRepository* offsets() = 0;
    virtual bool getConnectionParams(const QString &name, QString &address, int &interval) = 0;
protected:
    virtual ~Application();
};

}
