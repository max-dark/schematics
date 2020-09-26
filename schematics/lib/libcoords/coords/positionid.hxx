#pragma once


namespace Coords
{
/// Индификаторы позиций оборудования в системе координат линии
enum PositionId
{
    // Зона ФБС1

    /// левая фреза
    POS_ID_FBS1_LEFT_BLOCK,
    /// правая фреза
    POS_ID_FBS1_RIGHT_BLOCK,
    /// 1й прижимной ролик, высота предустановки
    POS_ID_FBS1_PRESS1_HEIGHT,
    /// 2й прижимной ролик, высота предустановки
    POS_ID_FBS1_PRESS2_HEIGHT,
    /// высота до центра поворотного устройства(ПУ)
    POS_ID_FBS1_ROTATOR_HEIGHT,
    /// предустановка ширины роликов ПУ на входе
    POS_ID_FBS1_ROTATOR_INP_WIDTH,
    /// предустановка ширины роликов ПУ на выходе
    POS_ID_FBS1_ROTATOR_OUT_WIDTH,

    // зона ФБС2

    /// левая фреза
    POS_ID_FBS2_LEFT_BLOCK,
    /// правая фреза
    POS_ID_FBS2_RIGHT_BLOCK,
    /// 1й ролик выравнивания
    POS_ID_FBS2_ALIGN1,
    /// 2й ролик выравнивания
    POS_ID_FBS2_ALIGN2,
    /// 3й ролик выравнивания
    POS_ID_FBS2_ALIGN3,
    // /// положение приемных роликов
    //TODO: POS_ID_FBS2_INP_ALIGN,
    /// 1й прижимной ролик, высота предустановки
    POS_ID_FBS2_PRESS1_HEIGHT,
    /// 2й прижимной ролик, высота предустановки
    POS_ID_FBS2_PRESS2_HEIGHT,
    /// 3й прижимной ролик, высота предустановки
    POS_ID_FBS2_PRESS3_HEIGHT,

    // зона 1го профилятора(PA350)

    /// положение левого блока
    POS_ID_P1_PA350_LEFT_BLOCK,
    /// положение правого блока
    POS_ID_P1_PA350_RIGHT_BLOCK,
    /// положение фрезы верхняя левая
    POS_ID_P1_PA350_TOP_LEFT,
    /// положение фрезы верхняя правая
    POS_ID_P1_PA350_TOP_RIGHT,
    /// положение фрезы нижняя левая
    POS_ID_P1_PA350_BOTTOM_LEFT,
    /// положение фрезы нижняя правая
    POS_ID_P1_PA350_BOTTOM_RIGHT,
    /// предустановка ширины центрирующих роликов на входе
    POS_ID_P1_PA350_ROLLERS_INP_WIDTH,

    // зона 2го профилятора(PKA300)

    /// положение левого блока
    POS_ID_P2_PKA350_LEFT_BLOCK,
    /// положение правого блока
    POS_ID_P2_PKA350_RIGHT_BLOCK,
    /// положение фрезы верхняя левая
    POS_ID_P2_PKA350_TOP_LEFT,
    /// положение фрезы верхняя правая
    POS_ID_P2_PKA350_TOP_RIGHT,
    /// положение фрезы нижняя левая
    POS_ID_P2_PKA350_BOTTOM_LEFT,
    /// положение фрезы нижняя правая
    POS_ID_P2_PKA350_BOTTOM_RIGHT,
    /// положние левого внутреннего ролика
    POS_ID_P2_PKA350_LEFT_ROLLER,
    /// положение правого внутреннего ролика
    POS_ID_P2_PKA350_RIGHT_ROLLER,
    /// положение левой внешней пилы
    POS_ID_P2_PKA350_LEFT_SAW,
    /// положение правой внешней пилы
    POS_ID_P2_PKA350_RIGHT_SAW,
    /// предустановка ширины роликов на входе
    POS_ID_P2_PKA350_ROLLERS_INP_WIDTH,
    /// предустановка ширины роликов на выходе
    POS_ID_P2_PKA350_ROLLERS_OUT_WIDTH,
    /// высота центра поворотного устройства
    POS_ID_P2_PKA350_ROTATOR_HEIGHT,
    /// высота центра левого блока пил
    POS_ID_P2_PKA350_LEFT_HEIGHT,
    /// высота центра правого блока пил
    POS_ID_P2_PKA350_RIGHT_HEIGHT,
    /// предустановка высоты прижима отделителя досок
    POS_ID_P2_PKA350_PRESS_HEIGHT,

    // зона 3го профилятора(PA300)

    /// положение левого блока
    POS_ID_P3_PA300_LEFT_BLOCK,
    /// положение правого блока
    POS_ID_P3_PA300_RIGHT_BLOCK,
    /// положение фрезы верхняя левая
    POS_ID_P3_PA300_TOP_LEFT,
    /// положение фрезы верхняя правая
    POS_ID_P3_PA300_TOP_RIGHT,
    /// положение фрезы нижняя левая
    POS_ID_P3_PA300_BOTTOM_LEFT,
    /// положение фрезы нижняя правая
    POS_ID_P3_PA300_BOTTOM_RIGHT,
    /// предустановка ширины центрирующих роликов на входе
    POS_ID_P3_PA300_ROLLERS_INP_WIDTH,

    // зона многопила(DWS350)

    /// высота 1го прижимного ролика
    POS_ID_DWS350_PRESS1_HEIGHT,
    /// высота 2го прижимного ролика
    POS_ID_DWS350_PRESS2_HEIGHT,
    /// высота 3го прижимного ролика
    POS_ID_DWS350_PRESS3_HEIGHT,
    /// предустановка ширины центрирующих роликов на входе
    POS_ID_DWS350_INP_ROLLERS_WIDTH,
    /// предустановка ширины центрирующих роликов на выходе
    POS_ID_DWS350_OUT_ROLLERS_WIDTH,
    /// позиция осей многопила
    POS_ID_DWS350_AXIS_HEIGHT,

    /// количество позиций
    POS_ID_COUNT
};
} // namespace Coords
