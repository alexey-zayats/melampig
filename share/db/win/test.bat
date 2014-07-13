@echo off

call pgenv.bat

echo testing

echo schema
%PGDIR%\bin\psql -qf ..\sql\test\schema.sql

echo generator
%PGDIR%\bin\psql -qf ..\sql\test\generate.sql

echo firstname
%PGDIR%\bin\psql -qf ..\sql\test\firstname.sql

echo lastname
%PGDIR%\bin\psql -qf ..\sql\test\lastname.sql

echo fio
%PGDIR%\bin\psql -qf ..\sql\test\fio.sql

echo competition
%PGDIR%\bin\psql -qf ..\sql\test\competition.sql

