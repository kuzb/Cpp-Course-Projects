#ifndef MiniFW_Modified_h
#define MiniFW_Modified_h

// Mini framework for Windows programming
// Copyright © 1994-2003 Ersin Karabudak.

#include <windows.h>

typedef BOOL Boolean;

#ifndef UPoint_h
#define UPoint_h

typedef int Coordinate;

struct UPoint
{
	UPoint ();
	UPoint (int u, int v);
	int operator== (UPoint &p);
	int operator!= (UPoint &p);
	int operator< (UPoint &p);
	int operator<= (UPoint &p);
	int operator> (UPoint &p);
	int operator>= (UPoint &p);

	UPoint operator+ (UPoint &p);
	UPoint operator- (UPoint &p);
	UPoint operator* (UPoint &p);
	UPoint operator/ (UPoint &p);
	UPoint operator* (int u);
	UPoint operator/ (int u);

	UPoint & operator+= (UPoint &p);
	UPoint & operator-= (UPoint &p);
	UPoint & operator*= (UPoint &p);
	UPoint & operator/= (UPoint &p);
	UPoint & operator*= (int u);
	UPoint & operator/= (int u);

	Coordinate x;
	Coordinate y;
};

inline
UPoint::UPoint ()
{
	x = 0;
	y = 0;
}

inline
UPoint::UPoint (int u, int v)
{
	x = u;
	y = v;
}

inline
int UPoint::operator== (UPoint &p)
{
	return x == p.x && y == p.y;
}

inline
int UPoint::operator!= (UPoint &p)
{
	return x != p.x || y != p.y;
}

inline
UPoint UPoint::operator+ (UPoint &p)
{
	return UPoint(x + p.x, y + p.y);
}

inline
UPoint UPoint::operator- (UPoint &p)
{
	return UPoint(x - p.x, y - p.y);
}

inline
UPoint UPoint::operator* (UPoint &p)
{
	return UPoint(x * p.x, y * p.y);
}

inline
UPoint UPoint::operator/ (UPoint &p)
{
	return UPoint(x / p.x, y / p.y);
}

inline
UPoint UPoint::operator* (int u)
{
	return UPoint(x * u, y * u);
}

inline
UPoint UPoint::operator/ (int u)
{
	return UPoint(x / u, y / u);
}

inline
UPoint & UPoint::operator+= (UPoint &p)
{
	x += p.x;
	y += p.y;
	return *this;
}

inline
UPoint & UPoint::operator-= (UPoint &p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

inline
UPoint & UPoint::operator*= (UPoint &p)
{
	x *= p.x;
	y *= p.y;
	return *this;
}

inline
UPoint & UPoint::operator/= (UPoint &p)
{
	x /= p.x;
	y /= p.y;
	return *this;
}

inline
UPoint & UPoint::operator*= (int u)
{
	x *= u;
	y *= u;
	return *this;
}

inline
UPoint & UPoint::operator/= (int u)
{
	x /= u;
	y /= u;
	return *this;
}

#endif UPoint_h

#ifndef URect_h
#define URect_h

typedef RECT URect;

inline
int Width (URect &r)
{
	return r.right - r.left;
}

inline
int Height (URect &r)
{
	return r.bottom - r.top;
}

inline
UPoint & TopLeft (URect &r)
{
	return *(UPoint *)&r.left;
}

inline
UPoint & BottomRight (URect &r)
{
	return *(UPoint *)&r.right;
}

inline
UPoint TopRight (URect &r)
{
	return UPoint(r.right, r.top);
}

inline
UPoint BottomLeft (URect &r)
{
	return UPoint(r.left, r.bottom);
}

inline
void Expand (URect &r, int dx, int dy)
{
	InflateRect(&r, dx, dy);
}

inline
int Contains (URect &r, UPoint p)
{
	return PtInRect(&r, *(POINT*)&p);
}


#endif URect_h

#ifndef LibEvent_h
#define LibEvent_h

enum { mouseMove, mouseDown, mouseUp };

enum { leftButton, rightButton, middleButton };

struct GMouseEvent
{
	short	what;
	short	button;
	short   click;
	UPoint	where;

	Boolean ShiftDown ()		{ return (wp & MK_SHIFT) != 0; }
	Boolean ControlDown ()		{ return (wp & MK_CONTROL) != 0; }
	Boolean OptionDown ()		{ return 0; }
	Boolean CommandDown ()		{ return GetKeyState(VK_MENU) < 0; }
	Boolean CapitalDown ()		{ return GetKeyState(VK_CAPITAL) & 1; }
	Boolean MouseDown ()		{ return (wp & (MK_LBUTTON|MK_RBUTTON|MK_MBUTTON)) != 0; }
	Boolean LeftButtonDown ()	{ return (wp & MK_LBUTTON) != 0; }
	Boolean RightButtonDown ()	{ return (wp & MK_RBUTTON) != 0; }
	Boolean MiddleButtonDown ()	{ return (wp & MK_MBUTTON) != 0; }

	void Set (UINT message, WPARAM wparam, LPARAM lparam);

private:

	WPARAM	wp;
};

//

enum
{
	keySpace		= ' ',
	keyReturn		= '\r',
	keyNewline		= '\n',
	keyTab			= '\t',
	keyBackspace	= '\b',
	keyEscape		= 0x1b,
	keyCancel		= 256 + VK_CANCEL,
	keyPageUp		= 256 + VK_PRIOR,
	keyPageDown		= 256 + VK_NEXT,
	keyEnd			= 256 + VK_END,
	keyHome			= 256 + VK_HOME,
	keyLeftArrow 	= 256 + VK_LEFT,
	keyUpArrow 		= 256 + VK_UP,
	keyRightArrow 	= 256 + VK_RIGHT,
	keyDownArrow 	= 256 + VK_DOWN,
	keyInsert 		= 256 + VK_INSERT,
	keyDelete		= 256 + VK_DELETE,
	keyHelp 		= 256 + VK_HELP,
	keyEnter		= 256,
	keyControl		= 256 + VK_CONTROL,
	keyShift		= 256 + VK_SHIFT,
	keyAlt 			= 256 + VK_MENU
};

#define FunctionKey(n)	(255 + (n) + VK_F1)

/*inline
Boolean IsPressed (int key)
{
	return GetAsyncKeyState(key & 0xFF) == -32767;  //updated and deleted by Albert
}*/

extern int theKeyPressedValue;  //added by Albert

inline
Boolean IsPressed (int key)   //added by Albert
{
	if (theKeyPressedValue == key)
	{
		theKeyPressedValue = -1;
	    return true;
	}
	else
	{
		return false;
	}
}


struct GKeyEvent
{
	short	key;
	short	repeat;

	Boolean Extended ()			{ return key >= 256; }
	Boolean ShiftDown ()		{ return GetKeyState(VK_SHIFT) < 0; }
	Boolean ControlDown ()		{ return GetKeyState(VK_CONTROL) < 0; }
	Boolean OptionDown ()		{ return 0; }
	Boolean CommandDown ()		{ return GetKeyState(VK_MENU) < 0; }
	Boolean CapitalDown ()		{ return GetKeyState(VK_CAPITAL) & 1; }
	Boolean MouseDown ()		{ return GetKeyState(VK_LBUTTON) < 0; }
	Boolean LeftButtonDown ()	{ return GetKeyState(VK_LBUTTON) < 0; }
	Boolean RightButtonDown ()	{ return GetKeyState(VK_RBUTTON) < 0; }
	Boolean MiddleButtonDown ()	{ return GetKeyState(VK_MBUTTON) < 0; }

	Boolean Set (UINT message, WPARAM wparam, LPARAM lparam);
};

//

enum
{
	sbarHorizontal	= SB_HORZ,
	sbarVertical	= SB_VERT
};

enum
{
	sbarUp 			= SB_LINEUP,
	sbarDown 		= SB_LINEDOWN,
	sbarPageUp 		= SB_PAGEUP,
	sbarPageDown	= SB_PAGEDOWN,
	sbarDrag 		= SB_THUMBTRACK,
	sbarSet 		= SB_THUMBPOSITION,
	sbarTop 		= SB_TOP,
	sbarBottom 		= SB_BOTTOM
};

struct GScrollEvent
{
	short	kind;
	short	action;
	long	value;
};

//

Boolean GetMouseEvent (HWND hwnd, GMouseEvent &event);
void ForceMouseEvent (HWND hwnd);

#endif LibEvent_h

#ifndef Canvas_h
#define Canvas_h

struct Window;

typedef COLORREF UColor;

const short
	justLeft	= 0x0000,
	justRight	= 0x0001,
	justCenter	= 0x0002,
	justAlign	= 0x0003,
	justTop		= 0x0000,
	justBottom	= 0x0010,
	justCenterV	= 0x0020,
	justAlignV	= 0x0030,
	textHilite	= 0x0100;

const short
	textNormal		= 0,
	textBold		= 1,
	textItalic		= 2,
	textUnderline	= 4;

struct TypeFace
{
	TypeFace (char *tname, int tsize, short tstyle = textNormal);
	TypeFace (int tfont, int tsize, short tstyle = textNormal);

	int size;
	short style;
	char name [LF_FACESIZE];
	UColor color;
};

extern TypeFace fontSystem;
extern TypeFace fontFixed;

const int
	modeCopy = R2_COPYPEN,
	modeXor	= R2_NOTXORPEN;

struct Canvas
{
	Canvas (Window *p, Boolean update = 0);
	~Canvas ();

	void SetDrawMode (int mode);

	void SetFont (TypeFace &t);
	void SetFont (int font);
	void SetFont (int font, int size);
	void SetFont (char *name);
	void SetFont (char *name, int size);
	void SetTextSize (int size);
	void SetTextStyle (short style);
	void SetTextColor (UColor color);

	UPoint GetCharSize ();		//XXX

	int PutText (UPoint p, char *str);
	int PutText (UPoint p, char *str, int n);
	int PutText (UPoint p, char *str, int n, URect &hilite);
	int PutText (UPoint p, char *str, int n, int hs, int he);
	int PutText (URect &r, char *str, int n, short how);

	void SetLineWidth (short n);
	void SetLineColor (UColor color);
	void SetFillColor (UColor color);
	void Line (UPoint p, UPoint q);
	void DrawRect (URect &r);
	void FillRect (URect &r);
	void HiliteRect (URect &r);
	void DrawEllipse (URect &r);
	void FillEllipse (URect &r);

	void SetClipRect (URect &r);

	HWND	hwnd;
	HDC     hdc;
	URect	invalid;

	//

	void ApplyTextColor (Boolean hilite);
	void ApplyFont ();
	void DetachFont ();
	UPoint TextWidth (char *str, int n);
	HBRUSH GetHiliteBrush ();
	HBRUSH GetBackBrush ();

	void NoPen ();
	void ApplyPen ();
	void DetachPen ();

	void NoBrush ();
	void ApplyBrush ();
	void DetachBrush ();

	Boolean 	_update;
	PAINTSTRUCT _ps;

	HFONT		hfont;
	LOGFONT     logFont;
	short		textStyle;
	Boolean		fontChanged;
	Boolean		fontSelected;
	UColor		textColor;

	HBRUSH		hiliteBrush;
	HBRUSH		backBrush;

	short		lineWidth;
	UColor		lineColor;
	UColor		fillColor;

	HPEN 		hpen;
	Boolean		penChanged;
	Boolean		penSelected;

	HBRUSH 		hbrush;
	Boolean		brushChanged;
	Boolean		brushSelected;
};

const UColor defaultColor = 0xFFFFFFFFL;

const UColor colorBlack		= 0x000000L;
const UColor colorWhite		= 0xFFFFFFL;
const UColor colorGray		= 0x808080L;
const UColor colorDarkGray	= 0x404040L;
const UColor colorLightGray	= 0xC0C0C0L;
const UColor colorRed		= 0x0000FFL;
const UColor colorGreen		= 0x00FF00L;
const UColor colorBlue		= 0xFF0000L;
const UColor colorYellow	= 0x00FFFFL;
const UColor colorMagenta	= 0xFF00FFL;
const UColor colorCyan		= 0xFFFF00L;

#endif Canvas_h

#ifndef FileSpec_h
#define FileSpec_h

#define FILE_SIZE	256//added
struct FileSpec
{
	FileSpec ();

	void GetFileName (char *str, size_t size);//changed
	void GetPathName (char *str, size_t size);//changed
	char * FileType ();
	void SetType (char *t);

	char path [FILE_SIZE];//changed
};

#endif FileSpec_h

#ifndef FileDlg_h
#define FileDlg_h

int GetOpenFileSpec (FileSpec &file, char *types, Window *owner);
int GetSaveFileSpec (FileSpec &file, char *name, Window *owner);

#endif FileDlg_h


// Menu commands

#define menuSystem	128
#define menuWindow	129
#define menuFont	130
#define menuFile	131
#define menuEdit	132
#define menuSearch	133

#define cmdWindow	0xE000

#define	cmdQuit			1
#define	cmdAbout		2
#define	cmdNew			3
#define	cmdOpen			4
#define	cmdClose		5
#define	cmdSave			6
#define	cmdSaveAs		7
#define	cmdRevert		8

#define cmdCascade		10
#define cmdTile			11
#define cmdArrange		12
#define cmdCloseAll		13

#define cmdUndo			20
#define cmdCut			21
#define cmdCopy			22
#define cmdPaste		23
#define cmdClear		24
#define cmdSelectAll	25

#define cmdFind		   	30
#define cmdFindNext	   	31
#define cmdFindSelected	32

#define cmdUser			1000

// UserMessage

#define mboxStop		MB_ICONSTOP
#define mboxInfo		MB_ICONINFORMATION
#define mboxQuestion	MB_ICONQUESTION

#define mboxOK			MB_OK
#define mboxOKCancel	MB_OKCANCEL
#define mboxYesNo		MB_YESNO
#define mboxYesNoCancel	MB_YESNOCANCEL

#define mboxDefault1	MB_DEFBUTTON1
#define mboxDefault2	MB_DEFBUTTON2
#define mboxDefault3	MB_DEFBUTTON3

#define answerOK		IDOK
#define answerYes		IDYES
#define answerNo		IDNO
#define answerCancel	IDCANCEL

int UserMessage (int type, char *title, char *message);
int UserMessage (struct Window *p, int type, char *title, char *message);

// hInstance

extern HINSTANCE hInstance;

HINSTANCE GetInstanceHandle ();

// UCaret

struct UCaret
{
	UCaret (UPoint s, HWND h);
	~UCaret ();

	void Realize ();
	void Unrealize ();
	void Move (UPoint p);
	void Show ();
	void Hide ();

	HWND hwnd;
	UPoint size;
	UPoint place;
	Boolean	exist;
	Boolean	visible;

	static short stdWidth;
};

// UScrollBarInfo

struct UScrollBarInfo
{
	short scale;
	short page;
};

// Tracker

struct Tracker
{
	virtual ~Tracker ();
	virtual void Do (GMouseEvent &event) = 0;
};

// WindowCreate

enum
{
	wtypeNormal,
	wtypeToolBar,
	wtypeFloating,
	wtypeModal
};

#define winResize		WS_THICKFRAME
#define winZoom			WS_MAXIMIZEBOX
#define winMinimize		WS_MINIMIZEBOX
#define winCloseBox		WS_SYSMENU
#define winTitleBar		WS_CAPTION
#define winBorder		WS_BORDER
#define winHScroll		WS_HSCROLL
#define winVScroll		WS_VSCROLL
#define winLarge		WS_MAXIMIZE
#define winSmall		WS_MINIMIZE

#define sysDefault		CW_USEDEFAULT

struct WindowCreate
{
	WindowCreate ();

	DWORD style;
	UPoint place;
	UPoint size;
	char *title;
};

// Window

struct Window
{
	Window ();
	virtual ~Window ();
	virtual void Realize (WindowCreate &q);
	virtual void Unrealize ();
	virtual int Close ();
	virtual int QueryClose ();

	virtual void HandleResize ();
	virtual void Draw (Canvas &z);
	virtual void ProcessKey (GKeyEvent &event);
	virtual void ProcessMouse (GMouseEvent &event);
	virtual void AdjustCursor (GMouseEvent &event);
	virtual void HandleGetFocus ();
	virtual void HandleLoseFocus ();
	virtual void ProcessScroll (GScrollEvent &event);
	virtual int MenuCommand (int cmd);

	void Track (Tracker &tracker, GMouseEvent &event);

	URect GetUserRect ();
	void Activate ();
	void SetTitle (char *s);
	void GetTitle (char *s, int n);
	void Refresh ();
	void Invalidate ();
	void Invalidate (URect &r);
	void Scroll (URect &r, int dx, int dy, int update);
	void SetScrollScale (short kind, int s);
	void SetScrollValue (short kind, long u);
	void SetScrollLimits (short kind, long u, long v);
	void SetBackColor (UColor color);

	void CaretCreate (UPoint s);
	void CaretMove (UPoint p);
	void CaretShow (Boolean visible);
	void CaretDelete ();

	Boolean StartTimer (int n, int interval);
	Boolean StopTimer (int n);
	virtual void ProcessTimer (int n);

	void DispatchMenuCommand (int cmd);

	void SetSize (UPoint s);

	virtual LRESULT WindowProc (HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	virtual void ControlMessage (HWND control, WORD id, WORD ncode);
	virtual Window* GetActiveWindow ();

	HWND hwnd;
	DWORD theStyle;
	WNDPROC defaultWindowProc;
	UCaret* theCaret;
	Canvas* theCanvas;

	UColor backColor;
	Boolean redrawOnResize;
	Boolean closingWindow;
	UScrollBarInfo sbar [2];

	static Window* focus;
	static Window* inConstruction;
};

// Cursors

enum { cursorArrow,	cursorText, cursorCross, cursorWait, cursorUser };

void SetCursorShape (int cursor);

// Application

int EventStep ();
void EventLoop ();

int RegisterWindowClass ();

void ApplicationStartup (HINSTANCE hInst, HANDLE hPrevInst, LPSTR cmdLine, int cmdShow);

#endif MiniFW_h
