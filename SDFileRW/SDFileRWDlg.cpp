// SDFileRWDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SDFileRW.h"
#include "SDFileRWDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSDFileRWDlg 对话框

CSDFileRWDlg::CSDFileRWDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSDFileRWDlg::IDD, pParent)
	, m_strFileName(_T(""))
	, m_write(_T(""))
	, m_strDisp(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSDFileRWDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_FILENAME, m_strFileName);
	DDX_Text(pDX, IDC_EDT_WRITE, m_write);
	DDX_Text(pDX, IDC_EDT_READ, m_strDisp);
}

BEGIN_MESSAGE_MAP(CSDFileRWDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_OPEN, &CSDFileRWDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_WRITE, &CSDFileRWDlg::OnBnClickedBtnWrite)
	ON_BN_CLICKED(IDC_BTN_READ, &CSDFileRWDlg::OnBnClickedBtnRead)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CSDFileRWDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CSDFileRWDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_CLEARO, &CSDFileRWDlg::OnBnClickedBtnClearo)
END_MESSAGE_MAP()


// CSDFileRWDlg 消息处理程序

HANDLE hFile = INVALID_HANDLE_VALUE;	/* 文件句柄 */

BOOL CSDFileRWDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_strFileName = "\\SDMEM\\"; // 文件名
    UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CSDFileRWDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_SDFILERW_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_SDFILERW_DIALOG));
	}
}
#endif

//打开/创建文件
void CSDFileRWDlg::OnBnClickedBtnOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filename = _T("");
	UpdateData(TRUE);

	if (m_strFileName == "")
	{
		MessageBox(_T("请输入文件名!"));
		return;
	}
	
	// 取得文件名及路径
	filename = m_strFileName;

	// 创建一个文件或打开一个文件
	hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, 
					   NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("在SD/MMC卡上创建文件失败!"));
		return;
	}

	MessageBox(_T("成功创建/打开文件:") + filename);
}
//写入文件
void CSDFileRWDlg::OnBnClickedBtnWrite()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD len,actlen;
	char *pcharbuff;

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("文件未打开!"));
		return;	
	}

	UpdateData(TRUE);
	len = m_write.GetLength();							/* 取得输入字符串的长度 */

	pcharbuff = new char[len];
	LPTSTR pStr = m_write.GetBuffer(len);					/* 取得输入字符串的缓冲区指针 */

	// 将 Unicode 的 CString 转换为 char 型
	WideCharToMultiByte(CP_ACP, 0, pStr, len, pcharbuff, len, NULL, NULL);
	m_write.ReleaseBuffer();

	BOOL ret = SetFilePointer(hFile, 0, NULL, FILE_END);	/* 移动文件指针到文件开头 */
	if (ret == 0xFFFFFFFF)
	{
		MessageBox(_T("将文件指针移至文件开头失败!"));
		delete[] pcharbuff;
		return;	
	}

	ret = WriteFile(hFile, pcharbuff, len, &actlen, NULL);	/* 将数据写入文件中 */
	if (ret == TRUE)
		MessageBox(_T("写文件成功!"));	
	else
		MessageBox(_T("写文件失败!"));	
	
	if (pcharbuff != NULL) 
		delete[] pcharbuff;
}
//读取文件
void CSDFileRWDlg::OnBnClickedBtnRead()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD filelen,actlen;
	char *pcharbuff;

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("文件未打开!"));
		return;	
	}
	
	filelen = GetFileSize(hFile, NULL);							/* 获取文件大小 */
	if (filelen == 0xFFFFFFFF)
	{
		MessageBox(_T("获取文件大小失败!"));
		return;	
	}

	BOOL ret = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);		/* 移动文件指针到文件开头 */	
	if (ret == 0xFFFFFFFF)
	{
		MessageBox(_T("将文件指针移至文件开头失败!"));
		return;	
	}

	pcharbuff = new char[filelen];
	ret = ReadFile(hFile, pcharbuff, filelen, &actlen, NULL);	/* 从文件中读出数据 */
	if (ret == TRUE)
	{
		LPTSTR pStr = m_strDisp.GetBuffer(filelen);	
		// 将字节转化为 Unicode 字符串
		MultiByteToWideChar(CP_ACP, 0, pcharbuff, filelen, pStr, filelen);
		m_strDisp.ReleaseBuffer();
		UpdateData(FALSE);										/* 将读出的数据显示出来 */
		MessageBox(_T("读文件成功!"));	
	}
	else
	{
		UpdateData(FALSE);
		MessageBox(_T("读文件失败!"));	
	}

	if (pcharbuff != NULL) 
		delete[] pcharbuff;
}
//关闭文件句柄
void CSDFileRWDlg::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(_T("文件未打开!"));
		return;	
	}
	CloseHandle(hFile);
}
//清空“写入”编辑框
void CSDFileRWDlg::OnBnClickedBtnClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_write = _T("");
	UpdateData(false);
}
//清空“读取”编辑框
void CSDFileRWDlg::OnBnClickedBtnClearo()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strDisp = _T("");
	UpdateData(false);
}
