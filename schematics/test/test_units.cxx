#include <QtTest>

#include <schema/units.hxx>
#include <utility>

using namespace libschema;

class TestUnits : public QObject
{
    Q_OBJECT
public:
    TestUnits() {}
    virtual ~TestUnits() {}
private slots:
    void from_mm_data()
    {
        QTest::addColumn<millimeter_t>("mm");
        QTest::addColumn<unit_t>("unit");

        millimeter_t mm = 0.0;
        unit_t unit = 0;

        while (unit <= 1000) {
            QTest::addRow("%0.2f mm == %d unit", mm, unit) << mm << unit;
            unit += 10;
            mm += 0.1;
        }
    }
    void from_mm()
    {
        QFETCH(millimeter_t, mm);
        QFETCH(unit_t, unit);

        auto u = Unit::from_mm(mm);
        QCOMPARE(u.units(), unit);
    }

    void to_mm_data()
    {
        QTest::addColumn<millimeter_t>("mm");
        QTest::addColumn<unit_t>("unit");

        millimeter_t mm = 0.0;
        unit_t unit = 0;

        while (unit <= 1000) {
            QTest::addRow("%d unit == %0.2f mm", unit, mm) << mm << unit;
            unit += 10;
            mm += 0.1;
        }
    }

    void to_mm()
    {
        QFETCH(millimeter_t, mm);
        QFETCH(unit_t, unit);

        auto u = Unit::from_units(unit);
        QCOMPARE(u.to_mm(), mm);
    }

    void is_eq()
    {
        unit_t val = -1000;

        while (val <= 1000) {
            auto u0 = Unit::from_units(val);
            auto u1 = Unit::from_units(val);

            QVERIFY(u0 == u1);

            val += 13;
        }
    }

    void is_ne()
    {
        unit_t val = -1000;

        while (val <= 1000) {
            auto u0 = Unit::from_units(val);

            val += 13;

            auto u1 = Unit::from_units(val);

            QVERIFY(u0 != u1);
        }
    }

    void add_data()
    {
        QTest::addColumn<unit_t>("a");
        QTest::addColumn<unit_t>("b");
        QTest::addColumn<unit_t>("c");

        unit_t u = -1000;
        while (u <= 1000) {
            auto a = u;
            auto b = u;
            auto c = u + u;
            QTest::addRow("%d + %d == %d", a, b, c) << a << b << c;
            u += 11;
        }
    }

    void add()
    {
        QFETCH(unit_t, a);
        QFETCH(unit_t, b);
        QFETCH(unit_t, c);

        auto ua = Unit::from_units(a);
        auto ub = Unit::from_units(b);
        auto uc = Unit::from_units(c);

        QCOMPARE(ua + ub, uc);
    }
};

QTEST_MAIN(TestUnits)

#include "test_units.moc"
