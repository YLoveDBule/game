#include "PropertyDesc.h"

PropertyDesc::PropertyDesc(const string& name)
{
	m_Name = name;
}

void PropertyDesc::Add(Desc& desc)
{
	m_Descs.push_back(desc);
	m_NameIndexMap[desc.m_Name] = (uint)m_Descs.size() - 1;
	if (desc.HasFlag(EPropertyFlag_SyncSelf))
	{
		m_SyncSelfIndexs.push_back((uint)(m_Descs.size() - 1));
	}
	else if (desc.HasFlag(EPropertyFlag_SyncScene))
	{
		// 场景同步的必然也对自己同步
		m_SyncSelfIndexs.push_back((uint)(m_Descs.size() - 1));
		m_SyncSceneIndexs.push_back((uint)(m_Descs.size() - 1));
	}
}

Desc* PropertyDesc::GetDesc(uint index)
{
	if (index >= m_Descs.size())
		return NULL;
	return &(m_Descs[index]);
}

Desc* PropertyDesc::GetDescByName(const string& name)
{
	return GetDesc(GetIndex(name));
}

uint PropertyDesc::GetIndex(const string& name)
{
	NameIndexMap::iterator it = m_NameIndexMap.find(name);
	if (it == m_NameIndexMap.end())
		return 0xffffffff;
	return it->second;
}

bool PropertyDesc::Has(const string& name)
{
	NameIndexMap::iterator it = m_NameIndexMap.find(name);
	if (it == m_NameIndexMap.end())
		return false;
	return true;
}

bool PropertyDesc::SaveNameList(Stream& stream)
{
	uint offset = stream.GetOffset();
	uint savePropertyCount = 0;
	stream << savePropertyCount;
	for (size_t i = 0; i<GetDescs().size(); ++i)
	{
		if (GetDescs()[i].HasFlag(EPropertyFlag_SaveDb))
		{
			++savePropertyCount;
			stream << (GetDescs()[i].m_Name);
		}
	}
	stream.Insert(savePropertyCount, offset);
	return (stream.GetError() == EStreamError_OK);
}

void PropertyDesc::GetNameList(vector<string>& nameList)
{
	for (size_t i = 0; i<GetDescs().size(); ++i)
	{
		if (GetDescs()[i].HasFlag(EPropertyFlag_SaveDb))
		{
			nameList.push_back(GetDescs()[i].m_Name);
		}
	}
}

bool PropertyDesc::LoadNameList(Stream& stream, vector<string>& nameList)
{
	uint propertyCount = 0;
	stream >> propertyCount;
	////Assert( propertyCount <= GetDescs().size() );
	for (uint i = 0; i<propertyCount; ++i)
	{
		string name;
		stream >> name;
		if (!stream.IsOK())
		{
			//Assert(false);
			return false;
		}
		nameList.push_back(name);
	}
	return (stream.GetError() == EStreamError_OK);
}

static PropertyDescMgr * s_propertyDescMrg = nullptr;
PropertyDescMgr::PropertyDescMgr()
{

}

PropertyDescMgr::~PropertyDescMgr()
{

}

PropertyDescMgr * PropertyDescMgr::getInstance()
{
	if (!s_propertyDescMrg)
	{
		s_propertyDescMrg = new (std::nothrow)PropertyDescMgr();
	}
	return s_propertyDescMrg;
}

void PropertyDescMgr::Add(PropertyDesc* desc)
{
	//Assert(desc);
	m_PropertyDescs[desc->GetName()] = desc;
}

PropertyDesc* PropertyDescMgr::Get(const string& name)
{
	PropertyDescMap::iterator it = m_PropertyDescs.find(name);
	if (it == m_PropertyDescs.end())
		return 0;
	return it->second;
}

bool PropertyDescMgr::Init(const string& fileList, bool isServer)
{
	XmlWrapper xmlWrapper;
	XmlDocument& doc = xmlWrapper.GetDoc();
	if (!xmlWrapper.LoadFile(fileList.c_str()))
	{
		//Assert(false);
		return false;
	}
	string outBasename;
	string outPath;
	StringUtil::SplitFilename(fileList, outBasename, outPath);
	XmlElement* rootNode = doc.FirstChildElement("FileList");
	if (rootNode == NULL)
	{
		return false;
	}
	for (XmlElement* fileNode = rootNode->FirstChildElement("File");
		fileNode; fileNode = fileNode->NextSiblingElement("File"))
	{
		const char* fileName = fileNode->Attribute("Name");
		if (fileName)
		{
			if (!LoadPropertyDescFile(outPath + fileName, isServer))
			{
				return false;
			}
		}
	}
	return true;
}

bool PropertyDescMgr::LoadPropertyDescFile(const string& fileName, bool isServer)
{
	XmlWrapper xmlWrapper;
	XmlDocument& doc = xmlWrapper.GetDoc();
	if (!xmlWrapper.LoadFile(fileName.c_str()))
	{
		//Assert(false);
		return false;
	}
	XmlElement* rootNode = doc.FirstChildElement("Propertys");
	if (rootNode == NULL)
	{
		//Assert(false);
		return false;
	}
	const char* entityName = rootNode->Attribute("EntityName");
	if (!entityName)
	{
		//Assert(false);
		return false;
	}
	// 已经加载过了,不重复加载
	if (Get(entityName) != NULL)
	{
		return true;
	}
	PropertyDesc* basePropertyDesc = NULL;
	const char* baseName = rootNode->Attribute("Base");
	if (baseName != NULL && strlen(baseName) > 0)
	{
		basePropertyDesc = Get(baseName);
		if (basePropertyDesc == NULL)
		{
			//Assert(false);
			return false;
		}
	}

	PropertyDesc* propertyDesc = new PropertyDesc(entityName);
	// 先继承父类的属性
	if (basePropertyDesc)
	{
		PropertyDesc::DescList& descList = basePropertyDesc->GetDescs();
		for (PropertyDesc::DescList::iterator it = descList.begin(); it != descList.end(); ++it)
		{
			propertyDesc->Add(*it);
		}
	}
	// 再读取自己的属性
	for (XmlElement* propertyNode = rootNode->FirstChildElement("Property");
		propertyNode; propertyNode = propertyNode->NextSiblingElement("Property"))
	{
		const char* propertyName = propertyNode->Attribute("Name");
		const char* type = propertyNode->Attribute("Type");
		const char* defaultValue = propertyNode->Attribute("Value");
		//const char* save = propertyNode->Attribute( "Save" );
		//const char* cs = propertyNode->Attribute( "CS" );
		//const char* sync = propertyNode->Attribute( "Sync" );
		//const char* comment = propertyNode->Attribute( "Comment" );
		if (!propertyName || !type)
		{
			Game_SAFE_DELETE_ARRAY(propertyDesc);
			return false;
		}
		Desc desc;
		desc.m_Name = propertyName;
		//if( comment )
		//	desc.m_Comment = comment;
		if (!desc.m_DefaultValue.InitFromString(type, defaultValue))
		{
			Game_SAFE_DELETE_ARRAY(propertyDesc);
			return false;
		}
		/*if( save && stricmp( "true", save ) == 0 )
		{
		desc.m_Flag |= EPropertyFlag_SaveDb;
		}
		if( cs )
		{
		if( stricmp( "cs", cs ) == 0 )
		{
		desc.m_Flag |= EPropertyFlag_Server;
		desc.m_Flag |= EPropertyFlag_Client;
		}
		else if( stricmp( "c", cs ) == 0 )
		{
		desc.m_Flag |= EPropertyFlag_Client;
		if( isServer )
		{
		continue;
		}
		}
		else if( stricmp( "s", cs ) == 0 )
		{
		desc.m_Flag |= EPropertyFlag_Server;
		if( !isServer )
		{
		continue;
		}
		}
		}*/
		desc.m_Flag |= EPropertyFlag_Client;

		/*if( sync )
		{
		if( stricmp( "self", sync ) == 0 )
		{
		desc.m_Flag |= EPropertyFlag_SyncSelf;
		}
		else if( stricmp( "scene", sync ) == 0 )
		{
		desc.m_Flag |= EPropertyFlag_SyncScene;
		}
		}*/
		propertyDesc->Add(desc);
	}
	m_PropertyDescs[entityName] = propertyDesc;

	return true;
}

void PropertyDescMgr::Shutdown()
{
	for (PropertyDescMap::iterator it = m_PropertyDescs.begin(); it != m_PropertyDescs.end(); ++it)
	{
		delete it->second;
	}
	m_PropertyDescs.clear();
}