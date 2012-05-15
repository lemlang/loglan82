@echo off
:begin
if "%1" == "" goto :end
echo %1
del %1
shift
goto :begin
:end

