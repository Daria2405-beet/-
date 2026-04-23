
// ConfidentialDetectorDialogDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "ConfidentialDetectorDialog.h"
#include "ConfidentialDetectorDialogDlg.h"
#include "afxdialogex.h"
#include <regex>
#include <string>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonScan();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CAboutDlg::OnBnClickedButtonScan)
END_MESSAGE_MAP()


// Диалоговое окно CConfidentialDetectorDialogDlg



CConfidentialDetectorDialogDlg::CConfidentialDetectorDialogDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CONFIDENTIALDETECTORDIALOG_DIALOG, pParent)
	, m_strInputText(_T(""))
	, m_strResultText(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConfidentialDetectorDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_strInputText);
	DDX_Text(pDX, IDC_EDIT_RESULT, m_strResultText);
}

BEGIN_MESSAGE_MAP(CConfidentialDetectorDialogDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// Обработчики сообщений CConfidentialDetectorDialogDlg

BOOL CConfidentialDetectorDialogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
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

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CConfidentialDetectorDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CConfidentialDetectorDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CConfidentialDetectorDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAboutDlg::OnBnClickedButtonScan()
{
    // Обновляем переменную m_strInputText из поля ввода
    UpdateData(TRUE);

    // Проверка на пустой ввод
    if (m_strInputText.IsEmpty())
    {
        m_strResultText = L"❌ Ошибка: Введите текст для анализа!";
        UpdateData(FALSE);
        AfxMessageBox(L"Пожалуйста, введите текст для сканирования.",
            MB_ICONWARNING | MB_OK, L"Пустой ввод");
        return;
    }

    // Выполняем сканирование
    m_strResultText = ScanForConfidentialData(m_strInputText);

    // Обновляем поле результатов
    UpdateData(FALSE);

    // Выводим дополнительное уведомление, если найдены данные
    if (m_strResultText.Find(L"НЕ найдено") == -1 &&
        m_strResultText.Find(L"не найдено") == -1)
    {
        // Звуковой сигнал или уведомление (опционально)
        MessageBeep(MB_ICONEXCLAMATION);
    }
}

// Вспомогательный метод для добавления строки в результат (если нужно)
void CConfidentialDetectorDialogDlg::AddToResult(const CString& strMessage)
{
    UpdateData(TRUE);
    m_strResultText += strMessage;
    UpdateData(FALSE);
}

CString CConfidentialDetectorDialogDlg::ScanForConfidentialData(const CString& strText)
{
    CString strResults;

    // Конвертируем CString в std::string для работы regex
    CT2A pszConverted(strText);
    std::string sText(pszConverted);

    // ===== Паттерны для поиска =====

    // 1. Кредитные карты (4 группы по 4 цифры, разделенные пробелами или дефисами)
    // Примеры: 4111 1111 1111 1111, 4111-1111-1111-1111
    std::regex card_regex(R"(\b(?:\d{4}[- ]?){3}\d{4}\b)");

    // 2. Паспорт РФ (серия и номер: 45 06 123456 или 4506123456)
    // Формат: 2 цифры, пробел/ничего, 2 цифры, пробел/ничего, 6 цифр
    std::regex passport_regex(R"(\b\d{2}[ ]?\d{2}[ ]?\d{6}\b)");

    // 3. Номера договоров (различные форматы)
    // Примеры: ДОГ-12345, Договор № 123/45, №98765, ДОГ12345
    std::regex contract_regex(R"((ДОГ|Договор|№)\s*[-:№\s]*\d{2,8}[-/]?\d{0,6})", std::regex::icase);

    // 4. Дополнительно: Номера телефонов (как пример конфиденциальных данных)
    std::regex phone_regex(R"(\+?7[- ]?\(?\d{3}\)?[- ]?\d{3}[- ]?\d{2}[- ]?\d{2})");

    // ===== Поиск и формирование отчета =====
    std::smatch matches;
    std::vector<std::string> foundItems;

    // Поиск кредитных карт
    if (std::regex_search(sText, matches, card_regex)) {
        CString strTemp;
        strTemp.Format(L"⚠️ КРЕДИТНАЯ КАРТА: %S\n", matches.str(0).c_str());
        strResults += strTemp;
    }

    // Поиск всех карт (если их несколько)
    std::sregex_iterator card_iter(sText.begin(), sText.end(), card_regex);
    std::sregex_iterator end;
    int card_count = 0;
    while (card_iter != end) {
        card_count++;
        CString strTemp;
        strTemp.Format(L"   Карта #%d: %S\n", card_count, (*card_iter).str().c_str());
        strResults += strTemp;
        ++card_iter;
    }

    // Поиск паспортов
    std::sregex_iterator pass_iter(sText.begin(), sText.end(), passport_regex);
    int pass_count = 0;
    while (pass_iter != end) {
        pass_count++;
        CString strTemp;
        strTemp.Format(L"🆔 ПАСПОРТ #%d: %S\n", pass_count, (*pass_iter).str().c_str());
        strResults += strTemp;
        ++pass_iter;
    }

    // Поиск договоров
    std::sregex_iterator contract_iter(sText.begin(), sText.end(), contract_regex);
    int contract_count = 0;
    while (contract_iter != end) {
        contract_count++;
        CString strTemp;
        strTemp.Format(L"📄 ДОГОВОР #%d: %S\n", contract_count, (*contract_iter).str().c_str());
        strResults += strTemp;
        ++contract_iter;
    }

    // Поиск телефонов
    std::sregex_iterator phone_iter(sText.begin(), sText.end(), phone_regex);
    int phone_count = 0;
    while (phone_iter != end) {
        phone_count++;
        CString strTemp;
        strTemp.Format(L"📞 ТЕЛЕФОН #%d: %S\n", phone_count, (*phone_iter).str().c_str());
        strResults += strTemp;
        ++phone_iter;
    }

    // Формирование итогового отчета
    CString strSummary;
    if (strResults.IsEmpty()) {
        strSummary = L"✅ Конфиденциальных данных не найдено.\n";
    }
    else {
        strSummary.Format(L"🔍 НАЙДЕНО УЯЗВИМОСТЕЙ:\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        strResults = strSummary + strResults;
        strResults += L"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

        // Статистика
        CString strStats;
        strStats.Format(L"\n📊 Статистика: Карт: %d, Паспортов: %d, Договоров: %d, Телефонов: %d\n",
            card_count, pass_count, contract_count, phone_count);
        strResults += strStats;
        strResults += L"⚠️ Рекомендуется удалить или замаскировать эти данные!";
    }

    return strResults;
}