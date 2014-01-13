@ECHO OFF

del *.BIN

:********************************************************************
REM 文本编译   /m:高位在前
echo TEXT.TXT is building....							>>error.txt
ToBin20.exe	TEXT_CN.TXT			TEXT_CN.BIN	/m			>>error.txt
:********************************************************************
del *.tmp

echo 更新模拟器目录Bin文件....
copy *.BIN D:\SDCard\V-Checker\OBD_PND

echo 更新发行目录Bin文件....
copy *.BIN E:\Projects\A601\Issue\V-Checker\OBD_PND