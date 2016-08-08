#include "tool.h"
#include <algorithm>
#include <glm/ext.hpp>
#include <Windows.h>

const int MAX_PATH_LEN = 1024;
const float FLOAT_EQUAL_EPSILON = 0.001f;

using namespace std;

void Tool::traverseFilesInDirectory(string directory, vector<string> &fileNames, bool needPath)
{
	char directory_ch[MAX_PATH_LEN];
	sprintf(directory_ch, "%s/*.*", directory.c_str());

	WIN32_FIND_DATAA fd;
	HANDLE hFind = ::FindFirstFileA(directory_ch, &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (needPath)
				{
					fileNames.push_back(directory + "/" + fd.cFileName);
				}
				else
				{
					fileNames.push_back(fd.cFileName);
				}
			}
		} while (::FindNextFileA(hFind, &fd));
		::FindClose(hFind);
	}
}

bool Tool::isFloatEqual(float lhs, float rhs)
{
	return abs(lhs - rhs) < FLOAT_EQUAL_EPSILON;
}

void Tool::clamp(float &val, float bottom, float up)
{
	if (val > up)
	{
		val = up;
	}
	else if (val < bottom)
	{
		val = bottom;
	}
}

QImage Tool::blur(std::string imagePath, int r, float alpha)
{
	QImage srcImage(QString::fromLocal8Bit(imagePath.c_str()));
	srcImage = srcImage.convertToFormat(QImage::Format_RGBA8888);
	uchar *srcBits = srcImage.bits();
	int w = srcImage.width();
	int h = srcImage.height();
	int len = w * h;

	int *srcRed = new int[len];
	int *srcGreen = new int[len];
	int *srcBlue = new int[len];
	int *srcAlpha = new int[len];

	int *dstRed = new int[len];
	int *dstGreen = new int[len];
	int *dstBlue = new int[len];
	int *dstAlpha = new int[len];

	for (int i = 0; i < len; i++)
	{
		srcRed[i] = srcBits[i * 4];
		srcGreen[i] = srcBits[i * 4 + 1];
		srcBlue[i] = srcBits[i * 4 + 2];
		srcAlpha[i] = srcBits[i * 4 + 3];
	}

	fastGaussainBlur(srcRed, dstRed, w, h, r);
	fastGaussainBlur(srcGreen, dstGreen, w, h, r);
	fastGaussainBlur(srcBlue, dstBlue, w, h, r);

	uchar *dstBits = new uchar[len * 4];
	for (int i = 0; i < len; i++)
	{
		dstBits[i * 4] = (uchar)dstRed[i] * alpha + 255 * (1 - alpha);
		dstBits[i * 4 + 1] = (uchar)dstGreen[i] * alpha + 255 * (1 - alpha);
		dstBits[i * 4 + 2] = (uchar)dstBlue[i] * alpha + 255 * (1 - alpha);
		dstBits[i * 4 + 3] = (uchar)srcAlpha[i] * alpha + 255 * (1 - alpha);
	}

	return QImage(dstBits, w, h, QImage::Format_RGBA8888);
}

void Tool::blur(unsigned char *srcBits, unsigned char *&dstBits, int w, int h, int r)
{
	int len = w * h;
	dstBits = new unsigned char[len * 4];

	int *srcRed = new int[len];
	int *srcGreen = new int[len];
	int *srcBlue = new int[len];
	int *srcAlpha = new int[len];

	int *dstRed = new int[len];
	int *dstGreen = new int[len];
	int *dstBlue = new int[len];
	int *dstAlpha = new int[len];

	for (int i = 0; i < len; i++)
	{
		srcRed[i] = srcBits[i * 4];
		srcGreen[i] = srcBits[i * 4 + 1];
		srcBlue[i] = srcBits[i * 4 + 2];
		srcAlpha[i] = srcBits[i * 4 + 3];
	}

	fastGaussainBlur(srcRed, dstRed, w, h, r);
	fastGaussainBlur(srcGreen, dstGreen, w, h, r);
	fastGaussainBlur(srcBlue, dstBlue, w, h, r);

	for (int i = 0; i < len; i++)
	{
		dstBits[i * 4] = (unsigned char)dstRed[i];
		dstBits[i * 4 + 1] = (unsigned char)dstGreen[i];
		dstBits[i * 4 + 2] = (unsigned char)dstBlue[i];
		dstBits[i * 4 + 3] = (unsigned char)srcAlpha[i];
	}
}

void Tool::blend(unsigned char *lhs, unsigned char *rhs, unsigned char *&ret, int w, int h, float alpha)
{
	int len = w * h;
	ret = new unsigned char[len * 4];

	for (int i = 0; i < len * 4; i++)
	{
		ret[i] = lhs[i] * alpha + rhs[i] * (1 - alpha);
	}
}

int Tool::randIntToInt(int lower, int upper)
{
	if (lower == upper)
	{
		return lower;
	}
	//note:不包括上边界
	return lower + rand() % (upper - lower);
}

float Tool::randZeroToOne()
{
	return ((float)rand()) / RAND_MAX;
}

float Tool::randFloatToFloat(float lower, float upper)
{
	return randZeroToOne() * (upper - lower) + lower;
}

glm::vec3 Tool::randVec3ToVec3(const glm::vec3 &lower, const glm::vec3 &upper)
{
	return glm::vec3(randFloatToFloat(lower.x, upper.x),
		randFloatToFloat(lower.y, upper.y),
		randFloatToFloat(lower.z, upper.z));
}

glm::vec3 Tool::interpVec3(const glm::vec3 &lhs, const glm::vec3 &rhs, float t)
{
	return lhs * (1 - t) + rhs * t;
}

//标准椭球分布(minEmitterRange为0：整个椭球 minEmitterRange为1：椭球壳)
glm::vec3 Tool::uniformEllipsoidSample(const glm::vec3 &ellipsoid, float minEmitterRange)
{
	//先计算标准正球分布
	float a = randFloatToFloat(-180.0f, 180.0f) * (3.1415926 / 180.0f);
	float b = acos(2 * randZeroToOne() - 1);
	float r = pow(randZeroToOne() * (1 - pow(minEmitterRange, 3) + pow(minEmitterRange, 3)), 1 / 3);

	glm::vec3 sampleP;
	sampleP.x = r * sin(b) * cos(a);
	sampleP.y = r * sin(b) * sin(a);
	sampleP.z = r * cos(b);

	return sampleP * ellipsoid;
}

void Tool::gaussainBlur(int *scl, int *tcl, int w, int h, int r)
{
	int rs = ceil(r * 2.57);     // significant radius
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			float val = 0, wsum = 0;
			for (int iy = i - rs; iy < i + rs + 1; iy++)
			for (int ix = j - rs; ix < j + rs + 1; ix++)
			{
				int x = min(w - 1, max(0, ix));
				int y = min(h - 1, max(0, iy));
				int dsq = (ix - j)*(ix - j) + (iy - i)*(iy - i);
				float wght = exp(-dsq / (2 * r*r)) / (glm::pi<float>() * 2 * r*r);
				val += scl[y*w + x] * wght;  
				wsum += wght;
			}
			tcl[i*w + j] = round(val / wsum);
		}
	}
}

void Tool::fastGaussainBlur(int *scl, int *tcl, int w, int h, int r)
{
	std::vector<int> bxs = boxesForGauss(r, 3);
	boxBlur(scl, tcl, w, h, (bxs[0] - 1) / 2);
	boxBlur(tcl, scl, w, h, (bxs[1] - 1) / 2);
	boxBlur(scl, tcl, w, h, (bxs[2] - 1) / 2);
}

std::vector<int> Tool::boxesForGauss(float sigma, int n)
{
	double wIdeal = sqrt((12 * sigma*sigma / n) + 1);  // Ideal averaging filter width 
	int wl = floor(wIdeal);  
	if (wl % 2 == 0) 
		wl--;
	int wu = wl + 2;

	double mIdeal = (12 * sigma*sigma - n*wl*wl - 4 * n*wl - 3 * n) / (-4 * wl - 4);
	int m = round(mIdeal);

	std::vector<int> sizes;
	for (int i = 0; i < n; i++) 
		sizes.push_back(i < m ? wl : wu);
	return sizes;
}

void Tool::boxBlur(int *scl, int *tcl, int w, int h, int r)
{
	memcpy(tcl, scl, sizeof(int) * w * h);
	boxBlurH(tcl, scl, w, h, r);
	boxBlurH(scl, tcl, w, h, r);
}

void Tool::boxBlurH(int *scl, int *tcl, int w, int h, int r)
{
	double iarr = 1.0f / (r + r + 1);
	for (int i = 0; i < h; i++) {
		int ti = i*w, li = ti, ri = ti + r;
		int fv = scl[ti], lv = scl[ti + w - 1], val = (r + 1)*fv;
		for (int j = 0; j < r; j++) val += scl[ti + j];
		for (int j = 0; j <= r; j++) { val += scl[ri++] - fv;   tcl[ti++] = round(val*iarr); }
		for (int j = r + 1; j < w - r; j++) { val += scl[ri++] - scl[li++];   tcl[ti++] = round(val*iarr); }
		for (int j = w - r; j < w; j++) { val += lv - scl[li++];   tcl[ti++] = round(val*iarr); }
	}
}

void Tool::boxBlurT(int *scl, int *tcl, int w, int h, int r)
{
	double iarr = 1.0f / (r + r + 1);
	for (int i = 0; i < w; i++) {
		int ti = i, li = ti, ri = ti + r*w;
		int fv = scl[ti], lv = scl[ti + w*(h - 1)], val = (r + 1)*fv;
		for (int j = 0; j < r; j++) val += scl[ti + j*w];
		for (int j = 0; j <= r; j++) { val += scl[ri] - fv;  tcl[ti] = round(val*iarr);  ri += w; ti += w; }
		for (int j = r + 1; j < h - r; j++) { val += scl[ri] - scl[li];  tcl[ti] = round(val*iarr);  li += w; ri += w; ti += w; }
		for (int j = h - r; j < h; j++) { val += lv - scl[li];  tcl[ti] = round(val*iarr);  li += w; ti += w; }
	}
}