#ifndef SCHEMATICS_ALPHASUPPORT_HXX
#define SCHEMATICS_ALPHASUPPORT_HXX

#include <services/machine.hxx>

#include <map>

namespace Schematics::Service
{

class AlphaSupport: public Machine
{
    Q_OBJECT
public:
    using StateMap = std::map<int, bool>;

    explicit AlphaSupport(QObject* parent = nullptr);
    ~AlphaSupport() override;

    StateMap getMotorsState() const;
    StateMap getAlarmsState() const;
private:
    void initMemoryMap();
};

} // namespace Schematics::Service

#endif //SCHEMATICS_ALPHASUPPORT_HXX
