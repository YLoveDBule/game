#pragma once
#include "../GameTool/GameStream.h"
#include "VarConfig.h"
class Var
{
protected:
	uint8 m_TypeId;
    
	union 
	{
		bool    _b;
		int     _i;
		uint    _u;
		int64   _i64;
		uint64  _u64;
		float   _f;
		double  _d;
		uint8*  _p;
		string* _s;
	};

	static string    s_EmptyString;

public:
	Var();
	~Var();

	Var(bool value)
		: m_TypeId(ETypeId_bool), _b(value) {}
	Var(int8 value)
		: m_TypeId(ETypeId_int8), _i(value) {}
	Var(uint8 value)
		: m_TypeId(ETypeId_uint8), _i(value) {}
	Var(int16 value)
		: m_TypeId(ETypeId_int), _i(value) {}
	Var(int value)
		: m_TypeId(ETypeId_int), _i(value) {}
	Var(uint value)
		: m_TypeId(ETypeId_uint), _u(value) {}
	Var(int64 value)
		: m_TypeId(ETypeId_int64), _i64(value) {}
	Var(uint64 value)
		: m_TypeId(ETypeId_uint64), _u64(value) {}
	Var(float value)
		: m_TypeId(ETypeId_float), _f(value) {}
	Var(double value)
		: m_TypeId(ETypeId_double), _d(value) {}
	Var(const char * value)
		: m_TypeId(ETypeId_string)
	{
		_s = new string(value ? value : "");
	}

	Var(const string& value)
		: m_TypeId(ETypeId_string)
	{
		_s = new string(value);
	}
	Var(uint8* data, uint dataSize)
		: m_TypeId(ETypeId_pointer), _p(data)/*, m_DataSize(dataSize)*/{}
	
	Var(const Var& other);

    //overide
	Var& operator = (const Var& src);

	bool operator == (const Var& other) const;

	bool operator != (const Var& other) const;

	void CopyFrom(const Var& src);

	bool IsNumber() const;

	ETypeId Type() const { return (ETypeId)m_TypeId; }
	ETypeId GetType() const{ return (ETypeId)m_TypeId; }

	int8 Int8() const { return ToNumber<int8>(); }
	uint8 Uint8() const { return ToNumber<uint8>(); }
	int16 Int16() const { return ToNumber<int16>(); }
	uint16 Uint16() const { return ToNumber<uint16>(); }
	int Int() const { return ToNumber<int>(); }
	uint Uint() const { return ToNumber<uint>(); }
	int64 Int64() const { return ToNumber<int64>(); }
	uint64 Uint64() const { return ToNumber<uint64>(); }
	float Float() const { return ToNumber<float>(); }
	double Double() const { return ToNumber<double>(); }

	const string& String() const
	{
		if (m_TypeId == ETypeId_string)
		{
			return (_s ? *_s : s_EmptyString);
		}
		return s_EmptyString;
	}

	bool Bool() const;
	
	void Serialize(Stream& stream) const;
	void Deserialize(Stream& stream);

	static ETypeId GetTypeFromString(const char* strType);

	bool InitFromString(ETypeId typeId, const char* strValue);
	bool InitFromString(const char* strValue);
	bool InitFromString(const char* strType, const char* strValue);
	bool InitFromNumber(ETypeId typeId, double value);
	bool InitFromInt64(ETypeId typeId, int64 value);
	bool InitFromUint64(ETypeId typeId, uint64 value);
	// ×ª³É×Ö·û´®
	string ToString() const;

	// ¸³Öµ
	void SetBool(bool value);
	void SetInt(int value);
	void SetUint(uint value);
	void SetInt64(int64 value);
	void SetUint64(uint64 value);
	void SetFloat(float value);
	void SetDouble(double value);
	void SetString(const char* value);
	void SetString(const string& value);
protected:
	template<typename ValueType>
	ValueType ToNumber() const
	{
		switch (m_TypeId)
		{
		case ETypeId_int8:
		case ETypeId_uint8:
		case ETypeId_int16:
		case ETypeId_uint16:
		case ETypeId_int:
			return (ValueType)_i;
			break;
		case ETypeId_uint:
			return (ValueType)_u;
			break;
		case ETypeId_int64:
			return (ValueType)_i64;
			break;
		case ETypeId_uint64:
			return (ValueType)_u64;
			break;
		case ETypeId_float:
			return (ValueType)_f;
			break;
		case ETypeId_double:
			return (ValueType)_d;
			break;
		default:
			////Assert( false );
			break;
		}
		return 0;
	}

	template<typename ValueType>
	void SetNumber(ValueType value)
	{
		switch (m_TypeId)
		{
		case ETypeId_bool:
			_b = (value != 0);
			break;
		case ETypeId_int8:
		case ETypeId_uint8:
		case ETypeId_int16:
		case ETypeId_uint16:
		case ETypeId_int:
			_i = (int)value;
			break;
		case ETypeId_uint:
			_u = (uint)value;
			break;
		case ETypeId_int64:
			_i64 = (int64)value;
			break;
		case ETypeId_uint64:
			_u64 = (uint64)value;
			break;
		case ETypeId_float:
			_f = (float)value;
			break;
		case ETypeId_double:
			_d = (double)value;
			break;
		default:
			////Assert( false );
			break;
		}
	}
};

typedef map<string, Var> VarMap;
typedef vector<Var>		VarList;