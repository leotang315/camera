
// cutpicDlg.h : header file
//

#pragma once

#include "Camera.h"
#include "CvvImage.h"
#include "StereoCalib.h"
#include <vector>
using namespace std;
	//�Զ�����ϢID��
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
	//afx_msg LRESULT OnSnapChange1(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnSnapChange2(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnSnapError(WPARAM wParam, LPARAM lParam);
	
private:
	/// �ɼ��ص��������û�Ҳ���Զ���Ϊȫ�ֺ����������Ϊ��ĳ�Ա����������Ϊ��̬��Ա������
	static int CALLBACK SnapThreadCallback1(HV_SNAP_INFO *pInfo);
	static int CALLBACK SnapThreadCallback2(HV_SNAP_INFO *pInfo);
	static unsigned __stdcall AcqThread1(void *params);
	static unsigned __stdcall AcqThread2(void *params);
	static void DrawMatToHDC(cv::Mat,HDC hDCDst,CRect rect);
		/// ��ȡ״̬��
	//HVSTATUS GetLastStatus();
	int  m_nError;			///< �������
	int  m_nLost;			///< ��֡����
	HANDLE m_hAcqEvent1;
	HANDLE m_hAcqEvent2;
	//HANDLE m_push;
	CRITICAL_SECTION m_protect_img1;
	CRITICAL_SECTION m_protect_img2;
	HANDLE m_hAcqThread1;
	HANDLE m_hAcqThread2;
	/*CImage m_imge1 ;
	CImage m_imge2 ;*/


	int num_save; 
	int num_capture;
public:
	cv::Mat mat1;
	cv::Mat mat2;
	vector<cv::Mat> vectmat1;
	vector<cv::Mat> vectmat2;
	bool capture_stat;               //�����ɼ���־λ
	bool rectify_stat;               //У����־λ
	StereoCalib stereocal;
	int dwLastStatus;
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
};
