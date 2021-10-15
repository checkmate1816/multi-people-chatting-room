// message.cpp: 实现文件
//

#include "stdafx.h"
#include "client.h"
#include "message.h"
#include "afxdialogex.h"
#include "resource.h"

// message 对话框

IMPLEMENT_DYNAMIC(message, CDialogEx)

message::message(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

message::~message()
{
}

void message::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(message, CDialogEx)
	ON_MESSAGE(M_RECVDATA, MRECVDATA)
	ON_BN_CLICKED(IDC_BUTTON2, &message::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &message::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &message::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &message::OnBnClickedButton1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// message 消息处理程序



LRESULT message::MRECVDATA(WPARAM wParam, LPARAM lParam)//将接受到的数据放入对话框中
{
	CString str,str1 ;
	str = (char*)wParam;
	str1 = (char*)lParam;
	str += ":";
	str += str1;
	str += "\r\n";
	SetDlgItemText(IDC_EDIT6, str);
	return true;
}

DWORD WINAPI message::Threadpro(LPVOID mlpvpid)//消息接收函数
{
	SOCKET sock = ((recvparam*)mlpvpid)->sock;
	HWND hwnd = ((recvparam*)mlpvpid)->hwnd;
	char name[20] = { '\0' };//接收发送消息的人
	char temp[255] = { '\0' };//接收发送的消息
	char temp1[235] = { '\0' };
	int retavl;
	while (true)
	{
		retavl = recv(sock, temp, sizeof(temp), 0);
		if (retavl== SOCKET_ERROR)
		{
			break;
		}
		else
		{
			strncpy(name, temp, 20);
			strcpy(temp1, temp + 20);
			::PostMessage(hwnd, M_RECVDATA, (WPARAM)name, (LPARAM)temp1);
		}
	}
	return 0;
}
BOOL message::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	recvparam *myrecvparam = new recvparam;
	myrecvparam->sock = theApp.msocket;
	myrecvparam->hwnd = m_hWnd;
	HANDLE mThread = CreateThread(NULL, 0, Threadpro, (LPVOID)myrecvparam, 0, NULL);
	CloseHandle(mThread);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



/*
1 加入群聊
2 发送给用户
3 发送群组消息
4 创建群聊
*/

void message::OnBnClickedButton2()//发送给用户
{
	CString  goal, strsend;
	char   temp[255] = { '\0' };
	temp[0] = { '2' };
	GetDlgItem(IDC_EDIT5)->GetWindowText(goal);//拷贝客户名称
	strcpy(temp+1, CW2A(goal));
	GetDlgItem(IDC_EDIT2)->GetWindowText(strsend);//拷贝信息
	strcat(temp+21, CW2A(strsend));
	send(theApp.msocket, temp, sizeof(temp), 0);//发送信息
	// TODO: 在此添加控件通知处理程序代码
}



void message::OnBnClickedButton3()//群发群组消息
{
	CString  goal, strsend;
	char  temp[255] = { '\0' };
	temp[0] = { '3' };
	GetDlgItem(IDC_EDIT5)->GetWindowText(goal);//拷贝群组号码,群号限定3位
	strcpy(temp+1, CW2A(goal));
	GetDlgItem(IDC_EDIT2)->GetWindowText(strsend);//拷贝信息
	strcat(temp+4, CW2A(strsend));
	send(theApp.msocket, temp, sizeof(temp), 0);//发送信息
	// TODO: 在此添加控件通知处理程序代码
}

void message::OnBnClickedButton4()//创建群聊
{
	char temp[255] = { '\0' };
	temp[0] = { '4' };
	send(theApp.msocket, temp, sizeof(temp), 0);//发送创建群组的命令
	// TODO: 在此添加控件通知处理程序代码
}


void message::OnBnClickedButton1()//加入群聊
{
	CString  goal;
	char  nam[10] = {'\0'};
	nam[0] = { '1' };
	GetDlgItem(IDC_EDIT5)->GetWindowText(goal);//拷贝群聊号码
	strcpy(nam+1, CW2A(goal));
	send(theApp.msocket, nam, sizeof(nam), 0);//发送目标群组号
	// TODO: 在此添加控件通知处理程序代码
}




void message::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	char temp[50] = { '\0' };
	temp[0] = { '5' };
	send(theApp.msocket, temp, sizeof(temp), 0);//发送关闭线程的命令
	CDialogEx::OnClose();
}
