//----------------------------------------------------------------------------------
/**
	\file		HVExtend.h
	\brief		the declaration of the Main interface for the HVExtend.dll module.
	\version	V1.0.0.2
	\date		2011-08-09
*/
//----------------------------------------------------------------------------------

#if !defined(H_HVEXTEND_H__49975EF8_B95B_4893_A34E_AA859F4DF307__INCLUDED_)
#define H_HVEXTEND_H__49975EF8_B95B_4893_A34E_AA859F4DF307__INCLUDED_

#include "HVDef.h"
#include "Raw2Rgb.h"

#ifdef __cplusplus
extern "C" {
#endif

///�����ƽ�����ϵ��
typedef struct tagHV_WB_RATIO
{
	double dRatioR;     ///<��ƽ����������ϵ��
	double dRatioB;     ///<��ƽ������������ϵ��
}HV_WB_RATIO;

// ��ʼ��HVExtend �ӿ���Ҫ����Դ
HVSTATUS __stdcall HVInitialize(HHV hhv);

// ���HVExtend�ӿ���Ҫ����Դ
HVSTATUS __stdcall HVUnInitialize(HHV hhv);

// ��ʼ��ƵԤ��
HVSTATUS __stdcall HVStartPreview(HHV hhv,
								  int nImgWid,
								  int nImgHei,
								  HV_ADC_LEVEL nLevel,
								  LPCSTR lpszTitle,
								  DWORD style,
								  int x,
								  int y,
								  int nWid,
								  int nHei,
								  HWND hwndParent,
								  HMENU hMenu);

// ��ͣ��ƵԤ��
HVSTATUS __stdcall HVPausePreview(HHV hhv, BOOL bPause);

// ֹͣ��ƵԤ��
HVSTATUS __stdcall HVStopPreview(HHV hhv);

// Bayerͼ��ת��ΪRGBͼ��
HVSTATUS __stdcall HVBayerToRGB(HHV  hhv,
								BYTE *pSrcBuf,
								int  nWidth,
								int  nHeight,
								bool bIsFlip,
								HV_ADC_LEVEL nLevel,
								HV_BAYER_CONVERT_TYPE cvtype,
								BYTE *pDestBuf,
								HV_WB_RATIO *pAWBRatio);



#ifdef __cplusplus
}
#endif

#endif // !defined(H_HVEXTEND_H__49975EF8_B95B_4893_A34E_AA859F4DF307__INCLUDED_)

