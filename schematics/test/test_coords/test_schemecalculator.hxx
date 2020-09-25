#pragma once

#include <QObject>

#include <schema/schema.hxx>
#include <schema/units.hxx>

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

    void checkPKA350();

    void checkPA350();
private:
    void setSchemaParams(libschema::Schema& schema,
                         libschema::millimeter_t diameter,
                         libschema::millimeter_t dws,
                         libschema::millimeter_t pka,
                         bool dont_rotate);
};

