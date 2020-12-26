#ifndef SCHEMATICS_LEDLIST_HXX
#define SCHEMATICS_LEDLIST_HXX

#include <QScrollArea>

#include <ui/widgets/leds.hxx>

#include <map>

namespace Schematics::Ui::Widgets
{

class LedList: public QScrollArea
{
    Q_OBJECT
public:
    using StateMap = std::map<int, bool>;
    using LedMap = std::map<int, Led*>;
    using LabelMap = std::map<int, QString>;
    explicit LedList(QWidget* parent = nullptr);
    ~LedList() override;

    void createLeds(const LabelMap& labels);
    void updateState(const StateMap& state);
    void setColors(Led::Color on, Led::Color off);
private:
    Led::Color selectColor(bool state);
    Led::Color color_on = Led::Color::GREEN;
    Led::Color color_off = Led::Color::GRAY;

    LedMap leds;
};

} // namespace Schematics::Ui::Widgets

#endif //SCHEMATICS_LEDLIST_HXX
