#ifndef _TOOL_H
#define _TOOL_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <QImage>

#define MAX_NUM 1e6

class Tool
{
public:
	static void traverseFilesInDirectory(std::string directory, std::vector<std::string> &fileNames, bool needPath);
	static bool isFloatEqual(float lhs, float rhs);
	static void clamp(float &val, float bottom, float up);
	static QImage blur(std::string imagePath, int r, float alpha);

private:
	static void gaussainBlur(int *scl, int *tcl, int w, int h, int r);
	static void fastGaussainBlur(int *scl, int *tcl, int w, int h, int r);
	static std::vector<int> boxesForGauss(float sigma, int n);
	static void boxBlur(int *scl, int *tcl, int w, int h, int r);
	static void boxBlurH(int *scl, int *tcl, int w, int h, int r);
	static void boxBlurT(int *scl, int *tcl, int w, int h, int r);
};

#endif //_TOOL_H