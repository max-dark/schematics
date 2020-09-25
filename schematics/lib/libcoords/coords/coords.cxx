#include "coords.hxx"

namespace Coords {

// >>>>>>>>>>>>>>>>>

void SchemeCalculator::calculate(const Schema *schema)
{
    //
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
