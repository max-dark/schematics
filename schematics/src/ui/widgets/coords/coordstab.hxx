#ifndef SCHEMATICS_COORDSTAB_HXX
#define SCHEMATICS_COORDSTAB_HXX

#include <QWidget>

namespace Schematics::Ui::Widgets
{
    class CoordsTab: public QWidget
    {
        Q_OBJECT
    public /* methods */:
        explicit CoordsTab(QWidget* parent = nullptr);
        ~CoordsTab() override;
    };
} // namespace Schematics::Ui::Widgets

#endif //SCHEMATICS_COORDSTAB_HXX
