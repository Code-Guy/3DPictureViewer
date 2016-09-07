#ifndef _TOOL_H
#define _TOOL_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <QImage>
#include <QFileInfo>

#define MAX_NUM 1e6

class Tool
{
public:
	static void traverseFilesInDirectory(std::string directory, std::vector<std::string> &fileNames, bool needPath);
	static bool isFloatEqual(float lhs, float rhs);
	static void clamp(float &val, float bottom, float up);
	static QImage blur(std::string imagePath, int r, float alpha);
	static void blur(unsigned char *srcBits, unsigned char *&dstBits, int w, int h, int r);
	static void blend(unsigned char *lhs, unsigned char *rhs, unsigned char *&ret, int w, int h, float alpha);

	static int randIntToInt(int lower, int upper);
	static float randZeroToOne();
	static float randFloatToFloat(float lower, float upper);
	static glm::vec3 randVec3ToVec3(const glm::vec3 &lower, const glm::vec3 &upper);
	static glm::vec3 interpVec3(const glm::vec3 &lhs, const glm::vec3 &rhs, float t);

	//标准椭球分布(minEmitterRange为0：整个椭球 minEmitterRange为1：椭球壳)
	static glm::vec3 uniformEllipsoidSample(const glm::vec3 &ellipsoid, float minEmitterRange);
	static void moveToTrash(QString file);

	static QString str2qstr(const std::string str);
	static std::string qstr2str(const QString qstr);

private:
	static void gaussainBlur(int *scl, int *tcl, int w, int h, int r);
	static void fastGaussainBlur(int *scl, int *tcl, int w, int h, int r);
	static std::vector<int> boxesForGauss(float sigma, int n);
	static void boxBlur(int *scl, int *tcl, int w, int h, int r);
	static void boxBlurH(int *scl, int *tcl, int w, int h, int r);
	static void boxBlurT(int *scl, int *tcl, int w, int h, int r);

	static QFileInfoList getFileList(QString path);
};

#endif //_TOOL_H