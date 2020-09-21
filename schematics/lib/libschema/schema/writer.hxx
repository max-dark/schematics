#ifndef LIBSCHEMA_WRITER_HXX
#define LIBSCHEMA_WRITER_HXX

#include <schema/schema.hxx>

#include <QIODevice>

namespace libschema
{
    struct Writer
    {
        virtual void write(const Schema* schema, QIODevice& output) = 0;
        virtual ~Writer();
    };
} // namespace libschema

#endif //LIBSCHEMA_WRITER_HXX
