#ifndef LIBSCHEMA_XMLREADER_HXX
#define LIBSCHEMA_XMLREADER_HXX

#include <schema/reader.hxx>

#include <QXmlStreamReader>

namespace libschema
{
    using XmlInputStream = QXmlStreamReader;
    struct XmlReader: public Reader
    {
        bool read(Schema* schema, QIODevice& input) override;
        bool hasError() const override;
        QString errorMessage() const override;
        ~XmlReader() override;
    private:
        XmlInputStream xml;
    };
} // namespace libschema

#endif //LIBSCHEMA_XMLREADER_HXX
