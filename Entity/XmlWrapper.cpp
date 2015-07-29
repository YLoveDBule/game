#include "XmlWrapper.h"
#include "IOUtil.h"
XmlWrapper::XmlWrapper()
{
	m_DataSize = 0;
	m_Data = NULL;
}

XmlWrapper::~XmlWrapper()
{
	m_Doc.Clear();
	Game_SAFE_DELETE_ARRAY(m_Data);
}

bool XmlWrapper::LoadFile(const char* fileName)
{
	//Assert(m_Data == NULL);
	m_Data = IOUtil::GetFileData(fileName, "rb", &m_DataSize);
	if (m_Data == NULL)
		return false;
	tinyxml3::XMLError errorNo = m_Doc.Parse((const char*)m_Data, m_DataSize);
	Game_SAFE_DELETE_ARRAY(m_Data);
	return (errorNo == tinyxml3::XML_NO_ERROR);
}