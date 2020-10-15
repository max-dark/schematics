BEGIN TRANSACTION;
DROP TABLE IF EXISTS "speeds";
CREATE TABLE IF NOT EXISTS "speeds" (
	"id"	INTEGER NOT NULL UNIQUE,
	"zone_id"	INTEGER NOT NULL DEFAULT 0,
	"address"	TEXT NOT NULL,
	"base"	INTEGER NOT NULL,
	"offset"	INTEGER NOT NULL,
	"description"	TEXT NOT NULL,
	PRIMARY KEY("id")
);
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (0,0,'M0:382',3859,0,'Бревнотаска');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (1,1,'M0:384',3971,0,'Ролики поворотного устройства');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (2,1,'M0:386',4005,0,'Цепная лента_8');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (3,1,'M0:388',4346,0,'Ролики выгрузки_9');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (4,1,'M0:390',3589,0,'Цепная лента_10');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (5,1,'M0:438',4000,4600,'Входные ролики поворотного устройства');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (6,1,'M0:392',4313,0,'Рольганг_12');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (7,2,'M0:394',4267,0,'Центрирующие ролики_13');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (8,2,'M0:396',4267,0,'Цепная лента_13');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (9,2,'M0:398',4463,0,'Ролики выгрузки_14');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (10,2,'M0:400',4425,0,'Рольганг_15');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (11,3,'M0:402',4122,0,'Центрирующие ролики_16');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (12,3,'M0:404',4122,0,'Центрирующие ролики_17');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (13,3,'M0:406',3960,0,'Ролики боковых досок_17');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (14,3,'M0:408',4412,0,'Ролики выгрузки_17');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (15,3,'M0:410',4445,0,'Отделитель боковых досок');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (16,3,'M0:412',4142,0,'Ролики поворотного устройства_19');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (17,3,'M0:440',2798,4600,'Входные ролики поворотного устройства_19');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (18,4,'M0:414',4596,0,'Рольганг_20');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (19,4,'M0:416',4376,0,'Центрирующие ролики_21');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (20,4,'M0:418',4376,0,'Центрирующие ролики_22');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (21,4,'M0:420',4168,0,'Ролики_23');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (22,4,'M0:422',4168,0,'Прижимные ролики_23');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (23,4,'M0:424',4168,0,'Ролики выгрузки_23');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (24,4,'M0:426',4895,0,'Рольганг_24');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (25,4,'M0:436',6144,0,'Сбрасыватель');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (26,5,'M0:428',4096,0,'Конвейер сбрасывателя');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (27,5,'M0:430',4096,0,'Конвейер боковых досок_1');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (28,5,'M0:432',4096,0,'Конвейер боковых досок_2');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (29,5,'M0:434',4096,0,'Цепной конвейер');
INSERT INTO "speeds" ("id","zone_id","address","base","offset","description") VALUES (30,0,'M0:442',2270,0,'Бревнотаска подачи в цех');
COMMIT;
