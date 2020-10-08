BEGIN TRANSACTION;
DROP TABLE IF EXISTS "config";
CREATE TABLE IF NOT EXISTS "config" (
	"name"	TEXT NOT NULL UNIQUE,
	"value"	TEXT NOT NULL,
	PRIMARY KEY("name")
);
DROP TABLE IF EXISTS "offsets";
CREATE TABLE IF NOT EXISTS "offsets" (
	"id"	INTEGER NOT NULL UNIQUE,
	"type"	INTEGER NOT NULL DEFAULT 0,
	"zone_id"	INTEGER NOT NULL DEFAULT 0,
	"address"	TEXT NOT NULL,
	"apply_bit"	TEXT NOT NULL,
	"per_mm"	REAL NOT NULL DEFAULT 100,
	"offset"	INTEGER NOT NULL DEFAULT 0,
	"description"	TEXT NOT NULL,
	PRIMARY KEY("id")
);
DROP TABLE IF EXISTS "motors";
CREATE TABLE IF NOT EXISTS "motors" (
	"id"	INTEGER NOT NULL UNIQUE,
	"address"	TEXT NOT NULL,
	"description"	TEXT NOT NULL,
	PRIMARY KEY("id")
);
DROP TABLE IF EXISTS "sensors";
CREATE TABLE IF NOT EXISTS "sensors" (
	"id"	INTEGER NOT NULL UNIQUE,
	"address"	TEXT NOT NULL,
	"description"	TEXT NOT NULL,
	PRIMARY KEY("id")
);
DROP TABLE IF EXISTS "delays";
CREATE TABLE IF NOT EXISTS "delays" (
	"id"	INTEGER NOT NULL UNIQUE,
	"address"	TEXT NOT NULL,
	"base"	INTEGER NOT NULL DEFAULT 0,
	"offset"	INTEGER NOT NULL DEFAULT 0,
	"description"	TEXT NOT NULL,
	PRIMARY KEY("id")
);
DROP TABLE IF EXISTS "speeds";
CREATE TABLE IF NOT EXISTS "speeds" (
	"id"	INTEGER NOT NULL UNIQUE,
	"address"	TEXT NOT NULL,
	"base"	INTEGER NOT NULL,
	"offset"	INTEGER NOT NULL,
	"description"	TEXT NOT NULL,
	PRIMARY KEY("id")
);
DROP TABLE IF EXISTS "alarms";
CREATE TABLE IF NOT EXISTS "alarms" (
	"id"	INTEGER NOT NULL UNIQUE,
	"address"	TEXT NOT NULL,
	"description"	TEXT NOT NULL,
	PRIMARY KEY("id")
);
INSERT INTO "config" ("name","value") VALUES ('plc/sab/ip','10.2.22.10');
INSERT INTO "config" ("name","value") VALUES ('plc/kdo/ip','10.2.22.192');
INSERT INTO "config" ("name","value") VALUES ('plc/sab/interval','50');
INSERT INTO "config" ("name","value") VALUES ('plc/kdo/interval','50');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (0,0,1,'M0:66','M0:31.0',100.0,-28916,'ФБС1/ левая фреза');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (1,0,1,'M0:70','M0:31.1',100.0,-92013,'ФБС1/ правая фреза');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (2,1,1,'M0:700','M0:45.0',-28.82,21461,'ФБС1/ 1й прижимной ролик, высота предустановки');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (3,1,1,'M0:702','M0:45.1',-35.96,22482,'ФБС1/ 2й прижимной ролик, высота предустановки');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (4,1,1,'M0:714','M0:45.7',48.0,2160,'ФБС1/ высота до центра поворотного устройства(ПУ)');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (5,1,1,'M0:710','M0:45.5',29.9,14243,'ФБС1/ предустановка ширины роликов ПУ на входе');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (6,1,1,'M0:712','M0:45.6',43.75,-1829,'ФБС1/ предустановка ширины роликов ПУ на выходе');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (7,0,2,'M0:74','M0:31.5',100.0,-29120,'ФБС2/ левая фреза');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (8,0,2,'M0:78','M0:31.6',100.0,-91916,'ФБС2/ правая фреза');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (9,0,2,'M0:82','M0:32.2',100.0,13699,'ФБС2/ 1й ролик выравнивания');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (10,0,2,'M0:86','M0:32.3',100.0,13808,'ФБС2/ 2й ролик выравнивания');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (11,0,2,'M0:90','M0:32.4',100.0,13733,'ФБС2/ 3й ролик выравнивания');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (12,1,2,'M0:704','M0:45.2',-40.84,19505,'ФБС2/ 1й прижимной ролик, высота предустановки');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (13,1,2,'M0:706','M0:45.3',-33.48,15487,'ФБС2/ 2й прижимной ролик, высота предустановки');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (14,1,2,'M0:708','M0:45.4',-0.08,31,'ФБС2/ 3й прижимной ролик, высота предустановки');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (15,0,3,'M0:150','M0:33.1',100.0,-29377,'PA350/ положение левого блока');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (16,0,3,'M0:154','M0:33.2',100.0,-121974,'PA350/ положение правого блока');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (17,0,3,'M0:166','M0:34.0',100.0,-3432,'PA350/ положение фрезы верхняя левая');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (18,0,3,'M0:170','M0:34.1',100.0,-3278,'PA350/ положение фрезы верхняя правая');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (19,0,3,'M0:158','M0:33.6',100.0,4191,'PA350/ положение фрезы нижняя левая');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (20,0,3,'M0:162','M0:33.7',100.0,4295,'PA350/ положение фрезы нижняя правая');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (21,1,3,'M0:716','M0:46.0',51.75,-1543,'PA350/ предустановка ширины центрирующих роликов на входе');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (22,0,4,'M0:190','M0:36.0',100.0,-29945,'PKA350/ положение левого блока');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (23,0,4,'M0:194','M0:36.1',100.0,-122439,'PKA350/ положение правого блока');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (24,0,4,'M0:182','M0:35.1',100.0,-3144,'PKA350/ положение фрезы верхняя левая');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (25,0,4,'M0:186','M0:35.2',100.0,-3091,'PKA350/ положение фрезы верхняя правая');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (26,0,4,'M0:174','M0:34.7',100.0,4569,'PKA350/ положение фрезы нижняя левая');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (27,0,4,'M0:178','M0:35.0',100.0,4683,'PKA350/ положение фрезы нижняя правая');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (28,0,4,'M0:198','M0:36.5',100.0,-17840,'PKA350/ положние левого внутреннего ролика');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (29,0,4,'M0:202','M0:36.6',100.0,-17770,'PKA350/ положние правого внутреннего ролика');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (30,0,4,'M0:206','M0:37.2',100.0,-10890,'PKA350/ положение левой внешней пилы');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (31,0,4,'M0:210','M0:37.3',100.0,-11450,'PKA350/ положение правой внешней пилы');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (32,1,4,'M0:718','M0:46.1',53.01,-3078,'PKA350/ предустановка ширины роликов на входе');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (33,1,4,'M0:720','M0:46.2',45.07,5932,'PKA350/ предустановка ширины роликов на выходе');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (34,1,4,'M0:722','M0:46.3',28.55,-1700,'PKA350/ высота центра поворотного устройства');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (35,2,4,'M0:730','M0:46.7',0.05,-2,'PKA350/ высота центра левого блока пил');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (36,2,4,'M0:734','M0:47.0',0.05,-2,'PKA350/ высота центра правого блока пил');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (37,2,4,'M0:738','M0:47.1',100.0,5000,'PKA350/ предустановка высоты прижима отделителя досок');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (38,0,5,'M0:214','M0:37.7',100.0,-28400,'PA300/ положение левого блока');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (39,0,5,'M0:218','M0:38.0',100.0,-122194,'PA300/ положение правого блока');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (40,0,5,'M0:230','M0:38.6',100.0,-3081,'PA300/ положение фрезы верхняя левая');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (41,0,5,'M0:234','M0:38.7',100.0,-2916,'PA300/ положение фрезы верхняя правая');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (42,0,5,'M0:222','M0:38.4',100.0,4100,'PA300/ положение фрезы нижняя левая');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (43,0,5,'M0:226','M0:38.5',100.0,4231,'PA300/ положение фрезы нижняя правая');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (44,1,5,'M0:724','M0:46.4',56.18,-3640,'PA300/ предустановка ширины центрирующих роликов на входе');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (45,0,6,'M0:238','M0:29.0',100.0,-40714,'DWS350/ высота 1го прижимного ролика');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (46,0,6,'M0:242','M0:29.1',100.0,-40513,'DWS350/ высота 2го прижимного ролика');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (47,0,6,'M0:246','M0:29.2',100.0,-40979,'DWS350/ высота 3го прижимного ролика');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (48,1,6,'M0:726','M0:46.5',-51.35,34005,'DWS350/ предустановка ширины центрирующих роликов на входе');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (49,1,6,'M0:728','M0:46.6',50.0,0,'DWS350/ предустановка ширины центрирующих роликов на выходе');
INSERT INTO "offsets" ("id","type","zone_id","address","apply_bit","per_mm","offset","description") VALUES (50,2,6,'M0:742','M0:47.2',100.0,0,'DWS350/ позиция осей многопила');
INSERT INTO "motors" ("id","address","description") VALUES (0,'M0:10.0','Автомат поз 3-6');
INSERT INTO "motors" ("id","address","description") VALUES (1,'M0:10.1','Автомат поз 3-6 запущен');
INSERT INTO "motors" ("id","address","description") VALUES (2,'M0:10.2','Бревнотаска поз 6 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (3,'M0:10.3','Автомат поз 8-12');
INSERT INTO "motors" ("id","address","description") VALUES (4,'M0:10.4','Автомат поз 8-12 запущен');
INSERT INTO "motors" ("id","address","description") VALUES (5,'M0:10.5','ФБС1 Фреза слева автомат');
INSERT INTO "motors" ("id","address","description") VALUES (6,'M0:10.6','ФБС1 Фреза справа автомат');
INSERT INTO "motors" ("id","address","description") VALUES (7,'M0:10.7','Поворотные ролики поз 8 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (8,'M0:11.0','Цепная лента поз 8 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (9,'M0:11.1','Ролики выгрузки поз 9 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (10,'M0:11.2','Цепная лента поз 10 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (11,'M0:11.3','Рольганг поз 12 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (12,'M0:11.4','Автомат поз 13-15');
INSERT INTO "motors" ("id","address","description") VALUES (13,'M0:11.5','Автомат поз 13-15 запущен');
INSERT INTO "motors" ("id","address","description") VALUES (14,'M0:11.6','Привод поворотного устройства поз 11 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (15,'M0:11.7','ФБС2 Фреза слева автомат');
INSERT INTO "motors" ("id","address","description") VALUES (16,'M0:12.0','ФБС2 Фреза справа автомат');
INSERT INTO "motors" ("id","address","description") VALUES (17,'M0:12.1','Центрирующие ролики поз 13 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (18,'M0:12.2','Цепная лента поз 13 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (19,'M0:12.3','Ролики выгрузки поз 14 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (20,'M0:12.4','Рольганг поз 15 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (21,'M0:12.5','Автомат поз 16-20');
INSERT INTO "motors" ("id","address","description") VALUES (22,'M0:12.6','Автомат поз 16-20 запущен');
INSERT INTO "motors" ("id","address","description") VALUES (23,'M0:12.7','Центрирующие ролики поз 16 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (24,'M0:13.0','Центрирующие ролики поз 17 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (25,'M0:13.1','Ролики боковых досок поз 17 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (26,'M0:13.2','Ролики выгрузки поз 17 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (27,'M0:13.3','Отделитель досок поз 18 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (28,'M0:13.4','Опорные ролики поворотного устройства поз 19 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (29,'M0:13.5','Привод поворотного устройства поз 19 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (30,'M0:13.6','Рольганг поз 20 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (31,'M0:13.7','Автомат поз 21-24');
INSERT INTO "motors" ("id","address","description") VALUES (32,'M0:14.0','Автомат поз 21-24 запущен');
INSERT INTO "motors" ("id","address","description") VALUES (33,'M0:14.1','Центрирующие ролики поз 21 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (34,'M0:14.2','Центрирующие ролики поз 22 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (35,'M0:14.3','Опорные ролики поз 23 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (36,'M0:14.4','Прижимные ролики поз 23 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (37,'M0:14.5','Ролики выгрузки поз 23 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (38,'M0:14.6','Рольганг поз 24 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (39,'M0:14.7','Автомат лента');
INSERT INTO "motors" ("id","address","description") VALUES (40,'M0:14.0','Автомат лента запущен');
INSERT INTO "motors" ("id","address","description") VALUES (41,'M0:15.1','Автомат гидростанция');
INSERT INTO "motors" ("id","address","description") VALUES (42,'M0:15.2','Автомат гидростанция запущен');
INSERT INTO "motors" ("id","address","description") VALUES (43,'M0:15.3','Поперечный транспортёр боковых досок поз 25 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (44,'M0:15.4','Конвейерная лента боковых досок 1 поз 25 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (45,'M0:15.5','Конвейерная лента боковых досок 2 поз 25 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (46,'M0:15.6','Цепной транспортёр поз 25 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (47,'M0:15.7','Поперечный отвод поз 24 автомат');
INSERT INTO "motors" ("id","address","description") VALUES (48,'M0:16.0','Поворотное устройство поз 19 влево');
INSERT INTO "motors" ("id","address","description") VALUES (49,'M0:16.1','Поворотное устройство поз 19 вправо');
INSERT INTO "motors" ("id","address","description") VALUES (50,'M0:16.2','Поворотное устройство поз 11 влево');
INSERT INTO "motors" ("id","address","description") VALUES (51,'M0:16.3','Поворотное устройство поз 11 вправо');
INSERT INTO "sensors" ("id","address","description") VALUES (0,'I0:72.0','Управление Центрирующие ролики Pos.8');
INSERT INTO "sensors" ("id","address","description") VALUES (1,'I0:72.1','Управление Приёмные ролики Pos.8');
INSERT INTO "sensors" ("id","address","description") VALUES (2,'I0:72.2','Блокировка Перестановка ширины Pos.9');
INSERT INTO "sensors" ("id","address","description") VALUES (3,'I0:73.3','Управление Ролики выгрузки Pos.9');
INSERT INTO "sensors" ("id","address","description") VALUES (4,'I0:73.4','Управление Поворотные ролики впереди Поворотное устройство Pos.11');
INSERT INTO "sensors" ("id","address","description") VALUES (5,'I0:73.5','Тормоза слева Поворотное устройство Pos.11');
INSERT INTO "sensors" ("id","address","description") VALUES (6,'I0:73.6','Исходное положение слева Поворотное устройство Pos.11');
INSERT INTO "sensors" ("id","address","description") VALUES (7,'I0:73.7','Тормоза направо Поворотное устройство Pos.11');
INSERT INTO "sensors" ("id","address","description") VALUES (8,'I0:74.0','Исходное положение направо Поворотное устройство Pos.11');
INSERT INTO "sensors" ("id","address","description") VALUES (9,'I0:74.1','Управление Поворотные ролики сзади Поворотное устройство Pos.11');
INSERT INTO "sensors" ("id","address","description") VALUES (10,'I0:80.1','Управление Центрирующие ролики Pos.13');
INSERT INTO "sensors" ("id","address","description") VALUES (11,'I0:80.2','Управление Приёмные ролики Pos.13');
INSERT INTO "sensors" ("id","address","description") VALUES (12,'I0:80.3','Блокировка Перестановка ширины Pos.14');
INSERT INTO "sensors" ("id","address","description") VALUES (13,'I0:81.4','Блокировка Перестановка ширины Pos.14');
INSERT INTO "sensors" ("id","address","description") VALUES (14,'I0:82.1','Термостат Управление Охлаждение Pos.26');
INSERT INTO "sensors" ("id","address","description") VALUES (15,'I0:82.2','Термостат Аварийное отключение 70C Pos.26');
INSERT INTO "sensors" ("id","address","description") VALUES (16,'I0:82.3','Уровень минимальный уровень заполнения Гидравлика Pos.26');
INSERT INTO "sensors" ("id","address","description") VALUES (17,'I0:82.4','Контакт давления Загрязнение Циркуляционный фильтр Pos.26');
INSERT INTO "sensors" ("id","address","description") VALUES (18,'I0:82.5','Контакт давления Загрязнение Напорный фильтр Pos.26');
INSERT INTO "sensors" ("id","address","description") VALUES (19,'I0:82.6','Контакт давления Такт Смазка 1 Pos.27');
INSERT INTO "sensors" ("id","address","description") VALUES (20,'I0:82.7','Контакт давления Такт Смазка 2 Pos.27');
INSERT INTO "sensors" ("id","address","description") VALUES (21,'I0:83.0','Резерв Напорный фильтр Pos.26');
INSERT INTO "sensors" ("id","address","description") VALUES (22,'I0:83.1','Резерв Напорный фильтр Pos.26');
INSERT INTO "sensors" ("id","address","description") VALUES (23,'I0:88.0','Центрирующие ролики закрывать Pos.16');
INSERT INTO "sensors" ("id","address","description") VALUES (24,'I0:88.1','Центрирующие ролики открывать Pos.16');
INSERT INTO "sensors" ("id","address","description") VALUES (25,'I0:88.2','Центрирующие ролики закрывать Pos.17');
INSERT INTO "sensors" ("id","address","description") VALUES (26,'I0:88.3','Центрирующие ролики открывать Pos.17');
INSERT INTO "sensors" ("id","address","description") VALUES (27,'I0:88.4','Управление Выгрузка Pos.17');
INSERT INTO "sensors" ("id","address","description") VALUES (28,'I0:89.1','Управление Ролики боковых досок Pos.18');
INSERT INTO "sensors" ("id","address","description") VALUES (29,'I0:89.2','Выталкиватель досок Pos.18');
INSERT INTO "sensors" ("id","address","description") VALUES (30,'I0:89.3','Управление Поворотное устройство Pos.19');
INSERT INTO "sensors" ("id","address","description") VALUES (31,'I0:89.4','Поворотное устройство Конечное положение направо Pos.19');
INSERT INTO "sensors" ("id","address","description") VALUES (32,'I0:89.5','Поворотное устройство Конечное положение слева Pos.19');
INSERT INTO "sensors" ("id","address","description") VALUES (33,'I0:89.6','Поворотное устройство Исходное положение Pos.19');
INSERT INTO "sensors" ("id","address","description") VALUES (34,'I0:89.7','Поворотные ролики закрывать Поворотное устройство Pos.19');
INSERT INTO "sensors" ("id","address","description") VALUES (35,'I0:96.1','Центрирующие ролики закрывать Pos.21');
INSERT INTO "sensors" ("id","address","description") VALUES (36,'I0:96.2','Центрирующие ролики открывать Pos.21');
INSERT INTO "sensors" ("id","address","description") VALUES (37,'I0:96.3','Центрирующие ролики закрывать Pos.22');
INSERT INTO "sensors" ("id","address","description") VALUES (38,'I0:96.4','Центрирующие ролики открывать Pos.22');
INSERT INTO "sensors" ("id","address","description") VALUES (39,'I0:96.5','Управлене Ролики выгрузки Pos.23');
INSERT INTO "sensors" ("id","address","description") VALUES (40,'I0:96.6','Управление Поперечный отвод Pos.24');
INSERT INTO "sensors" ("id","address","description") VALUES (41,'I0:96.7','Исходное положене Поперечный отвод Pos.24');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (0,'M0:748',300,0,'задержка прижима роликов выгрузки фбс1 поз 9');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (1,'M0:752',300,0,'задержка разжима роликов выгрузки фбс1 поз 9');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (2,'M0:756',1650,-1500,'задержка прижима поворотных роликов 1 поз 8');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (3,'M0:760',10,0,'задержка разжима поворотных роликов 1 поз 8');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (4,'M0:764',3580,-1580,'задержка прижима поворотных роликов 2 поз 8');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (5,'M0:768',1950,0,'задержка разжима поворотных роликов 2 поз 8');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (6,'M0:772',5200,0,'задержка прижима прижима 1 поз 8');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (7,'M0:776',4900,0,'задержка разжима прижима 1 поз 8');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (8,'M0:780',5520,-1520,'задержка прижима центрирующих роликов поз 8');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (9,'M0:784',1800,0,'задержка выключения прижима центрирующих роликов поз 8');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (10,'M0:788',5300,0,'задержка разжима центрирующих роликов поз 8');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (11,'M0:792',1200,0,'задержка прижима прижима 2 поз 8');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (12,'M0:796',800,0,'задержка разжима прижима 2 поз 8');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (13,'M0:800',6300,-1500,'задержка прижима приемных роликов поз 8');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (14,'M0:804',2000,0,'задержка разжима приемных роликов поз 8');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (15,'M0:808',700,0,'задержка прижима роликов 1 поворотного устройства поз 11');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (16,'M0:812',700,0,'задержка прижима роликов 2 поворотного устройства поз 11');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (17,'M0:816',50,0,'задержка разжима роликов поворотного устройства поз 11');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (18,'M0:820',3000,-2000,'задержка поворота поворотного устройства поз 11');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (19,'M0:824',10,0,'задержка прижима центрирующих роликов 1 поз 13');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (20,'M0:828',10,0,'задержка разжима центрирующих роликов 1 поз 13');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (21,'M0:832',2550,0,'задержка прижима прижима 1 поз 13');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (22,'M0:836',2300,0,'задержка разжима прижима 1 поз 13');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (23,'M0:840',2910,-1110,'задержка прижима центрирующих роликов 2 поз 13');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (24,'M0:844',10,0,'задержка разжима центрирующих роликов 2 поз 13');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (25,'M0:848',4400,0,'задержка прижима прижима 2 поз 13');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (26,'M0:852',4100,0,'задержка разжима прижима 2 поз 13');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (27,'M0:856',5000,-900,'задержка прижима центрирующих роликов 3 поз 13');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (28,'M0:860',2000,0,'задержка выключения прижима центрирующих роликов 3 поз 13');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (29,'M0:864',4800,0,'задержка разжима центрирующих роликов 3 поз 13');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (30,'M0:868',900,0,'задержка прижима прижима 3 поз 13');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (31,'M0:872',700,0,'задержка разжима прижима 3 поз 13');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (32,'M0:876',5500,-1500,'задержка прижима приемных роликов поз 13');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (33,'M0:880',1000,0,'задержка разжима приемных роликов поз 13');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (34,'M0:884',100,0,'задержка прижима роликов выгрузки фбс2 поз 14');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (35,'M0:888',10,0,'задержка разжима роликов выгрузки фбс2 поз 14');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (36,'M0:892',10,0,'задержка прижима центрирующих роликов поз 16');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (37,'M0:896',100,0,'задержка разжима центрирующих роликов поз 16');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (38,'M0:900',10,0,'задержка прижима центрирующих роликов поз 17');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (39,'M0:904',100,0,'задержка разжима центрирующих роликов поз 17');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (40,'M0:908',10,0,'задержка прижима роликов выгрузки поз 17');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (41,'M0:912',100,0,'задержка разжима роликов выгрузки поз 17');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (42,'M0:916',900,0,'задержка прижима роликов боковых досок 1 поз 18');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (43,'M0:920',300,0,'задержка разжима роликов боковых досок 1 поз 18');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (44,'M0:924',2700,0,'задержка прижима роликов боковых досок 2 поз 18');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (45,'M0:928',300,0,'задержка разжима роликов боковых досок 2 поз 18');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (46,'M0:932',4700,0,'задержка прижима роликов боковых досок 3 поз 18');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (47,'M0:936',300,0,'задержка разжима роликов боковых досок 3 поз 18');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (48,'M0:940',6600,0,'задержка прижима роликов боковых досок 4 поз 18');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (49,'M0:944',300,0,'задержка разжима роликов боковых досок 4 поз 18');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (50,'M0:948',8600,0,'задержка прижима роликов боковых досок 5 поз 18');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (51,'M0:952',300,0,'задержка разжима роликов боковых досок 5 поз 18');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (52,'M0:956',10600,0,'задержка прижима роликов боковых досок 6 поз 18');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (53,'M0:960',300,0,'задержка разжима роликов боковых досок 6 поз 18');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (54,'M0:964',11500,-10000,'задержка прижима верхнего прижима поз 18');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (55,'M0:968',1000,0,'задержка разжима верхнего прижима поз 18');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (56,'M0:972',500,0,'задержка прижима выталкивателя досок поз 18');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (57,'M0:976',3500,0,'задержка разжима выталкивателя досок поз 18');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (58,'M0:980',10,0,'задержка прижима роликов поворотного устройства поз 19');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (59,'M0:984',2300,0,'задержка разжима роликов поворотного устройства поз 19');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (60,'M0:988',10,0,'задержка поворота поворотного устройства поз 19');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (61,'M0:992',2000,0,'задержка включения поперечного отвода');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (62,'M0:996',10,0,'задержка прижима центрирующих роликов поз 21');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (63,'M0:1000',100,0,'задержка разжима центрирующих роликов поз 21');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (64,'M0:1004',10,0,'задержка прижима центрирующих роликов поз 22');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (65,'M0:1008',100,0,'задержка разжима центрирующих роликов поз 22');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (66,'M0:1012',950,0,'задержка прижима прижимных роликов 1 поз 23');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (67,'M0:1016',700,0,'задержка разжима прижимных роликов 1 поз 23');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (68,'M0:1020',10,0,'задержка прижима прижимных роликов 2 поз 23');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (69,'M0:1024',10,0,'задержка разжима прижимных роликов 2 поз 23');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (70,'M0:1028',450,0,'задержка прижима прижимных роликов 3 поз 23');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (71,'M0:1032',200,0,'задержка разжима прижимных роликов 3 поз 23');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (72,'M0:1036',1400,0,'задержка прижима роликов выгрузки поз 23');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (73,'M0:1040',1100,0,'задержка разжима роликов выгрузки поз 23');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (74,'M0:1044',700,0,'задержка прижима роликов боковых досок пка350 поз 17');
INSERT INTO "delays" ("id","address","base","offset","description") VALUES (75,'M0:1048',700,0,'задержка разжима роликов боковых досок пка350 поз 17');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (0,'M0:382',3859,0,'Бревнотаска');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (1,'M0:384',3971,0,'Ролики поворотного устройства');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (2,'M0:386',4005,0,'Цепная лента_8');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (3,'M0:388',4346,0,'Ролики выгрузки_9');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (4,'M0:390',3589,0,'Цепная лента_10');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (5,'M0:438',4000,4600,'Входные ролики поворотного устройства');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (6,'M0:392',4313,0,'Рольганг_12');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (7,'M0:394',4267,0,'Центрирующие ролики_13');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (8,'M0:396',4267,0,'Цепная лента_13');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (9,'M0:398',4463,0,'Ролики выгрузки_14');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (10,'M0:400',4425,0,'Рольганг_15');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (11,'M0:402',4122,0,'Центрирующие ролики_16');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (12,'M0:404',4122,0,'Центрирующие ролики_17');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (13,'M0:406',3960,0,'Ролики боковых досок_17');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (14,'M0:408',4412,0,'Ролики выгрузки_17');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (15,'M0:410',4445,0,'Отделитель боковых досок');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (16,'M0:412',4142,0,'Ролики поворотного устройства_19');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (17,'M0:440',2798,4600,'Входные ролики поворотного устройства_19');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (18,'M0:414',4596,0,'Рольганг_20');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (19,'M0:416',4376,0,'Центрирующие ролики_21');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (20,'M0:418',4376,0,'Центрирующие ролики_22');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (21,'M0:420',4168,0,'Ролики_23');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (22,'M0:422',4168,0,'Прижимные ролики_23');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (23,'M0:424',4168,0,'Ролики выгрузки_23');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (24,'M0:426',4895,0,'Рольганг_24');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (25,'M0:436',6144,0,'Сбрасыватель');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (26,'M0:428',4096,0,'Конвейер сбрасывателя');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (27,'M0:430',4096,0,'Конвейер боковых досок_1');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (28,'M0:432',4096,0,'Конвейер боковых досок_2');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (29,'M0:434',4096,0,'Цепной конвейер');
INSERT INTO "speeds" ("id","address","base","offset","description") VALUES (30,'M0:442',2270,0,'Бревнотаска подачи в цех');
INSERT INTO "alarms" ("id","address","description") VALUES (0,'M0:100.0','Бревнотаска со сканером POS6');
INSERT INTO "alarms" ("id","address","description") VALUES (1,'M0:100.1','ФБС1 / Левая фреза POS9');
INSERT INTO "alarms" ("id","address","description") VALUES (2,'M0:100.2','ФБС1 / Правая фреза POS9');
INSERT INTO "alarms" ("id","address","description") VALUES (3,'M0:100.3','Ролики вращения бревна POS8');
INSERT INTO "alarms" ("id","address","description") VALUES (4,'M0:100.4','Цепная лента POS8');
INSERT INTO "alarms" ("id","address","description") VALUES (5,'M0:100.5','ФБС1 / Ролики на выходе POS9');
INSERT INTO "alarms" ("id","address","description") VALUES (6,'M0:100.6','Цепная лента POS10');
INSERT INTO "alarms" ("id","address","description") VALUES (7,'M0:100.7','Рольганг POS12');
INSERT INTO "alarms" ("id","address","description") VALUES (8,'M0:101.0','ФБС2 / Левая фреза POS14');
INSERT INTO "alarms" ("id","address","description") VALUES (9,'M0:101.1','ФБС2 / Правая фреза POS14');
INSERT INTO "alarms" ("id","address","description") VALUES (10,'M0:101.2','Ролики выравнивания лафета POS13');
INSERT INTO "alarms" ("id","address","description") VALUES (11,'M0:101.3','Цепная лента POS13');
INSERT INTO "alarms" ("id","address","description") VALUES (12,'M0:101.4','ФБС2 / Ролики на выходе POS14');
INSERT INTO "alarms" ("id","address","description") VALUES (13,'M0:101.5','Рольганг POS15');
INSERT INTO "alarms" ("id","address","description") VALUES (14,'M0:101.6','PA350 / Центрирующие ролики POS16');
INSERT INTO "alarms" ("id","address","description") VALUES (15,'M0:101.7','PKA350 / Центрирующие ролики POS17');
INSERT INTO "alarms" ("id","address","description") VALUES (16,'M0:102.0','PKA350 / Боковые ролики POS17');
INSERT INTO "alarms" ("id","address","description") VALUES (17,'M0:102.1','PKA350 / Ролики на выходе POS17');
INSERT INTO "alarms" ("id","address","description") VALUES (18,'M0:102.2','Цепь отделителя боковых досок POS18');
INSERT INTO "alarms" ("id","address","description") VALUES (19,'M0:102.3','Опорные ролики поворотного устройства POS19');
INSERT INTO "alarms" ("id","address","description") VALUES (20,'M0:102.4','Рольганг POS20');
INSERT INTO "alarms" ("id","address","description") VALUES (21,'M0:102.5','Центрирующие ролики POS20');
INSERT INTO "alarms" ("id","address","description") VALUES (22,'M0:102.6','Центрирующие ролики POS21');
INSERT INTO "alarms" ("id","address","description") VALUES (23,'M0:102.7','DWS350 / Опорные ролики POS23');
INSERT INTO "alarms" ("id","address","description") VALUES (24,'M0:103.0','DWS350 / Прижимные ролики POS23');
INSERT INTO "alarms" ("id","address","description") VALUES (25,'M0:103.1','DWS350 / Ролики на выходе POS23');
INSERT INTO "alarms" ("id","address","description") VALUES (26,'M0:103.2','Рольганг POS24');
INSERT INTO "alarms" ("id","address","description") VALUES (27,'M0:103.3','Боковые / Поперечный транспортер POS25');
INSERT INTO "alarms" ("id","address","description") VALUES (28,'M0:103.4','Боковые / Ленточный транспортер №1 POS25');
INSERT INTO "alarms" ("id","address","description") VALUES (29,'M0:103.5','Боковые / Ленточный транспортер №2 POS25');
INSERT INTO "alarms" ("id","address","description") VALUES (30,'M0:103.6','Боковые / Цепной транспортер POS25');
INSERT INTO "alarms" ("id","address","description") VALUES (31,'M0:103.7','Поперечный отвод POS24');
INSERT INTO "alarms" ("id","address","description") VALUES (32,'M0:104.0','Привод поворотного устройства POS19');
INSERT INTO "alarms" ("id","address","description") VALUES (33,'M0:104.1','Привод поворотного устройства POS11');
INSERT INTO "alarms" ("id","address","description") VALUES (34,'M0:104.2','Бревнотаска подачи в цех');
COMMIT;
