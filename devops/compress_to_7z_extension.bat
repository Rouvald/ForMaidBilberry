@echo off

set inputPath=%~1
set outputPath=%~2
set compressionSwitches=%~3

set archivePath=c:\Program Files\7-Zip\7z.exe

:: for different types of compression
::set archiveType=7z
::"%archivePath%" a -t%archiveType% "%outputArchivePath%.%archiveType%" "%toArchivePath%" %compressionSwitches%

"%archivePath%" a -t7z "%outputPath%.7z" "%inputPath%" %compressionSwitches%
