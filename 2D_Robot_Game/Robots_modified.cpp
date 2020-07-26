
// Robot world
// Ersin Karabudak, 2003

#include "Robots_Modified.h"
#include "MiniFW_Modified.h"
#include "randgen.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define DESIGNER 0
#define STRSIZE 200//changed
#define INPUTBOX_WIDTH 400
#define INPUTBOX_HEIGHT 125

////////////////////////////////////////////////////////////////////////////////
// RobotWorld
////////////////////////////////////////////////////////////////////////////////

static
char* CopyString (char *p)
{
	int n = strlen(p);
	char *q = new char [n + 1];
	strcpy_s(q, n + 1, p);//changed
	return q;
}

////

struct CellInfo
{
	int xPos;
	int yPos;
	int count;

	bool Match (int x, int y);
	void Print (ostream &file);

	CellInfo *next;
};

bool CellInfo::Match (int x, int y)
{
	return x == xPos && y == yPos;
}

void CellInfo::Print (ostream &f)
{
	if (count > 0)
		f << "c " << xPos << " " << yPos << " " << count << endl;
}

////

struct WallInfo
{
	int xPos;
	int yPos;
	Direction direction;
	bool wall;

	bool Match (int x, int y, Direction dir);
	void Print (ostream &file);

	WallInfo *next;
};

bool WallInfo::Match (int x, int y, Direction dir)
{
	static Direction opposite [] = { west, east, south, north };
	static int dx [] = { 1, -1,  0,  0 };
	static int dy [] = { 0,  0,  1, -1 };

	if (dir == direction && x == xPos && y == yPos)
		return true;
	if (opposite[dir] == direction && x + dx[dir] == xPos && y + dy[dir] == yPos)
		return true;
	return false;
}

void WallInfo::Print (ostream &f)
{
	char* dirStr [] = { "e", "w", "n", "s" };
	if (wall)
		f << dirStr[direction] << " " << xPos << " " << yPos << endl;
}

////

struct RobotWorld
{
	RobotWorld ();
	RobotWorld (char *file);
	~RobotWorld ();

	void Save ();
	void SaveAs (char *file);
	void ClearAll ();
	void Reload ();

	int GetCount (int x, int y);
	int SetCount (int x, int y, int n);
	int IncrCount (int x, int y, int n = 1);
	int DecrCount (int x, int y, int n = 1);

	bool GetWall (int x, int y, Direction dir);
	bool SetWall (int x, int y, Direction dir);
	bool ClearWall (int x, int y, Direction dir);
	bool ToggleWall (int x, int y, Direction dir);

	CellInfo* FindCell (int x, int y, bool add);
	WallInfo* FindWall (int x, int y, Direction dir, bool add);

	char* DisplayName ();

	CellInfo *cells;
	WallInfo *walls;

	bool dirty;
	char *fileName;
};


static RobotWorld *theRobotWorld = 0;


RobotWorld::RobotWorld ()
{
	cells = 0;
	walls = 0;
	dirty = false;
	fileName = 0;
}

RobotWorld::RobotWorld (char *file)
{
	cells = 0;
	walls = 0;
	dirty = false;
	fileName = CopyString(file);
	Reload();
}

RobotWorld::~RobotWorld ()
{
	ClearAll();
	delete[] fileName;
}

void RobotWorld::Save ()
{
	if (fileName == 0)
		return;

	ofstream f(fileName);

	CellInfo *cip = cells;
	while (cip)
	{
		cip->Print(f);
		cip = cip->next;
	}

	WallInfo *wip = walls;
	while (wip)
	{
		wip->Print(f);
		wip = wip->next;
	}

	f.close();
	dirty = false;
}

void RobotWorld::SaveAs (char *name)
{
	delete[] fileName;
	fileName = CopyString(name);
	Save();
}

void RobotWorld::ClearAll ()
{
	while (cells)
	{
		CellInfo *q = cells;
		cells = q->next;
		delete q;
	}
	while (walls)
	{
		WallInfo *q = walls;
		walls = q->next;
		delete q;
	}
}

void RobotWorld::Reload ()
{
	ClearAll();

	ifstream f(fileName);

	char cm;
	while (f >> cm)
	{
		int x = 0;
		int y = 0;
		f >> x >> y;
		if (cm == 'c')
		{
			int n = 0;
			f >> n;
			SetCount(x, y, n);
		}
		else
		{
			if (cm == 'e')
				SetWall(x, y, east);
			else if (cm == 'w')
				SetWall(x, y, west);
			else if (cm == 'n')
				SetWall(x, y, north);
			else if (cm == 's')
				SetWall(x, y, south);
		}
	}

	f.close();
	dirty = false;
}

int RobotWorld::GetCount (int x, int y)
{
	CellInfo *p = FindCell(x, y, false);
	return p ? p->count : 0;
}

int RobotWorld::SetCount (int x, int y, int n)
{
	if (n < 0)
		n = 0;
	CellInfo *p = FindCell(x, y, true);
	dirty = (p->count != n);
	return p->count = n;
}

int RobotWorld::IncrCount (int x, int y, int n)
{
	CellInfo *p = FindCell(x, y, true);
	n = p->count + n;
	if (n < 0)
		n = 0;
	dirty = (p->count != n);
	return p->count = n;
}

int RobotWorld::DecrCount (int x, int y, int n)
{
	CellInfo *p = FindCell(x, y, true);
	n = p->count - n;
	if (n < 0)
		n = 0;
	dirty = (p->count != n);
	return p->count = n;
}

bool RobotWorld::GetWall (int x, int y, Direction dir)
{
	WallInfo *p = FindWall(x, y, dir, false);
	return p ? p->wall : false;
}

bool RobotWorld::SetWall (int x, int y, Direction dir)
{
	WallInfo *p = FindWall(x, y, dir, true);
	dirty = (p->wall != true);
	return p->wall = true;
}

bool RobotWorld::ClearWall (int x, int y, Direction dir)
{
	WallInfo *p = FindWall(x, y, dir, true);
	dirty = (p->wall != false);
	return p->wall = false;
}

bool RobotWorld::ToggleWall (int x, int y, Direction dir)
{
	WallInfo *p = FindWall(x, y, dir, true);
	dirty = true;
	return p->wall = !p->wall;
}

CellInfo* RobotWorld::FindCell (int x, int y, bool add)
{
	CellInfo *p = cells;
	while (p)
	{
		if (p->Match(x, y))
			return p;
		p = p->next;
	}
	if (!add)
		return 0;
	p = new CellInfo;
	p->xPos = x;
	p->yPos = y;
	p->count = 0;
	p->next = cells;
	cells = p;
	return p;
}

WallInfo* RobotWorld::FindWall (int x, int y, Direction dir, bool add)
{
	WallInfo *p = walls;
	while (p)
	{
		if (p->Match(x, y, dir))
			return p;
		p = p->next;
	}
	if (!add)
		return 0;
	p = new WallInfo;
	p->xPos = x;
	p->yPos = y;
	p->direction = dir;
	p->wall = false;
	p->next = walls;
	walls = p;
	return p;
}

char* RobotWorld::DisplayName ()
{
	return fileName ? fileName : "Untitled";
}





// InputBox Class : Begin


class CInputBox
{
    static HFONT m_hFont;
    static HWND  m_hWndInputBox;
    static HWND  m_hWndParent;
    static HWND  m_hWndEdit;
    static HWND  m_hWndOK;
    static HWND  m_hWndCancel;
    static HWND  m_hWndPrompt;

    static HINSTANCE m_hInst;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
    // text from InputBox
	LPTSTR Text;
    BOOL DoModal(LPCTSTR szCaption, LPCTSTR szPrompt);

	CInputBox(HWND hWndParent);
	virtual ~CInputBox();

};


HFONT CInputBox::m_hFont = NULL;
HWND  CInputBox::m_hWndInputBox = NULL;
HWND  CInputBox::m_hWndParent = NULL;
HWND  CInputBox::m_hWndEdit = NULL;
HWND  CInputBox::m_hWndOK = NULL;
HWND  CInputBox::m_hWndCancel = NULL;
HWND  CInputBox::m_hWndPrompt = NULL;

HINSTANCE CInputBox::m_hInst = NULL;


//////////////////////////////////////////////////////////////////////
// CInputBox Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
Author      : mah
Date        : 13.06.2002
Description :
    Constructs window class InputBox
*/
CInputBox::CInputBox(HWND hWndParent)
{
	HINSTANCE hInst = GetModuleHandle(NULL);

	WNDCLASSEX wcex;

	if (!GetClassInfoEx(hInst, "InputBox", &wcex))
	{
		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= (WNDPROC)WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInst;
		wcex.hIcon			= NULL;//LoadIcon(hInst, (LPCTSTR)IDI_MYINPUTBOX);
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= "InputBox";
		wcex.hIconSm		= NULL;

		if (RegisterClassEx(&wcex) == 0)
			MessageBox(NULL, "Can't create CInputBox!", "Error", MB_OK);
	}

    m_hWndParent = hWndParent;

    Text = NULL;

}

CInputBox::~CInputBox()
{
    if (Text) delete[] Text;
}

/*
Author      : mah
Date        : 13.06.2002
Description : Window procedure
*/
LRESULT CALLBACK CInputBox::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LOGFONT lfont;

	switch (message)
	{
		case WM_CREATE:
            // font
            memset(&lfont, 0, sizeof(lfont));
           // lstrcpy(lfont.lfFaceName, _T("Arial"));
            lfont.lfHeight = 16;
            lfont.lfWeight = FW_NORMAL;//FW_BOLD;
            lfont.lfItalic = FALSE;
            lfont.lfCharSet = DEFAULT_CHARSET;
            lfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
            lfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
            lfont.lfQuality = DEFAULT_QUALITY;
            lfont.lfPitchAndFamily = DEFAULT_PITCH;
	        m_hFont = CreateFontIndirect(&lfont);

	        m_hInst = GetModuleHandle(NULL);

			// creating Edit
			m_hWndEdit = CreateWindowEx(WS_EX_STATICEDGE,
				"edit","",
				WS_VISIBLE | WS_CHILD  | WS_TABSTOP | ES_AUTOHSCROLL,
				5, INPUTBOX_HEIGHT - 50, INPUTBOX_WIDTH - 16, 20,
				hWnd,
				NULL,
				m_hInst,
				NULL);

            // setting font
			SendMessage(m_hWndEdit, WM_SETFONT, (WPARAM)m_hFont, 0);

            // button OK
			m_hWndOK = CreateWindowEx(WS_EX_STATICEDGE,
				"button","OK",
				WS_VISIBLE | WS_CHILD | WS_TABSTOP,
				INPUTBOX_WIDTH - 100, 10, 90, 25,
				hWnd,
				NULL,
				m_hInst,
				NULL);

            // setting font
            SendMessage(m_hWndOK, WM_SETFONT, (WPARAM)m_hFont, 0);

            // button Cancel
			m_hWndCancel = CreateWindowEx(WS_EX_STATICEDGE,
				"button","Cancel",
				WS_VISIBLE | WS_CHILD | WS_TABSTOP,
				INPUTBOX_WIDTH - 100, 40, 90, 25,
				hWnd,
				NULL,
				m_hInst,
				NULL);

            // setting font
            SendMessage(m_hWndCancel, WM_SETFONT, (WPARAM)m_hFont, 0);

            // static Propmpt
			m_hWndPrompt = CreateWindowEx(WS_EX_STATICEDGE,
				"static","",
				WS_VISIBLE | WS_CHILD,
				5, 10, INPUTBOX_WIDTH - 110, INPUTBOX_HEIGHT - 70,
				hWnd,
				NULL,
				m_hInst,
				NULL);

            // setting font
            SendMessage(m_hWndPrompt, WM_SETFONT, (WPARAM)m_hFont, 0);

            SetFocus(m_hWndEdit);
			break;
		case WM_DESTROY:

			DeleteObject(m_hFont);


			EnableWindow(m_hWndParent, TRUE);
			SetForegroundWindow(m_hWndParent);
			DestroyWindow(hWnd);
			PostQuitMessage(0);

			break;
        case WM_COMMAND:
            switch (HIWORD(wParam))
            {
                case BN_CLICKED:
                    if ((HWND)lParam == m_hWndOK)
                        PostMessage(m_hWndInputBox, WM_KEYDOWN, VK_RETURN, 0);
                    if ((HWND)lParam == m_hWndCancel)
                        PostMessage(m_hWndInputBox, WM_KEYDOWN, VK_ESCAPE, 0);
                    break;
            }
            break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

/*
Author      : mah
Date        : 13.06.2002
Description :
        Constructs InputBox window
*/
BOOL CInputBox::DoModal(LPCTSTR szCaption, LPCTSTR szPrompt)
{
	RECT r;
	GetWindowRect(GetDesktopWindow(), &r);

	m_hWndInputBox = CreateWindowEx(WS_EX_TOOLWINDOW,
                "InputBox",
                szCaption,
                WS_POPUPWINDOW | WS_CAPTION | WS_TABSTOP,
                (r.right - INPUTBOX_WIDTH) / 2, (r.bottom - INPUTBOX_HEIGHT) / 2,
                INPUTBOX_WIDTH, INPUTBOX_HEIGHT,
                m_hWndParent,
                NULL,
                m_hInst,
                NULL);
    if(m_hWndInputBox == NULL)
        return FALSE;


    SetWindowText(m_hWndPrompt, szPrompt);

    SetForegroundWindow(m_hWndInputBox);

	EnableWindow(m_hWndParent, FALSE);

    ShowWindow(m_hWndInputBox, SW_SHOW);
    UpdateWindow(m_hWndInputBox);

    BOOL ret = 0;

	MSG msg;

    HWND hWndFocused;

    while (GetMessage(&msg, NULL, 0, 0))
    {
		if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)
            {
				SendMessage(m_hWndInputBox, WM_DESTROY, 0, 0);
                ret = 0;
            }
			if (msg.wParam == VK_RETURN)
            {
                int nCount = GetWindowTextLength(m_hWndEdit);
                nCount++;
                if (Text)
                {
                    delete[] Text;
                    Text = NULL;
                }
                Text = new TCHAR[nCount];
                GetWindowText(m_hWndEdit, Text, nCount);
			    SendMessage(m_hWndInputBox, WM_DESTROY, 0, 0);
                ret = 1;
            }
			if (msg.wParam == VK_TAB)
            {
                hWndFocused = GetFocus();
                if (hWndFocused == m_hWndEdit) SetFocus(m_hWndOK);
                if (hWndFocused == m_hWndOK) SetFocus(m_hWndCancel);
                if (hWndFocused == m_hWndCancel) SetFocus(m_hWndEdit);
            }

		}
        TranslateMessage(&msg);
		DispatchMessage(&msg);
    }

	return ret;
}

// InputBox Class : End


////////////////////////////////////////////////////////////////////////////////
// RobotWindow
////////////////////////////////////////////////////////////////////////////////


struct RobotWindow : Window
{
	RobotWindow ();
	virtual ~RobotWindow ();
	virtual void Realize (WindowCreate &q);
	virtual void Draw (Canvas &z);
	virtual void ProcessMouse (GMouseEvent &event);
	virtual void ProcessKey (GKeyEvent &event); //added by albert
	virtual int MenuCommand (int cmd);

	void CmdRun ();
	void CmdPause ();

	void ClearAll ();
	bool CloseCurrent ();
	void WorldChange ();
	bool CmdNew ();
	bool CmdOpen ();
	bool CmdSave ();
	bool CmdSaveAs ();
	bool CmdRevert ();
	bool CmdExit ();

	void MakeMenu ();

	void DrawWorld (Canvas &z);
	void DrawRobot (Canvas &z, Robot *p);
	void Invalid (Robot *p);
	void Redraw (Robot *p);
	void Redraw ();

	int GridToX (int x);
	int GridToY (int y);
	URect GridRect (int x, int y);

	HMENU menu;
	HANDLE threadHandle;
	DWORD threadId;
	static bool running;
	static bool suspended;
};


static RobotWindow *theRobotWindow = 0;
static int timeQuantum = 80;
int theKeyPressedValue=1;   //added Albert

#if DESIGNER
static char robotWindowTitle [] = "Robot World Designer";
#else
static char robotWindowTitle [] = "Robot World";
#endif

bool RobotWindow::running = false;
bool RobotWindow::suspended = false;


RobotWindow::RobotWindow ()
{
	backColor = 0xDDDDDD;
	redrawOnResize = true;
	threadHandle = 0;
	threadId = 0;
}

RobotWindow::~RobotWindow ()
{
}

void RobotWindow::Realize (WindowCreate &q)
{
	Window::Realize(q);
	MakeMenu();
	CmdNew();
}

void RobotWindow::ProcessKey (GKeyEvent &event)  //added by Albert
{
    theKeyPressedValue=event.key;
}

void RobotWindow::ProcessMouse (GMouseEvent &event)
{
	if (event.what != mouseDown)
		return;

	URect r = GetUserRect();
	int x = event.where.x;
	int y = r.bottom - event.where.y;

	bool v = (x + 4) % 32 <= 8;
	bool h = (y + 4) % 32 <= 8;
	if (v && h)
		return;

	if (v)
	{
		x = (x + 4) / 32;
		y = y / 32;
		if (x > 0)
		{
			theRobotWorld->ToggleWall(x, y, west);
			URect r = GridRect(x, y);
			Expand(r, 5, 5);
			Invalidate(r);
		}
	}
	else if (h)
	{
		x = x / 32;
		y = (y + 4) / 32;
		if (y > 0)
		{
			theRobotWorld->ToggleWall(x, y, south);
			URect r = GridRect(x, y);
			Expand(r, 5, 5);
			Invalidate(r);
		}
	}
	else
	{
		x = x / 32;
		y = y / 32;
		if (event.button == leftButton)
			theRobotWorld->IncrCount(x, y);
		else if (event.click > 1)
			theRobotWorld->SetCount(x, y, 0);
		else
			theRobotWorld->DecrCount(x, y);
		URect r = GridRect(x, y);
		Invalidate(r);
	}
}

const int rwcRun = cmdUser + 1;
const int rwcPause = cmdUser + 2;
const int rwcSlow = cmdUser + 3;
const int rwcFast = cmdUser + 4;
const int rwcClear = cmdUser + 5;

void RobotWindow::MakeMenu ()
{
	menu = CreateMenu();

	HMENU popup = CreateMenu();
	AppendMenu(popup, MF_STRING, cmdNew, "&New");
	AppendMenu(popup, MF_STRING, cmdOpen, "&Open...");
	AppendMenu(popup, MF_STRING, cmdSave, "&Save");
	AppendMenu(popup, MF_STRING, cmdSaveAs, "Save &As...");
	AppendMenu(popup, MF_STRING, cmdRevert, "&Reload");
	AppendMenu(popup, MF_SEPARATOR, 0, "");
	AppendMenu(popup, MF_STRING, cmdQuit, "E&xit");

	AppendMenu(menu, MF_POPUP, UINT(popup), "&File");

	#if !DESIGNER
	AppendMenu(menu, MF_STRING, rwcRun, "&Run");
	AppendMenu(menu, MF_STRING, rwcPause, "&Pause");
	AppendMenu(menu, MF_STRING, rwcSlow, "&Slow");
	AppendMenu(menu, MF_STRING, rwcFast, "&Fast");
	AppendMenu(menu, MF_STRING, rwcClear, "&Clear");
	#endif

	SetMenu(hwnd, menu);
}

int RobotWindow::MenuCommand (int cmd)
{
	if (cmd == rwcRun)
	{
		CmdRun();
		return 1;
	}
	if (cmd == rwcPause)
	{
		CmdPause();
		return 1;
	}
	if (cmd == rwcSlow)
	{
		if (timeQuantum < 1000)
			timeQuantum += 20;
		return 1;
	}
	if (cmd == rwcFast)
	{
		if (timeQuantum > 0)
			timeQuantum -= 20;
		return 1;
	}
	if (cmd == rwcClear)
	{
		if (!running)
			ClearAll();
		return 1;
	}
	if (cmd == cmdNew)
	{
		CmdNew();
		return 1;
	}
	if (cmd == cmdOpen)
	{
		CmdOpen();
		return 1;
	}
	if (cmd == cmdSave)
	{
		CmdSave();
		return 1;
	}
	if (cmd == cmdSaveAs)
	{
		CmdSaveAs();
		return 1;
	}
	if (cmd == cmdRevert)
	{
		CmdRevert();
		return 1;
	}
	if (cmd == cmdQuit)
	{
		CmdExit();
		return 1;
	}
	return Window::MenuCommand(cmd);
}


extern int main ();

static bool zombies = false;


DWORD WINAPI ThreadFunc (LPVOID)
{
	zombies = true;
	main();
	zombies = false;

	RobotWindow::running = false;
	UserMessage(theRobotWindow, mboxOK, "Stop", "End of execution");

	return 0;
}

void RobotWindow::CmdRun ()
{
	if (running)
	{
		if (suspended)
		{
			ResumeThread(threadHandle);
			suspended = false;
		}
	}
	else
	{
		if (threadHandle)
			CloseHandle(threadHandle);

		ClearAll();

		running = true;
		suspended = false;
		threadHandle = CreateThread(0, 0, ThreadFunc, 0, 0, &threadId);
	}
}

void RobotWindow::CmdPause ()
{
	if (running && !suspended)
	{
		SuspendThread(threadHandle);
		suspended = true;
	}
}

void RobotWindow::ClearAll ()
{
	while (Robot::list)
		delete Robot::list;
}

bool RobotWindow::CloseCurrent ()
{
	if (theRobotWorld == 0 || !theRobotWorld->dirty)
		return true;

	char str [STRSIZE];//changed
	strcpy_s(str,STRSIZE, "Do you want to save '");
	strcat_s(str,STRSIZE, theRobotWorld->DisplayName());
	strcat_s(str,STRSIZE, "'?");

	int ans = UserMessage(this, mboxYesNoCancel, "Confirm", str);
	if (ans == answerCancel)
		return false;
	if (ans == answerNo)
		return true;

	return CmdSave();
}

void RobotWindow::WorldChange ()
{
	if (!running)
		ClearAll();
	
	char str [STRSIZE];//changed
	strcpy_s(str,STRSIZE, robotWindowTitle);
	strcat_s(str,STRSIZE, " - ");
	strcat_s(str,STRSIZE, theRobotWorld->DisplayName());
	SetTitle(str);

	Invalidate();
}

bool RobotWindow::CmdNew ()
{
	if (!CloseCurrent())
		return false;

	delete theRobotWorld;
	theRobotWorld = new RobotWorld;
	WorldChange();
	return true;
}

bool RobotWindow::CmdOpen ()
{
	if (!CloseCurrent())
		return false;

	FileSpec file;
	if (!GetOpenFileSpec(file, "*.rw", this))
		return false;

	delete theRobotWorld;
	theRobotWorld = new RobotWorld(file.path);
	WorldChange();
	return true;
}

bool RobotWindow::CmdSave ()
{
	if (theRobotWorld == 0)
		return true;

	if (theRobotWorld->fileName == 0)
		return CmdSaveAs();

	theRobotWorld->Save();
	return true;
}

bool RobotWindow::CmdSaveAs ()
{
	if (theRobotWorld == 0)
		return true;

	FileSpec file;
	if (!GetSaveFileSpec(file, "*.rw", this))
		return false;

	file.SetType(".rw");
	theRobotWorld->SaveAs(file.path);

	WorldChange();
	return true;
}

bool RobotWindow::CmdRevert ()
{
	if (theRobotWorld == 0)
		return true;

	if (theRobotWorld->dirty)
	{
		int ans = UserMessage(this, mboxYesNo, "Confirm", "Do you want to discard changes?");
		if (ans == answerNo)
			return false;
	}

	theRobotWorld->Reload();
	WorldChange();
	return true;
}

bool RobotWindow::CmdExit ()
{
	if (!CloseCurrent())
		return false;

	Close();
	return true;
}


int RobotWindow::GridToX (int x)
{
	return 32*x;
}

int RobotWindow::GridToY (int y)
{
	URect r = GetUserRect();
	return r.bottom - 32*y;
}

URect RobotWindow::GridRect (int x, int y)
{
	URect r;
	r.left = GridToX(x);
	r.right = r.left + 32;
	r.bottom = GridToY(y);
	r.top = r.bottom - 32;
	return r;
}

static
char * IntToStr (int n)
{
	static char str [32];
	char *p = str + 32;
	*--p = 0;
	while (n > 0)
	{
		*--p = '0' + n % 10;
		n /= 10;
	}
	return p;
}

void RobotWindow::Draw (Canvas &z)
{
	// draw the grid
	z.SetLineWidth(0);
	z.SetLineColor(colorGray);

	for (int j = 0; j <= 100; j++)
	{
		int x = GridToX(j);
		int y = GridToY(0);
		z.SetLineColor(colorWhite);
		z.Line(UPoint(x - 1, 0), UPoint(x - 1, y));
		z.SetLineColor(colorGray);
		z.Line(UPoint(x, 0), UPoint(x, y));
	}
	for (int k = 0; k <= 100; k++)
	{
		int x = GridToX(100);
		int y = GridToY(k);
		z.SetLineColor(colorGray);
		z.Line(UPoint(0, y - 1), UPoint(x, y - 1));
		z.SetLineColor(colorWhite);
		z.Line(UPoint(0, y), UPoint(x, y));
	}

	// draw the world
	if (theRobotWorld)
		DrawWorld(z);

	// draw the robots
	Robot *p = Robot::list;
	if (p)
	{
		while (1)
		{
			DrawRobot(z, p);
			p = p->next;
			if (p == Robot::list)
				break;
		}
	}
}

void RobotWindow::DrawWorld (Canvas &z)
{
	// draw the walls
	z.SetLineColor(colorDarkGray);
	z.SetLineWidth(3);

	WallInfo *wip = theRobotWorld->walls;
	while (wip)
	{
		if (wip->wall)
		{
			URect r = GridRect(wip->xPos, wip->yPos);
			if (wip->direction == east)
				z.Line(TopRight(r), BottomRight(r));
			else if (wip->direction == west)
				z.Line(TopLeft(r), BottomLeft(r));
			else if (wip->direction == north)
				z.Line(TopLeft(r), TopRight(r));
			else
				z.Line(BottomLeft(r), BottomRight(r));
		}

		wip = wip->next;
	}

	// draw cell values
	z.SetFont("System", 9);
	z.SetTextColor(0x0000AA);

	CellInfo *cip = theRobotWorld->cells;
	while (cip)
	{
		if (cip->count)
		{
			URect r = GridRect(cip->xPos, cip->yPos);
			char *str = IntToStr(cip->count);
			z.PutText(r, str, -1, justCenter|justCenterV);
		}

		cip = cip->next;
	}
}

void RobotWindow::DrawRobot (Canvas &z, Robot *p)
{
	static UColor robotColor [] = {
		0xFFFFFF, 0x00FFFF, 0x4444FF, 0xFF7777,
		0x00AA00, 0xEE00EE, 0xAAAAFF, 0x0088FF,
	};

	if (!p->visible)
		return;

	URect r = GridRect(p->xPos, p->yPos);
	int x = (r.left + r.right)/2;
	int y = (r.top + r.bottom)/2;

	Expand(r, -3, -3);

	z.SetLineWidth(0);
	z.SetLineColor(colorBlack);
	z.SetFillColor(p->stalled ? colorGray : robotColor[p->color]);
	z.FillEllipse(r);
	z.DrawEllipse(r);

	z.SetLineWidth(3);
	z.SetLineColor(0xA0A0A0L);
	if (p->stalled)
	{
		z.Line(UPoint(x - 8, y - 8), UPoint(x + 8, y + 8));
		z.Line(UPoint(x - 8, y + 8), UPoint(x + 8, y - 8));
	}
	else if (p->direction == west)
	{
		z.Line(UPoint(x - 8, y), UPoint(x + 8, y));
		z.Line(UPoint(x - 8, y), UPoint(x, y + 5));
		z.Line(UPoint(x - 8, y), UPoint(x, y - 5));
	}
	else if (p->direction == east)
	{
		z.Line(UPoint(x + 8, y), UPoint(x - 8, y));
		z.Line(UPoint(x + 8, y), UPoint(x, y + 5));
		z.Line(UPoint(x + 8, y), UPoint(x, y - 5));
	}
	else if (p->direction == north)
	{
		z.Line(UPoint(x, y - 8), UPoint(x, y + 8));
		z.Line(UPoint(x, y - 8), UPoint(x + 5, y));
		z.Line(UPoint(x, y - 8), UPoint(x - 5, y));
	}
	else
	{
		z.Line(UPoint(x, y + 8), UPoint(x, y - 8));
		z.Line(UPoint(x, y + 8), UPoint(x + 5, y));
		z.Line(UPoint(x, y + 8), UPoint(x - 5, y));
	}
	char * thing=IntToStr(p->bag);		//////////////////////////AUNLU
	z.SetTextColor(0xB00000L);
	z.PutText(UPoint(x-8,y-8),thing);   ////////////////////////////////

}

void RobotWindow::Invalid (Robot *p)
{
	URect r = GridRect(p->xPos, p->yPos);
	Invalidate(r);
}

void RobotWindow::Redraw (Robot *p)
{
	Invalid(p);
	Refresh();
}

void RobotWindow::Redraw ()
{
	Invalidate();
	Refresh();
}


////////////////////////////////////////////////////////////////////////////////
// Robot
////////////////////////////////////////////////////////////////////////////////


static
void GetTargetPos (int x, int y, Direction dir, int &nx, int &ny)
{
	static int dx [] = { 1, -1,  0,  0 };
	static int dy [] = { 0,  0,  1, -1 };

	nx = x + dx[dir];
	ny = y + dy[dir];
}

static
void ShortDelay (Robot *p)
{
	static Robot *last = 0;
	if (last == p)
		Sleep(timeQuantum / 3);
	else
		Sleep(timeQuantum);
	last = p;
}


static
void LongDelay (Robot *p)
{
	static Robot *last = 0;
	if (last == p)
		Sleep(3*timeQuantum);
	else
		Sleep(timeQuantum);
	last = p;
}

/* Written by */
static 
void myDelay (Robot *p)
{
	static Robot *last = 0;
	if (last == p)
		Sleep(50);
	else
		Sleep(25);
	last = p;
}


static
void RobotError (char *msg)
{
	UserMessage(theRobotWindow, mboxOK, "Robot error", msg);
}

static
void Trace (char *msg)
{
}


//

Robot* Robot::list = 0;


Robot::Robot (int x, int y, Direction dir, int objects)
{
	xPos = x;
	yPos = y;
	direction = dir;
	color = yellow;
	bag = objects > 0 ? objects : 0;
	stalled = false;
	visible = true;

	if (list == 0)
		list = next = prev = this;
	else
	{
		prev = list->prev;
		next = list;
		next->prev = prev->next = this;
	}

	theRobotWindow->Invalid(this);
}

Robot::~Robot ()
{
	//**********************sinan****************
	/*if (zombies)
	{
		Robot *z = new Robot(xPos, yPos, direction, bag);
		z->color = color;
		z->stalled = false;//stalled;
	}*/

	//**********************sinan****************
	visible = false;
	stalled = true;

	theRobotWindow->Invalid(this);


	if (list == this) 
	{
		if (list->next == list) 
		{
			list = 0;
		} 
		else 
		{
			this->prev->next = this->next;
			this->next->prev = this->prev;

			list = list->next;
		}
	} 
	else 
	{
		Robot *p = list;

		for (;p->next != list; p = p->next) 
		{
			if (p == this) 
			{
				this->prev->next = this->next;
				this->next->prev = this->prev;

				break; //I am too lazy to rewrite it.
			}
		}

		if (p->next == list) 
		{
			p->prev->next = p->next;
			p->next->prev = p->prev;
		}
	}
}


//******************sinan*****************

void Robot::Move (int distance)		
{	
	Trace("Move");

	for (int i=1; i<= distance; i++)
	{
		if (stalled)
			return;

	// LongDelay(this);
	myDelay(this);

		if (Blocked())
		{
			stalled = true;
			theRobotWindow->Redraw(this);
     //		RobotError("Move while blocked");   //deleted by Albert
			if (!FacingWall())                  //added by Albert to kill the other robot too
			{
				int x, y;
				GetTargetPos(xPos, yPos, direction, x, y);
				Robot *p = next;
    			while (p != this)
				{
					if (p->xPos == x && p->yPos == y)
					{
						p->stalled=true;
						theRobotWindow->Redraw(p);
					}
					p = p->next;
				}
			}
		}
		else
		{
			visible = false;
			theRobotWindow->Invalid(this);
			GetTargetPos(xPos, yPos, direction, xPos, yPos);
			visible = true;
			theRobotWindow->Redraw(this);
		}
	}

}


bool Robot::Blocked ()
{
	if (FacingWall())
		return true;

	int x, y;
	GetTargetPos(xPos, yPos, direction, x, y);

	Robot *p = next;
	while (p != this)
	{
		if (p->xPos == x && p->yPos == y)
			return true;
		p = p->next;
	}
	return false;
}

void Robot::TurnRight ()
{
	Trace("TurnRight");

	Direction right[] = { south, north, east, west };

	if (stalled)
		return;

	ShortDelay(this);
	direction = right[direction];
	theRobotWindow->Redraw(this);
}

bool Robot::PickThing ()
{
	Trace("PickThing");

	if (stalled || theRobotWorld == 0)
		return false;

	if (theRobotWorld->GetCount(xPos, yPos) <= 0)
		return false;

	theRobotWorld->DecrCount(xPos, yPos);
	theRobotWorld->dirty = false; //XXX
	bag++;
	theRobotWindow->Redraw(this);
	return true;
}


bool Robot::PutThing ()
{
	Trace("PutThing");

	if (stalled || theRobotWorld == 0)
		return false;

	if (bag <= 0)
		return false;

	theRobotWorld->IncrCount(xPos, yPos);
	theRobotWorld->dirty = false; //XXX
	bag--;
	theRobotWindow->Redraw(this);
	return true;
}


void Robot::SetColor (Color color)
{
	if (stalled)
		return;

	this->color = color;
	theRobotWindow->Redraw(this);
	ShortDelay(this);
}


bool Robot::FacingEast ()
{
	return direction == east;
}

bool Robot::FacingWall ()
{
	if (direction == west && xPos == 0)
		return true;

	if (direction == south && yPos == 0)
		return true;

	if (theRobotWorld)
		return theRobotWorld->GetWall(xPos, yPos, direction);

	return false;
}

bool Robot::CellEmpty ()
{
	if (theRobotWorld)
		return theRobotWorld->GetCount(xPos, yPos) == 0;
	else
		return true;
}

bool Robot::BagEmpty ()
{
	return bag == 0;
}

void Robot::Teleport(int x, int y, Direction d)
{
	ShortDelay(this);

	bool kill_it=false;
	Robot *p = next;
    while (p != this)   //check all robots
	{
		if (p->xPos == x && p->yPos == y)  //if one is found in x,y
		{
			p->stalled=true;
            //kill it
			theRobotWindow->Redraw(p);
			kill_it=true;
		}
		p = p->next;
	}

	if (kill_it)  //if a robot was found in x,y
	{
		stalled = true;       //kill the object robot
		theRobotWindow->Redraw(this);
	}
	else   //otherwise beam it up Scotty!
	{
      visible = false;
	  theRobotWindow->Invalid(this);
	  xPos = x;
	  yPos = y;
	  direction = d;
	  visible = true;
	  theRobotWindow->Redraw(this);
	}
}



/**************** START: CS201 students' hw5 member functions ********************/ 
bool Robot :: IsAlive ()
{
	return !stalled;
}

int Robot :: GetBagCount ()
{
	return bag;
}

void Robot :: PickAllThings()
{
	while(!CellEmpty())
		PickThing();
}

void Robot :: Turn(Direction targetDirection)
{
	      if(!stalled)
      {
            direction = targetDirection;
            theRobotWindow->Redraw(this);
       }

}
 //*************

int Robot::GetXCoordinate() 
{
	return xPos;
}

int Robot::GetYCoordinate()
{
	return yPos;
}

void Robot::TurnFace(Direction Way) // gets the current direction
{
	direction = Way;
}

Direction Robot::GetDirection()  //returns the current direction
{
    return direction;
}

void Robot::Resurrect() //resurrects monster or players
{
  stalled= false;
}

void Robot::PutManyThings() 
{
	if((xPos == 0 && yPos == 0) || (xPos == 12 && yPos == 12) || (xPos == 0 && yPos == 12) || (xPos == 12 && yPos == 0)) {
		for(int countThing = GetCellCount(xPos, yPos); countThing < 10; countThing++){ ////puts things to corner cells up to point they have 10 of them
		PutThing();
		}
	}

}

void Robot::TurnRandom() //selects random position for the monster
{
   RandGen rand;
   int m;
   m = rand.RandInt(1,4); //random number is selected between 1 and 4 and match between one of the directions
   if(m == 1){
	   TurnFace(north);}
   if(m == 2){
	   TurnFace(east);}
   if(m == 3){
	   TurnFace(south);}
   if(m == 4){
	   TurnFace(west);}
}

/**************** END: CS201 students' hw5 member functions ********************/ 


void ShowMessage (string message)
{
	char* tempString;
	tempString = (char*) message.c_str();
	UserMessage(theRobotWindow, mboxOK, "Message", tempString);
}


void ShowMessage (int message)
{
	//works for 10 digit numbers
	char* tempString = (char *)malloc(sizeof(char*)*10);
	//tempString =
	_itoa_s(message,tempString,10,10);
	//itoa(message, tempString, 10);
	UserMessage(theRobotWindow, mboxOK, "Message", tempString);
}

struct inputStr : Window
{
	string getInp(string prompt);
};

string inputStr::getInp(string prompt)
{
	CInputBox ibox(hwnd);

	if (ibox.DoModal("Input Screen", prompt.c_str()))
	{
		string temp;
		char * tempStr;
		tempStr=ibox.Text;

		for(int i=0;tempStr[i]!=NULL;i++)
		{
			temp.resize(i+1);
			temp[i]=tempStr[i];
		}
		return temp;
	}
	else
	{
		MessageBox(NULL, "GetInput Error !!!" , "Error Screen", MB_OK);
		return "Error !!!";
	}
}


void GetInput(string prompt, string & var)
{
	inputStr tempStr;
	var=tempStr.getInp(prompt);
}

void GetInput(string prompt, int & var)
{
	inputStr tempStr;
	string temp;
	temp=tempStr.getInp(prompt);
	var=atoi(temp.c_str());
}

////////////////////////////////////////////////////////////////////////////////
/*
 * by Muge Erdogmus & Albert Levi
 * modified by Kubra Kalkan & Gulsen Demiroz
 * get count of things in a specific area
 */
int GetThingCount(int x1, int y1, int x2, int y2) 
{
/*
	-------------x2,y2
	|				|
	|				|
	x1,y1------------
*/  
    // to calculate the thing count in the cell
	if (x1 == x2 && y1 == y2)
	{
		return theRobotWorld->GetCount(x1,y1);
	}

	if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0) 
	{		
		return (-1); 
	}
	
	if (x1 > x2) 
	{
		int temp = x1;
		x1 = x2;
		x2 = temp;
		
	}
	else if (x1 == x2) 
	{
		return (-1);
	}

	if (y1 > y2) 
	{
		int temp = y1;
		y1 = y2;
		y2 = temp;
	} 
	else if (y1 == y2) 
	{
		return (-1);
	}
	
	int thingCount = 0;

	for (int i = x1; i <= x2; i++) 
	{
		for (int j = y1; j <= y2; j++) 
		{
			thingCount += theRobotWorld->GetCount(i,j);
		}
	}
	return (thingCount);
}



/*
 * by Muge Erdogmus & Albert Levi
 * get count of things in a given cell
 */
int GetCellCount(int x, int y)
{
	return GetThingCount(x,y,x,y);
}


/*
 * by Duygu Karaoðlan 
 * put given number of things in the given cell
 */
void PutThings(int xCor, int yCor, int thingCount)
{	
	if (theRobotWorld != 0)
	{
		for (int i=0; i<thingCount; i++)
			theRobotWorld->IncrCount(xCor, yCor);
		theRobotWorld->dirty = false; 
		theRobotWindow->Redraw();
	}
}

/*
 * by Duygu Karaoðlan
 * get count of cells that has things in
 */
int NumOfFullCells(int x1,int y1, int x2, int y2)
{
/*
	-------------x2,y2
	|				|
	|				|
	x1,y1------------
*/  
	if (x1 == x2 && y1 == y2 && GetCellCount(x1, y1)!=0)
	{
		return 1;
	}

	if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0) 
	{		
		return (-1); 
	}
	
	if (x1 > x2) 
	{
		int temp = x1;
		x1 = x2;
		x2 = temp;
		
	}
	else if (x1 == x2) 
	{
		return (-1);
	}

	if (y1 > y2) 
	{
		int temp = y1;
		y1 = y2;
		y2 = temp;
	} 
	else if (y1 == y2) 
	{
		return (-1);
	}
	
	int cellCount = 0;

	for (int i = x1; i <= x2; i++) 
	{
		for (int j = y1; j <= y2; j++) 
		{
			if (GetCellCount(i,j)!=0)
				cellCount ++;
		}
	}
	return (cellCount);
}


/*
 * by Duygu Karaoðlan
 * clears the things within the given cell
 */
void EmptyTheCell(int x, int y)
{
	while (GetCellCount(x,y)!=0)
		theRobotWorld->DecrCount(x, y);
	theRobotWorld->dirty = false; //XXX
	theRobotWindow->Redraw();	
}

/*
 * by Duygu Karaoðlan
 * clears the things within the given area
 */
void EmptyAllTheCells(int x1,int y1, int x2, int y2)
{
/*
	-------------x2,y2
	|				|
	|				|
	x1,y1------------
*/

	 // to calculate the thing count in the cell
	if (x1 == x2 && y1 == y2)
	{
		EmptyTheCell(x1,y1);
	}

	else
	{
		if (!(x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0) && x1!=x2 && y1!=y2)
		{		
			if (x1 > x2) 
			{
				int temp = x1;
				x1 = x2;
				x2 = temp;
				
			}

			if (y1 > y2) 
			{
				int temp = y1;
				y1 = y2;
				y2 = temp;
			} 
			
			for (int i = x1; i <= x2; i++) 
			{
				for (int j = y1; j <= y2; j++) 
				{
					while (GetCellCount(i,j)!=0)
						theRobotWorld->DecrCount(i,j);	
				}
			}
			theRobotWorld->dirty = false; //XXX
			theRobotWindow->Redraw();
		}
	}
}


/*
 * by Ýnanç Arýn
 * robot2 follows robot1
 */
 /*
void Chase (Robot & robot1, Robot & robot2, int xCor, int yCor)
// it is the function to provide that  r2 follows r1 
{
	if (robot1.getX() == xCor + 1)
	// if robot1 moves to right
	{
		if (robot2.getY() < robot1.getY())
		// if robot2 is under the robot1
		{
			robot2.Turn(north);
			robot2.Move();
		}
		else if (robot2.getY() == robot1.getY())
		// if robot2 is in left cell of the robot1
		// if robot2 is in the right cell then robot1 does not go the rigth
		{
			robot2.Turn(east);
			robot2.Move();
		}
		else if (robot2.getY() > robot1.getY()) 
		// if robot2 is in the up cell of the robot1
		{
			robot2.Turn(south);
			robot2.Move();
		}
	}
	else if (robot1.getX() == xCor - 1)
	// if robot1 moves to left
	{
		
		if (robot2.getY() < robot1.getY())
		//if robot2 is under the robot1
		{
			robot2.Turn(north);
			robot2.Move();
		}
		else if (robot2.getY() == robot1.getY())
		// if robot2 is in the right cell of the robot1
		// if robot2 is in the left cell then robot1 does not go the left
		{
			robot2.Turn(west);
			robot2.Move();
		}
		else if (robot2.getY() > robot1.getY()) 
		// if robot2 is in the up cell of the robot1
		{
			robot2.Turn(south);
			robot2.Move();
		}
	}
	else if (robot1.getY() == yCor + 1)
	// if robot1 moves to up
	{
		
		if (robot2.getX() < robot1.getX())
		// if robot2 is in the left cell of the robot1
		{
			robot2.Turn(east);
			robot2.Move();
		}
		else if (robot2.getX() == robot1.getX())
		// if robot2 is under the robot1
		// if robot2 is over the robot1 then robot1 does not go up
		{
			robot2.Turn(north);
			robot2.Move();
		}
		else if (robot2.getX() > robot1.getX())
		// if robot2 is in the right cell of the robot1
		{
			robot2.Turn(west);
			robot2.Move();
		}
	}
	else if (robot1.getY() == yCor - 1)
	// if robot1 moves down
	{
		
		if (robot2.getX() < robot1.getX())
		// if robot2 is in the left cell of the robot1
		{
			robot2.Turn(east);
			robot2.Move();
		}
		else if (robot2.getX() == robot1.getX())
		// if robot2 is in the up cell of the robot1
		// if robot2 is in the down cell of the robot1 then robot1 does not go down
		{
			robot2.Turn(south);
			robot2.Move();
		}
		else if (robot2.getX() > robot1.getX())
		// if robot2 is in the right cell of the robot1
		{
			robot2.Turn(west);
			robot2.Move();
		}
	}
}
*/

////////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow)
{
	ApplicationStartup(hInst, hPrevInst, cmdLine, cmdShow);

	WindowCreate wc;
	wc.title = robotWindowTitle;
	wc.style &= ~(winHScroll|winVScroll);
	theRobotWindow = new RobotWindow;
	theRobotWindow->Realize(wc);

	EventLoop();
	return 0;
}


