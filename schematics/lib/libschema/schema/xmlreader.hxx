#ifndef LIBSCHEMA_XMLREADER_HXX
#define LIBSCHEMA_XMLREADER_HXX

#include <schema/reader.hxx>

namespace libschema
{
    struct XmlReader: public Reader
    {
        bool read(Schema* schema, QIODevice& input) override;
        ~XmlReader() override;
    };
} // namespace libschema

#endif //LIBSCHEMA_XMLREADER_HXX
