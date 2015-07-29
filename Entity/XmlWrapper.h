#pragma once
#include "../GameTool/GameHeadConfig.h"
#include "../GameTool/GamePlatform.h"
#include "tinyxml3.h"

typedef tinyxml3::XMLDocument XmlDocument;
typedef tinyxml3::XMLElement XmlElement;

class XmlWrapper
{
protected:
	XmlDocument			m_Doc;
	uint8*					m_Data;
	uint					m_DataSize;
public:
	XmlWrapper();
	~XmlWrapper();
    
	XmlDocument& GetDoc()
	{
		return m_Doc;
	}

	//load 
	bool LoadFile(const char* fileName);
};