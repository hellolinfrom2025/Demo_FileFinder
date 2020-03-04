// CDlgShowResult.cpp: 实现文件
//

#include "pch.h"
#include "FileFinder.h"
#include "CDlgShowResult.h"
#include "afxdialogex.h"


// CDlgShowResult 对话框

IMPLEMENT_DYNAMIC(CDlgShowResult, CDialogEx)

CDlgShowResult::CDlgShowResult(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SHOWRESULT, pParent)
{

}

CDlgShowResult::~CDlgShowResult()
{
}

void CDlgShowResult::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgShowResult, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CDlgShowResult::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgShowResult 消息处理程序


void CDlgShowResult::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();

	PostQuitMessage(0);
}


void CDlgShowResult::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	PostQuitMessage(0);

	CDialogEx::OnClose();
}
