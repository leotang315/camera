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
	 *	双目标定的输入参数
	 */
	struct StereoParams
	{
		cv::Size		imageSize;		          // 图像分辨率
		cv::Mat			cameraMatrix1;			  // 摄像机矩阵
		cv::Mat			distortionCoefficients1;  // 摄像机畸变参数
		cv::Mat			cameraMatrix2;			  // 摄像机矩阵
		cv::Mat			distortionCoefficients2;  // 摄像机畸变参数

		cv::Mat			rotation;				  // 旋转矩阵
		cv::Mat			translation;			  // 平移向量
		cv::Mat			essential;				  // 本质矩阵
		cv::Mat			foundational;			  // 基础矩阵
		
	};

	/***
	 *	双目校正的输出参数
	 */
	struct RemapMatrixs
	{
		cv::Mat		mX1;	// 左视图 X 方向像素映射矩阵
		cv::Mat		mY1;	// 左视图 Y 方向像素映射矩阵
		cv::Mat		mX2;	// 右视图 X 方向像素映射矩阵
		cv::Mat		mY2;	// 右视图 Y 方向像素映射矩阵
		cv::Mat		Q;		// 用于计算三维点云的 Q 矩阵
		cv::Rect	roi1;	// 左视图有效区域的矩形
		cv::Rect	roi2;	// 右视图有效区域的矩形
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
