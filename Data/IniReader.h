#pragma once

class CIniReader
{
public:
	CIniReader(char* szFileName); 
	~CIniReader(); 
	
	int ReadInteger(const char* szSection, char* szKey, int iDefaultValue);
	float ReadFloat(const char* szSection, char* szKey, float fltDefaultValue);
	bool ReadBoolean(const char* szSection, char* szKey, bool bolDefaultValue);
	char* ReadString(const char* szSection, char* szKey, const char* szDefaultValue);
	unsigned char* ReadLargeString(const char* szSection, char* szKey, const char* szDefaultValue);
	unsigned char SectionCount() const;
	char* GetSections();

private:
	char m_szFileName[255];
	unsigned char m_szSections;
	char* buffer;

	unsigned char CountSections();
};