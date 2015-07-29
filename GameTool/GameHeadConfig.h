#pragma once
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>
using namespace std;

#include <algorithm>
#include <functional>
#include <limits>
#include <stdarg.h>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

typedef unsigned long long uint64;
typedef long long       int64;
typedef unsigned int	uint32;
typedef unsigned short	uint16;
typedef unsigned char	uint8;
typedef int				int32;
typedef short			int16;
typedef char			int8;
typedef	unsigned long	ulong;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned char	uchar;

typedef unsigned short	WCHAR;


#include <unordered_map>
#include <unordered_set>
#define UNORDERED_MAP unordered_map


// 单件模式
#define SINGLETON_MODE( ClassName ) \
	static ClassName& GetInstance() \
	{\
		static ClassName s_##ClassName;\
		return s_##ClassName;\
	}

#define Game_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
