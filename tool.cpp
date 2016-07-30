#include "tool.h"
#include <Windows.h>

const int MAX_PATH_LEN = 1024;
const float FLOAT_EQUAL_EPSILON = 0.0001f;

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