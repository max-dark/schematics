#pragma once

#include <QGroupBox>
#include <QDial>
#include <QSpinBox>

namespace Schematics::Ui::Widgets
{
class SpeedControl: public QGroupBox
{
    Q_OBJECT
public:
    explicit SpeedControl(const QString &title, QWidget *parent = nullptr);
    ~SpeedControl() override;

    void setSpeed(int speed);
    int speed() const;

    void setRange(int minimum, int maximum);

signals:
    void apply(int speed);

private slots:
    void applyClicked();
private:
    QDial* dial = nullptr;
    QSpinBox* edit = nullptr;
};

} // namespace Schematics::Ui::Widgets
