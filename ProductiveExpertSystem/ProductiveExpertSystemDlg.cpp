
// ProductiveExpertSystemDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ProductiveExpertSystem.h"
#include "ProductiveExpertSystemDlg.h"
#include "afxdialogex.h"
#include "ReasoningMachine.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CProductiveExpertSystemDlg 对话框



CProductiveExpertSystemDlg::CProductiveExpertSystemDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PRODUCTIVEEXPERTSYSTEM_DIALOG, pParent)
	, m_rule(_T(""))
	, m_reality(_T(""))
	, m_skipclass(_T(""))
	, m_reasoningPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProductiveExpertSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_rule);
	DDX_Text(pDX, IDC_EDIT2, m_reality);
	DDX_Text(pDX, IDC_EDIT4, m_skipclass);
	DDX_Text(pDX, IDC_EDIT3, m_reasoningPath);
}

BEGIN_MESSAGE_MAP(CProductiveExpertSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CProductiveExpertSystemDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_UPDATE_RULE, &CProductiveExpertSystemDlg::OnBnClickedUpdateRule)
	ON_BN_CLICKED(IDC_UPDATE_REALITY, &CProductiveExpertSystemDlg::OnBnClickedUpdateReality)
	ON_BN_CLICKED(IDC_SEARCH, &CProductiveExpertSystemDlg::OnBnClickedSearch)
END_MESSAGE_MAP()


// CProductiveExpertSystemDlg 消息处理程序

BOOL CProductiveExpertSystemDlg::OnInitDialog()
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
	CWnd* pWnd = GetDlgItem(IDC_STATIC_RESULT);
	cfont.CreatePointFont(150, _T("宋体"), NULL);
	pWnd->SetFont(&cfont);

	ifstream fin;
	fin.open("rule.txt");
	if (!fin.is_open())
		MessageBox(_T("打开rule.txt错误"));
	else {
		string temp;
		string rule;
		while (getline(fin, temp, '\n'))
			rule = rule + temp + "\r\n";
		rule = rule.substr(0, rule.length() - 2);
		m_rule = CA2T(rule.c_str());
		fin.close();
	}

	fin.open("reality.txt");
	if (!fin.is_open())
		MessageBox(_T("打开reality.txt错误"));
	else {
		std::string temp;
		std::string real;
		while (getline(fin, temp, '\n'))
			real = real + temp + "\r\n";
		real = real.substr(0, real.length() - 2);
		m_reality = CA2T(real.c_str());
		fin.close();
	}

	UpdateData(FALSE);
	// 编辑框设置为只可读
	((CEdit*)GetDlgItem(IDC_EDIT1))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT2))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT3))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT4))->SetReadOnly(TRUE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CProductiveExpertSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CProductiveExpertSystemDlg::OnPaint()
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
HCURSOR CProductiveExpertSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CProductiveExpertSystemDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CProductiveExpertSystemDlg::OnBnClickedUpdateRule()
{
	// TODO: 在此添加控件通知处理程序代码
	// 若是不可输入状态
	if (((CEdit*)GetDlgItem(IDC_EDIT1))->GetStyle() & ES_READONLY)
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetReadOnly(FALSE);
	else {
		UpdateData(TRUE);
		std::ofstream fout;
		fout.open("rule.txt");
		if (!fout.is_open())
			MessageBox(_T("打开rule.txt错误"));
		else {
			std::string rule = CW2A(m_rule);
			// 将\\r去掉并输入文件
			int p1 = -1, p2 = -1;
			std::string temp = "";
			rule += "\r";
			for (int i = 0; i < rule.length() - 1; i++) {
				if (rule[i] == '\n')
					p1 = i;
				if (rule[i + 1] == '\r' && rule[i] != '\r') {
					p2 = i + 1;
					temp += rule.substr(p1 + 1, p2 - p1 - 1) + "\n";
				}
			}
			fout << temp.substr(0, temp.length() - 1);
			fout.close();
		}
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetReadOnly(TRUE);
	}
		
}


void CProductiveExpertSystemDlg::OnBnClickedUpdateReality()
{
	// TODO: 在此添加控件通知处理程序代码
	if (((CEdit*)GetDlgItem(IDC_EDIT2))->GetStyle() & ES_READONLY)
		((CEdit*)GetDlgItem(IDC_EDIT2))->SetReadOnly(FALSE);
	else {
		UpdateData(TRUE);
		std::ofstream fout;
		fout.open("reality.txt");
		if (!fout.is_open())
			MessageBox(_T("打开reality.txt错误"));
		else {
			std::string reality = CW2A(m_reality);
			// 将\\r去掉并输入文件
			int p1 = -1, p2 = -1;
			std::string temp = "";
			reality += "\r";
			for (int i = 0; i < reality.length() - 1; i++) {
				if (reality[i] == '\n')
					p1 = i;
				if (reality[i + 1] == '\r' && reality[i] != '\r') {
					p2 = i + 1;
					temp += reality.substr(p1 + 1, p2 - p1 - 1) + "\n";
				}
			}
			fout << temp.substr(0, temp.length() - 1);
			fout.close();
		}
		((CEdit*)GetDlgItem(IDC_EDIT2))->SetReadOnly(TRUE);
	}
}


void CProductiveExpertSystemDlg::OnBnClickedSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	ReasoningMachine rm;
	rm.readReality();	rm.readRule();
	vector<Rule> path;
	double skipclass = rm.ratiocinate(path);
	if (skipclass > 0)
		m_skipclass = "翘课";
	else
		m_skipclass = "不翘课";
	m_reasoningPath = "翘课是不好的行为, 初始值设为-1\r\n";
	for (Rule r : path) {
		for (string s : r.reason) {
			m_reasoningPath += CA2T(s.c_str());
			m_reasoningPath += " ";
		}
		m_reasoningPath += ", ";
		m_reasoningPath += CA2T(r.result.c_str());
		m_reasoningPath += "\r\n";
	}
	// 设置系数精度
	stringstream ss;
	ss << setprecision(2) << skipclass;
	string s = "最终翘课系数为：";	s += ss.str();
	m_reasoningPath += CA2T(s.c_str());
	UpdateData(FALSE);
}
