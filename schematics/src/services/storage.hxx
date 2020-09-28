#pragma once

#include <QString>

namespace Schematics::Service {

struct Storage
{
    virtual bool getConnectionParams(const QString& name,
                                     QString& address,
                                     int& interval) = 0;
protected:
    virtual ~Storage();
};

} // namespace Schematics::Service
