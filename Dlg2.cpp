// Dlg2.cpp: 实现文件
//

#include "stdafx.h"
#include "client.h"
#include "Dlg2.h"
#include "afxdialogex.h"
#include"message.h"

// Dlg2 对话框

IMPLEMENT_DYNAMIC(Dlg2, CDialogEx)

Dlg2::Dlg2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

Dlg2::~Dlg2()
{
}

void Dlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dlg2, CDialogEx)
	ON_BN_CLICKED(IDOK, &Dlg2::OnBnClickedOk)
END_MESSAGE_MAP()


// Dlg2 消息处理程序


void Dlg2::OnBnClickedOk()
{
	char temp[11] = { '\0' };
	CString strEDIT1, strEDIT2;
	GetDlgItem(IDC_EDIT1)->GetWindowText(strEDIT1);//获取用户名
	GetDlgItem(IDC_EDIT2)->GetWindowText(strEDIT2);//获取密码
	char name[20], password[15];
	strcpy(name, CW2A(strEDIT1));
	strcpy(password, CW2A(strEDIT2));
	send(theApp.msocket,name, sizeof(name), 0);
	send(theApp.msocket,password, sizeof(password), 0);
	recv(theApp.msocket, temp, sizeof(temp), 0);
	MessageBoxA(NULL, temp, "ID号", MB_OK);
	message mes;
	mes.DoModal();
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
