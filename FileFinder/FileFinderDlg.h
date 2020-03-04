
// FileFinderDlg.h: 头文件
//

#pragma once
#define WM_ADD_ITEM (WM_USER+110)
struct SFindItem
{
	CString csPathFind;
	CString csFileName;
	HWND hWnd;
};

// CFileFinderDlg 对话框
class CFileFinderDlg : public CDialogEx
{
// 构造
public:
	CFileFinderDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEFINDER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFloder();
	CListCtrl m_listResult;
	afx_msg void OnBnClickedButtonStart();
	afx_msg LRESULT OnAddItem(WPARAM wparm, LPARAM lparam);
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonFloder2();
};
