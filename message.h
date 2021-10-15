#pragma once
#define M_RECVDATA (WM_USER+100)
struct recvparam {
	SOCKET sock;
	HWND hwnd;
};

// message 对话框

class message : public CDialogEx
{
	DECLARE_DYNAMIC(message)

public:
	message(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~message();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT MRECVDATA(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	static DWORD WINAPI Threadpro(LPVOID mlpvpid);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
};
