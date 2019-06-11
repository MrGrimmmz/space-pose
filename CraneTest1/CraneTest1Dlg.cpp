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
//#define new new(_CLIENT_BLOCK,__FILE__,__LINE__)       //注意，这里是__  不是_
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
	// 初始化参数
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
 //   title.Format("南京科远行车测角仪");//在标题栏动态显示Id的值
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

	// 若初始化设备库失败则直接退出应用程序
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}

	// 获取图像显示相关的UI资源

	
	m_pWnd       = GetDlgItem(IDC_STATIC_SHOW_WND);
	m_hDC        = ::GetDC(m_pWnd->m_hWnd);
	//

// 更新界面控件
	UpDateUI();


	hCom=CreateFile("COM4",GENERIC_READ|GENERIC_WRITE,NULL,0,OPEN_EXISTING,0,NULL);
	if(hCom==(HANDLE)-1)
	{AfxMessageBox("打开COM失败");
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
	memset(lpOutBuffer,'\0',12); //前12个字节先清零 
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

	lpOutBuffer[9]=bcc; //校验位
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
		//AfxMessageBox("写串口失败!"); 
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
\brief  点击"打开设备"按钮消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::OnBnClickedBtnOpenDevice() 
{
	// TODO: Add your control notification handler code here


	GX_STATUS     emStatus = GX_STATUS_SUCCESS;
	uint32_t      nDevNum  = 0;
	bool      bRet     = true;
    GX_OPEN_PARAM stOpenParam;
    
	// 初始化打开设备使用参数,默认打开序号为1的设备
    stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";
	
	// 枚举设备
	emStatus = GXUpdateDeviceList(&nDevNum, 1000);
	GX_VERIFY(emStatus);

	 ::GetPrivateProfileString(_T("biaoding"),_T("X1"),"",m_xbiaoding.GetBuffer(MAX_PATH),MAX_PATH,_T("C:\\Users\\lenovo\\Documents\\Visual Studio 2010\\Projects\\CraneTest1\\CraneTest1\\CraneTest1.ini"));//读取x偏移
	 ::GetPrivateProfileString(_T("biaoding"),_T("Y1"),"",m_ybiaoding.GetBuffer(MAX_PATH),MAX_PATH,_T("C:\\Users\\lenovo\\Documents\\Visual Studio 2010\\Projects\\CraneTest1\\CraneTest1\\CraneTest1.ini"));//读取x偏移
	m_xpian=atof(m_xbiaoding);
	m_ypian=atof(m_ybiaoding);
	//PostMessage( WM_DISPLAY_CHANGE, 0, 0);
	
	SinglePictureProcess() ;
		if (nDevNum <= 0)
	{
	 
		CString strMsg1;
		strMsg1.Format("未检测到可连接设备!");
		OnShowEnterMessage(strMsg1);
	
		return;
	}
	
	
	else if(nDevNum > 0)
	{
		GX_DEVICE_IP_INFO stIPInfo;
		//获取第一台设备的网络信息
		emStatus = GXGetDeviceIPInfo(1, &stIPInfo);

		CString strMsg2;
		strMsg2.Format("检测到%d台可连接设备",nDevNum);
		OnShowEnterMessage(strMsg2);
	}

	///////////////////////////////测试

	// 如果设备被打开则先关闭, 保证在初始化错误时能重新打开
	if(m_hDevice != NULL)
	{
		GXCloseDevice(m_hDevice);
		GX_VERIFY(emStatus);
		m_hDevice = NULL;
	}

	// 打开设备
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	GX_VERIFY(emStatus);


	m_bDevOpened = TRUE;

	//PrepareForShowImg();
	// 设置相机的默认参数:采集模式:连续采集,数据格式:8-bit
	emStatus = InitDevice();
	if(emStatus == GX_STATUS_SUCCESS)
	{
		CString strMsg3;
			strMsg3.Format("初始化相机成功！");
			OnShowEnterMessage(strMsg3);
	}
	else if (emStatus ==GX_STATUS_OFFLINE )
	{
		CString strMsg4;
			strMsg4.Format("相机掉线！");
			OnShowEnterMessage(strMsg4);

	}

	else 
	{
		CString strMsg5;
			strMsg5.Format("初始化相机失败！");
			OnShowEnterMessage(strMsg5);
	}


	GX_VERIFY(emStatus);

	// 获取设备的宽、高等属性信息
	emStatus = GetDeviceParam();
	GX_VERIFY(emStatus);

		//为图像显示准备资源
	bRet = PrepareForShowImg();
	if (!bRet)
	{
		CString strMsg6;
		strMsg6.Format("为显示图像准备资源失败!");
		OnShowEnterMessage(strMsg6);
		return;
	}



	// 获取相机参数,初始化界面控件
	InitUI();
	// 更新界面控件
	UpDateUI();

}


void CCraneTest1Dlg::OnShowEnterMessage(CString strMsg)
{
	g_csMsg.Lock();
	try
	{
		//设置显示内容
		CTime t = CTime::GetCurrentTime();
		g_strShowMsg += _T("\r\n") + t.Format("[%Y-%m-%d %H:%M:%S]") +strMsg;
		g_bIsNewMsg = TRUE;

		//文本框自动滚动显示新内容
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
\brief  弹出错误信息
\param  emErrorStatus  [in]错误码

\return 无返回值
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize        = 0;
	GX_STATUS emStatus     = GX_STATUS_ERROR;
	
	// 获取错误信息长度，并申请内存空间
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new char[nSize];
	if (NULL == pchErrorInfo)
	{
		return;
	}

	// 获取错误信息，并显示
	emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		MessageBox("GXGetLastError接口调用失败！");
	}
	else
	{
		MessageBox((LPCTSTR)pchErrorInfo);
	}

	// 释放申请的内存空间
	if (NULL != pchErrorInfo)
	{
		delete[] pchErrorInfo;
		pchErrorInfo = NULL;
	}
}



//----------------------------------------------------------------------------------
/**
\brief  关闭应用程序

\return 无
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::OnClose() 
{
	//if ( IDOK != AfxMessageBox("确定要退出？", MB_OKCANCEL) )
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
		// 发送停止采集命令
		emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_STOP);

		// 注销回调

		m_bIsSnap = FALSE;

		// 释放为采集图像开辟的图像Buffer
		UnPrepareForShowImg();
	}

	// 未关闭设备则关闭设备
	if (m_bDevOpened)
	{
		emStatus = GXCloseDevice(m_hDevice);
		m_bDevOpened = FALSE;
		m_hDevice    = NULL;
	}

	// 关闭设备库
	emStatus = GXCloseLib();

	
	// 释放图像显示相关DC
	::ReleaseDC(m_pWnd->m_hWnd,m_hDC);	

	_CrtDumpMemoryLeaks();  
	CDialog::OnClose();
}

// ---------------------------------------------------------------------------------
/**
\brief   初始化相机:设置默认参数

\return  GX_STATUS_SUCCESS:初始化设备成功，其它:初始化设备失败
*/
// ----------------------------------------------------------------------------------
GX_STATUS CCraneTest1Dlg::InitDevice()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//设置采集模式连续采集
	emStatus = GXSetEnum(m_hDevice,GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
	VERIFY_STATUS_RET(emStatus);
	//设置触发模式为开
	emStatus = GXSetEnum(m_hDevice,GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_ON);
	VERIFY_STATUS_RET(emStatus);
	//设置帧率
	emStatus = GXSetEnum(m_hDevice, GX_ENUM_ACQUISITION_FRAME_RATE_MODE, GX_ACQUISITION_FRAME_RATE_MODE_ON);
	emStatus = GXSetFloat(m_hDevice, GX_FLOAT_ACQUISITION_FRAME_RATE, 20.0);
	// 已知当前相机支持哪个8位图像数据格式可以直接设置
	// 例如设备支持数据格式GX_PIXEL_FORMAT_BAYER_GR8,则可按照以下代码实现
	// emStatus = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, GX_PIXEL_FORMAT_BAYER_GR8);
	// VERIFY_STATUS_RET(emStatus);

	// 不清楚当前相机的数据格式时，可以调用以下函数将图像数据格式设置为8Bit
	emStatus = SetPixelFormat8bit(); 
	VERIFY_STATUS_RET(emStatus);

	//选择触发源为软触发
	emStatus = GXSetEnum(m_hDevice,GX_ENUM_TRIGGER_SOURCE, GX_TRIGGER_SOURCE_SOFTWARE);
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  获取设备的宽高等属性信息

\return GX_STATUS_SUCCESS:全部获取成功，其它状态码:未成功获取全部
*/
//----------------------------------------------------------------------------------
GX_STATUS CCraneTest1Dlg::GetDeviceParam()
{
	GX_STATUS emStatus       = GX_STATUS_ERROR;
	bool      bIsImplemented = false;

	// 获取图像大小
	emStatus = GXGetInt(m_hDevice, GX_INT_PAYLOAD_SIZE, &m_nPayLoadSize);
	VERIFY_STATUS_RET(emStatus);

	// 获取宽度
	emStatus = GXGetInt(m_hDevice, GX_INT_WIDTH, &m_nImageWidth);
	VERIFY_STATUS_RET(emStatus);

	// 获取高度
	emStatus = GXGetInt(m_hDevice, GX_INT_HEIGHT, &m_nImageHeight);
	VERIFY_STATUS_RET(emStatus);

	// 判断相机是否支持bayer格式
	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &m_bIsColorFilter);
	VERIFY_STATUS_RET(emStatus);
	m_bIsColorFilter = bIsImplemented;
	// 支持表示输出彩色图像
	if(bIsImplemented)
	{
		emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &m_nPixelColorFilter);
		VERIFY_STATUS_RET(emStatus);
	}
    
	return emStatus;
}

// ---------------------------------------------------------------------------------
/**
\brief   设置相机的数据格式为8bit

\return  emStatus GX_STATUS_SUCCESS:设置成功，其它:设置失败
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

	// 获取像素点大小
	emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_SIZE, &nPixelSize);
	VERIFY_STATUS_RET(emStatus);

	// 判断为8bit时直接返回,否则设置为8bit
	if (nPixelSize == GX_PIXEL_SIZE_BPP8)
	{
		return GX_STATUS_SUCCESS;
	}
	else
	{
		// 获取设备支持的像素格式枚举数
		emStatus = GXGetEnumEntryNums(m_hDevice, GX_ENUM_PIXEL_FORMAT, &nEnmuEntry);
		VERIFY_STATUS_RET(emStatus);

		// 为获取设备支持的像素格式枚举值准备资源
		nBufferSize      = nEnmuEntry * sizeof(GX_ENUM_DESCRIPTION);
		pEnumDescription = new GX_ENUM_DESCRIPTION[nEnmuEntry];

		// 获取支持的枚举值
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

		// 遍历设备支持的像素格式,设置像素格式为8bit,
		// 如设备支持的像素格式为Mono10和Mono8则设置其为Mono8
		for (uint32_t i = 0; i < nEnmuEntry; i++)
		{
			if ((pEnumDescription[i].nValue & GX_PIXEL_8BIT) == GX_PIXEL_8BIT)
			{
				emStatus = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, pEnumDescription[i].nValue);
				break;
			}
		}	

		// 释放资源
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
\brief 初始化触发模式Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitTriggerModeUI()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//    
//	// 判断设备是否支持触发模式
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_TRIGGER_MODE, &m_bTriggerMode);
//	GX_VERIFY(emStatus);
//	if (!m_bTriggerMode)
//	{
//		return;
//	}
//
//	// 初始化触发模式Combox框
//	InitEnumUI(GX_ENUM_TRIGGER_MODE, (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_MODE), m_bTriggerMode);
//}

//----------------------------------------------------------------------------------
/**
\brief  初始化触发源Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitTriggerSourceUI()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//
//	// 判断设备是否支持触发源
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_TRIGGER_SOURCE, &m_bTriggerSource);
//	GX_VERIFY(emStatus);
//	if (!m_bTriggerSource)
//	{
//		return;
//	}
//
//	// 初始化触发源Combox框
//    InitEnumUI(GX_ENUM_TRIGGER_SOURCE, (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_SOURCE),m_bTriggerSource);
//}

//----------------------------------------------------------------------------------
/**
\brief 初始化触发极性Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitTriggerActivationUI()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//
//	// 判断设备是否支持触发极性
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_TRIGGER_ACTIVATION, &m_bTriggerActive);
//	GX_VERIFY(emStatus);
//	if (!m_bTriggerActive)
//	{
//		return;
//	}
//
//	// 初始化触发极性Combox框
//    InitEnumUI(GX_ENUM_TRIGGER_ACTIVATION, (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVATION), m_bTriggerActive);
//}

//----------------------------------------------------------------------------------
/**
\brief 初始化闪光灯开关Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitStrobeSwitchUI()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//
//	// 判断设备是否支持闪光灯开关
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_STROBE_SWITCH, &m_bStrobeSwitch);
//	GX_VERIFY(emStatus);
//	if (!m_bStrobeSwitch)
//	{
//		return;
//	}
//
//	// 初始化闪光灯开关Combox框
//	InitEnumUI(GX_ENUM_STROBE_SWITCH, (CComboBox *)GetDlgItem(IDC_COMBO_STROBE_SWITCH),m_bStrobeSwitch);
//}

//----------------------------------------------------------------------------------
/**
\brief 初始化用户IO输出模式Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitOutputModeUI()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//
//	// 判断设备是否支持用户IO输出模式
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_USER_OUTPUT_MODE, &m_bOutputMode);
//	GX_VERIFY(emStatus);
//	if (!m_bOutputMode)
//	{
//		return;
//	}
//
//	// 初始化用户IO输出模式Combox框
//    InitEnumUI(GX_ENUM_USER_OUTPUT_MODE, (CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_MODE), m_bOutputMode);
//}

//----------------------------------------------------------------------------------
/**
\brief  初始化用户自定义输出选择Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitOutputSelectorUI()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//
//	// 判断设备是否支持用户IO输出选择
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_USER_OUTPUT_SELECTOR, &m_bOutputSelector);
//	GX_VERIFY(emStatus);
//	if (!m_bOutputSelector)
//	{
//		return;
//	}
//
//	// 初始化用户IO输出选择Combox框
//	InitEnumUI(GX_ENUM_USER_OUTPUT_SELECTOR,(CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_SELECTOR),m_bOutputSelector);
//}

//----------------------------------------------------------------------------------
/**
\brief  初始化"引脚选择"Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitLineSelector()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//
//	// 判断设备是否支持引脚选择
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_LINE_SELECTOR, &m_bLineSelector);
//	GX_VERIFY(emStatus);
//	if (!m_bLineSelector)
//	{
//		return;
//	}
//
//	// 初始化引脚选择
//	InitEnumUI(GX_ENUM_LINE_SELECTOR, (CComboBox *)GetDlgItem(IDC_COMBO_LINE_SELECTOR), m_bLineSelector);
//}

//----------------------------------------------------------------------------------
/**
\brief  初始化"引脚方向"Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitLineMode()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//
//	// 判断设备是否支持引脚方向
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_LINE_MODE, &m_bLineMode);
//	GX_VERIFY(emStatus);
//	if (!m_bLineMode)
//	{
//		return;
//	}
//
//	// 初始化引脚方向
//	InitEnumUI(GX_ENUM_LINE_MODE, (CComboBox *)GetDlgItem(IDC_COMBO_LINE_MODE), m_bLineMode);
//
//}

//----------------------------------------------------------------------------------
/**
\brief  初始化"引脚电平反转"Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitLineInverter()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//	bool      bTemp    = true;
//
//	CComboBox *pLineInverter = (CComboBox *)GetDlgItem(IDC_COMBO_LINE_INVERTER);
//
//	// 判断设备是否支持引脚电平反转
//	emStatus = GXIsImplemented(m_hDevice, GX_BOOL_LINE_INVERTER, &m_bLineInverter);
//	GX_VERIFY(emStatus);
//	if (!m_bLineInverter)
//	{
//		return;
//	}
//
//	// 初始化引脚电平反转
//	pLineInverter->ResetContent();
//	pLineInverter->SetItemData(pLineInverter->AddString("false"), LINE_INVERTER_FALSE);
//	pLineInverter->SetItemData(pLineInverter->AddString("true"), LINE_INVERTER_TRUE);
//
//	// 获取当前设备的引脚电平反转的值，并选择更新到界面上
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
\brief  初始化"引脚输出源"Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitLineSource()
//{
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//
//	// 判断设备是否支持引脚输出源
//	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_LINE_SOURCE, &m_bLineSource);
//	GX_VERIFY(emStatus);
//	if (!m_bLineSource)
//	{
//		return;
//	}
//
//	// 初始化引脚输出源
//	InitEnumUI(GX_ENUM_LINE_SOURCE, (CComboBox *)GetDlgItem(IDC_COMBO_LINE_SOURCE), m_bLineSource);
//}

//---------------------------------------------------------------------------------
/**
\brief   初始化UI界面

\return  无
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::InitUI()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	int  nValue = 0;
    

	InitShutterUI();

	
	InitGainUI();
	// 初始化增益相关控件
	// 初始化引脚选择ComBox控件
	//InitLineSelector();

	// 初始化引脚模式ComBox控件
	//InitLineMode();

	// 初始化引脚电平反转ComBox控件
	//InitLineInverter();

	// 初始化引脚输出源ComBox控件
	//InitLineSource();
		// 初始化用户自定义输出值选项
    //InitOutputValue();

	// 触发模式下拉选项Combox控件
   // InitTriggerModeUI();
	
	// 初始化触发源Combox控件
	//InitTriggerSourceUI();

	// 初始化触发极性Combox控件
	//InitTriggerActivationUI();

	// 初始化闪光灯开关Combox控件
	//InitStrobeSwitchUI();

	// 初始化用户IO输出模式Combox控件
	//InitOutputModeUI();

	// 初始化用户自定义输出选择ComBox控件
	//InitOutputSelectorUI();
	
	// 初始化曝光时间相关控件
	UpdateData(false);
}

//----------------------------------------------------------------------------------
/**
\brief  初始化"用户自定义输出值"Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::InitOutputValue()
//{
//	CComboBox *pBox           = (CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_VALUE);
//	GX_STATUS  emStatus       = GX_STATUS_SUCCESS;
//	bool       bVal           = false;
//
//	// 初始化触发极性下拉列表
//	emStatus = GXIsImplemented(m_hDevice,GX_BOOL_USER_OUTPUT_VALUE, &m_bOutputValue);
//	GX_VERIFY(emStatus);
//	if (!m_bOutputValue)
//	{
//		return;
//	}
//
//	// 初始化当前控件的可选项
//	pBox->ResetContent();
//	pBox->SetItemData(pBox->AddString("false"), OUTPUT_ACTIVATION_FALSE);
//	pBox->SetItemData(pBox->AddString("true"), OUTPUT_ACTIVATION_TRUE);
//
//	// 获取枚举型的当前值,并设为界面当前显示值
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
\brief   初始化组合框UI界面
\param   emFeatureID    [in]    功能码ID
\param   pComboBox      [in]    控件指针
\param   bIsImplemented [in]    标识是否支持emFeatureID代表的功能

\return  无
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::InitEnumUI(GX_FEATURE_ID_CMD emFeatureID, CComboBox *pComboBox, bool bIsImplemented)
{
	// 判断控件
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

	// 获取功能的枚举数
	emStatus = GXGetEnumEntryNums(m_hDevice, emFeatureID, &nEntryNum);
	GX_VERIFY(emStatus);

	// 为获取枚举型的功能描述分配空间
	nbufferSize = nEntryNum * sizeof(GX_ENUM_DESCRIPTION);
	pEnum       = new GX_ENUM_DESCRIPTION[nEntryNum];
	if (pEnum == NULL)
	{
		MessageBox("分配缓冲区失败！");
		return;
	}

	// 获取功能的枚举描述
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

	// 获取枚举型的当前值,并设为界面当前显示值
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

	// 初始化当前控件的可选项
	pComboBox->ResetContent();
	for (uint32_t i = 0; i < nEntryNum; i++)
	{	
		pComboBox->SetItemData(pComboBox->AddString(pEnum[i].szSymbolic), (uint32_t)pEnum[i].nValue);
		if (pEnum[i].nValue == nEnumValue)
		{
			nCurcel = i;
		}
	}

	// 设置当前值为界面的显示值
	pComboBox->SetCurSel(nCurcel);

	// 释放函数资源
	if (pEnum != NULL)
	{  
		delete []pEnum;
		pEnum = NULL;
	}
}

////---------------------------------------------------------------------------------
///**
//\brief   切换"触发源"选项响应函数
//
//\return  无
//*/
////----------------------------------------------------------------------------------
//void CCraneTest1Dlg::OnCbnSelchangeComboTriggerSource()
//{
//	// TODO: Add your control notification handler code here
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_SOURCE);
//
//	int      nIndex    = pBox->GetCurSel();                   // 获取当前选项
//	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // 获取当前选项对应的枚举型值
//
//	// 将当前选择的触发源的枚举值设置到相机中
//	emStatus = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_SOURCE, nEnumVal);
//	GX_VERIFY(emStatus);
//}

////----------------------------------------------------------------------------------
///**
//\brief  切换"触发模式"Combox框消息响应函数
//
//\return 无
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
//	int      nIndex    = pBox->GetCurSel();                     // 获取当前选项
//	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);    // 获取当前选项对应的枚举型值
//
//	// 判断触发模式是否可写
//	emStatus = GXIsWritable(m_hDevice,GX_ENUM_TRIGGER_MODE,&bIsWritable);
//	GX_VERIFY(emStatus);
//
//	// 获取触发模式的当前值
//	emStatus = GXGetEnum(m_hDevice, GX_ENUM_TRIGGER_MODE, &nCurrentEnumValue);
//	GX_VERIFY(emStatus);
//
//	if (bIsWritable)
//	{
//		// 将当前选择的触发模式的值设置到相机中
//		emStatus = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_MODE, nEnumVal);
//		GX_VERIFY(emStatus);
//
//		// 更新当前触发模式
//		m_nTriggerMode = nEnumVal;
//	}
//	else
//	{
//		MessageBox("当前状态不可写，请停止采集后，请重新设置!");
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
//\brief  点击"软触发采图"按钮消息响应函数
//
//\return 无
//*/
////----------------------------------------------------------------------------------
//void CCraneTest1Dlg::OnBnClickedBtnSendSoftTrigger()
//{
//	// TODO: Add your control notification handler code here
//	//发送软触发命令(在触发模式为On且开始采集后有效)
//	GX_STATUS emStatus = GX_STATUS_ERROR;
//	emStatus = GXSendCommand(m_hDevice, GX_COMMAND_TRIGGER_SOFTWARE);
//
//	GX_VERIFY(emStatus);
//}

//----------------------------------------------------------------------------------
/**
\brief  切换"触发极性"Combox框消息响应函数

\return 无
*/
////----------------------------------------------------------------------------------
//void CCraneTest1Dlg::OnCbnSelchangeComboTriggerActivation()
//{
//	// TODO: Add your control notification handler code here
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVATION);
//
//	int      nIndex    = pBox->GetCurSel();                   // 获取当前选项
//	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // 获取当前选项对应的枚举型值
//
//	// 将当前选择的触发极性的值设置到相机中
//	emStatus = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_ACTIVATION, nEnumVal);
//	GX_VERIFY(emStatus);
//}

//---------------------------------------------------------------------------------
/**
\brief   初始化曝光相关控件

\return  无
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::InitShutterUI()
{
	CStatic     *pStcShutterShow = (CStatic *)GetDlgItem(IDC_STATIC_SHUTTER);
    CEdit       *pEditShutterVal = (CEdit *)GetDlgItem(IDC_EDIT_SHUTTER_VALUE);

	// 判断控件的有效性
	if ((pEditShutterVal == NULL) || (pStcShutterShow == NULL))
	{
		return;
	}

    GX_STATUS      emStatus = GX_STATUS_ERROR;
	CString        strTemp  = "";
	double      dShutterVal = 0.0;

	// 获取浮点型范围,并初始化Edit控件和static范围提示框
	emStatus = GXGetFloatRange(m_hDevice, GX_FLOAT_EXPOSURE_TIME, &m_stShutterFloatRange);
	GX_VERIFY(emStatus);

    strTemp.Format("曝光(%.4f~%.4f)%s",m_stShutterFloatRange.dMin, m_stShutterFloatRange.dMax, m_stShutterFloatRange.szUnit);
	pStcShutterShow->SetWindowText(strTemp);

	// 获取当前值并将当前值更新到界面
	emStatus = GXGetFloat(m_hDevice, GX_FLOAT_EXPOSURE_TIME, &dShutterVal);
	GX_VERIFY(emStatus);

	m_dShutterValue = dShutterVal;

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化增益相关控件
 
\return  无
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

	// 获取浮点型范围,并初始化Edit控件和static范围提示框
	emStatus = GXGetFloatRange(m_hDevice, GX_FLOAT_GAIN, &m_stGainFloatRange);
	GX_VERIFY(emStatus);

	// 显示范围
    strRange.Format("增益(%.4f~%.4f)%s", m_stGainFloatRange.dMin, m_stGainFloatRange.dMax, m_stGainFloatRange.szUnit);
	pStcGain->SetWindowText(strRange);
	
	// 获取当前值并更新到界面
	emStatus = GXGetFloat(m_hDevice, GX_FLOAT_GAIN, &dGainVal);
	GX_VERIFY(emStatus);

	m_dGainValue = dGainVal;
   
	UpdateData(FALSE);
}

//----------------------------------------------------------------------------------
/**
\brief  点击"开始采集"按钮消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::OnBnClickedBtnStartSnap()
{
	///GX_STATUS_ERROR
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;


	emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_START);
	//////////////////////////////////////////一开始有的
	if (emStatus != GX_STATUS_SUCCESS)
	{
		UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}
	m_bIsSnap = TRUE;
	CString strMsg8;
	strMsg8.Format("开始实时采集图像");
	OnShowEnterMessage(strMsg8);

	

	// 更新界面UI
    UpDateUI();


	//OnBnClickedButton1();

	
}

//----------------------------------------------------------------------------------
/**
\brief  点击"软触发采图"按钮消息响应函数

\return 无
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

	//每次发送触发命令之前清空采集输出队列
	//防止库内部缓存帧，造成本次GXGetImage得到的图像是上次发送触发得到的图
	/*emStatus = GXFlushQueue(m_hDevice);
	GX_VERIFY(emStatus);*/
	emStatus = GXSendCommand(m_hDevice,GX_COMMAND_TRIGGER_SOFTWARE);
	GX_VERIFY(emStatus);

	//////////////////////////////////
	//获取图像
	CString str1;
	str1.Format(_T("开始测试"));
	xPublic::WriteFileDebug(_T("12"),str1);
	emStatus = GXGetImage(m_hDevice, &m_stFrameData, m_nEditTimeOut);



	str1.Format(_T("测试结束"));
	xPublic::WriteFileDebug(_T("12"),str1);

	//判断GXGetImage返回值
	GX_VERIFY(emStatus);
		

	////////////////////////////////////////////////////处理图像、
	 ShowImage(&m_stFrameData);
	
}

//----------------------------------------------------------------------------------
/**
\brief  为保存图像分配Buffer,为图像显示准备资源
\return true:为图像显示准备资源成功  false:准备资源失败
*/
//----------------------------------------------------------------------------------
bool  CCraneTest1Dlg::PrepareForShowImg()
{	
	bool bRet = true;
	bool bRet1 = true;
		//为GetImage参数分配空间
     m_stFrameData.pImgBuf     = new BYTE[(size_t)m_nPayLoadSize];
	if (m_stFrameData.pImgBuf == NULL)
	{
		bRet1 = false;
	}
	
	if (m_bIsColorFilter)   
	{
		// 为彩色图像显示准备资源,分配Buffer
		bRet = PrepareForShowColorImg();
	}
	else  
	{
		// 为黑白图像显示准备资源,分配Buffer
		bRet = PrepareForShowMonoImg();
	}

	// 若分配失败则释放已分配的资源
	if (bRet==false||bRet1==false)
	{
		UnPrepareForShowImg();
	}

	return bRet;
}

//----------------------------------------------------------------------------------
/**
\brief  为彩色图像显示准备资源,分配Buffer

\return true:为图像显示准备资源成功  false:准备资源失败
*/
//----------------------------------------------------------------------------------
bool  CCraneTest1Dlg::PrepareForShowColorImg()
{	
	//--------------------------------------------------------------------
	//---------------------------初始化bitmap头---------------------------
	m_pBmpInfo								= (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth			= (LONG)m_nImageWidth;
	m_pBmpInfo->bmiHeader.biHeight			= (LONG)m_nImageHeight;	

	m_pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pBmpInfo->bmiHeader.biBitCount		= 24; // 彩色图像为24,黑白图像为8
	m_pBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pBmpInfo->bmiHeader.biClrImportant	= 0;


	//---------------------------------------------------------------------
	//----------------------------为图像数据分配Buffer---------------------

	// 为经过RGB转换后的图像分配Buffer
	m_pImgBuffer = new BYTE[(size_t)(m_nImageWidth * m_nImageHeight * 3)];
	if (m_pImgBuffer == NULL)
	{
		return false;
	}




	return true;
}

//----------------------------------------------------------------------------------
/**
\brief 为黑白图像显示准备资源,分配Buffer

\return true:为图像显示准备资源成功  false:准备资源失败
*/
//----------------------------------------------------------------------------------
bool  CCraneTest1Dlg::PrepareForShowMonoImg()
{	
	//--------------------------------------------------------------------
	//---------------------------初始化bitmap头---------------------------
	m_pBmpInfo								= (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth			= (LONG)m_nImageWidth;
	m_pBmpInfo->bmiHeader.biHeight			= (LONG)m_nImageHeight;	

	m_pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pBmpInfo->bmiHeader.biBitCount		= 8;  // 彩色图像为24,黑白图像为8
	m_pBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pBmpInfo->bmiHeader.biClrImportant	= 0;

	if (!m_bIsColorFilter)
	{
		// 黑白相机需要进行初始化调色板操作
		for(int i=0;i<256;i++)
		{
			m_pBmpInfo->bmiColors[i].rgbBlue	 = i;
			m_pBmpInfo->bmiColors[i].rgbGreen	 = i;
			m_pBmpInfo->bmiColors[i].rgbRed		 = i;
			m_pBmpInfo->bmiColors[i].rgbReserved = i;
		}
	}
	//---------------------------------------------------------------------
	//----------------------------为图像数据分配Buffer---------------------
	// 黑白图像Buffer分配
	m_pImgBuffer = new BYTE[(size_t)(m_nImageWidth * m_nImageHeight)];
	
	if (m_pImgBuffer == NULL)
	{
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------
/**
\brief  初始化相机内参矩阵
\param  

\return 无
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
\brief  将获取到的图像数据依据黑白彩色的不同进行处理并显示到界面
\param  pFrameData  [in] 指向用户传入的图像数据的地址指针

\return 无
*/
//----------------------------------------------------------------------------------


cv::Mat fast_ergodic4(cv::Mat image) {	
	if (!image.isContinuous())	{		
		return image;	
	}	
	int rows = image.rows;	
	int cols = image.cols;	/*使用常量会比变量快，因此当图像通道确定时，请将chans改为数字常量，如1，3，4等*/	//
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
	
	// 获取到的图像是否为完整的图像
	if (pFrameData->nStatus != 0)    
	{
		return;
	}

	//若支持彩色,转换为RGB图像后输出
	if (m_bIsColorFilter)
	{
		//将Raw8图像转换为RGB图像以供显示
		DxRaw8toRGB24((BYTE*)pFrameData->pImgBuf, m_pImgBuffer, (VxUint32)m_nImageWidth, (VxUint32)m_nImageHeight, RAW2RGB_NEIGHBOUR,
			DX_PIXEL_COLOR_FILTER(m_nPixelColorFilter), TRUE);
	}
	else   
	{
		// 黑白相机需要翻转数据后显示
		for(int i =0;i <m_nImageHeight;i++)
		{
			memcpy(m_pImgBuffer+i*m_nImageWidth, (BYTE *)(pFrameData->pImgBuf)+(m_nImageHeight-i-1)*m_nImageWidth,(size_t)m_nImageWidth);
		}
	}

	
	//在指定窗口上一帧一帧的画图
	DrawImg(m_pImgBuffer, m_nImageWidth, m_nImageHeight);

//
	//////////////////////////////////////////////////////////////相机矫正部分////////////////////////////////////////////////
	//float neican_data[] = { 6229.480256699168, 0, 753.3600831270645, 0, 6208.898904029267, 353.5292143532171, 0, 0, 1 };

	//float jibian_data[] = { -0.725636018154933, 71.59698241520449, -0.01102894789291404, -0.009635933311296271, -2360.33301002783 };
	///////////xuyao///////////////////////////////////////////////////图像处理部分////////////////////////////////////////////////
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
	//cvtColor( src_gray, src_gray, CV_BGR2GRAY );//灰度化

	Mat dst;

	//////////////////////buxuyao/////////////////矫正开始////////////////////////////////////////////////////
	//Mat R = Mat::eye(3, 3, CV_32F);
	//Size image_size;  /* 图像的尺寸 */
 //   //获取图像大小
 //   image_size.width = m_nImageWidth;
 //   image_size.height = m_nImageHeight;
	//Mat mapx = Mat(image_size, CV_32FC1);
 //   Mat mapy = Mat(image_size, CV_32FC1);
	//Mat cameraMatrix(3, 3, CV_32FC1);
 //   InitMat(cameraMatrix, neican_data);

	//Mat distCoeffs(1, 5, CV_32FC1); /* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */
 //   InitMat(distCoeffs, jibian_data);
	//initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix, image_size, CV_32FC1, mapx, mapy);

 //   Mat imageSource = src_gray; //读取畸变图片
 //   Mat newimage = imageSource.clone(); //校正后输出图片
 //   remap(imageSource, newimage, mapx, mapy, INTER_LINEAR);
	//int threshold_white = Otsu(src_gray );//阈值计算，利用otsu
	////////////////////////////////////////////矫正结束/////////////////////////////////////////////////////////////////
		

	GaussianBlur( src_gray, src_gray, Size(3,3), 3, 3, BORDER_DEFAULT );//////////////newimage
	Mat binary;
	threshold(src_gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);/////////newiamge
	Mat canny_output;

	//设置保存连通域的容器
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	/////////////////////////////////////////////////////////////////////////////////大约60ms
	///求取卷积核 
	Mat k = getStructuringElement(MORPH_RECT, Size(5,5));
	////对二值图像进行开运算
	morphologyEx(binary, binary, MORPH_OPEN, k);

	

	//CString strMsg1,strMsg2;
	//strMsg1.Format(("开始寻找连通域"));	
	findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

		//判断是否存在连通域
			if(0==contours.size()||contours.size()>3)
			{
				/*CString strMsg;
				strMsg.Format("未找到合适的连通域!");
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
				strMsg.Format(("有%d个连通域!"),contours.size());
				OnShowEnterMessage(strMsg);*/
			}

///不变矩
	vector<Moments> mu(contours.size() );
	for( int i = 0; i < contours.size(); i++ ) 		
	{ 		
		mu[i] = moments( contours[i], false ); 	
	} 		
	//计算轮廓的质心 	灰度重心法	
	vector<Point2f> mc( contours.size() ); 	
	Point pt[10000]; 
	for( int i = 0; i < contours.size(); i++ ) 		
	{ 	
		if (mu[i].m00 != 0)						   //除数不能为0
		{
			mc[i].x = cvRound(mu[i].m10 / mu[i].m00);//重心横坐标
			mc[i].y = cvRound(mu[i].m01 / mu[i].m00);//重心纵坐标
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



		
///////////////////////////////////////////////////////////////角度算法////////////////////////////////////////////
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
	/////////////自动校正//////
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
	
	//////////////共享资源上锁同步线程///////////////////
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
	////////////////////////////////////////////消息响应函数传到edit控件进行显示
	PostMessage( WM_DISPLAY_CHANGE, 0, 0);

	
	////////////////////////////////////////////////////////////////////////////////
	CString str2;
	str2.Format(_T("x轴角度：%s,y轴角度：%s,高度：%s,旋转角度：%s"),s,s1,s2,s3);
	xPublic::WriteFileDebug(_T("11"),str2);
	//显示灰度重心坐标
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
\brief   相机掉线回调函数
\param   pFrame      回调参数

\return  无
*/
//----------------------------------------------------------------------------------
void __stdcall CCraneTest1Dlg::OnDeviceOfflineCallbackFun(void *pUserParam)
{
	CCraneTest1Dlg *pDlg = (CCraneTest1Dlg*)(pUserParam);

	pDlg->OnBnClickedBtnStopSnap();
}





//---------------------------------------------------------------------------------
/**
\brief   采集图像回调函数
\param   pFrame      回调参数

\return  无
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
//			// 彩色图像需要将Raw8图像经过RGB转换后显示
//			DxRaw8toRGB24(pDlg->m_pRawBuffer, pDlg->m_pImgBuffer, nImageWidth , nImageHeight, RAW2RGB_NEIGHBOUR,
//				           DX_PIXEL_COLOR_FILTER(pDlg->m_nPixelColorFilter), TRUE);
//			//pDlg->DrawImg(pDlg->m_pImgBuffer);
//		}
//		else
//		{
//			// 黑白相机需要翻转数据后显示
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
\brief   显示图像
\param   pImageBuffer   [in]   要显示的图像数据

\return  无
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::DrawImg(void *pImageBuf, int64_t nImageWidth, int64_t nImageHeight)
{
	int nWndWidth  = 0;
	int nWndHeight = 0;

	// 为画图做准备
	RECT objRect;
	m_pWnd->GetClientRect(&objRect);
	nWndWidth  = objRect.right - objRect.left;
	nWndHeight = objRect.bottom - objRect.top;

	// 必须调用该语句，否则图像出现水纹
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
\brief  释放为图像显示准备的资源

\return 无
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
\brief  切换"闪光灯开关"Combox框消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
//void CCraneTest1Dlg::OnCbnSelchangeComboStrobeSwitch()
//{
//	// TODO: Add your control notification handler code here
//	GX_STATUS emStatus = GX_STATUS_SUCCESS;
//	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_STROBE_SWITCH);
//
//	int      nIndex    = pBox->GetCurSel();                   // 获取当前选项
//	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // 获取当前选项对应的枚举型值
//
//	// 将当前选择的闪光灯开关的值设置到相机中
//	emStatus = GXSetEnum(m_hDevice, GX_ENUM_STROBE_SWITCH, nEnumVal);
//	GX_VERIFY(emStatus);
//}

//----------------------------------------------------------------------------------
/**
\brief  点击"停止采集"按钮消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::OnBnClickedBtnStopSnap()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;


	//发送停止采集命令
	emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_STOP);
	GX_VERIFY(emStatus);
	m_bIsSnap = FALSE;

	
	CString strMsg9;
	strMsg9.Format("已暂停采集图像");
	OnShowEnterMessage(strMsg9);
	// 更新界面UI
	UpDateUI();

	m_theadConnect.SetClose();
	m_theadConnect.StopThread();
	KillTimer(1); 
		
}



//----------------------------------------------------------------------------------
/**
\brief  关闭设备

\return 关闭设备成功则返回GX_STATUS_SUCCESS，否则返回错误码
*/
//----------------------------------------------------------------------------------
GX_STATUS CCraneTest1Dlg::CloseDevice()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// 释放资源
    UnPrepareForShowImg();
	
	// 关闭设备
	emStatus = GXCloseDevice(m_hDevice);
		
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  点击"关闭设备"按钮消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CCraneTest1Dlg::OnBnClickedBtnCloseDevice()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	// 未停采时则停止采集
	if (m_bIsSnap)
	{
		//发送停止采集命令
		emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_STOP);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// 错误处理
		}
		m_bIsSnap = false;
	}
	
	//关闭相机
	UnPrepareForShowImg();
	
	// 关闭设备
	emStatus = GXCloseDevice(m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// 错误处理
	}

	m_bDevOpened = false;
	m_hDevice    = NULL;

	CString strMsg10;
	strMsg10.Format("设备已关闭");
	OnShowEnterMessage(strMsg10);

	// 更新界面UI
	UpDateUI();


}

//------------------------------------------------------
/**
\brief   响应曝光Edit控件失去焦点的函数

\return  无
*/
//------------------------------------------------------
void CCraneTest1Dlg::OnKillfocusEditShutterValue() 
{
	// TODO: Add your control notification handler code here
	//判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return;
	}

	UpdateData();
	GX_STATUS status = GX_STATUS_SUCCESS;

	//判断输入值是否在曝光时间的范围内
	//若大于最大值则将曝光值设为最大值
	if (m_dShutterValue > m_stShutterFloatRange.dMax)
	{
		m_dShutterValue = m_stShutterFloatRange.dMax;
	}
	//若小于最小值将曝光值设为最小值
	if (m_dShutterValue < m_stShutterFloatRange.dMin)
	{
		m_dShutterValue = m_stShutterFloatRange.dMin;
	}

	// 设置曝光的值
	status = GXSetFloat(m_hDevice,GX_FLOAT_EXPOSURE_TIME,m_dShutterValue);
	GX_VERIFY(status);

	UpdateData(FALSE);
}

//------------------------------------------------------
/**
\brief   响应增益Edit控件失去焦点的函数

  \return  无
*/
//------------------------------------------------------
void CCraneTest1Dlg::OnKillfocusEditGainValue() 
{
	// TODO: Add your control notification handler code here
	//判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return;
	}

	UpdateData();
	GX_STATUS status = GX_STATUS_SUCCESS;

	//判断输入值是否在增益值的范围内
	//若输入的值大于最大值则将增益值设置成最大值
	if (m_dGainValue > m_stGainFloatRange.dMax)
	{
		m_dGainValue = m_stGainFloatRange.dMax;
	}

	//若输入的值小于最小值则将增益的值设置成最小值
	if (m_dGainValue < m_stGainFloatRange.dMin)
	{
		m_dGainValue = m_stGainFloatRange.dMin;
	}

	// 设置增益的值
	status = GXSetFloat(m_hDevice,GX_FLOAT_GAIN,m_dGainValue);
	GX_VERIFY(status);

	UpdateData(FALSE);
}

//------------------------------------------------------
/**
\brief   分发函数(主要处理键盘的回车消息)
\param   pMsg  消息结构体

\return  成功：TRUE   失败：FALSE
*/
//------------------------------------------------------
BOOL CCraneTest1Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pWnd  = NULL;
	int nCtrlID = 0;               //保存获取的控件ID

	//判断是否是键盘回车消息
	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
	{
		//获取当前拥有输入焦点的窗口(控件)指针
		pWnd = GetFocus();

		//获取光标所处位置的控件ID
		nCtrlID = pWnd->GetDlgCtrlID();

		//判读ID类型
		switch (nCtrlID)
		{
		case IDC_EDIT_SHUTTER_VALUE:
		case IDC_EDIT_GAIN_VALUE:

			//失去焦点
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
	///////////多久采一次帧，考虑定时器////////////////////////////
	//100ms取一次。
	CCraneTest1Dlg*  pThis   = (CCraneTest1Dlg *)pParam;

	while ( WAIT_TIMEOUT == ::WaitForSingleObject(hCloseEvent,10) )
	{

		pThis->OnBnClickedBtnSendSoftCommand();
    }
}


LRESULT CCraneTest1Dlg::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{   
	g_csMsg1.Lock();
//更新Edit  
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
	//如果有先分配一个暂存空间
	//***********************************新的图像方案****************************************//
	Mat src = imread( "C:/Users/lenovo/Documents/Visual Studio 2010/Projects/CraneTest1/CraneTest1/10.bmp" );
	//IplImage* img1;

	cvtColor( src, src_gray, CV_BGR2GRAY );//灰度化
		int threshold_white = Otsu(src_gray );//阈值计算，利用otsu
	//高斯模糊滤波
	GaussianBlur( src_gray, src_gray, Size(3,3), 3, 3, BORDER_DEFAULT );
	//blur( src_gray, src_gray, Size(3,3) ); //滤波
	/////////进行转化为iplimage进行腐蚀和膨胀操作
	/*IplImage* pBinary = &IplImage(src_gray);*/
	//img1 = cvCreateImage(cvSize(pBinary->width,pBinary->height),8,1);
	//cvErode(img1, img1, 0, 1);
	//Mat ImgTemp;
 //  ImgTemp=cvarrToMat(img1);
	Mat binary;
	//阈值化
	threshold(src_gray, binary, threshold_white, 255, THRESH_BINARY | THRESH_OTSU);
	namedWindow( "threshold",CV_WINDOW_NORMAL ); 		
	imshow( "threshold", binary ); 		
	moveWindow("threshold",500,20);
	//Mat se(5,5,CV_8U,Scalar(1));
	Mat canny_output;
	//设置保存连通域的容器
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//canny提取轮廓
	Canny( binary, canny_output, 30, 100, 3 );
	///求取卷积核 
	Mat k = getStructuringElement(MORPH_RECT, Size(5,5));
	//dilate(canny_output,canny_output,k,Point(-1,-1),1);
	////对二值图像进行开运算
	morphologyEx(binary, binary, MORPH_OPEN, k);
	//morphologyEx(canny_output, canny_output, MORPH_BLACKHAT, k);
    // erode(binary,binary,k,Point(-1,-1),2);
	namedWindow( "Canny",CV_WINDOW_NORMAL ); 		
	imshow( "Canny", binary ); 		
	moveWindow("Canny",500,20);
	///寻找连通域的轮廓
	findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		//判断是否存在连通域
			if(0==contours.size())
			{
				CString strMsg11;
				strMsg11.Format("未找到合适的连通域!");
				OnShowEnterMessage(strMsg11);
				return;
			}
			else{
				CString strMsg;
				strMsg.Format(("有%d个连通域!"),contours.size());
				OnShowEnterMessage(strMsg);
			}
    ///不变矩
	vector<Moments> mu(contours.size() );
	for( int i = 0; i < contours.size(); i++ ) 		
	{ 		
		mu[i] = moments( contours[i], false ); 	
	} 		
	//计算轮廓的质心 	灰度重心法	
	vector<Point2f> mc( contours.size() ); 	
	Point pt[10000]; 
	for( int i = 0; i < contours.size(); i++ ) 		
	{ 	
		if (mu[i].m00 != 0)						   //除数不能为0
		{
			mc[i].x = cvRound(mu[i].m10 / mu[i].m00);//重心横坐标
			mc[i].y = cvRound(mu[i].m01 / mu[i].m00);//重心纵坐标
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


		///////////////////////////////////////////////////////////////角度算法////////////////////////////////////////////
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
	/////////////自动校正//////
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
	
	//////////////共享资源上锁同步线程///////////////////
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
	////////////////////////////////////////////消息响应函数传到edit控件进行显示
	PostMessage( WM_DISPLAY_CHANGE, 0, 0);

	



	//显示灰度重心坐标
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

    //初始化  
    for (i = 0; i < 256; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    //统计灰度级中每个像素在整幅图像中的个数  
    for (i = y; i < height; i++)
    {
        for (j = x; j<width; j++)
        {
            pixelCount[data[i * image.step + j]]++;
        }
    }


    //计算每个像素在整幅图像中的比例  
    for (i = 0; i < 256; i++)
    {
        pixelPro[i] = (float)(pixelCount[i]) / (float)(pixelSum);
    }

    //经典ostu算法,得到前景和背景的分割  
    //遍历灰度级[0,255],计算出方差最大的灰度值,为最佳阈值  
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    for (i = 0; i < 256; i++)
    {
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;

        for (j = 0; j < 256; j++)
        {
            if (j <= i) //背景部分  
            {
                //以i为阈值分类，第一类总的概率  
                w0 += pixelPro[j];
                u0tmp += j * pixelPro[j];
            }
            else       //前景部分  
            {
                //以i为阈值分类，第二类总的概率  
                w1 += pixelPro[j];
                u1tmp += j * pixelPro[j];
            }
        }

        u0 = u0tmp / w0;        //第一类的平均灰度  
        u1 = u1tmp / w1;        //第二类的平均灰度  
        u = u0tmp + u1tmp;      //整幅图像的平均灰度  
                                //计算类间方差  
        deltaTmp = w0 * (u0 - u)*(u0 - u) + w1 * (u1 - u)*(u1 - u);
        //找出最大类间方差以及对应的阈值  
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = i;
        }
    }
    //返回最佳阈值;  
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
