BEGIN TRANSACTION;
INSERT INTO "config" ("name","value") VALUES ('plc/sab/ip','10.2.22.10');
INSERT INTO "config" ("name","value") VALUES ('plc/kdo/ip','10.2.22.192');
INSERT INTO "config" ("name","value") VALUES ('plc/sab/interval','50');
INSERT INTO "config" ("name","value") VALUES ('plc/kdo/interval','50');
COMMIT;
