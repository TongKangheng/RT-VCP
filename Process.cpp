#include "Process.h"
#include<fstream>
using namespace std;
int flag = 0;
int key = 0;
int bf = 0;
int sf = 0;
Process::Process(RGBQUAD *p, UINT w, UINT h)
{
	pframe = p;
	width = w;
	height = h;
}
void Process::to_black_and_white()
{
	for (int i = 0; i < width*height; i++)
		pframe[i].rgbBlue = pframe[i].rgbGreen = pframe[i].rgbRed =
		pframe[i].rgbBlue*0.11 + pframe[i].rgbGreen*0.59 + pframe[i].rgbRed*0.3;
}
void Process::nagation()
{
	for (int i = 0; i < width* height; i++)
	{
		pframe[i].rgbBlue = 255 - pframe[i].rgbBlue;
		pframe[i].rgbRed = 255 - pframe[i].rgbRed;
		pframe[i].rgbGreen = 255 - pframe[i].rgbGreen;
	}
}
void Process::to_emboss()
{
	int r, g, b,i,j;
	for (i = 0; i < height - 1; i++)
	{
		for (j = 0; j < width - 1; j++)
		{
			b = abs(pframe[i*width + j].rgbBlue - pframe[(i + 1)*width + j + 1].rgbBlue + 128);
			g = abs(pframe[i*width + j].rgbGreen - pframe[(i + 1)*width + j + 1].rgbGreen + 128);
			r = abs(pframe[i*width + j].rgbRed - pframe[(i + 1)*width + j + 1].rgbRed + 128);
			if (b > 255) b = 255;
			if (g > 255) g = 255;
			if (r > 255) r = 255;
			pframe[i*width + j].rgbBlue = b;
			pframe[i*width + j].rgbGreen = g;
			pframe[i*width + j].rgbRed = r;
		}
		pframe[i*width + j].rgbBlue = 128;
		pframe[i*width + j].rgbGreen = 128;
		pframe[i*width + j].rgbRed = 128;
	}
	for(j=0;j<width;j++) 
		pframe[i*width + j].rgbBlue=
		pframe[i*width + j].rgbGreen=
		pframe[i*width + j].rgbRed =128;
}
void Process::smooth()
{
	RGBQUAD* temp;
	temp = new RGBQUAD[width*height];
	for (int i = 0; i < width*height; i++) temp[i] = pframe[i];
	int r, g, b, i, j, m, n, tempr,tempg,tempb,tr,tg,tb;
	tempr = tempg = tempb = 0;
	for (m = 0; m <= 2*width; m+=width)
	{
		for (n = -1; n <= 1; n++)
		{
			tempr += temp[m + (1 + n)].rgbRed;
			tempg += temp[m + (1 + n)].rgbGreen;
			tempb += temp[m + (1 + n)].rgbBlue;
		}
	}
	r = tempr;
	g = tempg;
	b = tempb;
	for (i = width; i < (height - 1)*width; i+=width)
	{
		for (j = 1; j < width - 1; j++)
		{
			tr = r;
			tg = g;
			tb = b;
			tr /= 9;
			tg /= 9;
			tb /= 9;
			if (tr > 255) tr = 255;
			if (tg > 255) tg = 255;
			if (tb > 255) tb = 255;
			pframe[i + j].rgbRed = tr;
			pframe[i + j].rgbGreen = tg;
			pframe[i + j].rgbBlue = tb;
			if (j < width - 2)
			{
				r = r - temp[i - width + j - 1].rgbRed - temp[i+ j - 1].rgbRed - temp[i +width + j - 1].rgbRed
					+ temp[i-width + j + 2].rgbRed + temp[i+ j + 2].rgbRed + temp[i+width + j + 2].rgbRed;
				g = g - temp[i-width + j - 1].rgbGreen - temp[i + j - 1].rgbGreen - temp[i +width + j - 1].rgbGreen
					+ temp[i -width + j + 2].rgbGreen + temp[i+ j + 2].rgbGreen + temp[i+width + j + 2].rgbGreen;
				b = b - temp[i -width + j - 1].rgbBlue - temp[i + j - 1].rgbBlue - temp[i+width + j - 1].rgbBlue
					+ temp[i -width + j + 2].rgbBlue + temp[i + j + 2].rgbBlue + temp[i +width + j + 2].rgbBlue;
			}
		}
		j = 1;
		if (i < (height - 2)*width)
		{
			tempr = tempr - temp[i -width + j - 1].rgbRed - temp[i - width + j].rgbRed - temp[i - width + j + 1].rgbRed
				+ temp[i+(width<<1) + j - 1].rgbRed + temp[i + (width << 1) + j].rgbRed + temp[i + (width << 1) + j + 1].rgbRed;
			tempg = tempg - temp[i-width + j - 1].rgbGreen - temp[i - width + j].rgbGreen - temp[i - width + j + 1].rgbGreen
				+ temp[i + (width << 1) + j - 1].rgbGreen + temp[i + (width << 1) + j].rgbGreen + temp[i + (width << 1) + j + 1].rgbGreen;
			tempb = tempb - temp[i - width + j - 1].rgbBlue - temp[i - width + j].rgbBlue - temp[i - width + j + 1].rgbBlue
				+ temp[i + (width << 1) + j - 1].rgbBlue + temp[i + (width << 1) + j].rgbBlue + temp[i + (width << 1) + j + 1].rgbBlue;
			r = tempr; g = tempg; b = tempb;
		}
	}
	delete temp;
}
void Process::sharp()
{
	RGBQUAD* temp;
	temp = new RGBQUAD[width*height];
	for (int i = 0; i < width*height; i++) temp[i] = pframe[i];
	int i, j;
	for (i = width; i < (height-1)*width; i+=width)
	{
		for (j = 1; j < width-1; j++)
		{
			pframe[i + j].rgbBlue = max(0,min(255,9 * temp[i + j].rgbBlue - temp[i - width + j - 1].rgbBlue - temp[i - width + j].rgbBlue
				- temp[i - width + j + 1].rgbBlue - temp[i + j - 1].rgbBlue - temp[i + j + 1].rgbBlue - temp[i + width + j - 1].rgbBlue
				- temp[i + width + j].rgbBlue - temp[i + width + j + 1].rgbBlue));
			pframe[i + j].rgbRed = max(0, min(255, 9 * temp[i + j].rgbRed- temp[i - width + j - 1].rgbRed - temp[i - width + j].rgbRed
				- temp[i - width + j + 1].rgbRed - temp[i + j - 1].rgbRed - temp[i + j + 1].rgbRed - temp[i + width + j - 1].rgbRed
				- temp[i + width + j].rgbRed - temp[i + width + j + 1].rgbRed));
			pframe[i + j].rgbGreen = max(0, min(255, 9 * temp[i + j].rgbGreen - temp[i - width + j - 1].rgbGreen - temp[i - width + j].rgbGreen
				- temp[i - width + j + 1].rgbGreen - temp[i + j - 1].rgbGreen - temp[i + j + 1].rgbGreen - temp[i + width + j - 1].rgbGreen
				- temp[i + width + j].rgbGreen - temp[i + width + j + 1].rgbGreen));
		}
	}
	delete temp;
}
void Process::dip()
{
	int temp, i;
	for (i = 0; i < width*height; i++)
	{
		temp = pframe[i].rgbBlue*0.11 + pframe[i].rgbGreen*0.59 + pframe[i].rgbRed*0.3;
		if (temp < 122) temp = 0;
		else temp = 255;
		pframe[i].rgbBlue = pframe[i].rgbGreen = pframe[i].rgbRed = temp;
	}
}
void Process::sketch()
{
	RGBQUAD* gray;
	gray = new RGBQUAD[width*height];
	RGBQUAD* inverse;
	inverse = new RGBQUAD[width*height];
	RGBQUAD* blur;
	blur = new RGBQUAD[width*height];
	int sum,a,b,temp;
	float ex;
	for (int i = 0; i < width*height; i++)
	{
		gray[i].rgbBlue = gray[i].rgbGreen = gray[i].rgbRed = (pframe[i].rgbBlue + pframe[i].rgbGreen + pframe[i].rgbRed) / 3;
		inverse[i].rgbBlue = inverse[i].rgbGreen = inverse[i].rgbRed = 255 - gray[i].rgbRed;
	}
	for (int i = width; i < (height - 1)*width; i+=width)
		for (int j = 1; j < width - 1; j++)
		{
			sum = inverse[i-width+j-1].rgbBlue + 2 * inverse[i-width+j].rgbBlue + inverse[i-width+j+1].rgbBlue +
				2 * inverse[i+j-1].rgbBlue + 4 * inverse[i+j].rgbBlue + 2 * inverse[i+j+1].rgbBlue +
				inverse[i+width+j-1].rgbBlue + 2 * inverse[i+width+j].rgbBlue + inverse[i+width+j + 1].rgbBlue;
			sum = sum / 16;
			blur[i+j].rgbBlue =blur[i+j].rgbRed=blur[i + j].rgbGreen = sum;
		}
	for(int i=0;i<width*height;i++)
	{
		b = blur[i].rgbBlue;
		a = gray[i].rgbBlue;
		temp = a + a * b / (256 - b);
		ex = temp * temp*1.0 / 255 / 255;
		temp = temp * ex;
		a = min(temp, 255);
		pframe[i].rgbBlue = pframe[i].rgbGreen = pframe[i].rgbRed = max(a-50,0);
	}
	delete gray;
	delete inverse;
	delete blur;
}
void Process::up()//调节亮度
{
	for (int i = 0; i < width*height; i++)
	{
		pframe[i].rgbBlue = max(min(255, pframe[i].rgbBlue + key),0);
		pframe[i].rgbGreen = max(min(255, pframe[i].rgbGreen + key),0);
		pframe[i].rgbRed = max(min(255, pframe[i].rgbRed + key),0);
	}
}



void IntTo4Bits(int v, unsigned char*bits)
{
	bits[3] = (unsigned char)(v >> 24);
	v = v - ((v >> 24) << 24);
	bits[2] = (unsigned char)(v >> 16);
	v = v - ((v >> 16) << 16);
	bits[1] = (unsigned char)(v >> 8);
	v = v - ((v >> 8) << 8);
	bits[0] = (unsigned char)v;
}

//保存BMP图像
void SaveAsBmp(const char *fn, char *bgra, int w, int h)
{
	ofstream out;
	out.open(fn, ios::trunc | ios::binary);


	//写BMP文件头
	unsigned char fileHead[14] = { 0x42, 0x4D,                //BM
								0x36, 0xE0, 0x10, 0x00,    //文件大小  //change
								0x00, 0x00,                //保留1
								0x00, 0x00,                //保留2
								0x36, 0x00, 0x00, 0x00     //数据相对文件起始位置偏移量 54
	};
	int len = w * h * 4 + 54;
	IntTo4Bits(len, &fileHead[2]);          //修改文件大小
	out.write((char*)fileHead, 14);

	//写BMP信息头
	unsigned char infoHead[40] = { 0x28, 0x00, 0x00, 0x00,    //该结构体大小  40
								0x00, 0x03, 0x00, 0x00,    //图像宽
								0xE0, 0x01, 0x00, 0x00,    //图像高
								0x01, 0x00,                //biPlanes
								0x20, 0x00,                //biBitCount  32位 //change
								0x00, 0x00, 0x00, 0x00,    //压缩方式，不压缩
								0x00, 0xE0, 0x10, 0x00,    //数据大小
								0x00, 0x00, 0x00, 0x00,    //biXPelsPerMeter
								0x00, 0x00, 0x00, 0x00,    //biYPelsPerMeter
								0x00, 0x00, 0x00, 0x00,    //biClrUsed
								0x00, 0x00, 0x00, 0x00,    //biClrImportant
	};
	IntTo4Bits(w, &infoHead[4]);          //修改图像高度
	IntTo4Bits(h, &infoHead[8]);          //修改图像宽度
	IntTo4Bits(len - 54, &infoHead[20]);    //修改数据大小
	out.write((char*)infoHead, 40);

	//写数据
	bgra += w * 4 * (h - 1);
	for (int i = 0; i < h; i++)
	{
		out.write(bgra, w * 4);
		bgra -= w * 4;
	}

	out.close();
}

void Process::Save()
{
	int w = width;
	int h = height;
	int i = 0, j = 0;

	char * data = new char[w*h * 4];
	memset(data, 0, w*h * 4);

	//修改图像数据
	for (i = 0; i < w*h; i += 1)
	{
		data[4 * i] = (pframe[i].rgbBlue);
		data[4 * i + 1] = (pframe[i].rgbGreen);
		data[4 * i + 2] = (pframe[i].rgbRed);
		data[4 * i + 3] = 0;
	}

	SaveAsBmp("test1.bmp", data, w, h);
	sf = 0;
	free(data);
}



