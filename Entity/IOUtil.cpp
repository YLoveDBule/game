#include "IOUtil.h"

GetFileDataAPI	IOUtil::s_GetFileDataAPI = IOUtil::DefaultGetFileDataAPI;

uint8* IOUtil::GetFileData(const char* fileName, const char* mode, uint* dataSize)
{
	return s_GetFileDataAPI(fileName, mode, dataSize);
}

uint8* IOUtil::DefaultGetFileDataAPI(const char* fileName, const char* mode, uint* dataSize)
{
	if (!fileName)
		return NULL;
	FILE* fp = fopen(fileName, mode ? mode : "rb");
	if (!fp)
		return NULL;

	uint size = 0;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if (size == 0)
	{
		fclose(fp);
		return NULL;
	}
	uint8* pData = new uint8[size];
	size = fread(pData, sizeof(uint8), size, fp);
	fclose(fp);

	if (dataSize)
	{
		*dataSize = size;
	}
	return pData;
}