// Monitor.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "daemon_instence_t.h"
#include "dahlia_utils.h"
#include <Windows.h>
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

int main(int argc, char* argv[])
{
    //�ж��Ƿ��ǵ���
    if(!is_signal_instence())
    {
        return 0;
    }

    dahlia::daemon_instence_t* daemon = new dahlia::daemon_instence_t;

    while (true)
    {
        Sleep(1000);
    }
	return 0;
}

