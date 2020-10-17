#include "editorwindow.hxx"

#include <schema/schema.hxx>

#include <ui/widgets/scheme/schemeview.hxx>
#include <ui/widgets/scheme/schemeeditor.hxx>

#include <QtWidgets>

namespace Schematics::Editor
{

using namespace Schematics::Ui::Widgets;
using namespace libschema;

struct EditorPrivate
{
    Schema* schema = nullptr;
    SchemeEditor* editor = nullptr;
    SchemeView* view = nullptr;

    void create(EditorWindow* self);
};

void EditorPrivate::create(EditorWindow *self)
{
    auto centralView = new QWidget;
    auto box = new QHBoxLayout;

    editor = new SchemeEditor{};
    view = new SchemeView{};

    box->addWidget(editor);
    box->addWidget(view);

    centralView->setLayout(box);
    self->setCentralWidget(centralView);
}

EditorWindow::EditorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui{new EditorPrivate}
{
    ui->create(this);
}

EditorWindow::~EditorWindow()
{
    delete ui;
}

} // namespace Schematics::Editor
