#include "StringManage.h"

static StringMgr* s_stringMgr = nullptr;
StringMgr::StringMgr()
{

}

StringMgr::~StringMgr()
{

}

StringMgr * StringMgr::getInstance()
{
	if (!s_stringMgr)
	{
		s_stringMgr = new (std::nothrow)StringMgr();
	}
	return s_stringMgr;
}

bool StringMgr::Init()
{
	bool result = false;
	XmlWrapper xmlWrapper;
	XmlDocument& doc = xmlWrapper.GetDoc();
	m_FilterList.clear();
	m_StringMap.clear();
	do
	{
		if (!xmlWrapper.LoadFile("res/Data/String/StringList.xml"))
			break;
		XmlElement* rootNode = doc.FirstChildElement("StringList");
		if (rootNode == NULL)
			break;
		bool hasError = false;
		for (XmlElement* fileNode = rootNode->FirstChildElement("String"); fileNode; fileNode = fileNode->NextSiblingElement("String"))
		{
			const char* fileName = fileNode->Attribute("File");
			if (!LoadString(fileName))
			{
				GameLog("LoadString error =%s", fileName ? fileName : "");
				hasError = true;
			}
		}
		result = !hasError;
	} while (0);

	return result;
}

bool StringMgr::LoadString(const char* fileName)
{
	return LoadStringWithPath("res/Data/String", fileName);
}

bool StringMgr::LoadStringWithPath(const char* dpath, const char* fileName, bool isFilter)
{
	if (!fileName)
		return false;
	bool result = false;
	XmlWrapper xmlWrapper;
	XmlDocument& doc = xmlWrapper.GetDoc();

	do
	{
		char path[256] = {};
		sprintf(path, "%s/%s", dpath, fileName);
		if (!xmlWrapper.LoadFile(path))
			break;
		XmlElement* rootNode = doc.FirstChildElement("Strings");
		if (rootNode == NULL)
			break;
		if (isFilter)
		{
			for (XmlElement* stringNode = rootNode->FirstChildElement("s"); stringNode; stringNode = stringNode->NextSiblingElement("s"))
			{
				const char* val = stringNode->Attribute("v");
				m_FilterList.push_back(val);
			}
		}
		else
		{
			for (XmlElement* stringNode = rootNode->FirstChildElement("String"); stringNode; stringNode = stringNode->NextSiblingElement("String"))
			{
				const char* key = stringNode->Attribute("Key");
				const char* value = stringNode->Attribute("Value");
				if (key && value && strlen(key) > 0 && strlen(value) > 0)
				{
					if (m_StringMap.find(key) != m_StringMap.end())
					{
						//Assert(false);
						GameLog("string key exist:%s file=%s", key, fileName);
					}
					m_StringMap[key] = value;
				}
			}
		}
		result = true;
	} while (0);

	return result;
}


const char* StringMgr::GetString(const char* key) const
{
	if (!key)
		return "";
	StringMap::const_iterator it = m_StringMap.find(key);
	if (it == m_StringMap.end())
		return key;
	return it->second.c_str();
}

std::string StringMgr::FormatString(const char* key)
{
	return GetString(key);
}

std::string StringMgr::FormatString(const char* key, const Var* arg)
{
	string text = GetString(key);
	vector<int> indexs;
	vector<ETypeId> typeIds;
	vector<string> strings;
	if (!Parse(key, text, indexs, typeIds, strings))
	{
		//Assert(false);
		return key ? key : "";
	}
	if (strings.size() != 2)
	{
		//Assert(false);
		return key ? key : "";
	}
	return (strings[0] + ArgToString(&arg, typeIds[0]) + strings[1]);
}

std::string StringMgr::FormatString(const char* key, const vector<Var *>& args)
{
	string text = GetString(key);
	vector<int> indexs;
	vector<ETypeId> typeIds;
	vector<string> strings;
	if (!Parse(key, text, indexs, typeIds, strings))
	{
		//Assert(false);
		return key ? key : "";
	}
	if (strings.size() != args.size() + 1)
	{
		//Assert(false);
		return key ? key : "";
	}
	string ret = strings[0];
	for (size_t i = 0; i < args.size(); ++i)
	{
		ret += ArgToString(*args.at(i), typeIds[i]) + strings[i + 1];
	}
	return ret;
}

bool StringMgr::Parse(const char* key, const string& text, vector<int>& indexs, vector<ETypeId>& typeIds, vector<string>& strings)
{
	int i = 0;
	int j = 0;
	int k = 0;
	bool hasError = false;
	while (true)
	{
		// 找到{%的位置
		for (int x = i; x < (int)text.length(); ++x)
		{
			if (text[x] == '{' && x < (int)text.length() - 1 && text[x + 1] == '%')
			{
				i = x;
				break;
			}
		}
		if (i < 0)
			break;
		// 0123{%s1}
		// 012345678
		if ((int)text.length() < i + 5)
			break;
		// 检查格式化类型
		char typeChar = text.at(i + 2);
		// 0123{%{%s1}
		if (typeChar != 's'
			&& typeChar != 'S'
			&& typeChar != 'd'
			&& typeChar != 'D'
			&& typeChar != 'i'
			&& typeChar != 'I'
			&& typeChar != 'u'
			&& typeChar != 'U'
			&& typeChar != 'f'
			&& typeChar != 'F')
		{
			i += 2;
			continue;
		}
		j = text.find_first_of("}", i + 4);
		if (j < 0)
		{
			// 0123{%s1{%d2}
			i += 4;
			continue;
		}
		int indexLen = j - i - 3;
		/// 0123{%s}8
		if (indexLen < 1)
		{
			i = j + 1;
			continue;
		}
		// 只支持1-99的序号
		// 0123{%s100}
		if (indexLen > 2)
		{
			i = j + 1;
			continue;
		}
		string indexStr = text.substr(i + 3, indexLen);
		/// 判断是否是数字
		if (indexStr < "0" || indexStr > "99")
		{
			// 0123{%sab}
			//Assert(false);
			i = j + 1;
			continue;
		}
		int index = TypeConvert::ToInt(indexStr.c_str());
		if (index <= 0)
		{
			GameLog("StringMgr::Parse Error : %s", key);
			hasError = true;
			break;
		}
		if (typeChar == 's' || typeChar == 'S')
			typeIds.push_back(ETypeId_string);
		else if (typeChar == 'd' || typeChar == 'D' || typeChar == 'i' || typeChar == 'I')
			typeIds.push_back(ETypeId_int);
		else if (typeChar == 'u' || typeChar == 'U')
			typeIds.push_back(ETypeId_uint);
		else if (typeChar == 'f' || typeChar == 'F')
			typeIds.push_back(ETypeId_double);
		indexs.push_back(index);
		strings.push_back(text.substr(k, i - k));
		k = j + 1;
		i = j + 1;
	}
	strings.push_back(text.substr(k, text.length() - k));
	/// 检查序号是否有误
	for (size_t i = 0; i<indexs.size(); ++i)
	{
		/// 序号超出范围
		if (indexs[i] >(int)indexs.size())
		{
			GameLog("StringParser::Parse Error :%s", key);
			hasError = true;
			break;
		}
		/// 序号重复
		for (size_t j = i + 1; j < indexs.size(); ++j)
		{
			if (indexs[i] == indexs[j])
			{
				GameLog("StringParser::Parse Error : %s", key);
				hasError = true;
				break;
			}
		}
	}
	if (hasError)
	{
		indexs.clear();
		strings.clear();
	}
	return !hasError;
}

std::string StringMgr::ArgToString(const Var& arg, ETypeId typeId)
{
	if (typeId == ETypeId_int)
		return TypeConvert::ToString(arg.Int());
	else if (typeId == ETypeId_uint)
		return TypeConvert::ToString(arg.Uint());
	else if (typeId == ETypeId_string)
		return arg.String();
	else if (typeId == ETypeId_double)
		return arg.ToString();
	return "";
}

string StringMgr::Format(const char* format, ...)
{
	char buf[16 * 1024] = { NULL };

	va_list args;
	va_start(args, format);
	vsprintf(buf, format, args);
	va_end(args);

	string returnStr = buf;
	return returnStr;
}

bool StringMgr::IsValidName(const string& str, string* errorStr)
{
	static const char* s_ForbidWords[] = { "(", ")", "%", "*", "?", "[", "^", "$", "'", "\"", "/", "\\", "\n", "\r", "\t", ";", "~", " ", ",", "=", "&" };
	uint wordCount = sizeof(s_ForbidWords) / sizeof(const char*);
	for (uint i = 0; i < wordCount; ++i)
	{
		if (str.find(s_ForbidWords[i]) != string::npos)
		{
			if (errorStr != NULL)
			{
				*errorStr = s_ForbidWords[i];
			}
			return false;
		}
	}

	//全角空格的检测
	if (str.find(StringMgr::getInstance()->FormatString("QuanJiaoSpace")) != string::npos)
	{
		if (errorStr != NULL)
		{
			*errorStr = StringMgr::getInstance()->FormatString("QuanJiaoSpace");
		}
		return false;
	}

	return true;
}


bool StringMgr::IsStrLenInRange(const string& str, int minLen, int maxLen)
{
	int rlen = COM_utf8_strlen(str.c_str(), str.length());
	if (rlen < minLen && minLen>0)
		return false;
	if (rlen > maxLen && maxLen > 0)
		return false;
	return true;
}



void StringMgr::LoadFilterString(const char* file)
{
	LoadStringWithPath("String", file, true);
}

string StringMgr::ProcessWithFilters(const char* s)
{
	if (s == NULL)
		return "";
	string str = s;
	for (uint i = 0; i < m_FilterList.size(); i++)
	{
		const string& ft = m_FilterList[i];
		if (ft.length() < 1)
			continue;

		int rlen = COM_utf8_strlen(ft.c_str(), ft.length());
		if (rlen <= 0 || rlen > 100)
		{
			continue;
		}

		char rpls[1024] = { 0 };
		for (int j = 0; j < rlen; ++j)
		{
			rpls[j] = 'X';
		}
		//while(rlen > 0)
		//{
		//	sprintf(rpls,"%s%s",rpls,"X");
		//	rlen-- ;
		//}

		uint cpos = 0;
		while (cpos < str.length())
		{
			cpos = str.find(ft.c_str(), 0);
			if (cpos == string::npos)
			{
				break;
			}
			else
			{
				int starPos = cpos;
				if (starPos >= 0)
					str = str.replace(starPos, ft.length(), rpls);
			}

		}

	}
	return str;
}

bool StringMgr::HasFilterWord(const string& str)
{
	if (str.empty())
		return false;
	string s = str;
	StringUtil::ToLowerCase(s);
	for (uint i = 0; i < m_FilterList.size(); i++)
	{
		const string& ft = m_FilterList[i];
		if (ft.length() < 1)
			continue;
		if (s.find(ft, 0) != string::npos)
			return true;
	}
	return false;
}

void StringMgr::ReplaceString(string& str, const string& findStr, const string& replaceStr)
{
	if (findStr.empty())
		return;
	for (int i = 0; i < 256; ++i)
	{
		string::size_type pos = str.find(findStr);
		if (pos != string::npos)
		{
			str.replace(pos, findStr.length(), replaceStr);
			pos += replaceStr.length();
		}
		else
		{
			break;
		}
	}
}