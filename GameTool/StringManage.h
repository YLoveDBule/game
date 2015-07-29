#pragma once
#include "../Entity/Var.h"
#include "GameHeadConfig.h"
#include "../Entity/XmlWrapper.h"
#include "../Entity/GameLog.h"
#include "../Entity/TypeConvert.h"
#include "comUTF.h"
#include "../Entity/StringUtil.h"
class StringMgr
{
protected:
	typedef UNORDERED_MAP<string, string> StringMap;
	StringMap	m_StringMap;
	std::vector<string> m_FilterList;
public:
	StringMgr();
	virtual ~StringMgr();

	static StringMgr *getInstance();

	// 加载文字资源
	bool Init();

	bool LoadString(const char* fileName);

	bool LoadStringWithPath(const char* path, const char* fileName, bool isFilter = false);

	// 获取文字
	const char* GetString(const char* key) const;

	// 格式化字符串
	// <String Key="KeyName" Value="SrtingValue"/>
	string FormatString(const char* key);
	// <String Key="KeyName" Value="SrtingValue{%d1}..."/>
	string FormatString(const char* key, const Var* arg);
	// <String Key="KeyName" Value="SrtingValue{%d1}...{%s2}...{%f3}..."/>
	string FormatString(const char* key, const vector<Var *>& args);
	string FormatStringArgs(const char* key, const vector<Var *>& args) { return FormatString(key, args); }

	bool Parse(const char* key, const string& text, std::vector<int>& indexs, vector<ETypeId>& typeIds, vector<string>& strings);
	string ArgToString(const Var& arg, ETypeId typeId);
	string Format(const char* format, ...);

	// 添加过滤字符
	void LoadFilterString(const char* file);


	// 检测名字的合法性(禁止玩家输入的字符)
	static bool IsValidName(const string& str, string* errorStr);
	// 检测名字的长度
	static bool IsStrLenInRange(const string& str, int minLen, int maxLen);

	//处理过滤字
	string ProcessWithFilters(const char* s);
	//是否含有过滤字
	bool HasFilterWord(const string& s);

	static void ReplaceString(string& str, const string& findStr, const string& replaceStr);
};