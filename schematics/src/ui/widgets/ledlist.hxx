#ifndef SCHEMATICS_LEDLIST_HXX
#define SCHEMATICS_LEDLIST_HXX

#include <QScrollArea>

#include <ui/widgets/leds.hxx>

#include <QMap>

namespace Schematics::Ui::Widgets
{

class LedList: public QScrollArea
{
    Q_OBJECT
public:
    using StateMap = QMap<int, bool>;
    using LedMap = QMap<int, Led*>;
    using LabelMap = QMap<int, QString>;
    explicit LedList(QWidget* parent = nullptr);
    ~LedList() override;

    void createLeds(const LabelMap& labels);
    void updateState(const StateMap& state);
    void setColors(Led::Color on, Led::Color off);
private:
    Led::Color selectColor(bool state);
    Led::Color color_on = Led::Color::GREEN;
    Led::Color color_off = Led::Color::RED;

    LedMap leds;
};

} // namespace Schematics::Ui::Widgets

#endif //SCHEMATICS_LEDLIST_HXX
