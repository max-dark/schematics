#include "xmltags.hxx"

namespace libschema::xml
{

    QString Scheme::rootTag() {
        return QString("schematics");
    }

    QString Scheme::paramsTag() {
        return QString("params");
    }

    QString Scheme::sawsTag() {
        return QString("saws");
    }

    QString Scheme::diameterTag() {
        return QString("diameter");
    }

    QString Scheme::rotatorTag() {
        return QString("rotator");
    }

    QString Scheme::versionAttr() {
        return QString("version");
    }

    QString Scheme::pkaSawTag() {
        return QString("pka");
    }

    QString Scheme::dwsSawTag() {
        return QString("dws");
    }

    QString Scheme::disabledAttr() {
        return QString("disable");
    }

    QString Scheme::schemeTag() {
        return QString("scheme");
    }

    QString Scheme::dws350Tag() {
        return QString("dws350");
    }

    QString Scheme::pa300Tag() {
        return QString("pa300");
    }

    QString Scheme::pka350Tag() {
        return QString("pka350");
    }

    QString Scheme::pa350Tag() {
        return QString("pa350");
    }

    QString Scheme::widthTag() {
        return QString("width");
    }

    QString Scheme::heightTag() {
        return QString("height");
    }

    QString Scheme::fileVersion() {
        return QString("1.0");
    }
} // namespace libschema::xml
