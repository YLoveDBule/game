#include "ustring.h"
#include "UTF8toUnicode.h"
ustring::~ustring()
{
// 	if(m_vData.size() > 2 && m_vData[1] == '#')
// 	{
// 		PrintLog("!!!!");
// 	}
}
//ustring::ustring(std::vector<SCHAR>& str)
//{
//	WCHAR* wstr = String_UTF8ToUnicode(str);
//	setustring(wstr);
//}

char* ustring::toNormalChars()
{
	char* retval = new char[m_vData.size() + 1];
	for(int i = 0; i < (int)m_vData.size(); i++)
	{
		retval[i] = m_vData[i] & 0xff;
	}
	retval[m_vData.size()] = '\0';
	return retval;
}


string ustring::toUTF8()
{
	WCHAR* unicodedata = toUnicode();
	string retval = unicode_to_utf8(unicodedata, m_vData.size()*2);
	Game_SAFE_DELETE_ARRAY(unicodedata);
	return retval;	
}


WCHAR* ustring::toUnicode()
{
	if(!m_vData.size()) 
	{
		return NULL;
	}
	WCHAR* retval = new WCHAR[m_vData.size()];
	for(int i = 0; i < (int)m_vData.size(); i++)
	{
		retval[i] = m_vData[i] & 0xffff;
	}
	return retval;
}


void ustring::append(WCHAR wChar)
{
	m_vData.push_back(wChar);
}


void ustring::append(const char* str)
{
	WCHAR* p = String_UTF8ToUnicode(str);
	while(*p)
	{
		m_vData.push_back(*p);
		p++;
	}
}

void ustring::clear()
{
	m_vData.clear();
}