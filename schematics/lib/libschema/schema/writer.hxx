#ifndef LIBSCHEMA_WRITER_HXX
#define LIBSCHEMA_WRITER_HXX

#include <schema/schema.hxx>

#include <QIODevice>
#include <QString>

namespace libschema
{
    struct Writer
    {
        virtual void write(const Schema* schema, QIODevice& output) = 0;
        virtual bool hasError() const = 0;
        virtual QString errorMessage() const = 0;
        virtual ~Writer();
    };
} // namespace libschema

#endif //LIBSCHEMA_WRITER_HXX
