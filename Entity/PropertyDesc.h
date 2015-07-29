#pragma once

#include "Var.h"
#include "../GameTool/GameStream.h"
#include "XmlWrapper.h"
#include "StringUtil.h"

// 属性标记
enum EPropertyFlag
{
	EPropertyFlag_SaveDb = 0x0001,	// 保存数据库
	EPropertyFlag_Server = 0x0002,	// 服务器的属性
	EPropertyFlag_Client = 0x0004,	// 客户端的属性
	EPropertyFlag_SyncSelf = 0x0008,	// 只同步给自己
	EPropertyFlag_SyncScene = 0x0010,	// 同步给周围的玩家
};

// 单个属性的描述
class Desc
{
public:
	string	m_Name;				// 属性名
	Var		m_DefaultValue;		// 默认值
	uint	m_Flag;				// 标记
	//string	m_Comment;			// 注释

	Desc()
	{
		m_Flag = 0;
	}

	bool HasFlag(uint propertyFlag)
	{
		return ((m_Flag & propertyFlag) != 0);
	}
};

// 属性描述
class PropertyDesc
{
public:
	typedef vector<Desc>			DescList;
	typedef UNORDERED_MAP<string, uint>	NameIndexMap;
protected:
	string			m_Name;				// 名字
	DescList		m_Descs;			// 属性列表
	NameIndexMap	m_NameIndexMap;		// 属性名和属性索引的映射
	vector<uint>	m_SyncSelfIndexs;	// 同步给自己的属性索引列表
	vector<uint>	m_SyncSceneIndexs;	// 同步给周围玩家的属性索引列表
public:
	PropertyDesc(const string& name);

	// 名字
	const string& GetName() { return m_Name; }

	// 添加一个描述
	void Add(Desc& desc);

	// 获取描述
	Desc* GetDesc(uint index);
	Desc* GetDescByName(const string& name);
	DescList& GetDescs() { return m_Descs; }

	// 获取属性索引
	uint GetIndex(const string& name);

	// 是否有某个属性
	bool Has(const string& name);

	// 获取同步的属性索引列表
	const vector<uint>& GetSyncSelfIndexs() const { return m_SyncSelfIndexs; }
	const vector<uint>& GetSyncSceneIndexs() const { return m_SyncSceneIndexs; }

	bool SaveNameList(Stream& stream);
	void GetNameList(vector<string>& nameList);
	static bool LoadNameList(Stream& stream, vector<string>& nameList);
};

// 属性描述管理
class PropertyDescMgr
{
public:
	typedef UNORDERED_MAP<string, PropertyDesc*>	PropertyDescMap;
protected:
	PropertyDescMap m_PropertyDescs;	// 属性表
public:
	PropertyDescMgr();
	~PropertyDescMgr();

	//SINGLETON_MODE(PropertyDescMgr);
	static PropertyDescMgr *getInstance();

	// 添加一个属性描述
	void Add(PropertyDesc* desc);
	// 获取属性描述
	PropertyDesc* Get(const string& name);

	// 属性表
	const PropertyDescMgr::PropertyDescMap& GetDescs() const { return m_PropertyDescs; }

	// 加载配置数据
	bool Init(const string& fileList, bool isServer);

	// 加载属性配置文件
	bool LoadPropertyDescFile(const string& fileName, bool isServer);

	// 释放
	void Shutdown();
};
