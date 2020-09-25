#pragma once

#include <QObject>

class TestSchemeCalculator : public QObject
{
    Q_OBJECT
public:
    explicit TestSchemeCalculator(QObject *parent = nullptr);
    ~TestSchemeCalculator() override;

private slots:
    void onlyCentralWithRotate();
    void onlyCentralWithoutRotate();


    void checkPA300_data();
    void checkPA300();
};

