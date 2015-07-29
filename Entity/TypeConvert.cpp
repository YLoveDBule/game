#include "TypeConvert.h"
int TypeConvert::ToInt(const char* str)
{
	if (!str)
		return 0;
	return atoi(str);
}

uint TypeConvert::ToUint(const char* str)
{
	if (!str)
		return 0;

	return static_cast<unsigned int>(strtoul(str, 0, 10));
}

int64 TypeConvert::ToInt64(const char* str)
{
	if (!str)
		return 0;

	// todo:fix
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	return (int64)_atoi64(str);
#else
	return atoll(str);
#endif
}

uint64 TypeConvert::ToUint64(const char* str)
{
	if (!str)
		return 0;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	return (uint64)(_atoi64(str));
#else
	return (uint64)atoll(str);
#endif
}

float TypeConvert::ToFloat(const char* str)
{
	if (!str)
		return 0.0f;

	return static_cast<float>(atof(str));
}

double TypeConvert::ToDouble(const char* str)
{
	if (!str)
		return 0.0f;

	return static_cast<double>(atof(str));
}

std::string TypeConvert::ToString(int value)
{
	char buffer[32];
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	itoa(value, buffer, 10);
#else
	sprintf(buffer, "%d", value);
#endif
	return string(buffer);
}

std::string TypeConvert::ToString(uint value)
{
	char buffer[32];
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	ultoa(value, buffer, 10);
#else
	sprintf(buffer, "%u", value);
#endif
	return string(buffer);
}

string TypeConvert::ToString(int64 value)
{
	char buffer[64];
	sprintf(buffer, "%lld", value);
	return string(buffer);
}

string TypeConvert::ToString(uint64 value)
{
	char buffer[64];
	sprintf(buffer, "%llu", value);
	return string(buffer);
}

std::string TypeConvert::ToString(float value)
{
	char buffer[64];
	sprintf(buffer, "%f", value);
	return string(buffer);
}

std::string TypeConvert::ToString(double value)
{

	char buffer[64];
	sprintf(buffer, "%f", value);
	return string(buffer);
}