/********************************************************************
	��������:	2016/06/25
	�� �� ��:	CommonTool
	�� չ ��:	cpp
	��    ��:	zz
	
	˵    ��:	��������ʵ���ļ�,������Ҫ���ղ�ͬ��ƽ̨���ò�ͬ��API
*********************************************************************/

#include "CommonTool.h"

#ifdef WIN32
#include <Windows.h>
#endif

namespace CommonTool
{
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

	std::string GetAppName()
	{
		char szBuf[1024] = {0};
		std::string strpath;
#ifdef WIN32
		// ��Windowsƽ̨���õĻ�ȡ����
		GetModuleFileNameA(NULL,szBuf,sizeof(szBuf));
		strpath = szBuf;
		strpath = strpath.substr(strpath.rfind('\\') + 1);
		strpath = strpath.substr(0,strpath.length() - 4);
#endif
		
		return strpath;
	}
}