
// PdbConfigDlg.h: 头文件
//

#pragma once


// CPdbConfigDlg 对话框
class CPdbConfigDlg : public CDialogEx
{
// 构造
public:
	CPdbConfigDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PDBCONFIG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	afx_msg void OnBnClickedBtnSet();
	afx_msg void OnBnClickedButtonChoose();

	void InitPdbServerTxt();
	// Pdb本地路径
	CString _pdb_path;
	// pdb服务器
	CComboBox _pdb_server;
};
