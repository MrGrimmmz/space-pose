
// CraneTest1.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCraneTest1App:
// �йش����ʵ�֣������ CraneTest1.cpp
//

class CCraneTest1App : public CWinApp
{
public:
	CCraneTest1App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCraneTest1App theApp;