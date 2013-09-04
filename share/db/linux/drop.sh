#!/bin/bash

.  .pgenv

echo dropdb
$PSQL -U $USER -d $DB -qf ../sql/drop/database.sql

echo dropuser
$PSQL -U $USER -d $DB -qf ../sql/drop/user.sql

