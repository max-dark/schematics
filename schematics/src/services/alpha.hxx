#pragma once

#include <coords/positionid.hxx>
#include <coords/coordinates.hxx>
#include <coords/offset.hxx>

#include <schema/units.hxx>

#include <services/application.hxx>
#include <services/machine.hxx>
#include <services/tags.hxx>

#include <QObject>


namespace Schematics::Service
{

struct Storage;

class Alpha: public Machine
{
    Q_OBJECT
public:
    explicit Alpha(Coords::OffsetRepository* offsets,
          QObject* parent = nullptr);

    /// получить статус завершения нинциализации
    /// перед вызовом нужно обновить кэш \see Machine::updateCache
    bool axisInitIsDone() const;
    bool axisInitIsStarted() const;
    /// запускает инициализацию контроллеров "@xis"
    /// для работы нужно подключение к ПЛК
    bool axisStartInit() const;
    /// сбрасывает биты инициализации "@xis"
    /// это необходимо для повторного вызова axisStartInit
    bool axisResetInit() const;

    /// Обновить разрешение на работу участка подачи
    bool allowFeederToWork(bool allow);

    bool setSpeedForZone(Storage* storage, int zone_id, int speed);
    SpeedMap getCurrentSpeeds();

    bool applyCoordById(Coords::PositionId id, libschema::Unit value);
    bool applyCoordinates(const Coords::Coordinates& coords);
    const CoordMap& getCoordMap() const;
private:
    void initCoordMap();
    void initMemoryMap();
    void initSpeedMap();
    Coords::OffsetRepository* offsetList = nullptr;
    CoordMap coord_map;
    std::map<int, TagAddress> speed_map;
};

} // namespace Schematics::Service
