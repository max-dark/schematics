#pragma once

#include <QObject>

namespace Schematics::Service
{

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database() override;
    bool open(const QString& database);
    bool checkStructure();
signals:

};

} // namespace Schematics::Service
