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
    for(const auto &led_id: leds.keys())
    {
        if (auto s = state.find(led_id); s != state.end())
        {
            auto new_state = s.value();
            auto new_color = selectColor(new_state);
            leds[led_id]->setColor(new_color);
        }
    }
}

void LedList::createLeds(const LedList::LabelMap &labels)
{
    auto owner = widget();
    auto old_box = owner->layout();

    // remove all widgets
    delete old_box;
    leds.clear();

    // create new
    auto box = new QGridLayout;
    auto col = 0;
    auto row = 0;
    auto half = labels.size() / 2 + (labels.size() % 2);
    for (auto id : labels.keys())
    {
        auto label = new QLabel{labels[id]};
        auto led = new Led{};
        led->setColor(selectColor(false));
        box->addWidget(led, row, col);
        box->addWidget(label, row, col + 1);

        ++row;
        if (row == half)
        {
            row = 0; col += 2;
        }
    }
    owner->setLayout(box);
}

LedList::~LedList() = default;
} // namespace Schematics::Ui::Widgets
