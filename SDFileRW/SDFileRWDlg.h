// SDFileRWDlg.h : 头文件
//

#pragma once

// CSDFileRWDlg 对话框
class CSDFileRWDlg : public CDialog
{
// 构造
public:
	CSDFileRWDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SDFILERW_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	CString m_strFileName;
	CString m_write;
	CString m_strDisp;
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnWrite();
	afx_msg void OnBnClickedBtnRead();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnClearo();
};
