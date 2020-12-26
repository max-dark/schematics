#include <QApplication>

#include <editor/editorwindow.hxx>

int main(int argc, char** argv)
{
    QApplication app{argc, argv};

    Schematics::Editor::EditorWindow view{};

    view.show();

    return app.exec();
}
