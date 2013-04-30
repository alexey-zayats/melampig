#!/bin/bash

# PGVERSION=`pg_config --version | awk {'print $2'} | awk -F '.' {'print $1"."$2'}`

PGBINDIR=`pg_config --bindir`
SHAREDIR=`pg_config --sharedir`
PSQL=$PGBINDIR/psql

export PGPASSFILE=pgpass
export PGDATABASE=template1
export PGPORT=5432
export PGUSER=postgres
export PGHOST=localhost
export USER=melampig

echo createuser
$PGBINDIR/psql -U $PGUSER -qf ../sql/create/user.sql

echo createdb
$PGBINDIR/createdb -U $USER melampig

export PGDATABASE=melampig

echo extentions
$PGBINDIR/psql -U $PGUSER melampig -qf ../sql/create/extention.sql 
$PGBINDIR/psql -U $PGUSER melampig -qf ../sql/create/cast.sql 
