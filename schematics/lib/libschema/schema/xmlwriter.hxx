#ifndef SCHEMATICS_XMLWRITER_HXX
#define SCHEMATICS_XMLWRITER_HXX

#include <schema/writer.hxx>

#include <QXmlStreamWriter>

namespace libschema
{
    using XmlOutputStream = QXmlStreamWriter;

    struct XmlWriter: public Writer
    {
        void write(const Schema* schema, QIODevice& output) override;
        ~XmlWriter() override;
    private:
        XmlOutputStream xml;
    };
} // namespace libschema

#endif //SCHEMATICS_XMLWRITER_HXX
