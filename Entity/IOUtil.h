#pragma once

#include "../GameTool/GameHeadConfig.h"

// 读取文件数据接口
typedef uint8* (*GetFileDataAPI)(const char* fileName, const char* mode, uint* dataSize);

class IOUtil
{
public:
	static GetFileDataAPI	s_GetFileDataAPI; // 可以更改读取文件的接口
public:
	static uint8* GetFileData(const char* fileName, const char* mode, uint* dataSize);

protected:
	// 默认的获取文件数据接口
	static uint8* DefaultGetFileDataAPI(const char* fileName, const char* mode, uint* dataSize);
};