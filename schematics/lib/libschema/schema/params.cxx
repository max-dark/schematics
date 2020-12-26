#include <schema/params.hxx>

namespace libschema
{
Params::Params(QObject* parent)
    : QObject{parent}
    , m_diameter{}
    , m_dws_gap{}
    , m_pka_gap{}
    , m_rot2_disabled{false}
{}

Params::Params(Unit diameter,
               Unit dws_gap, Unit pka_gap,
               bool rot2_disabled,
               QObject *parent)
    : QObject{parent}
    , m_diameter{diameter}
    , m_dws_gap{dws_gap}
    , m_pka_gap{pka_gap}
    , m_rot2_disabled{rot2_disabled}
{}

void Params::set_diameter(Unit new_diameter)
{
    update_field(m_diameter, new_diameter);
}

void Params::set_dws_gap(Unit gap)
{
    update_field(m_dws_gap, gap);
}

void Params::set_pka_gap(Unit gap)
{
    update_field(m_pka_gap, gap);
}

void Params::set_rot2_mode(bool is_disabled)
{
    update_field(m_rot2_disabled, is_disabled);
}
Params::~Params() = default;
} // namespace libschema
