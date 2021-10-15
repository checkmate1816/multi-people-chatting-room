
// clientDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "client.h"
#include "clientDlg.h"
#include "afxdialogex.h"
#include"Dlg2.h"
#include"message.h"
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

// CclientDlg 对话框



CclientDlg::CclientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
bool CclientDlg::InitSocket()
{
	theApp.msocket = socket(AF_INET,SOCK_STREAM,0);//打开套接字
	if (theApp.msocket == INVALID_SOCKET)
	{
		MessageBoxA( NULL,"create socket false","false", MB_OK);
		return false;
	}
	char IP[20] = { "113.54.206.50"};
	sockaddr_in serversock;
	serversock.sin_family = AF_INET;
	serversock.sin_port = htons(9999);
	serversock.sin_addr.S_un.S_addr = inet_addr(IP);
	if (connect(theApp.msocket, (sockaddr*)&serversock, sizeof(serversock)) == INVALID_SOCKET)
	{
		closesocket(theApp.msocket);
		MessageBoxA(NULL, "connect failed", "false", MB_OK);
		return false;
	}
	return true;
}

BEGIN_MESSAGE_MAP(CclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CclientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CclientDlg::OnBnClickedButton2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CclientDlg 消息处理程序

BOOL CclientDlg::OnInitDialog()
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
	flag=InitSocket();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == SC_CLOSE)
	{
		//shutdown(theApp.msocket, SD_BOTH);
		closesocket(theApp.msocket);
		EndDialog(0);
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CclientDlg::OnPaint()
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
HCURSOR CclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CclientDlg::OnBnClickedButton1()//登陆函数
{
	char a[] = { "login" };
	char temp[50] = { '\0' };
	if (flag == 1)
	{
		send(theApp.msocket, a, sizeof(temp), 0);//发送登陆的指令
		CString strEDIT1, strEDIT2;
		GetDlgItem(IDC_EDIT1)->GetWindowText(strEDIT1);//获取ID
		GetDlgItem(IDC_EDIT2)->GetWindowText(strEDIT2);//获取密码
		char id[30] = { '\0' }, password[20] = { '\0' };
		strcpy(id, CW2A(strEDIT1));//拷贝id
		strcpy(password, CW2A(strEDIT2));//拷贝密码
		strcat(id+10, password);//将数据放在一个数组中
		send(theApp.msocket, id, sizeof(id), 0);//发送密码
		recv(theApp.msocket, temp, sizeof(temp), 0);//接受回来的反馈
		if(strlen(temp)==7)
		{ 
			message mes;
			mes.DoModal();
		}
		else
		{
			MessageBoxA(NULL, "登陆失败", "error", MB_OK);
		}
	}
	else
		MessageBoxA(NULL, "link failed", "反馈", MB_OK);
}


void CclientDlg::OnBnClickedButton2()//注册函数
{
	char a[] = { "create" };
	char temp[50] = { '\0' };
	if (flag == 1)//连接成功
	{
		send(theApp.msocket, a, sizeof(a), 0);//发送创建的指令
		recv(theApp.msocket, temp, sizeof(temp), 0);//接受生成的ID
		MessageBoxA(NULL, temp, "ID", MB_OK);
		Dlg2 dlg;
		dlg.DoModal();
	}
	else
		MessageBoxA(NULL, "link failed", "反馈", MB_OK);
	// TODO: 在此添加控件通知处理程序代码
}


void CclientDlg::OnClose()
{
		closesocket(theApp.msocket);
		CDialogEx::OnClose();
}
