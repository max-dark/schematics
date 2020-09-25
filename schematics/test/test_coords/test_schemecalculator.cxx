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

void TestSchemeCalculator::checkPA300_data()
{
    QTest::addColumn<unit_t>("cw");
    QTest::addColumn<unit_t>("ch");
    QTest::addColumn<unit_t>("sw");
    QTest::addColumn<unit_t>("sh");
    QTest::addColumn<unit_t>("rwidth");
    QTest::addColumn<unit_t>("rbottom");
    QTest::addColumn<unit_t>("rtop");

    QTest::addRow("[150x50] + [100x20]")
        << 15000 << 5000 << 10000 << 2000
        << 15200 << 2500 << 12500;
    QTest::addRow("[150x50] + [120x20]")
        << 15000 << 5000 << 12000 << 2000
        << 15200 << 1500 << 13500;

    QTest::addRow("[200x50] + [100x20]")
        << 20000 << 5000 << 10000 << 2000
        << 15200 << 5000 << 15000;
    QTest::addRow("[200x50] + [100x25]")
        << 20000 << 5000 << 10000 << 2500
        << 16200 << 5000 << 15000;
    QTest::addRow("[200x40] + [100x25]")
        << 20000 << 4000 << 10000 << 2500
        << 14200 << 5000 << 15000;
}

void TestSchemeCalculator::checkPA300()
{
    QFETCH(unit_t, cw);
    QFETCH(unit_t, ch);
    QFETCH(unit_t, sw);
    QFETCH(unit_t, sh);
    QFETCH(unit_t, rwidth);
    QFETCH(unit_t, rbottom);
    QFETCH(unit_t, rtop);

    SchemeCalculator calc;
    Schema schema;

    schema.set_params(new Params{
        Unit::from_mm(200),
        Unit::from_mm(4), Unit::from_mm(6),
        true // отключить поворотное
    });
    schema.set_dws_board_width(Unit::from_units(cw));
    schema.add_dws_board(Unit::from_units(ch));
    schema.add_dws_board(Unit::from_units(ch));
    schema.set_pa300_board(Unit::from_units(sw), Unit::from_units(sh));

    calc.calculate(&schema);

    QCOMPARE(calc.p3Height(), calc.dwsHeight());
    QCOMPARE(calc.p3Width().units(), rwidth);
    QCOMPARE(calc.p3Bottom().units(), rbottom);
    QCOMPARE(calc.p3Top().units(), rtop);
}

TestSchemeCalculator::~TestSchemeCalculator() = default;
