BEGIN TRANSACTION;
DROP TABLE IF EXISTS "delays";
CREATE TABLE IF NOT EXISTS "delays" (
	"id"	INTEGER NOT NULL UNIQUE,
	"zone_id"	INTEGER NOT NULL,
	"address"	TEXT NOT NULL,
	"base"	INTEGER NOT NULL DEFAULT 0,
	"offset"	INTEGER NOT NULL DEFAULT 0,
	"description"	TEXT NOT NULL,
	PRIMARY KEY("id")
);
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (0,1,'M0:748',300,0,'задержка прижима роликов выгрузки фбс1 поз 9');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (1,1,'M0:752',300,0,'задержка разжима роликов выгрузки фбс1 поз 9');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (2,1,'M0:756',1650,-1500,'задержка прижима поворотных роликов 1 поз 8');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (3,1,'M0:760',10,0,'задержка разжима поворотных роликов 1 поз 8');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (4,1,'M0:764',3580,-1580,'задержка прижима поворотных роликов 2 поз 8');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (5,1,'M0:768',1950,0,'задержка разжима поворотных роликов 2 поз 8');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (6,1,'M0:772',5200,0,'задержка прижима прижима 1 поз 8');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (7,1,'M0:776',4900,0,'задержка разжима прижима 1 поз 8');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (8,1,'M0:780',5520,-1520,'задержка прижима центрирующих роликов поз 8');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (9,1,'M0:784',1800,0,'задержка выключения прижима центрирующих роликов поз 8');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (10,1,'M0:788',5300,0,'задержка разжима центрирующих роликов поз 8');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (11,1,'M0:792',1200,0,'задержка прижима прижима 2 поз 8');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (12,1,'M0:796',800,0,'задержка разжима прижима 2 поз 8');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (13,1,'M0:800',6300,-1500,'задержка прижима приемных роликов поз 8');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (14,1,'M0:804',2000,0,'задержка разжима приемных роликов поз 8');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (15,1,'M0:808',700,0,'задержка прижима роликов 1 поворотного устройства поз 11');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (16,1,'M0:812',700,0,'задержка прижима роликов 2 поворотного устройства поз 11');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (17,1,'M0:816',50,0,'задержка разжима роликов поворотного устройства поз 11');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (18,1,'M0:820',3000,-2000,'задержка поворота поворотного устройства поз 11');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (19,2,'M0:824',10,0,'задержка прижима центрирующих роликов 1 поз 13');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (20,2,'M0:828',10,0,'задержка разжима центрирующих роликов 1 поз 13');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (21,2,'M0:832',2550,0,'задержка прижима прижима 1 поз 13');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (22,2,'M0:836',2300,0,'задержка разжима прижима 1 поз 13');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (23,2,'M0:840',2910,-1110,'задержка прижима центрирующих роликов 2 поз 13');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (24,2,'M0:844',10,0,'задержка разжима центрирующих роликов 2 поз 13');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (25,2,'M0:848',4400,0,'задержка прижима прижима 2 поз 13');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (26,2,'M0:852',4100,0,'задержка разжима прижима 2 поз 13');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (27,2,'M0:856',5000,-900,'задержка прижима центрирующих роликов 3 поз 13');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (28,2,'M0:860',2000,0,'задержка выключения прижима центрирующих роликов 3 поз 13');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (29,2,'M0:864',4800,0,'задержка разжима центрирующих роликов 3 поз 13');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (30,2,'M0:868',900,0,'задержка прижима прижима 3 поз 13');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (31,2,'M0:872',700,0,'задержка разжима прижима 3 поз 13');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (32,2,'M0:876',5500,-1500,'задержка прижима приемных роликов поз 13');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (33,2,'M0:880',1000,0,'задержка разжима приемных роликов поз 13');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (34,2,'M0:884',100,0,'задержка прижима роликов выгрузки фбс2 поз 14');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (35,2,'M0:888',10,0,'задержка разжима роликов выгрузки фбс2 поз 14');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (36,3,'M0:892',10,0,'задержка прижима центрирующих роликов поз 16');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (37,3,'M0:896',100,0,'задержка разжима центрирующих роликов поз 16');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (38,3,'M0:900',10,0,'задержка прижима центрирующих роликов поз 17');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (39,3,'M0:904',100,0,'задержка разжима центрирующих роликов поз 17');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (40,3,'M0:908',10,0,'задержка прижима роликов выгрузки поз 17');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (41,3,'M0:912',100,0,'задержка разжима роликов выгрузки поз 17');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (42,3,'M0:916',900,0,'задержка прижима роликов боковых досок 1 поз 18');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (43,3,'M0:920',300,0,'задержка разжима роликов боковых досок 1 поз 18');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (44,3,'M0:924',2700,0,'задержка прижима роликов боковых досок 2 поз 18');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (45,3,'M0:928',300,0,'задержка разжима роликов боковых досок 2 поз 18');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (46,3,'M0:932',4700,0,'задержка прижима роликов боковых досок 3 поз 18');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (47,3,'M0:936',300,0,'задержка разжима роликов боковых досок 3 поз 18');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (48,3,'M0:940',6600,0,'задержка прижима роликов боковых досок 4 поз 18');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (49,3,'M0:944',300,0,'задержка разжима роликов боковых досок 4 поз 18');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (50,3,'M0:948',8600,0,'задержка прижима роликов боковых досок 5 поз 18');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (51,3,'M0:952',300,0,'задержка разжима роликов боковых досок 5 поз 18');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (52,3,'M0:956',10600,0,'задержка прижима роликов боковых досок 6 поз 18');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (53,3,'M0:960',300,0,'задержка разжима роликов боковых досок 6 поз 18');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (54,3,'M0:964',11500,-10000,'задержка прижима верхнего прижима поз 18');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (55,3,'M0:968',1000,0,'задержка разжима верхнего прижима поз 18');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (56,3,'M0:972',500,0,'задержка прижима выталкивателя досок поз 18');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (57,3,'M0:976',3500,0,'задержка разжима выталкивателя досок поз 18');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (58,3,'M0:980',10,0,'задержка прижима роликов поворотного устройства поз 19');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (59,3,'M0:984',2300,0,'задержка разжима роликов поворотного устройства поз 19');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (60,3,'M0:988',10,0,'задержка поворота поворотного устройства поз 19');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (61,4,'M0:992',2000,0,'задержка включения поперечного отвода');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (62,4,'M0:996',10,0,'задержка прижима центрирующих роликов поз 21');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (63,4,'M0:1000',100,0,'задержка разжима центрирующих роликов поз 21');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (64,4,'M0:1004',10,0,'задержка прижима центрирующих роликов поз 22');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (65,4,'M0:1008',100,0,'задержка разжима центрирующих роликов поз 22');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (66,4,'M0:1012',950,0,'задержка прижима прижимных роликов 1 поз 23');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (67,4,'M0:1016',700,0,'задержка разжима прижимных роликов 1 поз 23');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (68,4,'M0:1020',10,0,'задержка прижима прижимных роликов 2 поз 23');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (69,4,'M0:1024',10,0,'задержка разжима прижимных роликов 2 поз 23');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (70,4,'M0:1028',450,0,'задержка прижима прижимных роликов 3 поз 23');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (71,4,'M0:1032',200,0,'задержка разжима прижимных роликов 3 поз 23');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (72,4,'M0:1036',1400,0,'задержка прижима роликов выгрузки поз 23');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (73,4,'M0:1040',1100,0,'задержка разжима роликов выгрузки поз 23');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (74,3,'M0:1044',700,0,'задержка прижима роликов боковых досок пка350 поз 17');
INSERT INTO "delays" ("id","zone_id","address","base","offset","description") VALUES (75,3,'M0:1048',700,0,'задержка разжима роликов боковых досок пка350 поз 17');
COMMIT;
