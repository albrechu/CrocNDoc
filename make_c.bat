@REM ---------------------------------------------------------------------------------------------
@REM Vectrex C Windows Batch Make Script
@REM Peer Johannsen
@REM contact: vectrex@pforzheim-university.de
@REM ---------------------------------------------------------------------------------------------
@REM some windows batch stuff

@echo off
setlocal enableextensions enabledelayedexpansion

@REM ---------------------------------------------------------------------------------------------
@REM user edit: configure make script
@REM set options to either "yes" or "no"

@REM ENABLE_PREPROCESSING			save output files of the C preprocessor (*.i) separately
@REM ENABLE_C_DEBUGGING				do not pass macro NDEBUG to gcc6809, activate assert() in C
@REM ENABLE_C_OPTIMIZATION			set gcc6809 to either optimization level -O0 or -O2
@REM ENABLE_ASSEMBLY_OPTIMIZATION	activate assembly peephole postprocessing optimization
@REM C_OPTIMIZATION_LEVEL			set desired level of optimization, requires ENABLE_C_OPTIMIZATION=yes

set ENABLE_PREPROCESSING=no
set ENABLE_C_DEBUGGING=yes
set ENABLE_C_OPTIMIZATION=yes
set ENABLE_ASSEMBLY_OPTIMIZATION=yes
set C_OPTIMIZATION_LEVEL=-O3

@REM ---------------------------------------------------------------------------------------------
@REM chose emulator

@REM set EMULATOR=parajve
@REM set EMULATOR=vectrex_online	EXPERIMENTAL!
@REM set EMULATOR=jsvecx			EXPERIMENTAL!

set EMULATOR=parajve

@REM ---------------------------------------------------------------------------------------------
@REM path configuration, do not edit

set TOOLS_PATH=..\..\tools

set GCC_PATH=%TOOLS_PATH%\gcc6809
set GCC=%GCC_PATH%\bin\gcc6809.exe

set AS6809_PATH=%TOOLS_PATH%\as6809
set AS6809=%AS6809_PATH%\as6809_550.exe
set ASLINK=%AS6809_PATH%\aslink_550.exe

set LWTOOLS_PATH=%TOOLS_PATH%\lwtools\bin
set LWASM=%LWTOOLS_PATH%\lwasm.exe
set LWASM_FLAGS=--tabs=4 -fobj
set LWLINK=%LWTOOLS_PATH%\lwlink.exe

set RXREPL_PATH=%TOOLS_PATH%\rxrepl
set RXREPL=%RXREPL_PATH%\rxrepl.exe

set SREC2BIN_PATH=%TOOLS_PATH%\srec2bin
set SREC2BIN=%SREC2BIN_PATH%\srec2bin.exe

set FINDSTR_PATH=%TOOLS_PATH%\findstr
set FINDSTR=%FINDSTR_PATH%\findstr.exe

if not exist ".\build\rel" (
	mkdir ".\build\rel"
)

if not exist ".\build\bin" (
	mkdir ".\build\bin"
)

@REM ---------------------------------------------------------------------------------------------
@REM configure gcc flags, do not edit

set GCC_FLAGS_C=-std=gnu99
if [%ENABLE_C_DEBUGGING%]==[no] (
	set GCC_FLAGS_C=%GCC_FLAGS_C% -D NDEBUG
)

set GCC_FLAGS_GENERAL=-quiet -fno-time-report
set GCC_FLAGS_6809=-fomit-frame-pointer -fno-toplevel-reorder -mint8 -msoft-reg-count=0 -funit-at-a-time
set GCC_FLAGS_OPTIMIZE=-fno-ipa-reference -ftree-ter -fno-gcse
set GCC_FLAGS_WARN=-W -Wall -Werror -Wextra -Wlogical-op -Wdisabled-optimization -Wmissing-noreturn -Wpadded -Wredundant-decls -Winline  -Wstrict-overflow=5 -Wconversion -Winit-self -Wswitch-default -Wswitch-enum -Wno-error=switch -Wstrict-aliasing=0
set GCC_FLAGS= %GCC_FLAGS_GENERAL% %GCC_FLAGS_C% %GCC_FLAGS_6809% %GCC_FLAGS_WARN% %GCC_FLAGS_OPTIMIZE% 

set GCC_INC= -I .\include -I %GCC_PATH%\vectrex\include\system -I %GCC_PATH%\vectrex\include\lib -I .\src -I .\src\lib
for /d %%D in (.\src\lib\*) do (
	set GCC_INC=!GCC_INC! -I .\src\lib\%%~nxD
)

@REM ---------------------------------------------------------------------------------------------
@REM configure linker, do not edit

set LINK_PATH=-k %GCC_PATH%\vectrex\lib\system\ -k %GCC_PATH%\vectrex\lib\lib\ -k .\build\lib\
set LINK_LIBS=-l rum.lib -l gcc.lib -l libgcc.lib
set LINK_STATIC=%GCC_PATH%\vectrex\lib\system\static\*.rel

@REM ---------------------------------------------------------------------------------------------
@REM memory layout, do not edit

set ROM_BASE=0x0000
set ROM_SIZE=0x8000
set ROM_FILL=0xff
set FILL_ROM=no

set RAM_BASE=0xC880
set RAM_SIZE=0x036B
set RAM_FILL=0xff
set FILL_RAM=no

@REM ---------------------------------------------------------------------------------------------
@REM evaluate script parameter, do not edit

set TARGET=%1
if [%TARGET%] == [] (
	set TARGET=build
)

set OPT=%2
if [%OPT%] == [] (
	if [%ENABLE_C_OPTIMIZATION%] == [yes] (
		set OPT=%C_OPTIMIZATION_LEVEL%
	) else (
		set OPT=-O0
	)
)

@REM ---------------------------------------------------------------------------------------------
@REM call main target, do not edit

call :main %TARGET% %OPT%
exit /B %ERRORLEVEL%

@REM ---------------------------------------------------------------------------------------------
@REM auxiliary subprocedures, do not edit

:find_source_files - path pattern
    set source_paths=
    set source_files=
	set files=
    for /r "%~1" %%P in ("%~2") do (
        set source_paths=!source_paths! "%%~fP"
        set source_files=!source_files! "%%~nP%%~xP"
		set files=!files! "%%~nP"
    )
goto :eof

:separator
	echo *********************************************************************************
exit /B 0

:line
	echo ---------------------------------------------------------------------------------
exit /B 0

:remove - FILE
	set FILE=%1
	if exist %FILE% (
		echo removing %FILE%
		del %FILE% 
	)
exit /B 0

:wipe - FOLDER
	set FOLDER=%1
	call :remove .\%FOLDER%\*.i
	call :remove .\%FOLDER%\*.s
	call :remove .\%FOLDER%\*.lst
	call :remove .\%FOLDER%\*.rel
	call :remove .\%FOLDER%\*.rst
	call :remove .\%FOLDER%\*.hlr
	call :remove .\%FOLDER%\*.map
	call :remove .\%FOLDER%\*.s19
	call :remove .\%FOLDER%\*.bin
exit /B 0

:getFilesize - FILE
	set fileSize=%~z1
exit /B

:copy - PROJECT
	set PROJECT=%1
	echo copying...
	copy .\build\%PROJECT%.bin C:\temp\%PROJECT%.bin || exit \b
exit /B 0

@REM ---------------------------------------------------------------------------------------------
@REM make target subprocedures, do not edit

:clean
	call :wipe build
	call :wipe build\bin
	call :wipe build\rel
	rmdir /s /q build\lib
	mkdir build\lib
	rmdir /s /q build\src
	mkdir build\src
exit /B 0

:preprocess - SOURCE DEST GCC_OPT
	setlocal enableextensions enabledelayedexpansion
	set FILE=%~n1
	set SOURCE=%1
	set DEST=%2
	set GCC_OPT=-E %~3
	echo preprocessing %FILE%.c
	%GCC% %GCC_INC% %GCC_FLAGS% %GCC_OPT% -o %DEST%\%FILE%.i %SOURCE% || exit \b
exit /B 0

:compile - REL_FILE REL_DEST GCC_OPT REL_PATH
	setlocal enableextensions enabledelayedexpansion
	set CFILE=%1
	set CDEST=%2
	set COPT=%3
	set CPATH=%4
	if not [%CPATH%]==[] (
		if not exist %CDEST%\%CPATH% (
			echo creating directory %CDEST%\%CPATH%
			mkdir %CDEST%\%CPATH%
		)
	)
	if [%ENABLE_PREPROCESSING%]==[yes] (
		echo preprocessing %CPATH%%CFILE%.c
		%GCC% %GCC_INC% %GCC_FLAGS% %COPT% -E -o %CDEST%\%CPATH%%CFILE%.i src\%CPATH%%CFILE%.c || exit \b
	)
	echo compiling %CPATH%%CFILE%.c
	%GCC% %GCC_INC% %GCC_FLAGS% %COPT% -o %CDEST%\%CPATH%%CFILE%.s src\%CPATH%%CFILE%.c || exit \b
exit /B 0

:transfer - REL_FILE REL_DEST GCC_OPT REL_PATH
	setlocal enableextensions enabledelayedexpansion
	set CFILE=%1
	set CDEST=%2
	set COPT=%3
	set CPATH=%4
	if not [%CPATH%]==[] (
		if not exist %CDEST%\%CPATH% (
			echo creating directory %CDEST%\%CPATH%
			mkdir %CDEST%\%CPATH%
		)
	)
	echo transferring %CPATH%%CFILE%.s 
	@copy src\%CPATH%%CFILE%.s %CDEST%\%CPATH%%CFILE%.s || exit \b
exit /B 0

:optimize - FILE FOLDER
	setlocal enableextensions enabledelayedexpansion
	set FILE=%1
	set FOLDER=%2
	%RXREPL% --no-backup --return-count -f .\%FOLDER%\%FILE%.s -a  --options %RXREPL_PATH%\rules_optimize.txt
	echo %ERRORLEVEL% line(s) optimized in %FILE%.s
exit /B %ERRORLEVEL%

:assemble - SOURCE DEST FILE PATH PREF
	set FROM=%1
	set DEST=%2
	set FILE=%3
	set PATH=%4
	set PREF=%5
	if not [%PREF%]==[] (
		set PREF=!PREF:/=_!
		set PREF=!PREF:\=_!
	)
	echo assembling %PATH%%FILE%.s
	%AS6809% -x -f -p -l -g -o+.\%DEST%\%PREF%%FILE%.rel .\%FROM%\%PATH%%FILE%.s || exit \b
exit /B 0

:link - PROJECT FILES
	setlocal enabledelayedexpansion
	set PROJECT=%1
	set FILES=empty
	set RFILES=empty
	for %%F in (.\build\rel\*.rel) do (
		if !FILES!==empty (
			set FILES=%%~nF.rel
		) else (
			set FILES=!FILES! %%~nF.rel
		)
		if !RFILES!==empty (
			set RFILES=.\build\rel\%%~nF.rel
		) else (
			set RFILES=!RFILES! .\build\rel\%%~nF.rel
		)
	)
	set LFILES=empty
	for %%F in (%GCC_PATH%\vectrex\lib\system\static\*.rel) do (
		if !LFILES!==empty (
			set LFILES=%GCC_PATH%\vectrex\lib\system\static\%%~nF.rel
		) else (
			set LFILES=!LFILES! %GCC_PATH%\vectrex\lib\system\static\%%~nF.rel
		)
	)

	echo linking !FILES!

	for %%F in (.\build\lib\*.lib) do (
		set LINK_LIBS=-l %%~nF.lib !LINK_LIBS!
	)
	echo with libraries !LINK_LIBS!
	%ASLINK% -n -m -x -u -w -s %LINK_PATH% %LINK_LIBS% .\build\rel\cnd_config_memory_map.rel !LFILES! !RFILES! 1>nul || exit \b
exit /B 0

@REM ---------------------------------------------------------------------------------------------
@REM build subprocedures, do not edit

:build - PROJECT
	setlocal enabledelayedexpansion
	set PROJECT=%1

	if [%FILL_ROM%]==[yes] (
		%SREC2BIN% -q -o %ROM_BASE% -a %ROM_SIZE% -f %ROM_FILL% .\build\rel\*_rom.s19 .\build\%PROJECT%_rom.bin || exit \b
	) else (
		%SREC2BIN% -q -o %ROM_BASE% .\build\rel\*_rom.s19 .\build\%PROJECT%_rom.bin || exit \b
	)
	call :getFilesize .\build\%PROJECT%_rom.bin
	echo rom section size is !fileSize! bytes [code + global rom variables]

	if exist .\build\rel\*_ram.s19 (
		%SREC2BIN% -q -o %RAM_BASE% .\build\rel\*_ram.s19 .\build\%PROJECT%_ram.bin || exit \b
		call :getFilesize .\build\%PROJECT%_ram.bin
		echo ram section size is !fileSize! bytes [global ram variables]
	)

	@copy .\build\rel\*.map .\build\%PROJECT%.map 1>nul || exit \b

	call :line

	%FINDSTR% "\.cartridge" build\%PROJECT%.map
	%FINDSTR% "\.boot" build\%PROJECT%.map
	%FINDSTR% "\.text" build\%PROJECT%.map
	%FINDSTR% "\.data" build\%PROJECT%.map
	%FINDSTR% "\.bss" build\%PROJECT%.map
	%FINDSTR% "\.rom" build\%PROJECT%.map

	call :line

	echo building %PROJECT%.bin
	if exist .\build\%PROJECT%_ram.bin (
		@copy /b .\build\%PROJECT%_rom.bin+.\build\%PROJECT%_ram.bin .\build\bin\%PROJECT%.bin 1>nul || exit \b
	) else (
		@copy /b .\build\%PROJECT%_rom.bin .\build\bin\%PROJECT%.bin 1>nul || exit \b
	)

	@copy /b .\build\%PROJECT%.map .\build\bin\%PROJECT%.lst 1>nul || exit \b
	%RXREPL% --no-backup --return-count -f .\build\bin\%PROJECT%.lst -a --options %RXREPL_PATH%\rules_labels.txt

	call :getFilesize .\build\bin\%PROJECT%.bin
	@echo bin size is !fileSize! bytes
exit /B 0

@REM ---------------------------------------------------------------------------------------------
@REM make target procedures, do not edit

:make_clean - PROJECT
	set PROJECT=%1
	call :separator
	echo cleaning project %PROJECT% ...
	call :clean
exit /B 0

:make_compile - PROJECT OPT
	setlocal enableextensions enabledelayedexpansion
	set PROJECT=%1
	set OPT=%2
	call :make_clean %PROJECT% %OPT%
	call :separator
	echo compiling project %PROJECT% ...
	for /R src %%F in (*.c) do (
		call :line
		set DRIVE=%%~dF
		set ABS_PATH=!DRIVE!%%~pF
		set REL_PATH=!ABS_PATH:%CD%\src\=!
		set ABS_FILE=%%~nF
		set REL_FILE=!ABS_FILE:%CD%\src\=!
		call :compile !REL_FILE! build\src %OPT% !REL_PATH! 
	)
	for /R src %%F in (*.s) do (
		call :line
		set DRIVE=%%~dF
		set ABS_PATH=!DRIVE!%%~pF
		set REL_PATH=!ABS_PATH:%CD%\src\=!
		set ABS_FILE=%%~nF
		set REL_FILE=!ABS_FILE:%CD%\src\=!
		call :transfer !REL_FILE! build\src %OPT% !REL_PATH! 
	)
	if [%ENABLE_ASSEMBLY_OPTIMIZATION%]==[yes] (
		set ASS_OPT=0
		call :separator
		echo optimizing project %PROJECT% ...
		call :line
		for /R build\src %%F in (*.s) do (
			set DRIVE=%%~dF
			set ABS_PATH=!DRIVE!%%~pF
			set REL_PATH=!ABS_PATH:%CD%\build\src\=!
			set ABS_FILE=%%~nF
			set REL_FILE=!ABS_FILE:%CD%\build\src\=!
			call :optimize !REL_PATH!!REL_FILE! build\src
			set /a "ASS_OPT=!ASS_OPT! + !ERRORLEVEL!"
		)
		call :line
		if !ASS_OPT! == 1 (
			echo !ASS_OPT! line optimized in all files
		) else (
			echo !ASS_OPT! lines optimized in all files
		)
	)
exit /B 0

:make_assemble - PROJECT OPT
	set PROJECT=%1
	set OPT=%2
	call :make_compile %PROJECT% %OPT%
	if exist build/src/lib (
		call :separator
		echo assembling libraries of project %PROJECT% ...
		rmdir /s /q build\lib
		@move build/src/lib build 1>nul || exit \b
		call :separator
		for /R build/lib %%F in (*.s) do (
			call :line
			set DRIVE=%%~dF
			set ABS_PATH=!DRIVE!%%~pF
			set REL_PATH=!ABS_PATH:%CD%\build\lib\=!
			set ABS_FILE=%%~nF
			set REL_FILE=!ABS_FILE:%CD%\build\lib\=!
			call :assemble build/lib build/lib/!REL_PATH! !REL_FILE! !REL_PATH! 

			if not [!REL_PATH!]==[] (
				set REL_PATH=!REL_PATH:/=!
				set REL_PATH=!REL_PATH:\=!
			)
			echo \!REL_PATH!\!REL_FILE!.rel >> .\build\lib\!REL_PATH!.lib
		)
	)
	call :separator
	echo assembling source of project %PROJECT% ...
	for /R build/src %%F in (*.s) do (
		call :line
		set DRIVE=%%~dF
		set ABS_PATH=!DRIVE!%%~pF
		set REL_PATH=!ABS_PATH:%CD%\build\src\=!
		set ABS_FILE=%%~nF
		set REL_FILE=!ABS_FILE:%CD%\build\src\=!
		call :assemble build/src build/rel !REL_FILE! !REL_PATH! !REL_PATH!
	)
exit /B 0

:make_link - PROJECT OPT
	set PROJECT=%1
	set OPT=%2
	call :make_assemble %PROJECT% %OPT%
	call :separator
	echo linking project %PROJECT% ...
	call :link %PROJECT%
exit /B 0

:make_build - PROJECT OPT
	set PROJECT=%1
	set OPT=%2
	call :make_link %PROJECT% %OPT%
	call :separator
	echo building project %PROJECT% ...
	call :build %PROJECT%
exit /B 0

@REM ---------------------------------------------------------------------------------------------
@REM epxerimental linting script for ccpcheck

:make_lint - PROJECT
	set PROJECT=%1
	set FILES=
	call :separator
	echo linting project %PROJECT% ...
	start /B /W C:\Programme\Cppcheck\cppcheck -j 1 --quiet -D CPPCHECK --platform=vectrex --language=c --std=c99 --template=gcc --enable=all --suppress=arithOperationsOnVoidPointer --suppress=unusedLabel -I %GCC_PATH%\vectrex\include\system -I %GCC_PATH%\vectrex\include\lib .\src
	REM start /B /W C:\Programme\Cppcheck\cppcheck -j 1 --quiet -D CPPCHECK -D INLINE_RUM=0 --platform=vectrex --language=c --std=c99 --enable=warning,style,performance,portability,information,unusedFunction,missingInclude --template=gcc --suppress=arithOperationsOnVoidPointer --suppress=unusedLabel -I %GCC%\vectrex\include\system -I %GCC%\vectrex\include\lib %GCC%\vectrex\source .\source
	REM start /B /W C:\Programme\Cppcheck\cppcheck --std=c99 --quiet -j 1 -D CPPCHECK -D INLINE_RUM=0 --platform=vectrex --enable=all --template=gcc --suppress=arithOperationsOnVoidPointer -I %GCC%\vectrex\include\system -I %GCC%\vectrex\include\lib .\source
	REM start /B /W C:\Programme\Cppcheck\cppcheck -j 1 -D INLINE_RUM=0 --platform=vectrex --language=c --enable=unusedFunction --std=c99 --template=gcc --suppress=arithOperationsOnVoidPointer -I %GCC%\vectrex\include\system -I %GCC%\vectrex\include\lib .\source\level.c
	REM FOR /L %%I IN (1,1,99999) DO REM adapt delay loop if output is interleaved
exit /B 0

@REM ---------------------------------------------------------------------------------------------
@REM run script for ParaJVE

:make_run_vectrex_online - PROJECT
	start "%PROJECT%" ..\..\..\Emulators\VectrexEmulator_0_2_6_Alpha\VectrexOnline.exe .\build\bin\%PROJECT%.bin
exit /B 0

:make_run_jsvecx - PROJECT
	echo %~dp0
	start "%PROJECT%" https://drsnuggles.github.io/jsvecx/?rom=file:///%cd%/bin/%PROJECT%.bin
exit /B 0

:make_run_parajve - PROJECT
	set PROJECT=%1
	call :separator
	@del ..\..\ParaJVE\project\project.bin
	@copy .\build\bin\%PROJECT%.bin ..\..\ParaJVE\project\project.bin 1>nul || exit \b
	@del ..\..\ParaJVE\project\project.png
	@copy .\overlay\%PROJECT%.png ..\..\ParaJVE\project\project.png 1>nul
	@del ..\..\ParaJVE\project\project.pdf
	@copy .\manual\%PROJECT%.pdf ..\..\ParaJVE\project\project.pdf 1>nul
	echo running project %PROJECT% ...
	start "%PROJECT%" /D ..\..\ParaJVE ParaJVE.exe -game="PROJECT" -config="project/configuration.xml"
exit /B 0

:make_run - PROJECT
	echo on
	set PROJECT=%1
	if %EMULATOR% == parajve (
		@call :make_run_parajve %PROJECT%
	) else if %EMULATOR% == vectrex_online (
		@call :make_run_vectrex_online %PROJECT%
	) else if %EMULATOR% == jsvecx (
		@call :make_run_jsvecx %PROJECT%
	)
exit /B 0

@REM ---------------------------------------------------------------------------------------------
@REM make targets, do not edit

:make - TARGET PROJECT OPT
	set TARGET=%1
	set PROJECT=%2
	set "OPT=%3"
	@if %TARGET% == clean (
		@call :make_clean %PROJECT%
	) else if %TARGET% == preprocess (
		@call :make_preprocess %PROJECT% %OPT%
	) else if %TARGET% == compile (
		@call :make_compile %PROJECT% %OPT%
	) else if %TARGET% == optimize (
		@call :make_optimize %PROJECT% %OPT%
	) else if %TARGET% == assemble (
		@call :make_assemble %PROJECT% %OPT%
	) else if %TARGET% == link (
		@call :make_link %PROJECT% %OPT%
	) else if %TARGET% == build (
		@call :make_build %PROJECT% %OPT%
	) else if %TARGET% == lint (
		@call :make_lint %PROJECT%
	) else if %TARGET% == run (
		@call :make_run %PROJECT%
	) else (
		@echo ERROR - unknown target: clean, preprocess, compile, optimize, assemble, link, build, lint, run
		@exit /B 1
	)
	call :separator
	echo done
	call :separator
exit /B 0

@REM ---------------------------------------------------------------------------------------------
@REM main procedure, do not edit

:main - TARGET OPT
	set TARGET=%1
	if [%TARGET%] == [] (
		set TARGET=build
	)
	set OPT=%2
	if [%OPT%] == [] (
		set "OPT=-O0"
	)
	set PROJECT=
	for %%i in (.) do @set PROJECT=%%~ni
	if [%PROJECT%] == [] (
		@echo ERROR - could not determine project name
		@exit /B 1		
	) else (
		call :make %TARGET% %PROJECT% %OPT%
	)
exit /B 0

@REM ---------------------------------------------------------------------------------------------

