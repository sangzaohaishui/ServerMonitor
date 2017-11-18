#pragma once

#include <string>
#include <vector>
#include <map>
#include <google/protobuf/message.h>
#include "hiredis.h"

using namespace std;
#pragma pack(8)
namespace acl
{
    //�������Ľṹ����
	typedef struct subpubInfo 
	{
		string channel;//����Ƶ��
		string message;//��Ϣ
	}subpub;

	/************************************************************************/
	/*@brief �յ�������Ϣ��ص�����
	/*@param[in] rcvChannel �����������ĵ�Ƶ��
	/*@param[in] rcvmessage �յ��Ķ�����Ϣ
	/*@return true �����ɹ� false ����ʧ��
	/************************************************************************/	
	typedef int (redisSubscribeCallback)(string rcvChannel,string rcvMessage);	

//���ݷ���ӿ�
class  __declspec(dllexport) IDataService
{
public:

	/**
	/*@brief �����Ƿ�ɹ�
	/*@author liyongchao
	/*@version V1.00
	/*@return true ���ӳɹ� false ����ʧ��
	*/
	virtual bool IsConnected()=0;

	/**
	/*@brief ��������
	/*@author liyongchao
	/*@version V1.00
	/*@return true ���ӳɹ� false ����ʧ��
	*/
	virtual bool ReConnected()=0;

	/**
	/*@brief ��ȡ���һ�δ�����Ϣ
	/*@author liyongchao
	/*@version V1.00
	/*@return ������Ϣ
	*/
	virtual string GetLastError()=0;

	/**
	/*@brief �жϴ洢���Ƿ����
	/*@author liyongchao
	/*@version V1.00
	/*@param[in] s_key �洢��
	/*@param[in] type �洢������
	/*@return bool �����ڷ���true�����򷵻�false
	*/
	virtual bool ExistKey(const char *s_category,const char* s_key)=0;

	/**
	/*@brief ��ȡ��
	/*@author liyongchao
	/*@version V1.00
	/*@param[in] s_category �������
	/*@param[in] s_key_pattern �洢��ģʽ
	/*@param[out] v_key_list �洢���б�
	/*@return true ��ȡ�ɹ� false ��ȡʧ��
	*/
	virtual bool GetKeys(const char *s_category,const char *s_key_pattern,vector<string>& v_key_list)=0;

	/**
	/*@brief ���ݴ洢��ɾ��ֵ
	/*@author liyongchao
	/*@version V1.00
	/*@param[in] s_category �������
	/*@param[in] s_key �洢��
	/*@return true ɾ���ɹ� false ɾ��ʧ��
	*/
	virtual bool DeleteKey(const char *s_category,const char *s_key)=0;

	/**
	/*@brief ���ݴ洢��ģʽɾ��ֵ
	/*@author liyongchao
	/*@version V1.00
	/*@param[in] s_category �������
	/*@param[in] s_key_pattern �洢��ģʽ
	/*@return true ɾ���ɹ� false ɾ��ʧ��
	*/
	virtual bool DeleteKeys(const char *s_category,const char *s_key_pattern)=0;

	/**
	/*@brief ���û�����ַ���
	/*@author liyongchao
	/*@version V1.00
	/*@param[in] s_category �������
	/*@param[in] s_key �洢��
	/*@param[in] s_value �����õ��ַ���
	/*@return true �����ɹ� false ����ʧ��
	*/	
	virtual bool SetString(const char *s_category,const char *s_key,const char* s_value)=0;

	/**
	/*@brief ��ȡ�ַ���
	/*@author liyongchao
	/*@version V1.00
	/*@param[in] s_category �������
	/*@param[in] s_key �洢��
	/*@param[out] s_value ��ȡ���ַ���
	/*@return true �����ɹ� false ����ʧ��
	*/	
	virtual bool GetString(const char *s_category,const char *s_key,std::string& s_value)=0;

	/**
	/*@brief ���û���¶���
	/*@author liyongchao
	/*@version V1.00
	/*@param[in] s_category �������
	/*@param[in] s_key �洢��
	/*@param[in] p_message �����õĶ���
	/*@return true �����ɹ� false ����ʧ��
    */	
	virtual bool SetMessage(const char *s_category,const char *s_key,::google::protobuf::Message* p_message)=0;

	/**
	/*@brief �����������ʹ洢����ȡ����
	/*@author liyongchao
	/*@version V1.00
	/*@param[in] s_category �������
	/*@param[in] s_key �洢��
	/*@param[out] p_message ���صĶ���
	/*@return true �����ɹ� false ����ʧ��
   */
	virtual bool GetMessage(const char *s_category,const char *s_key, ::google::protobuf::Message* p_message)=0;

	/**
	/*@brief ���û���¶�������
	/*@author liyongchao
	/*@version V1.00
	/*@param[in] s_key �洢��
	/*@param[in] s_buffer ��������
	/*@param[in] i_size �ֽ���
	/*@return bool ���洢�ɹ��󷵻�true�����򷵻�false
	*/
	virtual bool SetBuffer(const char* s_category,const char* s_key,const char* s_buffer,unsigned int i_size)=0;

	/**
	/*@brief ��ȡ��������
	/*@author liyongchao
	/*@version V1.00
	/*@param[in] s_key �洢��
	/*@param[in] s_buffer ��������
    /*@param[in] i_size �ֽ���
	/*@return bool ����ȡ�ɹ��󷵻�true�����򷵻�false
    */
	virtual bool GetBuffer(const char* s_category,const char* s_key,char*& s_buffer,unsigned int& i_size)=0;

	/**
	/*@brief ע�ᶩ��Ƶ���Ļص�����
	/*@author liyongchao
	/*@version V1.00
	/*@param[in] s_channel ���ĵ�Ƶ��
	/*@param[in] redisSubCalBk �ص�����
	/*@return true ע��ɹ� false ע��ʧ�� 
	*/
	virtual bool RegistCalBakFuc(const string& s_channel,redisSubscribeCallback *redisSubCalBk)=0;

	/**
	/*@brief ����Ƶ��������Ϣ
	/*@author liyongchao
	/*@version V1.00
	/*@param[in] s_channel ������Ƶ��
	/*@param[in] s_pubInfo ��������Ϣ
	/*@return int ���ĸ�Ƶ���ĸ�����-1��ʾ����ʧ��
   */
	virtual int PublishMessage(const char * s_channel,const char* s_pubInfo)=0;

	/**
	/*@brief ������Ϣ��һ��Ҫ��ע����ص���������ʹ�ã�
	/*@author liyongchao
	/*@version V1.00
	/*@param[in] v_channel ���ĵ�Ƶ������
	/*@return string true �����ɹ� false ����ʧ��
	*/
	virtual bool SubscribeMessage(const vector<char*>& v_channel)=0;

	/**
	/*@brief �����ض���ʽ����Ƶ����һ�㶩�Ķ��Ƶ��
	/*@author liyongchao
	/*@version V1.00
	/*@param[in] s_psubPattern ���ĵķ�ʽ
	/*@param[in] redisSubCalBk ��Ӧ�Ļص�����
	/*@return string true �����ɹ� false ����ʧ��
   */
	virtual bool PsubscribeMessage(const char* s_psubPattern)=0;

	/**
	/*@brief ������ݿ�
	/*@author liyongchao
	/*@version V1.00
	/*@return true ��ճɹ� false ���ʧ��
   */
	virtual bool FlushDB()=0;

	/**
	/*@brief �����ڴ�����������
	/*@author liyongchao
	/*@version V1.00
	/*@return true ���ݳɹ� false ����ʧ��
	*/
	virtual bool SaveDB()=0;

	/**
	/*@brief ������Ϣ��һ��Ҫ��ע����ص���������ʹ�ã�
	/*@author liyongchao
	/*@version V1.00
	/*@param[in] v_channel ���ĵ�Ƶ������
	/*@return string true �����ɹ� false ����ʧ��
	*/
	virtual bool SubscribeMessage(const vector<string>& v_channel)=0;

	//************************************
	// Method:    getRedisConnection ��ȡRedis����
	// FullName:  acl::IDataService::getRedisConnection
	// Access:    virtual public 
	// Returns:   bool ������Ϊ�ջ��д��󣬷���false
	// Qualifier:
	// Parameter: redisContext * & conn ���Σ������ӷǿ����޴����򷵻�����
	//************************************
	virtual bool getRedisConnection(redisContext*& conn) = 0;

	//������Ϣ
	virtual void SubscribeMessageNew(const vector<string>& v_channel) = 0;
};
    
/************************************************************************/
/*@brief �������ݷ���ӿ�ָ��
/*@author liyongchao
/*@version V1.00
/*@param[in] s_ip ���ݿ�IP��ַ
/*@param[in] port ���ݿ�˿ں� 
/*@return ���ݷ���ӿ�ָ��
/************************************************************************/
__declspec(dllexport) IDataService* ICreateDataService(const char* s_ip,const int port);

/************************************************************************/
/*@brief �������ݷ������Ľӿ�ָ��
/*@author liyongchao
/*@version V1.00
/*@param[in] s_ip ���ݿ�IP��ַ
/*@param[in] port ���ݿ�˿ں� 
/*@return ���ݷ���ӿ�ָ��
/************************************************************************/
__declspec(dllexport) IDataService* ICreatePubSubService(const char* s_ip,const int port);

}
#pragma pack()
