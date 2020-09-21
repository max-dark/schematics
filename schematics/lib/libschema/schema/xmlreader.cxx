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

        void do_read_sizes(XmlStream& xml, Unit& width, Unit& height, bool& ok)
        {
            while (xml.readNextStartElement())
            {
                if (xml.name() == tags::widthTag())
                {
                    width = do_read_units(xml, ok);
                }
                else if (xml.name() == tags::heightTag())
                {
                    height = do_read_units(xml, ok);
                }
                else
                {
                    xml.skipCurrentElement();
                }
            }
        }

        void do_read_scheme(Schema* schema, XmlStream& xml)
        {
            bool ok;
            Unit tmp_width, tmp_height;
            while (xml.readNextStartElement())
            {
                qDebug() << xml.name();
                if (xml.name() == tags::dws350Tag())
                {
                    while (xml.readNextStartElement())
                    {
                        if (xml.name() == tags::widthTag())
                        {
                            auto w = do_read_units(xml, ok);
                            qDebug() << ok << "width = " << w.to_mm();
                        }
                        else if (xml.name() == tags::heightTag())
                        {
                            auto h = do_read_units(xml, ok);
                            qDebug() << ok << "height = " << h.to_mm();
                        }
                        else
                        {
                            xml.skipCurrentElement();
                        }
                    }
                }
                else if (xml.name() == tags::pa300Tag())
                {
                    do_read_sizes(xml, tmp_width, tmp_height, ok);
                    qDebug() << "pa300" << tmp_width.to_mm() << tmp_height.to_mm();
                }
                else if (xml.name() == tags::pka350Tag())
                {
                    do_read_sizes(xml, tmp_width, tmp_height, ok);
                    qDebug() << "pka350" << tmp_width.to_mm() << tmp_height.to_mm();
                }
                else if (xml.name() == tags::pa350Tag())
                {
                    do_read_sizes(xml, tmp_width, tmp_height, ok);
                    qDebug() << "pa350" << tmp_width.to_mm() << tmp_height.to_mm();
                }
            }
        }
        void do_read_params(Schema* schema, XmlStream& xml)
        {
            bool ok;
            while (xml.readNextStartElement())
            {
                qDebug() << xml.name();
                if (xml.name() == tags::diameterTag())
                {
                    auto diam = do_read_units(xml, ok);
                    if (ok)
                        qDebug() << diam.to_mm();//schema->params()->set_diameter(diam);
                    else
                        xml.raiseError("error in diam tag: cant parse num");
                }
                else if (xml.name() == tags::sawsTag())
                {
                    while (xml.readNextStartElement())
                    {
                        qDebug() << xml.name();
                        if (xml.name() == tags::pkaSawTag())
                        {
                            auto saw = do_read_units(xml, ok);
                            qDebug() << ok << saw.to_mm();
                        }
                        else if (xml.name() == tags::dwsSawTag())
                        {
                            auto saw = do_read_units(xml, ok);
                            qDebug() << ok << saw.to_mm();
                        }
                    }
                }
                else if (xml.name() == tags::rotatorTag())
                {
                    auto attr = xml.attributes().value(tags::disabledAttr());
                    qDebug() << attr;
                    xml.skipCurrentElement();
                }
            }
        }
        void do_read(Schema* schema, XmlStream& xml)
        {
            while (xml.readNextStartElement())
            {
                qDebug() << xml.name();
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
