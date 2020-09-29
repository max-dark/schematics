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
private:
    struct Connection;
    Connection* self = nullptr;
};

} // namespace Schematics::Service

#endif //SCHEMATICS_MACHINE_HXX
