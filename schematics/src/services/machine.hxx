#ifndef SCHEMATICS_MACHINE_HXX
#define SCHEMATICS_MACHINE_HXX

#include <QObject>

#include <cstdint>

namespace Schematics::Service
{

struct Tag
{
    using Byte = std::uint8_t;
    using Word = std::uint16_t;
    using DWord = std::uint32_t;
    using Int = std::int16_t;
    using DInt = std::int32_t;
    enum class Area
    {
        INPUT, OUTPUT, MEMORY, DATA
    };
    enum class Type
    {
        BIT, BYTE, WORD, DWORD, INT, DINT
    };
    int areaCode() const;
    int typeCode() const;
    int address() const;
    int blockNo() const;
    int itemCount() const;
    virtual const void* pointer() const = 0;
    virtual void* pointer() = 0;
protected:
    virtual ~Tag();
    Area area = Area::MEMORY;
    Type type = Type::BIT;
    int start = 0;
    int count = 1;
    int block = 0;
};

struct BoolTag: public Tag
{
    BoolTag(Area area_id,
            int byte_no, int bit_no, int block_no = 0);
    const void* pointer() const override { return &value; }
    void* pointer() override { return &value; }
    void set(bool v) { value = v; }
    bool get() const { return value; }
    void invert() { set(!get()); }
private:
    bool value;
};


struct ByteTag: public Tag
{
    ByteTag(Area area_id,
            int byte_no, int block_no = 0);
    const void* pointer() const override { return &value; }
    void* pointer() override { return &value; }
    void set(Byte v) { value = v; }
    Byte get() const { return value; }
private:
    Byte value;
};


struct IntTag: public Tag
{
    IntTag(Area area_id,
            int byte_no, int block_no = 0);
    const void* pointer() const override { return &value; }
    void* pointer() override { return &value; }
    void set(Int v);
    Int get() const;
private:
    Int value;
};

struct DIntTag: public Tag
{
    DIntTag(Area area_id,
           int byte_no, int block_no = 0);
    const void* pointer() const override { return &value; }
    void* pointer() override { return &value; }
    void set(DInt v);
    DInt get() const;
private:
    DInt value;
};

class Machine: public QObject
{
    Q_OBJECT
public:
    explicit Machine(QObject* parent = nullptr);
    ~Machine() override;
    bool connect(const QString& address) const
    { return connect(address, 0, 2); }
    bool connect(const QString& address, int rack, int slot) const;
    bool connected() const;
    void disconnect() const;

    bool readTag(Tag& tag) const;
    bool writeTag(const Tag& tag) const;

    bool hasError() const;
    int lastError() const;
    QString errorMessage() const;
private /* types */:
    struct Connection;
private /* methods */:
    Connection* self() const;
private /* members */:
    Connection* selfptr = nullptr;
};

} // namespace Schematics::Service

#endif //SCHEMATICS_MACHINE_HXX
