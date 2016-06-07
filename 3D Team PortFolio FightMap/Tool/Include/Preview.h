#pragma once


// CPreview 대화 상자입니다.
class CObjectView;
class CPreview : public CDialog
{
	DECLARE_DYNAMIC(CPreview)

public:
	CPreview(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPreview();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	void SetName(TCHAR* szName);

private:
	CObjectView*			m_ObjectView;

};
