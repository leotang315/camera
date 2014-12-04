// mylibelas.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include"opencv.hpp"
#include "cv.h"


#include <iostream>
//#include "elas.h"
//#include "image.h"

using namespace std;
using namespace cv;


//void mergeoft(InputArray _src1, InputArray _src2, OutputArray _dst)
//{
//	Mat src1 = _src1.getMat();
//	Mat src2 = _src2.getMat();
//	CV_Assert( src1.dims <= 2 );
//	CV_Assert( src2.dims <= 2 );
//	CV_Assert( src1.type()==src2.type() );
//
//	int row = src1.rows > src2.rows ? src1.rows : src2.rows;
//	int col = src1.cols+src2.cols+10;
//	_dst.create(row,col, src1.type());
//	Mat dst = _dst.getMat();
//
//	Mat roi_1 = dst(cv::Rect(0,0,src1.cols,src1.rows));
//	src1.copyTo(roi_1);
//
//	Mat roi_2(dst,cv::Rect(src1.cols+10,0,src2.cols,src2.rows));
//	src2.copyTo(roi_2);
//
//
//	//cvtColor(dst,dst, CV_GRAY2RGB);
//	int dt =10;
//	for(int i =0 ;i*dt<dst.rows;i++)
//	{
//		line(dst, Point(0,i*dt), Point(dst.cols,i*dt), Scalar( 255,0,0 ),0.5);
//	}
//
//
//}
//
//// compute disparities of pgm image input pair file_1, file_2
//void process (const char* file_1,const char* file_2) {
//
//  cout << "Processing: " << file_1 << ", " << file_2 << endl;
//
//  // load images
//  image<uchar> *I1,*I2;
//  I1 = loadPGM(file_1);
//  I2 = loadPGM(file_2);
//
//  // check for correct size
//  if (I1->width()<=0 || I1->height() <=0 || I2->width()<=0 || I2->height() <=0 ||
//      I1->width()!=I2->width() || I1->height()!=I2->height()) {
//    cout << "ERROR: Images must be of same size, but" << endl;
//    cout << "       I1: " << I1->width() <<  " x " << I1->height() << 
//                 ", I2: " << I2->width() <<  " x " << I2->height() << endl;
//    delete I1;
//    delete I2;
//    return;    
//  }
//
//  // get image width and height
//  int32_t width  = I1->width();
//  int32_t height = I1->height();
//
//  // allocate memory for disparity images
//  const int32_t dims[3] = {width,height,width}; // bytes per line = width
//  float* D1_data = (float*)malloc(width*height*sizeof(float));
//  float* D2_data = (float*)malloc(width*height*sizeof(float));
//
//  // process
//  Elas::parameters param;
//  param.postprocess_only_left = false;
//  Elas elas(param);
//  elas.process(I1->data,I2->data,D1_data,D2_data,dims);
//
//  // find maximum disparity for scaling output disparity images to [0..255]
//  float disp_max = 0;
//  for (int32_t i=0; i<width*height; i++) {
//    if (D1_data[i]>disp_max) disp_max = D1_data[i];
//    if (D2_data[i]>disp_max) disp_max = D2_data[i];
//  }
//
//  // copy float to uchar
//  image<uchar> *D1 = new image<uchar>(width,height);
//  image<uchar> *D2 = new image<uchar>(width,height);
//  for (int32_t i=0; i<width*height; i++) {
//    D1->data[i] = (uint8_t)max(255.0*D1_data[i]/disp_max,0.0);
//    D2->data[i] = (uint8_t)max(255.0*D2_data[i]/disp_max,0.0);
//  }
//
//  // save disparity images
//  char output_1[1024];
//  char output_2[1024];
//  strncpy_s(output_1,file_1,strlen(file_1)-4);
//  strncpy_s(output_2,file_2,strlen(file_2)-4);
//  output_1[strlen(file_1)-4] = '\0';
//  output_2[strlen(file_2)-4] = '\0';
//  strcat_s(output_1,"_disp.pgm");
//  strcat_s(output_2,"_disp.pgm");
//  savePGM(D1,output_1);
//  savePGM(D2,output_2);
//
//  // free memory
//  delete I1;
//  delete I2;
//  delete D1;
//  delete D2;
//  free(D1_data);
//  free(D2_data);
//}
//void function (cv::Mat &mat1,cv::Mat &mat2)
//{
//	Mat exmat1;
//	Mat exmat2;
//	  // check for correct size
//	if (mat1.cols<=0 || mat1.rows <=0 || mat2.cols<=0 ||mat2.rows <=0 ) 
//	{
//		cout << "error: image are not existed";
//		return;    
//
//	}
//	if(mat1.cols!=mat2.cols ||  mat1.rows!= mat2.rows)
//	{
//		cout << "ERROR: Images must be of same size, but" << endl;
//		cout << "       I1: " << mat1.cols <<  " x " <<  mat1.rows << 
//						", I2: " << mat2.cols <<  " x " <<  mat2.rows << endl;
//		int with = mat1.cols < mat2.cols ? mat1.cols : mat2.cols;
//		int hight =  mat1.rows <  mat2.rows ? mat1.rows : mat2.rows;
//		exmat1 = mat1(cv::Rect(0,0,with,hight));
//		exmat2 = mat2(cv::Rect(0,0,with,hight));
//		cout << "校正成了同大小图像"<<endl;
//	}
//	else
//	{
//		exmat1 = mat1;
//		exmat2 = mat2;
//	}
//	image<uchar> *I1,*I2;
//	I1 = loadMat(exmat1);
//	I2 = loadMat(exmat2);
//
//	// get image width and height
//	int32_t width  = I1->width();
//	int32_t height = I1->height();
//
//	// allocate memory for disparity images
//	const int32_t dims[3] = {width,height,width}; // bytes per line = width
//	float* D1_data = (float*)malloc(width*height*sizeof(float));
//	float* D2_data = (float*)malloc(width*height*sizeof(float));
//
//
//	// process
//	Elas::parameters param;
//	param.postprocess_only_left = false;
//	Elas elas(param);
//	elas.process(I1->data,I2->data,D1_data,D2_data,dims);
//
//	// find maximum disparity for scaling output disparity images to [0..255]
//	float disp_max = 0;
//	for (int32_t i=0; i<width*height; i++) {
//	if (D1_data[i]>disp_max) disp_max = D1_data[i];
//	if (D2_data[i]>disp_max) disp_max = D2_data[i];
//	}
//
//	// copy float to uchar
//	image<uchar> *D1 = new image<uchar>(width,height);
//	image<uchar> *D2 = new image<uchar>(width,height);
//	for (int32_t i=0; i<width*height; i++) {
//	D1->data[i] = (uint8_t)max(255.0*D1_data[i]/disp_max,0.0);
//	D2->data[i] = (uint8_t)max(255.0*D2_data[i]/disp_max,0.0);
//	}
//
//	savePGM(D1,"leftz_dis.pgm");
//	savePGM(D2,"right_dis.pgm");
//
//	Mat disp_l = convertImageToMat(*D1);
//	Mat disp_r = convertImageToMat(*D2);
//	// free memory
//	delete I1;
//	delete I2;
//	delete D1;
//	delete D2;
//	free(D1_data);
//	free(D2_data);
//
//}
//void my_cvRodrigues2(const cv::Mat &src, cv::Mat  &dst)
//{
//	CvMat m_src = src;
//
//    CvMat m_dst =dst;
//	cvRodrigues2(&m_src, &m_dst,0);
//	cv::Mat temp(&m_dst,1);
//	dst = temp;
//}


//int main (int argc, char** argv) {
//	MyStereoMatch m_match;
//	Mat pFrameMat_left_rec ;
//	Mat pFrameMat_right_rec ;
//	if(m_match.init(640,512,"aa.yml")!=1)
//	{
//		MyStereoMatch::RECTIFYMETHOD rectifyway = MyStereoMatch::RECTIFY_BOUGUET ;
//		MyStereoMatch::StereoParams temp_StereoParams;
//		MyStereoMatch::RemapMatrixs temp_RemapMatrixs;
//		setdata(temp_StereoParams);
//		m_match.rectifyStereoCamera(temp_StereoParams,temp_RemapMatrixs, rectifyway);
//		m_match.saveCalibrationDatas("aa.yml",rectifyway, temp_StereoParams,temp_RemapMatrixs);
//	}
//	if(m_match.init(640,512,"aa.yml")!=1)
//	{
//		printf("Failed to open CalibData");
//	}
//	Mat img1 = imread("left_000002.bmp", 1);
//    Mat img2 = imread("right_000002.bmp", 1);
//	//下面是libelas中的方法，对标准图还行，对自己校正的图完全没用
//	{
//		//imshow("left01",img1);
//		//imshow("right01",img2);
//		m_match.rectify_Image(img1,img2,pFrameMat_left_rec, pFrameMat_right_rec);
//		
//		Mat meges;
//		mergeoft(pFrameMat_left_rec,pFrameMat_right_rec,meges);
//
//		imshow("leftrec",pFrameMat_left_rec);
//		imshow("rightrec",pFrameMat_right_rec);
//		imshow("mege",meges);
//		function(pFrameMat_left_rec, pFrameMat_right_rec);
//	}
//	
////	function(img1, img2);
//
//	waitKey();
//
//  return 0;
//}


