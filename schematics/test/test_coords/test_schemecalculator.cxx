#include "test_schemecalculator.hxx"
#include <coords/coords.hxx>

#include <QtTest>

using namespace Coords;
using namespace libschema;

TestSchemeCalculator::TestSchemeCalculator(QObject *parent)
    : QObject(parent)
{}

void TestSchemeCalculator::onlyCentralWithRotate()
{
    SchemeCalculator calc;
    Schema schema;

    schema.set_params(new Params{
        Unit::from_mm(200),
        Unit::from_mm(4), Unit::from_mm(6),
        false
    });
    schema.set_dws_board_width(Unit::from_mm(150));
    schema.add_dws_board(Unit::from_mm(50));
    schema.add_dws_board(Unit::from_mm(50));

    calc.calculate(&schema);

    QCOMPARE(calc.dwsHeight(), Unit::from_units(15000));
    QCOMPARE(calc.dwsWidth(), Unit::from_units(10400));
    QCOMPARE(calc.fbs2Align().units(), 0);
    QCOMPARE(calc.fbs1Height(), Unit::from_units(20000));
    QCOMPARE(calc.fbs1Width(), Unit::from_units(15000));
}

void TestSchemeCalculator::onlyCentralWithoutRotate()
{
    SchemeCalculator calc;
    Schema schema;

    schema.set_params(new Params{
        Unit::from_mm(200),
        Unit::from_mm(4), Unit::from_mm(6),
        true
    });

    calc.calculate(&schema);

    QCOMPARE(calc.dwsHeight(), Unit::from_units(15000));
    QCOMPARE(calc.dwsWidth(), Unit::from_units(10400));
    QCOMPARE(calc.fbs2Align().units(), 0);
    QCOMPARE(calc.fbs1Height(), Unit::from_units(20000));
    QCOMPARE(calc.fbs1Width(), Unit::from_units(10400));
}

TestSchemeCalculator::~TestSchemeCalculator() = default;
