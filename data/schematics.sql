BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "offsets" (
	"id"	INTEGER NOT NULL UNIQUE,
	"type"	INTEGER NOT NULL DEFAULT 0,
	"zone_id"	INTEGER NOT NULL DEFAULT 0,
	"offset"	INTEGER NOT NULL DEFAULT 0,
	"per_mm"	REAL NOT NULL DEFAULT 100,
	"address"	TEXT NOT NULL DEFAULT "",
	"apply_bit"	TEXT NOT NULL DEFAULT "",
	"description"	TEXT NOT NULL DEFAULT "",
	PRIMARY KEY("id")
);
CREATE TABLE IF NOT EXISTS "config" (
	"name"	TEXT NOT NULL UNIQUE,
	"value"	TEXT NOT NULL,
	PRIMARY KEY("name")
);
COMMIT;
