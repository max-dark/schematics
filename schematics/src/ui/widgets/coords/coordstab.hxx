#ifndef SCHEMATICS_COORDSTAB_HXX
#define SCHEMATICS_COORDSTAB_HXX

#include <QWidget>

#include <schema/schema.hxx>
#include <schema/units.hxx>

#include <coords/coordinates.hxx>
#include <coords/positionid.hxx>

#include <ui/widgets/numbereditor.hxx>

#include <QVector>
#include <QGraphicsView>

namespace Schematics::Ui::Widgets
{
    class CoordsTab: public QWidget
    {
        Q_OBJECT
    public /* methods */:
        explicit CoordsTab(QWidget* parent = nullptr);
        ~CoordsTab() override;

        /// заменяет изображение предпросмотра
        void setPreview(QGraphicsScene* scene);
        /// возвращает список значений координат
        const Coords::Coordinates &coordinates() const;
    signals:
        /// @brief вызывается когда кликают по кнопке "применить"
        /// @param id coord id
        /// @param value cerent coord value
        void applyCoord(Coords::PositionId id, libschema::Unit value);
    public slots:
        /// @brief заполняет список координат по заданной схеме
        void fillCoords(const libschema::Schema* schema);
    private slots:
    private:
        /// вызывается при изменении координаты в редакторе
        void coordChanged(Coords::PositionId id, libschema::Unit value);
        /// реакция на нажатие кнопки "применить"
        void applyBtnClicked(Coords::PositionId id);
        /// создает интерфейс
        void buildUi();

        /// виджет предпросмотра схемы
        QGraphicsView* preview = nullptr;
        /// список редакторов
        QVector<NumberEditor*> pos_list{Coords::POS_ID_COUNT, nullptr};
        /// список кординат
        Coords::Coordinates coords{};
    };
} // namespace Schematics::Ui::Widgets

#endif //SCHEMATICS_COORDSTAB_HXX
