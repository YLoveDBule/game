#pragma once
#include <stdlib.h>
#include "GameHeadConfig.h"

#define MAX_LEN 2048
inline int getChLength(unsigned char c)
{
	if(c<0x7F) 
	{
		return 1;
	}

	else if((c & 0xE0) == 0xC0)
	{
		return 2;
	}

	else if((c & 0xF0) == 0xE0)
	{
		return 3;
	}

	return 0;
}

inline WCHAR Character_UTF8ToUnicode(const char *characterUTF8, int start, int len)
{
	WCHAR us[1] = {0};// = (WCHAR *)malloc(sizeof(WCHAR));
	char *uchar = (char *)us;
	switch (len) {
	case 1:
		uchar[1] = 0;
		uchar[0] = characterUTF8[start];
		break;
	case 2:
		uchar[1] = ((characterUTF8[start]>>2) & 0x07);
		uchar[0] = ((characterUTF8[start] & 0x03)<<6) + ((characterUTF8[start] & 0x3F));
		break;
	case 3: 
		uchar[1] = ((characterUTF8[start] & 0x0F)<<4) + ((characterUTF8[start+1]>>2) & 0x0F);
		uchar[0] = ((characterUTF8[start+1] & 0x03)<<6) + (characterUTF8[start+2] & 0x3F);
		break;
	}
	return us[0];
}

inline WCHAR Character_UTF8ToUnicode(vector<char> characterUTF8, int start, int len)
{
	WCHAR us[1] = {0};
	char *uchar = (char *)us;
	switch (len) {
	case 1:
		uchar[1] = 0;
		uchar[0] = characterUTF8[start];
		break;
	case 2:
		uchar[1] = ((characterUTF8[start]>>2) & 0x07);
		uchar[0] = ((characterUTF8[start] & 0x03)<<6) + ((characterUTF8[start] & 0x3F));
		break;
	case 3: 
		uchar[1] = ((characterUTF8[start] & 0x0F)<<4) + ((characterUTF8[start+1]>>2) & 0x0F);
		uchar[0] = ((characterUTF8[start+1] & 0x03)<<6) + (characterUTF8[start+2] & 0x3F);
		break;
	}
	return us[0];
}

inline WCHAR* String_UTF8ToUnicode(const char * textUTF8)
{
	static WCHAR unicode[MAX_LEN] = {0};
	WCHAR *result;
	int strCount = 0;
	int string_len = strlen(textUTF8);
	int ch_len, count_len = 0; 
	do {
		ch_len = getChLength(textUTF8[count_len]);
		if (ch_len == 0)
		{
			//FmLog("UTF8 Format Error!!!");
			//assert(0);
			break;
		}
		unicode[strCount] = Character_UTF8ToUnicode(textUTF8, count_len, ch_len); 
		strCount++;
		count_len += ch_len;
	} while (count_len < string_len);
	unicode[strCount] = '\0';
	result = unicode;

	return result;
}

inline bool SAFE_UTF8ToUnicode(char * pSrcStr,WCHAR* pDesSrt, int length)
{
	int strCount = 0;
	int string_len = length;
	int ch_len, count_len = 0; 
	do {
		ch_len = getChLength(pSrcStr[count_len]);
		pDesSrt[strCount] = Character_UTF8ToUnicode(pSrcStr, count_len, ch_len); 
		strCount++;
		count_len += ch_len;
	} while (count_len < string_len);
	pDesSrt[strCount] = '\0';
	return true;
}

inline WCHAR* String_UTF8ToUnicode(vector<char>& textUTF8)
{
	static WCHAR unicode[MAX_LEN] = {0};
	WCHAR *result;
	int strCount = 0;
	int string_len = textUTF8.size();
	int ch_len, count_len = 0; 
	do {
		ch_len = getChLength(textUTF8[count_len]);
		if (ch_len == 0)
		{
			//LOG("UTF8 Format Error!!!", "");
			//assert(0);
			break;
		}
		unicode[strCount] = Character_UTF8ToUnicode(textUTF8, count_len, ch_len); 
		strCount++;
		count_len += ch_len;
	} while (count_len < string_len);
	unicode[strCount] = '\0';
	result = unicode;
	return result;
}

string unicode_to_utf8(WCHAR *in, int insize)
{
	//SCHAR *out;
	int i = 0;
	int outsize = 0;
	int charscount = 0;
	char *result = NULL;
	char *tmp = NULL;

	charscount = insize / sizeof(WCHAR);
	result = (char *)malloc(charscount * 4 + 1);
	memset(result, 0, charscount * 4 + 1);
	tmp = result;

	for (i = 0; i < charscount; i++)
	{
		WCHAR unicode = in[i];

		if (unicode < 0x80)
		{
			*tmp++ = (char)unicode;
			outsize += 1;
		}
		else if (unicode < 0x0800)
		{
			*tmp++ = (char)((unicode >> 6) | 0xC0);
			*tmp++ = (char)((unicode & 0x3F) | 0x80);
			outsize += 2;
		}
		else if (unicode < 0x10000)
		{
			*tmp++ = (char)((unicode >> 12) | 0xE0);
			*tmp++ = (char)(((unicode >> 6) & 0x3F) | 0x80);
			*tmp++ = (char)((unicode & 0x3F) | 0x80);
			outsize += 3;
		}
		else
		{
			*tmp++ = (char)((unicode >> 18) | 0xF0);
			*tmp++ = (char)(((unicode >> 12) & 0x3F) | 0x80);
			*tmp++ = (char)(((unicode >> 6) & 0x3F) | 0x80);
			*tmp++ = (char)((unicode & 0x3F) | 0x80);
			outsize += 4;
		}
	}

	*tmp = '\0';
	string str = result;
	free(result);
	return str;
}

inline bool IsUTF8Text(const char* str, long length)
{
	int i;
	int nBytes=0;
	unsigned char chr;
	bool bAllAscii = true; 
	for(i = 0;i < length;i++)
	{
		chr= *(str + i);
		if( (chr & 0x80) != 0 ) 
			bAllAscii = false;
		if(nBytes == 0)
		{
			if(chr >= 0x80)
			{
				if(chr >= 0xFC&&chr <= 0xFD)
					nBytes = 6;
				else if(chr >= 0xF8)
					nBytes = 5;
				else if(chr >= 0xF0)
					nBytes = 4;
				else if(chr >= 0xE0)
					nBytes = 3;
				else if(chr >= 0xC0)
					nBytes = 2;
				else
				{
					return false;
				}
				nBytes--;
			}
		}
		else
		{
			if( (chr & 0xC0) != 0x80 )
			{
				return false;
			}
			nBytes--;
		}
	}

	if( nBytes > 0 )
	{
		return false;
	}

	if( bAllAscii )
	{
		return false;
	}
	return true;
}
