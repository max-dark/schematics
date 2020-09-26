#pragma once

#include <schema/schema.hxx>

#include <coords/positionid.hxx>

namespace Coords
{
    using libschema::Schema;
    using libschema::Unit;
    /**
     * @brief The SchemeCalculator class
     *  рассчитывает геометрию схемы
     */
    class SchemeCalculator
    {
    public:
        /// Рассчитывает геометрию да заданной схемы
        /// Рассчет ведется от конца линии к началу
        void calculate(const Schema* schema);

        /// Ширина лафета. Определяет позицию фрез ФБС1
        Unit fbs1Width() const;
        /// определяет позицию прижимных роликов и ПУ1
        Unit fbs1Height() const;

        /// позиция центра
        Unit fbs2Align() const;
        /// Ширина бруса на выходе ФБС2
        Unit fbs2Width() const;
        /// Высота бруса
        Unit fbs2Height() const;

        /// ширина бруса на входе в 1й профилятор
        /// определяет позицию предустановки
        /// центрирующих роликов зон 1й/2й профиляторов
        Unit p1Width() const;
        /// высота бруса на входе в П1
        Unit p1Height() const;
        /// положение нижних фрез П1
        Unit p1Bottom() const;
        /// положение верхних фрез П1
        Unit p1Top() const;

        /// ширина бруса на входе в ПУ2
        Unit r2Width() const;
        /// высота бруса на входе в ПУ2
        Unit r2Height() const;

        /// ширина с учетом внутренней доски
        /// определяет позицию блоков **П1**
        Unit p2Width() const;
        /// высота бруса на входе
        Unit p2Height() const;
        /// положение нижних фрез
        Unit p2Bottom() const;
        /// положение верхних фрез
        Unit p2Top() const;
        /// положение внешней пилы относительно внутренней
        Unit p2OuterSaw() const;
        /// положение роликов относительно внутренней пилы
        Unit p2RollerPos() const;

        /// ширина бруса на входе в П3
        /// отпределяет предустановку центрирующих роликов
        /// зоны П3 / многопил
        Unit p3Width() const;
        /// высота бруса на входе в П3
        Unit p3Height() const;
        /// положение нижних фрез
        Unit p3Bottom() const;
        /// положение верхних фрез
        Unit p3Top() const;

        /// ширина бруса из центральных досок
        /// определяет позицию П3
        Unit dwsWidth() const;
        /// высота бруса
        /// определяет предустановку прижимных роликов
        Unit dwsHeight() const;

    private:
        // f1
        Unit fbs1_width;
        Unit fbs1_height;
        // f2
        Unit fbs2_width;
        Unit fbs2_height;
        Unit fbs2_align;
        // p1
        Unit p1_width;
        Unit p1_height;
        Unit p1_bottom;
        Unit p1_top;
        // p2
        Unit p2_width;
        Unit p2_height;
        Unit p2_bottom;
        Unit p2_top;
        Unit p2_outer_saw;
        Unit p2_roller_pos;
        // p2 / rotator
        Unit r2_width;
        Unit r2_height;
        // p3
        Unit p3_width;
        Unit p3_height;
        Unit p3_bottom;
        Unit p3_top;
        // dws
        Unit dws_height;
        Unit dws_width;
    };
} // namespace CoordSystem
