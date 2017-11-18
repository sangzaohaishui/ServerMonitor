#pragma once
#include "GlobalStruct.h"
#include "com.cetc29.rpc.service.generated.CalculateElecRange2D3D.interface.h"

class CalcVisible2D3DRange
{
public:
	CalcVisible2D3DRange(void);
	~CalcVisible2D3DRange(void);
	static CalcVisible2D3DRange* m_pCalcInstance;
	static CalcVisible2D3DRange* GetCalcInstance();
	static void ReleaseCalcInstance();

public:

	//�ⲿ���ýӿڣ�nSeaAirFlag: 0�Կգ�1�Ժ��� calculateType��0:��ȡ2ά�������;1:��ȡ��ά�������;2:��ȡ2��3ά�������
	//rangeType��0,Ԥ��̽�� 1����췢�� 2����λʶ��
	bool CalcVisible2D3DRangeData(   int nSeaAirFlag,  int calculateType, int rangType, map<string, BaseCalcStruct::stDKRadarRange> &mapSingleEffRangle2D, SFSmartPtr<com::cetc29::rpc::service::generated::ICalculateRangeReplyOf2D3DProto> &result);

	//�״�Ч��ͨ�Ӽ���ӿ�<����״����><�ݲ�����>
	void CalcVisible2DRange(const vector<BaseCalcStruct::stDKRadarRange>& _vInData, vector<BaseCalcStruct::stDKRadarRange>& _vOutData );
	//�״�Ч��ͨ�Ӽ���ӿ�<�ݲ�����>
	void CalcVisible2DRangeOne(const BaseCalcStruct::stDKRadarRange& _stInData, BaseCalcStruct::stDKRadarRange& _stOutData);

private:

	//�����ʼ��
	void Init3DServer();
	//�������
	bool CallServer2Calc3D(SFSmartPtr<com::cetc29::rpc::service::generated::ICalculateRangeOf2D3DRequestProto> &param0, SFSmartPtr<com::cetc29::rpc::service::generated::ICalculateRangeReplyOf2D3DProto> &result);
	
	//�������ݸ�ʽת��<���ڲ���ʽת��ΪSF��ʽ>
	void CovertRange2DFor3DService(map<string,BaseCalcStruct::stDKRadarRange>& vRange, SFSmartPtr<com::cetc29::rpc::service::generated::ICalculateRangeOf2D3DRequestProto>& result );

private:

	SFSmartPtr<com::cetc29::rpc::service::generated::ICalculateElecRange2D3DRPCIProto> m_pServer3D;

	SFSmartPtr<IServiceParams> m_pServiceParams;

};

