#pragma once

#include <QString>

namespace Schematics::Service {

struct Storage
{
    virtual bool getValueByName(const QString &name, QString &value) = 0;
    virtual bool setValueByName(const QString &name, const QString &value) = 0;
protected:
    virtual ~Storage();
};

} // namespace Schematics::Service
