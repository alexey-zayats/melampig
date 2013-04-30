#!/bin/bash

PGVERSION=`pg_config --version | awk {'print $2'} | awk -F '.' {'print $1"."$2'}`

PGBINDIR=`pg_config --bindir`
SHAREDIR=`pg_config --sharedir`
PSQL=$PGBINDIR/psql

export PGPASSFILE=pgpass
export PGDATABASE=melampig
export PGPORT=5432
export PGUSER=postgres
export PGHOST=localhost
export USER=melampig

echo init
$PSQL -U $USER -qf ../sql/init/drop.sql

echo functions
$PSQL -U $USER -qf ../sql/init/functions/core.sql
$PSQL -U $USER -qf ../sql/init/functions/melampig.sql

echo tables
$PSQL -U $USER -qf ../sql/init/tables/core.sql
$PSQL -U $USER -qf ../sql/init/tables/melampig.sql

echo data
$PSQL -U $USER -qf ../sql/init/data/melampig.sql
$PSQL -U $USER -qf ../sql/init/data/geo.sql

echo indexes
$PSQL -U $USER -qf ../sql/init/indexes.sql

echo test
$PSQL -U $USER -qf ../sql/test/schema.sql
$PSQL -U $USER -qf ../sql/test/firstname.sql
$PSQL -U $USER -qf ../sql/test/lastname.sql
$PSQL -U $USER -qf ../sql/test/generate.sql
$PSQL -U $USER -qf ../sql/test/fio.sql
