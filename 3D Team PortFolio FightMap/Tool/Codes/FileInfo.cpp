#include "stdafx.h"
#include "FileInfo.h"

CFileInfo::CFileInfo(void)
{

}

CFileInfo::~CFileInfo(void)
{

}

std::wstring CFileInfo::ConvertRelativePath(const wstring& szFullPath)
{
	TCHAR		szRelativePath[MAX_PATH] = L"";
	TCHAR		szDirtoryPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirtoryPath), szDirtoryPath);
	PathRelativePathTo((LPWSTR)szRelativePath
		, szDirtoryPath
		, FILE_ATTRIBUTE_DIRECTORY
		, szFullPath.c_str()
		, FILE_ATTRIBUTE_NORMAL);

	return szRelativePath;
}

void CFileInfo::DirtoryInfoExtraction(const wstring& wstrPath, list<MESHPATH*>& rPathInfolist)
{
	CFileFind		Find;
	Find.FindFile(wstrPath.c_str());

	int		iContinue = 1;
	while(iContinue)
	{
		iContinue = Find.FindNextFile();

		if(Find.IsDots())
			continue;

		if(Find.IsDirectory())
			DirtoryInfoExtraction(wstring(Find.GetFilePath() + L"\\*.*"), rPathInfolist);
		else
		{
			if(Find.IsSystem())
				continue;

			MESHPATH*   pFilePath = new MESHPATH;
			ZeroMemory(pFilePath, sizeof(MESHPATH));

			TCHAR   szPathBuf[MAX_PATH] = L"";
			lstrcpy(szPathBuf, wstrPath.c_str());														//szPathBuf = 0x0013dd98 "C:\Users\Administrator.JUSIN-20150429V\Desktop\Export\Bush\Bush1\*.*"
			PathRemoveFileSpec(szPathBuf);		    													// szPathBuf = 0x0013dd98 "C:\Users\Administrator.JUSIN-20150429V\Desktop\Export\Bush\Bush1\*.*"
		

			wstring      wstrFileName = szPathBuf;														//szPathBuf = 0x0013dd98 "C:\Users\Administrator.JUSIN-20150429V\Desktop\Export\Bush\Bush1"
			wstrFileName = PathFindFileName(wstrFileName.c_str());										//wstrFileName = "Bush1"
			pFilePath->wstrObjTag = wstrFileName.c_str();         									// pFilePath = 0x11c5ff50 {wstrFileName="Bush1" wstrFilePath="" wstrFullPath="" ...}

			// 파일 이름 다시 정한다.
			while(true)
			{
				wstrFileName = Find.GetFilePath().operator LPCWSTR();									//wstrFileName = "C:\Users\Administrator.JUSIN-20150429V\Desktop\Export\Bush\Bush1\Bush1.X"

				wstring      wstrFileTag = wstrFileName.substr( wstrPath.find_last_of(L"\\") + 1 );		//wstrFileTag = "Bush1.X"
				wstrFileTag = wstrFileTag.substr(wstrFileTag.length() - 2, wstrFileTag.length());		//wstrFileTag = ".X"
				if(wstrFileTag == L".X" ||
					wstrFileTag == L".x")      // 대 소문자 구분하니까, 구분해서 찾아주기
				{
					wstrFileName = wstrFileName.substr( wstrPath.find_last_of(L"\\") + 1 );				//wstrFileName = "Bush1.X"
					pFilePath->wstrFileTag = wstrFileName.c_str();      // 파일이름						//pFilePath->wstrType = "Bush1.X"
					break;
				}
				Find.FindNextFile();
			}
			//          wstrFileName = wstrFileName + L".X";
			//          lstrcpy(pFilePath->szFileTag, wstrFileName.c_str());      // 파일이름

			TCHAR   szBuf[MAX_PATH] = L"";
			lstrcpy(szBuf, (LPWSTR)Find.GetFilePath().operator LPCWSTR());								//szBuf = 0x0013db38 "C:\Users\Administrator.JUSIN-20150429V\Desktop\Export\Bush\Bush1\Bush1.X"
			PathRemoveFileSpec(szBuf);																	//szBuf = 0x0013db38 "C:\Users\Administrator.JUSIN-20150429V\Desktop\Export\Bush\Bush1"

			PathCombine(szBuf, szBuf, (LPWSTR)wstrFileName.c_str());									//szBuf = 0x0013db38 "C:\Users\Administrator.JUSIN-20150429V\Desktop\Export\Bush\Bush1\Bush1.X"
			PathRemoveFileSpec(szBuf);																	//szBuf = 0x0013db38 "C:\Users\Administrator.JUSIN-20150429V\Desktop\Export\Bush\Bush1"
			//PathRemoveFileSpec(szBuf);
			pFilePath->wstrFullPath = ConvertRelativePath(szBuf).c_str();								//pFilePath->wstrFullPath = "..\..\..\..\Export\Bush\Bush1"
			// 마지막 파일 이름 없애야 함

			PathRemoveFileSpec(szBuf);																	//szBuf = 0x0013db38 "C:\Users\Administrator.JUSIN-20150429V\Desktop\Export\Bush"
			pFilePath->wstrGroupTag = PathFindFileName(szBuf);											//pFilePath->wstrFilePath = "Bush"


			pFilePath->wstrFullPath += L"\\";      // 마지막에 하나 붙여주자							//pFilePath->wstrFullPath = "..\..\..\..\Export\Bush\Bush1\"
			rPathInfolist.push_back(pFilePath);															//pFilePath = 0x11c5ff50 {wstrFileName="Bush1" wstrFilePath="Bush" wstrFullPath="..\..\..\..\Export\Bush\Bush1\" ...}

			Find.FindFile(Find.GetFilePath());
			Find.FindNextFile();
		}
	}
}

std::wstring CFileInfo::GetPath(void)
{
	return m_strPath;
}