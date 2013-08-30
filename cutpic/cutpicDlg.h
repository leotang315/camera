
// cutpicDlg.h : header file
//

#pragma once

#include "Camera.h"
#include "CvvImage.h"
#include "StereoCalib.h"
#include <vector>
#include "libelas\elas.h"
using namespace std;

// CcutpicDlg dialog
class CcutpicDlg : public CDialogEx
{
// Construction
public:
	CcutpicDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CUTPIC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
private:
	/// 采集回调函数，用户也可以定义为全局函数，如果作为类的成员函数，必须为静态成员函数。
	static int CALLBACK SnapThreadCallback1(HV_SNAP_INFO *pInfo);   //作为大恒摄像机的连续采集图像的回调函数
	static int CALLBACK SnapThreadCallback2(HV_SNAP_INFO *pInfo);
	static unsigned __stdcall AcqThread1(void *params);				//程序线程函数 从摄像机读取并显示图像
	static unsigned __stdcall AcqThread2(void *params);
	static unsigned __stdcall ShowdispThread(void *params);			//程序线程函数 处理和现实视差图
	static unsigned __stdcall ReadfromfileThread(void *params);			//程序线程函数  从文件读取图像和SnapThreadCallback1有类似功能

	static void DrawMatToHDC(cv::Mat,HDC hDCDst,CRect rect);		
	int getDisparityImage(cv::Mat& disparity, cv::Mat& disparityImage, bool isColor);

	HANDLE m_hAcqEvent1;				    //摄像机每次采集一帧图像  通过回调函数 SnapThreadCallback1 将发出此事件                   
	HANDLE m_hAcqEvent2;
	HANDLE m_hReadEvent1;
	HANDLE m_hReadEvent2;
	CRITICAL_SECTION m_protect_mat1;		//mat1的保护区				
	CRITICAL_SECTION m_protect_mat2;		//mat2的保护区	
	CRITICAL_SECTION m_protect_workmat1;	//workmat1的保护区				
	CRITICAL_SECTION m_protect_workmat2;	//workmat2的保护区	
	HANDLE m_hAcqThread1;
	HANDLE m_hAcqThread2;
	HANDLE m_hShowdispThread;
	HANDLE m_hReadfromfileThread;

	int num_save; 
	int num_capture;
public:
	cv::Mat mat1,mat2;				//一直保存摄像机采集的图像，更新很快
	cv::Mat workmat1,workmat2;		//程序拷贝下来需要处理的图像，更新随程序，较慢

	vector<cv::Mat> vectmat1;
	vector<cv::Mat> vectmat2;
	bool capture_stat;               //连续采集标志位
	bool rectify_stat;               //校正标志位
	StereoCalib stereocal;
	HV_Camera m_cam1;
	HV_Camera m_cam2;
	afx_msg LRESULT OnSnapexStop(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedOpencam();
	afx_msg void OnBnClickedSavepic();
	afx_msg void OnBnClickedCaptureimages();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStopcapture();
	afx_msg void OnBnClickedLoadcalibdata();
	afx_msg void OnBnClickedRemapimage();
	afx_msg void OnBnClickedShowdisp();
	afx_msg void OnBnClickedWorkonfile();
};
