#include "stdafx.h"
#include "CalcVisibleRange.h"
#include "IBaseCalcFunction.h"
#include "ILog4Mgr.h"
using namespace com::cetc29::rpc::service::generated;
USING_BASECALC_STRUCT

CalcVisible2D3DRange* CalcVisible2D3DRange::m_pCalcInstance = NULL;
CalcVisible2D3DRange::CalcVisible2D3DRange(void)
{
	m_pServer3D = NULL;
	Init3DServer();
}


CalcVisible2D3DRange::~CalcVisible2D3DRange(void)
{
}

CalcVisible2D3DRange* CalcVisible2D3DRange::GetCalcInstance()
{
	if (m_pCalcInstance == NULL)
	{
		m_pCalcInstance = new CalcVisible2D3DRange;
	} 
	return m_pCalcInstance;
}

void CalcVisible2D3DRange::ReleaseCalcInstance()
{
	if (m_pCalcInstance != NULL)
	{
		delete m_pCalcInstance;
		m_pCalcInstance = NULL;
	} 
}

void CalcVisible2D3DRange::Init3DServer()
{
	//�������ļ�
	std::string strConfigPath = GetAppPath();
	strConfigPath = strConfigPath + "/" + GetAppName() + ".ini";

	char name[1024] = {0};
	GetPrivateProfileStringA("SERVICENAME","nameServiceEnvelop","",name,sizeof(name),strConfigPath.c_str());

	//�������ƣ�����Ҫ
	wchar_t* WStr = NULL;
	int ilen = MultiByteToWideChar(CP_ACP,0,name,strlen(name),NULL,0);
	WStr = new wchar_t[ilen+1];
	MultiByteToWideChar(CP_ACP,0,name,strlen(name),WStr,ilen);
	WStr[ilen] = '\0';

	m_pServiceParams = SFCreateServiceParams();
	try
	{
		if(!m_pServiceParams.isNull())
		{
			m_pServiceParams->Set("service", WStr);//���ı���ʹ��L
			m_pServiceParams->Set("version", L"0.0.0.1");
		}
	}
	catch (SFException &e)
	{
		string lhx = e.what();
		GetLog4Mgr().LogMsgInfo("%s", e.what());
		return;
	}
	catch (...)
	{
		return;
	}
	m_pServer3D = SFCreateService<com::cetc29::rpc::service::generated::ICalculateElecRange2D3DRPCIProto>(m_pServiceParams);
}

bool CalcVisible2D3DRange::CallServer2Calc3D(SFSmartPtr<ICalculateRangeOf2D3DRequestProto> &param0, SFSmartPtr<ICalculateRangeReplyOf2D3DProto> &result)
{
	SFProxyContext ProxyContext;
	SFSmartPtr<com::cetc29::rpc::service::generated::ICalculateElecRange2D3DRPCIProto> pServer3D = SFCreateService<com::cetc29::rpc::service::generated::ICalculateElecRange2D3DRPCIProto>(m_pServiceParams);

	try
	{
		if (pServer3D != NULL)
		{
			pServer3D->calculate2D3D(&ProxyContext, param0, result);
		}
	}
	catch (SFException &e)
	{
		string lhx = e.what();
		GetLog4Mgr().LogMsgInfo("ͨ�ӷ�������ά����������δ���������ڳ����������ӡ������룺%s", e.what());
		Init3DServer();
		return false;
	}
	catch (...)
	{
		return false;
	}
	return true;
}

void CalcVisible2D3DRange::CovertRange2DFor3DService(map<string,BaseCalcStruct::stDKRadarRange>& mapRange, SFSmartPtr<ICalculateRangeOf2D3DRequestProto>& result )
{
	//��������
// 	result->Set_calculatetarget(0);//�Կ�0���Ժ�1
// 	result->Set_calculatetype(calculateType);//0:��ȡ2ά�������;1:��ȡ��ά�������;2:��ȡ2��3ά�������
// 	result->Set_rangetype(0);//0,Ԥ��̽�� 1����췢�� 2����λʶ��
	//Ч����Ϣ
	SFSmartPtr<IList<SFSmartPtr<com::cetc29::rpc::service::generated::ICalBasicInfoProto>>> calinfolist = SFCreateData<IList<SFSmartPtr<com::cetc29::rpc::service::generated::ICalBasicInfoProto>>>();
	map<string,BaseCalcStruct::stDKRadarRange>::iterator iter = mapRange.begin();
	int idx = 0;
	for (; iter != mapRange.end(); iter ++)
	{
		result->Set_height(iter->second.fTrgHeight);

		SFSmartPtr<com::cetc29::rpc::service::generated::ICalBasicInfoProto> BasicInfo = SFCreateData<com::cetc29::rpc::service::generated::ICalBasicInfoProto>();
		//����Ϣ
		SFSmartPtr<IList<SFSmartPtr<com::cetc29::rpc::service::generated::IDProto>>> lpos = SFCreateData<IList<SFSmartPtr<com::cetc29::rpc::service::generated::IDProto>>>();
		SFSmartPtr<com::cetc29::rpc::service::generated::IDProto> posTmp = SFCreateData<com::cetc29::rpc::service::generated::IDProto>();
		
		for( int j = 0; j < iter->second.vAreaPos.size(); j++ )
		{
			EW_Point _pos = iter->second.vAreaPos[j];
			posTmp->Set_jd(_pos.dLon);
			posTmp->Set_wd(_pos.dLat);
			posTmp->Set_gc(_pos.dHeight);
			lpos->Add(posTmp);
		}
		BasicInfo->Set_calposlist(lpos);
		//�������
		BasicInfo->Set_index(idx++);
		//Ŀ��λ��
		SFSmartPtr<com::cetc29::rpc::service::generated::IDProto> pos = SFCreateData<com::cetc29::rpc::service::generated::IDProto>();
		EW_Point pnt = EW_Point(iter->second.fLon, iter->second.fLat, iter->second.fHeight);
		pos->Set_jd(pnt.dLon);
		pos->Set_wd(pnt.dLat);
		pos->Set_gc(pnt.dHeight);
		BasicInfo->Set_centerposition(pos);
		//�������ͣ�Ԥ��̽��
		BasicInfo->Set_type(com::cetc29::rpc::service::generated::RadarPower);
		int maxDist = 0;
		for (int j = 0; j < iter->second.vAreaPos.size(); j ++)
		{
			double dist(0);
			BaseCalcFunction::GetInstance()->CalcDis2Angle(pnt, iter->second.vAreaPos[j], dist);
			if (dist > maxDist)
			{
				maxDist = dist;
			}
		}
		//Ŀ��̽������
		BasicInfo->Set_distance(maxDist * 1000);
		double ratio = 1.5;
		BasicInfo->Set_elevation(ratio);
		ratio = 1.0;
		BasicInfo->Set_azimuth(ratio);
		ratio = 90;
		BasicInfo->Set_horizontalangle(ratio);
		ratio = 90;
		BasicInfo->Set_veticalangle(ratio);

		calinfolist->Add(BasicInfo);
	}
	result->Set_calinfolist(calinfolist);
}

bool CalcVisible2D3DRange::CalcVisible2D3DRangeData(  int nSeaAirFlag,  int calculateType, int rangType, map<string, BaseCalcStruct::stDKRadarRange> &mapSingleEffRangle2D, SFSmartPtr<ICalculateRangeReplyOf2D3DProto> &result )
{
	//����������
	SFSmartPtr<com::cetc29::rpc::service::generated::ICalculateRangeOf2D3DRequestProto> param0 = SFCreateData<com::cetc29::rpc::service::generated::ICalculateRangeOf2D3DRequestProto>();
	
	param0->Set_calculatetarget(nSeaAirFlag);//�Կ�0���Ժ�1
	param0->Set_calculatetype(calculateType);//0:��ȡ2ά�������;1:��ȡ��ά�������;2:��ȡ2��3ά�������
	param0->Set_rangetype(rangType);//0,Ԥ��̽�� 1����췢�� 2����λʶ��
	CovertRange2DFor3DService(mapSingleEffRangle2D, param0);

	//���ݶ�ά���������ά����
	bool bOk = CallServer2Calc3D(param0, result);
	return bOk;
}

//�״�Ч��ͨ�Ӽ���ӿ�
void CalcVisible2D3DRange::CalcVisible2DRange( const vector<BaseCalcStruct::stDKRadarRange>& _vInData, vector<BaseCalcStruct::stDKRadarRange>& _vOutData )
{
	//1.�жϲ�����Ч��
	if (_vInData.size() <= 0)
	{
		return;
	}
	//2.���ת��
	//3.���ýӿڼ���
	//4.���ת��
}

//�״�Ч��ͨ�Ӽ���ӿ�
void CalcVisible2D3DRange::CalcVisible2DRangeOne( const BaseCalcStruct::stDKRadarRange& _stInData, BaseCalcStruct::stDKRadarRange& _stOutData )
{
	//1.�жϲ�����Ч��
	//2.���ת��(todo)
	map<string, stDKRadarRange> mapInData;
	int nPointNum = _stInData.vAreaPos.size();
	if (nPointNum <= 0)
	{
		return;
	}
	mapInData.insert(make_pair(_stInData.strRadarID, _stInData));
	SFSmartPtr<ICalculateRangeOf2D3DRequestProto> sfInData = SFCreateData<ICalculateRangeOf2D3DRequestProto>();
	//3.���ýӿڼ���
/*	SFSmartPtr<ICalculateRangeReplyOf2D3DProto> sfResult = */
	//4.���ת��
}
