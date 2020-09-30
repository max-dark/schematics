#ifndef SCHEMATICS_MACHINE_HXX
#define SCHEMATICS_MACHINE_HXX

#include <QObject>

namespace Schematics::Service
{

class Machine: public QObject
{
    Q_OBJECT
public:
    explicit Machine(QObject* parent = nullptr);
    ~Machine() override;
    bool connect(const QString& address) const
    { return connect(address, 0, 2); }
    bool connect(const QString& address, int rack, int slot) const;
    bool connected() const;
    void disconnect() const;
    bool hasError() const;
    int lastError() const;
    QString errorMessage() const;
private /* types */:
    struct Connection;
private /* methods */:
    Connection* self() const;
private /* members */:
    Connection* selfptr = nullptr;
};

} // namespace Schematics::Service

#endif //SCHEMATICS_MACHINE_HXX
