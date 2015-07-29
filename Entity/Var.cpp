#include "Var.h"
#include "TypeConvert.h"

Var::Var()
{
	m_TypeId = ETypeId_None;
	_u64 = 0;
	_p = nullptr;
	_s = nullptr;
}

Var::~Var()
{
	if (m_TypeId == ETypeId_string)
	{
		Game_SAFE_DELETE(_s);
	}
}

Var::Var(const Var& src)
{
	if (this == &src)
		return;
	m_TypeId = ETypeId_None;
	_u64 = 0;
	_p = nullptr;
	_s = nullptr;
	CopyFrom(src);
}

Var& Var::operator = (const Var& src)
{
	if (this == &src)
		return *this;
	//m_TypeId = ETypeId_None;
	//u64 = 0;
	//p = NULL;
	//s = NULL;
	CopyFrom(src);
	return *this;
}

bool Var::operator == (const Var& src) const
{
	switch (m_TypeId)
	{
	case ETypeId_None:
		return src.m_TypeId == ETypeId_None;
		break;
	case ETypeId_bool:
		if (m_TypeId != src.m_TypeId)
			return false;
		return _b == src._b;
		break;
	case ETypeId_int8:
	case ETypeId_uint8:
	case ETypeId_int16:
	case ETypeId_uint16:
	case ETypeId_int:
		if (!src.IsNumber())
			return false;
		return _i == src.Int();
		break;
	case ETypeId_uint:
		if (!src.IsNumber())
			return false;
		return _u == src._u;
		break;
	case ETypeId_int64:
		if (!src.IsNumber())
			return false;
		return _i64 == src._i64;
		break;
	case ETypeId_uint64:
		if (!src.IsNumber())
			return false;
		return _u64 == src._u64;
		break;
	case ETypeId_float:
		if (!src.IsNumber())
			return false;
		return _f == src._f;
		break;
	case ETypeId_double:
		if (!src.IsNumber())
			return false;
		return _d == src._d;
		break;
	case ETypeId_string:
		return String() == src.String();
		break;
		//case ETypeId_pointer:
		//	return ( (p == src.p) && (m_DataSize == src.m_DataSize) );
		//	break;
	default:
		break;
	}
	return false;
}

bool Var::operator !=(const Var& other) const
{
	return !(*this == other);
}

bool Var::IsNumber() const
{
	switch (m_TypeId)
	{
	case ETypeId_int8:
	case ETypeId_uint8:
	case ETypeId_int16:
	case ETypeId_uint16:
	case ETypeId_int:
	case ETypeId_uint:
	case ETypeId_int64:
	case ETypeId_uint64:
	case ETypeId_float:
	case ETypeId_double:
		return true;
	default:
		break;
	}
	return false;
}

bool Var::Bool() const
{
	if (m_TypeId == ETypeId_bool)
	{
		return _b;
	}
	return false;
}

void Var::CopyFrom(const Var& src)
{
	if (m_TypeId == ETypeId_string && src.m_TypeId != ETypeId_string)
	{
		Game_SAFE_DELETE(_s);
	}
	else if (m_TypeId != ETypeId_string && src.m_TypeId == ETypeId_string)
	{
		_s = nullptr;
	}

	m_TypeId = src.m_TypeId;
	switch (m_TypeId)
	{
	case ETypeId_None:
		break;
	case ETypeId_bool:
		_b = src._b;
		break;
	case ETypeId_int8:
	case ETypeId_uint8:
	case ETypeId_int16:
	case ETypeId_uint16:
	case ETypeId_int:
		_i = src._i;
		break;
	case ETypeId_uint:
		_u = src._u;
		break;
	case ETypeId_int64:
		_i64 = src._i64;
		break;
	case ETypeId_uint64:
		_u64 = src._u64;
		break;
	case ETypeId_float:
		_f = src._f;
		break;
	case ETypeId_double:
		_d = src._d;
	    break;
	case ETypeId_string:
	{
		if (_s)
		{
			*_s = src.String();
		}
		else
		{
			_s = new string(src.String());
		}
	}
	break;
	default:
		break;
	}
}

void Var::Serialize(Stream& stream) const
{
	stream << m_TypeId;
	switch (m_TypeId)
	{
	case ETypeId_bool:
		stream << _b;
		break;
	case ETypeId_int8:
		stream << (int8)_i;
		break;
	case ETypeId_uint8:
		stream << (uint8)_i;
		break;
	case ETypeId_int16:
		stream << (int16)_i;
		break;
	case ETypeId_uint16:
		stream << (uint16)_i;
		break;
	case ETypeId_int:
		stream << _i;
		break;
	case ETypeId_uint:
		stream << _u;
		break;
	case ETypeId_int64:
		stream << _i64;
		break;
	case ETypeId_uint64:
		stream << _u64;
		break;
	case ETypeId_float:
		stream << _f;
		break;
	case ETypeId_double:
		stream << _d;
		break;
	case ETypeId_string:
		stream << (_s ? *_s : "");
		break;
	default:
		break;
	}
}

void Var::Deserialize(Stream& stream)
{
	_u64 = 0;
	stream >> m_TypeId;
	switch (m_TypeId)
	{
	case ETypeId_bool:
		stream >> _b;
		break;
	case ETypeId_int8:
	{
		int8 tmp = 0;
		stream >> tmp;
		_i = (int)tmp;
	}
	break;
	case ETypeId_uint8:
	{
		uint8 tmp = 0;
		stream >> tmp;
		_i = (int)tmp;
	}
	break;
	case ETypeId_int16:
	{
		int16 tmp = 0;
		stream >> tmp;
		_i = (int)tmp;
	}
	break;
	case ETypeId_uint16:
	{
		uint16 tmp = 0;
		stream >> tmp;
		_i = (int)tmp;
	}
	break;
	case ETypeId_int:
		stream >> _i;
		break;
	case ETypeId_uint:
		stream >> _u;
		break;
	case ETypeId_int64:
		stream >> _i64;
		break;
	case ETypeId_uint64:
		stream >> _u64;
		break;
	case ETypeId_float:
		stream >> _f;
		break;
	case ETypeId_double:
		stream >> _d;
		break;
	case ETypeId_string:
	{
		if (_s == NULL)
			_s = new string();
		stream >> (*_s);
	}
	break;
	default:
		break;
	}
}

ETypeId Var::GetTypeFromString(const char* strType)
{
	if (strType == NULL)
		return ETypeId_None;
	if (stricmp("int", strType) == 0)
	{
		return ETypeId_int;
	}
	else if (stricmp("uint", strType) == 0)
	{
		return ETypeId_uint;
	}
	else if (stricmp("int8", strType) == 0)
	{
		return ETypeId_int8;
	}
	else if (stricmp("uint8", strType) == 0)
	{
		return ETypeId_uint8;
	}
	else if (stricmp("int16", strType) == 0)
	{
		return ETypeId_int16;
	}
	else if (stricmp("uint16", strType) == 0)
	{
		return ETypeId_uint16;
	}
	else if (stricmp("int64", strType) == 0)
	{
		return ETypeId_int64;
	}
	else if (stricmp("uint64", strType) == 0)
	{
		return ETypeId_uint64;
	}
	else if (stricmp("float", strType) == 0)
	{
		return ETypeId_float;
	}
	else if (stricmp("double", strType) == 0)
	{
		return ETypeId_double;
	}
	else if (stricmp("bool", strType) == 0)
	{
		return ETypeId_bool;
	}
	else if (stricmp("string", strType) == 0)
	{
		return ETypeId_string;
	}
	else
	{
	}
	return ETypeId_None;
}

bool Var::InitFromString(ETypeId typeId, const char* strValue)
{
	if (m_TypeId == ETypeId_None)
	{
		m_TypeId = (uint8)typeId;
	}
	switch (typeId)
	{
	case ETypeId_int8:
	case ETypeId_uint8:
	case ETypeId_int16:
	case ETypeId_uint16:
	case ETypeId_int:
	{
		if (strValue)
		{
			SetInt(TypeConvert::ToInt(strValue));
		}
	}
	break;
	case ETypeId_uint:
	{
		if (strValue)
		{
			SetUint(TypeConvert::ToUint(strValue));
		}
	}
	break;
	case ETypeId_int64:
	{
		if (strValue)
		{
			SetInt64(TypeConvert::ToInt64(strValue));
		}
	}
	break;
	case ETypeId_uint64:
	{
		if (strValue)
		{
			SetUint64(TypeConvert::ToUint64(strValue));
		}
	}
	break;
	case ETypeId_float:
	{
		if (strValue)
		{
			SetFloat(TypeConvert::ToFloat(strValue));
		}
	}
	break;
	case ETypeId_double:
	{
		if (strValue)
		{
			SetDouble(TypeConvert::ToDouble(strValue));
		}
	}
	break;
	case ETypeId_bool:
	{
		if (strValue)
		{
			SetBool((stricmp("true", strValue) == 0 || stricmp("1", strValue) == 0));
		}
	}
	break;
	case ETypeId_string:
	{
		if (strValue)
		{
			SetString(strValue);
		}
	}
	break;
	default:
		return false;
		break;
	}
	return true;
}

bool Var::InitFromNumber(ETypeId typeId, double value)
{
	if (m_TypeId == ETypeId_None)
	{
		m_TypeId = (uint8)typeId;
	}
	switch (typeId)
	{
	case ETypeId_int8:
	case ETypeId_uint8:
	case ETypeId_int16:
	case ETypeId_uint16:
	case ETypeId_int:
	{
		SetInt((int)value);
	}
	break;
	case ETypeId_uint:
	{
		SetUint((uint)value);
	}
	break;
	case ETypeId_int64:
	{
		SetInt64((int64)value);
	}
	break;
	case ETypeId_uint64:
	{
		SetUint64((uint64)value);
	}
	break;
	case ETypeId_float:
	{
		SetFloat((float)value);
	}
	break;
	case ETypeId_double:
	{
		SetDouble(value);
	}
	break;
	default:
		return false;
		break;
	}
	return true;
}

bool Var::InitFromInt64(ETypeId typeId, int64 value)
{
	if (m_TypeId == ETypeId_None)
	{
		m_TypeId = (uint8)typeId;
	}
	switch (typeId)
	{
	case ETypeId_int64:
	{
		SetInt64(value);
	}
	break;
	case ETypeId_uint64:
	{
		SetUint64((uint64)value);
	}
	break;
	default:
		return false;
		break;
	}
	return true;
}

bool Var::InitFromUint64(ETypeId typeId, uint64 value)
{
	if (m_TypeId == ETypeId_None)
	{
		m_TypeId = (uint8)typeId;
	}
	switch (typeId)
	{
	case ETypeId_int64:
	{
		SetInt64((int64)value);
	}
	break;
	case ETypeId_uint64:
	{
		SetUint64(value);
	}
	break;
	default:
		return false;
		break;
	}
	return true;
}

bool Var::InitFromString(const char* strValue)
{
	return InitFromString(GetType(), strValue);
}

bool Var::InitFromString(const char* strType, const char* strValue)
{
	ETypeId typeId = GetTypeFromString(strType);
	return InitFromString(typeId, strValue);
}

std::string Var::ToString() const
{
	switch (m_TypeId)
	{
	case ETypeId_bool:
		return (_b ? "true" : "false");
		break;
	case ETypeId_int8:
	case ETypeId_uint8:
	case ETypeId_int16:
	case ETypeId_uint16:
	case ETypeId_int:
		return TypeConvert::ToString((int)_i);
		break;
	case ETypeId_uint:
		return TypeConvert::ToString(_u);
		break;
	case ETypeId_int64:
		return TypeConvert::ToString(_i64);
		break;
	case ETypeId_uint64:
		return TypeConvert::ToString(_u64);
		break;
	case ETypeId_float:
		return TypeConvert::ToString(_f);
		break;
	case ETypeId_double:
		return TypeConvert::ToString(_d);
		break;
	case ETypeId_string:
		return (_s ? *_s : "");
		break;
	default:
		break;
	}
	return "";
}

void Var::SetBool(bool value)
{
	if (m_TypeId == ETypeId_bool)
		_b = value;
}

void Var::SetInt(int value)
{
	SetNumber<int>(value);
}

void Var::SetUint(uint value)
{
	SetNumber<uint>(value);
}

void Var::SetInt64(int64 value)
{
	SetNumber<int64>(value);
}

void Var::SetUint64(uint64 value)
{
	SetNumber<uint64>(value);
}

void Var::SetFloat(float value)
{
	SetNumber<float>(value);
}

void Var::SetDouble(double value)
{
	SetNumber<double>(value);
}

void Var::SetString(const char* value)
{
	if (m_TypeId == ETypeId_None || m_TypeId == ETypeId_string)
	{
		if (_s == NULL)
			_s = new string(value ? value : "");
		else
			*_s = (value ? value : "");
	}
}

void Var::SetString(const string& value)
{
	if (m_TypeId == ETypeId_None || m_TypeId == ETypeId_string)
	{
		if (_s == NULL)
			_s = new string(value);
		else
			*_s = value;
	}
}

string Var::s_EmptyString;