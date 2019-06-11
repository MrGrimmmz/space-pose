//Common.h
#pragma once
#include "stdafx.h"
#include <vector>

namespace xPublic{
;/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


/********************************************************************
��������: ���س���Ĺ���·��
�������: ��
�������: ��
�� �� ֵ: ��
********************************************************************/
CString GetWorkPath(LPCTSTR lpszAppName);

/********************************************************************
��������: ���س���Ĺ���·��
�������: ��
�������: ��
�� �� ֵ: ��
********************************************************************/
CString GetAppPath(void);

//APP�Դ�ע������ȡ�ĺ���
//#define GETSTR ::AfxGetApp()->GetProfileString							//��ע����ȡ����(�ַ�)
//#define GETINT ::AfxGetApp()->GetProfileInt								//��ע����ȡ����(��ֵ)
//#define WRISTR ::AfxGetApp()->WriteProfileString							//��ע���������(�ַ�)
//#define WRIINT ::AfxGetApp()->WriteProfileInt								//��ע���������(��ֵ)

/********************************************************************
��������: ��Ini�����ļ��ж�ȡ�ַ���
�������: LPCTSTR lpAppName��������
		  LPCTSTR lpKeyName���ؼ���
		  LPCTSTR lpDefault��Ĭ��ֵ
�������: ��
�� �� ֵ: ��Ini��ȡ��ʵ��ֵ�ַ��������ini�����ڻ���iniû�и�����Ĭ��ֵ����
********************************************************************/
CString GETSTR2(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault);

/********************************************************************
��������: ��Ini�����ļ��ж�ȡ��ֵ
�������: LPCTSTR lpAppName��������
		  LPCTSTR lpKeyName���ؼ���
		  LPCTSTR lpDefault��Ĭ��ֵ
�������: ��
�� �� ֵ: ��Ini��ȡ��ʵ����ֵ(UINT)�����ini�����ڻ���iniû�и�����Ĭ��ֵ����
********************************************************************/
UINT GETINT2(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault);

/********************************************************************
��������: д�ַ�����Ini�����ļ�
�������: LPCTSTR lpAppName��������
		  LPCTSTR lpKeyName���ؼ���
		  LPCTSTR lpString ��ֵ
�������: ��
�� �� ֵ: ��
********************************************************************/
void WRISTR2(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString);

/********************************************************************
��������: д����ֵ��Ini�����ļ�
�������: LPCTSTR lpAppName��������
		  LPCTSTR lpKeyName���ؼ���
		  UINT nValue      ��ֵ
�������: ��
�� �� ֵ: ��
********************************************************************/
void WRIINT2(LPCTSTR lpAppName, LPCTSTR lpKeyName, UINT nValue);


//================================================================================================
//================================================================================================

/********************************************************************
��������: �ַ���16�������Ϊ�ַ���
�������: BYTE *inBuffer���ַ�����
		  int inLength  ���ַ����鳤��
�������: ��
�� �� ֵ: �ַ���
********************************************************************/
CStringA Hex2STR(const BYTE *inBuffer, int inLength);

//================================================================================================
//================================================================================================

/********************************************************************
��������: д��Ϣ��¼�ļ����ļ��������������֣�ÿ����Ϣ����ʱ�������
		  ����ļ����������Զ������������뱣֤���ļ�Ŀ¼����
�������: LPCTSTR lpFileName���ļ���
		  char *pszMsg      ����Ϣ
�������: ��
�� �� ֵ: ��
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
