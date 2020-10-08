#ifndef SCHEMATICS_LEDLIST_HXX
#define SCHEMATICS_LEDLIST_HXX

#include <QScrollArea>

#include <ui/widgets/leds.hxx>

namespace Schematics::Ui::Widgets
{

class LedList: public QScrollArea
{
    Q_OBJECT
public:
    explicit LedList(QWidget* parent = nullptr);
    ~LedList() override;
};

} // namespace Schematics::Ui::Widgets

#endif //SCHEMATICS_LEDLIST_HXX
