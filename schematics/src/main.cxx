#include <QApplication>
#include "ui/mainwindow.hxx"

#include "services/facade.hxx"

#include <QtDebug>

#include <iostream>
struct Printer: public Coords::OffsetVisitor
{
    void visit(const Coords::UnitOffset& o) override {
        std::clog << "Unit " << o.offset;
    }
    void visit(const Coords::DigitOffset& o) override
    {
        std::clog << "Digit " << o.offset << ' ' << o.per_mm;
    }
};

int main(int argc, char** argv)
{
    QApplication app{argc, argv};

    auto svc = new Schematics::Service::Facade{&app};
    // parse args
    svc->parseArguments(app.arguments(),
                          app.applicationDirPath(),
                          "schematics.db");
    // init database service
    svc->startStorage();
    {
        Printer printer;
        auto off = svc->offsets();
        auto l = off->offsets();
        for (auto const &p: l)
        {
            std::clog << p.first << ' ';
            p.second->accept(printer);
            std::clog << std::endl;
        }
        off->clear(l);
    }
    // connect to main PLC
    {
        svc->startSawPlc();
    }
    // connect to secondary PLC
    {
        svc->startKdoPlc();
    }

    // start UI
    Schematics::MainWindow view{};
    view.show();

    return app.exec();
}
