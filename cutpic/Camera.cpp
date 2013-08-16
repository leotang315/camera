


#include "stdafx.h"
#include "Camera.h"

#pragma comment(lib,"..\\daheng_lib\\HVDAILT.lib") 
#pragma comment(lib,"..\\daheng_lib\\Raw2Rgb.lib") 




//�����Լ���0
#define  MY_ZERO 0.000000001

int HV_Camera::ObjNum = 0;

HV_Camera::HV_Camera()
{	 
	 DeviceId          = 1;
	 Resolution          = RES_MODE1;
	 SnapMode            = CONTINUATION;
	 ConvertType = BAYER2RGB_NEIGHBOUR1;

	 Gain               = 10;
	 ExposureTint_Upper = 60;
	 ExposureTint_Lower = 1000;
	 ShutterDelay       = 0;
	 ADCLevel           = ADC_LEVEL2;
	 XStart              = 0;
	 YStart              = 0;
	 Width               = 640;
	 Height              = 512;
	 lVBlanking         = 0;
	 SnapSpeed = HIGH_SPEED;
	 ObjNum++;

	for(int i=0;i<256;i++)
	{
		m_pLutR[i] = i;
		m_pLutG[i] = i;
		m_pLutB[i] = i;
	}

	// ����Ĭ��ֵ
	m_lHBlanking = 0;
	m_lVBlanking = 0;


	//ֻ����1��2���������� 0�������������� �����������������һ�����
	if(CameraCount()==2)
	{
		if( ObjNum==1)
		{
			 DeviceId =1;
		}
		else if(ObjNum==2)
		{
			DeviceId =2;
		}
		
	}
	


	InitCamera();

}
HV_Camera::~HV_Camera()
{
	HVSTATUS status =STATUS_OK;
	
	//	ֹͣ�ɼ�ͼ���ڴ棬�����ٴε���HVStartSnapEx��������������ɼ�
	status = HVStopSnap(m_hhv);
	HV_VERIFY(status);
	
	 status = STATUS_OK;

	/*
	 *	��ֹ����������ɼ�ͼ���ڴ棬ͬʱ�ͷ����вɼ�������
	 *	�ٴ���������������ɼ����������³�ʼ��	
	 */
	status = HVCloseSnap(m_hhv);
	HV_VERIFY(status);

	delete m_pRawBuffer;
	delete m_pImageBuffer;

};

void HV_Camera::InitCamera()
{
	
	HVSTATUS status = BeginHVDevice(DeviceId, &m_hhv);//	����������� 1
	
	HV_VERIFY(status);                         //	���麯��ִ��״̬�����ʧ�ܣ��򷵻ش���״̬��Ϣ��

	m_Layout = HVGetBayerType();
	
	/*************************************************************
		��ʼ�����������Ӳ��״̬���û�Ҳ����������λ�ó�ʼ�������������
		��Ӧ��֤����������Ѿ��򿪣������û���Ӧ�ó����ʼ��ʱ��
		ͬʱ��ʼ�����������Ӳ����
	*************************************************************/

	HVSetResolution(m_hhv, Resolution);		  //��������������ֱ���
	HVSetSnapMode(m_hhv, SnapMode);           //�ɼ�ģʽ������ CONTINUATION(����)��TRIGGER(�ⴥ��)		
		
	for (int i = 0; i < 4; i++){              //���ø�������������
		HVAGCControl(m_hhv, RED_CHANNEL + i, Gain);
	}
			
	HVADCControl(m_hhv, ADC_BITS, ADCLevel);  //����ADC�ļ���
	
	HVTYPE type = UNKNOWN_TYPE;               //��ȡ�豸����
	int size    = sizeof(HVTYPE);
	HVGetDeviceInfo(m_hhv,DESC_DEVICE_TYPE, &type, &size);	
	
	//  ����ͼ�񴰿ں�����ǰ�����Ȳ鿴�Ƿ���������������������ƣ�
/***************************************************************************/
	if (IS_USB_GV400(type))
	{
		int rate = 0;
		switch(Resolution)
		{
			case RES_MODE0:
				rate = 1;
				break;
			case RES_MODE1:
				rate = 2;
				break;
			default:
				break;
		}
		
		//GV400Ux���Ҫ��ͼ���Ⱥ�ˮƽ�����������¹�ʽ��
		//lHBlanking + Width*rate > 0x236
		//���������������������Ҫ�û���������������ͼ���Ȼ�����ˮƽ����
		
		if (m_lHBlanking + Width*rate <= 0x236)
		{
			//ȡ�������ı߽�ֵ
			DWORD pBlankSize[4];
			int nBufSize = 0;
			int nHmin = 0;
			int nHmax = 0;
			HVGetDeviceInfo(m_hhv,DESC_DEVICE_BLANKSIZE,NULL,&nBufSize);	
			HVGetDeviceInfo(m_hhv,DESC_DEVICE_BLANKSIZE,pBlankSize,&nBufSize);	
			nHmin = (int)pBlankSize[0];
			nHmax = (int)pBlankSize[2];
			int tHor = 0x236-Width*rate+1;
			
			//��ȡ��ͼ�����������ˮƽ������Сֵ
			m_lHBlanking = max(nHmin,min(tHor, nHmax));
		}

		//Ϊ��ֹ�������ò��ɹ��Ƚ�ͼ�񴰿���Ϊ���
		int nBuffSize = 0;
		HVGetDeviceInfo(m_hhv, DESC_RESOLUTION, NULL,&nBuffSize);	
		BYTE *pbContext = new BYTE[nBuffSize];
		DWORD *pdContext = (DWORD *)(pbContext);
		HVGetDeviceInfo(m_hhv, DESC_RESOLUTION, pdContext,&nBuffSize);	
		int	nMaxWid = *(pdContext + 2*Resolution);
		int nMaxHei = *(pdContext + 2*Resolution +1);		
		delete []pbContext;
		HVSetOutputWindow(m_hhv, 0, 0, nMaxWid, nMaxHei);
		
		//����������
		HVSetBlanking(m_hhv, m_lHBlanking, m_lVBlanking);
		}
		else
		{
			//����������
			HVSetBlanking(m_hhv, m_lHBlanking, m_lVBlanking);
		}
/***************************************************************************/

	/************************************************************************
		��Ƶ������ڣ�����Ƶ�����Χ���������ȡֵ��Χ���������봰�ڷ�Χ���ڣ�
	    ��Ƶ�������Ͻ�X����ʹ��ڿ��ӦΪ4�ı��������Ͻ�Y����ʹ��ڸ߶�ӦΪ2�ı���
		������ڵ���ʼλ��һ������Ϊ(0, 0)���ɡ�
	*************************************************************************/
	HVSetOutputWindow(m_hhv, XStart, YStart, Width, Height);
	HVSetSnapSpeed(m_hhv, SnapSpeed);							   //���òɼ��ٶ�
	SetExposureTime(Width, ExposureTint_Upper, ExposureTint_Lower, //	�����ع�ʱ��
							m_lHBlanking, SnapSpeed, Resolution);

	
	m_pBmpInfo								= (BITMAPINFO *)m_chBmpBuf;//	m_pBmpInfo��ָ��m_chBmpBuf���������û������Լ�����BTIMAPINFO������	
	m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);//	��ʼ��BITMAPINFO �ṹ���˽ṹ�ڱ���bmp�ļ�����ʾ�ɼ�ͼ��ʱʹ��
	m_pBmpInfo->bmiHeader.biWidth			= Width;                   //	ͼ���ȣ�һ��Ϊ������ڿ��
	m_pBmpInfo->bmiHeader.biHeight			= Height;				   //	ͼ���ȣ�һ��Ϊ������ڸ߶�
	
	/***********************************************
		��������һ����ͬ��
		���ڵ���8λ��λͼ����Ӧ������Ӧ��λͼ��ɫ��
	************************************************/
	m_pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pBmpInfo->bmiHeader.biBitCount		= 24;
	m_pBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pBmpInfo->bmiHeader.biClrImportant	= 0;
	
	/***********************************************
		����ԭʼͼ�񻺳�����һ�������洢�ɼ�ͼ��ԭʼ����
	    һ��ͼ�񻺳�����С��������ڴ�С����Ƶ��ʽȷ����
	************************************************/
	 

	

	m_pRawBuffer = new BYTE[Width * Height];
	ASSERT(m_pRawBuffer);
	
	/***********************************************
		����Bayerת����ͼ�����ݻ���
	************************************************/
	m_pImageBuffer = new BYTE[Width * Height*3];
	ASSERT(m_pImageBuffer);

}

IplImage* HV_Camera::HV_QueryFrame()
{
	IplImage*  Pimage;
	HVSTATUS status = STATUS_OK;
	CString strMsg;
	

	//ֻ����һ��������������ͼ������
	BYTE *ppBuf[1];
	ppBuf[0] = m_pRawBuffer;
	
	status = HVSnapShot(m_hhv, ppBuf, 1);	
	if (STATUS_OK < status  || status < STATUS_INTERNAL_ERROR)
	{
		CString str="";
		str.Format("Error : 0x%08x",status);
		::MessageBox(NULL, str , "Error", MB_ICONWARNING | MB_OK);   
	}else{
			HV_VERIFY(status);
	}
		
	
	//	��ԭʼͼ�����ݽ���Bayerת����ת����Ϊ24λ��
	//ͬʱ��ԭʼ���ݽ������·�ת
	ConvertBayer2Rgb(m_pImageBuffer,m_pRawBuffer,Width,Height,ConvertType,
									m_pLutR,m_pLutG,m_pLutB,true,m_Layout);
	Pimage = Byte2IplImg(m_pImageBuffer, Width ,Height );

	return Pimage;


}


HVSTATUS HV_Camera::HVregister(HV_SNAP_PROC pSnapFunc,void *pParam)
{
	HVSTATUS status = STATUS_OK;

	/*
	 *	��ʼ������������ɼ�ͼ���ڴ�Ŀ��ƣ�
	 *	ָ���ص�����SnapThreadCallback��thisָ��
	 */
	status = HVOpenSnap(m_hhv, pSnapFunc,pParam);					
	HV_VERIFY(status);
	
	 status = STATUS_OK;

	/*
	 *	��������������ɼ�ͼ���ڴ�
	 */
	BYTE *ppBuf[1];
	ppBuf[0] = m_pRawBuffer;
	status = HVStartSnap(m_hhv, ppBuf,1);

	HV_VERIFY(status);
	return status;
}
 

/////////////////////////////////////////////////////////////////////////////

/****************************************************************************

˵��      ��byte*  ת�� IplImage* 

****************************************************************************/
IplImage *  HV_Camera::Byte2IplImg( byte *pImg,int width,int height,int bitCount,int nchannel,int original )
{
	if (!pImg )
	{
		return NULL;
	}
 //int nchannel = 1; //create IplImage in 3channel.
	IplImage *pIplImgHeader = NULL;
	if (bitCount == 8)
	{
		pIplImgHeader = cvCreateImageHeader(cvSize(width,height),IPL_DEPTH_8U,nchannel);
	}
	if (bitCount == 24)
	{
		pIplImgHeader = cvCreateImageHeader(cvSize(width,height),IPL_DEPTH_32F,nchannel);
	}
 
	long lWidthByte = (width * bitCount+31)/32 *4;
	pIplImgHeader->origin = original; //0- leftupΪ��㣬1- leftdown
	cvSetData(pIplImgHeader,pImg,lWidthByte*nchannel);

	return pIplImgHeader;
 
}
/****************************************************************************

˵��      ���ڴ�m_pImageBuffer ת�� Mat

****************************************************************************/
cv::Mat  HV_Camera::getmat()
{
	 
	IplImage*  Pimage;
	Pimage = Byte2IplImg(m_pImageBuffer, Width ,Height );
	cv::Mat mat(Pimage);
	return mat;
}
/****************************************************************************

˵��      ���ڴ�m_pImageBuffer ת�� CImage

****************************************************************************/
int HV_Camera::getcimage(ATL::CImage &CI)
{
	ConvertBayer2Rgb(m_pImageBuffer,m_pRawBuffer,Width,Height,ConvertType,
									m_pLutR,m_pLutG,m_pLutB,false,m_Layout);
	CI.Create( Width , -Height, 8*3);// If Height is negative, the bitmap is a top-down DIB and its origin is the upper left corner.
	
	uchar *pImg=(uchar *)CI.GetBits();//�õ�CImage��������ַ

	memcpy(pImg,m_pImageBuffer,Width*Height*3);
	//int step=CI.GetPitch();
	//for(int i=0;i<Height;i++)
	//{
	//	//pS=imgShow.ptr<uchar>(i);
	//	for(int j=0;j<Width;j++)
	//	{
	//		for(int k=0;k<3;k++)
	//			*(pImg+i*step+j*3+k)=m_pImageBuffer[i*(step)+j*3+k];
	//	}
	//}
	return 1;
}
/*********************************
	��ȡbayer��ʽ
**********************************/
HV_BAYER_LAYOUT HV_Camera::HVGetBayerType() 
{
    int nSize = 0; 
	HVSTATUS status = STATUS_OK;;
	HV_BAYER_LAYOUT Layout;

	status = HVGetDeviceInfo(m_hhv, DESC_DEVICE_BAYER_LAYOUT, NULL,&nSize);	          
    if(STATUS_OK != status)
	{
		Layout = BAYER_GR;
	}
	else
	{
		BYTE *pbBayerLayout = NULL;
		pbBayerLayout = new BYTE[nSize];
        status = HVGetDeviceInfo(m_hhv, DESC_DEVICE_BAYER_LAYOUT, pbBayerLayout,&nSize);
		Layout = (HV_BAYER_LAYOUT)*pbBayerLayout;
		delete []pbBayerLayout; 
		pbBayerLayout=NULL;
	}
	return Layout;
}



/**************************************************************************
����:
		SetExposureTime
�������:
		int nWindWidth			��ǰͼ����		
		int lTintUpper			�ع�ʱ��ķ��ӣ�shutter��ȡֵ
		int lTintLower			�ع�ʱ��ķ�ĸ����shutter�ĵ�λ���
								��ms:1000��s:1��
�������:
		��						
˵��:
		�����ع�ʱ�䣨�����Ĳ����������ʱ��Ƶ�ʣ�����ֵ��ȡĬ��ֵ��
***************************************************************************/
HVSTATUS HV_Camera::SetExposureTime(int nWindWidth,
									 long lTintUpper,
									 long lTintLower, 
									 long HBlanking,
									 HV_SNAP_SPEED SnapSpeed, 
									 HV_RESOLUTION Resolution)
{
	HVTYPE type = UNKNOWN_TYPE;
	int size    = sizeof(HVTYPE);
	HVGetDeviceInfo(m_hhv,DESC_DEVICE_TYPE, &type, &size);	

	int nOutputWid = nWindWidth;

	double dExposure = 0.0;
	double dTint = max((double)lTintUpper/(double)lTintLower,MY_ZERO);

	double lClockFreq = 0.0;  
	if (IS_USB_GV400(type))
	{
		long lTb = HBlanking;
		lTb += 0x5e;	
		lClockFreq = (SnapSpeed == HIGH_SPEED)? 26600000.0:13300000.0; 		
		long rate = 0;

		switch(Resolution)
		{
		case RES_MODE0:
			rate = 1;
			break;
		case RES_MODE1:
			rate = 2;
			break;

		default:
			return STATUS_PARAMETER_OUT_OF_BOUND;
		}
		
		nOutputWid = nOutputWid * rate;	//width*rate
		if ((dTint * lClockFreq) <= (nOutputWid+ lTb - 255))
		{
			dExposure = 1;
		}
		else
			dExposure = ((dTint * lClockFreq) - (nOutputWid + lTb - 255) ) / (nOutputWid + lTb);				

		if (dExposure < 3) 
			dExposure = 3;
		else if (dExposure > 32766)
			dExposure = 32766;

	}
	else if( IS_USB_HV300(type))
	{
		lClockFreq = (SnapSpeed == HIGH_SPEED)? 24000000:12000000;
		long lTb = HBlanking;
		lTb += 142;
		if(lTb<21)
			lTb = 21;
		long lP1 = 331, lP2 = 38;
		long lp4 = 316;
		if(Resolution == RES_MODE1)
		{
			lP1 = 673;
			lP2 = 22;
			lp4 = 316*2;
		}
		long lAQ = (long)(nOutputWid + lP1 + lP2 + lTb);
		long lTrow = max(lAQ,(lP1+lp4));
		dExposure = ((dTint* lClockFreq)+ lP1 - 132.0)/lTrow;	
		
		if((dExposure-(int)dExposure) >0.5)
			dExposure += 1.0;		
		if(dExposure <= 0)
			dExposure = 1;
		else if(dExposure > 1048575) 
			dExposure = 1048575;
	}
	else if(IS_USB_HV200(type))
	{
		lClockFreq = (SnapSpeed == HIGH_SPEED)? 24000000:12000000;
		long lTb = HBlanking;
		lTb += 53;
		if(lTb<19)
			lTb = 19;
		long lAQ = (long)(nOutputWid + 305.0 + lTb);
		long lTrow = max(617,lAQ);
		dExposure = (dTint* lClockFreq + 180.0)/lTrow + 1;
		
		if((dExposure-(int)dExposure) >0.5)//��������
			dExposure += 1.0;
		if(dExposure <= 0)
			dExposure = 1;
		else if(dExposure > 16383) 
			dExposure = 16383;
	}
	else if(IS_USB_HV5051(type))
	{
		SHUTTER_UNIT_VALUE nUnit = SHUTTER_MS;
		HVSTATUS status          = STATUS_OK;

		if (lTintLower == 1000000)
		{
			nUnit = SHUTTER_US;
		}
		
		//�����ع�ʱ�䵥λ
		status = HVAECControl(m_hhv, AEC_SHUTTER_UNIT, nUnit);
		if (!HV_SUCCESS(status))
		{
			return status;
		}
		
		//�����ع�ʱ�䡣
		return HVAECControl(m_hhv, AEC_SHUTTER_SPEED, lTintUpper);			
	}
	else
	{		
		lClockFreq = (SnapSpeed == HIGH_SPEED)? 24000000:12000000;
		long lTb = HBlanking;
		lTb += 9;
		lTb -= 19;	
		if(lTb <= 0) lTb =0;
		if(((double)nOutputWid + 244.0 + lTb ) > 552)	
			dExposure = (dTint* lClockFreq + 180.0)/((double)nOutputWid + 244.0 + lTb);
		else
			dExposure = ((dTint * lClockFreq)+ 180.0) / 552 ;

		if((dExposure-(int)dExposure) >0.5)
			dExposure += 1.0;
		if(dExposure <= 0)
			dExposure = 1;
		else if(dExposure > 16383) 
			dExposure = 16383;
	}

	return HVAECControl(m_hhv, AEC_EXPOSURE_TIME, (long)dExposure);
}

int HV_Camera::CameraCount()
{
	int nDeviceCount = 0;   //�豸��������

	HVSTATUS status = HVGetDeviceTotal(&nDeviceCount);

	HV_VERIFY(status);

	return nDeviceCount;


}




