#include "ledlist.hxx"

#include <QLayout>

namespace Schematics::Ui::Widgets
{

LedList::LedList(QWidget *parent)
        : QScrollArea(parent)
{
    setWidget(new QWidget);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

Led::Color LedList::selectColor(bool state)
{
    return state /* is on */ ? color_on : color_off;
}

void LedList::setColors(Led::Color on, Led::Color off)
{
    color_on = on;
    color_off = off;
}

void LedList::updateState(const LedList::StateMap& state)
{
    for(const auto &[led_id, led]: leds)
    {
        if (auto s = state.find(led_id); s != state.end())
        {
            auto new_state = s->second;
            auto new_color = selectColor(new_state);
            led->setColor(new_color);
        }
    }
}

void LedList::createLeds(const LedList::LabelMap &labels)
{
    // create new
    auto owner = new QWidget{};
    auto box = new QGridLayout;
    auto col = 0u;
    auto row = 0u;
    auto half = labels.size() / 2 + (labels.size() % 2);
    for (const auto &[id, desc] : labels)
    {
        auto label = new QLabel{desc};
        auto led = new Led{};

        leds[id] = led;
        led->setColor(selectColor(false));
        box->addWidget(led, row, col);
        box->addWidget(label, row, col + 1);

        ++row;
        if (row == half)
        {
            row = 0; col += 2;
        }
    }

    auto old_owner = widget();
    owner->setLayout(box);
    setWidget(owner);
    old_owner->deleteLater();
}

LedList::~LedList() = default;
} // namespace Schematics::Ui::Widgets
