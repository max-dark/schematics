#include <QApplication>
#include "ui/mainwindow.hxx"

#include "services/fasade.hxx"

int main(int argc, char** argv)
{
    QApplication app{argc, argv};

    Schematics::Service::Fasade svc{};
    // parse args
    svc.parseArguments(app.arguments(),
                          app.applicationDirPath(),
                          "schematics.db");
    // init database service
    svc.startDatabase();
    // connect to main PLC
    // conncet to secondary PLC

    // start UI
    Schematics::MainWindow view{};
    view.show();

    return app.exec();
}
