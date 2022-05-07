@echo on

set toArchivePath=%~1
set outputArchiveName=%~2
set compressionSwitches=%~3

set archivePath=c:\Program Files\7-Zip\7z.exe

:: for different types of compression
::set archiveType=7z
::"%archivePath%" a -t%archiveType% "%outputArchiveName%.%archiveType%" "%toArchivePath%" %compressionSwitches%

"%archivePath%" a -t7z "%outputArchiveName%.7z" "%toArchivePath%" %compressionSwitches%
