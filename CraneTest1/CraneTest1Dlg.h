// CraneTest1Dlg.h : header file
//

#if !defined(AFX_CraneTest1Dlg_H__26540673_FD9B_4799_889E_E05C3076016C__INCLUDED_)
#define AFX_CraneTest1Dlg_H__26540673_FD9B_4799_889E_E05C3076016C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCraneTest1Dlg dialog
#include "GxIAPI.h"
#include "DxImageProc.h"
#include <string>
#include <iostream>
#include "ThreadBase.h"

using namespace std;
using namespace cv;
using namespace cv::gpu;
///< 宏定义用户IO输出值 0:false 1:true
#define OUTPUT_ACTIVATION_FALSE  0
#define OUTPUT_ACTIVATION_TRUE   1

///< 宏定义引脚电平反转值 1:true 0:false
#define LINE_INVERTER_FALSE 0
#define LINE_INVERTER_TRUE  1
#define WM_DISPLAY_CHANGE (WM_USER + 100)

///< 错误提示函数宏定义
#define  GX_VERIFY(emStatus) \
	if (emStatus != GX_STATUS_SUCCESS)\
							{\
							ShowErrorString(emStatus); \
							return ;\
							 } 
///< 判断返回值宏定义
#define VERIFY_STATUS_RET(emStatus) \
	if (emStatus != GX_STATUS_SUCCESS) \
								   {\
								   return emStatus;\
								   }\
extern CString		g_strShowMsg;

extern BOOL			g_bIsNewMsg;

//extern CCriticalSection g_csMsg;

extern double  m_anglex;
extern	double  m_angley;
extern	double  m_h;
extern	double  m_row;

extern int  m_anglex1;
extern	int  m_angley1;
extern	int  m_h1;
extern	int  m_row1;

class CCraneTest1Dlg : public CDialog
{
// Construction
public:
	CCraneTest1Dlg(CWnd* pParent = NULL);	// standard constructor

public:
	/// 回调函数
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);
	static void __stdcall OnDeviceOfflineCallbackFun(void *pUserParam);
	/// 初始化相机:设置默认参数
	GX_STATUS InitDevice();

	/// 设置相机的数据格式为8bit
    GX_STATUS SetPixelFormat8bit();

	/// 获取设备的宽高等属性信息
	GX_STATUS GetDeviceParam();
	GX_STATUS CloseDevice();
	/// 初始化UI界面
	void InitUI();

	/// 初始化组合框UI控件
	void InitEnumUI(GX_FEATURE_ID_CMD emFeatureID, CComboBox *pComboBox, bool bIsImplemented);

	/// 初始化增益等相关控件
	void InitGainUI();

	/// 初始化曝光相关控件
	void InitShutterUI();

	///  为彩色图像显示准备资源
	bool PrepareForShowColorImg();

	/// 为黑白图像显示准备资源
	bool PrepareForShowMonoImg();

	/// 为图像显示准备资源,分配Buffer
	bool PrepareForShowImg();

	/// 删除为图像显示准备的资源
	void UnPrepareForShowImg();
	//////////采集单帧图片超时时间
	UINT	m_nEditTimeOut;

	/// 显示图像
    void DrawImg(void *pImageBuf, int64_t nImageWidth, int64_t nImageHeight);
	void Rs485Send();
	/// 刷新UI
	void UpDateUI();
	void InitMat(Mat& m, float* num);
	/// 错误信息提示
	void ShowErrorString(GX_STATUS emErrorStatus);
	void OnShowEnterMessage(CString strMsg);
	//void SinglePictureProcess() ;
	void OnBnClickedBtnSendSoftCommand();
	void ShowImage(GX_FRAME_DATA* pFrameData);
	
	int  Otsu(Mat &image);
	/////////////图像处理线程
	xPublic::CThreadBase m_theadOpenCVProcess;
	xPublic::CThreadBase m_theadConnect;
	void SinglePictureProcess() ;

	static void CALLBACK OnThreadOpenCVProcess(LPVOID pParam, HANDLE hCloseEvent);
	static void CALLBACK OnThreadConnect(LPVOID lParam, HANDLE hCloseEvent);


public:
	GX_DEV_HANDLE           m_hDevice;          ///< 设备句柄
	GX_EVENT_CALLBACK_HANDLE  hCB;
	GX_FLOAT_RANGE          m_stShutterFloatRange;///< 保存曝光时间的范围
	GX_FLOAT_RANGE          m_stGainFloatRange;   ///< 保存增益值的范围
	
	int64_t                 m_nImageWidth;        ///< 相机输出图像宽
	int64_t                 m_nImageHeight;       ///< 相机输出图像高
	int64_t                 m_nMaxWid;  ///English:    ///Chinese: 分辨率最大宽度
	int64_t                 m_nMaxHei;  ///English:    ///Chinese: 分辨率最大高度
	int64_t                 m_nPayLoadSize;       ///< 相机输出原始图像大小
	int64_t                 m_nPixelColorFilter;  ///< Bayer格式:RAW图转换成RGB图像时使用的参数
	//int64_t                 m_nTriggerMode;       ///< 记录当前的触发模式
	BITMAPINFO             *m_pBmpInfo;	          ///< BITMAPINFO 结构指针，显示图像时使用

    BOOL                    m_bDevOpened;         ///< 标识设备是否已打开
    BOOL                    m_bIsSnap;            ///< 标识设备是否已开采
	bool                    m_bIsColorFilter;     ///< 判断相机是否支持Bayer格式

	BYTE                   *m_pRawBuffer;         ///< 存放原始RAW图的Buffer
	BYTE                   *m_pImgBuffer;         ///< 指向经过处理后的图像数据缓冲区

	char                    m_chBmpBuf[2048];     ///< BIMTAPINFO 存储缓冲区，m_pBmpInfo即指向此缓冲区

	bool                    m_bTriggerMode;       ///< 是否支持触发模式
	bool                    m_bTriggerActive;     ///< 是否支持触发极性
	bool                    m_bTriggerSource;     ///< 是否支持触发源
	bool                    m_bOutputSelector;    ///< 是否支持输出源
	bool                    m_bOutputMode;        ///< 是否支持输出模式
	bool                    m_bOutputValue;       ///< 是否支持输出值
	bool                    m_bStrobeSwitch;      ///< 是否支持闪光灯开关
	
	bool                    m_bLineSelector;      ///< 是否支持引脚选择
	bool                    m_bLineMode;          ///< 是否支持引脚方向
	bool                    m_bLineInverter;      ///< 是否支持引脚电平反转
	bool                    m_bLineSource;        ///< 是否支持引脚输出源
	bool                    m_bStrobeSwitchIsUse; ///< 闪光灯开关是否可用
	bool                    m_bNewFrame;          ///< 相机是否有新的图像帧
		GX_FRAME_DATA  m_stFrameData;       ///< GetImage获取的图像地址	

	// 图像显示资源定义
	CWnd                   *m_pWnd;             ///< 图像显示控件窗口指针
	HDC                     m_hDC;              ///< 图像显示窗口DC
	::CCriticalSection m_cs;
// Dialog Data
	//{{AFX_DATA(CCraneTest1Dlg)
	enum { IDD = IDD_CRANETEST1_DIALOG };
	double	m_dGainValue;
	double	m_dShutterValue;
	double   m_xpian;
	double   m_ypian;

	double	m_xValue1;
	double	m_xValue2;
	double	m_xValue3;
	double	m_yValue1;
	double	m_yValue2;
	double	m_yValue3;
	

	CString  s,s1,s2,s3;
	CString  m_xbiaoding; //X标定
	CString  m_ybiaoding; //Y标定
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCraneTest1Dlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCraneTest1Dlg)
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnOpenDevice();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnStartSnap();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnStopSnap();
	afx_msg void OnBnClickedBtnCloseDevice();
	afx_msg void OnKillfocusEditShutterValue();
	afx_msg void OnKillfocusEditGainValue();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnDisplayChange(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	
public:
	//afx_msg void OnStnClickedStaticShutter();
	
};

#endif // !defined(AFX_CraneTest1Dlg_H__26540673_FD9B_4799_889E_E05C3076016C__INCLUDED_)
