#pragma once
#include "GameHeadConfig.h"
class ustring
{
public:
	void append(WCHAR wChar);

	void append(const char* str);

	void clear();

	char* toNormalChars();

	string toUTF8();

	WCHAR* toUnicode();
	~ustring();

private:
	vector<WCHAR> m_vData;
};