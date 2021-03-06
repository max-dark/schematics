#ifndef SCHEMATICS_MAINWINDOW_HXX
#define SCHEMATICS_MAINWINDOW_HXX

#include <QMainWindow>

#include <coords/positionid.hxx>
#include <schema/units.hxx>

namespace libschema
{
    class Schema;
}

namespace Schematics::Service {
struct Application;
}

namespace Schematics {

    namespace Ui {
        struct MainView;
    }

    class MainWindow : public QMainWindow {
    Q_OBJECT
    public /* methods */:
        explicit MainWindow(Service::Application* app, QWidget *parent = nullptr);

        ~MainWindow() override;

    signals:
        void needUpdates();
    public slots:
        void updateSuccess();
        void updateSabFailure(const QString& error);
        void updateKdoFailure(const QString& error);
        void toggleDoor(int doorId);
    private slots:
        void on_newScheme();
        void on_loadScheme();
        void on_saveScheme();

        void on_cfgCoords();
        void on_cfgSpeeds();
        void on_cfgDelays();
        void on_initConnection();
        void on_initAxis();

        void on_about();
        void on_aboutQt();

        void schemeParamChanged();

        void calculateScheme();

        void applyCoordById(Coords::PositionId id, libschema::Unit value);
        void applyCoords();

        void centralWidthChanged(double width);
        void addCentralBoards(double height, size_t count);
        void deleteAllCentralBoards();
        void deleteCentralBoardByPos();

        void setAllCentralHeights(double height);
        void setCentralHeightByPos(double height);

        void pa300Changed(bool enabled, double width, double height);
        void pka350Changed(bool enabled, double width, double height);
        void pa350Changed(bool enabled, double width, double height);
    private /* methods */:
        void bindEvents();
        void bindMenus();
        void bindSchemeEditor();
        void bindCoordsTab();

        void updateViews();
        void updateSab(bool is_connected);
        void updateKdo(bool is_connected);

        void applyScheme(const libschema::Schema *new_schema);
    private /* members */:
        Ui::MainView *ui = nullptr;
        libschema::Schema* scheme = nullptr;
        Service::Application* app = nullptr;
    };

} // namespace Schematics

#endif // SCHEMATICS_MAINWINDOW_HXX
