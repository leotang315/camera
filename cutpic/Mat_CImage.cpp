#include "Mat_CImage.h"


int ShowMat( cv::Mat img, HWND hWndDisplay )
{
if (img.channels()<3 )
{
return -1;
}

RECT rect;
GetClientRect(hWndDisplay, &rect);
cv::Mat imgShow( abs(rect.top - rect.bottom), abs(rect.right - rect.left), CV_8UC3 );
resize( img, imgShow, imgShow.size() );

ATL::CImage CI;
int w=imgShow.cols;//宽
int h=imgShow.rows;//高
int channels=imgShow.channels();//通道数

CI.Create( w, h, 8*channels);
uchar *pS;
uchar *pImg=(uchar *)CI.GetBits();//得到CImage数据区地址
int step=CI.GetPitch();
for(int i=0;i<h;i++)
{
pS=imgShow.ptr<uchar>(i);
for(int j=0;j<w;j++)
{
for(int k=0;k<3;k++)
*(pImg+i*step+j*3+k)=pS[j*3+k];
//注意到这里的step不用乘以3
}
}

HDC dc ;
dc = GetDC( hWndDisplay );
CI.Draw( dc, 0, 0 );
ReleaseDC( hWndDisplay, dc);
CI.Destroy();

return 0;
}