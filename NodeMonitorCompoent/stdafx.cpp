// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// ActionMagServerComponent.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

// TODO: �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������


std::string GetAppPath()
{
	char szBuf[1024] = {0};
	std::string strpath;
#ifdef WIN32
	// ��Windowsƽ̨���õĻ�ȡ����
	GetModuleFileNameA(NULL,szBuf,sizeof(szBuf));
	strpath = szBuf;
	strpath = strpath.substr(0, strpath.rfind('\\'));
#endif
	return strpath;
}
