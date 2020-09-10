#ifndef SCHEMATICS_MAINWINDOW_HXX
#define SCHEMATICS_MAINWINDOW_HXX

#include <QMainWindow>

namespace Schematics {

    namespace Ui {
        struct MainView;
    }

    class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow() override;

    private:
        Ui::MainView *ui = nullptr;
    };

} // namespace Schematics

#endif // SCHEMATICS_MAINWINDOW_HXX
