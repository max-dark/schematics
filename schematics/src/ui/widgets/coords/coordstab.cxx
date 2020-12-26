#include "coordstab.hxx"

#include <coords/coordinates.hxx>

#include <QtWidgets>

#include <ui/tools/tool.hxx>

namespace /* static */ {
/// Индификаторы позиций оборудования в системе координат линии
const char* PositionName[] =
{
    // Зона "ФБС1/

    "ФБС1/ левая фреза",
    //POS_ID_FBS1_LEFT_BLOCK,
    "ФБС1/ правая фреза",
    //POS_ID_FBS1_RIGHT_BLOCK,
    "ФБС1/ 1й прижимной ролик, высота предустановки",
    //POS_ID_FBS1_PRESS1_HEIGHT,
    "ФБС1/ 2й прижимной ролик, высота предустановки",
    //POS_ID_FBS1_PRESS2_HEIGHT,
    "ФБС1/ высота до центра поворотного устройства(ПУ)",
    //POS_ID_FBS1_ROTATOR_HEIGHT,
    "ФБС1/ предустановка ширины роликов ПУ на входе",
    //POS_ID_FBS1_ROTATOR_INP_WIDTH,
    "ФБС1/ предустановка ширины роликов ПУ на выходе",
    //POS_ID_FBS1_ROTATOR_OUT_WIDTH,

    // зона ФБС2

    "ФБС2/ левая фреза",
    //POS_ID_FBS2_LEFT_BLOCK,
    "ФБС2/ правая фреза",
    //POS_ID_FBS2_RIGHT_BLOCK,
    "ФБС2/ 1й ролик выравнивания",
    //POS_ID_FBS2_ALIGN1,
    "ФБС2/ 2й ролик выравнивания",
    //POS_ID_FBS2_ALIGN2,
    "ФБС2/ 3й ролик выравнивания",
    //POS_ID_FBS2_ALIGN3,
    "ФБС2/ 1й прижимной ролик, высота предустановки",
    //POS_ID_FBS2_PRESS1_HEIGHT,
    "ФБС2/ 2й прижимной ролик, высота предустановки",
    //POS_ID_FBS2_PRESS2_HEIGHT,
    "ФБС2/ 3й прижимной ролик, высота предустановки",
    //POS_ID_FBS2_PRESS3_HEIGHT,

    // зона 1го профилятора(PA350)

    "PA350/ положение левого блока",
    //POS_ID_P1_PA350_LEFT_BLOCK,
    "PA350/ положение правого блока",
    //POS_ID_P1_PA350_RIGHT_BLOCK,
    "PA350/ положение фрезы верхняя левая",
    //POS_ID_P1_PA350_TOP_LEFT,
    "PA350/ положение фрезы верхняя правая",
    //POS_ID_P1_PA350_TOP_RIGHT,
    "PA350/ положение фрезы нижняя левая",
    //POS_ID_P1_PA350_BOTTOM_LEFT,
    "PA350/ положение фрезы нижняя правая",
    //POS_ID_P1_PA350_BOTTOM_RIGHT,
    "PA350/ предустановка ширины центрирующих роликов на входе",
    //POS_ID_P1_PA350_ROLLERS_INP_WIDTH,

    // зона 2го профилятора(PKA350)

    "PKA350/ положение левого блока",
    //POS_ID_P2_PKA350_LEFT_BLOCK,
    "PKA350/ положение правого блока",
    //POS_ID_P2_PKA350_RIGHT_BLOCK,
    "PKA350/ положение фрезы верхняя левая",
    //POS_ID_P2_PKA350_TOP_LEFT,
    "PKA350/ положение фрезы верхняя правая",
    //POS_ID_P2_PKA350_TOP_RIGHT,
    "PKA350/ положение фрезы нижняя левая",
    //POS_ID_P2_PKA350_BOTTOM_LEFT,
    "PKA350/ положение фрезы нижняя правая",
    //POS_ID_P2_PKA350_BOTTOM_RIGHT,
    "PKA350/ положние левого внутреннего ролика",
    //POS_ID_P2_PKA350_LEFT_ROLLER,
    "PKA350/ положение правого внутреннего ролика",
    //POS_ID_P2_PKA350_RIGHT_ROLLER,
    "PKA350/ положение левой внешней пилы",
    //POS_ID_P2_PKA350_LEFT_SAW,
    "PKA350/ положение правой внешней пилы",
    //POS_ID_P2_PKA350_RIGHT_SAW,
    "PKA350/ предустановка ширины роликов на входе",
    //POS_ID_P2_PKA350_ROLLERS_INP_WIDTH,
    "PKA350/ предустановка ширины роликов на выходе",
    //POS_ID_P2_PKA350_ROLLERS_OUT_WIDTH,
    "PKA350/ высота центра поворотного устройства",
    //POS_ID_P2_PKA350_ROTATOR_HEIGHT,
    "PKA350/ высота центра левого блока пил",
    //POS_ID_P2_PKA350_LEFT_HEIGHT,
    "PKA350/ высота центра правого блока пил",
    //POS_ID_P2_PKA350_RIGHT_HEIGHT,
    "PKA350/ предустановка высоты прижима отделителя досок",
    //POS_ID_P2_PKA350_PRESS_HEIGHT,

    // зона 3го профилятора(PA300)

    "PA300/ положение левого блока",
    //POS_ID_P3_PA300_LEFT_BLOCK,
    "PA300/ положение правого блока",
    //POS_ID_P3_PA300_RIGHT_BLOCK,
    "PA300/ положение фрезы верхняя левая",
    //POS_ID_P3_PA300_TOP_LEFT,
    "PA300/ положение фрезы верхняя правая",
    //POS_ID_P3_PA300_TOP_RIGHT,
    "PA300/ положение фрезы нижняя левая",
    //POS_ID_P3_PA300_BOTTOM_LEFT,
    "PA300/ положение фрезы нижняя правая",
    //POS_ID_P3_PA300_BOTTOM_RIGHT,
    "PA300/ предустановка ширины центрирующих роликов на входе",
    //POS_ID_P3_PA300_ROLLERS_INP_WIDTH,

    // зона многопила(DWS350)

    "DWS350/ высота 1го прижимного ролика",
    //POS_ID_DWS350_PRESS1_HEIGHT,
    "DWS350/ высота 2го прижимного ролика",
    //POS_ID_DWS350_PRESS2_HEIGHT,
    "DWS350/ высота 3го прижимного ролика",
    //POS_ID_DWS350_PRESS3_HEIGHT,
    "DWS350/ предустановка ширины центрирующих роликов на входе",
    //POS_ID_DWS350_INP_ROLLERS_WIDTH,
    "DWS350/ предустановка ширины центрирующих роликов на выходе",
    //POS_ID_DWS350_OUT_ROLLERS_WIDTH,
    "DWS350/ позиция осей многопила",
    //POS_ID_DWS350_AXIS_HEIGHT,

    "/// количество позиций",
    //POS_ID_COUNT
};
} // namespace static

namespace Schematics::Ui::Widgets
{

    CoordsTab::CoordsTab(QWidget *parent)
        :QWidget(parent)
    {
        buildUi();
    }

    void CoordsTab::setPreview(QGraphicsScene *scene)
    {
        auto oldScene = preview->scene();
        if (scene == oldScene) return;

        if (nullptr != oldScene)
        {
            oldScene->disconnect(this);
        }
        preview->setScene(scene);
        preview->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
        connect(scene, &QGraphicsScene::sceneRectChanged,
                this, &CoordsTab::updatePreviewRect);
    }

    const Coords::Coordinates &CoordsTab::coordinates() const
    {
        return coords;
    }

    void CoordsTab::fillCoords(const libschema::Schema *schema)
    {
        QSignalBlocker block{this};

        coords.fill_from(schema);

        auto p = pos_list.begin();
        for(const auto coord: coords)
        {
            (*p)->setValue(coord.to_mm());
            ++p;
        }
    }

    void CoordsTab::updatePreviewRect(const QRectF &rect)
    {
        preview->fitInView(rect, Qt::KeepAspectRatio);
    }

    void CoordsTab::coordChanged(Coords::PositionId id, libschema::Unit value)
    {
        coords.setById(id, value);
    }

    void CoordsTab::applyBtnClicked(Coords::PositionId id)
    {
        emit applyCoord(id, coords.byId(id));
    }

    void CoordsTab::buildUi()
    {
        auto main = new QHBoxLayout;
        auto coordsView = new QScrollArea{};
        //coordsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        auto coordsBox = new QGridLayout;
        {
            int col = 0;
            int row = 0;
            int id = 0;
            QString prev_group{};
            int group_row = 0;
            int group_col = 0;
            QGridLayout* box = nullptr;
            for (auto& edit : pos_list) {
                Coords::PositionId pos_id = Coords::PositionId(id);
                auto name = QString{PositionName[id]};
                auto groups = name.split("/ ");
                auto group = groups.at(0);
                auto text = groups.at(1);

                if (prev_group != group)
                {
                    auto group_box = new QGroupBox{group};
                    //group_box->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
                    if (box != nullptr)
                    {
                        tool::addGridRow(box, tool::createVSpace());
                    }
                    box = new QGridLayout{};
                    group_box->setLayout(box);
                    coordsBox->addWidget(group_box, group_row, group_col);
                    ++group_row;
                    if (group_row % 2 == 0)
                    {
                        group_row = 0;
                        group_col += 1;
                    }
                    prev_group = group;
                }

                edit = new NumberEditor{};
                edit->setDecimals(2);
                edit->setRange(-1500, 1500);

                connect(edit, qOverload<double>(&NumberEditor::valueChanged), [this, pos_id](double value)
                {
                    coordChanged(pos_id, libschema::Unit::from_mm(value));
                });

                auto num = new QLabel{QString{"№%1"}.arg(id + 1)};
                num->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
                auto lbl = new QLabel{text};
                lbl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
                lbl->setWordWrap(true);

                auto btn = new QPushButton{QString{"Применить"}};
                connect(btn, &QPushButton::clicked, [this, pos_id]{
                    applyBtnClicked(pos_id);
                });


                btn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
                edit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

                box->addWidget( num, row + 0, col + 0/*, 1, 1, Qt::AlignTop*/);
                box->addWidget( lbl, row + 0, col + 1);
                box->addWidget(edit, row + 0, col + 2);
                box->addWidget( btn, row + 0, col + 3);
                row += 1;
                ++id;
            }
            tool::addGridRow(box, tool::createVSpace());
        }

        auto sideBox = new QVBoxLayout;
        {
            preview = new QGraphicsView;
            preview->setFixedSize(400, 400);
            auto calcBtn = new QPushButton{"Рассчитать по схеме"};
            calcBtn->setFixedHeight(50);
            connect(calcBtn, &QPushButton::clicked, this, &CoordsTab::needCaclculate);

            sideBox->addWidget(preview);
            sideBox->addWidget(calcBtn);
            sideBox->addItem(tool::createVSpace());
        }

        auto lst = new QWidget{};
        lst->setLayout(coordsBox);
        coordsView->setWidget(lst);
        main->addWidget(coordsView);
        main->addLayout(sideBox);
        setLayout(main);
    }

    CoordsTab::~CoordsTab() = default;
} // namespace Schematics::Ui::Widgets
