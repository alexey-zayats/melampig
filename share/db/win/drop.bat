@echo off

call pgenv.bat

echo dropdb
%PGDIR%\bin\dropdb -U %USER% melampig

echo dropuser
%PGDIR%\bin\dropuser -U %USER% melampig

