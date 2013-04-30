#!/bin/bash

# PGVERSION=`pg_config --version | awk {'print $2'} | awk -F '.' {'print $1"."$2'}`

PGBINDIR=`pg_config --bindir`
SHAREDIR=`pg_config --sharedir`

export PGPASSFILE=pgpass
export PGDATABASE=template1
export PGPORT=5432
export PGUSER=postgres
export PGHOST=localhost

echo dropdb
$PGBINDIR/dropdb -U $PGUSER melampig

echo dropuser
$PGBINDIR/dropuser -U $PGUSER melampig


