#pragma once
#include "GameHeadConfig.h"
#include "CCLuaEngine.h"
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};
class ScriptSys
{
public:
	static ScriptSys *getInstance();
public:
	ScriptSys();
	~ScriptSys();
public:
	//------------  c++ -> lua ------------//

	/*
	getLuaVarString : 调用lua全局string

	luaFileName  = lua文件名
	varName = 所要取Lua中的变量名
	*/
	const char* getLuaVarString(const char* luaFileName, const char* varName);

	/*
	getLuaVarOneOfTable : 调用lua全局table中的一个元素

	luaFileName  = lua文件名
	varName = 所要取Lua中的table变量名
	keyName = 所要取Lua中的table中某一个元素的Key
	*/
	const char* getLuaVarOneOfTable(const char* luaFileName, const char* varName, const char* keyName);

	/*
	getLuaVarTable : 调用lua全局table

	luaFileName  = lua文件名
	varName = 所要取的table变量名

	（注：返回的是所有的数据，童鞋们可以自己使用Map等处理）
	*/
	const char* getLuaVarTable(const char* luaFileName, const char* varName);

	/*
	   获取lua 全局table 这里的table格式是
	   g_table = ｛｝

	*/
	void getLuaVarTable(const char *luaFileName, const char* varName, vector<string>& luatable);
	/*
	callLuaFunction : 调用lua函数

	luaFileName  = lua文件名
	functionName = 所要调用Lua中的的函数名
	*/
	const char* callLuaFunction(const char* luaFileName, const char* functionName);
protected:
	const char* getFileFullPath(const char* fileName);
};