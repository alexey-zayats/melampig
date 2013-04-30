@echo off

call pgenv.bat

echo createuser
%PGDIR%\bin\psql -U %USER% -d %DB% -qf ..\sql\create\user.sql

echo createdb
%PGDIR%\bin\createdb -U %PGUSER% melampig

set PGDATABASE=melampig

echo extention
%PGDIR%\bin\psql -U %USER% -qf ..\sql\create\extention.sql
%PGDIR%\bin\psql -U %USER% -qf ..\sql\create\cast.sql
