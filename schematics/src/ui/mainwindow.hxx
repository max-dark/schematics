#ifndef SCHEMATICS_MAINWINDOW_HXX
#define SCHEMATICS_MAINWINDOW_HXX

#include <QMainWindow>

namespace Schematics {

    namespace Ui {
        struct MainView;
    }

    class MainWindow : public QMainWindow {
    Q_OBJECT
    public /* methods */:
        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow() override;

    private slots:
        void on_newScheme();
        void on_loadScheme();
        void on_saveScheme();
    private /* methods */:
        void bindEvents();
        void bindSchemeEditor();
    private /* members */:
        Ui::MainView *ui = nullptr;
    };

} // namespace Schematics

#endif // SCHEMATICS_MAINWINDOW_HXX
