#!/bin/bash

.  .pgenv

echo testing

echo schema
$PSQL -qf ../sql/test/schema.sql

echo generator
$PSQL -qf ../sql/test/generate.sql

echo firstname
$PSQL -qf ../sql/test/firstname.sql

echo lastname
$PSQL -qf ../sql/test/lastname.sql

echo fio
$PSQL -qf ../sql/test/fio.sql

echo competition
$PSQL -qf ../sql/test/competition.sql

