#pragma once

#include <QString>
#include <QMap>

#include <services/tags.hxx>

namespace Schematics::Service {

struct Boolean
{
    BitAddress address;
    QString description;
};

using BooleanMap = QMap<int, Boolean>;

struct Number
{
    TagAddress address;
    int32_t base = 0;
    int32_t offset = 0;
    QString description;
};

using NumberMap = QMap<int, Number>;

struct Storage
{
    virtual bool getValueByName(const QString &name, QString &value) = 0;
    virtual bool setValueByName(const QString &name, const QString &value) = 0;
protected:
    virtual ~Storage();
};

} // namespace Schematics::Service
