echo "更新OBD_PND头文件"
copy E:\Projects\A601\Program\OBD_PND\OBD_PND\resource.h E:\Projects\A601\Program\Include\OBD_PND_Res_resource.h

echo "更新OBD_PND库文件"
e:
cd E:\Projects\A601\Program\OBD_PND\OBD_PND\
cd STANDARDSDK_500 (ARMV4I)
cd Release

echo "更新OBD_PND模拟器文件"
copy *.exe D:\SDCard\V-Checker\OBD_PND

echo "更新OBD_PND发布文件"
copy *.exe E:\Projects\A601\Issue\V-Checker\OBD_PND