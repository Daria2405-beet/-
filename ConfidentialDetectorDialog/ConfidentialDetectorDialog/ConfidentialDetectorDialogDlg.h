// ConfidentialDetectorDialogDlg.h
#pragma once

#include "afxwin.h"

class CConfidentialDetectorDialogDlg : public CDialogEx
{
    // Construction
public:
    CConfidentialDetectorDialogDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_CONFIDENTIALDETECTORDIALOG_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

protected:
    HICON m_hIcon;

    // Объявления сгенерированных функций
    afx_msg void OnBnClickedButtonScan();  // <-- ЭТА СТРОКА ДОЛЖНА БЫТЬ
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

public:
    // ДОБАВЬТЕ ЭТИ СТРОКИ ВРУЧНУЮ:
    CString m_strInputText;    // Переменная для ввода
    CString m_strResultText;   // Переменная для результатов

    CString ScanForConfidentialData(const CString& strText);  // Объявление метода
};