#include "facade.hxx"

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QTimer>

#include <QDir>

#include <QDebug>

#include <services/database.hxx>
#include <services/alpha.hxx>
#include <services/alphasupport.hxx>

#include <algorithm>

namespace Schematics::Service
{
Facade::Facade(QObject *parent)
    : QObject(parent)
    , Application{}
    {};

void Facade::parseArguments(const QStringList &argv, const QString &defaultPath, const QString &defaultFile)
{
    QCommandLineOption config{
        QStringList{} << "C" << "config",
        "path to config", "path",
        defaultPath
    };
    QCommandLineOption file{
        QStringList{} << "F" << "file",
        "config file name", "config.db",
        defaultFile
    };
    QCommandLineOption mode{
        QStringList{} << "M" << "mode",
        "running mode(production|testing)", "mode",
        "testing"
    };
    QCommandLineParser parser{};

    parser.addOption(config);
    parser.addOption(file);
    parser.addOption(mode);

    auto ok = parser.parse(argv);

    if (ok)
    {
        configPath = parser.value(config);
        configFile = parser.value(file);
        is_prod_mode = "production" == parser.value(mode);
    }
    else {
        configPath = defaultPath;
        configFile = defaultFile;
        is_prod_mode = false;
    }
    qInfo() << "Parse args ok?" << ok;
    qInfo() << "Using database" << databaseFile();
    qInfo() << "Runing mode:" << (isProductionMode() ? "production" : "testing");
}

QString Facade::databaseFile()
{
    return QDir{configPath}.absoluteFilePath(configFile);
}

bool Facade::isProductionMode() const noexcept
{
    return is_prod_mode;
}

bool Facade::kdoIsRunning()
{
    const auto motors_state = getSupportMotorsState();
    const auto alarms_state = getSupportAlarmsState();
    const auto motors_run = std::all_of(motors_state.begin(), motors_state.end(), [](const auto &item){
        return item.second; // is running
    });
    const auto no_alarms = std::all_of(alarms_state.begin(), alarms_state.end(), [](const auto& item){
        return item.second; // is ok
    });
    return motors_run && no_alarms;
}

void Facade::startStorage()
{
    if (nullptr == database)
    {
        database = new Database{this};
        auto ok = database->open(databaseFile());
        qInfo() << "Start db:" << ok;
        auto valid_db = database->checkStructure();
        qInfo() << "Validate db:" << valid_db;
        if (valid_db)
        {
            alarms = database->getBooleansByName("alarms");
            motors = database->getBooleansByName("motors");
            sensors = database->getBooleansByName("sensors");
            doors = database->getBooleansByName("doors");
        }
    }
    else {
        qWarning() << "db already started";
    }
}

bool Facade::getConnectionParams(const QString &name, QString &address, int &interval)
{
    QString str_addr, str_int;
    int tmp_int;
    auto ok = database->getValueByName("plc/" + name + "/ip", str_addr);
    ok = ok && database->getValueByName("plc/" + name + "/interval", str_int);
    if (ok)
    {
        tmp_int = str_int.toInt(&ok);
        if(ok)
        {
            address = str_addr;
            interval = tmp_int;
        }
    }
    return ok;
}

void Facade::getConnectionState(bool &alphaPlc, bool &supportPlc)
{
    alphaPlc = this->sab->connected() && (!this->sab->hasError());
    supportPlc = this->kdo->connected() && (!this->kdo->hasError());
}

SettingsTable *Facade::coordsTable()
{
    return database->coordsTable();
}

SettingsTable *Facade::delaysTable()
{
    return database->delaysTable();
}

SettingsTable *Facade::speedsTable()
{
    return database->speedsTable();
}

void Facade::resetConnection()
{
    sab->reconnect();
    kdo->reconnect();
}

void Facade::getAxisInitState(bool &is_started, bool &is_done)
{
    is_started = sab->axisInitIsStarted();
    is_done = sab->axisInitIsDone();
}

bool Facade::startAxisInit()
{
    auto reset_ok = sab->axisResetInit();
    auto update_ok = sab->updateCache();
    return reset_ok && update_ok && sab->axisStartInit();
}

void Facade::startSabPlc()
{
    QString addr;
    int interval = 0;
    // connect to main PLC
    sab = new Alpha{database, this};

    const auto mem_coords = sab->getCoordMap();
    const auto db_coords = database->getCoordMap();

    qInfo() << "start coords check...";
    int errors = 0;
    for(const auto&[id, value]: mem_coords)
    {
        auto item = db_coords.find(id);
        if (item == db_coords.end())
        {
            qWarning() << "cant find id" << id << "in db map";
            ++errors;
        }
        else
        {
            auto coord_ok = value.coord.to_string() == item->second.coord.to_string();
            auto apply_ok = value.apply.to_string() == item->second.apply.to_string();
            auto item_ok = coord_ok && apply_ok;
            if (!item_ok)
            {
                qWarning() << "found diff in id" << id;
                ++errors;
            }
        }
    }
    for(const auto&[id, value]: db_coords)
    {
        auto item = mem_coords.find(id);
        if (item == mem_coords.end())
        {
            qWarning() << "cant find id" << id << "in mem map";
            ++errors;
        }
        else
        {
            auto coord_ok = value.coord.to_string() == item->second.coord.to_string();
            auto apply_ok = value.apply.to_string() == item->second.apply.to_string();
            auto item_ok = coord_ok && apply_ok;
            if (!item_ok)
            {
                qWarning() << "found diff in id" << id;
                ++errors;
            }
        }
    }
    qInfo() << "coords check done / errors:" << errors;

    auto ok = getConnectionParams("sab", addr, interval);
    if (ok && isProductionMode())
    {
        ok = sab->connect(addr);
        qInfo() << "try connect to" << addr
                << ok << sab->errorMessage();
        ok = sab->updateCache();
        qInfo() << "Try update:" << sab->errorMessage()
                << sab->axisInitIsDone();
    }
    qDebug() << ok << addr << interval;
}

void Facade::startKdoPlc()
{
    QString addr;
    int interval = 0;
    // connect to secondary PLC

    kdo = new AlphaSupport{this};

    auto ok = getConnectionParams("kdo", addr, interval);
    qDebug() << ok << addr << interval;

    if (ok && isProductionMode())
    {
        qInfo() << "try connect to" << addr;
        ok = kdo->connect(addr);
        qInfo() << ok << kdo->errorMessage();
        if (ok)
        {
            ok = kdo->updateCache();
            qInfo() << "Update kdo:" << ok << kdo->lastError() << kdo->errorMessage();
        }
    }
}

void Facade::startUpdateCycle()
{
    requestStateUpdate();
}

void Facade::updateState()
{
    auto kdo_ok = kdo->updateCache();
    auto kdo_run = kdo_ok && kdoIsRunning();

    if (! kdo_ok)
    {
        emit updateKdoFailure(kdo->errorMessage());
    }

    ///TODO: раскоментировать для работы
    auto sab_ok = true;//sab->allowFeederToWork(kdo_run);
    sab_ok = sab_ok && sab->updateCache();

    if (! sab_ok)
    {
        emit updateSabFailure(sab->errorMessage());
    }

    if (kdo_ok && sab_ok)
    {
        emit updateSuccess();
    }
}

void Facade::requestStateUpdate()
{
    QTimer::singleShot(100, this, &Facade::updateState);
}

bool Facade::applyCoordById(PositionId id, libschema::Unit value)
{
    // get offset by id
    // get address by id
    // convert to plc format
    // send value to plc
    // send "apply" command
    return sab->applyCoordById(id, value);
}

bool Facade::applyCoordinates(const Coordinates &coords)
{
    // get offsets
    // get coord addr list
    // get addr of "apply" bits
    // for each offset
    //  convert coord to plc varible
    //  send coord variable to plc
    //  send "apply" command
    return sab->applyCoordinates(coords);
}

LabelMap Facade::getMotorLabels()
{
    LabelMap labels{};

    for(const auto&[id, value]: motors)
    {
        labels[id] = value.description;
    }
    return labels;
}

LabelMap Facade::getAlarmLabels()
{
    LabelMap labels{};

    for(const auto&[id, value]: alarms)
    {
        labels[id] = value.description;
    }
    return labels;
}

LabelMap Facade::getSensorLabels()
{
    LabelMap labels{};

    for(const auto&[id, value]: sensors)
    {
        labels[id] = value.description;
    }
    return labels;
}

BoolMap Facade::getMotorState()
{
    BoolMap state{};

    for(const auto&[id, value]: motors)
    {
        state[id] = sab->readCachedBit(value.address);
    }

    return state;
}

BoolMap Facade::getAlarmState()
{
    BoolMap state{};

    for(const auto&[id, value]: alarms)
    {
        state[id] = sab->readCachedBit(value.address);
    }

    return state;
}

BoolMap Facade::getSensorState()
{
    BoolMap state{};

    for(const auto&[id, value]: sensors)
    {
        state[id] = sab->readCachedBit(value.address);
    }

    return state;
}

LabelMap Facade::getDoorsLabels()
{
    LabelMap labels{};

    for(const auto&[id, value]: doors)
    {
        labels[id] = value.description;
    }
    return labels;
}

bool Facade::toggleDoorById(int doorId)
{
    if (sab->connected())
    {
        auto address = doors.at(doorId).address;
        auto is_open = sab->readCachedBit(address);
        BoolTag door{address};
        door.set(is_open);
        door.invert();

        return sab->writeTag(door);
    }
    return false;
}

BoolMap Facade::getSupportMotorsState()
{
    return kdo->getMotorsState();
}

BoolMap Facade::getSupportAlarmsState()
{
    return kdo->getAlarmsState();
}

SpeedMap Facade::getCurrentSpeeds()
{
    return sab->getCurrentSpeeds();
}

bool Facade::setSpeedForZone(int zone_id, int speed)
{
    return sab->setSpeedForZone(database, zone_id, speed);
}

Facade::~Facade() = default;

} // namespace Schematics::Service
