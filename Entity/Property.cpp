#include "Property.h"
#include "GameLog.h"
Var	PropertySet::s_EmptyProperty;

PropertySet::PropertySet()
{
	m_Properties = NULL;
	m_ChangedFlag = NULL;
	m_PropertyCount = 0;
	m_PropertyDesc = NULL;
	m_IsChanged = false;
}
PropertySet::~PropertySet()
{
	Game_SAFE_DELETE_ARRAY(m_Properties);
	Game_SAFE_DELETE_ARRAY(m_ChangedFlag);
}

bool PropertySet::Init(uint propertyCount)
{
	//Assert(m_Properties == NULL);
	//Assert(m_ChangedFlag == NULL);
	m_PropertyCount = propertyCount;
	m_Properties = new Var[m_PropertyCount];
	m_ChangedFlag = new bool[m_PropertyCount];
	for (uint i = 0; i < m_PropertyCount; ++i)
	{
		m_ChangedFlag[i] = false;
	}
	return true;
}

bool PropertySet::Init(const string& descName)
{
	return Init(PropertyDescMgr::getInstance()->Get(descName));
}

bool PropertySet::Init(PropertyDesc* desc)
{
	//Assert(m_Properties == NULL);
	//Assert(m_ChangedFlag == NULL);
	//Assert(desc);
	if (desc == NULL)
		return false;
	m_PropertyDesc = desc;
	m_PropertyCount = (uint)desc->GetDescs().size();
	m_Properties = new Var[m_PropertyCount];
	m_ChangedFlag = new bool[m_PropertyCount];
	for (uint i = 0; i < m_PropertyCount; ++i)
	{
		m_Properties[i] = desc->GetDescs()[i].m_DefaultValue;
		m_ChangedFlag[i] = false;
	}
	return true;
}

uint PropertySet::GetPropertyIndex(const string& name)
{
	if (m_PropertyDesc == NULL)
		return 0xffffffff;
	return m_PropertyDesc->GetIndex(name);
}

const char* PropertySet::GetPropertyName(uint index)
{
	if (index >= m_PropertyCount || m_PropertyDesc == NULL)
	{
		//Assert(false);
		return "";
	}
	return m_PropertyDesc->GetDesc(index)->m_Name.c_str();
}

const Var& PropertySet::GetValue(uint index) const
{
	if (index >= m_PropertyCount)
	{
		//Assert(false);
		return s_EmptyProperty;
	}
	return m_Properties[index];
}

const Var& PropertySet::GetValueByName(const string& name) const
{
	if (m_PropertyDesc == 0)
	{
		//Assert(false);
		return s_EmptyProperty;
	}
	return GetValue(m_PropertyDesc->GetIndex(name));
}

bool PropertySet::SetValue(uint index, const Var& value)
{
	if (index >= m_PropertyCount)
	{
		//Assert(false);
		return false;
	}
	// 检查属性类型
	if (m_Properties[index].Type() != ETypeId_None)
	{
		if (value.Type() != m_Properties[index].Type()
			&& !(value.IsNumber() && m_Properties[index].IsNumber()))
		{
			//Assert(false);
			return false;
		}
		if (m_Properties[index] == value)
			return true;
	}
	m_Properties[index] = value;
	m_ChangedFlag[index] = true;
	m_IsChanged = true;
	return true;
}

bool PropertySet::SetValueByName(const char* name, const Var& value)
{
	if (m_PropertyDesc == 0)
	{
		//Assert(false);
		return false;
	}
	return SetValue(m_PropertyDesc->GetIndex(name), value);
}

bool PropertySet::IsChanged(uint index) const
{
	if (index >= m_PropertyCount)
	{
		//Assert(false);
		return false;
	}
	return m_ChangedFlag[index];
}

void PropertySet::ResetChangedFlag()
{
	m_IsChanged = false;
	for (uint i = 0; i < m_PropertyCount; ++i)
	{
		m_ChangedFlag[i] = false;
	}
}

bool PropertySet::Save(Stream& stream)
{
	uint offset = stream.GetOffset();
	uint savePropertyCount = 0;
	stream << savePropertyCount;
	for (uint i = 0; i < m_PropertyCount; ++i)
	{
		if (m_PropertyDesc == NULL)
		{
			stream << (m_Properties[i]);
		}
		else if (m_PropertyDesc->GetDescs()[i].HasFlag(EPropertyFlag_SaveDb))
		{
			++savePropertyCount;
			stream << (m_PropertyDesc->GetDescs()[i].m_Name);
			stream << (m_Properties[i]);
		}
	}
	stream.Insert(savePropertyCount, offset);
	return (stream.GetError() == EStreamError_OK);
}

bool PropertySet::Load(Stream& stream)
{
	uint propertyCount = 0;
	stream >> propertyCount;
	//Assert(propertyCount <= m_PropertyCount);
	for (uint i = 0; i < propertyCount; ++i)
	{
		if (m_PropertyDesc == NULL)
		{
			Var value;
			stream >> value;
			if (!stream.IsOK())
			{
				//Assert(false);
				return false;
			}
			SetValue(i, value);
		}
		else
		{
			string name;
			Var value;
			stream >> name >> value;
			if (!stream.IsOK())
			{
				//Assert(false);
				return false;
			}
			SetValueByName(name.c_str(), value);
		}
	}
	return (stream.GetError() == EStreamError_OK);
}

bool PropertySet::SaveMini(Stream& stream)
{
	uint offset = stream.GetOffset();
	uint savePropertyCount = 0;
	stream << savePropertyCount;
	for (uint i = 0; i < m_PropertyCount; ++i)
	{
		if (m_PropertyDesc == NULL)
		{
			stream << (m_Properties[i]);
		}
		else if (m_PropertyDesc->GetDescs()[i].HasFlag(EPropertyFlag_SaveDb))
		{
			++savePropertyCount;
			stream << (m_Properties[i]);
		}
	}
	stream.Insert(savePropertyCount, offset);
	return (stream.GetError() == EStreamError_OK);
}

bool PropertySet::LoadMini(Stream& stream, const vector<string>& nameList)
{
	uint propertyCount = 0;
	stream >> propertyCount;
	//Assert(propertyCount <= m_PropertyCount);
	for (uint i = 0; i < propertyCount; ++i)
	{
		if (m_PropertyDesc == NULL)
		{
			Var value;
			stream >> value;
			if (!stream.IsOK())
			{
				//Assert(false);
				return false;
			}
			SetValue(i, value);
		}
		else
		{
			Var value;
			stream >> value;
			if (!stream.IsOK())
			{
				//Assert(false);
				return false;
			}
			if (i < nameList.size())
				SetValueByName(nameList[i].c_str(), value);
		}
	}
	return (stream.GetError() == EStreamError_OK);
}

bool PropertySet::GetBool(uint index)
{
	return GetValue(index).Bool();
}
int PropertySet::GetInt(uint index)
{
	return GetValue(index).Int();
}

uint PropertySet::GetUint(uint index)
{
	return GetValue(index).Uint();
}

int64 PropertySet::GetInt64(uint index)
{
	return GetValue(index).Int64();
}

uint64 PropertySet::GetUint64(uint index)
{
	return GetValue(index).Uint64();
}

float PropertySet::GetFloat(uint index)
{
	return GetValue(index).Float();
}

double PropertySet::GetDouble(uint index)
{
	return GetValue(index).Double();
}

const string& PropertySet::GetString(uint index)
{
	return GetValue(index).String();
}

// bool SetNumber( uint index, ValueType value )
#define PROPERTYSET_SET_NUMBER( index, typeName, value )\
	if( index >= m_PropertyCount )\
			{\
		DbgLog("PROPERTYSET_SET_NUMBER error1 %s idx=%u",m_PropertyDesc?m_PropertyDesc->GetName().c_str():"", index);\
		return false;\
			}\
	if( m_Properties[index].Type() != ETypeId_None )\
			{\
		if( !( m_Properties[index].IsNumber() ) )\
						{\
			DbgLog("PROPERTYSET_SET_NUMBER error2 %s idx=%u",m_PropertyDesc?m_PropertyDesc->GetName().c_str():"", index);\
			return false;\
						}\
		if( m_Properties[index].typeName() == value )\
			return true;\
		m_Properties[index].Set##typeName( value );\
			}\
				else\
	{\
		m_Properties[index] = value;\
	}\
	m_ChangedFlag[index] = true;\
	m_IsChanged = true;\
	return true;

bool PropertySet::SetBool(uint index, bool value)
{
	if (index >= m_PropertyCount)
	{
		//Assert(false);
		DbgLog("PropertySet::SetBool error1 %s idx=%u", m_PropertyDesc ? m_PropertyDesc->GetName().c_str() : "", index);
		return false;
	}
	// 检查属性类型
	if (m_Properties[index].Type() != ETypeId_None)
	{
		if (!(m_Properties[index].Type() == ETypeId_bool))
		{
			//Assert(false);
			DbgLog("PropertySet::SetBool error2 %s idx=%u", m_PropertyDesc ? m_PropertyDesc->GetName().c_str() : "", index);
			return false;
		}
		m_Properties[index].SetBool(value);
	}
	else
	{
		m_Properties[index] = value;
	}
	m_ChangedFlag[index] = true;
	m_IsChanged = true;
	return true;
}

bool PropertySet::SetInt(uint index, int value)
{
	PROPERTYSET_SET_NUMBER(index, Int, value);
	//return SetNumber<int>( index, value );
}

bool PropertySet::SetUint(uint index, uint value)
{
	PROPERTYSET_SET_NUMBER(index, Uint, value);
}

bool PropertySet::SetInt64(uint index, int64 value)
{
	PROPERTYSET_SET_NUMBER(index, Int64, value);
}

bool PropertySet::SetUint64(uint index, uint64 value)
{
	PROPERTYSET_SET_NUMBER(index, Uint64, value);
}

bool PropertySet::SetFloat(uint index, float value)
{
	PROPERTYSET_SET_NUMBER(index, Float, value);
}

bool PropertySet::SetDouble(uint index, double value)
{
	PROPERTYSET_SET_NUMBER(index, Double, value);
}

bool PropertySet::SetString(uint index, const char* value)
{
	if (index >= m_PropertyCount)
	{
		//Assert(false);
		DbgLog("PropertySet::SetString error1 %s idx=%u", m_PropertyDesc ? m_PropertyDesc->GetName().c_str() : "", index);
		return false;
	}
	// 检查属性类型
	if (m_Properties[index].Type() != ETypeId_None)
	{
		if (!(m_Properties[index].Type() == ETypeId_string))
		{
			//Assert(false);
			DbgLog("PropertySet::SetString error2 %s idx=%u", m_PropertyDesc ? m_PropertyDesc->GetName().c_str() : "", index);
			return false;
		}
		m_Properties[index].SetString(value);
	}
	else
	{
		m_Properties[index] = Var(value ? value : "");
	}
	m_ChangedFlag[index] = true;
	m_IsChanged = true;
	return true;
}

bool PropertySet::SetBoolByName(const char* name, bool value)
{
	return SetBool(m_PropertyDesc->GetIndex(name), value);
}

bool PropertySet::SetIntByName(const char* name, int value)
{
	return SetInt(m_PropertyDesc->GetIndex(name), value);
}

bool PropertySet::SetUintByName(const char* name, uint value)
{
	return SetUint(m_PropertyDesc->GetIndex(name), value);
}

bool PropertySet::SetInt64ByName(const char* name, int64 value)
{
	return SetInt64(m_PropertyDesc->GetIndex(name), value);
}

bool PropertySet::SetUint64ByName(const char* name, uint64 value)
{
	return SetUint64(m_PropertyDesc->GetIndex(name), value);
}

bool PropertySet::SetFloatByName(const char* name, float value)
{
	return SetFloat(m_PropertyDesc->GetIndex(name), value);
}

bool PropertySet::SetDoubleByName(const char* name, double value)
{
	return SetDouble(m_PropertyDesc->GetIndex(name), value);
}

bool PropertySet::SetStringByName(const char* name, const char* value)
{
	return SetString(m_PropertyDesc->GetIndex(name), value);
}

bool PropertySet::IncUint(uint index, int value)
{
	// 检查值的越界
	uint oldValue = GetUint(index);
	if (value < 0 && (uint)(-value) > oldValue)
		return SetUint(index, 0);
	else
		return SetUint(index, oldValue + value);
}

bool PropertySet::IncUint64(uint index, int64 value)
{
	// 检查值的越界
	uint64 oldValue = GetUint64(index);
	if (value < 0 && (uint64)(-value) > oldValue)
		return SetUint64(index, 0);
	else
		return SetUint64(index, oldValue + value);
}

bool PropertySet::SaveNameList(Stream& stream)
{
	if (m_PropertyDesc == NULL)
		return false;
	return m_PropertyDesc->SaveNameList(stream);
}