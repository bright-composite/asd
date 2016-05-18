@echo off

@set arg="%~1"
		
@if %arg% == "" (
	@set arg="%VS140COMNTOOLS%"
) else (
	@if %arg% == "--build" (
		@goto :build
	)
)

%comspec% /k "%arg%"VsMSBuildCmd" & build-vs --build"
@exit

:build
call cmake-build
@cd "../build-vs-x86"
msbuild ALL_BUILD.vcxproj /p:Configuration=Release
msbuild ALL_BUILD.vcxproj /p:Configuration=Debug
@cd "../build-vs-x64"
msbuild ALL_BUILD.vcxproj /p:Configuration=Release
msbuild ALL_BUILD.vcxproj /p:Configuration=Debug
@cd "../tools"

@pause
@exit