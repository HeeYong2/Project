#ifndef FileInfo_h__
#define FileInfo_h__

/********************************************************************
	created:	2015/04/16
	file path:	Yong\Tool\Code
	file base:	FileInfo
	file ext:	h
	author:		Yong	
	purpose:	FileInfo
*********************************************************************/

#include "Tool_Defines.h"

class CFileInfo
{
public:
	explicit CFileInfo(void);
	~CFileInfo(void);

public:
	static wstring ConvertRelativePath(const wstring& szFullPath);
	static void DirtoryInfoExtraction(const wstring& wstrPath, 
		list<MESHPATH*>& rPathInfolist);

	wstring GetPath(void);

private:
	wstring	m_strPath;
};

#endif // FileInfo_h__