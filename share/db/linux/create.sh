#!/bin/bash

.  .pgenv

echo createuser
$PSQL -U $USER -d $DB -qf ../sql/create/user.sql

echo createdb
$PSQL -U $USER -d $DB -qf ../sql/create/database.sql

echo extentions
$PSQL -U $USER -qf ../sql/create/extention.sql 
$PSQL -U $USER -qf ../sql/create/cast.sql 
