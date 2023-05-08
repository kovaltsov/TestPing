
// TestPingDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "TestPing.h"
#include "TestPingDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <boost/process.hpp>
#include <boost/process/windows.hpp>
#include <boost/locale.hpp>

namespace bp = ::boost::process;
namespace bl = ::boost::locale::conv;

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


// Диалоговое окно CTestPingDlg



CTestPingDlg::CTestPingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTPING_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestPingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_PingText);
}

BEGIN_MESSAGE_MAP(CTestPingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTestPingDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// Обработчики сообщений CTestPingDlg

BOOL CTestPingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

void CTestPingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CTestPingDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CTestPingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestPingDlg::OnBnClickedButton1()
{
	//setlocale(LC_ALL, "Russian");

	//// TODO: добавьте свой код обработчика уведомлений
	//m_PingText.SetWindowTextA("Hello, World!");
	//char dir[255];
	//GetCurrentDirectoryA(255, dir);

	//auto ret = system("C:\\Windows\\System32\\ping.exe yandex.ru >D:\\Temp\\ping_yandex.txt");
	//m_PingText.SetWindowTextA("Hello");

	//std::string line, full;
	//std::ifstream in("D:\\Temp\\ping_yandex.txt"); // окрываем файл для чтения
	//if (in.is_open())
	//{
	//	while (std::getline(in, line))
	//	{
	//		full += line;
	//	}
	//}
	//m_PingText.SetWindowTextA(full.c_str());

	bp::ipstream pipe_stream;
	bp::child c("ping ya.ru", bp::std_out > pipe_stream);

	std::string line, rez;

	while (pipe_stream && std::getline(pipe_stream, line) && !line.empty())
		rez += line;

	boost::locale::generator generator;
	std::locale locale = generator.generate("ru-RU.cp1251");

	std::string str = rez;

	for (size_t i = 0; i < str.size(); ++i)
	{
		if ((unsigned char)str[i] == 0xF1) // для ё
			str[i] = 0xB8;
		else if ((unsigned char)str[i] == 0xF0) // для Ё
			str[i] = 0xA8;
		else if ((unsigned char)str[i] > 0x7F && (unsigned char)str[i] < 0xB0)
			str[i] += 0x40;
		else if ((unsigned char)str[i] > 0xDF && (unsigned char)str[i] < 0xF0)
			str[i] += 0x10;
		if (str[i] == '\r') str.insert(i+1, "\n");
	}
	
	m_PingText.SetWindowTextA(str.c_str());
	//c.wait();
	
}
