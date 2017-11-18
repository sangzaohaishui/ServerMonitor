#pragma once

#include <string>
using namespace std;


#ifdef DDS_EXPORTS
	#define DDS_API extern "C" __declspec(dllexport)
#else
	#define DDS_API extern "C" __declspec(dllimport)
#endif

//8 bytes struct member alignment
#pragma pack( push, enter_include1 )
#pragma pack(8)

#ifndef CALLBACK
	#define CALLBACK __stdcall
#endif

//////////////////////////////////////////////////////////////////////

//�ַ���
typedef string							DDS_STRING;

//������
typedef unsigned long					DDS_COMPONENT_NUMBER;

//////////////////////////////////////////////////////////////////////

//���ݽ��ջص�����ԭ��
//ucFuncCallMode		(����)	�ص���������ģʽ
//								0: �첽����(���ߵ��ûص��������߳������߽������ݵ��̲߳���ͬһ��)
//								1: ͬ������(���ߵ��ûص��������߳������߽������ݵ��߳�Ϊͬһ��)
//								3: ��������(�û��������ýӿڣ�������������)
//								������Ԥ������ֵ
//domainName			(����)������
//topicName				(����)��������
//componentNumber		(����)������
//pBuf					(����)���ݻ�����
//ulBufSize				(����)���ݻ���������
//pStatusBuf			(����)״̬������
//ulStatusBufSize		(����)״̬����������
typedef void (CALLBACK* DDS_PFN_RECV_DATA)(unsigned char				ucFuncCallMode, 
										   DDS_STRING					domainName, 
										   DDS_STRING					topicName, 
										   const DDS_COMPONENT_NUMBER	&componentNumber, 
										   const char					*pBuf, 
										   unsigned long				ulBufSize, 
										   const char					*pStatusBuf, 
										   unsigned long				ulStatusBufSize);

//////////////////////////////////////////////////////////////////////
//
// ����ֵ˵����
// �����ر�˵�����ӿں��������� bool ���ͣ�true ��ʾ�ɹ���false ��ʾʧ��
//
// ������Ϣ����˵����
// ��ܵ��ýӿں���ʧ��ʱ����ͨ�� FrameGetLastError�� FrameGetErrorMsg �� FrameGetLastErrorMsg ��ȡ������Ϣ
// ������ýӿں���ʧ��ʱ����ͨ�� ComponentGetLastError�� ComponentGetErrorMsg �� ComponentGetLastErrorMsg ��ȡ������Ϣ
//
//////////////////////////////////////////////////////////////////////

//�����ʶ
struct sComponentKey
{
	//������
	DDS_STRING				m_strDomainName;

	//������
	DDS_COMPONENT_NUMBER	m_componentNumber;


	sComponentKey() : m_strDomainName(""), m_componentNumber(0)
	{
	}

	sComponentKey(DDS_STRING strDomainName, 
		DDS_COMPONENT_NUMBER componentNumber) : 
		m_strDomainName(strDomainName), m_componentNumber(componentNumber)
	{
	}

	sComponentKey(const sComponentKey& r)
	{
		*this = r;
	}

	sComponentKey& operator=(const sComponentKey& r)
	{
		if (this == &r)
		{
			return (*this);
		}

		m_strDomainName		= r.m_strDomainName;
		m_componentNumber	= r.m_componentNumber;

		return (*this);
	}

	bool operator==(const sComponentKey& r) const
	{
		if (m_strDomainName	== r.m_strDomainName &&
			m_componentNumber == r.m_componentNumber)
		{
			return true;
		} else {
			return false;
		}
	}

	bool operator<(const sComponentKey& r) const
	{
		//��һά
		if (m_strDomainName	< r.m_strDomainName)
		{
			return true;
		} else if (m_strDomainName > r.m_strDomainName) {
			return false;
		} else {
			//�ڶ�ά
			if (m_componentNumber < r.m_componentNumber)
			{
				return true;
			} else if (m_componentNumber > r.m_componentNumber) {
				return false;
			} else {
				return false;
			}
		}
	}
};

//////////////////////////////////////////////////////////////////////
//
//���ʹ�õ����߽ӿ�
//
//////////////////////////////////////////////////////////////////////

//��ܳ�ʼ������
//strLocalAppName			(����)����Ӧ��(����)���ƣ��������32�ֽ�
//strLocalSeatName			(����)����ϯλ(����)���ƣ��������32�ֽ�
//strInterfaceType			(����)�ӿ������ַ������������32�ֽ�
//ucHighVersion				(����)�߰汾��
//ucLowVersion				(����)�Ͱ汾��
DDS_API 
bool FrameInitBus(DDS_STRING strLocalAppName, 
				  DDS_STRING strLocalSeatName = "",	
				  DDS_STRING strInterfaceType = "WS_ES_8CA_IDOMAIN_1", 
				  unsigned char ucHighVersion = 0, 
				  unsigned char ucLowVersion = 1);

//����˳�����
DDS_API 
bool FrameExitBus(void);

//////////////////////////////////////////////////////////////////////

//��ܼ�����
//˵������Ҫʱ����ͨ����ͬ������ε��ñ��ӿڣ���ͬһ��ܼ�������
//strDomainName				(����)�����ƣ��������32�ֽ�
DDS_API 
bool FrameJoinDomain(DDS_STRING strDomainName);

//����뿪��
//strDomainName				(����)�����ƣ��������32�ֽ�
DDS_API 
bool FrameLeaveDomain(DDS_STRING strDomainName);

//////////////////////////////////////////////////////////////////////

//��ȡ���´����
//strDomainName				(����)�����ƣ��������32�ֽ�
DDS_API 
unsigned long FrameGetLastError(DDS_STRING strDomainName);

//��ȡ�����ı�����
//strDomainName				(����)�����ƣ��������32�ֽ�
//ulErrorNumber				(����)����ţ��������32�ֽ�
//strErrorText				(���)�����ı�����
DDS_API 
bool FrameGetErrorMsg(DDS_STRING strDomainName, 
					  unsigned long ulErrorNumber, 
					  DDS_STRING &strErrorText);

//��ȡ���´����ı�����
//strDomainName				(����)�����ƣ��������32�ֽ�
//strErrorText				(���)�����ı�����
DDS_API 
bool FrameGetLastErrorMsg(DDS_STRING strDomainName, 
						  DDS_STRING &strErrorText);


//////////////////////////////////////////////////////////////////////
//
//���ʹ�õ����߽ӿ�
//
//////////////////////////////////////////////////////////////////////

//���������
//˵������Ҫʱ����ͨ����ͬ������ε��ñ��ӿڣ���ͬһ�����������
//strDomainName				(����)�����ƣ��������32�ֽ�
//strComponentName			(����)������ƣ��������32�ֽ�
DDS_API 
bool ComponentJoinDomain(DDS_STRING strDomainName, 
						 DDS_STRING strComponentName);

//����뿪��
//strDomainName				(����)�����ƣ��������32�ֽ�
//strComponentName			(����)������ƣ��������32�ֽ�
DDS_API 
bool ComponentLeaveDomain(DDS_STRING strDomainName, 
						  DDS_STRING strComponentName);

//////////////////////////////////////////////////////////////////////

//ע�����
//strDomainName				(����)�����ƣ��������32�ֽ�
//strComponentName			(����)������ƣ��������32�ֽ�
//componentNumber			(���)������
DDS_API 
bool RegisterComponent(DDS_STRING strDomainName, 
					   DDS_STRING strComponentName, 
					   DDS_COMPONENT_NUMBER &componentNumber);

//ע�����
//strDomainName				(����)�����ƣ��������32�ֽ�
//strComponentName			(����)������ƣ��������32�ֽ�
//componentNumber			(����)������
DDS_API 
bool UnregisterComponent(DDS_STRING strDomainName, 
						 DDS_STRING strComponentName, 
						 const DDS_COMPONENT_NUMBER &componentNumber);

//////////////////////////////////////////////////////////////////////

//������������
//componentKey				(����)�����ʶ
//strTopicName				(����)�������ƣ��������32�ֽ�
DDS_API 
bool OpenTopic(const sComponentKey &componentKey, 
			   DDS_STRING strTopicName);

//����ر��������
//componentKey				(����)�����ʶ
//strTopicName				(����)�������ƣ��������32�ֽ�
DDS_API 
bool CloseTopic(const sComponentKey &componentKey, 
				DDS_STRING strTopicName);

//////////////////////////////////////////////////////////////////////

//�����������
//componentKey				(����)�����ʶ
//strTopicName				(����)�������ƣ��������32�ֽ�
DDS_API 
bool PublishTopic(const sComponentKey &componentKey, 
				  DDS_STRING strTopicName);

//���ȡ����������
//componentKey				(����)�����ʶ
//strTopicName				(����)�������ƣ��������32�ֽ�
DDS_API 
bool UnpublishTopic(const sComponentKey &componentKey, 
					DDS_STRING strTopicName);

//�����������
//componentKey				(����)�����ʶ
//strTopicName				(����)��������
DDS_API 
bool SubscribeTopic(const sComponentKey &componentKey, 
					DDS_STRING strTopicName);

//���ȡ����������
//componentKey				(����)�����ʶ
//strTopicName				(����)�������ƣ��������32�ֽ�
DDS_API 
bool UnsubscribeTopic(const sComponentKey &componentKey, 
					  DDS_STRING strTopicName);

//////////////////////////////////////////////////////////////////////

//������������
//componentKey				(����)�����ʶ���������32�ֽ�
//strTopicName				(����)�������ƣ��������32�ֽ�
//pSendBuf					(����)���ݻ�����
//ulSendSize				(����)���ݳ���
//ucMode					(����)����ģʽ
//								0 �첽����(�����첽�߳����ʵ�ʵ����ݷ���)
//								1 ͬ������(��������ײ����ϵͳ����󷵻�)
//								������Ԥ������ֵ
DDS_API 
bool SendData(const sComponentKey &componentKey,
			  DDS_STRING strTopicName, 
			  char *pSendBuf, 
			  unsigned long ulSendSize, 
			  unsigned char ucMode = 1);

//ע��ڵ����ݽ��ջص�����
//componentKey				(����)�����ʶ
//pFunc						(����)�ص�����ָ��
//ucFuncCallMode			(����)�ص���������ģʽ
//  	   						0: �첽����(���ߵ��ûص��������߳������߽������ݵ��̲߳���ͬһ��)
//								1: ͬ������(���ߵ��ûص��������߳������߽������ݵ��߳�Ϊͬһ��)
//								������Ԥ������ֵ
DDS_API 
bool RegRecvNodeDataCallbackFunc(const sComponentKey &componentKey, 
								 DDS_PFN_RECV_DATA pFunc, 
								 unsigned char ucFuncCallMode = 1);

//ע���ڵ����ݽ��ջص�����
//componentKey				(����)�����ʶ
//pFunc						(����)�ص�����ָ��
DDS_API 
bool UnregRecvNodeDataCallbackFunc(const sComponentKey &componentKey,
								   DDS_PFN_RECV_DATA pFunc);

//////////////////////////////////////////////////////////////////////

//��ȡ���´����
//strDomainName				(����)�����ƣ��������32�ֽ�
//strComponentName			(����)������ƣ��������32�ֽ�
DDS_API 
unsigned long ComponentGetLastError(DDS_STRING strDomainName,  
									DDS_STRING strComponentName);

//��ȡ�����ı�����
//strDomainName				(����)�����ƣ��������32�ֽ�
//strComponentName			(����)������ƣ��������32�ֽ�
//ulErrorNumber				(����)�����
//strErrorText				(���)�����ı�����
DDS_API 
bool ComponentGetErrorMsg(DDS_STRING strDomainName,  
						  DDS_STRING strComponentName, 
						  unsigned long ulErrorNumber, 
						  DDS_STRING &strErrorText);

//��ȡ���´����ı�����
//strDomainName				(����)�����ƣ��������32�ֽ�
//strComponentName			(����)������ƣ��������32�ֽ�
//strErrorText				(���)�����ı�����
DDS_API 
bool ComponentGetLastErrorMsg(DDS_STRING strDomainName,  
							  DDS_STRING strComponentName, 
							  DDS_STRING &strErrorText);

//////////////////////////////////////////////////////////////////////

#pragma pack( pop, enter_include1 )
