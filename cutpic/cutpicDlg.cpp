
// cutpicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cutpic.h"
#include "cutpicDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcutpicDlg dialog



CcutpicDlg::CcutpicDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcutpicDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcutpicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CcutpicDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_MESSAGE(WM_SNAP_CHANGE1, OnSnapChange1)
	//ON_MESSAGE(WM_SNAP_CHANGE2, OnSnapChange2)
	//ON_MESSAGE(WM_SNAP_ERROR, OnSnapError)
	ON_MESSAGE(WM_SNAP_STOP, OnSnapexStop)
	ON_BN_CLICKED(IDC_OPENCAM, &CcutpicDlg::OnBnClickedOpencam)
	ON_BN_CLICKED(IDC_SAVEPIC, &CcutpicDlg::OnBnClickedSavepic)
	ON_BN_CLICKED(IDC_Capture_images, &CcutpicDlg::OnBnClickedCaptureimages)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_StopCapture, &CcutpicDlg::OnBnClickedStopcapture)
END_MESSAGE_MAP()


// CcutpicDlg message handlers

BOOL CcutpicDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	num_save=0;
	num_capture=0;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CcutpicDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CcutpicDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CcutpicDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




/*
	函数:
		SnapThreadCallback
	输入参数:
		SNAP_INFO *pInfo		SNAP_INFO结构包括当前数字摄像机SNAP执行状态
	输出参数:
		int						
	说明:
		数字摄像机采集到内存回调函数，但用户一般不用调用，由用户提供给SDK使用，
		用户在回调函数内实现对采集数据的处理和显示即可
 */
int CALLBACK  CcutpicDlg::SnapThreadCallback1(HV_SNAP_INFO *pInfo)
{
	CcutpicDlg *This = (CcutpicDlg *)(pInfo->pParam);
	HWND hwnd = This->m_hWnd;

	SetEvent(This->m_hAcqEvent1);
	//::PostMessage(hwnd, WM_SNAP_CHANGE1, 0, 0);
	

	return 1;
}

int CALLBACK  CcutpicDlg::SnapThreadCallback2(HV_SNAP_INFO *pInfo)
{
	CcutpicDlg *This = (CcutpicDlg *)(pInfo->pParam);
	HWND hwnd = This->m_hWnd;

	SetEvent(This->m_hAcqEvent2);
	//::PostMessage(hwnd, WM_SNAP_CHANGE2, 0, 0);
	

	return 1;
}


///*
//	函数:
//		OnSnapError
//	输入参数:
//		WPARAM wParam			没有使用		
//		LPARAM lParam			异常状态码
//	输出参数:
//		LRESULT						
//	说明:
//		摄像机采集异常错误报告
//*/
//LRESULT  CcutpicDlg::OnSnapError(WPARAM wParam, LPARAM lParam)
//{	
//	//CErrorBox ErrDlg;
//	//ErrDlg.m_dwStatus = lParam;
//	//if (ErrDlg.DoModal()==IDOK)
//	//{
//	//	OnSnapexStop();
//	//}	
//	return 1;
//}
////
////*
////	函数:
////		OnSnapChange
////	输入参数:
////		WPARAM wParam			字参数，在消息中为当前可以处理的图像序号		
////		LPARAM lParam			没有使用
////	输出参数:
////		LRESULT						
////	说明:
////		实现对采集数据的处理和显示
//// */
////LRESULT  CcutpicDlg::OnSnapChange1(WPARAM wParam, LPARAM lParam)
////{
////	HVSTATUS status = STATUS_OK;
////	
////
////	CWnd *pWnd1 = GetDlgItem(IDC_IMAGE1);
////	CDC  *pDC1	= pWnd1->GetDC();	
////	CImage m_imge1 ;
////	m_cam1.getcimage(m_imge1);
////
////
////
////
////	
////	CRect rect;                       //图片适应控件大小
////	pWnd1->GetClientRect(&rect);	 //取得客户区尺寸
////	pDC1->SetStretchBltMode(STRETCH_HALFTONE);	 //保持图片不失真
////	m_imge1.Draw( pDC1->m_hDC,rect);	 //已控件尺寸大小来绘图
////	ReleaseDC( pDC1 );
////	
////	return 1;
////}
////
////LRESULT  CcutpicDlg::OnSnapChange2(WPARAM wParam, LPARAM lParam)
////{
////	HVSTATUS status = STATUS_OK;
////	CWnd *pWnd2 = GetDlgItem(IDC_IMAGE2);
////	CDC  *pDC2	= pWnd2->GetDC();	
////	CImage m_imge2 ;
////	m_cam2.getcimage(m_imge2);
////
////	
////	CRect rect2;                       //图片适应控件大小
////	pWnd2->GetClientRect(&rect2);	 //取得客户区尺寸
////	pDC2->SetStretchBltMode(STRETCH_HALFTONE);	 //保持图片不失真
////	m_imge2.Draw( pDC2->m_hDC,rect2);	 //已控件尺寸大小来绘图
////	ReleaseDC( pDC2 );
////	
////	return 1;
////}
//
//HVSTATUS  CcutpicDlg::GetLastStatus()
//{
//	//HV_ARG_GET_LAST_STATUS ArgFeature;
//	//ArgFeature.type = HV_LAST_STATUS_TRANSFER;
//	//
//	//HV_RES_GET_LAST_STATUS ResFeature;
//	//
//	//HVAPI_CONTROL_PARAMETER  p;
//	//p.pInBuf		 = &ArgFeature;
//	//p.dwInBufSize	 = sizeof(ArgFeature);	
//	//p.pOutBuf		 = &ResFeature;
//	//p.dwOutBufSize	 = sizeof(ResFeature);
//	//p.pBytesRet		 = NULL;		
//	//p.code			 = ORD_GET_LAST_STATUS;
//	//int	dwSize = sizeof(p);
//	
////	HVSTATUS status = HVCommand(m_hhv, CMD_HVAPI_CONTROL, &p, &dwSize);
//	/*if(status != STATUS_OK)
//	{
//		TRACE("HVCommand return status is %d", status);
//	}
//	*/
//	/*dwLastStatus = 0;
//	
//	if (HV_SUCCESS(status)) {
//		dwLastStatus = ResFeature.status;
//	}
//	*/
//	HVSTATUS status =STATUS_OK;
//	return status;
//}


LRESULT CcutpicDlg::OnSnapexStop(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your command handler code here
	//HVSTATUS status =STATUS_OK;
	
	//	停止采集图像到内存，可以再次调用HVStartSnapEx启动数字摄像机采集
	/*status = HVStopSnap(m_hhv);
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) {
		m_bStart = FALSE;
	}*/
	return 1;
}

void CcutpicDlg::OnBnClickedOpencam()
{
	m_cam1.HVregister( SnapThreadCallback1,this);
	m_cam2.HVregister( SnapThreadCallback2,this);

	m_hAcqEvent1 = CreateEvent(NULL,FALSE, FALSE, NULL);
	m_hAcqThread1=false;
	m_hAcqThread1 = (HANDLE) _beginthreadex(NULL, 0, AcqThread1, this, 0, NULL);

	m_hAcqEvent2 = CreateEvent(NULL,FALSE, FALSE, NULL);
	m_hAcqThread2=false;
	m_hAcqThread2 = (HANDLE) _beginthreadex(NULL, 0, AcqThread2, this, 0, NULL);

	//m_push =CreateEvent(NULL,TRUE, TRUE, NULL);
	InitializeCriticalSection(&m_protect_img1);
	InitializeCriticalSection(&m_protect_img2);
}

unsigned CcutpicDlg::AcqThread1(void* params)
{
	
	CcutpicDlg* pCcutpicDlg = (CcutpicDlg*) params;

	while(WaitForSingleObject(pCcutpicDlg->m_hAcqEvent1, INFINITE)==WAIT_OBJECT_0)
	{
		EnterCriticalSection(&pCcutpicDlg->m_protect_img1); 
		CWnd *pWnd1 = pCcutpicDlg->GetDlgItem(IDC_IMAGE1);
		CDC  *pDC1	= pWnd1->GetDC();	
		pCcutpicDlg->mat1 = pCcutpicDlg->m_cam1.getmat();

	
		CRect rect;                                   //图片适应控件大小
		pWnd1->GetClientRect(&rect);	              //取得客户区尺寸
		pDC1->SetStretchBltMode(STRETCH_HALFTONE);	  //保持图片不失真
		pCcutpicDlg->DrawMatToHDC(pCcutpicDlg->mat1, pDC1->m_hDC,rect);//已控件尺寸大小来绘图
		pCcutpicDlg->ReleaseDC( pDC1 );
		LeaveCriticalSection(&pCcutpicDlg->m_protect_img1);   
	}
	return 1;
}

unsigned CcutpicDlg::AcqThread2(void* params)
{
	
	CcutpicDlg* pCcutpicDlg = (CcutpicDlg*) params;

	while(WaitForSingleObject(pCcutpicDlg->m_hAcqEvent2, INFINITE)==WAIT_OBJECT_0)
	{
		EnterCriticalSection(&pCcutpicDlg->m_protect_img2); 
		CWnd *pWnd2 = pCcutpicDlg->GetDlgItem(IDC_IMAGE2);
		CDC  *pDC2	= pWnd2->GetDC();	
		pCcutpicDlg->mat2 = pCcutpicDlg->m_cam2.getmat();

		CRect rect;                                   //图片适应控件大小
		pWnd2->GetClientRect(&rect);	              //取得客户区尺寸
		pDC2->SetStretchBltMode(STRETCH_HALFTONE);	  //保持图片不失真
		pCcutpicDlg->DrawMatToHDC(pCcutpicDlg->mat2, pDC2->m_hDC,rect); //已控件尺寸大小来绘图
		pCcutpicDlg->ReleaseDC( pDC2 );
		LeaveCriticalSection(&pCcutpicDlg->m_protect_img2);   
	}
	return 1;
}

void CcutpicDlg::OnBnClickedSavepic()
{
	vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

	num_save++;
	std::string dir="aa";
	char base_name[256]; sprintf(base_name,"%06d.png",num_save);
	std::string left_img_file_name  = dir + "/left_" + base_name;
	std::string right_img_file_name = dir + "/right_" + base_name;

	//ResetEvent(m_push);
	EnterCriticalSection(&m_protect_img1); 
	EnterCriticalSection(&m_protect_img2); 
	//m_imge1.Save(left_img_file_name.c_str());
	//m_imge2.Save(right_img_file_name.c_str());
	cv::imwrite(left_img_file_name,mat1,compression_params);
	cv::imwrite(right_img_file_name,mat2,compression_params);
	LeaveCriticalSection(&m_protect_img2);   
	LeaveCriticalSection(&m_protect_img1);  
	//SetEvent(m_push);

}

void  CcutpicDlg::DrawMatToHDC(cv::Mat mat,HDC hDCDst,CRect rect)
{
	IplImage limge = mat;
	CvvImage cimge;
	cimge.CopyOf(&limge,1);
	cimge.DrawToHDC(hDCDst,rect);

}

void CcutpicDlg::OnBnClickedCaptureimages()
{
	SetTimer(1,500,NULL);

}


void CcutpicDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

	num_capture++;
	std::string dir="Captureimages";
	char base_name[256]; sprintf(base_name,"%06d.png",num_capture);
	std::string left_img_file_name  = dir + "/left_" + base_name;
	std::string right_img_file_name = dir + "/right_" + base_name;

	//ResetEvent(m_push);
	EnterCriticalSection(&m_protect_img1); 
	EnterCriticalSection(&m_protect_img2); 
	//m_imge1.Save(left_img_file_name.c_str());
	//m_imge2.Save(right_img_file_name.c_str());
	cv::imwrite(left_img_file_name,mat1,compression_params);
	cv::imwrite(right_img_file_name,mat2,compression_params);
	LeaveCriticalSection(&m_protect_img2);   
	LeaveCriticalSection(&m_protect_img1);  
	//SetEvent(m_push);
	CDialogEx::OnTimer(nIDEvent);
}


void CcutpicDlg::OnBnClickedStopcapture()
{
	KillTimer(1);
}
