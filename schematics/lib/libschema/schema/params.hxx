#pragma once

#include <QObject>

#include <schema/units.hxx>

namespace libschema
{
    class Params: public QObject
    {
        Q_OBJECT
    public:
        Params(QObject* parent = nullptr);
        Params(Unit diameter,
               Unit dws_gap,
               Unit pka_gap,
               bool rot2_disabled,
               QObject* parent = nullptr);
        virtual ~Params();

        Unit diameter() const { return m_diameter; }
        Unit dws_gap() const { return m_dws_gap; }
        Unit pka_gap() const { return m_pka_gap; }
        bool is_rot2_disabled() const { return m_rot2_disabled; }

        void set_diameter(Unit new_diameter);
        void set_dws_gap(Unit gap);
        void set_pka_gap(Unit gap);
        void set_rot2_mode(bool is_disabled);

    signals:
        void changed(const libschema::Params* params);
    private:
        template<class MemberT>
        void update_field(MemberT& field, const MemberT& value)
        {
            if (field != value)
            {
                field = value;
                emit changed(this);
            }
        }
        Unit m_diameter {};
        Unit m_dws_gap {};
        Unit m_pka_gap {};
        bool m_rot2_disabled = false;
    };
}
