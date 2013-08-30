
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
	/// �ɼ��ص��������û�Ҳ���Զ���Ϊȫ�ֺ����������Ϊ��ĳ�Ա����������Ϊ��̬��Ա������
	static int CALLBACK SnapThreadCallback1(HV_SNAP_INFO *pInfo);   //��Ϊ���������������ɼ�ͼ��Ļص�����
	static int CALLBACK SnapThreadCallback2(HV_SNAP_INFO *pInfo);
	static unsigned __stdcall AcqThread1(void *params);				//�����̺߳��� ���������ȡ����ʾͼ��
	static unsigned __stdcall AcqThread2(void *params);
	static unsigned __stdcall ShowdispThread(void *params);			//�����̺߳��� �������ʵ�Ӳ�ͼ
	static unsigned __stdcall ReadfromfileThread(void *params);			//�����̺߳���  ���ļ���ȡͼ���SnapThreadCallback1�����ƹ���

	static void DrawMatToHDC(cv::Mat,HDC hDCDst,CRect rect);		
	int getDisparityImage(cv::Mat& disparity, cv::Mat& disparityImage, bool isColor);

	HANDLE m_hAcqEvent1;				    //�����ÿ�βɼ�һ֡ͼ��  ͨ���ص����� SnapThreadCallback1 ���������¼�                   
	HANDLE m_hAcqEvent2;
	HANDLE m_hReadEvent1;
	HANDLE m_hReadEvent2;
	CRITICAL_SECTION m_protect_mat1;		//mat1�ı�����				
	CRITICAL_SECTION m_protect_mat2;		//mat2�ı�����	
	CRITICAL_SECTION m_protect_workmat1;	//workmat1�ı�����				
	CRITICAL_SECTION m_protect_workmat2;	//workmat2�ı�����	
	HANDLE m_hAcqThread1;
	HANDLE m_hAcqThread2;
	HANDLE m_hShowdispThread;
	HANDLE m_hReadfromfileThread;

	int num_save; 
	int num_capture;
public:
	cv::Mat mat1,mat2;				//һֱ����������ɼ���ͼ�񣬸��ºܿ�
	cv::Mat workmat1,workmat2;		//���򿽱�������Ҫ�����ͼ�񣬸�������򣬽���

	vector<cv::Mat> vectmat1;
	vector<cv::Mat> vectmat2;
	bool capture_stat;               //�����ɼ���־λ
	bool rectify_stat;               //У����־λ
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
