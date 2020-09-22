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
            bool w_ok = false;
            bool h_ok = false;
            while (xml.readNextStartElement())
            {
                if ((!w_ok) && (xml.name() == tags::widthTag()))
                {
                    width = do_read_units(xml, w_ok);
                }
                else if ((!h_ok) && (xml.name() == tags::heightTag()))
                {
                    height = do_read_units(xml, h_ok);
                }
                else
                {
                    xml.skipCurrentElement();
                }
            }

            ok = w_ok && h_ok;
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
                            if (ok)
                            {
                                schema->set_dws_board_width(w);
                            }
                            qDebug() << ok << "width = " << w.to_mm();
                        }
                        else if (xml.name() == tags::heightTag())
                        {
                            auto h = do_read_units(xml, ok);
                            if (ok)
                            {
                                schema->add_dws_board(h);
                            }
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
                    if (ok)
                    {
                        schema->set_pa300_board(tmp_width, tmp_height);
                    }
                    qDebug() << ok << "pa300" << tmp_width.to_mm() << tmp_height.to_mm();
                }
                else if (xml.name() == tags::pka350Tag())
                {
                    do_read_sizes(xml, tmp_width, tmp_height, ok);
                    if (ok)
                    {
                        schema->set_pka350_board(tmp_width, tmp_height);
                    }
                    qDebug() << ok << "pka350" << tmp_width.to_mm() << tmp_height.to_mm();
                }
                else if (xml.name() == tags::pa350Tag())
                {
                    do_read_sizes(xml, tmp_width, tmp_height, ok);
                    if (ok)
                    {
                        schema->set_pa350_board(tmp_width, tmp_height);
                    }
                    qDebug() << ok << "pa350" << tmp_width.to_mm() << tmp_height.to_mm();
                }
                else
                {
                    xml.skipCurrentElement();
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
                    {

                        qDebug() << diam.to_mm();
                        schema->params()->set_diameter(diam);
                    }
                    else
                    {
                        xml.raiseError("Parse error in /params/diameter");
                    }
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
                            if (ok)
                            {
                                schema->params()->set_pka_gap(saw);
                            }
                            else
                            {
                                xml.raiseError("Parse error in /params/saws/pka");
                            }
                        }
                        else if (xml.name() == tags::dwsSawTag())
                        {
                            auto saw = do_read_units(xml, ok);
                            qDebug() << ok << saw.to_mm();
                            if (ok)
                            {
                                schema->params()->set_dws_gap(saw);
                            }
                            else
                            {
                                xml.raiseError("Parse error in /params/saws/dws");
                            }
                        }
                    }
                }
                else if (xml.name() == tags::rotatorTag())
                {
                    auto attr = xml.attributes().value(tags::disabledAttr());
                    auto disable = ("true" == attr);
                    schema->params()->set_rot2_mode(disable);
                    qDebug() << attr;
                    xml.skipCurrentElement();
                }
                else
                {
                    xml.raiseError(
                        QString{"Format error: unknown tag /params/%1"}
                            .arg(xml.name()));
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
                    xml.raiseError(
                        QString{"Format error: unknown tag /%1"}
                            .arg(xml.name()));
                }
            }
        }
    } // internal

    bool XmlReader::read(Schema* schema, QIODevice &input) {
        XmlStream xml;

        Q_ASSERT(schema != nullptr);
        Q_ASSERT(schema->params() != nullptr);

        xml.setDevice(&input);
        if (xml.readNextStartElement())
        {
            if (is_file_supported(xml))
            {
                schema->remove_all();
                do_read(schema, xml);
            }
            else
            {
                xml.raiseError("Format error: This file is not supported");
            }
        }
        qDebug() << "Error message:" << xml.errorString();
        return !xml.hasError();
    }

    XmlReader::~XmlReader() = default;
} // namespace libschema
