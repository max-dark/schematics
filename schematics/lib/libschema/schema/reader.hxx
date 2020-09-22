#ifndef LIBSCHEMA_READER_HXX
#define LIBSCHEMA_READER_HXX

#include <schema/schema.hxx>

#include <QIODevice>

namespace libschema
{
    struct Reader
    {
        virtual bool read(Schema* schema, QIODevice& input) = 0;
        virtual bool hasError() const = 0;
        virtual QString errorMessage() const = 0;
        virtual ~Reader();
    };
} // namespace libschema

#endif //LIBSCHEMA_READER_HXX
