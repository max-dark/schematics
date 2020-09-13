#include "mainwindow.hxx"

#include <QLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGroupBox>
#include <QGraphicsEllipseItem>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QDebug>

#include <ui/widgets/scheme/schemeview.hxx>
#include <ui/widgets/scheme/schemeeditor.hxx>

namespace Schematics {

    namespace Ui {

        struct MainView {
            QLabel *lbl_status = nullptr;
            Widgets::SchemeView *schemeView = nullptr;
            Widgets::SchemeEditor* schemeEditor = nullptr;

            void buildView(QMainWindow *self);

        private:
            QWidget *createEditorTab();

        };

        void MainView::buildView(QMainWindow *self) {
            self->setCentralWidget(new QWidget);
            self->setWindowTitle("Schematics PoC Demo");
            auto mainBox = new QVBoxLayout;

            // add top panel
            {
                auto topBox = new QHBoxLayout;

                lbl_status = new QLabel{QString::fromUtf8(u8"test")};
                topBox->addWidget(lbl_status);
                mainBox->addLayout(topBox);
            }
            // add tabs
            {
                auto tabList = new QTabWidget;
                {
                    QWidget *schemeTab = createEditorTab();

                    tabList->addTab(schemeTab, QString::fromUtf8(u8"Схема раскроя"));
                }

                mainBox->addWidget(tabList);
            }
            self->centralWidget()->setLayout(mainBox);
        }

        QWidget *MainView::createEditorTab() {
            auto schemeTab = new QWidget;
            auto schemeBox = new QHBoxLayout;

            schemeView = new Widgets::SchemeView();

            schemeEditor = new Widgets::SchemeEditor{};


            schemeBox->addWidget(schemeView);
            schemeBox->addWidget(schemeEditor);
            schemeTab->setLayout(schemeBox);
            return schemeTab;
        }

    }

    MainWindow::MainWindow(QWidget *parent)
            : QMainWindow(parent), ui{new Ui::MainView()} {
        ui->buildView(this);
        setWindowState(Qt::WindowMaximized);
        bindEvents();
    }

    MainWindow::~MainWindow() {
        delete ui;
    }

    void MainWindow::bindEvents() {
        bindSchemeEditor();
    }

    void MainWindow::bindSchemeEditor() {
        using Schematics::Ui::Widgets::SchemeEditor;
        connect(ui->schemeEditor, &SchemeEditor::newScheme,
                this, &MainWindow::on_newScheme);
        connect(ui->schemeEditor, &SchemeEditor::loadScheme,
                this, &MainWindow::on_loadScheme);
        connect(ui->schemeEditor, &SchemeEditor::saveScheme,
                this, &MainWindow::on_saveScheme);
    }

    void MainWindow::on_newScheme() {

    }

    void MainWindow::on_loadScheme() {

    }

    void MainWindow::on_saveScheme() {

    }

} // namespace Schematics
