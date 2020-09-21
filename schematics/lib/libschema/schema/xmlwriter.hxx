#ifndef SCHEMATICS_XMLWRITER_HXX
#define SCHEMATICS_XMLWRITER_HXX

#include <schema/writer.hxx>

namespace libschema
{
    struct XmlWriter: public Writer
    {
        void write(const Schema* schema, QIODevice& output) override;
        ~XmlWriter() override;
    };
} // namespace libschema

#endif //SCHEMATICS_XMLWRITER_HXX
