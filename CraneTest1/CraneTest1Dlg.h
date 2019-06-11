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
///< �궨���û�IO���ֵ 0:false 1:true
#define OUTPUT_ACTIVATION_FALSE  0
#define OUTPUT_ACTIVATION_TRUE   1

///< �궨�����ŵ�ƽ��תֵ 1:true 0:false
#define LINE_INVERTER_FALSE 0
#define LINE_INVERTER_TRUE  1
#define WM_DISPLAY_CHANGE (WM_USER + 100)

///< ������ʾ�����궨��
#define  GX_VERIFY(emStatus) \
	if (emStatus != GX_STATUS_SUCCESS)\
							{\
							ShowErrorString(emStatus); \
							return ;\
							 } 
///< �жϷ���ֵ�궨��
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
	/// �ص�����
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);
	static void __stdcall OnDeviceOfflineCallbackFun(void *pUserParam);
	/// ��ʼ�����:����Ĭ�ϲ���
	GX_STATUS InitDevice();

	/// ������������ݸ�ʽΪ8bit
    GX_STATUS SetPixelFormat8bit();

	/// ��ȡ�豸�Ŀ�ߵ�������Ϣ
	GX_STATUS GetDeviceParam();
	GX_STATUS CloseDevice();
	/// ��ʼ��UI����
	void InitUI();

	/// ��ʼ����Ͽ�UI�ؼ�
	void InitEnumUI(GX_FEATURE_ID_CMD emFeatureID, CComboBox *pComboBox, bool bIsImplemented);

	/// ��ʼ���������ؿؼ�
	void InitGainUI();

	/// ��ʼ���ع���ؿؼ�
	void InitShutterUI();

	///  Ϊ��ɫͼ����ʾ׼����Դ
	bool PrepareForShowColorImg();

	/// Ϊ�ڰ�ͼ����ʾ׼����Դ
	bool PrepareForShowMonoImg();

	/// Ϊͼ����ʾ׼����Դ,����Buffer
	bool PrepareForShowImg();

	/// ɾ��Ϊͼ����ʾ׼������Դ
	void UnPrepareForShowImg();
	//////////�ɼ���֡ͼƬ��ʱʱ��
	UINT	m_nEditTimeOut;

	/// ��ʾͼ��
    void DrawImg(void *pImageBuf, int64_t nImageWidth, int64_t nImageHeight);
	void Rs485Send();
	/// ˢ��UI
	void UpDateUI();
	void InitMat(Mat& m, float* num);
	/// ������Ϣ��ʾ
	void ShowErrorString(GX_STATUS emErrorStatus);
	void OnShowEnterMessage(CString strMsg);
	//void SinglePictureProcess() ;
	void OnBnClickedBtnSendSoftCommand();
	void ShowImage(GX_FRAME_DATA* pFrameData);
	
	int  Otsu(Mat &image);
	/////////////ͼ�����߳�
	xPublic::CThreadBase m_theadOpenCVProcess;
	xPublic::CThreadBase m_theadConnect;
	void SinglePictureProcess() ;

	static void CALLBACK OnThreadOpenCVProcess(LPVOID pParam, HANDLE hCloseEvent);
	static void CALLBACK OnThreadConnect(LPVOID lParam, HANDLE hCloseEvent);


public:
	GX_DEV_HANDLE           m_hDevice;          ///< �豸���
	GX_EVENT_CALLBACK_HANDLE  hCB;
	GX_FLOAT_RANGE          m_stShutterFloatRange;///< �����ع�ʱ��ķ�Χ
	GX_FLOAT_RANGE          m_stGainFloatRange;   ///< ��������ֵ�ķ�Χ
	
	int64_t                 m_nImageWidth;        ///< ������ͼ���
	int64_t                 m_nImageHeight;       ///< ������ͼ���
	int64_t                 m_nMaxWid;  ///English:    ///Chinese: �ֱ��������
	int64_t                 m_nMaxHei;  ///English:    ///Chinese: �ֱ������߶�
	int64_t                 m_nPayLoadSize;       ///< ������ԭʼͼ���С
	int64_t                 m_nPixelColorFilter;  ///< Bayer��ʽ:RAWͼת����RGBͼ��ʱʹ�õĲ���
	//int64_t                 m_nTriggerMode;       ///< ��¼��ǰ�Ĵ���ģʽ
	BITMAPINFO             *m_pBmpInfo;	          ///< BITMAPINFO �ṹָ�룬��ʾͼ��ʱʹ��

    BOOL                    m_bDevOpened;         ///< ��ʶ�豸�Ƿ��Ѵ�
    BOOL                    m_bIsSnap;            ///< ��ʶ�豸�Ƿ��ѿ���
	bool                    m_bIsColorFilter;     ///< �ж�����Ƿ�֧��Bayer��ʽ

	BYTE                   *m_pRawBuffer;         ///< ���ԭʼRAWͼ��Buffer
	BYTE                   *m_pImgBuffer;         ///< ָ�򾭹�������ͼ�����ݻ�����

	char                    m_chBmpBuf[2048];     ///< BIMTAPINFO �洢��������m_pBmpInfo��ָ��˻�����

	bool                    m_bTriggerMode;       ///< �Ƿ�֧�ִ���ģʽ
	bool                    m_bTriggerActive;     ///< �Ƿ�֧�ִ�������
	bool                    m_bTriggerSource;     ///< �Ƿ�֧�ִ���Դ
	bool                    m_bOutputSelector;    ///< �Ƿ�֧�����Դ
	bool                    m_bOutputMode;        ///< �Ƿ�֧�����ģʽ
	bool                    m_bOutputValue;       ///< �Ƿ�֧�����ֵ
	bool                    m_bStrobeSwitch;      ///< �Ƿ�֧������ƿ���
	
	bool                    m_bLineSelector;      ///< �Ƿ�֧������ѡ��
	bool                    m_bLineMode;          ///< �Ƿ�֧�����ŷ���
	bool                    m_bLineInverter;      ///< �Ƿ�֧�����ŵ�ƽ��ת
	bool                    m_bLineSource;        ///< �Ƿ�֧���������Դ
	bool                    m_bStrobeSwitchIsUse; ///< ����ƿ����Ƿ����
	bool                    m_bNewFrame;          ///< ����Ƿ����µ�ͼ��֡
		GX_FRAME_DATA  m_stFrameData;       ///< GetImage��ȡ��ͼ���ַ	

	// ͼ����ʾ��Դ����
	CWnd                   *m_pWnd;             ///< ͼ����ʾ�ؼ�����ָ��
	HDC                     m_hDC;              ///< ͼ����ʾ����DC
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
	CString  m_xbiaoding; //X�궨
	CString  m_ybiaoding; //Y�궨
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
