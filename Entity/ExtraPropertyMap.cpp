#include "ExtraPropertyMap.h"

ExtraPropertyMap::ExtraPropertyMap()
{
	m_ExtraPropertys = NULL;
}

ExtraPropertyMap::~ExtraPropertyMap()
{
	Game_SAFE_DELETE(m_ExtraPropertys);
}


bool ExtraPropertyMap::LoadExtraPropertyNode(XmlElement* parentNode)
{
	if (parentNode == NULL)
	{
		//Assert(false);
		return false;
	}
	for (XmlElement* propertyNode = parentNode->FirstChildElement("Property");
		propertyNode;
		propertyNode = propertyNode->NextSiblingElement("Property"))
	{
		const char* propertyName = propertyNode->Attribute("Name");
		if (!propertyName)
		{
			//Assert(false);
			return false;
		}
		Var propertyValue;
		if (!propertyValue.InitFromString(propertyNode->Attribute("Type"), propertyNode->Attribute("Value")))
		{
			//Assert(false);
			return false;
		}
		AllocExtraPropertys();
		(*m_ExtraPropertys)[propertyName] = propertyValue;
	}
	return true;
}

bool ExtraPropertyMap::LoadTmxPropertyNode(XmlElement* propertysNode)
{
	if (propertysNode == NULL)
	{
		//Assert(false);
		return false;
	}
	for (XmlElement* node = propertysNode->FirstChildElement("property"); node; node = node->NextSiblingElement("property"))
	{
		const char* strName = node->Attribute("name");
		const char* strValue = node->Attribute("value");
		if (strName && strValue)
		{
			AllocExtraPropertys();
			(*m_ExtraPropertys)[strName] = Var(strValue);
		}
	}
	return true;
}

// 获取额外的属性的值
const Var& ExtraPropertyMap::GetExtraValue(const char* name) const
{
	if (name && m_ExtraPropertys)
	{
		VarMap::const_iterator it = m_ExtraPropertys->find(name);
		if (it != m_ExtraPropertys->end())
			return it->second;
	}
	static Var s_EmptyVar;
	return s_EmptyVar;
}

bool ExtraPropertyMap::GetExtraBool(const char* name)
{
	return GetExtraValue(name).Bool();
}

int ExtraPropertyMap::GetExtraInt(const char* name)
{
	return GetExtraValue(name).Int();
}

uint ExtraPropertyMap::GetExtraUint(const char* name)
{
	return GetExtraValue(name).Uint();
}
int64 ExtraPropertyMap::GetExtraInt64(const char* name)
{
	return GetExtraValue(name).Int64();
}
uint64 ExtraPropertyMap::GetExtraUint64(const char* name)
{
	return GetExtraValue(name).Uint64();
}
float ExtraPropertyMap::GetExtraFloat(const char* name)
{
	return GetExtraValue(name).Float();
}
double ExtraPropertyMap::GetExtraDouble(const char* name)
{
	return GetExtraValue(name).Double();
}
const string& ExtraPropertyMap::GetExtraString(const char* name)
{
	return GetExtraValue(name).String();
}

// 设置额外的属性的值
bool ExtraPropertyMap::SetExtraValue(const char* name, const Var& value)
{
	if (name)
	{
		AllocExtraPropertys();
		(*m_ExtraPropertys)[name] = value;
		return true;
	}
	return false;
}
bool ExtraPropertyMap::SetExtraBool(const char* name, bool value)
{
	return SetExtraValue(name, Var(value));
}
bool ExtraPropertyMap::SetExtraInt(const char* name, int value)
{
	return SetExtraValue(name, Var(value));
}
bool ExtraPropertyMap::SetExtraUint(const char* name, uint value)
{
	return SetExtraValue(name, Var(value));
}
bool ExtraPropertyMap::SetExtraInt64(const char* name, int64 value)
{
	return SetExtraValue(name, Var(value));
}
bool ExtraPropertyMap::SetExtraUint64(const char* name, uint64 value)
{
	return SetExtraValue(name, Var(value));
}
bool ExtraPropertyMap::SetExtraFloat(const char* name, float value)
{
	return SetExtraValue(name, Var(value));
}
bool ExtraPropertyMap::SetExtraDouble(const char* name, double value)
{
	return SetExtraValue(name, Var(value));
}
bool ExtraPropertyMap::SetExtraString(const char* name, const char* value)
{
	return SetExtraValue(name, Var(value));
}

// 改变额外的属性的值
bool ExtraPropertyMap::IncExtraInt(const char* name, int value)
{
	return SetExtraInt(name, GetExtraInt(name) + value);
}
bool ExtraPropertyMap::IncExtraUint(const char* name, int value)
{
	// 检查值的越界
	uint oldValue = GetExtraUint(name);
	if (value < 0 && (uint)(-value) > oldValue)
		return SetExtraUint(name, 0);
	else
		return SetExtraUint(name, oldValue + value);
}
bool ExtraPropertyMap::IncExtraInt64(const char* name, int64 value)
{
	return SetExtraInt64(name, GetExtraInt64(name) + value);
}
bool ExtraPropertyMap::IncExtraUint64(const char* name, int64 value)
{
	// 检查值的越界
	uint64 oldValue = GetExtraUint64(name);
	if (value < 0 && (uint64)(-value) > oldValue)
		return SetExtraUint64(name, 0);
	else
		return SetExtraUint64(name, oldValue + value);
}
bool ExtraPropertyMap::IncExtraFloat(const char* name, float value)
{
	return SetExtraFloat(name, GetExtraFloat(name) + value);
}
bool ExtraPropertyMap::IncExtraDouble(const char* name, double value)
{
	return SetExtraDouble(name, GetExtraDouble(name) + value);
}

bool ExtraPropertyMap::SaveExtraPropertys(Stream& stream)
{
	uint offset = stream.GetOffset();
	uint savePropertyCount = (m_ExtraPropertys == NULL ? 0 : (uint)m_ExtraPropertys->size());
	stream << savePropertyCount;
	if (m_ExtraPropertys)
	{
		for (VarMap::iterator it = m_ExtraPropertys->begin(); it != m_ExtraPropertys->end(); ++it)
		{
			stream << it->first;
			stream << (it->second);
		}
	}
	return (stream.GetError() == EStreamError_OK);
}

bool ExtraPropertyMap::LoadExtraPropertys(Stream& stream)
{
	uint propertyCount = 0;
	stream >> propertyCount;
	for (uint i = 0; i < propertyCount; ++i)
	{
		string name;
		Var value;
		stream >> name >> value;
		if (!stream.IsOK())
		{
			//Assert(false);
			return false;
		}
		SetExtraValue(name.c_str(), value);
	}
	return (stream.GetError() == EStreamError_OK);
}

void ExtraPropertyMap::AllocExtraPropertys()
{
	if (m_ExtraPropertys == NULL)
	{
		m_ExtraPropertys = new VarMap();
	}
}