#include <QApplication>
#include "ui/mainwindow.hxx"

#include "services/facade.hxx"

#include <QtDebug>

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
    // connect to main PLC
    {
        svc->startSabPlc();
    }
    // connect to secondary PLC
    {
        svc->startKdoPlc();
    }

    // start UI
    Schematics::MainWindow view{svc};
    {
        using Schematics::Service::Facade;
        using Schematics::MainWindow;

        // prepare update cycle

        QObject::connect(svc, &Facade::updateSuccess, &view, &MainWindow::updateSuccess);
        QObject::connect(svc, &Facade::updateKdoFailure, &view, &MainWindow::updateKdoFailure);
        QObject::connect(svc, &Facade::updateSabFailure, &view, &MainWindow::updateSabFailure);

        QObject::connect(&view, &MainWindow::needUpdates, svc, &Facade::requestStateUpdate);

        svc->startUpdateCycle();
    }
    view.show();

    return app.exec();
}
