#!/bin/bash

.  .pgenv

echo drop 
$PSQL -qf ../sql/init/drop.sql

echo functions
$PSQL -qf ../sql/init/functions/core.sql
$PSQL -qf ../sql/init/functions/build_fight_list.sql
$PSQL -qf ../sql/init/functions/build_match_fights.sql
$PSQL -qf ../sql/init/functions/build_match_list.sql
$PSQL -qf ../sql/init/functions/fight_update_personal.sql
$PSQL -qf ../sql/init/functions/fight_update_team.sql
$PSQL -qf ../sql/init/functions/fight_log_after_update.sql
$PSQL -qf ../sql/init/functions/generate_classement_points.sql
$PSQL -qf ../sql/init/functions/build_fight_list.sql
$PSQL -qf ../sql/init/functions/match_after_update.sql
$PSQL -qf ../sql/init/functions/remove_competition_fight_match.sql
$PSQL -qf ../sql/init/functions/wrestler_classement.sql	

echo tables
$PSQL -qf ../sql/init/tables/core.sql
$PSQL -qf ../sql/init/tables/melampig.sql

echo data
$PSQL -qf ../sql/init/data/melampig.sql
$PSQL -qf ../sql/init/data/geo.sql

echo indexes
$PSQL -qf ../sql/init/indexes.sql

echo views
$PSQL -qf ../sql/init/views/v_fight_video.sql

