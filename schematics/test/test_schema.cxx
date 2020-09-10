#include <QtTest>

#include <schema/schema.hxx>
#include <schema/params.hxx>
#include <schema/units.hxx>

using namespace libschema;

class TestSchema : public QObject
{
    Q_OBJECT
public:
    TestSchema() {}
    virtual ~TestSchema() {}

private slots:
    void set_params()
    {
        Schema schema{};
        auto params = new Params{};

        QSignalSpy spy{&schema, &Schema::schemeChanged};

        schema.set_params(params);
        QVERIFY(schema.params() == params);
        QVERIFY2(&schema == params->parent(), "schema must own params");
        QCOMPARE(spy.count(), 1);
        schema.params()->set_diameter(Unit::from_mm(200.0));
        QCOMPARE(spy.count(), 2);
        params->deleteLater();
    }

    void calc_dws350()
    {
        Schema schema;

        auto params = new Params{
                Unit::from_mm(200.0),
                Unit::from_mm(4.8),
                Unit::from_mm(5.6),
                false
        };

        schema.set_params(params);

        schema.set_dws_board_width(Unit::from_mm(100.0));

        // add 1st board
        schema.add_dws_board(Unit::from_mm(20.0));

        QVERIFY2(schema.dws350().validate(), "DWS350 must be valid");
        {
            auto height = schema.calc_dws350_height();
            auto width = schema.calc_dws350_width();

            QCOMPARE(height, Unit::from_units(10000));
            QCOMPARE(width, Unit::from_units(2000));
        }

        // add 2nd board
        schema.add_dws_board(Unit::from_mm(20.0));

        QVERIFY2(schema.dws350().validate(), "DWS350 must be valid");
        {
            auto height = schema.calc_dws350_height();
            auto width = schema.calc_dws350_width();

            QCOMPARE(height, Unit::from_units(10000));
            QCOMPARE(width, Unit::from_units(4480));
        }
    }
};

QTEST_MAIN(TestSchema)

#include "test_schema.moc"
