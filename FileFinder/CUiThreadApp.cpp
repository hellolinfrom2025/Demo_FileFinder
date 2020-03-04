// CUiThreadApp.cpp: 实现文件
//

#include "pch.h"
#include "FileFinder.h"
#include "CUiThreadApp.h"
#include "CDlgShowResult.h"

// CUiThreadApp

IMPLEMENT_DYNCREATE(CUiThreadApp, CWinThread)

CUiThreadApp::CUiThreadApp()
{
}

CUiThreadApp::~CUiThreadApp()
{
	delete m_DlgShowResult;
}

BOOL CUiThreadApp::InitInstance()
{
	// TODO:    在此执行任意逐线程初始化
	//1. 关闭模态框，后退出界面线程
	/*CDlgShowResult dlg;
	dlg.DoModal();
	return FALSE;*/

	//2. 关闭非模态框，后退出界面线程，窗口有发送退出消息PostQuitMessage
	if (m_DlgShowResult==NULL)
	{
		m_DlgShowResult = new CDlgShowResult();
		m_DlgShowResult->Create(IDD_DIALOG_SHOWRESULT);
		m_DlgShowResult->ShowWindow(SW_SHOW);
	}
	return TRUE;

}

int CUiThreadApp::ExitInstance()
{
	// TODO:    在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUiThreadApp, CWinThread)
END_MESSAGE_MAP()


// CUiThreadApp 消息处理程序
