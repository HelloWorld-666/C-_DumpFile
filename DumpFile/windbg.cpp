#include <stdio.h>
#include "minidump.h"

/*
	注：调试过程中不产生.dmp文件
	需要通过debug目录下的.exe启动，然后在log目录下产生.dmp文件
	（log目录需要手动创建，不会自动生成，否则.exe启动时崩溃!!!!!!）
*/

int main()
{
    //注册崩溃.dmp文件初始化设置
    InitExceptionHandler();

	// 分母为0
    int a = 0;
    int b = 5;
    int c = b / a;
    printf("%d\n", c);

	/*int array[1] = {0};
	for (int i = 0; i < 2; i++)
	{
		array[i] = 1;
	}*/

    return 0;
}
