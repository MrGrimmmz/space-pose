// CraneTest1Dlg.cpp : implementation file
//
#define _CRTDBG_MAP_ALLOC   
#include <stdlib.h>   
#include <crtdbg.h>
#include "stdAfx.h"
#include "CraneTest1.h"
#include "CraneTest1Dlg.h"
#include < iostream > 
 #include < sstream > 
#include < string > 

//using namespace cv;
#define chans 3
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new new(_CLIENT_BLOCK,__FILE__,__LINE__)       //ע�⣬������__  ����_
#endif

CCriticalSection g_csMsg;
size_t nSize = 0;
BOOL		g_bIsNewMsg = FALSE;
CString		g_strShowMsg;

HANDLE      hCom;
double  m_anglex=0;
double  m_angley=0;
double  m_h=0;
double  m_row=0;

int  m_anglex1=0;
int  m_angley1=0;
int  m_h1=0;
int  m_row1=0;


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
CCriticalSection g_csMsg1;
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strProductVersion;
	CString	m_strLegalCopyRight;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strProductVersion = _T("");
	m_strLegalCopyRight = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_STATIC_PRODUCTVERSION, m_strProductVersion);
	DDX_Text(pDX, IDC_STATIC_LEGALCOPYRIGHT, m_strLegalCopyRight);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCraneTest1Dlg dialog

CCraneTest1Dlg::CCraneTest1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCraneTest1Dlg::IDD, pParent)
	, m_bDevOpened(false)
	, m_bIsSnap(false)
	, m_bIsColorFilter(false)
	, m_nPixelColorFilter(GX_COLOR_FILTER_NONE)
	, m_nImageWidth(0)
	, m_nImageHeight(0)
	, m_nPayLoadSize(0)
	, m_pImgBuffer(NULL)
	, m_pBmpInfo(NULL)
	, m_hDevice(NULL)
	, m_pWnd(NULL)
	, m_hDC(NULL)
	, m_xValue1(0)
	, m_xValue2(0)
	, m_xValue3(0)
	, m_yValue1(0)
	, m_yValue2(0)
	, m_yValue3(0)
	,m_xbiaoding("")
    ,m_ybiaoding("")
	//, m_theadOpenCVProcess(this,OnThreadOpenCVProcess)
	, m_theadConnect(this,OnThreadConnect)
{
	//{{AFX_DATA_INIT(CCraneTest1Dlg)
	m_dGainValue = 0.0;
	m_dShutterValue = 0.0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nEditTimeOut = 50;
	// ��ʼ������
	memset(&m_stFrameData,0,sizeof(GX_FRAME_DATA)); 
	memset(&m_stShutterFloatRange, 0, sizeof(m_stShutterFloatRange));
	memset(&m_stGainFloatRange, 0, sizeof(m_stGainFloatRange));
    memset(m_chBmpBuf, 0, sizeof(m_chBmpBuf));
}

void CCraneTest1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCraneTest1Dlg)
	DDX_Text(pDX, IDC_EDIT_GAIN_VALUE, m_dGainValue);
	DDX_Text(pDX, IDC_EDIT_SHUTTER_VALUE, m_dShutterValue);
	DDX_Text(pDX, IDC_EDIT6, m_xbiaoding);
	DDX_Text(pDX, IDC_EDIT7, m_ybiaoding);
	DDX_Text(pDX, IDC_EDIT_X1, m_xValue1);
	DDX_Text(pDX, IDC_EDIT_X2, m_xValue2);
	DDX_Text(pDX, IDC_EDIT_X3, m_xValue3);
	DDX_Text(pDX, IDC_EDIT_Y1, m_yValue1);
	DDX_Text(pDX, IDC_EDIT_Y2, m_yValue2);
	DDX_Text(pDX, IDC_EDIT_Y3, m_yValue3);
	DDX_Text(pDX, IDC_EDIT1, m_anglex);
	DDX_Text(pDX, IDC_EDIT4, m_angley);
	DDX_Text(pDX, IDC_EDIT3, m_h);
	DDX_Text(pDX, IDC_EDIT5, m_row);
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CCraneTest1Dlg, CDialog)
	//{{AFX_MSG_MAP(CCraneTest1Dlg)
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_OPEN_DEVICE, OnBnClickedBtnOpenDevice)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_START_SNAP, OnBnClickedBtnStartSnap)
	ON_BN_CLICKED(IDC_BTN_STOP_SNAP, OnBnClickedBtnStopSnap)
	ON_BN_CLICKED(IDC_BTN_CLOSE_DEVICE, OnBnClickedBtnCloseDevice)
	ON_EN_KILLFOCUS(IDC_EDIT_SHUTTER_VALUE, OnKillfocusEditShutterValue)
	ON_EN_KILLFOCUS(IDC_EDIT_GAIN_VALUE, OnKillfocusEditGainValue)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DISPLAY_CHANGE, OnDisplayChange)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCraneTest1Dlg message handlers

BOOL CCraneTest1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//CString title;
 //   title.Format("�Ͼ���Զ�г������");//�ڱ�������̬��ʾId��ֵ
 //   this->SetWindowText(title);
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// ����ʼ���豸��ʧ����ֱ���˳�Ӧ�ó���
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}

	// ��ȡͼ����ʾ��ص�UI��Դ

	
	m_pWnd       = GetDlgItem(IDC_STATIC_SHOW_WND);
	m_hDC        = ::GetDC(m_pWnd->m_hWnd);
	//

// ���½���ؼ�
	UpDateUI();


	hCom=CreateFile("COM4",GENERIC_READ|GENERIC_WRITE,NULL,0,OPEN_EXISTING,0,NULL);
	if(hCom==(HANDLE)-1)
	{AfxMessageBox("��COMʧ��");
	//exit(1);
	}
	SetupComm(hCom,1024,1024);
	COMMTIMEOUTS TimeOuts;
	TimeOuts.ReadIntervalTimeout=MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier=0;
	TimeOuts.ReadTotalTimeoutConstant=0;
	TimeOuts.WriteTotalTimeoutMultiplier=100;
	TimeOuts.WriteTotalTimeoutConstant=500;
	SetCommTimeouts(hCom,&TimeOuts);

	DCB dcb;
	GetCommState(hCom,&dcb);
	dcb.BaudRate=9600;
	dcb.ByteSize=8;
	dcb.Parity=NOPARITY;
	dcb.StopBits=ONESTOPBIT;
	SetCommState(hCom,&dcb);
	PurgeComm(hCom,PURGE_TXCLEAR|PURGE_RXCLEAR);

	///OnBnClickedBtnOpenDevice();
	//OnBnClickedBtnStartSnap();
    //OnBnClickedButton1();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCraneTest1Dlg::Rs485Send()
{
	/*GX_STATUS     emStatus = GX_STATUS_SUCCESS;
	uint32_t      nDevNum  = 0;
	emStatus = GXUpdateDeviceList(&nDevNum, 1000);
	GX_VERIFY(emStatus);*/
	
	int m_anglex2=5650;
	int m_angley2=5650;
	int m_h2=5650;
	int m_row2=5650;
	unsigned char lpOutBuffer[12]; 
	memset(lpOutBuffer,'\0',12); //ǰ12���ֽ������� 
	/*if(nDevNum<1)
	{*/
    // lpOutBuffer[0]=0x01;
	//}
	//else
	//{
		lpOutBuffer[0]=0x00;
	//}
	lpOutBuffer[1]=char((m_anglex1)&0x00ff);
	lpOutBuffer[2]=char((m_anglex1>>8)&0x00ff);
	lpOutBuffer[3]=char((m_angley1)&0x00ff);
	lpOutBuffer[4]=char((m_angley1>>8)&0x00ff);
	lpOutBuffer[5]=char((m_h1)&0x00ff);
	lpOutBuffer[6]=char((m_h1>>8)&0x00ff);
	lpOutBuffer[7]=char((m_row1)&0x00ff);  
	lpOutBuffer[8]=char((m_row1>>8)&0x00ff); 

	 char bcc=0x00;
     for(int i=0;i<9;i++)
     {
      bcc^=lpOutBuffer[i];
     };

	lpOutBuffer[9]=bcc; //У��λ
	lpOutBuffer[10]=0x0D; //
	lpOutBuffer[11]=0x0A; //

	DWORD dwBytesWrite=12; 
	COMSTAT ComStat; 
	DWORD dwErrorFlags; 
	BOOL bWriteStat; 
	ClearCommError(hCom,&dwErrorFlags,&ComStat); 
	bWriteStat=WriteFile(hCom,lpOutBuffer,dwBytesWrite,& dwBytesWrite,NULL); 
	if(!bWriteStat) 
	{ 
		//AfxMessageBox("д����ʧ��!"); 
	}   

}

void CALLBACK CCraneTest1Dlg::OnThreadConnect(LPVOID lParam, HANDLE hCloseEvent)
{
	CCraneTest1Dlg*  pThis = (CCraneTest1Dlg *)lParam;
	while ( WAIT_TIMEOUT == ::WaitForSingleObject(hCloseEvent,100) )
	{
		pThis->Rs485Send();
	}

}

void CCraneTest1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		char strFileName[MAX_PATH] = {'\0'};
		GetModuleFileName(NULL, strFileName, MAX_PATH);
		CFileVersion fv(strFileName);
		CAboutDlg dlgAbout;		
		dlgAbout.m_strProductVersion = _T("Version: ") + fv.GetProductVersion();
		dlgAbout.m_strLegalCopyRight = fv.GetLegalCopyright();
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCraneTest1Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCraneTest1Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//----------------------------------------------------------------------------------
/**
\brief  ���"���豸"��ť��Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::OnBnClickedBtnOpenDevice() 
{
	// TODO: Add your control notification handler code here


	GX_STATUS     emStatus = GX_STATUS_SUCCESS;
	uint32_t      nDevNum  = 0;
	bool      bRet     = true;
    GX_OPEN_PARAM stOpenParam;
    
	// ��ʼ�����豸ʹ�ò���,Ĭ�ϴ����Ϊ1���豸
    stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";
	
	// ö���豸
	emStatus = GXUpdateDeviceList(&nDevNum, 1000);
	GX_VERIFY(emStatus);

	 ::GetPrivateProfileString(_T("biaoding"),_T("X1"),"",m_xbiaoding.GetBuffer(MAX_PATH),MAX_PATH,_T("C:\\Users\\lenovo\\Documents\\Visual Studio 2010\\Projects\\CraneTest1\\CraneTest1\\CraneTest1.ini"));//��ȡxƫ��
	 ::GetPrivateProfileString(_T("biaoding"),_T("Y1"),"",m_ybiaoding.GetBuffer(MAX_PATH),MAX_PATH,_T("C:\\Users\\lenovo\\Documents\\Visual Studio 2010\\Projects\\CraneTest1\\CraneTest1\\CraneTest1.ini"));//��ȡxƫ��
	m_xpian=atof(m_xbiaoding);
	m_ypian=atof(m_ybiaoding);
	//PostMessage( WM_DISPLAY_CHANGE, 0, 0);
	
	SinglePictureProcess() ;
		if (nDevNum <= 0)
	{
	 
		CString strMsg1;
		strMsg1.Format("δ��⵽�������豸!");
		OnShowEnterMessage(strMsg1);
	
		return;
	}
	
	
	else if(nDevNum > 0)
	{
		GX_DEVICE_IP_INFO stIPInfo;
		//��ȡ��һ̨�豸��������Ϣ
		emStatus = GXGetDeviceIPInfo(1, &stIPInfo);

		CString strMsg2;
		strMsg2.Format("��⵽%d̨�������豸",nDevNum);
		OnShowEnterMessage(strMsg2);
	}

	///////////////////////////////����

	// ����豸�������ȹر�, ��֤�ڳ�ʼ������ʱ�����´�
	if(m_hDevice != NULL)
	{
		GXCloseDevice(m_hDevice);
		GX_VERIFY(emStatus);
		m_hDevice = NULL;
	}

	// ���豸
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	GX_VERIFY(emStatus);


	m_bDevOpened = TRUE;

	//PrepareForShowImg();
	// ���������Ĭ�ϲ���:�ɼ�ģʽ:�����ɼ�,���ݸ�ʽ:8-bit
	emStatus = InitDevice();
	if(emStatus == GX_STATUS_SUCCESS)
	{
		CString strMsg3;
			strMsg3.Format("��ʼ������ɹ���");
			OnShowEnterMessage(strMsg3);
	}
	else if (emStatus ==GX_STATUS_OFFLINE )
	{
		CString strMsg4;
			strMsg4.Format("������ߣ�");
			OnShowEnterMessage(strMsg4);

	}

	else 
	{
		CString strMsg5;
			strMsg5.Format("��ʼ�����ʧ�ܣ�");
			OnShowEnterMessage(strMsg5);
	}


	GX_VERIFY(emStatus);

	// ��ȡ�豸�Ŀ��ߵ�������Ϣ
	emStatus = GetDeviceParam();
	GX_VERIFY(emStatus);

		//Ϊͼ����ʾ׼����Դ
	bRet = PrepareForShowImg();
	if (!bRet)
	{
		CString strMsg6;
		strMsg6.Format("Ϊ��ʾͼ��׼����Դʧ��!");
		OnShowEnterMessage(strMsg6);
		return;
	}



	// ��ȡ�������,��ʼ������ؼ�
	InitUI();
	// ���½���ؼ�
	UpDateUI();

}


void CCraneTest1Dlg::OnShowEnterMessage(CString strMsg)
{
	g_csMsg.Lock();
	try
	{
		//������ʾ����
		CTime t = CTime::GetCurrentTime();
		g_strShowMsg += _T("\r\n") + t.Format("[%Y-%m-%d %H:%M:%S]") +strMsg;
		g_bIsNewMsg = TRUE;

		//�ı����Զ�������ʾ������
		SetDlgItemText(IDC_EDIT_SHOW,g_strShowMsg);
		((CEdit*)GetDlgItem(IDC_EDIT_SHOW))->SetSel(g_strShowMsg.GetLength(),g_strShowMsg.GetLength(),FALSE);

	}catch(...)
	{}
	g_csMsg.Unlock();
}



//----------------------------------------------------------------------------------
void CCraneTest1Dlg::UpDateUI()
{
	GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(!m_bDevOpened);
	GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_BTN_START_SNAP)->EnableWindow(m_bDevOpened && !m_bIsSnap);
	GetDlgItem(IDC_BTN_STOP_SNAP)->EnableWindow(m_bDevOpened && m_bIsSnap);

	GetDlgItem(IDC_EDIT_GAIN_VALUE)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_EDIT_SHUTTER_VALUE)->EnableWindow(m_bDevOpened);

	
}

//----------------------------------------------------------------------------------
/**
\brief  ����������Ϣ
\param  emErrorStatus  [in]������

\return �޷���ֵ
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize        = 0;
	GX_STATUS emStatus     = GX_STATUS_ERROR;
	
	// ��ȡ������Ϣ���ȣ��������ڴ�ռ�
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new char[nSize];
	if (NULL == pchErrorInfo)
	{
		return;
	}

	// ��ȡ������Ϣ������ʾ
	emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		MessageBox("GXGetLastError�ӿڵ���ʧ�ܣ�");
	}
	else
	{
		MessageBox((LPCTSTR)pchErrorInfo);
	}

	// �ͷ�������ڴ�ռ�
	if (NULL != pchErrorInfo)
	{
		delete[] pchErrorInfo;
		pchErrorInfo = NULL;
	}
}



//----------------------------------------------------------------------------------
/**
\brief  �ر�Ӧ�ó���

\return ��
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::OnClose() 
{
	//if ( IDOK != AfxMessageBox("ȷ��Ҫ�˳���", MB_OKCANCEL) )
	//{
	//	return;
	//}
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus = GX_STATUS_ERROR;

	if (hCom != NULL)
	{
		CloseHandle(hCom);
		hCom = NULL;
	}

if (m_bIsSnap)
	{
		// ����ֹͣ�ɼ�����
		emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_STOP);

		// ע���ص�

		m_bIsSnap = FALSE;

		// �ͷ�Ϊ�ɼ�ͼ�񿪱ٵ�ͼ��Buffer
		UnPrepareForShowImg();
	}

	// δ�ر��豸��ر��豸
	if (m_bDevOpened)
	{
		emStatus = GXCloseDevice(m_hDevice);
		m_bDevOpened = FALSE;
		m_hDevice    = NULL;
	}

	// �ر��豸��
	emStatus = GXCloseLib();

	
	// �ͷ�ͼ����ʾ���DC
	::ReleaseDC(m_pWnd->m_hWnd,m_hDC);	

	_CrtDumpMemoryLeaks();  
	CDialog::OnClose();
}

// ---------------------------------------------------------------------------------
/**
\brief   ��ʼ�����:����Ĭ�ϲ���

\return  GX_STATUS_SUCCESS:��ʼ���豸�ɹ�������:��ʼ���豸ʧ��
*/
// ----------------------------------------------------------------------------------
GX_STATUS CCraneTest1Dlg::InitDevice()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//���òɼ�ģʽ�����ɼ�
	emStatus = GXSetEnum(m_hDevice,GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
	VERIFY_STATUS_RET(emStatus);
	//���ô���ģʽΪ��
	emStatus = GXSetEnum(m_hDevice,GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_ON);
	VERIFY_STATUS_RET(emStatus);
	//����֡��
	emStatus = GXSetEnum(m_hDevice, GX_ENUM_ACQUISITION_FRAME_RATE_MODE, GX_ACQUISITION_FRAME_RATE_MODE_ON);
	emStatus = GXSetFloat(m_hDevice, GX_FLOAT_ACQUISITION_FRAME_RATE, 20.0);
	// ��֪��ǰ���֧���ĸ�8λͼ�����ݸ�ʽ����ֱ������
	// �����豸֧�����ݸ�ʽGX_PIXEL_FORMAT_BAYER_GR8,��ɰ������´���ʵ��
	// emStatus = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, GX_PIXEL_FORMAT_BAYER_GR8);
	// VERIFY_STATUS_RET(emStatus);

	// �������ǰ��������ݸ�ʽʱ�����Ե������º�����ͼ�����ݸ�ʽ����Ϊ8Bit
	emStatus = SetPixelFormat8bit(); 
	VERIFY_STATUS_RET(emStatus);

	//ѡ�񴥷�ԴΪ����
	emStatus = GXSetEnum(m_hDevice,GX_ENUM_TRIGGER_SOURCE, GX_TRIGGER_SOURCE_SOFTWARE);
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡ�豸�Ŀ�ߵ�������Ϣ

\return GX_STATUS_SUCCESS:ȫ����ȡ�ɹ�������״̬��:δ�ɹ���ȡȫ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CCraneTest1Dlg::GetDeviceParam()
{
	GX_STATUS emStatus       = GX_STATUS_ERROR;
	bool      bIsImplemented = false;

	// ��ȡͼ���С
	emStatus = GXGetInt(m_hDevice, GX_INT_PAYLOAD_SIZE, &m_nPayLoadSize);
	VERIFY_STATUS_RET(emStatus);

	// ��ȡ���
	emStatus = GXGetInt(m_hDevice, GX_INT_WIDTH, &m_nImageWidth);
	VERIFY_STATUS_RET(emStatus);

	// ��ȡ�߶�
	emStatus = GXGetInt(m_hDevice, GX_INT_HEIGHT, &m_nImageHeight);
	VERIFY_STATUS_RET(emStatus);

	// �ж�����Ƿ�֧��bayer��ʽ
	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &m_bIsColorFilter);
	VERIFY_STATUS_RET(emStatus);
	m_bIsColorFilter = bIsImplemented;
	// ֧�ֱ�ʾ�����ɫͼ��
	if(bIsImplemented)
	{
		emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &m_nPixelColorFilter);
		VERIFY_STATUS_RET(emStatus);
	}
    
	return emStatus;
}

// ---------------------------------------------------------------------------------
/**
\brief   ������������ݸ�ʽΪ8bit

\return  emStatus GX_STATUS_SUCCESS:���óɹ�������:����ʧ��
*/
// ----------------------------------------------------------------------------------
GX_STATUS CCraneTest1Dlg::SetPixelFormat8bit()
{
	GX_STATUS emStatus    = GX_STATUS_SUCCESS;
	int64_t   nPixelSize  = 0;
	uint32_t  nEnmuEntry  = 0;
	size_t    nBufferSize = 0;
	BOOL      bIs8bit     = TRUE;

	GX_ENUM_DESCRIPTION  *pEnumDescription = NULL;
	GX_ENUM_DESCRIPTION  *pEnumTemp        = NULL;

	// ��ȡ���ص��С
	emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_SIZE, &nPixelSize);
	VERIFY_STATUS_RET(emStatus);

	// �ж�Ϊ8bitʱֱ�ӷ���,��������Ϊ8bit
	if (nPixelSize == GX_PIXEL_SIZE_BPP8)
	{
		return GX_STATUS_SUCCESS;
	}
	else
	{
		// ��ȡ�豸֧�ֵ����ظ�ʽö����
		emStatus = GXGetEnumEntryNums(m_hDevice, GX_ENUM_PIXEL_FORMAT, &nEnmuEntry);
		VERIFY_STATUS_RET(emStatus);

		// Ϊ��ȡ�豸֧�ֵ����ظ�ʽö��ֵ׼����Դ
		nBufferSize      = nEnmuEntry * sizeof(GX_ENUM_DESCRIPTION);
		pEnumDescription = new GX_ENUM_DESCRIPTION[nEnmuEntry];

		// ��ȡ֧�ֵ�ö��ֵ
		emStatus         = GXGetEnumDescription(m_hDevice, GX_ENUM_PIXEL_FORMAT, pEnumDescription, &nBufferSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			if (pEnumDescription != NULL)
			{
				delete []pEnumDescription;
				pEnumDescription = NULL;
			}

			return emStatus;
		}

		// �����豸֧�ֵ����ظ�ʽ,�������ظ�ʽΪ8bit,
		// ���豸֧�ֵ����ظ�ʽΪMono10��Mono8��������ΪMono8
		for (uint32_t i = 0; i < nEnmuEntry; i++)
		{
			if ((pEnumDescription[i].nValue & GX_PIXEL_8BIT) == GX_PIXEL_8BIT)
			{
				emStatus = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, pEnumDescription[i].nValue);
				break;
			}
		}	

		// �ͷ���Դ
		if (pEnumDescription != NULL)
		{
			delete []pEnumDescription;
			pEnumDescription = NULL;
		}
	}

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief ��ʼ������ģʽCombox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitTriggerModeUI()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//    
//	// �ж��豸�Ƿ�֧�ִ���ģʽ
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_TRIGGER_MODE, &m_bTriggerMode);
//	GX_VERIFY(emStatus);
//	if (!m_bTriggerMode)
//	{
//		return;
//	}
//
//	// ��ʼ������ģʽCombox��
//	InitEnumUI(GX_ENUM_TRIGGER_MODE, (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_MODE), m_bTriggerMode);
//}

//----------------------------------------------------------------------------------
/**
\brief  ��ʼ������ԴCombox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitTriggerSourceUI()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//
//	// �ж��豸�Ƿ�֧�ִ���Դ
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_TRIGGER_SOURCE, &m_bTriggerSource);
//	GX_VERIFY(emStatus);
//	if (!m_bTriggerSource)
//	{
//		return;
//	}
//
//	// ��ʼ������ԴCombox��
//    InitEnumUI(GX_ENUM_TRIGGER_SOURCE, (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_SOURCE),m_bTriggerSource);
//}

//----------------------------------------------------------------------------------
/**
\brief ��ʼ����������Combox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitTriggerActivationUI()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//
//	// �ж��豸�Ƿ�֧�ִ�������
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_TRIGGER_ACTIVATION, &m_bTriggerActive);
//	GX_VERIFY(emStatus);
//	if (!m_bTriggerActive)
//	{
//		return;
//	}
//
//	// ��ʼ����������Combox��
//    InitEnumUI(GX_ENUM_TRIGGER_ACTIVATION, (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVATION), m_bTriggerActive);
//}

//----------------------------------------------------------------------------------
/**
\brief ��ʼ������ƿ���Combox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitStrobeSwitchUI()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//
//	// �ж��豸�Ƿ�֧������ƿ���
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_STROBE_SWITCH, &m_bStrobeSwitch);
//	GX_VERIFY(emStatus);
//	if (!m_bStrobeSwitch)
//	{
//		return;
//	}
//
//	// ��ʼ������ƿ���Combox��
//	InitEnumUI(GX_ENUM_STROBE_SWITCH, (CComboBox *)GetDlgItem(IDC_COMBO_STROBE_SWITCH),m_bStrobeSwitch);
//}

//----------------------------------------------------------------------------------
/**
\brief ��ʼ���û�IO���ģʽCombox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitOutputModeUI()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//
//	// �ж��豸�Ƿ�֧���û�IO���ģʽ
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_USER_OUTPUT_MODE, &m_bOutputMode);
//	GX_VERIFY(emStatus);
//	if (!m_bOutputMode)
//	{
//		return;
//	}
//
//	// ��ʼ���û�IO���ģʽCombox��
//    InitEnumUI(GX_ENUM_USER_OUTPUT_MODE, (CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_MODE), m_bOutputMode);
//}

//----------------------------------------------------------------------------------
/**
\brief  ��ʼ���û��Զ������ѡ��Combox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitOutputSelectorUI()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//
//	// �ж��豸�Ƿ�֧���û�IO���ѡ��
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_USER_OUTPUT_SELECTOR, &m_bOutputSelector);
//	GX_VERIFY(emStatus);
//	if (!m_bOutputSelector)
//	{
//		return;
//	}
//
//	// ��ʼ���û�IO���ѡ��Combox��
//	InitEnumUI(GX_ENUM_USER_OUTPUT_SELECTOR,(CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_SELECTOR),m_bOutputSelector);
//}

//----------------------------------------------------------------------------------
/**
\brief  ��ʼ��"����ѡ��"Combox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitLineSelector()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//
//	// �ж��豸�Ƿ�֧������ѡ��
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_LINE_SELECTOR, &m_bLineSelector);
//	GX_VERIFY(emStatus);
//	if (!m_bLineSelector)
//	{
//		return;
//	}
//
//	// ��ʼ������ѡ��
//	InitEnumUI(GX_ENUM_LINE_SELECTOR, (CComboBox *)GetDlgItem(IDC_COMBO_LINE_SELECTOR), m_bLineSelector);
//}

//----------------------------------------------------------------------------------
/**
\brief  ��ʼ��"���ŷ���"Combox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitLineMode()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//
//	// �ж��豸�Ƿ�֧�����ŷ���
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_LINE_MODE, &m_bLineMode);
//	GX_VERIFY(emStatus);
//	if (!m_bLineMode)
//	{
//		return;
//	}
//
//	// ��ʼ�����ŷ���
//	InitEnumUI(GX_ENUM_LINE_MODE, (CComboBox *)GetDlgItem(IDC_COMBO_LINE_MODE), m_bLineMode);
//
//}

//----------------------------------------------------------------------------------
/**
\brief  ��ʼ��"���ŵ�ƽ��ת"Combox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitLineInverter()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//	bool      bTemp    = true;
//
//	CComboBox *pLineInverter = (CComboBox *)GetDlgItem(IDC_COMBO_LINE_INVERTER);
//
//	// �ж��豸�Ƿ�֧�����ŵ�ƽ��ת
//	emStatus = GXIsImplemented(m_hDevice, GX_BOOL_LINE_INVERTER, &m_bLineInverter);
//	GX_VERIFY(emStatus);
//	if (!m_bLineInverter)
//	{
//		return;
//	}
//
//	// ��ʼ�����ŵ�ƽ��ת
//	pLineInverter->ResetContent();
//	pLineInverter->SetItemData(pLineInverter->AddString("false"), LINE_INVERTER_FALSE);
//	pLineInverter->SetItemData(pLineInverter->AddString("true"), LINE_INVERTER_TRUE);
//
//	// ��ȡ��ǰ�豸�����ŵ�ƽ��ת��ֵ����ѡ����µ�������
//	emStatus = GXGetBool(m_hDevice, GX_BOOL_LINE_INVERTER, &bTemp);
//	GX_VERIFY(emStatus);
//	if (bTemp)
//	{
//		pLineInverter->SetCurSel(LINE_INVERTER_TRUE);
//	}
//	else
//	{
//		pLineInverter->SetCurSel(LINE_INVERTER_FALSE);
//	}
//}

//----------------------------------------------------------------------------------
/**
\brief  ��ʼ��"�������Դ"Combox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitLineSource()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//
//	// �ж��豸�Ƿ�֧���������Դ
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_LINE_SOURCE, &m_bLineSource);
//	GX_VERIFY(emStatus);
//	if (!m_bLineSource)
//	{
//		return;
//	}
//
//	// ��ʼ���������Դ
//	InitEnumUI(GX_ENUM_LINE_SOURCE, (CComboBox *)GetDlgItem(IDC_COMBO_LINE_SOURCE), m_bLineSource);
//}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ��UI����

\return  ��
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::InitUI()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	int  nValue = 0;
    

	InitShutterUI();

	
	InitGainUI();
	// ��ʼ��������ؿؼ�
	// ��ʼ������ѡ��ComBox�ؼ�
	//InitLineSelector();

	// ��ʼ������ģʽComBox�ؼ�
	//InitLineMode();

	// ��ʼ�����ŵ�ƽ��תComBox�ؼ�
	//InitLineInverter();

	// ��ʼ���������ԴComBox�ؼ�
	//InitLineSource();
		// ��ʼ���û��Զ������ֵѡ��
    //InitOutputValue();

	// ����ģʽ����ѡ��Combox�ؼ�
   // InitTriggerModeUI();
	
	// ��ʼ������ԴCombox�ؼ�
	//InitTriggerSourceUI();

	// ��ʼ����������Combox�ؼ�
	//InitTriggerActivationUI();

	// ��ʼ������ƿ���Combox�ؼ�
	//InitStrobeSwitchUI();

	// ��ʼ���û�IO���ģʽCombox�ؼ�
	//InitOutputModeUI();

	// ��ʼ���û��Զ������ѡ��ComBox�ؼ�
	//InitOutputSelectorUI();
	
	// ��ʼ���ع�ʱ����ؿؼ�
	UpdateData(false);
}

//----------------------------------------------------------------------------------
/**
\brief  ��ʼ��"�û��Զ������ֵ"Combox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitOutputValue()
//{
//	CComboBox *pBox           = (CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_VALUE);
//	GX_STATUS  emStatus       = GX_STATUS_SUCCESS;
//	bool       bVal           = false;
//
//	// ��ʼ���������������б�
//	emStatus = GXIsImplemented(m_hDevice,GX_BOOL_USER_OUTPUT_VALUE, &m_bOutputValue);
//	GX_VERIFY(emStatus);
//	if (!m_bOutputValue)
//	{
//		return;
//	}
//
//	// ��ʼ����ǰ�ؼ��Ŀ�ѡ��
//	pBox->ResetContent();
//	pBox->SetItemData(pBox->AddString("false"), OUTPUT_ACTIVATION_FALSE);
//	pBox->SetItemData(pBox->AddString("true"), OUTPUT_ACTIVATION_TRUE);
//
//	// ��ȡö���͵ĵ�ǰֵ,����Ϊ���浱ǰ��ʾֵ
//	emStatus = GXGetBool(m_hDevice, GX_BOOL_USER_OUTPUT_VALUE, &bVal);
//	GX_VERIFY(emStatus);
//	if (bVal)
//	{
//		pBox->SetCurSel(OUTPUT_ACTIVATION_TRUE);
//	}
//	else
//	{
//		pBox->SetCurSel(OUTPUT_ACTIVATION_FALSE);
//	}
//}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ����Ͽ�UI����
\param   emFeatureID    [in]    ������ID
\param   pComboBox      [in]    �ؼ�ָ��
\param   bIsImplemented [in]    ��ʶ�Ƿ�֧��emFeatureID����Ĺ���

\return  ��
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::InitEnumUI(GX_FEATURE_ID_CMD emFeatureID, CComboBox *pComboBox, bool bIsImplemented)
{
	// �жϿؼ�
	if ((pComboBox == NULL))
	{
		return;
	}

	GX_ENUM_DESCRIPTION *pEnum       = NULL;
	GX_STATUS emStatus       = GX_STATUS_ERROR;
	size_t    nbufferSize    = 0;
	uint32_t  nEntryNum      = 0;
	int64_t   nEnumValue     = -1;
	int       nCurcel        = 0;

	if (!bIsImplemented)
	{
		return;
	}

	// ��ȡ���ܵ�ö����
	emStatus = GXGetEnumEntryNums(m_hDevice, emFeatureID, &nEntryNum);
	GX_VERIFY(emStatus);

	// Ϊ��ȡö���͵Ĺ�����������ռ�
	nbufferSize = nEntryNum * sizeof(GX_ENUM_DESCRIPTION);
	pEnum       = new GX_ENUM_DESCRIPTION[nEntryNum];
	if (pEnum == NULL)
	{
		MessageBox("���仺����ʧ�ܣ�");
		return;
	}

	// ��ȡ���ܵ�ö������
	emStatus = GXGetEnumDescription(m_hDevice, emFeatureID, pEnum, &nbufferSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		if (pEnum != NULL)
		{  
			delete []pEnum;
			pEnum = NULL;
		}

		return;
	}

	// ��ȡö���͵ĵ�ǰֵ,����Ϊ���浱ǰ��ʾֵ
	emStatus = GXGetEnum(m_hDevice, emFeatureID, &nEnumValue);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		if (pEnum != NULL)
		{  
			delete []pEnum;
			pEnum = NULL;
		}

		return;
	}

	// ��ʼ����ǰ�ؼ��Ŀ�ѡ��
	pComboBox->ResetContent();
	for (uint32_t i = 0; i < nEntryNum; i++)
	{	
		pComboBox->SetItemData(pComboBox->AddString(pEnum[i].szSymbolic), (uint32_t)pEnum[i].nValue);
		if (pEnum[i].nValue == nEnumValue)
		{
			nCurcel = i;
		}
	}

	// ���õ�ǰֵΪ�������ʾֵ
	pComboBox->SetCurSel(nCurcel);

	// �ͷź�����Դ
	if (pEnum != NULL)
	{  
		delete []pEnum;
		pEnum = NULL;
	}
}

////---------------------------------------------------------------------------------
///**
//\brief   �л�"����Դ"ѡ����Ӧ����
//
//\return  ��
//*/
////----------------------------------------------------------------------------------
//void CCraneTest1Dlg::OnCbnSelchangeComboTriggerSource()
//{
//	// TODO: Add your control notification handler code here
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_SOURCE);
//
//	int      nIndex    = pBox->GetCurSel();                   // ��ȡ��ǰѡ��
//	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // ��ȡ��ǰѡ���Ӧ��ö����ֵ
//
//	// ����ǰѡ��Ĵ���Դ��ö��ֵ���õ������
//	emStatus = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_SOURCE, nEnumVal);
//	GX_VERIFY(emStatus);
//}

////----------------------------------------------------------------------------------
///**
//\brief  �л�"����ģʽ"Combox����Ϣ��Ӧ����
//
//\return ��
//*/
////----------------------------------------------------------------------------------
//void CCraneTest1Dlg::OnCbnSelchangeComboTriggerMode()
//{
//	// TODO: Add your control notification handler code here
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//	bool bIsWritable   = TRUE;
//	int64_t  nCurrentEnumValue = 0;
//	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_MODE);
//
//	int      nIndex    = pBox->GetCurSel();                     // ��ȡ��ǰѡ��
//	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);    // ��ȡ��ǰѡ���Ӧ��ö����ֵ
//
//	// �жϴ���ģʽ�Ƿ��д
//	emStatus = GXIsWritable(m_hDevice,GX_ENUM_TRIGGER_MODE,&bIsWritable);
//	GX_VERIFY(emStatus);
//
//	// ��ȡ����ģʽ�ĵ�ǰֵ
//	emStatus = GXGetEnum(m_hDevice, GX_ENUM_TRIGGER_MODE, &nCurrentEnumValue);
//	GX_VERIFY(emStatus);
//
//	if (bIsWritable)
//	{
//		// ����ǰѡ��Ĵ���ģʽ��ֵ���õ������
//		emStatus = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_MODE, nEnumVal);
//		GX_VERIFY(emStatus);
//
//		// ���µ�ǰ����ģʽ
//		m_nTriggerMode = nEnumVal;
//	}
//	else
//	{
//		MessageBox("��ǰ״̬����д����ֹͣ�ɼ�������������!");
//		if (nCurrentEnumValue != nEnumVal)
//		{
//			for (int i = 0;i < pBox->GetCount();i++)
//			{
//				if (nCurrentEnumValue == pBox->GetItemData(i))
//				{
//					pBox->SetCurSel(i);
//					break;
//				}
//			}
//		}
//		else
//		{
//			return;
//		}
//	}
//
//	UpDateUI();
//}

////----------------------------------------------------------------------------------
///**
//\brief  ���"������ͼ"��ť��Ϣ��Ӧ����
//
//\return ��
//*/
////----------------------------------------------------------------------------------
//void CCraneTest1Dlg::OnBnClickedBtnSendSoftTrigger()
//{
//	// TODO: Add your control notification handler code here
//	//������������(�ڴ���ģʽΪOn�ҿ�ʼ�ɼ�����Ч)
//	GX_STATUS emStatus = GX_STATUS_ERROR;
//	emStatus = GXSendCommand(m_hDevice, GX_COMMAND_TRIGGER_SOFTWARE);
//
//	GX_VERIFY(emStatus);
//}

//----------------------------------------------------------------------------------
/**
\brief  �л�"��������"Combox����Ϣ��Ӧ����

\return ��
*/
////----------------------------------------------------------------------------------
//void CCraneTest1Dlg::OnCbnSelchangeComboTriggerActivation()
//{
//	// TODO: Add your control notification handler code here
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVATION);
//
//	int      nIndex    = pBox->GetCurSel();                   // ��ȡ��ǰѡ��
//	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // ��ȡ��ǰѡ���Ӧ��ö����ֵ
//
//	// ����ǰѡ��Ĵ������Ե�ֵ���õ������
//	emStatus = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_ACTIVATION, nEnumVal);
//	GX_VERIFY(emStatus);
//}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ���ع���ؿؼ�

\return  ��
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::InitShutterUI()
{
	CStatic     *pStcShutterShow = (CStatic *)GetDlgItem(IDC_STATIC_SHUTTER);
    CEdit       *pEditShutterVal = (CEdit *)GetDlgItem(IDC_EDIT_SHUTTER_VALUE);

	// �жϿؼ�����Ч��
	if ((pEditShutterVal == NULL) || (pStcShutterShow == NULL))
	{
		return;
	}

    GX_STATUS      emStatus = GX_STATUS_ERROR;
	CString        strTemp  = "";
	double      dShutterVal = 0.0;

	// ��ȡ�����ͷ�Χ,����ʼ��Edit�ؼ���static��Χ��ʾ��
	emStatus = GXGetFloatRange(m_hDevice, GX_FLOAT_EXPOSURE_TIME, &m_stShutterFloatRange);
	GX_VERIFY(emStatus);

    strTemp.Format("�ع�(%.4f~%.4f)%s",m_stShutterFloatRange.dMin, m_stShutterFloatRange.dMax, m_stShutterFloatRange.szUnit);
	pStcShutterShow->SetWindowText(strTemp);

	// ��ȡ��ǰֵ������ǰֵ���µ�����
	emStatus = GXGetFloat(m_hDevice, GX_FLOAT_EXPOSURE_TIME, &dShutterVal);
	GX_VERIFY(emStatus);

	m_dShutterValue = dShutterVal;

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ��������ؿؼ�
 
\return  ��
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::InitGainUI()
{
	CStatic     *pStcGain     = (CStatic *)GetDlgItem(IDC_STATIC_GAIN);
	CEdit       *pEditGainVal = (CEdit *)GetDlgItem(IDC_EDIT_GAIN_VALUE);

	if ((pStcGain == NULL) || (pEditGainVal == NULL))
	{
		return;
	}
    
	GX_STATUS emStatus = GX_STATUS_ERROR;
	CString   strRange = "";
	double    dGainVal = 0;

	// ��ȡ�����ͷ�Χ,����ʼ��Edit�ؼ���static��Χ��ʾ��
	emStatus = GXGetFloatRange(m_hDevice, GX_FLOAT_GAIN, &m_stGainFloatRange);
	GX_VERIFY(emStatus);

	// ��ʾ��Χ
    strRange.Format("����(%.4f~%.4f)%s", m_stGainFloatRange.dMin, m_stGainFloatRange.dMax, m_stGainFloatRange.szUnit);
	pStcGain->SetWindowText(strRange);
	
	// ��ȡ��ǰֵ�����µ�����
	emStatus = GXGetFloat(m_hDevice, GX_FLOAT_GAIN, &dGainVal);
	GX_VERIFY(emStatus);

	m_dGainValue = dGainVal;
   
	UpdateData(FALSE);
}

//----------------------------------------------------------------------------------
/**
\brief  ���"��ʼ�ɼ�"��ť��Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::OnBnClickedBtnStartSnap()
{
	///GX_STATUS_ERROR
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;


	emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_START);
	//////////////////////////////////////////һ��ʼ�е�
	if (emStatus != GX_STATUS_SUCCESS)
	{
		UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}
	m_bIsSnap = TRUE;
	CString strMsg8;
	strMsg8.Format("��ʼʵʱ�ɼ�ͼ��");
	OnShowEnterMessage(strMsg8);

	

	// ���½���UI
    UpDateUI();


	//OnBnClickedButton1();

	
}

//----------------------------------------------------------------------------------
/**
\brief  ���"������ͼ"��ť��Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::OnBnClickedBtnSendSoftCommand()
{
		
	if (!UpdateData(TRUE))
	{
		m_nEditTimeOut = 100;
		return;
	}
	GX_STATUS emStatus  = GX_STATUS_ERROR;

	//ÿ�η��ʹ�������֮ǰ��ղɼ��������
	//��ֹ���ڲ�����֡����ɱ���GXGetImage�õ���ͼ�����ϴη��ʹ����õ���ͼ
	/*emStatus = GXFlushQueue(m_hDevice);
	GX_VERIFY(emStatus);*/
	emStatus = GXSendCommand(m_hDevice,GX_COMMAND_TRIGGER_SOFTWARE);
	GX_VERIFY(emStatus);

	//////////////////////////////////
	//��ȡͼ��
	CString str1;
	str1.Format(_T("��ʼ����"));
	xPublic::WriteFileDebug(_T("12"),str1);
	emStatus = GXGetImage(m_hDevice, &m_stFrameData, m_nEditTimeOut);



	str1.Format(_T("���Խ���"));
	xPublic::WriteFileDebug(_T("12"),str1);

	//�ж�GXGetImage����ֵ
	GX_VERIFY(emStatus);
		

	////////////////////////////////////////////////////����ͼ��
	 ShowImage(&m_stFrameData);
	
}

//----------------------------------------------------------------------------------
/**
\brief  Ϊ����ͼ�����Buffer,Ϊͼ����ʾ׼����Դ
\return true:Ϊͼ����ʾ׼����Դ�ɹ�  false:׼����Դʧ��
*/
//----------------------------------------------------------------------------------
bool  CCraneTest1Dlg::PrepareForShowImg()
{	
	bool bRet = true;
	bool bRet1 = true;
		//ΪGetImage��������ռ�
     m_stFrameData.pImgBuf     = new BYTE[(size_t)m_nPayLoadSize];
	if (m_stFrameData.pImgBuf == NULL)
	{
		bRet1 = false;
	}
	
	if (m_bIsColorFilter)   
	{
		// Ϊ��ɫͼ����ʾ׼����Դ,����Buffer
		bRet = PrepareForShowColorImg();
	}
	else  
	{
		// Ϊ�ڰ�ͼ����ʾ׼����Դ,����Buffer
		bRet = PrepareForShowMonoImg();
	}

	// ������ʧ�����ͷ��ѷ������Դ
	if (bRet==false||bRet1==false)
	{
		UnPrepareForShowImg();
	}

	return bRet;
}

//----------------------------------------------------------------------------------
/**
\brief  Ϊ��ɫͼ����ʾ׼����Դ,����Buffer

\return true:Ϊͼ����ʾ׼����Դ�ɹ�  false:׼����Դʧ��
*/
//----------------------------------------------------------------------------------
bool  CCraneTest1Dlg::PrepareForShowColorImg()
{	
	//--------------------------------------------------------------------
	//---------------------------��ʼ��bitmapͷ---------------------------
	m_pBmpInfo								= (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth			= (LONG)m_nImageWidth;
	m_pBmpInfo->bmiHeader.biHeight			= (LONG)m_nImageHeight;	

	m_pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pBmpInfo->bmiHeader.biBitCount		= 24; // ��ɫͼ��Ϊ24,�ڰ�ͼ��Ϊ8
	m_pBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pBmpInfo->bmiHeader.biClrImportant	= 0;


	//---------------------------------------------------------------------
	//----------------------------Ϊͼ�����ݷ���Buffer---------------------

	// Ϊ����RGBת�����ͼ�����Buffer
	m_pImgBuffer = new BYTE[(size_t)(m_nImageWidth * m_nImageHeight * 3)];
	if (m_pImgBuffer == NULL)
	{
		return false;
	}




	return true;
}

//----------------------------------------------------------------------------------
/**
\brief Ϊ�ڰ�ͼ����ʾ׼����Դ,����Buffer

\return true:Ϊͼ����ʾ׼����Դ�ɹ�  false:׼����Դʧ��
*/
//----------------------------------------------------------------------------------
bool  CCraneTest1Dlg::PrepareForShowMonoImg()
{	
	//--------------------------------------------------------------------
	//---------------------------��ʼ��bitmapͷ---------------------------
	m_pBmpInfo								= (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth			= (LONG)m_nImageWidth;
	m_pBmpInfo->bmiHeader.biHeight			= (LONG)m_nImageHeight;	

	m_pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pBmpInfo->bmiHeader.biBitCount		= 8;  // ��ɫͼ��Ϊ24,�ڰ�ͼ��Ϊ8
	m_pBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pBmpInfo->bmiHeader.biClrImportant	= 0;

	if (!m_bIsColorFilter)
	{
		// �ڰ������Ҫ���г�ʼ����ɫ�����
		for(int i=0;i<256;i++)
		{
			m_pBmpInfo->bmiColors[i].rgbBlue	 = i;
			m_pBmpInfo->bmiColors[i].rgbGreen	 = i;
			m_pBmpInfo->bmiColors[i].rgbRed		 = i;
			m_pBmpInfo->bmiColors[i].rgbReserved = i;
		}
	}
	//---------------------------------------------------------------------
	//----------------------------Ϊͼ�����ݷ���Buffer---------------------
	// �ڰ�ͼ��Buffer����
	m_pImgBuffer = new BYTE[(size_t)(m_nImageWidth * m_nImageHeight)];
	
	if (m_pImgBuffer == NULL)
	{
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------
/**
\brief  ��ʼ������ڲξ���
\param  

\return ��
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::InitMat(Mat& m, float* num)
{
    for (int i = 0; i<m.rows; i++)
    for (int j = 0; j<m.cols; j++)
        m.at<float>(i, j) = *(num + i * m.rows + j);
}

//----------------------------------------------------------------------------------
/**
\brief  ����ȡ����ͼ���������ݺڰײ�ɫ�Ĳ�ͬ���д�����ʾ������
\param  pFrameData  [in] ָ���û������ͼ�����ݵĵ�ַָ��

\return ��
*/
//----------------------------------------------------------------------------------


cv::Mat fast_ergodic4(cv::Mat image) {	
	if (!image.isContinuous())	{		
		return image;	
	}	
	int rows = image.rows;	
	int cols = image.cols;	/*ʹ�ó�����ȱ����죬��˵�ͼ��ͨ��ȷ��ʱ���뽫chans��Ϊ���ֳ�������1��3��4��*/	//
	//int chans = image.channels();	
	cv::Mat outImage = cv::Mat::zeros(image.size(), image.type());	
	cols = cols*rows*chans;	
	uchar* inData = image.data;	
	uchar* outData = outImage.data;	
	for (int j = 0; j < cols; j += chans)	
	{		
		outData[j] = 10 + inData[j];//B		
		outData[j + 1] = 10 + inData[j + 1];//G		
		outData[j + 2] = 10 + inData[j + 2];//R		//
		//outData[j + 3] = 10 + inData[j + 3];//A	
	}	
	return outImage;
}




void  CCraneTest1Dlg::ShowImage(GX_FRAME_DATA* pFrameData)
{
	
	// ��ȡ����ͼ���Ƿ�Ϊ������ͼ��
	if (pFrameData->nStatus != 0)    
	{
		return;
	}

	//��֧�ֲ�ɫ,ת��ΪRGBͼ������
	if (m_bIsColorFilter)
	{
		//��Raw8ͼ��ת��ΪRGBͼ���Թ���ʾ
		DxRaw8toRGB24((BYTE*)pFrameData->pImgBuf, m_pImgBuffer, (VxUint32)m_nImageWidth, (VxUint32)m_nImageHeight, RAW2RGB_NEIGHBOUR,
			DX_PIXEL_COLOR_FILTER(m_nPixelColorFilter), TRUE);
	}
	else   
	{
		// �ڰ������Ҫ��ת���ݺ���ʾ
		for(int i =0;i <m_nImageHeight;i++)
		{
			memcpy(m_pImgBuffer+i*m_nImageWidth, (BYTE *)(pFrameData->pImgBuf)+(m_nImageHeight-i-1)*m_nImageWidth,(size_t)m_nImageWidth);
		}
	}

	
	//��ָ��������һ֡һ֡�Ļ�ͼ
	DrawImg(m_pImgBuffer, m_nImageWidth, m_nImageHeight);

//
	//////////////////////////////////////////////////////////////�����������////////////////////////////////////////////////
	//float neican_data[] = { 6229.480256699168, 0, 753.3600831270645, 0, 6208.898904029267, 353.5292143532171, 0, 0, 1 };

	//float jibian_data[] = { -0.725636018154933, 71.59698241520449, -0.01102894789291404, -0.009635933311296271, -2360.33301002783 };
	///////////xuyao///////////////////////////////////////////////////ͼ������////////////////////////////////////////////////
	int thresh = 30; 
    int max_thresh = 255;

	double u0=0.0;
	double v0=0.0;
	double u1=0.0;
	double v1=0.0;
	int square=0;
	double h=0.0;
	int a=0;
	int b=0;
	int c=0;
	double anglex=0.0;
	double angley=0.0;
	double l=0.0;
	double length=0.0;
	double truex=0.0;
	double truey=0.0;
	double xtruel=0.0;
	double ytruel=0.0;
	double vertical=0.0;
	double angleA=0.0;
	double angleB=0.0;
	double angleC=0.0;
	double maxangle=0.0;
	double k1=0.0;
	double row=0.0;
	double trueh=0.0;
	cv::setUseOptimized(true);
	Mat src_gray;
	IplImage *img=cvCreateImage(cvSize(m_nImageWidth, m_nImageHeight), 8, 1);
	memcpy(img->imageData, m_pImgBuffer, m_nImageWidth*m_nImageHeight);
	src_gray=cvarrToMat(img);
	//src_gray.convertTo(src_gray, CV_32FC3);
	//cvtColor( src_gray, src_gray, CV_BGR2GRAY );//�ҶȻ�

	Mat dst;

	//////////////////////buxuyao/////////////////������ʼ////////////////////////////////////////////////////
	//Mat R = Mat::eye(3, 3, CV_32F);
	//Size image_size;  /* ͼ��ĳߴ� */
 //   //��ȡͼ���С
 //   image_size.width = m_nImageWidth;
 //   image_size.height = m_nImageHeight;
	//Mat mapx = Mat(image_size, CV_32FC1);
 //   Mat mapy = Mat(image_size, CV_32FC1);
	//Mat cameraMatrix(3, 3, CV_32FC1);
 //   InitMat(cameraMatrix, neican_data);

	//Mat distCoeffs(1, 5, CV_32FC1); /* �������5������ϵ����k1,k2,p1,p2,k3 */
 //   InitMat(distCoeffs, jibian_data);
	//initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix, image_size, CV_32FC1, mapx, mapy);

 //   Mat imageSource = src_gray; //��ȡ����ͼƬ
 //   Mat newimage = imageSource.clone(); //У�������ͼƬ
 //   remap(imageSource, newimage, mapx, mapy, INTER_LINEAR);
	//int threshold_white = Otsu(src_gray );//��ֵ���㣬����otsu
	////////////////////////////////////////////��������/////////////////////////////////////////////////////////////////
		

	GaussianBlur( src_gray, src_gray, Size(3,3), 3, 3, BORDER_DEFAULT );//////////////newimage
	Mat binary;
	threshold(src_gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);/////////newiamge
	Mat canny_output;

	//���ñ�����ͨ�������
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	/////////////////////////////////////////////////////////////////////////////////��Լ60ms
	///��ȡ����� 
	Mat k = getStructuringElement(MORPH_RECT, Size(5,5));
	////�Զ�ֵͼ����п�����
	morphologyEx(binary, binary, MORPH_OPEN, k);

	

	//CString strMsg1,strMsg2;
	//strMsg1.Format(("��ʼѰ����ͨ��"));	
	findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

		//�ж��Ƿ������ͨ��
			if(0==contours.size()||contours.size()>3)
			{
				/*CString strMsg;
				strMsg.Format("δ�ҵ����ʵ���ͨ��!");
				OnShowEnterMessage(strMsg);*/
				m_xValue1=0;
				m_xValue2=0;
				m_xValue3=0;
				m_yValue1=0;
				m_yValue2=0;
				m_yValue3=0;
				PostMessage( WM_DISPLAY_CHANGE, 0, 0);
				return;
			}
			else{
			
				/*CString strMsg;
				strMsg.Format(("��%d����ͨ��!"),contours.size());
				OnShowEnterMessage(strMsg);*/
			}

///�����
	vector<Moments> mu(contours.size() );
	for( int i = 0; i < contours.size(); i++ ) 		
	{ 		
		mu[i] = moments( contours[i], false ); 	
	} 		
	//�������������� 	�Ҷ����ķ�	
	vector<Point2f> mc( contours.size() ); 	
	Point pt[10000]; 
	for( int i = 0; i < contours.size(); i++ ) 		
	{ 	
		if (mu[i].m00 != 0)						   //��������Ϊ0
		{
			mc[i].x = cvRound(mu[i].m10 / mu[i].m00);//���ĺ�����
			mc[i].y = cvRound(mu[i].m01 / mu[i].m00);//����������
		}
	}  	
	switch (contours.size())
	{
	case 0:
		m_xValue1=0;
		m_xValue2=0;
		m_xValue3=0;
		m_yValue1=0;
		m_yValue2=0;
		m_yValue3=0;
		m_anglex=0;
		m_angley=0;
		m_h=0;

		break;
	case 1:
		m_xValue1=mc[0].x;
		m_xValue2=0;
		m_xValue3=0;
		m_yValue1=mc[0].y;
		m_yValue2=0;
		m_yValue3=0;
		m_anglex=0;
		m_angley=0;
		m_h=0;
		break;
	case 2:
		m_xValue1=mc[0].x;
		m_xValue2=mc[1].x;
		m_xValue3=0;
		m_yValue1=mc[0].y;
		m_yValue2=mc[1].y;
		m_yValue3=0;
		m_anglex=0;
		m_angley=0;
		m_h=0;
		break;

	case 3:
		m_xValue1=mc[0].x;
		m_xValue2=mc[1].x;
		m_xValue3=mc[2].x;
		m_yValue1=mc[0].y;
		m_yValue2=mc[1].y;
		m_yValue3=mc[2].y;



		
///////////////////////////////////////////////////////////////�Ƕ��㷨////////////////////////////////////////////
	square=-0.5*(m_xValue1*m_yValue2+m_xValue2*m_yValue3+m_xValue3*m_yValue1-m_xValue1*m_yValue3-m_xValue2*m_yValue1-m_xValue3*m_yValue2);
	h=255.9*(pow(square,-0.4845))-0.1108;
	trueh=h/(cos((m_xpian*3.1416)/180));
	length=368.1*(pow(h,-0.9771))-0.07858;

	u0=753+((h*tan((m_xpian*3.1416)/180)*length)/0.07);
	v0=566+((h*tan((m_xpian*3.1416)/180)*length)/0.07);
	a=sqrt((m_yValue3-m_yValue2)*(m_yValue3-m_yValue2)+(m_xValue3-m_xValue2)*(m_xValue3-m_xValue2));
	b=sqrt((m_yValue3-m_yValue1)*(m_yValue3-m_yValue1)+(m_xValue3-m_xValue1)*(m_xValue3-m_xValue1));
	c=sqrt((m_yValue2-m_yValue1)*(m_yValue2-m_yValue1)+(m_xValue2-m_xValue1)*(m_xValue2-m_xValue1));
	u1=(a*m_xValue1+b*m_xValue2+c*m_xValue3)/(a+b+c);
	v1=(a*m_yValue1+b*m_yValue2+c*m_yValue3)/(a+b+c);
	/////////////�Զ�У��//////
	//double  xpian =u1-u0;
	//double  ypian =v1-v0;

	//l=sqrt((u0-u1)*(u0-u1)+(v0-v1)*(v0-v1));
	
	truex=(0.07*(u1-u0))/length;
	truey=(0.07*(v1-v0))/length;

	//vertical=sqrt(h*h-truel*truel);
	if(c>a&&c>b)
	{
		k1=(m_yValue3-(m_yValue1+m_yValue2)/2)/(m_xValue3-(m_xValue1+m_xValue2)/2);
		row=atan(k1)*180.0/3.1416;
	}
	if(a>c&&a>b)
	{
		k1=(m_yValue1-(m_yValue2+m_yValue3)/2)/(m_xValue1-(m_xValue2+m_xValue3)/2);
		row=atan(k1)*180.0/3.1416;
	}
	if(b>c&&b>a)
	{
		k1=(m_yValue2-(m_yValue1+m_yValue3)/2)/(m_xValue2-(m_xValue1+m_xValue3)/2);
		row=atan(k1)*180.0/3.1416;
	}

	//xtruel=(0.1*(u1-u0))/length;
	anglex=atan(truex/h)*180.0/3.1416;
	//ytruel=(0.1*(v1-v0))/length;
	angley=atan(truey/h)*180.0/3.1416-1;


	m_anglex=anglex;
	m_angley=angley;
	m_h=h;
	if(row>=0)
	{
		m_row=row;
	}
	else
	{
		m_row=row+360;
	}
		break;
	default:


		m_xValue1=0;
		m_xValue2=0;
		m_xValue3=0;
		m_yValue1=0;
		m_yValue2=0;
		m_yValue3=0;
		m_anglex=0;
		m_angley=0;
		m_h=0;
		break;
	}
	
	//////////////������Դ����ͬ���߳�///////////////////
	m_cs.Lock();
	try
	{
		m_anglex1=m_anglex*1000;
		m_angley1=m_angley*1000;
		m_h1=m_h*1000;
		m_row1=m_row*100;
	}
	catch(...)
	{}
	m_cs.Unlock();

	CString s,s1,s2,s3;
	s.Format(_T("%.3f"),m_anglex);
	s1.Format(_T("%.3f"),m_angley);
	s2.Format(_T("%.3f"),m_h);
	s3.Format(_T("%.3f"),m_row);
	////////////////////////////////////////////��Ϣ��Ӧ��������edit�ؼ�������ʾ
	PostMessage( WM_DISPLAY_CHANGE, 0, 0);

	
	////////////////////////////////////////////////////////////////////////////////
	CString str2;
	str2.Format(_T("x��Ƕȣ�%s,y��Ƕȣ�%s,�߶ȣ�%s,��ת�Ƕȣ�%s"),s,s1,s2,s3);
	xPublic::WriteFileDebug(_T("11"),str2);
	//��ʾ�Ҷ���������
	/*Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 ); 		
	for( int i = 0; i< contours.size(); i++ ) 		
	{ 				
		Scalar color = Scalar( 255, 0, 0); 				
		drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() ); 				
		circle( drawing, mc[i], 5, Scalar( 0, 0, 255), -1, 8, 0 );		 				
		rectangle(drawing, boundingRect(contours.at(i)), cvScalar(0,255,0)); 					
		char tam[100]; 		
		sprintf(tam, "(%0.0f,%0.0f)",mc[i].x,mc[i].y); 		
		putText(drawing, tam, Point(mc[i].x, mc[i].y), FONT_HERSHEY_SIMPLEX, 1, cvScalar(255,0,255),2); 
	}*/		
	//namedWindow( "Contours",CV_WINDOW_NORMAL ); 		
	//imshow( "Contours", drawing ); 		
	//moveWindow("Contours",20,20); 	
	//src.release(); 	

	src_gray.release();
	cvReleaseImage(&img);
	
}
//---------------------------------------------------------------------------------
/**
\brief   ������߻ص�����
\param   pFrame      �ص�����

\return  ��
*/
//----------------------------------------------------------------------------------
void __stdcall CCraneTest1Dlg::OnDeviceOfflineCallbackFun(void *pUserParam)
{
	CCraneTest1Dlg *pDlg = (CCraneTest1Dlg*)(pUserParam);

	pDlg->OnBnClickedBtnStopSnap();
}





//---------------------------------------------------------------------------------
/**
\brief   �ɼ�ͼ��ص�����
\param   pFrame      �ص�����

\return  ��
*/
//----------------------------------------------------------------------------------
//void __stdcall CCraneTest1Dlg::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
//{
//	CCraneTest1Dlg *pDlg = (CCraneTest1Dlg*)(pFrame->pUserParam);
//	VxUint32 nImageHeight = (VxUint32)pDlg->m_nImageHeight;
//	VxUint32 nImageWidth  = (VxUint32)pDlg->m_nImageWidth;
//
//	if (pFrame->status == 0)
//	{
//		memcpy(pDlg->m_pRawBuffer,pFrame->pImgBuf,pFrame->nImgSize);
//
//		pDlg->m_bNewFrame = true;
//
//		if (pDlg->m_bIsColorFilter)
//		{
//			// ��ɫͼ����Ҫ��Raw8ͼ�񾭹�RGBת������ʾ
//			DxRaw8toRGB24(pDlg->m_pRawBuffer, pDlg->m_pImgBuffer, nImageWidth , nImageHeight, RAW2RGB_NEIGHBOUR,
//				           DX_PIXEL_COLOR_FILTER(pDlg->m_nPixelColorFilter), TRUE);
//			//pDlg->DrawImg(pDlg->m_pImgBuffer);
//		}
//		else
//		{
//			// �ڰ������Ҫ��ת���ݺ���ʾ
//			for(VxUint32 i =0;i <nImageHeight;i++)
//			{
//				memcpy(pDlg->m_pImgBuffer+i*nImageWidth, pDlg->m_pRawBuffer+(nImageHeight-i-1)*nImageWidth,(size_t)nImageWidth);
//			}
//
//			//pDlg->DrawImg(pDlg->m_pImgBuffer);
//		}		
//	}
//}

//---------------------------------------------------------------------------------
/**
\brief   ��ʾͼ��
\param   pImageBuffer   [in]   Ҫ��ʾ��ͼ������

\return  ��
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::DrawImg(void *pImageBuf, int64_t nImageWidth, int64_t nImageHeight)
{
	int nWndWidth  = 0;
	int nWndHeight = 0;

	// Ϊ��ͼ��׼��
	RECT objRect;
	m_pWnd->GetClientRect(&objRect);
	nWndWidth  = objRect.right - objRect.left;
	nWndHeight = objRect.bottom - objRect.top;

	// ������ø���䣬����ͼ�����ˮ��
	::SetStretchBltMode(m_hDC, COLORONCOLOR);
	::StretchDIBits(m_hDC,
		            0,						
		            0,
		            nWndWidth,
		            nWndHeight,
		            0,
		            0,
		           (int)nImageWidth,
		           (int)nImageHeight,
		            pImageBuf,
		            m_pBmpInfo,
		            DIB_RGB_COLORS,
		            SRCCOPY
		            );	
}

//----------------------------------------------------------------------------------
/**
\brief  �ͷ�Ϊͼ����ʾ׼������Դ

\return ��
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::UnPrepareForShowImg()
{
	m_xbiaoding.ReleaseBuffer();
	m_ybiaoding.ReleaseBuffer();


	if (m_stFrameData.pImgBuf != NULL)
	{
		delete []m_stFrameData.pImgBuf;
		m_stFrameData.pImgBuf = NULL;
	}



	if (m_pImgBuffer != NULL)
	{
		
		delete []m_pImgBuffer;
		m_pImgBuffer = NULL;
	}

}

//----------------------------------------------------------------------------------
/**
\brief  �л�"����ƿ���"Combox����Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::OnCbnSelchangeComboStrobeSwitch()
//{
//	// TODO: Add your control notification handler code here
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_STROBE_SWITCH);
//
//	int      nIndex    = pBox->GetCurSel();                   // ��ȡ��ǰѡ��
//	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // ��ȡ��ǰѡ���Ӧ��ö����ֵ
//
//	// ����ǰѡ�������ƿ��ص�ֵ���õ������
//	emStatus = GXSetEnum(m_hDevice, GX_ENUM_STROBE_SWITCH, nEnumVal);
//	GX_VERIFY(emStatus);
//}

//----------------------------------------------------------------------------------
/**
\brief  ���"ֹͣ�ɼ�"��ť��Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::OnBnClickedBtnStopSnap()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;


	//����ֹͣ�ɼ�����
	emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_STOP);
	GX_VERIFY(emStatus);
	m_bIsSnap = FALSE;

	
	CString strMsg9;
	strMsg9.Format("����ͣ�ɼ�ͼ��");
	OnShowEnterMessage(strMsg9);
	// ���½���UI
	UpDateUI();

	m_theadConnect.SetClose();
	m_theadConnect.StopThread();
	KillTimer(1); 
		
}



//----------------------------------------------------------------------------------
/**
\brief  �ر��豸

\return �ر��豸�ɹ��򷵻�GX_STATUS_SUCCESS�����򷵻ش�����
*/
//----------------------------------------------------------------------------------
GX_STATUS CCraneTest1Dlg::CloseDevice()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// �ͷ���Դ
    UnPrepareForShowImg();
	
	// �ر��豸
	emStatus = GXCloseDevice(m_hDevice);
		
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ���"�ر��豸"��ť��Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::OnBnClickedBtnCloseDevice()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	// δͣ��ʱ��ֹͣ�ɼ�
	if (m_bIsSnap)
	{
		//����ֹͣ�ɼ�����
		emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_STOP);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// ������
		}
		m_bIsSnap = false;
	}
	
	//�ر����
	UnPrepareForShowImg();
	
	// �ر��豸
	emStatus = GXCloseDevice(m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// ������
	}

	m_bDevOpened = false;
	m_hDevice    = NULL;

	CString strMsg10;
	strMsg10.Format("�豸�ѹر�");
	OnShowEnterMessage(strMsg10);

	// ���½���UI
	UpDateUI();


}

//------------------------------------------------------
/**
\brief   ��Ӧ�ع�Edit�ؼ�ʧȥ����ĺ���

\return  ��
*/
//------------------------------------------------------
void CCraneTest1Dlg::OnKillfocusEditShutterValue() 
{
	// TODO: Add your control notification handler code here
	//�жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
	if (m_hDevice == NULL)
	{
		return;
	}

	UpdateData();
	GX_STATUS status = GX_STATUS_SUCCESS;

	//�ж�����ֵ�Ƿ����ع�ʱ��ķ�Χ��
	//���������ֵ���ع�ֵ��Ϊ���ֵ
	if (m_dShutterValue > m_stShutterFloatRange.dMax)
	{
		m_dShutterValue = m_stShutterFloatRange.dMax;
	}
	//��С����Сֵ���ع�ֵ��Ϊ��Сֵ
	if (m_dShutterValue < m_stShutterFloatRange.dMin)
	{
		m_dShutterValue = m_stShutterFloatRange.dMin;
	}

	// �����ع��ֵ
	status = GXSetFloat(m_hDevice,GX_FLOAT_EXPOSURE_TIME,m_dShutterValue);
	GX_VERIFY(status);

	UpdateData(FALSE);
}

//------------------------------------------------------
/**
\brief   ��Ӧ����Edit�ؼ�ʧȥ����ĺ���

  \return  ��
*/
//------------------------------------------------------
void CCraneTest1Dlg::OnKillfocusEditGainValue() 
{
	// TODO: Add your control notification handler code here
	//�жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
	if (m_hDevice == NULL)
	{
		return;
	}

	UpdateData();
	GX_STATUS status = GX_STATUS_SUCCESS;

	//�ж�����ֵ�Ƿ�������ֵ�ķ�Χ��
	//�������ֵ�������ֵ������ֵ���ó����ֵ
	if (m_dGainValue > m_stGainFloatRange.dMax)
	{
		m_dGainValue = m_stGainFloatRange.dMax;
	}

	//�������ֵС����Сֵ�������ֵ���ó���Сֵ
	if (m_dGainValue < m_stGainFloatRange.dMin)
	{
		m_dGainValue = m_stGainFloatRange.dMin;
	}

	// ���������ֵ
	status = GXSetFloat(m_hDevice,GX_FLOAT_GAIN,m_dGainValue);
	GX_VERIFY(status);

	UpdateData(FALSE);
}

//------------------------------------------------------
/**
\brief   �ַ�����(��Ҫ������̵Ļس���Ϣ)
\param   pMsg  ��Ϣ�ṹ��

\return  �ɹ���TRUE   ʧ�ܣ�FALSE
*/
//------------------------------------------------------
BOOL CCraneTest1Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pWnd  = NULL;
	int nCtrlID = 0;               //�����ȡ�Ŀؼ�ID

	//�ж��Ƿ��Ǽ��̻س���Ϣ
	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
	{
		//��ȡ��ǰӵ�����뽹��Ĵ���(�ؼ�)ָ��
		pWnd = GetFocus();

		//��ȡ�������λ�õĿؼ�ID
		nCtrlID = pWnd->GetDlgCtrlID();

		//�ж�ID����
		switch (nCtrlID)
		{
		case IDC_EDIT_SHUTTER_VALUE:
		case IDC_EDIT_GAIN_VALUE:

			//ʧȥ����
			SetFocus();

			break;

		default:
			break;
		}

		return TRUE;
	}

	if ((WM_KEYDOWN == pMsg->message) && (VK_ESCAPE == pMsg->wParam))
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CALLBACK CCraneTest1Dlg::OnThreadOpenCVProcess(LPVOID pParam, HANDLE hCloseEvent)
{
	///////////��ò�һ��֡�����Ƕ�ʱ��////////////////////////////
	//100msȡһ�Ρ�
	CCraneTest1Dlg*  pThis   = (CCraneTest1Dlg *)pParam;

	while ( WAIT_TIMEOUT == ::WaitForSingleObject(hCloseEvent,10) )
	{

		pThis->OnBnClickedBtnSendSoftCommand();
    }
}


LRESULT CCraneTest1Dlg::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{   
	g_csMsg1.Lock();
//����Edit  
    SetDlgItemInt(IDC_EDIT_X1, m_xValue1);
	SetDlgItemInt(IDC_EDIT_X2, m_xValue2);
	SetDlgItemInt(IDC_EDIT_X3, m_xValue3);
	SetDlgItemInt(IDC_EDIT_Y1, m_yValue1);
	SetDlgItemInt(IDC_EDIT_Y2, m_yValue2);
	SetDlgItemInt(IDC_EDIT_Y3, m_yValue3);
	
	CString s,s1,s2,s3;
	s.Format(_T("%.3f"),m_anglex);
	s1.Format(_T("%.3f"),m_angley);
	s2.Format(_T("%.3f"),m_h);
	s3.Format(_T("%.3f"),m_row);

	SetDlgItemText(IDC_EDIT1, s);
	SetDlgItemText(IDC_EDIT4, s1);
	SetDlgItemText(IDC_EDIT3, s2);
	SetDlgItemText(IDC_EDIT5, s3);
	SetDlgItemText(IDC_EDIT6, m_xbiaoding);
	SetDlgItemText(IDC_EDIT7, m_ybiaoding);



	g_csMsg.Unlock();
	return 0 ;
}

void CCraneTest1Dlg::OnTimer(UINT nIDEvent) 
{
	

	OnBnClickedBtnSendSoftCommand();
    
	
	CDialog::OnTimer(nIDEvent);
	
}


void CCraneTest1Dlg::SinglePictureProcess() 
{

	int thresh = 30; 
    int max_thresh = 255;
	Mat src_gray;
	double u0=0.0;
	double v0=0.0;
	double u1=0.0;
	double v1=0.0;
	int square=0;
	double h=0.0;
	int a=0;
	int b=0;
	int c=0;
	double anglex=0.0;
	double angley=0.0;
	double l=0.0;
	double length=0.0;
	double truex=0.0;
	double truey=0.0;
	double xtruel=0.0;
	double ytruel=0.0;
	double vertical=0.0;
	double angleA=0.0;
	double angleB=0.0;
	double angleC=0.0;
	double maxangle=0.0;
	double k1=0.0;
	double row=0.0;
	double trueh=0.0;
	//������ȷ���һ���ݴ�ռ�
	//***********************************�µ�ͼ�񷽰�****************************************//
	Mat src = imread( "C:/Users/lenovo/Documents/Visual Studio 2010/Projects/CraneTest1/CraneTest1/10.bmp" );
	//IplImage* img1;

	cvtColor( src, src_gray, CV_BGR2GRAY );//�ҶȻ�
		int threshold_white = Otsu(src_gray );//��ֵ���㣬����otsu
	//��˹ģ���˲�
	GaussianBlur( src_gray, src_gray, Size(3,3), 3, 3, BORDER_DEFAULT );
	//blur( src_gray, src_gray, Size(3,3) ); //�˲�
	/////////����ת��Ϊiplimage���и�ʴ�����Ͳ���
	/*IplImage* pBinary = &IplImage(src_gray);*/
	//img1 = cvCreateImage(cvSize(pBinary->width,pBinary->height),8,1);
	//cvErode(img1, img1, 0, 1);
	//Mat ImgTemp;
 //  ImgTemp=cvarrToMat(img1);
	Mat binary;
	//��ֵ��
	threshold(src_gray, binary, threshold_white, 255, THRESH_BINARY | THRESH_OTSU);
	namedWindow( "threshold",CV_WINDOW_NORMAL ); 		
	imshow( "threshold", binary ); 		
	moveWindow("threshold",500,20);
	//Mat se(5,5,CV_8U,Scalar(1));
	Mat canny_output;
	//���ñ�����ͨ�������
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//canny��ȡ����
	Canny( binary, canny_output, 30, 100, 3 );
	///��ȡ����� 
	Mat k = getStructuringElement(MORPH_RECT, Size(5,5));
	//dilate(canny_output,canny_output,k,Point(-1,-1),1);
	////�Զ�ֵͼ����п�����
	morphologyEx(binary, binary, MORPH_OPEN, k);
	//morphologyEx(canny_output, canny_output, MORPH_BLACKHAT, k);
    // erode(binary,binary,k,Point(-1,-1),2);
	namedWindow( "Canny",CV_WINDOW_NORMAL ); 		
	imshow( "Canny", binary ); 		
	moveWindow("Canny",500,20);
	///Ѱ����ͨ�������
	findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		//�ж��Ƿ������ͨ��
			if(0==contours.size())
			{
				CString strMsg11;
				strMsg11.Format("δ�ҵ����ʵ���ͨ��!");
				OnShowEnterMessage(strMsg11);
				return;
			}
			else{
				CString strMsg;
				strMsg.Format(("��%d����ͨ��!"),contours.size());
				OnShowEnterMessage(strMsg);
			}
    ///�����
	vector<Moments> mu(contours.size() );
	for( int i = 0; i < contours.size(); i++ ) 		
	{ 		
		mu[i] = moments( contours[i], false ); 	
	} 		
	//�������������� 	�Ҷ����ķ�	
	vector<Point2f> mc( contours.size() ); 	
	Point pt[10000]; 
	for( int i = 0; i < contours.size(); i++ ) 		
	{ 	
		if (mu[i].m00 != 0)						   //��������Ϊ0
		{
			mc[i].x = cvRound(mu[i].m10 / mu[i].m00);//���ĺ�����
			mc[i].y = cvRound(mu[i].m01 / mu[i].m00);//����������
		}
	}  	
	switch (contours.size())
	{
	case 0:
		m_xValue1=0;
		m_xValue2=0;
		m_xValue3=0;
		m_yValue1=0;
		m_yValue2=0;
		m_yValue3=0;
		break;
	case 1:
		m_xValue1=mc[0].x;
		m_yValue1=mc[0].y;
		break;
	case 2:
		m_xValue1=mc[0].x;
		m_xValue2=mc[1].x;
		m_yValue1=mc[0].y;
		m_yValue2=mc[1].y;
		break;

	case 3:
		m_xValue1=mc[0].x;
		m_xValue2=mc[1].x;
		m_xValue3=mc[2].x;
		m_yValue1=mc[0].y;
		m_yValue2=mc[1].y;
		m_yValue3=mc[2].y;


		///////////////////////////////////////////////////////////////�Ƕ��㷨////////////////////////////////////////////
	square=-0.5*(m_xValue1*m_yValue2+m_xValue2*m_yValue3+m_xValue3*m_yValue1-m_xValue1*m_yValue3-m_xValue2*m_yValue1-m_xValue3*m_yValue2);
	h=255.9*(pow(square,-0.4845))-0.1108;
	trueh=h/(cos((m_xpian*3.1416)/180));
	length=368.1*(pow(h,-0.9771))-0.07858;

	u0=753+((h*tan((m_xpian*3.1416)/180)*length)/0.07);
	v0=566+((h*tan((m_xpian*3.1416)/180)*length)/0.07);
	a=sqrt((m_yValue3-m_yValue2)*(m_yValue3-m_yValue2)+(m_xValue3-m_xValue2)*(m_xValue3-m_xValue2));
	b=sqrt((m_yValue3-m_yValue1)*(m_yValue3-m_yValue1)+(m_xValue3-m_xValue1)*(m_xValue3-m_xValue1));
	c=sqrt((m_yValue2-m_yValue1)*(m_yValue2-m_yValue1)+(m_xValue2-m_xValue1)*(m_xValue2-m_xValue1));
	u1=(a*m_xValue1+b*m_xValue2+c*m_xValue3)/(a+b+c);
	v1=(a*m_yValue1+b*m_yValue2+c*m_yValue3)/(a+b+c);
	/////////////�Զ�У��//////
	//double  xpian =u1-u0;
	//double  ypian =v1-v0;

	//l=sqrt((u0-u1)*(u0-u1)+(v0-v1)*(v0-v1));
	
	truex=(0.07*(u1-u0))/length;
	truey=(0.07*(v1-v0))/length;

	//vertical=sqrt(h*h-truel*truel);
	if(c>a&&c>b)
	{
		k1=(m_yValue3-(m_yValue1+m_yValue2)/2)/(m_xValue3-(m_xValue1+m_xValue2)/2);
		row=atan(k1)*180.0/3.1416;
	}
	if(a>c&&a>b)
	{
		k1=(m_yValue1-(m_yValue2+m_yValue3)/2)/(m_xValue1-(m_xValue2+m_xValue3)/2);
		row=atan(k1)*180.0/3.1416;
	}
	if(b>c&&b>a)
	{
		k1=(m_yValue2-(m_yValue1+m_yValue3)/2)/(m_xValue2-(m_xValue1+m_xValue3)/2);
		row=atan(k1)*180.0/3.1416;
	}

	//xtruel=(0.1*(u1-u0))/length;
	anglex=atan(truex/h)*180.0/3.1416;
	//ytruel=(0.1*(v1-v0))/length;
	angley=atan(truey/h)*180.0/3.1416-1;


	m_anglex=anglex;
	m_angley=angley;
	m_h=h;
	if(row>=0)
	{
		m_row=row;
	}
	else
	{
		m_row=row+360;
	}
		break;
	default:


		m_xValue1=0;
		m_xValue2=0;
		m_xValue3=0;
		m_yValue1=0;
		m_yValue2=0;
		m_yValue3=0;
		m_anglex=0;
		m_angley=0;
		m_h=0;
		break;
	}
	
	//////////////������Դ����ͬ���߳�///////////////////
	m_cs.Lock();
	try
	{
		m_anglex1=m_anglex*1000;
		m_angley1=m_angley*1000;
		m_h1=m_h*1000;
		m_row1=m_row*100;
	}
	catch(...)
	{}
	m_cs.Unlock();

	CString s,s1,s2,s3;
	s.Format(_T("%.3f"),m_anglex);
	s1.Format(_T("%.3f"),m_angley);
	s2.Format(_T("%.3f"),m_h);
	s3.Format(_T("%.3f"),m_row);
	////////////////////////////////////////////��Ϣ��Ӧ��������edit�ؼ�������ʾ
	PostMessage( WM_DISPLAY_CHANGE, 0, 0);

	



	//��ʾ�Ҷ���������
	Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 ); 		
	for( int i = 0; i< contours.size(); i++ ) 		
	{ 				
		Scalar color = Scalar( 255, 0, 0); 				
		drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() ); 				
		circle( drawing, mc[i], 5, Scalar( 0, 0, 255), -1, 8, 0 );		 				
		rectangle(drawing, boundingRect(contours.at(i)), cvScalar(0,255,0)); 					
		char tam[100]; 		
		sprintf(tam, "(%0.0f,%0.0f)",mc[i].x,mc[i].y); 		
		putText(drawing, tam, Point(mc[i].x, mc[i].y), FONT_HERSHEY_SIMPLEX, 1, cvScalar(255,0,255),2); 
	}		
	namedWindow( "Contours",CV_WINDOW_NORMAL ); 		
	imshow( "Contours", drawing ); 		
	moveWindow("Contours",20,20); 	
	src.release(); 	
	src_gray.release();
}


int CCraneTest1Dlg:: Otsu(Mat &image)
{
    int width = image.cols;
    int height = image.rows;
    int x = 0, y = 0;
    int pixelCount[256];
    float pixelPro[256];
    int i, j, pixelSum = width * height, threshold = 0;

    uchar* data = (uchar*)image.data;

    //��ʼ��  
    for (i = 0; i < 256; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���  
    for (i = y; i < height; i++)
    {
        for (j = x; j<width; j++)
        {
            pixelCount[data[i * image.step + j]]++;
        }
    }


    //����ÿ������������ͼ���еı���  
    for (i = 0; i < 256; i++)
    {
        pixelPro[i] = (float)(pixelCount[i]) / (float)(pixelSum);
    }

    //����ostu�㷨,�õ�ǰ���ͱ����ķָ�  
    //�����Ҷȼ�[0,255],������������ĻҶ�ֵ,Ϊ�����ֵ  
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    for (i = 0; i < 256; i++)
    {
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;

        for (j = 0; j < 256; j++)
        {
            if (j <= i) //��������  
            {
                //��iΪ��ֵ���࣬��һ���ܵĸ���  
                w0 += pixelPro[j];
                u0tmp += j * pixelPro[j];
            }
            else       //ǰ������  
            {
                //��iΪ��ֵ���࣬�ڶ����ܵĸ���  
                w1 += pixelPro[j];
                u1tmp += j * pixelPro[j];
            }
        }

        u0 = u0tmp / w0;        //��һ���ƽ���Ҷ�  
        u1 = u1tmp / w1;        //�ڶ����ƽ���Ҷ�  
        u = u0tmp + u1tmp;      //����ͼ���ƽ���Ҷ�  
                                //������䷽��  
        deltaTmp = w0 * (u0 - u)*(u0 - u) + w1 * (u1 - u)*(u1 - u);
        //�ҳ������䷽���Լ���Ӧ����ֵ  
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = i;
        }
    }
    //���������ֵ;  
    return threshold;
}




void CCraneTest1Dlg::OnBnClickedButton1()
{

	m_theadConnect.StartThread();
	//m_theadOpenCVProcess.StartThread();
	SetTimer(1,60,NULL);
	//OnBnClickedBtnSendSoftCommand();

	//OnBnClickedBtnSendSoftCommand();
	//SetTimer(2,100,NULL);
}
