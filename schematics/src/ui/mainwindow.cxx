#include "mainwindow.hxx"

#include <QtWidgets>
#include <QtGui>
#include <QtCore>

#include <ui/widgets/scheme/schemeview.hxx>
#include <ui/widgets/scheme/schemeeditor.hxx>

#include <ui/widgets/coords/coordstab.hxx>
#include <ui/widgets/leds.hxx>
#include <ui/widgets/ledlist.hxx>
#include <ui/tools/tool.hxx>

#include <ui/dialogs/tabledialog.hxx>

#include <schema/schema.hxx>
#include <schema/params.hxx>
#include <schema/units.hxx>

#include <schema/xmlwriter.hxx>
#include <schema/xmlreader.hxx>

#include <services/application.hxx>

#include <vector>
#include <algorithm>

using libschema::Unit;

namespace Schematics
{

static const char scheme_filter[]="Cutting scheme (*.cut)";

namespace Ui
{

using LedVector = std::vector<Widgets::Led *>;

struct MainView
{
    QAction *scheme_new = nullptr;
    QAction *scheme_load = nullptr;
    QAction *scheme_save = nullptr;

    QAction *cfg_coords = nullptr;
    QAction *cfg_speeds = nullptr;
    QAction *cfg_delays = nullptr;

    QAction *a_connect = nullptr;
    QAction *a_init = nullptr;

    QAction *help_about = nullptr;
    QAction *help_qt = nullptr;

    Widgets::Led *sab_plc = nullptr;
    Widgets::Led *kdo_plc = nullptr;
    QLabel *sab_status = nullptr;
    Widgets::Led *sab_alarm = nullptr;
    LedVector kdo_leds;

    QTabWidget *tabList = nullptr;

    Widgets::SchemeView *schemeView = nullptr;
    Widgets::SchemeEditor *schemeEditor = nullptr;

    Widgets::CoordsTab *coordsTab = nullptr;

    Widgets::LedList *motorsTab = nullptr;
    Widgets::LedList *alarmsTab = nullptr;
    Widgets::LedList *sensorsTab = nullptr;
    QWidget *doorsTab = nullptr;

    void buildView(QMainWindow *self);

    void setStatusMessage(const QString &message) const
    {
        sab_status->setText(message);
    }

    void createDoors(const Schematics::Service::LabelMap &doors,
                     MainWindow *object)
    {
        auto box = new QGridLayout;

        for (const auto&[id, name]: doors)
        {
            auto label = createLabel(name);
            auto door = new QPushButton{"Открыть/закрыть"};
            QObject::connect(door, &QPushButton::clicked, [doorId = id, view = object]()
            {
                view->toggleDoor(doorId);
            });
            auto row = box->rowCount();
            box->addWidget(label, row, 0);
            box->addWidget(door, row, 1);
            box->addItem(tool::createHSpace(), row, 2);
        }
        box->addItem(tool::createVSpace(), box->rowCount(), 0);
        doorsTab->setLayout(box);
    }

private:
    QLabel *createLabel(const QString &title);

    QWidget *createEditorTab();

    QMenuBar *createMainMenu();

    void createTabs(QLayout *mainBox);
};

QLabel *MainView::createLabel(const QString &title)
{
    auto label = new QLabel{title};
    label->setAlignment(Qt::AlignVCenter);
    return label;
}

void MainView::buildView(QMainWindow *self)
{
    self->setCentralWidget(new QWidget);
    self->setWindowTitle("Schematics PoC Demo");
    auto mainBox = new QVBoxLayout;

    // add top panel
    {
        using namespace Ui::Widgets;
        auto topBox = new QHBoxLayout;

        auto mainGroup = new QGroupBox{"Лесопиление"};
        {
            auto vbox = new QVBoxLayout;
            {
                auto box = new QHBoxLayout;
                sab_plc = new Led{};
                sab_plc->setColor(Led::RED);
                box->addWidget(sab_plc);
                box->addWidget(createLabel("PLC"));

                sab_alarm = new Led{};
                sab_alarm->setColor(Led::RED);
                box->addWidget(sab_alarm);
                box->addWidget(createLabel("Авария"));
                box->addItem(tool::createHSpace());
                vbox->addLayout(box);
            }
            {
                auto box = new QHBoxLayout;
                box->addWidget(createLabel("Состояние: "));
                sab_status = createLabel("need init");
                box->addWidget(sab_status);
                box->addItem(tool::createHSpace());
                vbox->addLayout(box);
            }
            mainGroup->setLayout(vbox);
        }

        topBox->addWidget(mainGroup);
        auto supportGroup = new QGroupBox{"КДО"};
        {
            auto box = new QHBoxLayout;
            kdo_plc = new Led{};
            kdo_plc->setColor(Led::RED);
            box->addWidget(kdo_plc);
            box->addWidget(createLabel("PLC"));

            auto led_names = QStringList{}
                    << "Улица / щепа" << "Улица / опилки"
                    << "ФБС"
                    << "Профиляторы" << "Многопил";
            kdo_leds.reserve(led_names.size());
            for (auto &name : led_names)
            {
                auto led = new Led{};
                box->addWidget(led);
                kdo_leds.push_back(led);
                auto lbl = createLabel(name);
                lbl->setWordWrap(true);
                box->addWidget(lbl);

            }
            supportGroup->setLayout(box);
        }
        topBox->addWidget(supportGroup);
        mainBox->addLayout(topBox);
    }

    // main layout
    {
        auto main = new QHBoxLayout;
        // left tool box
        {
            auto left = new QVBoxLayout;
            auto speeds = new QGroupBox{"Настройка скоростей"};
            {
                auto box = new QGridLayout;
                auto dial = new QDial;
                box->addWidget(dial);
                speeds->setLayout(box);
            }
            left->addWidget(speeds);
            left->addItem(tool::createVSpace());
            //
            main->addLayout(left);
        }
        // add tabs
        {
            createTabs(main);
        }
        mainBox->addLayout(main);
    }

    // add main menu
    {
        auto menuBar = createMainMenu();
        self->setMenuBar(menuBar);
    }

    self->centralWidget()->setLayout(mainBox);
}

void MainView::createTabs(QLayout *mainBox)
{
    tabList = new QTabWidget;
    {
        auto schemeTab = createEditorTab();
        coordsTab = new Widgets::CoordsTab{};
        coordsTab->setPreview(schemeView->scene());

        motorsTab = new Widgets::LedList;
        alarmsTab = new Widgets::LedList;
        sensorsTab = new Widgets::LedList;
        doorsTab = new QWidget;

        // alarm state == On means OK, Off - failure
        alarmsTab->setColors(Widgets::Led::GREEN, Widgets::Led::RED);

        tabList->addTab(schemeTab, QString::fromUtf8("Схема раскроя"));
        tabList->addTab(coordsTab, QString::fromUtf8("Координаты оборудования"));
        tabList->addTab(motorsTab, "Механизмы");
        tabList->addTab(alarmsTab, "Индикаторы аварий");
        tabList->addTab(sensorsTab, "Датчики");
        tabList->addTab(doorsTab, "Сервисные двери");
    }

    mainBox->addWidget(tabList);
}

QWidget *MainView::createEditorTab()
{
    auto schemeTab = new QWidget;
    auto schemeBox = new QHBoxLayout;

    schemeView = new Widgets::SchemeView();

    schemeEditor = new Widgets::SchemeEditor{};


    schemeBox->addWidget(schemeView);
    schemeBox->addWidget(schemeEditor);
    schemeTab->setLayout(schemeBox);
    return schemeTab;
}

QMenuBar *MainView::createMainMenu()
{
    auto mainMenu = new QMenuBar{};
    // schema
    {
        auto menu = mainMenu->addMenu("Схема");

        scheme_new = menu->addAction("Новая");
        scheme_load = menu->addAction("Загрузить");
        scheme_save = menu->addAction("Сохранить");
    }

    // tools
    {
        auto menu = mainMenu->addMenu("Инструменты");

        menu->addSection("Калибровка");
        cfg_coords = menu->addAction("Координаты");
        cfg_speeds = menu->addAction("Скорости");
        cfg_delays = menu->addAction("Интервалы");

        menu->addSection("Линия");
        a_connect = menu->addAction("Подключение");
        a_init = menu->addAction("Инициализация");
    }

    // about
    {
        auto menu = mainMenu->addMenu("Справка");

        help_about = menu->addAction("О программе");
        help_qt = menu->addAction("О Qt");
    }

    return mainMenu;
}

}

template<class ObjectT, class MethodT>
auto bindAction(QAction *action, ObjectT *object, MethodT method)
{
    return QObject::connect(
            action, &QAction::triggered,
            object, method
    );
}

MainWindow::MainWindow(Service::Application *app, QWidget *parent)
        : QMainWindow(parent), ui{new Ui::MainView()}
{
    this->app = app;
    ui->buildView(this);
    setMinimumWidth(1200);

    ui->alarmsTab->createLeds(app->getAlarmLabels());
    ui->motorsTab->createLeds(app->getMotorLabels());
    ui->sensorsTab->createLeds(app->getSensorLabels());
    ui->createDoors(app->getDoorsLabels(), this);
    bindEvents();

    auto params = new libschema::Params{};
    scheme = new libschema::Schema{this};
    scheme->set_params(params);

    ui->schemeEditor->setParams(
            250, 250,
            4.8, 5.6,
            false);

    updateViews();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSuccess()
{
    updateViews();
    emit needUpdates();
}

void MainWindow::updateSabFailure(const QString &error)
{
    updateViews();
    QMessageBox::critical(this, windowTitle(),
                          "Ошибка обмена с PLC линии лесопиления:\n" +
                          error);
}

void MainWindow::updateKdoFailure(const QString &error)
{
    updateViews();
    QMessageBox::critical(this, windowTitle(),
                          "Ошибка обмена с PLC линии КДО:\n" +
                          error);
}

void MainWindow::toggleDoor(int doorId)
{
    auto ok = app->toggleDoorById(doorId);
    if (!ok)
    {
        QMessageBox::critical(this, windowTitle(),
                              "Ошибка обмена с PLC");
    }
}

void MainWindow::bindEvents()
{
    bindMenus();
    bindSchemeEditor();
    bindCoordsTab();
}

void MainWindow::bindMenus()
{
    bindAction(ui->scheme_new, this, &MainWindow::on_newScheme);
    bindAction(ui->scheme_load, this, &MainWindow::on_loadScheme);
    bindAction(ui->scheme_save, this, &MainWindow::on_saveScheme);

    bindAction(ui->cfg_coords, this, &MainWindow::on_cfgCoords);
    bindAction(ui->cfg_speeds, this, &MainWindow::on_cfgSpeeds);
    bindAction(ui->cfg_delays, this, &MainWindow::on_cfgDelays);
    bindAction(ui->a_connect, this, &MainWindow::on_initConnection);
    bindAction(ui->a_init, this, &MainWindow::on_initAxis);

    bindAction(ui->help_about, this, &MainWindow::on_about);
    bindAction(ui->help_qt, this, &MainWindow::on_aboutQt);
}

void MainWindow::bindSchemeEditor()
{
    using Schematics::Ui::Widgets::SchemeEditor;

    connect(ui->schemeEditor, &SchemeEditor::schemeParamChanged,
            this, &MainWindow::schemeParamChanged);
    connect(ui->schemeEditor, &SchemeEditor::centralWidthChanged,
            this, &MainWindow::centralWidthChanged);
    connect(ui->schemeEditor, &SchemeEditor::addCentralBoards,
            this, &MainWindow::addCentralBoards);
    connect(ui->schemeEditor, &SchemeEditor::deleteAllCentralBoards,
            this, &MainWindow::deleteAllCentralBoards);
    connect(ui->schemeEditor, &SchemeEditor::deleteCentralBoardByPos,
            this, &MainWindow::deleteCentralBoardByPos);

    connect(ui->schemeEditor, &SchemeEditor::setAllCentralHeights,
            this, &MainWindow::setAllCentralHeights);
    connect(ui->schemeEditor, &SchemeEditor::setCentralHeightByPos,
            this, &MainWindow::setCentralHeightByPos);

    connect(ui->schemeEditor, &SchemeEditor::pa300Changed,
            this, &MainWindow::pa300Changed);
    connect(ui->schemeEditor, &SchemeEditor::pka350Changed,
            this, &MainWindow::pka350Changed);
    connect(ui->schemeEditor, &SchemeEditor::pa350Changed,
            this, &MainWindow::pa350Changed);

    connect(ui->schemeEditor, &SchemeEditor::calculateSchemeCoords,
            this, &MainWindow::calculateScheme);
    connect(ui->schemeEditor, &SchemeEditor::applySchemeCoords,
            this, &MainWindow::applyCoords);
}

void MainWindow::bindCoordsTab()
{
    using Schematics::Ui::Widgets::CoordsTab;
    connect(ui->coordsTab, &CoordsTab::needCaclculate,
            this, &MainWindow::calculateScheme);
    connect(ui->coordsTab, &CoordsTab::applyCoord,
            this, &MainWindow::applyCoordById);
}

void MainWindow::updateViews()
{
    bool sab, kdo;
    app->getConnectionState(sab, kdo);
    updateSab(sab);
    updateKdo(kdo);

    ui->alarmsTab->updateState(app->getAlarmState());
    ui->motorsTab->updateState(app->getMotorState());
    ui->sensorsTab->updateState(app->getSensorState());
}

void MainWindow::updateSab(bool is_connected)
{
    using namespace Ui::Widgets;
    ui->sab_plc->setColor(is_connected ? Led::GREEN : Led::RED);
    if (is_connected)
    {
        const auto alarms = app->getAlarmState();
        const auto all_ok = std::all_of(
                std::begin(alarms), std::end(alarms), [](const auto &state)
                {
                    return state.second;
                });
        bool is_started, is_done;
        app->getAxisInitState(is_started, is_done);

        ui->sab_alarm->setColor(all_ok ? Led::GREEN : Led::RED);
        if (is_done)
        {
            ui->setStatusMessage("Инициализация завершена");
        }
        else
        {
            if (is_started)
            {
                ui->setStatusMessage("Идет инициализация...");
            }
            else
            {
                ui->setStatusMessage("Необходим запуск инициализации");
            }
        }
    }
    else
    {
        ui->sab_alarm->setColor(Led::RED);
        ui->setStatusMessage("Отсутствует соединение с PLC");
    }
}

void MainWindow::updateKdo(bool is_connected)
{
    using namespace Ui::Widgets;
    ui->kdo_plc->setColor(is_connected ? Led::GREEN : Led::RED);

    const auto motors = app->getSupportMotorsState();
    const auto alarms = app->getSupportAlarmsState();
    auto idx = 0;
    for (const auto led: ui->kdo_leds)
    {
        auto is_running = motors.at(idx);
        auto is_ok = is_connected && alarms.at(idx);
        auto color = Led::RED;
        if (is_ok)
        {
            color = is_running ? Led::GREEN : Led::GRAY;
        }
        led->setColor(color);
        ++idx;
    }
}

void MainWindow::on_newScheme()
{
    ui->schemeView->clear();
    ui->schemeEditor->clearAll();
    scheme->remove_all();
}

void MainWindow::on_loadScheme()
{
    auto fname = QFileDialog::getOpenFileName(
            this, "Загрузка: Выберите файл схемы раскроя", "",
            scheme_filter);
    if (!fname.isEmpty())
    {
        QFile input{fname};
        auto ok = input.open(QIODevice::ReadOnly);

        if (ok)
        {
            auto tmp_schema = new libschema::Schema{this};
            tmp_schema->set_params(new libschema::Params{});
            libschema::XmlReader reader;
            auto read_ok = reader.read(tmp_schema, input);
            if (read_ok)
            {
                applyScheme(tmp_schema);

                // set current scheme
                std::swap(scheme, tmp_schema);
            }
            else
            {
                QMessageBox::critical(
                        this,
                        "Ошибка при загрузке файла",
                        reader.errorMessage());
            }

            // cleanup
            tmp_schema->deleteLater();
        }
        else
        {
            QMessageBox::critical(
                    this,
                    "Ошибка при открытии файла",
                    input.errorString());
        }
    }
}

void MainWindow::applyScheme(const libschema::Schema *new_schema)
{
    // clear views
    ui->schemeView->clear();
    ui->schemeEditor->clearAll();

    // load params
    auto params = new_schema->params();
    ui->schemeEditor->setParams(
            params->diameter().to_mm(),
            params->diameter().to_mm(),
            params->dws_gap().to_mm(),
            params->pka_gap().to_mm(),
            params->is_rot2_disabled()
    );

    // load dws350
    ui->schemeEditor->setDWS350(
            new_schema->dws_board_width().to_mm(),
            0.0);
    for (const auto h: new_schema->dws350().boards)
    {
        ui->schemeView->addCentral(h.to_mm());
    }

    // load pa300
    ui->schemeEditor->setPA300(
            new_schema->pa300().is_valid(),
            new_schema->pa300().board_width.to_mm(),
            new_schema->pa300().board_height.to_mm());

    // load pka350
    ui->schemeEditor->setPKA350(
            new_schema->pka350().is_valid(),
            new_schema->pka350().board_width.to_mm(),
            new_schema->pka350().board_height.to_mm());

    // load pa350
    ui->schemeEditor->setPA350(
            new_schema->pa350().is_valid(),
            new_schema->pa350().board_width.to_mm(),
            new_schema->pa350().board_height.to_mm());
}

void MainWindow::on_saveScheme()
{
    auto fname = QFileDialog::getSaveFileName(
            this, "Сохранение: Выберите файл схемы раскроя", "",
            scheme_filter);
    if (!fname.isEmpty())
    {
        QFile output{fname};
        auto ok = output.open(QIODevice::WriteOnly);
        if (ok)
        {
            libschema::XmlWriter writer;
            writer.write(scheme, output);
            if (writer.hasError())
            {
                QMessageBox::critical(
                        this,
                        "Ошибка при записи файла",
                        writer.errorMessage());
            }
        }
        else
        {
            QMessageBox::critical(
                    this,
                    "Ошибка при открытии файла",
                    output.errorString());
        }
    }
}

void MainWindow::schemeParamChanged()
{
    const auto editor = ui->schemeEditor;
    const auto view = ui->schemeView;
    view->setDiameter(editor->minDiam());
    view->setSawSizes(
            editor->dwsSaw(),
            editor->pkaSaw()
    );
    view->setVertical(editor->isVertical());

    auto params = scheme->params();

    params->set_diameter(Unit::from_mm(editor->minDiam()));
    params->set_dws_gap(Unit::from_mm(editor->dwsSaw()));
    params->set_pka_gap(Unit::from_mm(editor->pkaSaw()));
    params->set_rot2_mode(editor->isVertical());
}

void MainWindow::calculateScheme()
{
    ui->coordsTab->fillCoords(scheme);
}

void MainWindow::applyCoordById(Coords::PositionId id, libschema::Unit value)
{
    auto ok = app->applyCoordById(id, value);
    if (!ok)
    {
        QMessageBox::critical(this, windowTitle(), "Возникла ошибка связи при отправке координат в PLC");
    }
}

void MainWindow::applyCoords()
{
    auto ok = app->applyCoordinates(ui->coordsTab->coordinates());
    if (!ok)
    {
        QMessageBox::critical(this, windowTitle(), "Возникла ошибка связи при отправке координат в PLC");
    }
}

void MainWindow::centralWidthChanged(double width)
{
    ui->schemeView->setCentralWidth(width);
    scheme->set_dws_board_width(Unit::from_mm(width));
}

void MainWindow::addCentralBoards(double height, size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        ui->schemeView->addCentral(height);
        scheme->add_dws_board(Unit::from_mm(height));
    }
}

void MainWindow::deleteAllCentralBoards()
{
    auto count = scheme->dws350().count().units();
    while (count > 0)
    {
        count -= 1;
        scheme->remove_dws_board(count);
        ui->schemeView->removeCentral(count);
    }
}

void MainWindow::deleteCentralBoardByPos()
{
    bool ok;
    auto count = scheme->dws350().count().units();

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
            scheme->remove_dws_board(idx);
            ui->schemeView->removeCentral(idx);
        }
    }
}

void MainWindow::setAllCentralHeights(double height)
{
    if (height > 0.0)
    {
        auto count = scheme->dws350().count().units();
        while (count > 0)
        {
            count -= 1;
            scheme->set_dws_board_height(count, Unit::from_mm(height));
            ui->schemeView->setCentralHeight(count, height);
        }
    }
}

void MainWindow::setCentralHeightByPos(double height)
{
    if (height > 0.0)
    {
        bool ok;
        auto count = scheme->dws350().count().units();

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
                scheme->set_dws_board_height(idx, Unit::from_mm(height));
                ui->schemeView->setCentralHeight(idx, height);
            }
        }
    }
}

void MainWindow::pa300Changed(bool enabled, double width, double height)
{
    ui->schemeView->setPA300Enabled(enabled);
    ui->schemeView->setPA300Size(width, height);

    if (enabled)
    {
        scheme->set_pa300_board(Unit::from_mm(width),
                                Unit::from_mm(height));
    }
    else
    {
        scheme->remove_pa300_poard();
    }
}

void MainWindow::pka350Changed(bool enabled, double width, double height)
{
    ui->schemeView->setPKA350Enabled(enabled);
    ui->schemeView->setPKA350Size(width, height);

    if (enabled)
    {
        scheme->set_pka350_board(Unit::from_mm(width),
                                 Unit::from_mm(height));
    }
    else
    {
        scheme->remove_pka350_poard();
    }
}

void MainWindow::pa350Changed(bool enabled, double width, double height)
{
    ui->schemeView->setPA350Enabled(enabled);
    ui->schemeView->setPA350Size(width, height);

    if (enabled)
    {
        scheme->set_pa350_board(Unit::from_mm(width),
                                Unit::from_mm(height));
    }
    else
    {
        scheme->remove_pa350_poard();
    }
}

void MainWindow::on_cfgCoords()
{
    auto dlg = new Ui::Dialogs::TableDialog{this};
    dlg->setWindowTitle("Настройка координатной системы");
    dlg->setData(app->coordsTable());
    dlg->open();
}

void MainWindow::on_cfgSpeeds()
{
    auto dlg = new Ui::Dialogs::TableDialog{this};
    dlg->setWindowTitle("Настройка скоростей двигателей");
    dlg->setData(app->speedsTable());
    dlg->open();
}

void MainWindow::on_cfgDelays()
{
    auto dlg = new Ui::Dialogs::TableDialog{this};
    dlg->setWindowTitle("Настройка интервалов");
    dlg->setData(app->delaysTable());
    dlg->open();
}

void MainWindow::on_initConnection()
{
    auto run = QMessageBox::question(this, windowTitle(),
                                     "Сбросить состояние подключения?");
    if (run == QMessageBox::Yes)
    {
        bool sab_ok, kdo_ok;
        app->getConnectionState(sab_ok, kdo_ok);
        app->resetConnection();
        auto need_updates = (!sab_ok) || (!kdo_ok);
        if (need_updates)
        {
            emit needUpdates();
        }
    }
}

void MainWindow::on_initAxis()
{
    auto run = QMessageBox::question(this, windowTitle(),
                                     "Запустить инициализацию?");
    if (run == QMessageBox::Yes)
    {
        auto ok = app->startAxisInit();
        if (ok)
        {
            QMessageBox::information(this, windowTitle(),
                                     "Инициализация линии запущена");
        }
        else
        {
            QMessageBox::critical(this, windowTitle(),
                                  "Ошибка запуска инициализации линии");
        }
    }
}

void MainWindow::on_about()
{
    QMessageBox::about(this, windowTitle(),
                       "Разработано:\n"
                       "Max Dark <maxim.dark@gmail.com>\n"
                       "Репозиторий проекта:\n"
                       "https://gitlab.com/andreevo-ldk/alpha/schematics");
}

void MainWindow::on_aboutQt()
{
    QMessageBox::aboutQt(this);
}

} // namespace Schematics
