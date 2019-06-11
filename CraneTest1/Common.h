//Common.h
#pragma once
#include "stdafx.h"
#include <vector>

namespace xPublic{
;/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


/********************************************************************
函数功能: 返回程序的工作路径
输入参数: 无
输出参数: 无
返 回 值: 无
********************************************************************/
CString GetWorkPath(LPCTSTR lpszAppName);

/********************************************************************
函数功能: 返回程序的工作路径
输入参数: 无
输出参数: 无
返 回 值: 无
********************************************************************/
CString GetAppPath(void);

//APP自带注册表项获取的函数
//#define GETSTR ::AfxGetApp()->GetProfileString							//从注册表读取设置(字符)
//#define GETINT ::AfxGetApp()->GetProfileInt								//从注册表读取设置(数值)
//#define WRISTR ::AfxGetApp()->WriteProfileString							//向注册表保存设置(字符)
//#define WRIINT ::AfxGetApp()->WriteProfileInt								//向注册表保存设置(数值)

/********************************************************************
函数功能: 从Ini配置文件中读取字符串
输入参数: LPCTSTR lpAppName：配置项
		  LPCTSTR lpKeyName：关键字
		  LPCTSTR lpDefault：默认值
输出参数: 无
返 回 值: 从Ini读取的实际值字符串，如果ini不存在或者ini没有该项由默认值代替
********************************************************************/
CString GETSTR2(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault);

/********************************************************************
函数功能: 从Ini配置文件中读取数值
输入参数: LPCTSTR lpAppName：配置项
		  LPCTSTR lpKeyName：关键字
		  LPCTSTR lpDefault：默认值
输出参数: 无
返 回 值: 从Ini读取的实际数值(UINT)，如果ini不存在或者ini没有该项由默认值代替
********************************************************************/
UINT GETINT2(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault);

/********************************************************************
函数功能: 写字符串到Ini配置文件
输入参数: LPCTSTR lpAppName：配置项
		  LPCTSTR lpKeyName：关键字
		  LPCTSTR lpString ：值
输出参数: 无
返 回 值: 无
********************************************************************/
void WRISTR2(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString);

/********************************************************************
函数功能: 写整型值到Ini配置文件
输入参数: LPCTSTR lpAppName：配置项
		  LPCTSTR lpKeyName：关键字
		  UINT nValue      ：值
输出参数: 无
返 回 值: 无
********************************************************************/
void WRIINT2(LPCTSTR lpAppName, LPCTSTR lpKeyName, UINT nValue);


//================================================================================================
//================================================================================================

/********************************************************************
函数功能: 字符以16进制输出为字符串
输入参数: BYTE *inBuffer：字符数组
		  int inLength  ：字符数组长度
输出参数: 无
返 回 值: 字符串
********************************************************************/
CStringA Hex2STR(const BYTE *inBuffer, int inLength);

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
void WriteFileDebug(LPCTSTR lpFileName, const char *pszMsg);


typedef struct struct_SAFEBOOL
{
	
	BOOL m_Bool;
	int count;
	::CCriticalSection	m_cs;

	struct_SAFEBOOL()
	{
		count = 0;
		m_Bool = TRUE;
	};
}SAFEBOOL;

class CSafeBool
{
public:
	CSafeBool(SAFEBOOL *pBool)
	{
		//ASSERT (*pBool);
		pBool->m_cs.Lock();
		pBool->count++;
		if( pBool->count == 1 ) 
		{
			pBool->m_Bool = FALSE;
		}
		pBool->m_cs.Unlock();

		m_pBool = pBool;
	}

	CSafeBool(SAFEBOOL *pBool,CString strFunc)
	{
		//ASSERT (*pBool);
		pBool->m_cs.Lock();
		pBool->count++;
		if( pBool->count == 1 ) 
		{
			pBool->m_Bool = FALSE;
		}
		pBool->m_cs.Unlock();

		m_pBool = pBool;
		m_sFunc = strFunc;
	/*	{
			CString str;
			str.Format(_T("%s:count+1=%d"),strFunc, m_pBool->count);
			WriteFileDebug(_T("SAFEBOOL"),str);
		}*/
	}
	
	~CSafeBool ()
	{
		m_pBool->m_cs.Lock();
		m_pBool->count--;
	/*	{
			CString str;
			str.Format(_T("%s:count-1=%d"),m_sFunc, m_pBool->count);
			WriteFileDebug(_T("SAFEBOOL"),str);
		}*/
		if( m_pBool->count <= 0 ) 
		{
			m_pBool->m_Bool = TRUE;
		}
		else
		{
			/*CString str;
			str.Format(_T("Warning:%s:count=%d when exit"), m_sFunc, m_pBool->count);
			WriteFileDebug(_T("SAFEBOOL"),str);*/
		}
		m_pBool->m_cs.Unlock();
	}
	
private:
	SAFEBOOL *m_pBool;
	CString m_sFunc;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
}//namespace
