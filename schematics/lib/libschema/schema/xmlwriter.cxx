#include "xmlwriter.hxx"
#include "xmltags.hxx"

#include <QXmlStreamWriter>

namespace libschema
{
namespace tags = ::libschema::xml::Scheme;

using XmlStream = QXmlStreamWriter;

XmlWriter::~XmlWriter() = default;
namespace {
    void write_unit(XmlStream& xml, const QString& tagName, Unit value)
    {
        xml.writeTextElement(tagName, QString::number(value.units()));
    }

    void write_params(const Params *params, XmlStream &xml) {
        xml.writeStartElement(tags::paramsTag());
        write_unit(xml, tags::diameterTag(), params->diameter());
        xml.writeStartElement(tags::sawsTag());
        write_unit(xml, tags::dwsSawTag(), params->dws_gap());
        write_unit(xml, tags::pkaSawTag(), params->pka_gap());
        xml.writeEndElement(); // saws tag
        xml.writeEmptyElement(tags::rotatorTag());
        xml.writeAttribute(tags::disabledAttr(), (params->is_rot2_disabled()? "true" : "false"));
        xml.writeEndElement();
    }

    void write_group(XmlStream& xml, const QString& name, Unit width, Unit height)
    {
        xml.writeStartElement(name);
        write_unit(xml, tags::widthTag(), width);
        write_unit(xml, tags::heightTag(), height);
        xml.writeEndElement();
    }
}
void XmlWriter::write(const libschema::Schema *schema, QIODevice &output) {
    XmlStream xml;

    xml.setDevice(&output);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement(tags::rootTag());
    xml.writeAttribute(tags::versionAttr(), tags::fileVersion());
    {
        write_params(schema->params(), xml);
        xml.writeStartElement(tags::schemeTag());
        {
            xml.writeStartElement(tags::dws350Tag());
            write_unit(xml, tags::widthTag(), schema->dws_board_width());
            for (const auto &h: schema->dws350().boards) {
                write_unit(xml, tags::heightTag(), h);
            }
            xml.writeEndElement();

            write_group(xml, tags::pa300Tag(),
                        schema->pa300().board_width, schema->pa300().board_height);
            write_group(xml, tags::pka350Tag(),
                        schema->pka350().board_width, schema->pka350().board_height);
            write_group(xml, tags::pa350Tag(),
                        schema->pa350().board_width, schema->pa350().board_height);
        }
        xml.writeEndElement(); // scheme tag
    }
    xml.writeEndElement(); // root tag
    xml.writeEndDocument();
}

} // namespace libschema
