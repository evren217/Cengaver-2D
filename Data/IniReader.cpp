#include "IniReader.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include "../Utilities/Macro.h"
#include "../Utilities/Globals.h"

CIniReader::CIniReader(char* szFileName)
{
	memset(m_szFileName, 0x00, 255);
	memcpy(m_szFileName, szFileName, strlen(szFileName));

	m_szSections = CountSections();
	buffer = new char[4096];
}

CIniReader::~CIniReader(){
	CLEAN(buffer)
}

int CIniReader::ReadInteger(const char* szSection, char* szKey, int iDefaultValue)
{
	int iResult = GetPrivateProfileInt(szSection,  szKey, iDefaultValue, m_szFileName); 
	return iResult;
}

float CIniReader::ReadFloat(const char* szSection, char* szKey, float fltDefaultValue)
{
	char szResult[255];
	char szDefault[255];
	float fltResult;
	sprintf_s(szDefault, "%f",fltDefaultValue);
	GetPrivateProfileString(szSection,  szKey, szDefault, szResult, 255, m_szFileName); 
	fltResult = static_cast<float>(atof(szResult));
	return fltResult;
}

bool CIniReader::ReadBoolean(const char* szSection, char* szKey, bool bolDefaultValue)
{
	char szResult[255];
	char szDefault[255];
	bool bolResult;
	sprintf_s(szDefault, "%s", bolDefaultValue? "True" : "False");
	GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName); 
	bolResult = (strcmp(szResult, "True") == 0 || strcmp(szResult, "true") == 0) ? true : false;
	return bolResult;
}

char* CIniReader::ReadString(const char* szSection, char* szKey, const char* szDefaultValue)
{
	char* szResult = new char[255];
	memset(szResult, 0x00, 255);
	GetPrivateProfileString(szSection, szKey, szDefaultValue, szResult, 255, m_szFileName); 
	return szResult;
}

unsigned char* CIniReader::ReadLargeString(const char* szSection, char* szKey, const char* szDefaultValue)
{
	unsigned char* szResult = new unsigned char[LEVEL_MAX_WIDTH*LEVEL_MAX_HEIGHT+1];
	memset(szResult, 0x00, LEVEL_MAX_WIDTH*LEVEL_MAX_HEIGHT+1);
	GetPrivateProfileStringA(szSection, szKey, szDefaultValue, (char *)szResult, LEVEL_MAX_WIDTH*LEVEL_MAX_HEIGHT+1, m_szFileName); 
	return szResult;
}

unsigned char CIniReader::SectionCount() const
{
	return m_szSections;  
}

unsigned char CIniReader::CountSections()
{
	int l = 0;
	int nSize = 4096;
	char buffer[4096];
	unsigned char count = 0;

    GetPrivateProfileSectionNames(buffer, nSize, m_szFileName);

    for (int i = 0; i < nSize; ++i)
    {
        if (buffer[i] == 0x00)
        {
            if (i == l) // the second 0x00 is EOF
				break;

            l = i + 1;
			count++;
        }
    }

	return count;
}

char* CIniReader::GetSections()
{
	int nSize = 4096;		
	buffer = new char[4096];
	GetPrivateProfileSectionNames(buffer, nSize, m_szFileName);
    return buffer;
}