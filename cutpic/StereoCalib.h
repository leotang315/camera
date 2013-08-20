#ifndef _STEREO_CALIB_H
#define _STEREO_CALIB_H

#pragma once


#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
using namespace std;
using namespace cv;
class StereoCalib
{
public:
	StereoCalib(void);
	~StereoCalib(void);

	/***
	 *	˫Ŀ�궨���������
	 */
	struct StereoParams
	{
		cv::Size		imageSize;		          // ͼ��ֱ���
		cv::Mat			cameraMatrix1;			  // ���������
		cv::Mat			distortionCoefficients1;  // ������������
		cv::Mat			cameraMatrix2;			  // ���������
		cv::Mat			distortionCoefficients2;  // ������������

		cv::Mat			rotation;				  // ��ת����
		cv::Mat			translation;			  // ƽ������
		cv::Mat			essential;				  // ���ʾ���
		cv::Mat			foundational;			  // ��������
		
	};

	/***
	 *	˫ĿУ�����������
	 */
	struct RemapMatrixs
	{
		cv::Mat		mX1;	// ����ͼ X ��������ӳ�����
		cv::Mat		mY1;	// ����ͼ Y ��������ӳ�����
		cv::Mat		mX2;	// ����ͼ X ��������ӳ�����
		cv::Mat		mY2;	// ����ͼ Y ��������ӳ�����
		cv::Mat		Q;		// ���ڼ�����ά���Ƶ� Q ����
		cv::Rect	roi1;	// ����ͼ��Ч����ľ���
		cv::Rect	roi2;	// ����ͼ��Ч����ľ���
	};

	int LoadCalibData(const char* filename = "CalibData.yml");

	int SaveCalibData(const char* filename = "CalibData.yml");
    int RectifyStereoCamera();

	int RemapStereoImage(cv::Mat& img1, cv::Mat& img2);
	

	StereoParams m_stereoparams;
	RemapMatrixs m_remapmatrixs;
private:
	void my_cvRodrigues2(const cv::Mat &src, cv::Mat  &dst);
};

#endif
