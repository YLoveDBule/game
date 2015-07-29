#pragma once
#include "Var.h"
#include "XmlWrapper.h"
class ExtraPropertyMap
{
protected:
	VarMap*	m_ExtraPropertys;	// 属性表
public:
	ExtraPropertyMap();
	virtual ~ExtraPropertyMap();

	VarMap* GetExtraVarMap() { return m_ExtraPropertys; }
	void AllocExtraPropertys();

	// 获取额外的属性的值
	const Var& GetExtraValue(const char* name) const;
	bool GetExtraBool(const char* name);
	int GetExtraInt(const char* name);
	uint GetExtraUint(const char* name);
	int64 GetExtraInt64(const char* name);
	uint64 GetExtraUint64(const char* name);
	float GetExtraFloat(const char* name);
	double GetExtraDouble(const char* name);
	const string& GetExtraString(const char* name);

	// 设置额外的属性的值
	bool SetExtraValue(const char* name, const Var& value);
	bool SetExtraBool(const char* name, bool value);
	bool SetExtraInt(const char* name, int value);
	bool SetExtraUint(const char* name, uint value);
	bool SetExtraInt64(const char* name, int64 value);
	bool SetExtraUint64(const char* name, uint64 value);
	bool SetExtraFloat(const char* name, float value);
	bool SetExtraDouble(const char* name, double value);
	bool SetExtraString(const char* name, const char* value);

	// 改变额外的属性的值
	bool IncExtraInt(const char* name, int value);
	bool IncExtraUint(const char* name, int value);
	bool IncExtraInt64(const char* name, int64 value);
	bool IncExtraUint64(const char* name, int64 value);
	bool IncExtraFloat(const char* name, float value);
	bool IncExtraDouble(const char* name, double value);

	// 保存额外属性
	bool SaveExtraPropertys(Stream& stream);
	// 加载额外属性
	bool LoadExtraPropertys(Stream& stream);

	// 加载属性值
	bool LoadExtraPropertyNode(XmlElement* parentNode);

	// 读取tmx的属性
	bool LoadTmxPropertyNode(XmlElement* propertysNode);
};