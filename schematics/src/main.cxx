#include <QApplication>
#include "ui/mainwindow.hxx"

#include "services/facade.hxx"

#include <QtDebug>

int main(int argc, char** argv)
{
    QApplication app{argc, argv};

    Schematics::Service::Facade svc{};
    // parse args
    svc.parseArguments(app.arguments(),
                          app.applicationDirPath(),
                          "schematics.db");
    // init database service
    svc.startStorage();
    QString addr;
    int interval = 0;
    auto s = svc.storage();
    // connect to main PLC
    {
        auto ok = s->getConnectionParams("sab", addr, interval);
        qDebug() << ok << addr << interval;
    }
    // conncet to secondary PLC
    {
        auto ok = s->getConnectionParams("kdo", addr, interval);
        qDebug() << ok << addr << interval;
    }

    // start UI
    Schematics::MainWindow view{};
    view.show();

    return app.exec();
}
