#ifndef SCHEMATICS_EDITORWINDOW_HXX
#define SCHEMATICS_EDITORWINDOW_HXX

#include <QMainWindow>

namespace Schematics::Editor
{

struct EditorPrivate;

class EditorWindow: public QMainWindow
{
    Q_OBJECT
public:
    explicit EditorWindow(QWidget* parent = nullptr);
    ~EditorWindow() override;

private slots:
    void on_newScheme();
    void on_loadScheme();
    void on_saveScheme();

    void schemeParamChanged();

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
private /* members */:
    EditorPrivate* ui = nullptr;
};

} // namespace Schematics::Editor

#endif //SCHEMATICS_EDITORWINDOW_HXX
