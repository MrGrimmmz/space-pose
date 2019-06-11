// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__93ECACE7_DE68_472A_8633_143BBAD00ED5__INCLUDED_)
#define AFX_STDAFX_H__93ECACE7_DE68_472A_8633_143BBAD00ED5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT 0x0501

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxmt.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "FileVersion.h"

#include<opencv2/opencv.hpp>
#include <cv.h>
#include <opencv2/objdetect/objdetect.hpp>
#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/opencv.hpp" 
#include <opencv2/core/core.hpp>
#include "opencv2/gpu/gpu.hpp"

#include <math.h>;
#include "Common.h";
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__93ECACE7_DE68_472A_8633_143BBAD00ED5__INCLUDED_)
