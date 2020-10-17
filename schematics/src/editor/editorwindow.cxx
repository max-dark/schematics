#include "editorwindow.hxx"

#include <schema/schema.hxx>

#include <ui/widgets/scheme/schemeview.hxx>
#include <ui/widgets/scheme/schemeeditor.hxx>

#include <QtWidgets>
#include <schema/xmlreader.hxx>
#include <schema/xmlwriter.hxx>

namespace Schematics::Editor
{

using namespace Schematics::Ui::Widgets;
using namespace libschema;

static constexpr auto scheme_filter = "Cutting scheme (*.cut)";

template<class ObjectT, class MethodT>
static auto bindAction(QAction *action, ObjectT *object, MethodT method)
{
    return QObject::connect(
            action, &QAction::triggered,
            object, method
    );
}

struct EditorPrivate
{
    QAction *scheme_new = nullptr;
    QAction *scheme_load = nullptr;
    QAction *scheme_save = nullptr;

    Schema* schema = nullptr;
    SchemeEditor* editor = nullptr;
    SchemeView* view = nullptr;

    void create(EditorWindow* self);

    void clear() const;

    void loadSchema(QWidget* parent = nullptr);
    void saveSchema(QWidget* parent = nullptr) const;

    void apply(const Schema* new_schema) const;
};

void EditorPrivate::create(EditorWindow *self)
{
    auto params = new Params{};
    schema = new Schema{self};
    schema->set_params(params);

    auto centralView = new QWidget;
    auto box = new QHBoxLayout;


    auto mainMenu = new QMenuBar{};
    // schema
    {
        auto menu = mainMenu->addMenu("Схема");

        scheme_new = menu->addAction("Новая");
        scheme_load = menu->addAction("Загрузить");
        scheme_save = menu->addAction("Сохранить");
    }

    editor = new SchemeEditor{};
    view = new SchemeView{};

    box->addWidget(editor);
    box->addWidget(view);

    centralView->setLayout(box);
    self->setMenuBar(mainMenu);
    self->setCentralWidget(centralView);
}

void EditorPrivate::apply(const Schema *new_schema) const
{
    // clear views
    view->clear();
    editor->clearAll();

    // load params
    auto params = new_schema->params();
    editor->setParams(
            params->diameter().to_mm(),
            params->diameter().to_mm(),
            params->dws_gap().to_mm(),
            params->pka_gap().to_mm(),
            params->is_rot2_disabled()
    );

    // load dws350
    editor->setDWS350(
            new_schema->dws_board_width().to_mm(),
            0.0);
    for (const auto h: new_schema->dws350().boards)
    {
        view->addCentral(h.to_mm());
    }

    // load pa300
    editor->setPA300(
            new_schema->pa300().is_valid(),
            new_schema->pa300().board_width.to_mm(),
            new_schema->pa300().board_height.to_mm());

    // load pka350
    editor->setPKA350(
            new_schema->pka350().is_valid(),
            new_schema->pka350().board_width.to_mm(),
            new_schema->pka350().board_height.to_mm());

    // load pa350
    editor->setPA350(
            new_schema->pa350().is_valid(),
            new_schema->pa350().board_width.to_mm(),
            new_schema->pa350().board_height.to_mm());
}

void EditorPrivate::clear() const
{
    view->clear();
    editor->clearAll();
    schema->remove_all();
}

void EditorPrivate::loadSchema(QWidget *parent)
{
    auto fileName = QFileDialog::getOpenFileName(
            parent, "Загрузка: Выберите файл схемы раскроя", "",
            scheme_filter);
    if (!fileName.isEmpty())
    {
        QFile input{fileName};
        auto ok = input.open(QIODevice::ReadOnly);

        if (ok)
        {
            auto tmp_schema = new Schema{parent};
            tmp_schema->set_params(new libschema::Params{});
            libschema::XmlReader reader;
            auto read_ok = reader.read(tmp_schema, input);
            if (read_ok)
            {
                apply(tmp_schema);

                // set current scheme
                std::swap(schema, tmp_schema);
            }
            else
            {
                QMessageBox::critical(
                        parent,
                        "Ошибка при загрузке файла",
                        reader.errorMessage());
            }

            // cleanup
            tmp_schema->deleteLater();
        }
        else
        {
            QMessageBox::critical(
                    parent,
                    "Ошибка при открытии файла",
                    input.errorString());
        }
    }
}

void EditorPrivate::saveSchema(QWidget *parent) const
{
    auto fileName = QFileDialog::getSaveFileName(
            parent, "Сохранение: Выберите файл схемы раскроя", "",
            scheme_filter);
    if (!fileName.isEmpty())
    {
        QFile output{fileName};
        auto ok = output.open(QIODevice::WriteOnly);
        if (ok)
        {
            XmlWriter writer;
            writer.write(schema, output);
            if (writer.hasError())
            {
                QMessageBox::critical(
                        parent,
                        "Ошибка при записи файла",
                        writer.errorMessage());
            }
        }
        else
        {
            QMessageBox::critical(
                    parent,
                    "Ошибка при открытии файла",
                    output.errorString());
        }
    }
}

EditorWindow::EditorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui{new EditorPrivate}
{
    ui->create(this);
    bindEvents();

    ui->editor->setParams(
            250, 250,
            4.8, 5.6,
            false);
}

EditorWindow::~EditorWindow()
{
    delete ui;
}

void EditorWindow::bindEvents()
{
    using MainWindow = EditorWindow;

    bindAction(ui->scheme_new, this, &MainWindow::on_newScheme);
    bindAction(ui->scheme_load, this, &MainWindow::on_loadScheme);
    bindAction(ui->scheme_save, this, &MainWindow::on_saveScheme);

    connect(ui->editor, &SchemeEditor::schemeParamChanged,
            this, &MainWindow::schemeParamChanged);
    connect(ui->editor, &SchemeEditor::centralWidthChanged,
            this, &MainWindow::centralWidthChanged);
    connect(ui->editor, &SchemeEditor::addCentralBoards,
            this, &MainWindow::addCentralBoards);
    connect(ui->editor, &SchemeEditor::deleteAllCentralBoards,
            this, &MainWindow::deleteAllCentralBoards);
    connect(ui->editor, &SchemeEditor::deleteCentralBoardByPos,
            this, &MainWindow::deleteCentralBoardByPos);

    connect(ui->editor, &SchemeEditor::setAllCentralHeights,
            this, &MainWindow::setAllCentralHeights);
    connect(ui->editor, &SchemeEditor::setCentralHeightByPos,
            this, &MainWindow::setCentralHeightByPos);

    connect(ui->editor, &SchemeEditor::pa300Changed,
            this, &MainWindow::pa300Changed);
    connect(ui->editor, &SchemeEditor::pka350Changed,
            this, &MainWindow::pka350Changed);
    connect(ui->editor, &SchemeEditor::pa350Changed,
            this, &MainWindow::pa350Changed);
}

void EditorWindow::on_newScheme()
{
    ui->clear();
}

void EditorWindow::on_loadScheme()
{
    ui->loadSchema(this);
}

void EditorWindow::on_saveScheme()
{
    ui->saveSchema(this);
}

void EditorWindow::schemeParamChanged()
{
    const auto editor = ui->editor;
    const auto view = ui->view;
    view->setDiameter(editor->minDiam());
    view->setSawSizes(
            editor->dwsSaw(),
            editor->pkaSaw()
    );
    view->setVertical(editor->isVertical());

    auto params = ui->schema->params();

    params->set_diameter(Unit::from_mm(editor->minDiam()));
    params->set_dws_gap(Unit::from_mm(editor->dwsSaw()));
    params->set_pka_gap(Unit::from_mm(editor->pkaSaw()));
    params->set_rot2_mode(editor->isVertical());
}

void EditorWindow::centralWidthChanged(double width)
{
    ui->view->setCentralWidth(width);
    ui->schema->set_dws_board_width(Unit::from_mm(width));
}

void EditorWindow::addCentralBoards(double height, size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        ui->view->addCentral(height);
        ui->schema->add_dws_board(Unit::from_mm(height));
    }
}

void EditorWindow::deleteAllCentralBoards()
{
    auto count = ui->schema->dws350().count().units();
    while (count > 0)
    {
        count -= 1;
        ui->schema->remove_dws_board(count);
        ui->view->removeCentral(count);
    }
}

void EditorWindow::deleteCentralBoardByPos()
{
    bool ok;
    auto count = ui->schema->dws350().count().units();

    if (count > 0)
    {
        auto idx = QInputDialog::getInt(
                this,
                "Введите номер доски",
                "Введите номер доски для удаления (счет идет слева направо)",
                1, 1, count, 1, &ok);
        if (ok)
        {
            idx -= 1;
            ui->schema->remove_dws_board(idx);
            ui->view->removeCentral(idx);
        }
    }
}

void EditorWindow::setAllCentralHeights(double height)
{
    if (height > 0.0)
    {
        auto count = ui->schema->dws350().count().units();
        while (count > 0)
        {
            count -= 1;
            ui->schema->set_dws_board_height(count, Unit::from_mm(height));
            ui->view->setCentralHeight(count, height);
        }
    }
}

void EditorWindow::setCentralHeightByPos(double height)
{
    if (height > 0.0)
    {
        bool ok;
        auto count = ui->schema->dws350().count().units();

        if (count > 0)
        {
            auto idx = QInputDialog::getInt(
                    this,
                    "Введите номер доски",
                    "Введите номер доски для замены (счет идет слева направо)",
                    1, 1, count, 1, &ok);
            if (ok)
            {
                idx -= 1;
                ui->schema->set_dws_board_height(idx, Unit::from_mm(height));
                ui->view->setCentralHeight(idx, height);
            }
        }
    }
}

void EditorWindow::pa300Changed(bool enabled, double width, double height)
{
    ui->view->setPA300Enabled(enabled);
    ui->view->setPA300Size(width, height);

    if (enabled)
    {
        ui->schema->set_pa300_board(Unit::from_mm(width),
                                Unit::from_mm(height));
    }
    else
    {
        ui->schema->remove_pa300_poard();
    }
}

void EditorWindow::pka350Changed(bool enabled, double width, double height)
{
    ui->view->setPKA350Enabled(enabled);
    ui->view->setPKA350Size(width, height);

    if (enabled)
    {
        ui->schema->set_pka350_board(Unit::from_mm(width),
                                 Unit::from_mm(height));
    }
    else
    {
        ui->schema->remove_pka350_poard();
    }
}

void EditorWindow::pa350Changed(bool enabled, double width, double height)
{
    ui->view->setPA350Enabled(enabled);
    ui->view->setPA350Size(width, height);

    if (enabled)
    {
        ui->schema->set_pa350_board(Unit::from_mm(width),
                                Unit::from_mm(height));
    }
    else
    {
        ui->schema->remove_pa350_poard();
    }
}

} // namespace Schematics::Editor
