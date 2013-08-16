

#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include "..\daheng_inc\HVDAILT.h"
#include "..\daheng_inc\Raw2Rgb.h"
#include "cv.h"
#include"opencv.hpp"

#define IS_USB_HV130(type)	       ((type) == (HV1300UCTYPE) || (type) == (HV1300UMTYPE) ||\
									(type) == (HV1301UCTYPE) || (type) == (HV1302UMTYPE) ||\
									(type) == (HV1302UCTYPE) || (type) == (HV1303UMTYPE) ||\
									(type) == (HV1303UCTYPE) || (type) == (HV1350UMTYPE) ||\
									(type) == (HV1350UCTYPE) || (type) == (HV1351UMTYPE) ||\
									(type) == (HV1351UCTYPE))

#define IS_USB_HV200(type)		   ((type) == (HV2000UCTYPE) || (type) == (HV2001UCTYPE) ||\
									(type) == (HV2002UCTYPE) || (type) == (HV2003UCTYPE) ||\
									(type) == (HV2050UCTYPE) || (type) == (HV2051UCTYPE))

#define IS_USB_HV300(type)		   ((type) == (HV3000UCTYPE) || (type) == (HV3102UCTYPE) ||\
									(type) == (HV3103UCTYPE) || (type) == (HV3150UCTYPE) ||\
									(type) == (HV3151UCTYPE))

#define IS_USB_GV400(type)		   ((type) == (GV400UCTYPE) || (type) == (GV400UMTYPE))

#define IS_USB_HV5051(type)        ((type) == (HV5051UCTYPE) || (type) == (HV5051UMTYPE))


#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

class HV_Camera
{
public:
	HV_Camera();  

	IplImage* HV_QueryFrame();
	virtual~HV_Camera();
	HVSTATUS  HVregister(HV_SNAP_PROC pSnapFunc,void *pParam);
	int getcimage(ATL::CImage &CI);
	cv::Mat getmat();

	static int CameraCount(); 
	static int ObjNum ;

private:
	void InitCamera();

	HV_BAYER_LAYOUT  HVGetBayerType() ;

	HVSTATUS         SetExposureTime(int nWindWidth,
									 long lTintUpper,
									 long lTintLower, 
									 long HBlanking,
									 HV_SNAP_SPEED SnapSpeed, 
									 HV_RESOLUTION Resolution);

	IplImage *  Byte2IplImg( byte *pImg,int width,int height,int bitCount=8,int nchannel=3,int original=1 );
	
	




private:
	 int DeviceId         ;   
	
	 HV_RESOLUTION Resolution     ;    
	 HV_SNAP_MODE SnapMode        ;    
	 HV_BAYER_CONVERT_TYPE ConvertType;

	 long Gain					 ;
	 long ExposureTint_Upper ;
	 long ExposureTint_Lower;
	 long ShutterDelay ;     
	 long ADCLevel  ;        
	 int XStart    ;         
	 int YStart ;             
	 int Width   ;           
	 int Height  ;           
	 long lVBlanking  ;      
	 HV_SNAP_SPEED SnapSpeed ;

	HHV	m_hhv;			///< ������������	
	long m_lHBlanking;	///< ˮƽ����
	long m_lVBlanking;	///< ��ֱ����

	HV_BAYER_LAYOUT m_Layout;	///< bayer��ʽ	
	BITMAPINFO *m_pBmpInfo;		///< BITMAPINFO �ṹָ�룬��ʾͼ��ʱʹ��
	BYTE *m_pRawBuffer;			///< �ɼ�ͼ��ԭʼ���ݻ�����
	BYTE *m_pImageBuffer;		///< Bayerת���󻺳���
	char m_chBmpBuf[2048];		///< BIMTAPINFO �洢��������m_pBmpInfo��ָ��˻�����
	
	
	BYTE m_pLutR[256] ;		///< ��ɫ���ұ�,R����
	BYTE m_pLutG[256] ;		///< ��ɫ���ұ�,G����
	BYTE m_pLutB[256] ;		///< ��ɫ���ұ�,B����

};




#endif 