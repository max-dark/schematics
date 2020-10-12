BEGIN TRANSACTION;
DROP TABLE IF EXISTS "doors";
CREATE TABLE IF NOT EXISTS "doors" (
	"id"	INTEGER NOT NULL UNIQUE,
	"address"	TEXT NOT NULL,
	"description"	TEXT NOT NULL,
	PRIMARY KEY("id")
);
INSERT INTO "doors" ("id","address","description") VALUES (0,'M0:48.0','ФБС1 дверь слева');
INSERT INTO "doors" ("id","address","description") VALUES (1,'M0:48.1','ФБС1 дверь справа');
INSERT INTO "doors" ("id","address","description") VALUES (2,'M0:48.2','ФБС2 дверь слева');
INSERT INTO "doors" ("id","address","description") VALUES (3,'M0:48.3','ФБС2 дверь справа');
INSERT INTO "doors" ("id","address","description") VALUES (4,'M0:48.4','PKA350 кожух суппорта');
INSERT INTO "doors" ("id","address","description") VALUES (5,'M0:48.5','Платформа  ФБС 1');
INSERT INTO "doors" ("id","address","description") VALUES (6,'M0:48.6','Фиксатор платформы  ФБС 1');
INSERT INTO "doors" ("id","address","description") VALUES (7,'M0:48.7','Платформа ФБС 2');
INSERT INTO "doors" ("id","address","description") VALUES (8,'M0:49.0','Фиксатор платформы  ФБС 1');
INSERT INTO "doors" ("id","address","description") VALUES (9,'M0:49.1','Платформа  PKA350');
COMMIT;
