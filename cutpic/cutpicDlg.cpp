
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
	ON_BN_CLICKED(IDC_OPENCAM, &CcutpicDlg::OnBnClickedOpencam)
	ON_BN_CLICKED(IDC_SAVEPIC, &CcutpicDlg::OnBnClickedSavepic)
	ON_BN_CLICKED(IDC_Capture_images, &CcutpicDlg::OnBnClickedCaptureimages)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_Stop_capture, &CcutpicDlg::OnBnClickedStopcapture)
	ON_BN_CLICKED(IDC_Load_calib_data, &CcutpicDlg::OnBnClickedLoadcalibdata)
	ON_BN_CLICKED(IDC_Remap_image, &CcutpicDlg::OnBnClickedRemapimage)
	ON_BN_CLICKED(IDC_Show_disp, &CcutpicDlg::OnBnClickedShowdisp)
	ON_BN_CLICKED(IDC_Workon_file, &CcutpicDlg::OnBnClickedWorkonfile)
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
	capture_stat=false;
	rectify_stat=false;

	InitializeCriticalSection(&m_protect_mat1);
	InitializeCriticalSection(&m_protect_mat2);
	InitializeCriticalSection(&m_protect_workmat1);
	InitializeCriticalSection(&m_protect_workmat2);

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
	����:
		SnapThreadCallback
	�������:
		SNAP_INFO *pInfo		SNAP_INFO�ṹ������ǰ���������SNAPִ��״̬
	�������:
		int						
	˵��:
		����������ɼ����ڴ�ص����������û�һ�㲻�õ��ã����û��ṩ��SDKʹ�ã�
		�û��ڻص�������ʵ�ֶԲɼ����ݵĴ������ʾ����
 */
int CALLBACK  CcutpicDlg::SnapThreadCallback1(HV_SNAP_INFO *pInfo)
{
	CcutpicDlg *This = (CcutpicDlg *)(pInfo->pParam);
	HWND hwnd = This->m_hWnd;
	EnterCriticalSection(&This->m_protect_mat1); 
	This->mat1 = This->m_cam1.getmat();          //��ȡͼ���Ƶ������ڴ���mat1���Ա㴦��
	LeaveCriticalSection(&This->m_protect_mat1); 

	SetEvent(This->m_hAcqEvent1);
	
	return 1;
}

int CALLBACK  CcutpicDlg::SnapThreadCallback2(HV_SNAP_INFO *pInfo)
{
	CcutpicDlg *This = (CcutpicDlg *)(pInfo->pParam);
	HWND hwnd = This->m_hWnd;
	EnterCriticalSection(&This->m_protect_mat2); 
	This->mat2 = This->m_cam2.getmat();          //��ȡͼ���Ƶ������ڴ���mat2���Ա㴦��
	LeaveCriticalSection(&This->m_protect_mat2); 
	
	SetEvent(This->m_hAcqEvent2);

	return 1;
}


///*
//	����:
//		OnSnapError
//	�������:
//		WPARAM wParam			û��ʹ��		
//		LPARAM lParam			�쳣״̬��
//	�������:
//		LRESULT						
//	˵��:
//		������ɼ��쳣���󱨸�
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
////	����:
////		OnSnapChange
////	�������:
////		WPARAM wParam			�ֲ���������Ϣ��Ϊ��ǰ���Դ����ͼ�����		
////		LPARAM lParam			û��ʹ��
////	�������:
////		LRESULT						
////	˵��:
////		ʵ�ֶԲɼ����ݵĴ������ʾ
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
////	CRect rect;                       //ͼƬ��Ӧ�ؼ���С
////	pWnd1->GetClientRect(&rect);	 //ȡ�ÿͻ����ߴ�
////	pDC1->SetStretchBltMode(STRETCH_HALFTONE);	 //����ͼƬ��ʧ��
////	m_imge1.Draw( pDC1->m_hDC,rect);	 //�ѿؼ��ߴ��С����ͼ
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
////	CRect rect2;                       //ͼƬ��Ӧ�ؼ���С
////	pWnd2->GetClientRect(&rect2);	 //ȡ�ÿͻ����ߴ�
////	pDC2->SetStretchBltMode(STRETCH_HALFTONE);	 //����ͼƬ��ʧ��
////	m_imge2.Draw( pDC2->m_hDC,rect2);	 //�ѿؼ��ߴ��С����ͼ
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

	

}

unsigned CcutpicDlg::AcqThread1(void* params)
{
	
	CcutpicDlg* pCcutpicDlg = (CcutpicDlg*) params;

	while(WaitForSingleObject(pCcutpicDlg->m_hAcqEvent1, INFINITE)==WAIT_OBJECT_0)
	{
		//����mat1���ݷ���workmat1
		EnterCriticalSection(&pCcutpicDlg->m_protect_workmat1); 
		EnterCriticalSection(&pCcutpicDlg->m_protect_mat1); 
		pCcutpicDlg->workmat1 = pCcutpicDlg->mat1.clone();
		LeaveCriticalSection(&pCcutpicDlg->m_protect_mat1);  
		LeaveCriticalSection(&pCcutpicDlg->m_protect_workmat1);
		//У��workmat1ͼ�񣬲����俼����ʱ��������
		EnterCriticalSection(&pCcutpicDlg->m_protect_workmat1); 
		if(pCcutpicDlg->rectify_stat)
			remap(pCcutpicDlg->workmat1, pCcutpicDlg->workmat1, pCcutpicDlg->stereocal.m_remapmatrixs.mX1, pCcutpicDlg->stereocal.m_remapmatrixs.mY1, INTER_LINEAR);//��ͼ�����У��
		Mat tempmat =pCcutpicDlg->workmat1.clone();
		LeaveCriticalSection(&pCcutpicDlg->m_protect_workmat1);  

		//�����ɼ�ͼ�񲢱��浽�ڴ�vectmat1��	
		if(pCcutpicDlg->capture_stat)									
			pCcutpicDlg->vectmat1.push_back(tempmat);

		//��ʾͼ��ָ��ͼ��ؼ�			
		CWnd *pWnd1 = pCcutpicDlg->GetDlgItem(IDC_IMAGE1);
		CDC  *pDC1	= pWnd1->GetDC();	
		CRect rect;												//ͼƬ��Ӧ�ؼ���С
		pWnd1->GetClientRect(&rect);							//ȡ�ÿͻ����ߴ�
		pDC1->SetStretchBltMode(STRETCH_HALFTONE);				//����ͼƬ��ʧ��
		pCcutpicDlg->DrawMatToHDC(tempmat, pDC1->m_hDC,rect);	//�ѿؼ��ߴ��С����ͼ
		pCcutpicDlg->ReleaseDC( pDC1 );
		
	}
	return 1;
}

unsigned CcutpicDlg::AcqThread2(void* params)
{
	
	CcutpicDlg* pCcutpicDlg = (CcutpicDlg*) params;

	while(WaitForSingleObject(pCcutpicDlg->m_hAcqEvent2, INFINITE)==WAIT_OBJECT_0)
	{
		//����mat2���ݷ���workmat2
		EnterCriticalSection(&pCcutpicDlg->m_protect_workmat2); 
		EnterCriticalSection(&pCcutpicDlg->m_protect_mat2); 
		pCcutpicDlg->workmat2 = pCcutpicDlg->mat2.clone();
		LeaveCriticalSection(&pCcutpicDlg->m_protect_mat2);  
		LeaveCriticalSection(&pCcutpicDlg->m_protect_workmat2);	
		
		//У��workmat2ͼ�񣬲����俼����ʱ��������
		EnterCriticalSection(&pCcutpicDlg->m_protect_workmat2);
		if(pCcutpicDlg->rectify_stat)
			remap(pCcutpicDlg->workmat2, pCcutpicDlg->workmat2, pCcutpicDlg->stereocal.m_remapmatrixs.mX2, pCcutpicDlg->stereocal.m_remapmatrixs.mY2, INTER_LINEAR);
		cv::Mat tempmat=pCcutpicDlg->mat2.clone();	
		LeaveCriticalSection(&pCcutpicDlg->m_protect_workmat2); 

		//�����ɼ�ͼ�񲢱��浽�ڴ�vectmat2��	
		if(pCcutpicDlg->capture_stat)
			pCcutpicDlg->vectmat2.push_back(tempmat);

		//��ʾͼ��ָ��ͼ��ؼ�	
		CWnd *pWnd2 = pCcutpicDlg->GetDlgItem(IDC_IMAGE2);
		CDC  *pDC2	= pWnd2->GetDC();
		CRect rect;														//ͼƬ��Ӧ�ؼ���С
		pWnd2->GetClientRect(&rect);									//ȡ�ÿͻ����ߴ�
		pDC2->SetStretchBltMode(STRETCH_HALFTONE);						//����ͼƬ��ʧ��
		pCcutpicDlg->DrawMatToHDC(tempmat, pDC2->m_hDC,rect); //�ѿؼ��ߴ��С����ͼ
		pCcutpicDlg->ReleaseDC( pDC2 );  
	}
	return 1;
}

unsigned CcutpicDlg::ShowdispThread(void *params)
{
	CcutpicDlg* pCcutpicDlg = (CcutpicDlg*) params;
	Elas::parameters param;
	Elas elas(param);
	Mat tempmat1,tempmat2,disp;
	if(!pCcutpicDlg->rectify_stat)
	{
		   //ûУ��ͼ�������
	}
	while(1)
	{
		
		EnterCriticalSection(&pCcutpicDlg->m_protect_workmat1); 
		EnterCriticalSection(&pCcutpicDlg->m_protect_workmat2); 
		tempmat1 = pCcutpicDlg->workmat1.clone();
		tempmat2 = pCcutpicDlg->workmat2.clone();
		LeaveCriticalSection(&pCcutpicDlg->m_protect_workmat2);  
		LeaveCriticalSection(&pCcutpicDlg->m_protect_workmat1); 

		elas.function(tempmat1,tempmat2,disp);
		pCcutpicDlg->getDisparityImage(disp,disp,1);
		CWnd *pWnd3 = pCcutpicDlg->GetDlgItem(IDC_IMAGE3);
		CDC  *pDC3	= pWnd3->GetDC();	
		CRect rect;											//ͼƬ��Ӧ�ؼ���С
		pWnd3->GetClientRect(&rect);						//ȡ�ÿͻ����ߴ�
		pDC3->SetStretchBltMode(STRETCH_HALFTONE);			//����ͼƬ��ʧ��
		pCcutpicDlg->DrawMatToHDC(disp, pDC3->m_hDC,rect);  //�ѿؼ��ߴ��С����ͼ
		pCcutpicDlg->ReleaseDC( pDC3 );
		SetEvent(pCcutpicDlg->m_hReadEvent1);
		SetEvent(pCcutpicDlg->m_hReadEvent2);
	}
	
	return 1;
}

unsigned CcutpicDlg::ReadfromfileThread(void* params)
{
	CcutpicDlg* pCcutpicDlg = (CcutpicDlg*) params;

	std::string dir="D:/my_opencv/С����/�����/Debug/aa/data4";
	CString path;
	path=dir.c_str();
	int count = 0;
	CFileFind finder;
	BOOL working = finder.FindFile(path + "\\*.*");
	while (working)
	{
		working = finder.FindNextFile();
		if (finder.IsDots())
		continue;
		if (!finder.IsDirectory())
		count++;
	}
	count =count/2;

	for(int i=1;i<+count;i++)
	{
		if(WaitForSingleObject(pCcutpicDlg->m_hReadEvent1, INFINITE)==WAIT_OBJECT_0
			  &&WaitForSingleObject(pCcutpicDlg->m_hReadEvent2, INFINITE)==WAIT_OBJECT_0)
		{
			
			char base_name[256]; sprintf(base_name,"%06d.png",i);
			std::string left_img_file_name  = dir + "/left_" + base_name;
			std::string right_img_file_name = dir + "/right_" + base_name;
			EnterCriticalSection(&pCcutpicDlg->m_protect_mat1); 
			EnterCriticalSection(&pCcutpicDlg->m_protect_mat2); 
			pCcutpicDlg->mat1 = imread(left_img_file_name);
			pCcutpicDlg->mat2 = imread(right_img_file_name);
			LeaveCriticalSection(&pCcutpicDlg->m_protect_mat2);  
			LeaveCriticalSection(&pCcutpicDlg->m_protect_mat1);
			
			SetEvent(pCcutpicDlg->m_hAcqEvent1);
			SetEvent(pCcutpicDlg->m_hAcqEvent2);
		
		 
		}
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
	EnterCriticalSection(&m_protect_workmat1); 
	EnterCriticalSection(&m_protect_workmat2); 
	//m_imge1.Save(left_img_file_name.c_str());
	//m_imge2.Save(right_img_file_name.c_str());
	cv::imwrite(left_img_file_name,workmat1,compression_params);
	cv::imwrite(right_img_file_name,workmat2,compression_params);
	LeaveCriticalSection(&m_protect_workmat2);   
	LeaveCriticalSection(&m_protect_workmat1);  
	//SetEvent(m_push);

}

void  CcutpicDlg::DrawMatToHDC(cv::Mat mat,HDC hDCDst,CRect rect)
{
	IplImage limge = mat;
	CvvImage cimge;
	cimge.CopyOf(&limge,1);
	cimge.DrawToHDC(hDCDst,rect);

}

int CcutpicDlg::getDisparityImage(cv::Mat& src, cv::Mat& dest, bool isColor)
{
	// ��ԭʼ�Ӳ����ݵ�λ��ת��Ϊ 8 λ
	cv::Mat m_src8u;
	if (src.depth() != CV_8U)
	{
		//src.convertTo(m_src8u, CV_8U, 255/(m_numberOfDisparies*16.));
		
	} 
	else
	{
		m_src8u = src;
	}

	// ת��Ϊα��ɫͼ�� �� �Ҷ�ͼ��
	if (isColor)
	{
		if (dest.empty() || dest.type() != CV_8UC3 )
		{
			dest = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);
		}

		for (int y=0;y<m_src8u.rows;y++)
		{
			for (int x=0;x<m_src8u.cols;x++)
			{
				uchar val = m_src8u.at<uchar>(y,x);
				uchar r,g,b;

				if (val==0) 
					r = g = b = 0;
				else
				{
					r = 255-val;
					g = val < 128 ? val*2 : (uchar)((255 - val)*2);
					b = val;
				}

				dest.at<cv::Vec3b>(y,x) = cv::Vec3b(r,g,b);
			}
		}
	} 
	else
	{
		m_src8u.copyTo(dest);
	}

	return 1;
}

void CcutpicDlg::OnBnClickedCaptureimages()
{
	//SetTimer(1,100,NULL);
	capture_stat=true;

}

void CcutpicDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//double t = (double)cv::getTickCount();

	//vector<int> compression_params;
 //   compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
 //   compression_params.push_back(3);

	//num_capture++;
	//std::string dir="Captureimages";
	//char base_name[256]; sprintf(base_name,"%06d.png",num_capture);
	//std::string left_img_file_name  = dir + "/left_" + base_name;
	//std::string right_img_file_name = dir + "/right_" + base_name;

	////ResetEvent(m_push);
	//EnterCriticalSection(&m_protect_mat1); 
	//EnterCriticalSection(&m_protect_mat2); 
	////m_imge1.Save(left_img_file_name.c_str());
	////m_imge2.Save(right_img_file_name.c_str());
	//cv::imwrite(left_img_file_name,mat1,compression_params);
	//cv::imwrite(right_img_file_name,mat2,compression_params);
	//LeaveCriticalSection(&m_protect_mat2);   
	//LeaveCriticalSection(&m_protect_mat1);  

	//t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();

	//EnterCriticalSection(&m_protect_mat1); 
	//EnterCriticalSection(&m_protect_mat2); 
	//vectmat1.push_back(mat1);
	//vectmat2.push_back(mat2);
	//LeaveCriticalSection(&m_protect_mat2);   
	//LeaveCriticalSection(&m_protect_mat1);  

	//SetEvent(m_push);
	CDialogEx::OnTimer(nIDEvent);
}

void CcutpicDlg::OnBnClickedStopcapture()
{
	KillTimer(1);
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(3);

	capture_stat=false;
	const int n =vectmat1.size();

	for(int i=0;i<n;i++)
	{
		std::string dir="Captureimages";
		char base_name[256]; sprintf(base_name,"%06d.png",i);
		std::string left_img_file_name  = dir + "/left_" + base_name;
		std::string right_img_file_name = dir + "/right_" + base_name;

		cv::imwrite(left_img_file_name,vectmat1[i],compression_params);
		cv::imwrite(right_img_file_name,vectmat2[i],compression_params);
	}

	
}


void CcutpicDlg::OnBnClickedLoadcalibdata()
{
	if(!stereocal.LoadCalibData())
	{
		//�������ݴ�����add��ʾ����
	};

	if(!stereocal.RectifyStereoCamera())
	{
		//У�����ݴ�����add��ʾ����
	}

	{
		//���ر궨���ݳɹ�����һ������
	}
}


void CcutpicDlg::OnBnClickedRemapimage()
{
	rectify_stat =true;
}


void CcutpicDlg::OnBnClickedShowdisp()
{
	m_hShowdispThread=false;
	m_hShowdispThread = (HANDLE) _beginthreadex(NULL, 0, ShowdispThread, this, 0, NULL);
}


void CcutpicDlg::OnBnClickedWorkonfile()
{
	m_hAcqEvent1 = CreateEvent(NULL,FALSE, FALSE, NULL);
	m_hAcqThread1=false;
	m_hAcqThread1 = (HANDLE) _beginthreadex(NULL, 0, AcqThread1, this, 0, NULL);

	m_hAcqEvent2 = CreateEvent(NULL,FALSE, FALSE, NULL);
	m_hAcqThread2=false;
	m_hAcqThread2 = (HANDLE) _beginthreadex(NULL, 0, AcqThread2, this, 0, NULL);

	m_hReadEvent1 = CreateEvent(NULL,FALSE, TRUE, NULL);
	m_hReadEvent2 = CreateEvent(NULL,FALSE, TRUE, NULL);
	m_hReadfromfileThread=false;
	m_hReadfromfileThread = (HANDLE) _beginthreadex(NULL, 0, ReadfromfileThread, this, 0, NULL);



	
}

