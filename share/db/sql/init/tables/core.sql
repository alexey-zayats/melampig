
CREATE SEQUENCE obj_id_seq;

-- ABSTRACT -- >

CREATE TABLE obj (
	id integer not null default nextval('obj_id_seq'),
	guid uuid not null,
	relname varchar,
	name varchar,
    created timestamp not null default now(),
    updated timestamp not null default now()
);

CREATE TRIGGER _1_not_insert_update BEFORE INSERT OR UPDATE ON obj FOR EACH ROW EXECUTE PROCEDURE not_insert_update();

CREATE TABLE tree (
	pid integer,
	trail ltree not null,
	level integer
) INHERITS (obj);

CREATE TRIGGER _1_not_insert_update BEFORE INSERT OR UPDATE ON tree FOR EACH ROW EXECUTE PROCEDURE not_insert_update();

CREATE TABLE link (
	src integer not null,
	dst integer not null
) inherits (obj);

CREATE TRIGGER _1_not_insert_update BEFORE INSERT OR UPDATE ON link FOR EACH ROW EXECUTE PROCEDURE not_insert_update();

-- < -- ABSTRACT



