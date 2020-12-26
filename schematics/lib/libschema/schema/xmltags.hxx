#ifndef SCHEMATICS_XMLTAGS_HXX
#define SCHEMATICS_XMLTAGS_HXX

#include <QString>

namespace libschema::xml
{
    namespace Scheme
    {
        QString rootTag();
        QString versionAttr();
        QString paramsTag();
        QString sawsTag();
        QString pkaSawTag();
        QString dwsSawTag();
        QString diameterTag();
        QString rotatorTag();
        QString disabledAttr();
        QString schemeTag();
        QString dws350Tag();
        QString pa300Tag();
        QString pka350Tag();
        QString pa350Tag();
        QString widthTag();
        QString heightTag();

        QString fileVersion();
    };
} // namespace libschema::xml

#endif //SCHEMATICS_XMLTAGS_HXX
