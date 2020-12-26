#include "test_schemecalculator.hxx"
#include <coords/geometry.hxx>

#include <QtTest>

using namespace Coords;
using namespace libschema;

namespace QTest {
    inline bool qCompare(Unit t1, Unit t2, const char *actual, const char *expected,
                    const char *file, int line)
    {
        return qCompare(t1.units(), t2.units(), actual, expected,
                        file, line);
    }
}

Q_DECLARE_METATYPE(libschema::Unit)

TestSchemeCalculator::TestSchemeCalculator(QObject *parent)
    : QObject(parent)
{}

void TestSchemeCalculator::initTestCase()
{
    qRegisterMetaType<libschema::Unit>("libschema::Unit");
    qRegisterMetaType<Unit>("Unit");
}

void TestSchemeCalculator::onlyCentralWithRotate()
{
    Geometry calc;
    Schema schema;

    setSchemaParams(schema,
        200,
        4, 6,
        false // активировать поворотное
    );
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
    Geometry calc;
    Schema schema;

    setSchemaParams(schema,
        200,
        4, 6,
        true // отключить поворотное
    );
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

void TestSchemeCalculator::checkSecondRotator_data()
{
    QTest::addColumn<Unit>("out_width");
    QTest::addColumn<Unit>("out_height");
    QTest::addColumn<bool>("disabled");
    QTest::addColumn<Unit>("inp_width");
    QTest::addColumn<Unit>("inp_height");

    auto width = Unit::from_mm(150);
    auto height = Unit::from_mm(100);

    QTest::newRow("Rotate is Activated") << width << height << false << height << width;
    QTest::newRow("Rotate is Disabled" ) << width << height << true  << width  << height;
}

void TestSchemeCalculator::checkSecondRotator()
{
    QFETCH(Unit, out_width);
    QFETCH(Unit, out_height);
    QFETCH(bool, disabled);
    QFETCH(Unit, inp_width);
    QFETCH(Unit, inp_height);


    Geometry calc;
    Schema schema;

    setSchemaParams(schema,
        200,
        4, 6,
        disabled
    );
    schema.set_dws_board_width(out_height);
    schema.add_dws_board(out_width);

    calc.calculate(&schema);

    QCOMPARE(calc.r2Height(), inp_height);
    QCOMPARE(calc.r2Width() , inp_width );
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

    Geometry calc;
    Schema schema;

    setSchemaParams(schema,
        200,
        4, 6,
        true // отключить поворотное
    );
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

void TestSchemeCalculator::checkPKA350_data()
{
    // params
    QTest::addColumn<Unit>("cb_width");
    QTest::addColumn<Unit>("cb_height");
    QTest::addColumn<bool>("disabled");
    QTest::addColumn<Unit>("pb_width");
    QTest::addColumn<Unit>("pb_height");

    // result
    QTest::addColumn<Unit>("p2_width");
    QTest::addColumn<Unit>("p2_height");
    QTest::addColumn<Unit>("p2_bottom");
    QTest::addColumn<Unit>("p2_top");
    QTest::addColumn<Unit>("p2_roller_pos");

    // ширина центального бруса
    auto cb_width = Unit::from_mm(150);
    // высота центрального бруса
    auto cb_height = Unit::from_mm(120);

    auto b_width = Unit::from_mm(100);
    auto b_height = Unit::from_mm(30);

    QTest::newRow("Rotate is Activated")
            << cb_width << cb_height << false << b_width << b_height
            << Unit::from_mm(192) << Unit::from_mm(150) // размер бруса на входе
            << Unit::from_mm( 25) << Unit::from_mm(125) // позиция фрез
            << Unit::from_mm( 36);                      // позиция ролика

    QTest::newRow("Rotate is Disabled" )
            << cb_width << cb_height << true << b_width << b_height
            << Unit::from_mm(222) << Unit::from_mm(120) // размер бруса на входе
            << Unit::from_mm( 10) << Unit::from_mm(110) // позиция фрез
            << Unit::from_mm( 36);                      // позиция ролика
}

void TestSchemeCalculator::checkPKA350()
{
    QFETCH(Unit, cb_width);
    QFETCH(Unit, cb_height);
    QFETCH(bool, disabled);
    QFETCH(Unit, pb_width);
    QFETCH(Unit, pb_height);

    QFETCH(Unit, p2_width);
    QFETCH(Unit, p2_height);
    QFETCH(Unit, p2_bottom);
    QFETCH(Unit, p2_top);
    QFETCH(Unit, p2_roller_pos);

    Geometry calc;
    Schema schema;

    setSchemaParams(schema,
        200,
        4, 6,
        disabled
    );
    schema.set_dws_board_width(cb_height);
    schema.add_dws_board(cb_width);
    schema.set_pka350_board(pb_width, pb_height);

    calc.calculate(&schema);

    QCOMPARE(calc.p2Width() , p2_width );
    QCOMPARE(calc.p2Height(), p2_height);
    QCOMPARE(calc.p2Bottom(), p2_bottom);
    QCOMPARE(calc.p2Top()   , p2_top   );
    QCOMPARE(calc.p2RollerPos(), p2_roller_pos);
}

void TestSchemeCalculator::checkPA350_data()
{
    QTest::addColumn<Schema*>("schema");

    // result
    QTest::addColumn<Unit>("width");
    QTest::addColumn<Unit>("height");
    QTest::addColumn<Unit>("bottom");
    QTest::addColumn<Unit>("top");
    QTest::addColumn<Unit>("saw_pos");
    QTest::addColumn<Unit>("roller_pos");
    QTest::addColumn<Unit>("r2_width");
    QTest::addColumn<Unit>("r2_height");
    QTest::addColumn<Unit>("p3_width");
    QTest::addColumn<Unit>("dws_width");

    {
        auto schema = new Schema{this};
        setSchemaParams(*schema, 200, 4, 6, false);
        schema->set_dws_board_width(Unit::from_mm(150));
        schema->add_dws_board(Unit::from_mm(60));
        schema->set_pa300_board(Unit::from_mm(120), Unit::from_mm(60));
        schema->set_pka350_board(Unit::from_mm(120), Unit::from_mm(30));

        schema->set_pa350_board(Unit::from_mm(100), Unit::from_mm(30));

        QTest::newRow("Normal mode[with rotate]")
                << schema
                << Unit::from_mm(294) << Unit::from_mm(188)
                << Unit::from_mm( 44) << Unit::from_mm(144)
                << Unit::from_mm( 36) << Unit::from_mm( 72)
                << Unit::from_mm(150) << Unit::from_mm(188)
                << Unit::from_mm(188) << Unit::from_mm( 60);
    }

    {
        auto schema = new Schema{this};
        setSchemaParams(*schema, 200, 4, 6, true);
        schema->set_dws_board_width(Unit::from_mm(150));
        schema->add_dws_board(Unit::from_mm(60));
        schema->set_pa300_board(Unit::from_mm(120), Unit::from_mm(60));
        schema->set_pka350_board(Unit::from_mm(120), Unit::from_mm(30));

        schema->set_pa350_board(Unit::from_mm(100), Unit::from_mm(30));

        QTest::newRow("Vertical mode[without rotate] with P2(PKA350) activated")
                << schema
                << Unit::from_mm(332) << Unit::from_mm(150)
                << Unit::from_mm( 25) << Unit::from_mm(125)
                << Unit::from_mm( 36) << Unit::from_mm( 72)
                << Unit::from_mm(188) << Unit::from_mm(150)
                << Unit::from_mm(188) << Unit::from_mm( 60);
    }

    {
        auto schema = new Schema{this};
        setSchemaParams(*schema, 200, 4, 6, true);
        schema->set_dws_board_width(Unit::from_mm(150));
        schema->add_dws_board(Unit::from_mm(60));
        schema->set_pa300_board(Unit::from_mm(120), Unit::from_mm(60));

        schema->set_pa350_board(Unit::from_mm(100), Unit::from_mm(30));

        QTest::newRow("Vertical mode without P2(PKA350)")
                << schema
                // в этом режиме должны использоваться пилы DWS
                << Unit::from_mm(256) << Unit::from_mm(150)
                << Unit::from_mm( 25) << Unit::from_mm(125)
                << Unit::from_mm(  0) << Unit::from_mm(  0)
                // А так же нужно увеличить ширину роликов P3/DWS
                << Unit::from_mm(256) << Unit::from_mm(150)
                << Unit::from_mm(256) << Unit::from_mm( 60);
    }
}

void TestSchemeCalculator::checkPA350()
{
    QFETCH(Schema*, schema);

    QFETCH(Unit, width);
    QFETCH(Unit, height);
    QFETCH(Unit, bottom);
    QFETCH(Unit, top);
    QFETCH(Unit, saw_pos);
    QFETCH(Unit, roller_pos);
    QFETCH(Unit, r2_width);
    QFETCH(Unit, r2_height);
    QFETCH(Unit, p3_width);
    QFETCH(Unit, dws_width);

    Geometry calc;
    calc.calculate(schema);

    QCOMPARE(calc.p1Width() , width );
    QCOMPARE(calc.p1Height(), height);
    QCOMPARE(calc.p1Bottom(), bottom);
    QCOMPARE(calc.p1Top()   , top   );
    QCOMPARE(calc.p2OuterSaw() , saw_pos);
    QCOMPARE(calc.p2RollerPos(), roller_pos);
    QCOMPARE(calc.r2Width() , r2_width );
    QCOMPARE(calc.r2Height(), r2_height);
    QCOMPARE(calc.p3Width() , p3_width );
    QCOMPARE(calc.dwsWidth(), dws_width);
}

void TestSchemeCalculator::setSchemaParams(
        Schema &schema, millimeter_t diameter,
        millimeter_t dws, millimeter_t pka,
        bool dont_rotate)
{
    schema.set_params(new Params{
        Unit::from_mm(diameter),
        Unit::from_mm(dws), Unit::from_mm(pka),
        dont_rotate
    });
}

TestSchemeCalculator::~TestSchemeCalculator() = default;
