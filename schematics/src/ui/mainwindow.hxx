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

        void schemeParamChanged();

        void centralWidthChanged(double width);
        void addCentralBoard(double width, double height);

        void pa300Changed(bool enabled, double width, double height);
        void pka350Changed(bool enabled, double width, double height);
        void pa350Changed(bool enabled, double width, double height);
    private /* methods */:
        void bindEvents();
        void bindMenus();
        void bindSchemeEditor();
    private /* members */:
        Ui::MainView *ui = nullptr;
    };

} // namespace Schematics

#endif // SCHEMATICS_MAINWINDOW_HXX
