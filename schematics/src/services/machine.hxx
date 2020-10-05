#ifndef SCHEMATICS_MACHINE_HXX
#define SCHEMATICS_MACHINE_HXX

#include <QObject>

#include <services/tags.hxx>

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

    bool readTag(Tag& tag) const;
    bool writeTag(const Tag& tag) const;

    bool readBlock(const TagAddress& address, std::size_t size, void* block) const;

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
