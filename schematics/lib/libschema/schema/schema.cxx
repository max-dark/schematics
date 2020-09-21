#include <schema/schema.hxx>
#include <algorithm>

namespace libschema
{
Schema::Schema(QObject* parent):QObject{parent} {}

void Schema::set_params(Params *params)
{
    if (m_params != params)
    {
        if (m_params) m_params->deleteLater();
        m_params = params;
        if (m_params) {
            m_params->setParent(this);
            connect(m_params, &Params::changed,
                    this, &Schema::on_params_changed);
        }
        emit schemeChanged();
    }
}

Params *Schema::params() const
{
    return m_params;
}

void Schema::remove_all()
{
    m_pa350.clear();
    m_pka350.clear();
    m_pa300.clear();
    m_dws350.clear();

    emit schemeChanged();
}

void Schema::remove_dws_board(int idx)
{
    m_dws350.boards.removeAt(idx);
    emit schemeChanged();
}

void Schema::set_dws_board_width(Unit width)
{
    if (width != m_dws350.board_width)
    {
        m_dws350.board_width = width;
        emit schemeChanged();
    }
}

Unit Schema::dws_board_width() const
{
    return m_dws350.board_width;
}

Unit Schema::dws_board_height(int idx) const
{
    return m_dws350.boards.at(idx);
}

void Schema::set_dws_board_height(int idx, Unit height)
{
    auto & board = m_dws350.boards[idx];
    if (board != height)
    {
        board = height;
        emit schemeChanged();
    }
}

void Schema::add_dws_board(Unit height)
{
    m_dws350.boards.push_back(height);
    emit schemeChanged();
}

const Schema::DWS350 &Schema::dws350() const
{
    return m_dws350;
}

Unit Schema::calc_dws350_height() const
{
    return m_dws350.height();
}

Unit Schema::calc_dws350_width() const
{
    return m_dws350.calc_width() + m_dws350.calc_gap(params()->dws_gap());
}

const Schema::PA300 &Schema::pa300() const
{
    return m_pa300;
}

void Schema::remove_pa300_poard()
{
    m_pa300.clear();
    emit schemeChanged();
}

void Schema::set_pa300_board(Unit width, Unit height)
{
    m_pa300 = PA300{width, height};
    emit schemeChanged();
}

bool Schema::is_pa300_enabled() const
{
    return m_dws350.validate() and m_pa300.is_valid();
}

const Schema::PKA350 &Schema::pka350() const
{
    return m_pka350;
}

void Schema::remove_pka350_poard()
{
    m_pka350.clear();
}

void Schema::set_pka350_board(Unit width, Unit height)
{
    m_pka350.board_width = width;
    m_pka350.board_height = height;
}

bool Schema::is_pka350_enabled() const
{
    return pka350().is_valid();
}

const Schema::PA350 &Schema::pa350() const
{
    return m_pa350;
}

void Schema::remove_pa350_poard()
{
    m_pa350.clear();
}

void Schema::set_pa350_board(Unit width, Unit height)
{
    m_pa350.board_width = width;
    m_pa350.board_height = height;
}

bool Schema::is_pa350_enabled() const
{
    return pa350().is_valid();
}


void Schema::on_params_changed(const Params* /* params */)
{
    emit schemeChanged();
}

Schema::~Schema() = default;

bool Schema::DWS350::validate() const
{
    bool valid =  (board_width.units() > 0) && (boards.size() > 0);

    for (auto b: boards)
    {
        valid &= (b.units() > 0);
    }

    return valid;
}

Unit Schema::DWS350::calc_width() const
{
    return std::accumulate(boards.begin(), boards.end(), Unit{});
}

Unit Schema::DWS350::height() const
{
    return board_width;
}

Unit Schema::DWS350::count() const
{
    return Unit::from_units(boards.count());
}

void Schema::DWS350::clear()
{
    board_width = Unit{};
    boards.clear();
}

Unit Schema::DWS350::calc_gap(Unit gap) const
{
    Unit gap_cnt = count() - Unit::from_units(1);
    if (gap_cnt.units() < 0)
        gap_cnt = Unit{};

    return gap * gap_cnt;
}

bool Schema::PA300::is_valid() const
{
    return (board_width.units() > 0) && (board_height.units() > 0);
}

void Schema::PA300::clear()
{
    board_height = board_width = Unit::from_units(0);
}

bool Schema::PKA350::is_valid() const
{
    return (board_width.units() > 0) && (board_height.units() > 0);
}

void Schema::PKA350::clear()
{
    board_height = board_width = Unit::from_units(0);
}

bool Schema::PA350::is_valid() const
{
    return (board_width.units() > 0) && (board_height.units() > 0);
}

void Schema::PA350::clear()
{
    board_height = board_width = Unit::from_units(0);
}

}
