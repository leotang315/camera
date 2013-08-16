
// cutpicDlg.h : header file
//

#pragma once

#include "Camera.h"

	//自定义消息ID号
#define WM_SNAP_CHANGE1		(WM_USER + 100)
#define WM_SNAP_CHANGE2		(WM_USER + 101)
#define WM_SNAP_ERROR		(WM_USER + 102)	
#define WM_SNAP_STOP		(WM_USER + 103)
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
public:
	afx_msg LRESULT OnSnapChange1(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSnapChange2(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSnapError(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSnapexStop(WPARAM wParam, LPARAM lParam);
private:
	/// 采集回调函数，用户也可以定义为全局函数，如果作为类的成员函数，必须为静态成员函数。
	static int CALLBACK SnapThreadCallback1(HV_SNAP_INFO *pInfo);
	static int CALLBACK SnapThreadCallback2(HV_SNAP_INFO *pInfo);
	static unsigned __stdcall AcqThread1(void *params);
	static unsigned __stdcall AcqThread2(void *params);
		/// 获取状态码
	HVSTATUS GetLastStatus();
	int  m_nError;			///< 错误计数
	int  m_nLost;			///< 丢帧计数
	HANDLE m_hAcqEvent1;
	HANDLE m_hAcqEvent2;
	//HANDLE m_push;
	CRITICAL_SECTION m_protect_img1;
	CRITICAL_SECTION m_protect_img2;
	HANDLE m_hAcqThread1;
	HANDLE m_hAcqThread2;
	CImage m_imge1 ;
	CImage m_imge2 ;
	int num; 
public:
	int dwLastStatus;
	HV_Camera m_cam1;
	HV_Camera m_cam2;
	afx_msg void OnBnClickedOpencam();
	afx_msg void OnBnClickedSavepic();
};
