#include "leds.hxx"

#include <vector>


static void initResources()
{
    Q_INIT_RESOURCE(ledpix);
}

namespace Schematics::Ui::Widgets
{

QPixmap &Led::getPixmap(Led::Color color)
{
    static std::vector<QPixmap> cache;

    if (cache.empty())
    {
        initResources();
        cache.emplace_back(":/led/gray.png");
        cache.emplace_back(":/led/green.png");
        cache.emplace_back(":/led/yellow.png");
        cache.emplace_back(":/led/orange.png");
        cache.emplace_back(":/led/red.png");
    }

    return cache[color];
}

Led::Led(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(21, 21);
    setAlignment(Qt::AlignCenter);
    applyColor(GRAY);
}

void Led::setColor(Led::Color color)
{
    if (current != color)
    {
        applyColor(color);
    }
}

void Led::applyColor(Led::Color color)
{
    current = color;
    setPixmap(getPixmap(color));
}

Led::~Led() = default;

} // namespace Schematics::Ui::Widgets
