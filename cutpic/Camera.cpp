


#include "stdafx.h"
#include "Camera.h"

#pragma comment(lib,"..\\daheng_lib\\HVDAILT.lib") 
#pragma comment(lib,"..\\daheng_lib\\Raw2Rgb.lib") 




//定义自己的0
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

	// 消隐默认值
	m_lHBlanking = 0;
	m_lVBlanking = 0;


	//只考虑1个2个相机的情况 0个相机将输出错误 多余两个将都输出第一个相机
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
	
	//	停止采集图像到内存，可以再次调用HVStartSnapEx启动数字摄像机采集
	status = HVStopSnap(m_hhv);
	HV_VERIFY(status);
	
	 status = STATUS_OK;

	/*
	 *	终止数字摄像机采集图像到内存，同时释放所有采集环境，
	 *	再次启动数字摄像机采集，必须重新初始化	
	 */
	status = HVCloseSnap(m_hhv);
	HV_VERIFY(status);

	delete m_pRawBuffer;
	delete m_pImageBuffer;

};

void HV_Camera::InitCamera()
{
	
	HVSTATUS status = BeginHVDevice(DeviceId, &m_hhv);//	打开数字摄像机 1
	
	HV_VERIFY(status);                         //	检验函数执行状态，如果失败，则返回错误状态消息框

	m_Layout = HVGetBayerType();
	
	/*************************************************************
		初始化数字摄像机硬件状态，用户也可以在其他位置初始化数字摄像机，
		但应保证数字摄像机已经打开，建议用户在应用程序初始化时，
		同时初始化数字摄像机硬件。
	*************************************************************/

	HVSetResolution(m_hhv, Resolution);		  //设置数字摄像机分辨率
	HVSetSnapMode(m_hhv, SnapMode);           //采集模式，包括 CONTINUATION(连续)、TRIGGER(外触发)		
		
	for (int i = 0; i < 4; i++){              //设置各个分量的增益
		HVAGCControl(m_hhv, RED_CHANNEL + i, Gain);
	}
			
	HVADCControl(m_hhv, ADC_BITS, ADCLevel);  //设置ADC的级别
	
	HVTYPE type = UNKNOWN_TYPE;               //获取设备类型
	int size    = sizeof(HVTYPE);
	HVGetDeviceInfo(m_hhv,DESC_DEVICE_TYPE, &type, &size);	
	
	//  设置图像窗口和消隐前，首先查看是否满足特殊相机的条件限制？
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
		
		//GV400Ux相机要求图像宽度和水平消隐满足以下公式：
		//lHBlanking + Width*rate > 0x236
		//如果不满足上述条件，需要用户做出调整，增大图像宽度或增大水平消隐
		
		if (m_lHBlanking + Width*rate <= 0x236)
		{
			//取得消隐的边界值
			DWORD pBlankSize[4];
			int nBufSize = 0;
			int nHmin = 0;
			int nHmax = 0;
			HVGetDeviceInfo(m_hhv,DESC_DEVICE_BLANKSIZE,NULL,&nBufSize);	
			HVGetDeviceInfo(m_hhv,DESC_DEVICE_BLANKSIZE,pBlankSize,&nBufSize);	
			nHmin = (int)pBlankSize[0];
			nHmax = (int)pBlankSize[2];
			int tHor = 0x236-Width*rate+1;
			
			//获取此图像宽度下允许的水平消隐最小值
			m_lHBlanking = max(nHmin,min(tHor, nHmax));
		}

		//为防止消隐设置不成功先将图像窗口设为最大
		int nBuffSize = 0;
		HVGetDeviceInfo(m_hhv, DESC_RESOLUTION, NULL,&nBuffSize);	
		BYTE *pbContext = new BYTE[nBuffSize];
		DWORD *pdContext = (DWORD *)(pbContext);
		HVGetDeviceInfo(m_hhv, DESC_RESOLUTION, pdContext,&nBuffSize);	
		int	nMaxWid = *(pdContext + 2*Resolution);
		int nMaxHei = *(pdContext + 2*Resolution +1);		
		delete []pbContext;
		HVSetOutputWindow(m_hhv, 0, 0, nMaxWid, nMaxHei);
		
		//设置消隐。
		HVSetBlanking(m_hhv, m_lHBlanking, m_lVBlanking);
		}
		else
		{
			//设置消隐。
			HVSetBlanking(m_hhv, m_lHBlanking, m_lVBlanking);
		}
/***************************************************************************/

	/************************************************************************
		视频输出窗口，即视频输出范围，输出窗口取值范围必须在输入窗口范围以内，
	    视频窗口左上角X坐标和窗口宽度应为4的倍数，左上角Y坐标和窗口高度应为2的倍数
		输出窗口的起始位置一般设置为(0, 0)即可。
	*************************************************************************/
	HVSetOutputWindow(m_hhv, XStart, YStart, Width, Height);
	HVSetSnapSpeed(m_hhv, SnapSpeed);							   //设置采集速度
	SetExposureTime(Width, ExposureTint_Upper, ExposureTint_Lower, //	设置曝光时间
							m_lHBlanking, SnapSpeed, Resolution);

	
	m_pBmpInfo								= (BITMAPINFO *)m_chBmpBuf;//	m_pBmpInfo即指向m_chBmpBuf缓冲区，用户可以自己分配BTIMAPINFO缓冲区	
	m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);//	初始化BITMAPINFO 结构，此结构在保存bmp文件、显示采集图像时使用
	m_pBmpInfo->bmiHeader.biWidth			= Width;                   //	图像宽度，一般为输出窗口宽度
	m_pBmpInfo->bmiHeader.biHeight			= Height;				   //	图像宽度，一般为输出窗口高度
	
	/***********************************************
		以下设置一般相同，
		对于低于8位的位图，还应设置相应的位图调色板
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
		分配原始图像缓冲区，一般用来存储采集图像原始数据
	    一般图像缓冲区大小由输出窗口大小和视频格式确定。
	************************************************/
	 

	

	m_pRawBuffer = new BYTE[Width * Height];
	ASSERT(m_pRawBuffer);
	
	/***********************************************
		分配Bayer转换后图像数据缓冲
	************************************************/
	m_pImageBuffer = new BYTE[Width * Height*3];
	ASSERT(m_pImageBuffer);

}

IplImage* HV_Camera::HV_QueryFrame()
{
	IplImage*  Pimage;
	HVSTATUS status = STATUS_OK;
	CString strMsg;
	

	//只定义一个缓冲区来保存图象数据
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
		
	
	//	将原始图像数据进行Bayer转换，转换后为24位。
	//同时将原始数据进行上下翻转
	ConvertBayer2Rgb(m_pImageBuffer,m_pRawBuffer,Width,Height,ConvertType,
									m_pLutR,m_pLutG,m_pLutB,true,m_Layout);
	Pimage = Byte2IplImg(m_pImageBuffer, Width ,Height );

	return Pimage;


}


HVSTATUS HV_Camera::HVregister(HV_SNAP_PROC pSnapFunc,void *pParam)
{
	HVSTATUS status = STATUS_OK;

	/*
	 *	初始化数字摄像机采集图像到内存的控制，
	 *	指定回调函数SnapThreadCallback和this指针
	 */
	status = HVOpenSnap(m_hhv, pSnapFunc,pParam);					
	HV_VERIFY(status);
	
	 status = STATUS_OK;

	/*
	 *	启动数字摄像机采集图像到内存
	 */
	BYTE *ppBuf[1];
	ppBuf[0] = m_pRawBuffer;
	status = HVStartSnap(m_hhv, ppBuf,1);

	HV_VERIFY(status);
	return status;
}
 

/////////////////////////////////////////////////////////////////////////////

/****************************************************************************

说明      将byte*  转换 IplImage* 

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
	pIplImgHeader->origin = original; //0- leftup为起点，1- leftdown
	cvSetData(pIplImgHeader,pImg,lWidthByte*nchannel);

	return pIplImgHeader;
 
}
/****************************************************************************

说明      将内存m_pImageBuffer 转换 Mat

****************************************************************************/
cv::Mat  HV_Camera::getmat()
{
	 
	IplImage*  Pimage;
	Pimage = Byte2IplImg(m_pImageBuffer, Width ,Height );
	cv::Mat mat(Pimage);
	return mat;
}
/****************************************************************************

说明      将内存m_pImageBuffer 转换 CImage

****************************************************************************/
int HV_Camera::getcimage(ATL::CImage &CI)
{
	ConvertBayer2Rgb(m_pImageBuffer,m_pRawBuffer,Width,Height,ConvertType,
									m_pLutR,m_pLutG,m_pLutB,false,m_Layout);
	CI.Create( Width , -Height, 8*3);// If Height is negative, the bitmap is a top-down DIB and its origin is the upper left corner.
	
	uchar *pImg=(uchar *)CI.GetBits();//得到CImage数据区地址

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
	获取bayer格式
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
函数:
		SetExposureTime
输入参数:
		int nWindWidth			当前图像宽度		
		int lTintUpper			曝光时间的分子，shutter的取值
		int lTintLower			曝光时间的分母，与shutter的单位相关
								（ms:1000；s:1）
输出参数:
		无						
说明:
		设置曝光时间（其他的参数如摄像机时钟频率，消隐值都取默认值）
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
		
		if((dExposure-(int)dExposure) >0.5)//四舍五入
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
		
		//设置曝光时间单位
		status = HVAECControl(m_hhv, AEC_SHUTTER_UNIT, nUnit);
		if (!HV_SUCCESS(status))
		{
			return status;
		}
		
		//设置曝光时间。
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
	int nDeviceCount = 0;   //设备总数变量

	HVSTATUS status = HVGetDeviceTotal(&nDeviceCount);

	HV_VERIFY(status);

	return nDeviceCount;


}




