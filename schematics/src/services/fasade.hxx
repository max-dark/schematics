#pragma once

#include <QObject>

#include <QString>
#include <QStringList>

namespace Schematics::Service
{

class Fasade : public QObject
{
    Q_OBJECT
public:
    explicit Fasade(QObject *parent = nullptr);
    ~Fasade() override;

    void parseArguments(const QStringList& argv,
                           const QString& defaultPath,
                           const QString &defaultFile);
    QString databaseFile();
signals:
private:
    QString configPath;
    QString configFile;
};

} // namespace Schematics::Service
