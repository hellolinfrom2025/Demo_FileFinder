#pragma once


// CDlgShowResult 对话框

class CDlgShowResult : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgShowResult)

public:
	CDlgShowResult(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgShowResult();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SHOWRESULT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
};
