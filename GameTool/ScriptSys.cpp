#include "ScriptSys.h"

static ScriptSys * s_scriptSys = nullptr;
ScriptSys * ScriptSys::getInstance()
{
	if (!s_scriptSys)
	{
		s_scriptSys = new (std::nothrow)ScriptSys();
		CCASSERT(s_scriptSys, "FATAL: Not enough memory");
	}
	return s_scriptSys;
}

ScriptSys::ScriptSys()
{

}

ScriptSys::~ScriptSys()
{

}

const char* ScriptSys::getLuaVarString(const char* luaFileName, const char* varName)
{
	lua_State*  ls = LuaEngine::getInstance()->getLuaStack()->getLuaState();

	int isOpen = luaL_dofile(ls, getFileFullPath(luaFileName));
	if (isOpen != 0){
		CCLOG("Open Lua Error: %i", isOpen);
		return NULL;
	}

	lua_settop(ls, 0);
	lua_getglobal(ls, varName);

	int statesCode = lua_isstring(ls, 1);
	if (statesCode != 1){
		CCLOG("Open Lua Error: %i", statesCode);
		return NULL;
	}

	const char* str = lua_tostring(ls, 1);
	lua_pop(ls, 1);

	return str;
}

const char* ScriptSys::getLuaVarOneOfTable(const char* luaFileName, const char* varName, const char* keyName)
{
	lua_State*  ls = LuaEngine::defaultEngine()->getLuaStack()->getLuaState();

	int isOpen = luaL_dofile(ls, getFileFullPath(luaFileName));
	if (isOpen != 0){
		CCLOG("Open Lua Error: %i", isOpen);
		return NULL;
	}

	lua_getglobal(ls, varName);

	int statesCode = lua_istable(ls, -1);
	if (statesCode != 1){
		CCLOG("Open Lua Error: %i", statesCode);
		return NULL;
	}

	lua_pushstring(ls, keyName);
	lua_gettable(ls, -2);
	const char* valueString = lua_tostring(ls, -1);

	lua_pop(ls, -1);

	return valueString;
}

const char* ScriptSys::getLuaVarTable(const char* luaFileName, const char* varName)
{
	lua_State*  ls = LuaEngine::defaultEngine()->getLuaStack()->getLuaState();

	int isOpen = luaL_dofile(ls, getFileFullPath(luaFileName));
	if (isOpen != 0){
		CCLOG("Open Lua Error: %i", isOpen);
		return NULL;
	}

	lua_getglobal(ls, varName);

	int it = lua_gettop(ls);
	lua_pushnil(ls);

	string result = "";

	while (lua_next(ls, it) != 0)
	{
		string key = lua_tostring(ls, -2);
		string value = lua_tostring(ls, -1);

		result = result + key + ":" + value + "\t";

		lua_pop(ls, 1);
	}
	lua_pop(ls, 1);

	return result.c_str();
}



void ScriptSys::getLuaVarTable(const char *luaFileName, const char* varName, vector<string>& luatable)
{
	auto engine = LuaEngine::getInstance();
	ScriptEngineManager::getInstance()->setScriptEngine(engine);
	lua_State* ls = engine->getLuaStack()->getLuaState();

	int isOpen = engine->executeScriptFile(luaFileName);//luaL_dofile(ls, getFileFullPath(luaFileName));
	if (isOpen != 0){
		CCLOG("Open Lua Error: %i", isOpen);
		return;
	}

	lua_getglobal(ls, varName);

	int it = lua_gettop(ls);
	lua_pushnil(ls);

	string result = "";

	while (lua_next(ls, it) != 0)
	{
		string value = lua_tostring(ls, -1);
		luatable.push_back(value);
		lua_pop(ls, 1);
	}
	lua_pop(ls, 1);
}

const char* ScriptSys::callLuaFunction(const char* luaFileName, const char* functionName)
{
	//lua_State*  ls = LuaEngine::defaultEngine()->getLuaStack()->getLuaState();

	//int isOpen = luaL_dofile(ls, getFileFullPath(luaFileName));
	//if (isOpen != 0){
	//	CCLOG("Open Lua Error: %i", isOpen);
	//	return NULL;
	//}

	//lua_getglobal(ls, functionName);

	//lua_pushstring(ls, "Himi");
	//lua_pushnumber(ls, 23);
	//lua_pushboolean(ls, true);

	///*
	//lua_call
	//第一个参数:函数的参数个数
	//第二个参数:函数返回值个数
	//*/
	//lua_call(ls, 3, 1);

	const char* iResult = "";//lua_tostring(ls, -1);

	return iResult;
}

const char* ScriptSys::getFileFullPath(const char* fileName)
{
	string s = FileUtils::sharedFileUtils()->fullPathForFilename(fileName);
	return s.c_str();
}

