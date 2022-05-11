@echo on

:: make white list to posible Build paths.
:: set inputPaths="c:\UE4_Projects\ForMaidBilberry\Build" "d:\BuildFMB\Build"
set inputPath="c:\UE4_Projects\ForMaidBilberry\Build"
set outputPath=d:\BuildFMB\Build
set compressionSwitches=-mx9 -m0=LZMA2:d1024m:fb64 -ms=1t -mmt=2 -slp

devops/compress_to_7z_extension.bat "%inputPath%" "%outputPath%" "%compressionSwitches%"