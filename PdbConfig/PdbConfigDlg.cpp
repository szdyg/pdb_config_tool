
// PdbConfigDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PdbConfig.h"
#include "PdbConfigDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <string>
#include <codecvt>
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


// CPdbConfigDlg 对话框



CPdbConfigDlg::CPdbConfigDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_PDBCONFIG_DIALOG, pParent)
    , _pdb_path(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPdbConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_PDBPATH, _pdb_path);
    DDX_Control(pDX, IDC_COMBO_PDBSERVER, _pdb_server);
}

BEGIN_MESSAGE_MAP(CPdbConfigDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(ID_BTN_SET, &CPdbConfigDlg::OnBnClickedBtnSet)
    ON_BN_CLICKED(IDC_BUTTON_CHOOSE, &CPdbConfigDlg::OnBnClickedButtonChoose)
END_MESSAGE_MAP()


// CPdbConfigDlg 消息处理程序

BOOL CPdbConfigDlg::OnInitDialog()
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

    CenterWindow();
    ShowWindow(SW_SHOW);

    // TODO: 在此添加额外的初始化代码
    _pdb_path = L"C:\\symbols";
    InitPdbServerTxt();
    UpdateData(FALSE);
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPdbConfigDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPdbConfigDlg::OnPaint()
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
HCURSOR CPdbConfigDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



void CPdbConfigDlg::OnBnClickedBtnSet()
{
    UpdateData(TRUE);
    CString pdb_path; GetDlgItemText(IDC_COMBO_PDBSERVER, pdb_path);
    if (pdb_path.IsEmpty() || _pdb_path.IsEmpty())
    {
        MessageBox(L"请输入信息", L"错误", MB_OK);
        return;
    }

    // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Environment
    // _NT_SYMBOL_PATH

    CString reg_value = L"srv*" + _pdb_path + L"*" + pdb_path;
    do
    {

        LSTATUS reg_ret = RegSetKeyValue(HKEY_LOCAL_MACHINE,
            L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment",
            L"_NT_SYMBOL_PATH",
            REG_SZ,
            reg_value.GetBuffer(),
            (reg_value.GetLength() + 1) * sizeof(WCHAR));

        if (reg_ret == ERROR_SUCCESS)
        {
            MessageBox(L"设置成功", L"提示", MB_OK);
        }
        else
        {
            CString err; err.Format(L"设置失败,错误：%d", reg_ret);
            MessageBox(err, L"提示", MB_OK);
        }

    } while (false);

}


void CPdbConfigDlg::OnBnClickedButtonChoose()
{

    WCHAR select_dir[MAX_PATH];     //存放选择的目录路径 
    CString str;

    ZeroMemory(select_dir, sizeof(select_dir));

    BROWSEINFO bi;
    bi.hwndOwner = m_hWnd;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = select_dir;
    bi.lpszTitle = L"请选择保存Pdb的目录：";
    bi.ulFlags = 0;
    bi.lpfn = NULL;
    bi.lParam = 0;
    bi.iImage = 0;

    //弹出选择目录对话框
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);
    if (lp && SHGetPathFromIDList(lp, select_dir))
    {
        _pdb_path = select_dir;
    }
    UpdateData(FALSE);
}

void CPdbConfigDlg::InitPdbServerTxt()
{
    std::fstream pdb_text_read;
    std::fstream pdb_text_write;
    pdb_text_read.open("pdb_config.txt", std::fstream::in);
    if (pdb_text_read.good())
    {
        while (pdb_text_read.peek() != EOF)
        {
            std::string line;
            std::getline(pdb_text_read, line);
            if (!line.empty())
            {
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::wstring wline = converter.from_bytes(line);
                _pdb_server.AddString(wline.c_str());
            }
        }

        _pdb_server.SetCurSel(_pdb_server.GetCount() - 1);
    }
    else
    {

        pdb_text_write.open("pdb_config.txt", std::fstream::app);
        pdb_text_write << "http://msdl.szdyg.icu/download/symbols" << std::endl;
        pdb_text_write << "http://msdl.microsoft.com/download/symbols" << std::endl;
        _pdb_server.AddString(L"http://msdl.szdyg.icu/download/symbols");
        _pdb_server.AddString(L"http://msdl.microsoft.com/download/symbols");
        _pdb_server.SetCurSel(_pdb_server.GetCount() - 1);
    }
    pdb_text_read.close();
    pdb_text_write.close();
}
