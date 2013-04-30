
DROP SCHEMA IF EXISTS test CASCADE;

CREATE SCHEMA test;

CREATE TABLE test.lastname (
	title varchar
);

CREATE TABLE test.firstname (
	title varchar
);

CREATE TABLE test.fio (
	lastname varchar,
	firstname varchar
);
