#include <QtTest>

#include <schema/params.hxx>
#include <schema/units.hxx>

using namespace libschema;

class TestParams : public QObject
{
    Q_OBJECT
public:
    TestParams() {}
    virtual ~TestParams() {}
private slots:
    void initTestCase()
    {
        //qRegisterMetaType<Params>();
        //qRegisterMetaType<Params*>();
        qRegisterMetaType<const Params*>("const Params*");
    }

    void set_diameter()
    {
        Params param{};
        QSignalSpy spy(&param, &Params::changed);
        param.set_diameter(Unit::from_mm(100.00));
        QCOMPARE(spy.count(), 1);
        param.set_diameter(Unit::from_mm(100.00));
        QCOMPARE(spy.count(), 1);
    }

    void set_dws_gap()
    {
        Params param{};
        QSignalSpy spy(&param, &Params::changed);
        param.set_dws_gap(Unit::from_mm(100.00));
        QCOMPARE(spy.count(), 1);
        param.set_dws_gap(Unit::from_mm(100.00));
        QCOMPARE(spy.count(), 1);
    }

    void set_pka_gap()
    {
        Params param{};
        QSignalSpy spy(&param, &Params::changed);
        param.set_pka_gap(Unit::from_mm(100.00));
        QCOMPARE(spy.count(), 1);
        param.set_pka_gap(Unit::from_mm(100.00));
        QCOMPARE(spy.count(), 1);
    }

    void set_rot2_mode()
    {
        Params param{};
        QSignalSpy spy(&param, &Params::changed);
        param.set_rot2_mode(true);
        QCOMPARE(spy.count(), 1);
        param.set_rot2_mode(true);
        QCOMPARE(spy.count(), 1);
    }
};

QTEST_MAIN(TestParams)

#include "test_params.moc"
