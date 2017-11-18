// OurZZLLStatusComponent.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "NodeMonitorCompoent.h"
#include "IMainServiceControl.h"
#include "daemon_client_t.h"

extern IMainServiceControl* g_MainAttmpter = NULL;
NodeMonitorCompoent * g_Component = NULL;

/************************************************************************
*��  �ƣ��������ʵ������
*˵  �������ͨ���˺��������ʵ����
*��  ������                                                                    
*����ֵ�����������
************************************************************************/
extern "C" __declspec(dllexport) void* CreateComponentInstance()
{
	if (g_Component == NULL)
	{
		g_Component = new NodeMonitorCompoent;
	}
	return g_Component;
}

/************************************************************************
*��  �ƣ��ͷ����ʵ�� 
*˵  ��������˳�ʱ���ô˺���
*��  ������                                                                    
*����ֵ����
************************************************************************/
extern "C" __declspec(dllexport) void DestroyComponentInstance()
{
	if (g_Component != NULL)
	{
		delete g_Component;
		g_Component = NULL;
	}
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� OurZZLLStatusComponent.h
NodeMonitorCompoent::NodeMonitorCompoent()
{
    dahlia::daemon_client_t*   _global_client = new dahlia::daemon_client_t;
    _global_client->create_monitor();
	return;
}

void NodeMonitorCompoent::OnInitialization( const IMainServiceControl* ptrMainServiceControl )
{
	g_MainAttmpter = const_cast<IMainServiceControl*>(ptrMainServiceControl);
	
}

void NodeMonitorCompoent::OnRelease()
{
}

IInterface* NodeMonitorCompoent::QueryInterface( std::string interfaceID )
{

	return NULL;
}

std::string NodeMonitorCompoent::GetComponentID()
{
	return std::string("NodeMonitorCompoentJKLHKLHKJGJTFGVBJFYTFCHGFY");
}

void NodeMonitorCompoent::OnMessage( std::string topic, void* buf, unsigned int length )
{

}
