#include <stdio.h>
#include "minidump.h"

/*
	ע�����Թ����в�����.dmp�ļ�
	��Ҫͨ��debugĿ¼�µ�.exe������Ȼ����logĿ¼�²���.dmp�ļ�
	��logĿ¼��Ҫ�ֶ������������Զ����ɣ�����.exe����ʱ����!!!!!!��
*/

int main()
{
    //ע�����.dmp�ļ���ʼ������
    InitExceptionHandler();

	// ��ĸΪ0
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
