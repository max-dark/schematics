#include "speedcontrol.hxx"

#include <QVBoxLayout>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSizePolicy>

namespace Schematics::Ui::Widgets
{

SpeedControl::SpeedControl(const QString &title, QWidget *parent)
    : QGroupBox(title, parent)
{
    auto box = new QVBoxLayout;
    QSizePolicy policy{QSizePolicy::Minimum, QSizePolicy::Fixed};
    QSizePolicy policyX2{QSizePolicy::Maximum, QSizePolicy::Maximum};

    setSizePolicy(policyX2);
    setMaximumWidth(120);

    dial = new QDial;
    {
        dial->setNotchesVisible(true);
        dial->setRange(0, 90);
        dial->setSizePolicy(policyX2);
    }

    edit = new QSpinBox;
    {
        edit->setRange(0, 90);
        edit->setAlignment(Qt::AlignRight);
        edit->setSizePolicy(policy);
    }

    auto btn = new QPushButton{"Применить"};
    btn->setSizePolicy(policy);

    box->addWidget(dial);
    box->addWidget(edit);

    connect(dial, &QDial::valueChanged,
            edit, &QSpinBox::setValue);
    connect(edit, qOverload<int>(&QSpinBox::valueChanged),
            dial, &QDial::setValue);

    box->addWidget(btn);

    connect(btn, &QPushButton::clicked,
            this, &SpeedControl::applyClicked);

    setLayout(box);
}

SpeedControl::~SpeedControl()
{}

void SpeedControl::setSpeed(int speed)
{
    QSignalBlocker block{this};

    dial->setValue(speed);
    edit->setValue(speed);
}

int SpeedControl::speed() const
{
    return edit->value();
}

void SpeedControl::setRange(int minimum, int maximum)
{
    QSignalBlocker block{this};

    dial->setRange(minimum, maximum);
    edit->setRange(minimum, maximum);
}

void SpeedControl::applyClicked()
{
    emit apply(speed());
}

} // namespace Schematics::Ui::Widgets
