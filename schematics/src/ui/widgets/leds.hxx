#ifndef SCHEMATICS_LEDS_HXX
#define SCHEMATICS_LEDS_HXX

#include <QLabel>

namespace Schematics::Ui::Widgets
{

class Led : public QLabel
{
    Q_OBJECT
public:
    enum Color
    {
        GRAY, GREEN, YELLOW, ORANGE, RED
    };

    explicit Led(QWidget* parent = nullptr);
    virtual ~Led();

    void setColor(Color color);

private:
    static QPixmap &getPixmap(Color color);
    void applyColor(Color color);
    Color current = RED;
};

} // namespace Schematics::Ui::Widgets

#endif //SCHEMATICS_LEDS_HXX
