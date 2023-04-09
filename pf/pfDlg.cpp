
// pfDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "pf.h"
#include "pfDlg.h"
#include "afxdialogex.h"
#include "thread_parallelfor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CpfDlg dialog



CpfDlg::CpfDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PF_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CpfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CpfDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PARALLEL_FOR_BUTTON,&CpfDlg::OnBnClickedParallelForButton)
END_MESSAGE_MAP()


// CpfDlg message handlers

BOOL CpfDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CpfDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CpfDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CpfDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

class op
{
public:
	op(std::vector<unsigned int>& v):m_v(v){}
	~op()=default;
	void operator()(const afthread::parallel_for_range& r)
	{
		for( unsigned int n = r.getfrom() ; n <= r.getinclusiveto() ; ++n )
		{
			// do something with element 'n'
			m_v[n] = n;
		}
	}
protected:
	std::vector<unsigned int>& m_v;
};

std::vector<unsigned int> v(1000000,0);

void CpfDlg::OnBnClickedParallelForButton()
{
	// TODO: Add your control notification handler code here

	// store this somewhere like the 'app'
	std::shared_ptr<afthread::parallel_for_pool> spPool(new afthread::parallel_for_pool);

	op b(v);
	const unsigned int nProcess = static_cast<unsigned int>(v.size());
	const unsigned int nCores = std::thread::hardware_concurrency();
	const afthread::parallel_for_range r(0,nProcess,nCores);
	spPool->parallel_for<op>(b,r);																						// we have a million pieces of data to process, and spread the work over ALL available cores
	[](void)->void{auto i=v.cbegin(),end=v.cend();for(;i!=end;++i)ASSERT(std::distance(v.cbegin(),i)==*i);}();			// check ALL work done
}
