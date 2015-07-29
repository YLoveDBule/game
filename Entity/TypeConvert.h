#pragma once
#include "../GameTool/GameHeadConfig.h"
class TypeConvert
{
public:
	static int ToInt(const char* str);

	static uint ToUint(const char* str);

	static int64 ToInt64(const char* str);

	static uint64 ToUint64(const char* str);

	static float ToFloat(const char* str);
	static double ToDouble(const char* str);

	static string ToString(int value);
	static string ToString(uint value);
	static string ToString(int64 value);
	static string ToString(uint64 value);
	static string ToString(float value);
	static string ToString(double value);
};