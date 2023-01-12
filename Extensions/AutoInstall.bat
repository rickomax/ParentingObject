echo MFX File: %1
echo MFX Destination: %2

set mmfpath=c:\development\clickteam fusion 2.5\
echo MMF2 Std Path: %mmfpath%

copy /y "%1" "%mmfpath%%2"

set mmfpath=
set status=

goto end

:nmfs
echo You don't have MMF2 Standard

:end
