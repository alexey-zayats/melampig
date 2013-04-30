@echo off

call pgenv.bat

echo drop
%PGDIR%\bin\psql -qf ..\sql\init\drop.sql

echo functions
%PGDIR%\bin\psql -qf ..\sql\init\functions\core.sql
%PGDIR%\bin\psql -qf ..\sql\init\functions\build_fight_list.sql
%PGDIR%\bin\psql -qf ..\sql\init\functions\build_match_fights.sql
%PGDIR%\bin\psql -qf ..\sql\init\functions\build_match_list.sql
%PGDIR%\bin\psql -qf ..\sql\init\functions\fight_update_personal.sql
%PGDIR%\bin\psql -qf ..\sql\init\functions\fight_update_team.sql
%PGDIR%\bin\psql -qf ..\sql\init\functions\fight_log_after_update.sql
%PGDIR%\bin\psql -qf ..\sql\init\functions\generate_classement_points.sql
%PGDIR%\bin\psql -qf ..\sql\init\functions\build_fight_list.sql
%PGDIR%\bin\psql -qf ..\sql\init\functions\match_after_update.sql
%PGDIR%\bin\psql -qf ..\sql\init\functions\remove_competition_fight_match.sql
%PGDIR%\bin\psql -qf ..\sql\init\functions\wrestler_classement.sql	

echo tables
%PGDIR%\bin\psql -qf ..\sql\init\tables\core.sql
%PGDIR%\bin\psql -qf ..\sql\init\tables\melampig.sql

echo data
%PGDIR%\bin\psql -qf ..\sql\init\data\melampig.sql
%PGDIR%\bin\psql -qf ..\sql\init\data\geo.sql

echo indexes
%PGDIR%\bin\psql -qf ..\sql\init\indexes.sql

echo views
%PGDIR%\bin\psql -qf ..\sql\init\views\v_fight_video.sql
