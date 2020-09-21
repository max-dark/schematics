#include "xmlreader.hxx"

#include <schema/xmltags.hxx>

#include <QXmlStreamReader>
#include <QDebug>

namespace libschema
{
    namespace tags = xml::Scheme;
    using XmlStream = QXmlStreamReader;

    namespace /*internal*/
    {
        bool is_file_supported(XmlStream& xml)
        {
            return xml.name() == tags::rootTag() &&
                xml.attributes().value(tags::versionAttr()) == tags::fileVersion();
        }

        Unit do_read_units(XmlStream& xml, bool& ok)
        {
            auto str = xml.readElementText();
            auto num = str.toLong(&ok);
            return Unit::from_units(num);
        }

        void do_read_scheme(Schema* schema, XmlStream& xml)
        {
            ;
        }
        void do_read_params(Schema* schema, XmlStream& xml)
        {
            bool ok;
            while (xml.readNextStartElement())
            {
                if (xml.name() == tags::diameterTag())
                {
                    auto diam = do_read_units(xml, ok);
                    if (ok)
                        qDebug() << diam.units();//schema->params()->set_diameter(diam);
                    else
                        xml.raiseError("error in diam tag: cant parse num");
                }
                else if (xml.name() == tags::sawsTag())
                {
                    while (xml.readNextStartElement())
                    {

                    }
                }
            }
        }
        void do_read(Schema* schema, XmlStream& xml)
        {
            while (xml.readNextStartElement())
            {
                if (xml.name() == tags::paramsTag())
                {
                    do_read_params(schema, xml);
                }
                else if (xml.name() == tags::schemeTag())
                {
                    do_read_scheme(schema, xml);
                }
                else
                {
                    xml.skipCurrentElement();
                }
            }
        }
    } // internal

    bool XmlReader::read(Schema* schema, QIODevice &input) {
        XmlStream xml;
        xml.setDevice(&input);
        if (xml.readNextStartElement())
        {
            if (is_file_supported(xml))
            {
                do_read(schema, xml);
            }
            else
            {
                xml.raiseError("This file not supported");
            }
        }
        qDebug() << xml.errorString();
        return !xml.hasError();
    }

    XmlReader::~XmlReader() = default;
} // namespace libschema
