#include <QApplication>
#include "ui/mainwindow.hxx"

int main(int argc, char** argv)
{
    QApplication app{argc, argv};

    Schematics::MainWindow view{};

    view.show();

    return app.exec();
}
