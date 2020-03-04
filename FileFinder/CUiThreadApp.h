#pragma once



// CUiThreadApp
class CDlgShowResult;
class CUiThreadApp : public CWinThread
{
	DECLARE_DYNCREATE(CUiThreadApp)

protected:
	CUiThreadApp();           // 动态创建所使用的受保护的构造函数
	virtual ~CUiThreadApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()

private:
	CDlgShowResult* m_DlgShowResult;
};


