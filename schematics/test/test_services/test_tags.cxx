#include <QtTest>

// #include <services/tags.hxx>

class TestTags: public QObject
{
    Q_OBJECT
public:
    TestTags() = default;
    ~TestTags() override = default;

private slots:
};

QTEST_MAIN(TestTags)

#include "test_tags.moc"
