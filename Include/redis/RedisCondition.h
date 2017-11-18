#ifndef RedisCondition_INCLUDED
#define RedisCondition_INCLUDED

#include <vector>

using namespace std;

#pragma pack(8)

/************************************************************************/
/* ��ѯģʽ, ������ȷ��ģ���ͷ�Χ����ģʽ                                                               
/************************************************************************/ 
enum QueryMode
{
	PRECISE = 1, /// ��ȷ��ѯ
	FUZZY   = 2, /// ģ����ѯ
	RANGE   = 3  /// ��Χ��ѯ
};

/************************************************************************/
/* ����������, �����ַ��������͡������ͺ�������������                                                              
/************************************************************************/ 
enum FieldType
{
	STRING     = 1, ///�ַ���
	INTEGER    = 2, ///����
	FLOATING   = 3, ///������
	DATETIME   = 4  ///������
};

/************************************************************************/
/* ����ģʽ, ��������ͽ�������ģʽ                                                               
/************************************************************************/ 
enum SortMode
{
	ASC  =1, ///����
	DESC =2  ///����
};

typedef struct _field{
	string       m_field; /// ��ֵ
	FieldType    m_fieldtype;/// �����ͣ�STRING=�ַ��� INTEGER=���� FLOATING=���� DATETIME=����
	string       m_timeFormat;/// ���ڸ�ʽ��������Ϊ����ʱ���ã� Ĭ�ϸ�ʽ %d-%d-%d %d:%d:%d�� ���� 2015-3-2 9:39:15
} Field;

typedef struct _wherecondition{
	Field        s_field;/// ��
	QueryMode    m_queryMode;/// ��ѯģʽ��PRECISE=��ȷ FUZZY=ģ�� RANGE=��Χ
	string       m_value;/// ֵ����ѯģʽΪPRECISE��FUZZYʱ����
	string       m_maxValue;/// ���ֵ����ѯģʽΪRANGEʱ����, invalid��ʾ��Чֵ��������Ƚ�
	string       m_minValue;/// ��Сֵ����ѯģʽΪRANGEʱ����, invalid��ʾ��Чֵ��������Ƚ�
	bool         m_leftBoundary; ///�Ƿ������߽磬��ѯģʽΪRANGEʱ����
	bool         m_rightBoundary; ///�Ƿ�����ұ߽磬��ѯģʽΪRANGEʱ����
	
} WhereCondition;

typedef struct _ordercondition{
	Field    s_field;/// ��
	SortMode m_sortMode;/// ����ģʽ��ASC=���� DESC=���� 
} OrderCondition;

class RedisCondition
{
public:
	vector<WhereCondition> andList;/// AND����
	vector<WhereCondition> orList;/// OR����
	vector<OrderCondition> orderList;/// ��������
};

#pragma pack()

#endif