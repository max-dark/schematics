#include "coords.hxx"

namespace Coords {

// >>>>>>>>>>>>>>>>>

void SchemeCalculator::calculate(const Schema *schema)
{
    const auto* const params = schema->params();
    const auto saw_dws = params->dws_gap();
    const auto saw_pka = params->pka_gap();
    const auto do_rotate = ! params->is_rot2_disabled();
    const auto diameter = params->diameter();

    const auto infinity = Unit::from_units(-1);
    const auto two = Unit::from_units(2);
    const auto zero = Unit::from_units(0);
    // calc dws zone
    // размер бруса из центральных досок
    dws_height = schema->calc_dws350_height();
    dws_width = schema->calc_dws350_width();
    // calc p3 zone
    // размер на входе в 3й профилятор
    p3_height = dwsHeight();
    p3_width = dwsWidth();
    p3_bottom = infinity;
    p3_top = infinity;

    // учтем наличие боковой доски
    if(schema->is_pa300_enabled())
    {
        auto h = schema->pa300().board_width;
        auto w = schema->pa300().board_height;
        p3_bottom = (p3Height() - h) / two;
        p3_top = p3Bottom() + h;

        p3_width = p3_width + (saw_dws + w) * two;
    }

    // размер бруса на входе 2го поворотного
    r2_width = p3_width;
    r2_height = p3_height;
    if (do_rotate)
    {
        // работает 2е ПУ: поворачиваем брус на бок
        std::swap(r2_width, r2_height);
    }
    // calc p2 zone
    p2_width = r2_width;
    p2_height = r2_height;
    p2_bottom = infinity;
    p2_top = infinity;
    p2_roller_pos = infinity;
    p2_outer_saw = infinity;
    if(schema->is_pka350_enabled())
    {
        auto& pka = schema->pka350();
        p2_bottom = (p2Height() - pka.board_width) / two;
        p2_top = p2Bottom() + pka.board_width;

        p2_width = p2_width + (saw_pka + pka.board_height) * two;

        p2_roller_pos = saw_pka + pka.board_height;

        if (schema->is_pa350_enabled())
        {
            p2_outer_saw = p2_roller_pos;
            p2_roller_pos = p2_roller_pos + saw_pka + schema->pa350().board_height;
        }
    }

    // calc p1 zone
    p1_width = p2_width;
    p1_height = p2_height;
    p1_bottom = infinity;
    p1_top = infinity;
    if (schema->is_pa350_enabled())
    {
        auto saw = saw_pka;
        auto& pa = schema->pa350();
        p1_bottom = (p1Height() - pa.board_width) / two;
        p1_top = p1Bottom() + pa.board_width;

        p1_width = p1_width + (saw + pa.board_height) * two;
    }
    // calc f2 zone
    fbs2_align = zero;
    fbs2_width = p1_width;
    fbs2_height = p1_height;

    // calc f1 zone
    fbs1_width = fbs2_height;
    fbs1_height = diameter;
}

// <<<<<<<<<<<<<<<<<
// >>>>>>>>>>>>>>>>>

Unit SchemeCalculator::fbs1Width() const
{
    return fbs1_width;
}

Unit SchemeCalculator::fbs1Height() const
{
    return fbs1_height;
}

// <<<<<<<<<<<<<<<<<
// >>>>>>>>>>>>>>>>>

libschema::Unit SchemeCalculator::fbs2Align() const
{
    return fbs2_align;
}

libschema::Unit SchemeCalculator::fbs2Width() const
{
    return fbs2_width;
}

libschema::Unit SchemeCalculator::fbs2Height() const
{
    return fbs2_height;
}

// <<<<<<<<<<<<<<<<<
// >>>>>>>>>>>>>>>>>

libschema::Unit SchemeCalculator::p1Width() const
{
    return p1_width;
}

libschema::Unit SchemeCalculator::p1Height() const
{
    return p1_height;
}

libschema::Unit SchemeCalculator::p1Bottom() const
{
    return p1_bottom;
}

libschema::Unit SchemeCalculator::p1Top() const
{
    return p1_top;
}

libschema::Unit SchemeCalculator::r2Width() const
{
    return r2_width;
}

libschema::Unit SchemeCalculator::r2Height() const
{
    return r2_height;
}

// <<<<<<<<<<<<<<<<<
// >>>>>>>>>>>>>>>>>

libschema::Unit SchemeCalculator::p2Width() const
{
    return p2_width;
}

libschema::Unit SchemeCalculator::p2Height() const
{
    return p2_height;
}

libschema::Unit SchemeCalculator::p2Bottom() const
{
    return p2_bottom;
}

libschema::Unit SchemeCalculator::p2Top() const
{
    return p2_top;
}

Unit SchemeCalculator::p2OuterSaw() const
{
    return p2_outer_saw;
}

Unit SchemeCalculator::p2RollerPos() const
{
    return p2_roller_pos;
}

// <<<<<<<<<<<<<<<<<
// >>>>>>>>>>>>>>>>>

libschema::Unit SchemeCalculator::p3Width() const
{
    return p3_width;
}

libschema::Unit SchemeCalculator::p3Height() const
{
    return p3_height;
}

libschema::Unit SchemeCalculator::p3Bottom() const
{
    return p3_bottom;
}

libschema::Unit SchemeCalculator::p3Top() const
{
    return p3_top;
}

// <<<<<<<<<<<<<<<<<
// >>>>>>>>>>>>>>>>>

libschema::Unit SchemeCalculator::dwsWidth() const
{
    return dws_width;
}

libschema::Unit SchemeCalculator::dwsHeight() const
{
    return dws_height;
}

// <<<<<<<<<<<<<<<<<

} // namespace CoordSystem
