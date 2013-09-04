@echo off

call pgenv.bat

echo %USER% %DB%

echo createuser
%PGDIR%\bin\psql -U %USER% -d %DB% -qf ..\sql\create\user.sql

echo createdb
%PGDIR%\bin\psql -U %USER% -d %DB% -qf ..\sql\create\database.sql

set PGDATABASE=melampig

echo extention
%PGDIR%\bin\psql -U %USER% -f ..\sql\create\extention.sql
%PGDIR%\bin\psql -U %USER% -f ..\sql\create\cast.sql
