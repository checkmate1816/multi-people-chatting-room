
// clientDlg.h: 头文件
//

#pragma once


// CclientDlg 对话框
class CclientDlg : public CDialogEx
{
// 构造
public:
	CclientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	bool InitSocket();//套接字函数

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
	afx_msg void OnBnClickedButton1();//登陆事件处理函数
//private:
	//SOCKET	msocket;//客户端套接字
public:
	afx_msg void OnBnClickedButton2();//创建账户事件处理函数
public:
	int flag;
	afx_msg void OnClose();
};
