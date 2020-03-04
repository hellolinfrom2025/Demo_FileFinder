
// FileFinderDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FileFinder.h"
#include "FileFinderDlg.h"
#include "afxdialogex.h"
#include "CUiThreadApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LONG g_lFileNum;
LONG g_lThreadNum;
BOOL g_bStop=FALSE;
//查找的线程函数
UINT __cdecl FindProc(LPVOID pParam) {
	InterlockedIncrement(&g_lThreadNum);

	SFindItem* pItem =(SFindItem*) pParam;
	CString csRootPath = pItem->csPathFind;
	if (csRootPath[csRootPath.GetLength()-1]!= '\\')
	{
		csRootPath = csRootPath + _T("\\");
	}
	CFileFind finder;
	BOOL bFInd=finder.FindFile(csRootPath + _T("*.*"));
	while (bFInd && !g_bStop)
	{
		bFInd = finder.FindNextFileA();
		if (finder.IsDots())
			continue;
		//如果是目录的话继续开线程查找
		else if (finder.IsDirectory())
		{
			SFindItem* pSubItem = new SFindItem();
			pSubItem->csPathFind = finder.GetFilePath();
			pSubItem->csFileName = pItem->csFileName;
			pSubItem->hWnd = pItem->hWnd;
			AfxBeginThread(FindProc, pSubItem);
		}
		//如果找到了目标文件，则添加到组对话框的listCtrl
		else
		{
			CString csFileName = finder.GetFileName();
			if (csFileName.CompareNoCase(pItem->csFileName)==0)
			{
				CString csPath = finder.GetFilePath();
				SendMessage(pItem->hWnd, WM_ADD_ITEM, 0, (LPARAM)csPath.GetBuffer(0));
				InterlockedIncrement(&g_lFileNum);
			}
		}
	}

	InterlockedDecrement(&g_lThreadNum);
	delete pItem;
	return 0;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileFinderDlg 对话框



CFileFinderDlg::CFileFinderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILEFINDER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileFinderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE, m_listResult);
}

BEGIN_MESSAGE_MAP(CFileFinderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_FLODER, &CFileFinderDlg::OnBnClickedButtonFloder)
	ON_BN_CLICKED(IDC_BUTTON_START, &CFileFinderDlg::OnBnClickedButtonStart)
	ON_MESSAGE(WM_ADD_ITEM, &CFileFinderDlg::OnAddItem)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CFileFinderDlg::OnBnClickedButtonStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SHOW_RESULT, &CFileFinderDlg::OnBnClickedButtonFloder2)
END_MESSAGE_MAP()


// CFileFinderDlg 消息处理程序

BOOL CFileFinderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_listResult.SetExtendedStyle(m_listResult.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listResult.InsertColumn(0, _T("查找结果"), LVCFMT_LEFT,400);

	SetTimer(1, 100, NULL);
#ifdef _DEBUG
	SetDlgItemText(IDC_EDIT_FLODER, _T("C:\\MyProject\\C++"));
	SetDlgItemText(IDC_EDIT_FILE_NAME, _T("resource.h"));

#endif

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFileFinderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFileFinderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFileFinderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileFinderDlg::OnBnClickedButtonFloder()
{
	TCHAR szPathp[MAX_PATH] = { 0 };
	BROWSEINFO mBroinfo = { 0 };
	mBroinfo.hwndOwner = m_hWnd;
	ITEMIDLIST *pidl = SHBrowseForFolder(&mBroinfo);
	if (SHGetPathFromIDList(pidl,szPathp))
	{
		SetDlgItemText(IDC_EDIT_FLODER, szPathp);
	}
	CoTaskMemFree(pidl);
}


void CFileFinderDlg::OnBnClickedButtonStart()
{
	g_bStop = FALSE;

	g_lFileNum = 0;
	g_lThreadNum = 0;
	m_listResult.DeleteAllItems();
	SFindItem *pItem=new SFindItem();
	GetDlgItemText(IDC_EDIT_FLODER, pItem->csPathFind);
	GetDlgItemText(IDC_EDIT_FILE_NAME, pItem->csFileName);
	pItem->hWnd = GetSafeHwnd();
	AfxBeginThread(FindProc, pItem);
}

LRESULT CFileFinderDlg::OnAddItem(WPARAM wparm, LPARAM lparam)
{
	CString csPath=(LPCTSTR)lparam;
	m_listResult.InsertItem(m_listResult.GetItemCount(), csPath);
	return TRUE;
}


void CFileFinderDlg::OnBnClickedButtonStop()
{
	g_bStop = TRUE;
}


void CFileFinderDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (g_lThreadNum>0)
	{
		GetDlgItem(IDC_EDIT_FLODER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FLODER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FILE_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_FLODER)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_FLODER)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FILE_NAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	}


	CDialogEx::OnTimer(nIDEvent);
}


void CFileFinderDlg::OnBnClickedButtonFloder2()
{
	AfxBeginThread(RUNTIME_CLASS(CUiThreadApp));
}
