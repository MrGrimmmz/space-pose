// SerialPort.cpp: implementation of the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThreadBase.h"
#include <process.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace xPublic{
;/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//构造时决定线程处理方式(hParent和onWork都有效时通过回调处理，否则使用虚函数处理)
//一般情况，一个类要使用多个线程采用回调处理，若是单线程可使用虚函数，也可以回调
CThreadBase::CThreadBase(HANDLE hParent, ONWORK onWork)
{
	m_hParent	  = hParent;
	m_OnWork	  = onWork;
	m_hWorkThread = NULL;	//线程句柄
	m_hCloseEvent = NULL;	//线程关闭事件
}

CThreadBase::~CThreadBase()
{
	StopThread();
}

/********************************************************************
函数功能: 线程处理事件，虚函数，通知上层处理
输入参数: HANDLE hCloseEvent：退出事件
输出参数: 无
返 回 值: 无
********************************************************************/
void CThreadBase::DoWork(HANDLE hCloseEvent)
{
	while ( WaitTimeOut(1000) )
	{
	}
}

/********************************************************************
函数功能：线程函数
输入参数：lParam：指向当前类的句柄
输出参数：无
返 回 值：0，无实际意义
/********************************************************************/
unsigned _stdcall CThreadBase::WorkProc(LPVOID lParam)
{
	ASSERT(lParam != NULL);
	CThreadBase *pThis = (CThreadBase*)lParam;

	if ( NULL != pThis->m_hParent && NULL != pThis->m_OnWork )
	{
		pThis->m_OnWork(pThis->m_hParent, pThis->m_hCloseEvent);
	}
	else
	{
		pThis->DoWork(pThis->m_hCloseEvent);
	}
	return 0;
}

/********************************************************************
函数功能: 停止线程
输入参数: 无
输出参数: 无
返 回 值: 无
********************************************************************/
void CThreadBase::StopThread()
{
	// 设置关闭事件有信号
	if ( NULL != m_hCloseEvent )
	{
		::SetEvent(m_hCloseEvent);
	}
	// 关闭线程
	if ( NULL != m_hWorkThread )
	{
		if ( ::WaitForSingleObject(m_hWorkThread, 30000) == WAIT_TIMEOUT )
		{
			::TerminateThread(m_hWorkThread, 0);
		}
		::CloseHandle(m_hWorkThread); m_hWorkThread = NULL;
	}
	// 关闭事件
	if ( NULL != m_hCloseEvent )
	{
		::CloseHandle(m_hCloseEvent); m_hCloseEvent = NULL;
	}
}

/********************************************************************
函数功能: 启动线程
输入参数: 无
输出参数: 无
返 回 值: TRUE 成功；FALSE 失败
********************************************************************/
BOOL CThreadBase::StartThread()
{
	if ( NULL == m_hCloseEvent ) m_hCloseEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if ( NULL != m_hCloseEvent && NULL == m_hWorkThread )
	{
		unsigned int nThreadID;	
		m_hWorkThread = (HANDLE) _beginthreadex(NULL,0,WorkProc,this,0,&nThreadID);
	}
	return ( NULL != m_hWorkThread );
}

/********************************************************************
函数功能: 关闭通知,一般用于提前通知关闭事件
输入参数: 无
输出参数: 无
返 回 值: TRUE 成功；FALSE 失败
********************************************************************/
void CThreadBase::SetClose()
{
	// 设置关闭事件有信号
	if ( NULL != m_hCloseEvent )
	{
		::SetEvent(m_hCloseEvent);
	}
}

/********************************************************************
函数功能: 等待，必须在启动线程后才有效
输入参数: DWORD dwTimeOut：等待时间,允许为0
输出参数: 无
返 回 值: TRUE 成功；FALSE 失败
********************************************************************/
BOOL CThreadBase::WaitTimeOut(DWORD dwTimeOut /*=10*/)
{
	return ( WAIT_TIMEOUT == ::WaitForSingleObject(m_hCloseEvent, dwTimeOut) );
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
}//namespace


