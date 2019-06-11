//Common.cpp
#include "stdafx.h"
#include "Common.h"
#include <afxglobals.h>

namespace xPublic{
;/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
static void App_SetFont(void)
{
	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);
	afxGlobalData.fontRegular.DeleteObject();
	lf.lfHeight = -12;
	lstrcpy(lf.lfFaceName, _T("宋体"));
	afxGlobalData.fontRegular.CreateFontIndirect(&lf);

	//设置字体加粗
	afxGlobalData.fontBold.GetLogFont(&lf);
	afxGlobalData.fontBold.DeleteObject();
	lf.lfHeight = -12;
	lstrcpy(lf.lfFaceName, _T("宋体"));
	afxGlobalData.fontBold.CreateFontIndirect(&lf);

	//设置提示字体
	afxGlobalData.fontTooltip.GetLogFont(&lf);
	afxGlobalData.fontTooltip.DeleteObject();
	lf.lfHeight = -12;
	lstrcpy(lf.lfFaceName, _T("宋体"));
	afxGlobalData.fontTooltip.CreateFontIndirect(&lf);
}

static CString	s_sAppIni;	//程序配置文件全路径
static CString	s_sAppPath;	//程序工作路径
/********************************************************************
函数功能: 获取程序的工作路径
输入参数: LPCTSTR lpszIniName：程序的ini文件路径
输出参数: 无
返 回 值: 无
********************************************************************/
CString GetWorkPath(LPCTSTR lpszAppName)
{
	App_SetFont();
	TCHAR szWorkPath[MAX_PATH] = {};
	::GetModuleFileName(NULL,szWorkPath,MAX_PATH);
	CString strTmp = szWorkPath;
	s_sAppPath = strTmp.Left(strTmp.ReverseFind(_T('\\')));
	s_sAppIni.Format(_T("%s\\%s.ini"), s_sAppPath,lpszAppName);
	return s_sAppPath;
}
/********************************************************************
函数功能: 返回程序的工作路径
输入参数: 无
输出参数: 无
返 回 值: 无
********************************************************************/
CString GetAppPath(void)
{
	return s_sAppPath;
}

/********************************************************************
函数功能: 从Ini配置文件中读取字符串
输入参数: LPCTSTR lpAppName：配置项
		  LPCTSTR lpKeyName：关键字
		  LPCTSTR lpDefault：默认值
输出参数: 无
返 回 值: 从Ini读取的实际值字符串，如果ini不存在或者ini没有该项由默认值代替
********************************************************************/
CString GETSTR2(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault)
{
	CString strReturn;
	::GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, strReturn.GetBuffer(MAX_PATH), MAX_PATH, s_sAppIni);
	strReturn.ReleaseBuffer();
	return strReturn;
}


/********************************************************************
函数功能: 从Ini配置文件中读取数值
输入参数: LPCTSTR lpAppName：配置项
		  LPCTSTR lpKeyName：关键字
		  LPCTSTR lpDefault：默认值
输出参数: 无
返 回 值: 从Ini读取的实际数值(UINT)，如果ini不存在或者ini没有该项由默认值代替
********************************************************************/
UINT GETINT2(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault)
{
	return ::GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, s_sAppIni);
}

/********************************************************************
函数功能: 写字符串到Ini配置文件
输入参数: LPCTSTR lpAppName：配置项
		  LPCTSTR lpKeyName：关键字
		  LPCTSTR lpString ：值
输出参数: 无
返 回 值: 无
********************************************************************/
void WRISTR2(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString)
{
	::WritePrivateProfileString(lpAppName, lpKeyName, lpString, s_sAppIni);
}

/********************************************************************
函数功能: 写整型值到Ini配置文件
输入参数: LPCTSTR lpAppName：配置项
		  LPCTSTR lpKeyName：关键字
		  UINT nValue      ：值
输出参数: 无
返 回 值: 无
********************************************************************/
void WRIINT2(LPCTSTR lpAppName, LPCTSTR lpKeyName, UINT nValue)
{
	CString sValue;
	sValue.Format(_T("%d"), nValue);
	WRISTR2(lpAppName, lpKeyName, sValue);
}
//================================================================================================
//================================================================================================

/********************************************************************
函数功能: 字符以16进制输出为字符串
输入参数: BYTE *inBuffer：字符数组
		  int inLength  ：字符数组长度
输出参数: 无
返 回 值: 字符串
********************************************************************/
CStringA Hex2STR(const BYTE *inBuffer, int inLength)
{
	CStringA strMsg,strTmp;
	for ( int i=0; i<inLength; i++ )
	{
		strTmp.Format("%02X ", inBuffer[i]);
		strMsg += strTmp;
	}
	return strMsg;
}
//================================================================================================
//================================================================================================

/********************************************************************
函数功能: 写消息记录文件，文件名以年月日区分，每条消息增加时分秒毫秒
		  如果文件不存在则自动创建，但必须保证该文件目录存在
输入参数: LPCTSTR lpFileName：文件名
		  char *pszMsg      ：消息
输出参数: 无
返 回 值: 无
********************************************************************/
void WriteFileDebug(LPCTSTR lpFileName, const char *pszMsg)
{
	CStringA strMsg,str1;
	SYSTEMTIME t; 
	::GetLocalTime(&t);
	strMsg.Format("%02d:%02d:%02d.%03d %s\r\n", t.wHour,t.wMinute,t.wSecond,t.wMilliseconds,pszMsg);


	str1.Format(_T("C:\\Users\\lenovo\\Documents\\Visual Studio 2010\\Projects\\CraneTest1\\CraneTest1"));
	CString sFileName;
	sFileName.Format(_T("%s\\logs(Debug)\\%s(%04d-%02d-%02d).txt"), str1,lpFileName,t.wYear,t.wMonth,t.wDay);
	CFile cf;
	if ( cf.Open(sFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyNone|CFile::typeBinary) )
	{
		cf.SeekToEnd();
		cf.Write(strMsg, strMsg.GetLength());
		cf.Close();
	}
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
}//namespace
