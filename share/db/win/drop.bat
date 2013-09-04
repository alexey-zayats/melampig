@echo off

call pgenv.bat

echo dropdb
%PGDIR%\bin\psql -U %USER% -d %DB% -qf ..\sql\drop\database.sql

echo dropuser
%PGDIR%\bin\psql -U %USER% -d %DB% -qf ..\sql\drop\user.sql

