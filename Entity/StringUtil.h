#pragma once
#include "../GameTool/GameHeadConfig.h"
class StringUtil
{
public:
	typedef std::stringstream StrStreamType;

	static void Trim(string& str, bool left = true, bool right = true);

	static std::vector<string> Split(const string& str, const string& delims = "\t\n ", unsigned int maxSplits = 0);

	static void ToLowerCase(string& str);

	static void ToUpperCase(string& str);

	static bool StartsWith(const string& str, const string& pattern, bool lowerCase = true);

	static bool EndsWith(const string& str, const string& pattern, bool lowerCase = true);

	static string StandardisePath(const string &init);

	static void SplitFilename(const string& qualifiedName, string& outBasename, string& outPath);

	static bool Match(const string& str, const string& pattern, bool caseSensitive = true);
};