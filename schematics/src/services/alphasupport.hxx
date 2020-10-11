#ifndef SCHEMATICS_ALPHASUPPORT_HXX
#define SCHEMATICS_ALPHASUPPORT_HXX

#include <services/machine.hxx>

namespace Schematics::Service
{

class AlphaSupport: public Machine
{
    Q_OBJECT
public:
    explicit AlphaSupport(QObject* parent = nullptr);
    ~AlphaSupport() override;
};

} // namespace Schematics::Service

#endif //SCHEMATICS_ALPHASUPPORT_HXX
