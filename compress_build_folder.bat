@echo off

set toArchivePath=c:\UE4_Projects\ForMaidBilberry\Build\WindowsNoEditor\
set outputArchiveName=Build
set compressionSwitches=-mx9 -m0=LZMA2:d1024m:fb64 -ms=1t -mmt=2 -slp

devops/compress_to_7z_extension.bat "%toArchivePath%" "%outputArchiveName%" "%compressionSwitches%"