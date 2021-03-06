@echo off
setlocal enabledelayedexpansion

rem -------------------------------------

if x%1 equ x goto usage

set command=%1
shift /1

if "%command%" equ "make" goto make
if "%command%" equ "add" goto add

goto incorrect_usage

rem -------------------------------------

:make
if x%1 equ x goto usage
set list=%1
:append_targets
shift /1

if x%1 equ x (
	call cmake -DBUILD_TARGET=%list% -DWORKSPACE_DIR=%cd% -P %~dp0/tools/asd-make.cmake
	goto end
)

list=!list!;%1
goto append_targets

rem -------------------------------------

:add
if x%1 equ x goto incorrect_usage
if x%2 equ x goto incorrect_usage

set type=%1
set name=%2

if "%type%" equ "module" (
	set append_string=add_module^(%name%^)
	goto append_module
)
if "%type%" equ "test" (
	set append_string=add_module^(%name% tests^)
	goto append_module
)
if "%type%" equ "project" (
	set append_string=add_module^(%name% projects^)
	goto append_module
)
if "%type%" equ "vendor" (
	mkdir ./vendor/%name%
	echo "" > ./vendor/%name%/setup.cmake
	echo "Vendor library %name% successfully added"
	goto end
)

goto incorrect_usage

:append_module
echo %append_string% >> CMakeLists.txt
goto end

rem -------------------------------------

:incorrect_usage
echo Incorrect usage!

:usage
echo Usage:
echo ^* make - asd make ^<target^> ^[targets...^] - make project files for specified targets
echo ^* add - asd add module^|test^|project^|vendor ^<name^>

rem -------------------------------------

:end
