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

private:
    EditorPrivate* ui = nullptr;
};

} // namespace Schematics::Editor

#endif //SCHEMATICS_EDITORWINDOW_HXX
