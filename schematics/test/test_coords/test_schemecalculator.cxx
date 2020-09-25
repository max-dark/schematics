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
        false // активировать поворотное
    });
    schema.set_dws_board_width(Unit::from_mm(150));
    schema.add_dws_board(Unit::from_mm(50));
    schema.add_dws_board(Unit::from_mm(50));

    calc.calculate(&schema);

    // ширина центральной доски
    QCOMPARE(calc.dwsHeight(), Unit::from_units(15000));
    // сумма толщины досок и пропила
    QCOMPARE(calc.dwsWidth(), Unit::from_units(10400));
    // всегда 0
    QCOMPARE(calc.fbs2Align().units(), 0);
    // поворотное задействовано
    QCOMPARE(calc.fbs2Height(), calc.dwsWidth());
    QCOMPARE(calc.fbs2Width(), calc.dwsHeight());

    QCOMPARE(calc.fbs1Height(), Unit::from_units(20000));
    // ширина 1го == высота для 2го
    QCOMPARE(calc.fbs1Width(), calc.fbs2Height());
}

void TestSchemeCalculator::onlyCentralWithoutRotate()
{
    SchemeCalculator calc;
    Schema schema;

    schema.set_params(new Params{
        Unit::from_mm(200),
        Unit::from_mm(4), Unit::from_mm(6),
        true // отключить поворотное
    });
    schema.set_dws_board_width(Unit::from_mm(150));
    schema.add_dws_board(Unit::from_mm(50));
    schema.add_dws_board(Unit::from_mm(50));

    calc.calculate(&schema);

    QCOMPARE(calc.dwsHeight(), Unit::from_units(15000));
    QCOMPARE(calc.dwsWidth(), Unit::from_units(10400));
    QCOMPARE(calc.fbs2Align().units(), 0);
    // поворотное отключено и брус идет напрямую
    QCOMPARE(calc.fbs2Height(), calc.dwsHeight());
    QCOMPARE(calc.fbs2Width(), calc.dwsWidth());
    QCOMPARE(calc.fbs1Height(), Unit::from_units(20000));
    QCOMPARE(calc.fbs1Width(), calc.fbs2Height());
}

TestSchemeCalculator::~TestSchemeCalculator() = default;
