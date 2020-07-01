@echo off

set WIX=C:\Users\Sam Haskins\Desktop\Archives\dist\wix

echo Building EnderCalc installer..

copy ..\EnderCalc.exe appdir
copy ..\LICENSE.txt appdir
copy ..\reference.pdf appdir
copy ..\THIRD-PARTY.txt appdir

"%WIX%\heat.exe" dir appdir -ag -srd -cg EnderCalcProgram -dr INSTALLDIR -out EnderCalcProgram.wxs -swall
"%WIX%\candle.exe" -arch x64 EnderCalcProgram.wxs

"%WIX%\candle.exe" -arch x64 EnderCalc.wxs

"%WIX%\light.exe" -b appdir -ext WixUIExtension EnderCalc.wixobj EnderCalcProgram.wixobj -o EnderCalc.msi

del EnderCalc.wixobj EnderCalcProgram.wixobj EnderCalcProgram.wxs EnderCalc.wixpdb