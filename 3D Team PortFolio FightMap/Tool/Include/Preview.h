#pragma once


// CPreview ��ȭ �����Դϴ�.
class CObjectView;
class CPreview : public CDialog
{
	DECLARE_DYNAMIC(CPreview)

public:
	CPreview(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPreview();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	void SetName(TCHAR* szName);

private:
	CObjectView*			m_ObjectView;

};
