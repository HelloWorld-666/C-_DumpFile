# C-_DumpFile
若程序异常退出，则在log目录下生成一个.dump文件

1.生成.dump文件：  
注：调试过程中不产生.dump文件  
需要通过debug目录下的.exe启动，然后在log目录下产生.dmp文件  
（**log目录需要手动创建，不会自动生成，否则.exe启动时崩溃!!!!!!**）  

2.windbg分析dump文件：
见 有道笔记 ~