#ifndef _TOOL_H
#define _TOOL_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

class Tool
{
public:
	static void traverseFilesInDirectory(std::string directory, std::vector<std::string> &fileNames, bool needPath);
	static bool isFloatEqual(float lhs, float rhs);
	static void clamp(float &val, float bottom, float up);
};

#endif //_TOOL_H