#include "editorwindow.hxx"

#include <schema/schema.hxx>

#include <ui/widgets/scheme/schemeview.hxx>
#include <ui/widgets/scheme/schemeeditor.hxx>

#include <QtWidgets>
#include <schema/xmlreader.hxx>
#include <schema/xmlwriter.hxx>

#include <ui/tools/tool.hxx>

namespace Schematics::Editor
{

using namespace Schematics::Ui::Widgets;
using namespace Schematics::Ui::tool;
using namespace libschema;

static constexpr auto scheme_filter = "Cutting scheme (*.cut)";

struct EditorPrivate
{
    QAction *schema_new = nullptr;
    QAction *schema_load = nullptr;
    QAction *schema_save = nullptr;

    Schema* schema = nullptr;
    SchemeEditor* editor = nullptr;
    SchemeView* view = nullptr;

    void create(QMainWindow* self);

    void clear() const;

    void loadSchema(QWidget* parent = nullptr);
    void saveSchema(QWidget* parent = nullptr) const;

    void apply(const Schema* new_schema) const;


    void schemeParamChanged() const;

    void centralWidthChanged(double width) const;

    void addCentralBoards(double height, size_t count) const;

    void deleteAllCentralBoards() const;

    void deleteCentralBoardByPos(QWidget* parent = nullptr) const;

    void setAllCentralHeights(double height) const;

    void setCentralHeightByPos(double height, QWidget* parent) const;

    void pa300Changed(bool enabled, double width, double height) const;

    void pka350Changed(bool enabled, double width, double height) const;

    void pa350Changed(bool enabled, double width, double height) const;

};

void EditorPrivate::create(QMainWindow *self)
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

        schema_new = menu->addAction("Новая");
        schema_load = menu->addAction("Загрузить");
        schema_save = menu->addAction("Сохранить");
    }

    editor = new SchemeEditor{false};
    view = new SchemeView{};

    box->addWidget(view);
    box->addWidget(editor);

    centralView->setLayout(box);
    self->setMenuBar(mainMenu);
    self->setCentralWidget(centralView);
    self->setWindowTitle("Schematics Editor");
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

void EditorPrivate::schemeParamChanged() const
{
    view->setDiameter(editor->minDiam());
    view->setSawSizes(
            editor->dwsSaw(),
            editor->pkaSaw()
    );
    view->setVertical(editor->isVertical());

    auto params = schema->params();

    params->set_diameter(Unit::from_mm(editor->minDiam()));
    params->set_dws_gap(Unit::from_mm(editor->dwsSaw()));
    params->set_pka_gap(Unit::from_mm(editor->pkaSaw()));
    params->set_rot2_mode(editor->isVertical());
}

void EditorPrivate::centralWidthChanged(double width) const
{
    view->setCentralWidth(width);
    schema->set_dws_board_width(Unit::from_mm(width));
}

void EditorPrivate::addCentralBoards(double height, size_t count) const
{
    for (size_t i = 0; i < count; ++i)
    {
        view->addCentral(height);
        schema->add_dws_board(Unit::from_mm(height));
    }
}

void EditorPrivate::deleteAllCentralBoards() const
{
    auto count = schema->dws350().count().units();
    while (count > 0)
    {
        count -= 1;
        schema->remove_dws_board(count);
        view->removeCentral(count);
    }
}

void EditorPrivate::deleteCentralBoardByPos(QWidget *parent) const
{
    bool ok;
    auto count = schema->dws350().count().units();

    if (count > 0)
    {
        auto idx = QInputDialog::getInt(
                parent,
                "Введите номер доски",
                "Введите номер доски для удаления (счет идет слева направо)",
                1, 1, count, 1, &ok);
        if (ok)
        {
            idx -= 1;
            schema->remove_dws_board(idx);
            view->removeCentral(idx);
        }
    }
}

void EditorPrivate::setAllCentralHeights(double height) const
{
    if (height > 0.0)
    {
        auto count = schema->dws350().count().units();
        while (count > 0)
        {
            count -= 1;
            schema->set_dws_board_height(count, Unit::from_mm(height));
            view->setCentralHeight(count, height);
        }
    }
}

void EditorPrivate::setCentralHeightByPos(double height, QWidget *parent) const
{
    if (height > 0.0)
    {
        bool ok;
        auto count = schema->dws350().count().units();

        if (count > 0)
        {
            auto idx = QInputDialog::getInt(
                    parent,
                    "Введите номер доски",
                    "Введите номер доски для замены (счет идет слева направо)",
                    1, 1, count, 1, &ok);
            if (ok)
            {
                idx -= 1;
                schema->set_dws_board_height(idx, Unit::from_mm(height));
                view->setCentralHeight(idx, height);
            }
        }
    }
}

void EditorPrivate::pa300Changed(bool enabled, double width, double height) const
{
    view->setPA300Enabled(enabled);
    view->setPA300Size(width, height);

    if (enabled)
    {
        schema->set_pa300_board(Unit::from_mm(width),
                                Unit::from_mm(height));
    }
    else
    {
        schema->remove_pa300_poard();
    }
}

void EditorPrivate::pka350Changed(bool enabled, double width, double height) const
{
    view->setPKA350Enabled(enabled);
    view->setPKA350Size(width, height);

    if (enabled)
    {
        schema->set_pka350_board(Unit::from_mm(width),
                                 Unit::from_mm(height));
    }
    else
    {
        schema->remove_pka350_poard();
    }
}

void EditorPrivate::pa350Changed(bool enabled, double width, double height) const
{
    view->setPA350Enabled(enabled);
    view->setPA350Size(width, height);

    if (enabled)
    {
        schema->set_pa350_board(Unit::from_mm(width),
                                Unit::from_mm(height));
    }
    else
    {
        schema->remove_pa350_poard();
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
    bindAction(ui->schema_new, this, &EditorWindow::on_newScheme);
    bindAction(ui->schema_load, this, &EditorWindow::on_loadScheme);
    bindAction(ui->schema_save, this, &EditorWindow::on_saveScheme);

    connect(ui->editor, &SchemeEditor::schemeParamChanged,
            this, &EditorWindow::schemeParamChanged);
    connect(ui->editor, &SchemeEditor::centralWidthChanged,
            this, &EditorWindow::centralWidthChanged);
    connect(ui->editor, &SchemeEditor::addCentralBoards,
            this, &EditorWindow::addCentralBoards);
    connect(ui->editor, &SchemeEditor::deleteAllCentralBoards,
            this, &EditorWindow::deleteAllCentralBoards);
    connect(ui->editor, &SchemeEditor::deleteCentralBoardByPos,
            this, &EditorWindow::deleteCentralBoardByPos);

    connect(ui->editor, &SchemeEditor::setAllCentralHeights,
            this, &EditorWindow::setAllCentralHeights);
    connect(ui->editor, &SchemeEditor::setCentralHeightByPos,
            this, &EditorWindow::setCentralHeightByPos);

    connect(ui->editor, &SchemeEditor::pa300Changed,
            this, &EditorWindow::pa300Changed);
    connect(ui->editor, &SchemeEditor::pka350Changed,
            this, &EditorWindow::pka350Changed);
    connect(ui->editor, &SchemeEditor::pa350Changed,
            this, &EditorWindow::pa350Changed);
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
    ui->schemeParamChanged();
}

void EditorWindow::centralWidthChanged(double width)
{
    ui->centralWidthChanged(width);
}

void EditorWindow::addCentralBoards(double height, size_t count)
{
    ui->addCentralBoards(height, count);
}

void EditorWindow::deleteAllCentralBoards()
{
    ui->deleteAllCentralBoards();
}

void EditorWindow::deleteCentralBoardByPos()
{
    ui->deleteCentralBoardByPos(this);
}

void EditorWindow::setAllCentralHeights(double height)
{
    ui->setAllCentralHeights(height);
}

void EditorWindow::setCentralHeightByPos(double height)
{
    ui->setCentralHeightByPos(height, this);
}

void EditorWindow::pa300Changed(bool enabled, double width, double height)
{
    ui->pa300Changed(enabled, width, height);
}

void EditorWindow::pka350Changed(bool enabled, double width, double height)
{
    ui->pka350Changed(enabled, width, height);
}

void EditorWindow::pa350Changed(bool enabled, double width, double height)
{
    ui->pa350Changed(enabled, width, height);
}

} // namespace Schematics::Editor
