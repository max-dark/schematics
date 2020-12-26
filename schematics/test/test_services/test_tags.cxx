#include <QtTest>

#include <services/tags.hxx>

using Schematics::Service::Tag;
using Schematics::Service::BitAddress;
using Schematics::Service::TagAddress;

#include <string>

class TestTags: public QObject
{
    Q_OBJECT
public:
    TestTags() = default;
    ~TestTags() override = default;
private:
    int to_int(Tag::Area area) const noexcept { return int(area); }
    Tag::Area to_area(int area) const noexcept { return Tag::Area(area); }
private slots:
    void canParseTag_data()
    {
        using namespace QTest;

        addColumn<QString>("input");
        addColumn<bool>("is_valid");
        addColumn<int>("area");
        addColumn<int>("block");
        addColumn<int>("byte");

        newRow("M0:0") << "M0:0" << true << to_int(Tag::Area::MEMORY) << 0 << 0;
        newRow("I0:0") << "I0:0" << true << to_int(Tag::Area::INPUT) << 0 << 0;
        newRow("Q0:0") << "Q0:0" << true << to_int(Tag::Area::OUTPUT) << 0 << 0;
        newRow("D1:0") << "D1:0" << true << to_int(Tag::Area::DATA) << 1 << 0;

        auto tag = TagAddress{Tag::Area::DATA, 4, 2};
        newRow("tag == from_string(to_string(tag))")
                << tag.to_string().c_str() << true
                << to_int(tag.area) << tag.db << tag.byte;
    }
    void canParseTag()
    {
        QFETCH(QString, input);
        QFETCH(bool, is_valid);
        QFETCH(int, area);
        QFETCH(int, block);
        QFETCH(int, byte);

        bool ok;
        auto tag = TagAddress::from_string(input.toStdString(), ok);

        QCOMPARE(ok, is_valid);
        if(is_valid)
        {
            QCOMPARE(tag.area, to_area(area));
            QCOMPARE(tag.db, block);
            QCOMPARE(tag.byte, byte);
        }
    }
    void canParseBit_data()
    {
        using namespace QTest;

        addColumn<QString>("input");
        addColumn<bool>("is_valid");
        addColumn<int>("area");
        addColumn<int>("block");
        addColumn<int>("byte");
        addColumn<int>("bit");

        newRow("M0:0.1") << "M0:0.1" << true << to_int(Tag::Area::MEMORY) << 0 << 0 << 1;
        newRow("I0:0.2") << "I0:0.2" << true << to_int(Tag::Area::INPUT ) << 0 << 0 << 2;
        newRow("Q0:0.3") << "Q0:0.3" << true << to_int(Tag::Area::OUTPUT) << 0 << 0 << 3;
        newRow("D1:0.4") << "D1:0.4" << true << to_int(Tag::Area::DATA  ) << 1 << 0 << 4;

        auto tag = BitAddress{Tag::Area::DATA, 1, 2, 3};
        newRow("tag == from_string(to_string(tag))")
                << tag.to_string().c_str() << true
                << to_int(tag.area) << tag.db << tag.byte << tag.bit;
    }
    void canParseBit()
    {
        QFETCH(QString, input);
        QFETCH(bool, is_valid);
        QFETCH(int, area);
        QFETCH(int, block);
        QFETCH(int, byte);
        QFETCH(int, bit);

        bool ok;
        auto tag = BitAddress::from_string(input.toStdString(), ok);

        QCOMPARE(ok, is_valid);
        if(is_valid)
        {
            QCOMPARE(tag.area, to_area(area));
            QCOMPARE(tag.db  , block);
            QCOMPARE(tag.byte, byte);
            QCOMPARE(tag.bit , bit);
        }
    }
};

QTEST_MAIN(TestTags)

#include "test_tags.moc"
