//-----------------------------------------------------------------------------------
/**
 @file  Raw2Rgb.h
 @brief  		
 
 Copyright (c) 2009, China Daheng Group,Inc. Beijing Image Vision Technology Branch
 Development Kit
 Microsoft Win32 SDK, Visual C++ 6.00 
 
 @Author Xu Yibo, HuJing, Liumin, LuPeng,zhao zuxuan 
 @Date   2011-12-23
 @version V1.7.7.12
*/
//-----------------------------------------------------------------------------------

#ifndef __RAW_TO_RGB
#define __RAW_TO_RGB


/** By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#else

typedef char bool;

#endif

#include "HVDef.h"

///����bayerת���ͻ�ȡ��ƽ��ϵ��ת����
typedef enum tagHV_CONVERT_CODE { 
	CONV_RAW2RGB = 0,
	CONV_GET_WHITEBALANCE_VALUE = 1
} HV_CONVERT_CODE;

///���屴��ת������
typedef enum tagHV_BAYER_CONVERT_TYPE
{
	BAYER2RGB_ZHC,  
	BAYER2RGB_NEIGHBOUR,  
	BAYER2RGB_BILINER,    
	BAYER2RGB_PATTERN,       
	BAYER2RGB_EDGESENSING,  
	BAYER2RGB_RF,          
	BAYER2RGB_PIXELGROUPING, 
	BAYER2RGB_NEIGHBOUR1,
	BAYER2RGB_ADAPTIVE,	 
	BAYER2RGB_RESERVERED2,	 
	BAYER2RGB_RESERVERED3	
		
} HV_BAYER_CONVERT_TYPE;

///����bayer2rgb�����ṹ��
typedef struct tagHV_CONVERT_RAW2RGB {	
	BYTE   *pDestData;
	HV_COLOR_CODE   ccDestColorCode;
	DWORD  dwDestBitCount;
	DWORD  dwDestDataDepth;
	BYTE   *pSrceData;
	HV_COLOR_CODE   ccSrceColorCode;
	DWORD  dwSrceBitCount;
	DWORD  dwSrceDataDepth;
	HV_COLOR_FILTER cfFilter;
	int    nWidth;
	int    nHeight;
	HV_BAYER_CONVERT_TYPE cvType;
	BYTE   *pLutR;
	BYTE   *pLutG;
	BYTE   *pLutB;
	bool   bIsFlip;
	
}HV_CONVERT_RAW2RGB, *PHV_CONVERT_RAW2RGB;

///�����ȡ��ƽ������ṹ��
typedef struct tagHV_GET_WHITEBALANCE_VALUE {	
	BYTE    *pData;
	HV_COLOR_CODE ccColorCode;
	DWORD   dwBitCount;
	DWORD   dwDataDepth;
	int     nWidth;
	int     nHeight;
	double  *dValueR;
	double  *dValueG;
	double  *dValueB;
	
}HV_GET_WHITEBALANCE_VALUE, *PHV_GET_WHITEBALANCE_VALUE;


///����bayer����
typedef enum tagHV_BAYER_LAYOUT
{
	BAYER_GB = 1,    ///< start from GB
	BAYER_GR = 2,    ///< start from GR
	BAYER_BG = 3,    ///< start from BG
	BAYER_RG = 4     ///< start from RG
} HV_BAYER_LAYOUT;


///�����������
typedef enum tagHV_ILLUMINATE
{
	DAYLIGHT,
	FLUORESCENT,
	INCANDESCENT,
}HV_ILLUMINANT;

//--------------------------------------------------
/**
\brief  Convert Raw12Packed to Raw16
\param  pInBuf   [in]The input Buffer.      
\param  pOutBuf  [out]The output Buffer(new buffer).  
\param  iWidth   [in]The width of image.    
\param  iHeight  [in]The height of image.
return   void  
*/
//--------------------------------------------------
void __stdcall Raw12PackedToRaw16(BYTE* pInBuf, BYTE* pOutBuf,int iWidth, int iHeight);


//-------------------------------------------------
/**
\brief  Convert Raw10Packed to Raw16
\param  pInBuf   [in]The input Buffer.      
\param  pOutBuf  [out]The output Buffer(new buffer).  
\param  iWidth   [in]The width of image.    
\param  iHeight  [in]The height of image.
return  void
*/
//-------------------------------------------------
void __stdcall Raw10PackedToRaw16(BYTE* pInBuf, BYTE* pOutBuf,int iWidth, int iHeight);


//-------------------------------------------------
/**
\brief   Color Correction for AOI(24λRGBͼ��)
\param   pImg		  [in,out]RGB Image Buffer
\param   roi_left	  [in]Left of ROI in Pixel
\param   roi_top	  [in]Top of ROI 
\param   roi_wid	  [in]Width of ROI
\param   roi_hei	  [in]Height of ROI
\param   nImgWid	  [in]Width of Image
\param   nImgHei	  [in]Height of Image
return   void 
*/
//-------------------------------------------------
void __stdcall ROIColorCorrection( void *pImg, int roi_left, int roi_top, int roi_wid, int roi_hei, int nImgWid, int nImgHei);

//------------------------------------------------
/**
\brief  8λͼ��˳ʱ����ת90��
\param  pInputBuffer  [in]����ͼ�񻺳���
\param  Width         [in]ͼ����
\param  nHeight       [in]ͼ��߶�
\param  pOutputBuffer [out]���ͼ�񻺳�����new buffer��
return  ״̬����
*/
//------------------------------------------------
HVSTATUS __stdcall HVRotate90CW8B(const BYTE* pInputBuffer, int nWidth, int nHeight, BYTE* pOutputBuffer);

//------------------------------------------------
/**
\brief  8λͼ����ʱ����ת90��
\param  pInputBuffer    [in]����ͼ�񻺳���
\param  nWidth          [in]ͼ����
\param  nHeight         [in]ͼ��߶�
\param  pOutputBuffer   [out]���ͼ�񻺳�����new buffer��
return  ״̬����
*/
//------------------------------------------------
HVSTATUS __stdcall HVRotate90CCW8B(const BYTE* pInputBuffer, int nWidth, int nHeight, BYTE* pOutputBuffer);

//-----------------------------------------------
/**
\brief  ͼ�����ȵ���(24λRGBͼ�����8λ�Ҷ�ͼ��)
\param  pInputBuffer  [in]����ͼ�񻺳���      
\param  nImagesize    [in]ԭʼͼ��Ĵ�С�����ֽ�Ϊ��λ(RGB:ͼ���xͼ���x3)
\param  factor        [in]�������ӣ���Χ��-150~150
\param  pOutputBuffer [out]���ͼ�񻺳���
return  ״̬����
*/
//-----------------------------------------------
HVSTATUS __stdcall HVBrightness(BYTE* pInputBuffer,int nImagesize, int nFactor,BYTE* pOutputBuffer);

//--------------------------------------------------
/**
\brief  ͼ��Աȶȵ���(24λRGBͼ�����8λ�Ҷ�ͼ��)
\param  pInputBuffer	[in]����ͼ�񻺳���      
\param  nImagesize		[in]ԭʼͼ��Ĵ�С�����ֽ�Ϊ��λ(RGB:ͼ���xͼ���x3)
\param  factor			[in]�������ӣ���Χ��-50~100
\param  pOutputBuffer	[out]���ͼ�񻺳���
return  ״̬����
*/
//--------------------------------------------------
HVSTATUS __stdcall HVContrast(BYTE* pInputBuffer,int nImagesize, int nFactor,BYTE* pOutputBuffer);

//--------------------------------------------------
/**
\brief  24λRGBͼ����
\param  pInputBuffer  [in]����ͼ�񻺳���      
\param  nWidth        [in]ͼ����
\param  nHeight       [in]ͼ��߶�
\param  factor        [in]�������ӣ���Χ��0.1~5.0
\param  pOutputBuffer [out]���ͼ�񻺳���
return  ״̬����
*/
//--------------------------------------------------
HVSTATUS __stdcall HVSharpen24B(BYTE* pInputBuffer,int nWidth,int nHeight,float factor,BYTE* pOutputBuffer);

//--------------------------------------------------
/**
\brief  24λRGBͼ�񱥺Ͷȵ��ں���
\param  pOutputBuffer  [out]�������bufferָ��
\param  pInputBuffer   [in]��������bufferָ��
\param  nImageSize     [in]RGBͼ��ߴ� (ͼ���xͼ���)
\param  nFactor        [in]���ڲ���,��Χ��0 ~ 128
return  ״̬����   
*/
//--------------------------------------------------
HVSTATUS __stdcall HVSaturation(BYTE* pInputBuffer,int nImagesize, int nFactor,BYTE* pOutputBuffer);

//--------------------------------------------------
/**
\brief  bayerͼ��ת����RGBͼ����
\param  pDest   [out]�������bufferָ�루24λRGBͼ��
\param  pSrce   [in]��������bufferָ�루8λRawͼ��
\param  nWid    [in]ͼ���
\param  nHei    [in]ͼ���
\param  cvtype  [in]��ֵ�㷨����
\param  pLutR   [in]R��������������ұ�
\param  pLutG   [in]G��������������ұ�
\param  pLutB   [in]B��������������ұ�
\param  bFlip   [in]ͼ��ת��� true:��ת false:����ת
\param  Layout  [in]bayer����
return  void  
*/
//--------------------------------------------------
void __stdcall ConvertBayer2Rgb(BYTE *pDest,BYTE *pSrce, int nWid,int nHei,HV_BAYER_CONVERT_TYPE cvtype,BYTE* pLutR,BYTE* pLutG, BYTE* pLutB,bool bFlip,HV_BAYER_LAYOUT Layout);


//--------------------------------------------------
/**
\brief  ��ȡ��ƽ��ϵ��������Ϊ�˼���׼ȷ������տ͹ۡ��ס����򣬻��ߴ���ͼ��Ϊ����ɫ����
\param  pSrce   [in]��������bufferָ�루24bitRGBͼ��
\param  nWid    [in]ͼ���
\param  nHei    [in]ͼ���
\param  dRatioR [out]R������ƽ��ϵ��
\param  dRatioG [out]G������ƽ��ϵ��
\param  dRatioB [out]B������ƽ��ϵ��
return  void  
*/
//--------------------------------------------------
void __stdcall GetWhiteBalanceRatio(BYTE *pSrce,int nWid,int nHei,double* dRatioR,double* dRatioG,double* dRatioB);

//--------------------------------------------------
/**
\brief ����Gamma���ұ���
\param  pLutGamma[256]   [out]�������8λͼ��Gamma���ұ�
\param  dGammaRatio      [in]Gammaϵ��   (0.1 ~ 10.0)
return  void  
*/
//--------------------------------------------------
void __stdcall SetGammaLut(BYTE pLutGamma[256],double dGammaRatio);


//-----------------------------------------------------
/**
\brief  8λRawͼ�񻵵��⺯�������ʱ������Ͼ�ͷ��
\param  pRawImgBuf         [in]8λRawͼ����bufferָ��
\param  pBadPixelPosBuf    [out]��Ż���λ�õ�bufferָ��
\param  nImgWid            [in]ͼ���
\param  nImgHei            [in]ͼ���
return  void  
*/
//-----------------------------------------------------
void __stdcall DetectDeadPixel(BYTE * pRawImgBuf,BYTE * pBadPixelPosBuf,int nImgWid,int nImgHei);


//-----------------------------------------------------
/**
\brief  8λRawͼ�񻵵�У������
\param  pRawImgBuf         [in,out]8λRawͼ����bufferָ��
\param  pBadPixelPosBuf    [in]��Ż���λ�õ�bufferָ��(DetectDeadPixel���������)
\param  nXPos              [in]ͼ����ʼλ��X����(��Ի���ģ��)
\param  nYpos              [in]ͼ����ʼλ��Y����(��Ի���ģ��)
\param  nImgWid            [in]ͼ���
\param  nImgHei            [in]ͼ���
\param  nBadPixelBufWid    [in]���滵��λ����Ϣ�Ļ������Ŀ�
\param  nBadPixelBufHei    [in]���滵��λ����Ϣ�Ļ������ĸ�
return  void  
*/
//-----------------------------------------------------
void __stdcall EraseDeadPixel(BYTE * pRawImgBuf,BYTE * pBadPixelPosBuf,int nXPos,int nYPos,int nImgWid,int nImgHei,int nBadPixelBufWid,int nBadPixelBufHei);


//-----------------------------------------------------
/**
\brief  Rawͼ�񻵵��⺯��,�÷���ֻ�ܼ���ͼ�����Ͼ�ͷ����֧��8 ~ 16 λ����,���λ��ָʵ��λ����
        ������8λʱ������ռ�����ֽڣ���ʵ����Чλ�������� 9 ~ 16��������������Packed��ʽ������ת����raw16
\param  pRawImgBuf         [in]Rawͼ����bufferָ��
\param  pBadPixelPosBuf    [out]��Ż���λ�õ�bufferָ��,buffer��С��ͼ���Сͬ
\param  nImgWid            [in]ͼ���
\param  nImgHei            [in]ͼ���
\param  nBitNum            [in]ͼ�����ݵ�ʵ��λ����ע������10λ����������10��12λ��������12���Դ����ƣ�ֵ��Χ8 ~ 16�����ο�˵����
return  ״̬�� 
*/
//-----------------------------------------------------
HVSTATUS __stdcall HVDetectRawDeadPos(BYTE *pRawImgBuf,BYTE *pBadPixelPosBuf,int nImgWid,int nImgHei,int nBitNum);


//-----------------------------------------------------
/**
\brief  Rawͼ�񻵵�У������,֧��8 ~ 16 λ����,���λ��ָʵ��λ����������8λʱ��
        ����ռ�����ֽڣ���ʵ����Чλ�������� 9 ~ 16��������,����Packed��ʽ������ת����raw16
\param  pRawImgBuf         [in,out]Rawͼ����bufferָ��
\param  pBadPixelPosBuf    [in]��Ż���λ�õ�bufferָ��(HVDetectRawDeadPos���������)
\param  nImgWid            [in]ͼ���
\param  nImgHei            [in]ͼ���
\param  nBitNum            [in]ͼ�����ݵ�ʵ��λ����ע������10λ����������10��12λ��������12���Դ����ƣ�ֵ��Χ8 ~ 16�����ο�˵����
return  ״̬��  
*/
//-----------------------------------------------------
HVSTATUS __stdcall HVCorrectRawBadPos(BYTE *pRawImgBuf,BYTE *pBadPixelPosBuf,int nImgWid,int nImgHei,int nBitNum);

//-----------------------------------------------------
/**
\brief  Rawͼ���Զ�������У������,֧��8 ~ 16 λ���ݣ����λ��ָʵ��λ����������8λʱ������ռ�����ֽڣ�
        ��ʵ����Чλ�������� 9 ~ 16�����������ú���������ʱ���У����ÿ֡�������,����Packed��ʽ������ת����raw16
\param  pRawImgBuf         [in,out]Rawͼ����bufferָ��
\param  nImgWid            [in]ͼ���
\param  nImgHei            [in]ͼ���
\param  nBitNum            [in]ͼ�����ݵ�ʵ��λ����ע������10λ����������10��12λ��������12���Դ����ƣ�ֵ��Χ8 ~ 16�����ο�˵����
return  ״̬��  
*/
//-----------------------------------------------------
HVSTATUS __stdcall HVAutoRawDefectivePixelCorrect(BYTE *pRawImgBuf,int nImgWid,int nImgHei,int nBitNum);

//-----------------------------------------------------
/**
\brief  ��������ͼ���ȡ����ģ��(����8λRawͼ)
\param  pRawImgBuf     [in]8λRawͼ����bufferָ��
\param  pPatternBuf    [out]����ģ����Ϣ�Ļ�����
\param  nPatternWid    [in]ͼ���
\param  nPatternHei    [in]ͼ���
return  void  
*/
//-----------------------------------------------------
void __stdcall GetPatternNoise(BYTE * pRawImgBuf,int * pPatternBuf,int nPatternWid,int nPatternHei);

//-----------------------------------------------------
/**
\brief  �Զ��ȡ��ģ��ȡƽ��
\param  pPatternBuf    [in]����ģ����Ϣ�Ļ�����
\param  nPatternWid    [in]ͼ���(ģ���)
\param  nPatternHei    [in]ͼ���(ģ���)
\param  nAvgTimes      [in]ƽ������
return  void  
*/
//-----------------------------------------------------
void __stdcall AvgPatternNoise(int * pPatternBuf,int nPatternWid,int nPatternHei,int nAvgTimes);

//-----------------------------------------------------
/**
\brief  ��ȡ�õ�ģ���ͼ�����У��
\param  pRawImgBuf     [in,out]Ҫ���л������������8λRawͼ������
\param  pPatternBuf    [in]����ģ����Ϣ�Ļ�����
\param  nXPos          [in]ͼ����ʼλ��x����(���ģ��)
\param  nYPos          [in]ͼ����ʼλ��y����(���ģ��)
\param  nImgWid        [in]ͼ���
\param  nImgHei        [in]ͼ���
\param  nPatternWid    [in]ģ�建�����Ŀ�
\param  nPatternHei    [in]ģ�建�����ĸ�
return  void  
*/
//-----------------------------------------------------
void __stdcall FixPatternNoise(BYTE * pRawImgBuf,int * pPatternBuf,int nXPos,int nYPos,int nImgWid,int nImgHei,int nPatternWid,int nPatternHei);

//-----------------------------------------------------
/**
\brief  ���û�������ֵ
\param  [in]BadPixelThreshold   ��������ֵ   
return  void  
*/
//-----------------------------------------------------
void __stdcall SetBadPixelThreshold(int BadPixelThreshold);

//-----------------------------------------------------
/**
\brief  ͼ������ת������
\param  ConvertCode   [in]���ܴ��룬�μ�HV_CONVERT_CODE
\param  pContext      [in,out]ָ��ConvertCode�Ĳ���������
\param  pLength       [in]pContextָ��Ļ������Ĵ�С����λ���ֽ�
return  void  
*/
//-----------------------------------------------------
void __stdcall HVConvert(HV_CONVERT_CODE ConvertCode, void *pContext, int *pLength);


#ifdef __cplusplus
}
#endif

#endif


