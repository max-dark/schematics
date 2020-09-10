#pragma once

#include <QObject>

#include <schema/params.hxx>
#include <schema/units.hxx>

namespace libschema
{

class Schema:public QObject
{
    Q_OBJECT
public:
    struct DWS350
    {
        Unit board_width;
        QList<Unit> boards;

        bool validate() const;

        Unit count() const;
        void clear();
        Unit height() const;
        Unit calc_width() const;
        Unit calc_gap(Unit gap) const;
    };

    struct PA300
    {
        Unit board_width;
        Unit board_height;

        bool is_valid() const;

        void clear();
    };

    struct PKA350
    {
        Unit board_width;
        Unit board_height;

        bool is_valid() const;

        void clear();
    };

    struct PA350
    {
        Unit board_width;
        Unit board_height;

        bool is_valid() const;

        void clear();
    };

    Schema(QObject* parent = nullptr);
    virtual ~Schema();

    void set_params(Params* params);
    Params * params() const;

    void remove_all();

    void remove_dws_board(int idx);
    void set_dws_board_width(Unit width);
    Unit dws_board_width() const;

    Unit dws_board_height(int idx) const;
    void set_dws_board_height(int idx, Unit height);
    void add_dws_board(Unit height);

    const DWS350& dws350() const;
    Unit calc_dws350_height() const;
    Unit calc_dws350_width() const;

    void remove_pa300_poard();
    void set_pa300_board(Unit width, Unit height);
    bool is_pa300_enabled() const;
signals:
    void schemeChanged();
private slots:
    void on_params_changed(const Params* params);

private:
    Params* m_params = nullptr;
    DWS350 m_dws350;
    PA300  m_pa300;
    PKA350 m_pka350;
    PA350  m_pa350;
};

}
