#include "StdAfx.h"
#include "StereoCalib.h"


StereoCalib::StereoCalib(void)
{
}


StereoCalib::~StereoCalib(void)
{
}

int StereoCalib::LoadCalibData(const char* filename)
{
	cv::FileStorage fs(filename, cv::FileStorage::READ);		
	if ( !fs.isOpened() )
		return (0);
	cv::Size imageSize;
	cv::FileNodeIterator it = fs["imageSize"].begin(); 
	it >> m_stereoparams.imageSize.width >> m_stereoparams.imageSize.height;

	fs["cameraMatrix1"] >>  m_stereoparams.cameraMatrix1;
	fs["distortionCoefficients1"] >>  m_stereoparams.distortionCoefficients1;
	fs["cameraMatrix2"] >>  m_stereoparams.cameraMatrix2;
	fs["distortionCoefficients2"] >>  m_stereoparams.distortionCoefficients2;
	fs["translation"] >>  m_stereoparams.translation;

	Mat R;
	m_stereoparams.rotation = (Mat_<double>(3,3) << 0, 0, 0, 0, 0, 0, 0, 0,0);
	fs["rotation"] >> R;
	my_cvRodrigues2(R,m_stereoparams.rotation);
	

	return 1;
}

int StereoCalib::SaveCalibData(const char* filename)
{
	cv::FileStorage fs(filename, cv::FileStorage::WRITE);		
	if ( !fs.isOpened() )
		return (0);
	fs<<"imageSize"<<"["<<m_stereoparams.imageSize.width<<m_stereoparams.imageSize.height<<"]";
	fs<<"cameraMatrix1"<<m_stereoparams.cameraMatrix1;
	fs<<"distortionCoefficients1"<<m_stereoparams.distortionCoefficients1;
	fs<<"cameraMatrix2"<<m_stereoparams.cameraMatrix2;
	fs<<"distortionCoefficients2"<<m_stereoparams.distortionCoefficients2;
	fs<<"rotation"<<m_stereoparams.rotation;
	fs<<"translation"<<m_stereoparams.translation;
	return 1;
}

int StereoCalib::RectifyStereoCamera()
{
	cv::Mat  R1, P1, R2, P2;
	stereoRectify( m_stereoparams.cameraMatrix1, m_stereoparams.distortionCoefficients1, 
				   m_stereoparams.cameraMatrix2, m_stereoparams.distortionCoefficients2, 
				   m_stereoparams.imageSize, m_stereoparams.rotation,m_stereoparams.translation, 
				   R1, R2, P1, P2, m_remapmatrixs.Q, CALIB_ZERO_DISPARITY, 0,  m_stereoparams.imageSize, &m_remapmatrixs.roi1, &m_remapmatrixs.roi2 );

    initUndistortRectifyMap(m_stereoparams.cameraMatrix1, m_stereoparams.distortionCoefficients1, 
							R1, P1, m_stereoparams.imageSize, CV_16SC2, m_remapmatrixs.mX1, m_remapmatrixs.mY1);
    initUndistortRectifyMap(m_stereoparams.cameraMatrix2, m_stereoparams.distortionCoefficients2, 
							R2, P2, m_stereoparams.imageSize, CV_16SC2, m_remapmatrixs.mX2, m_remapmatrixs.mY2);

	return 1;
}

int StereoCalib::RemapStereoImage(cv::Mat& img1, cv::Mat& img2)
{
	cv::Mat img1r;
	cv::Mat img2r;
	remap(img1, img1r, m_remapmatrixs.mX1, m_remapmatrixs.mY1, INTER_LINEAR);
    remap(img2, img2r, m_remapmatrixs.mX2, m_remapmatrixs.mY2,INTER_LINEAR);

	img1 =img1r;
	img2 =img2r;
	return 1;
}

void StereoCalib::my_cvRodrigues2(const cv::Mat &src, cv::Mat  &dst)
{
	CvMat m_src = src;

    CvMat m_dst =dst;
	cvRodrigues2(&m_src, &m_dst,0);
	cv::Mat temp(&m_dst,1);
	dst = temp;
}