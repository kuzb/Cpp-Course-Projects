
// Mini framework for Windows programming
// Copyright Â© 1994-2003 Ersin Karabudak.

#include "MiniFW_Modified.h"
#include <stdlib.h>
#include <string.h>
#include <commdlg.h>


////////////////////////////////////////////////////////////////////////////////
// UPoint
////////////////////////////////////////////////////////////////////////////////


int UPoint::operator< (UPoint &p)
{
	return y < p.y || y == p.y && x < p.x;
}

int UPoint::operator<= (UPoint &p)
{
	return y < p.y || y == p.y && x <= p.x;
}

int UPoint::operator> (UPoint &p)
{
	return y > p.y || y == p.y && x > p.x;
}

int UPoint::operator>= (UPoint &p)
{
	return y > p.y || y == p.y && x >= p.x;
}


////////////////////////////////////////////////////////////////////////////////
// LibEvent
////////////////////////////////////////////////////////////////////////////////


void GMouseEvent::Set (UINT message, WPARAM wparam, LPARAM lparam)
{
	static char msgType [] =
	{
		mouseMove,
		mouseDown, mouseUp, mouseDown,
		mouseDown, mouseUp, mouseDown,
		mouseDown, mouseUp, mouseDown
	};
	static char msgButton [] =
	{
		-1,
		leftButton, leftButton, leftButton,
		rightButton, rightButton, rightButton,
		middleButton, middleButton, middleButton
	};
	static char msgClick [] =
	{
		0,  1, 0, 2,  1, 0, 2,  1, 0, 2
	};

	what = msgType[message - WM_MOUSEFIRST];
	button = msgButton[message - WM_MOUSEFIRST];
	click = msgClick[message - WM_MOUSEFIRST];
	#ifdef WIN32
	where = UPoint(MAKEPOINTS(lparam).x, MAKEPOINTS(lparam).y);
	#else
	where = UPoint(LOWORD(lparam), HIWORD(lparam));
	#endif
	wp = wparam;
}


//

Boolean GKeyEvent::Set (UINT message, WPARAM wparam, LPARAM lparam)
{
	if (message == WM_CHAR)
	{
		#ifdef WIN32
		key = TCHAR(wparam);
		repeat = LOWORD(lparam);
		if (key == VK_RETURN && (lparam & 0x01000000L))
			key = keyEnter;
		#else
		key = wparam;
		repeat = LOWORD(lparam);
		if (key == VK_RETURN && (lparam & 0x01000000L))
			key = keyEnter;
		#endif
		return 1;
	}
	if (message == WM_KEYDOWN)
	{
		//if (wparam >= '0' && wparam <= '9' || wparam >= 'A' && wparam <= 'Z')
		//	return 0;
		key = 256 + wparam;
		repeat = LOWORD(lparam);
		return 1;
	}
	return 0;
}


//

Boolean GetMouseEvent (HWND hwnd, GMouseEvent &event)
{
	MSG msg;
	if (!PeekMessage(&msg, hwnd, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE))
		return 0;
	ScreenToClient(hwnd, &msg.pt);
	event.Set(msg.message, msg.wParam, MAKELPARAM(msg.pt.x, msg.pt.y));
	return 1;
}

void ForceMouseEvent (HWND hwnd)
{
	MSG msg;
	if (PeekMessage(&msg, hwnd, WM_MOUSEFIRST, WM_MOUSELAST, PM_NOREMOVE))
		return;
	POINT pos;
	GetCursorPos(&pos);
	SetCursorPos(pos.x, pos.y);
}


////////////////////////////////////////////////////////////////////////////////
// Canvas
////////////////////////////////////////////////////////////////////////////////


TypeFace::TypeFace (char *tname, int tsize, short tstyle)
{
	strcpy_s(name, LF_FACESIZE, tname);//changed
	size = tsize;
	style = tstyle;
	color = colorBlack;
}

TypeFace::TypeFace (int tfont, int tsize, short tstyle)
{
	name[0] = 0;	//XXX
	size = tsize;
	style = tstyle;
	color = colorBlack;
}


TypeFace fontSystem(0, 0);
TypeFace fontFixed("Courier New", 16);


//

Canvas::Canvas (Window *p, Boolean update)
{
	hwnd = p->hwnd;
	_update = update;
	if (_update)
	{
		hdc = BeginPaint(hwnd, &_ps);
		invalid = _ps.rcPaint;
	}
	else
	{
		hdc = GetDC(hwnd);
		GetClientRect(hwnd, &invalid);
	}

	hfont = 0;
	memset(&logFont, 0, sizeof logFont);
	textColor = defaultColor;
	textStyle = textNormal;
	fontChanged = 1;
	fontSelected = 0;

	hiliteBrush = 0;
	backBrush = 0;

	lineWidth = 1;
	lineColor = colorBlack;
	fillColor = colorBlack;

	hpen = 0;
	penChanged = 1;
	penSelected = 0;

	hbrush = 0;
	brushChanged = 1;
	brushSelected = 0;
}

Canvas::~Canvas ()
{
	DetachFont();
	DetachPen();
	DetachBrush();

	if (hiliteBrush)
		DeleteObject(hiliteBrush);
	if (backBrush)
		DeleteObject(backBrush);

	if (_update)
		EndPaint(hwnd, &_ps);
	else
		ReleaseDC(hwnd, hdc);
}


void Canvas::SetDrawMode (int mode)
{
	SetROP2(hdc, mode);
}

void Canvas::SetFont (TypeFace &t)
{
	SetFont(t.name, t.size);
	SetTextStyle(t.style);
	textColor = t.color;
}

void Canvas::SetFont (char *name)
{
	strcpy_s(logFont.lfFaceName, LF_FACESIZE, name);//changed
	fontChanged = 1;
}

void Canvas::SetFont (char *name, int size)
{
	strcpy_s(logFont.lfFaceName, LF_FACESIZE, name);//changed
	logFont.lfHeight = size;
	fontChanged = 1;
}

void Canvas::SetTextSize (int size)
{
	if (logFont.lfHeight == size)
		return;
	logFont.lfHeight = size;
	fontChanged = 1;
}

void Canvas::SetTextStyle (short style)
{
	if (textStyle == style)
		return;
	textStyle = style;
	logFont.lfWeight = (style & textBold) ? 700 : 0;
	logFont.lfItalic = (style & textItalic) != 0;
	logFont.lfUnderline = (style & textUnderline) != 0;
	fontChanged = 1;
}

void Canvas::SetTextColor (UColor color)
{
	textColor = color;
}

UPoint Canvas::GetCharSize ()
{
	ApplyFont();
	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	UPoint charCell;
	charCell.x = tm.tmAveCharWidth;
	charCell.y = tm.tmExternalLeading + tm.tmAscent + tm.tmDescent;
	return charCell;
}

int Canvas::PutText (UPoint p, char *str)
{
	return PutText(p, str, strlen(str));
}

int Canvas::PutText (UPoint p, char *str, int n)
{
	ApplyFont();
	ApplyTextColor(0);
	TextOut(hdc, p.x, p.y, str, n);
	return TextWidth(str, n).x;
}

int Canvas::PutText (UPoint p, char *str, int n, URect &hilite)
{
	ApplyFont();
	ApplyTextColor(1);
	::FillRect(hdc, &hilite, GetHiliteBrush());
	TextOut(hdc, p.x, p.y, str, n);
	return Width(hilite);
}

int Canvas::PutText (UPoint p, char *str, int n, int hs, int he)
{
	ApplyFont();
	UPoint q = p;
	if (hs > 0)
		p.x += PutText(p, str, hs);
	if (he > hs)
	{
		URect r;
		TopLeft(r) = p;
		BottomRight(r) = TextWidth(str + hs, he - hs);
		BottomRight(r) += p;
		if (he > n)
		{
			r.right = invalid.right;
			he = n;
		}
		p.x += PutText(p, str + hs, he - hs, r);
	}
	if (n > he)
		p.x += PutText(p, str + he, n - he);
	return p.x - q.x;
}

int Canvas::PutText (URect &r, char *str, int n, short how)
{
	ApplyFont();
    if (how & textHilite)
	{
		ApplyTextColor(1);
		::FillRect(hdc, &r, GetHiliteBrush());
    }
    else
		ApplyTextColor(0);

    UINT format = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP;
    switch (how & justAlign)
	{
    	case justLeft:
        	format |= DT_LEFT;
            break;
        case justRight:
        	format |= DT_RIGHT;
			break;
        default:
			format |= DT_CENTER;
            break;
    }
    switch (how & justAlignV)
    {
    	case justTop:
        	format |= DT_TOP;
			break;
        case justBottom:
        	format |= DT_BOTTOM;
            break;
        default:
			format |= DT_VCENTER;
            break;
    }
    
	DrawText(hdc, str, n, &r, format);
	return Width(r);
}

void Canvas::ApplyTextColor (Boolean hilite)
{
	SetBkMode(hdc, TRANSPARENT);
	if (hilite)
		::SetTextColor(hdc, GetSysColor(COLOR_HIGHLIGHTTEXT));
	else if (textColor == defaultColor)
		::SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
	else
		::SetTextColor(hdc, textColor);
}

void Canvas::ApplyFont ()
{
	if (fontChanged)
	{
		DetachFont();
		hfont = CreateFontIndirect(&logFont);
		fontChanged = 0;
	}
	if (!fontSelected)
	{
		hfont = (HFONT)SelectObject(hdc, hfont);
		fontSelected = 1;
	}
}

void Canvas::DetachFont ()
{
	if (fontSelected)
	{
		hfont = (HFONT)SelectObject(hdc, hfont);
		fontSelected = 0;
	}
	if (hfont)
	{
		DeleteObject(hfont);
		hfont = 0;
	}
}

UPoint Canvas::TextWidth (char *str, int n)
{
	SIZE size;
	GetTextExtentPoint(hdc, str, n, &size);
	return UPoint(size.cx, size.cy);
}

HBRUSH Canvas::GetHiliteBrush ()
{
	if (hiliteBrush == 0)
		hiliteBrush = CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
	return hiliteBrush;
}

HBRUSH Canvas::GetBackBrush ()
{
	if (backBrush == 0)
		backBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	return backBrush;
}


void Canvas::NoPen ()
{
	if (penSelected)
	{
		hpen = (HPEN)SelectObject(hdc, hpen);
		penSelected = 0;
	}
	SelectObject(hdc, GetStockObject(NULL_PEN));
}

void Canvas::ApplyPen ()
{
	if (penChanged)
	{
		DetachPen();
		hpen = CreatePen(PS_SOLID, lineWidth, lineColor);
		penChanged = 0;
	}
	if (!penSelected)
	{
		hpen = (HPEN)SelectObject(hdc, hpen);
		penSelected = 1;
	}
}

void Canvas::DetachPen ()
{
	if (penSelected)
	{
		hpen = (HPEN)SelectObject(hdc, hpen);
		penSelected = 0;
	}
	if (hpen)
	{
		DeleteObject(hpen);
		hpen = 0;
	}
}

void Canvas::NoBrush ()
{
	if (brushSelected)
	{
		hbrush = (HBRUSH)SelectObject(hdc, hbrush);
		brushSelected = 0;
	}
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
}

void Canvas::ApplyBrush ()
{
	if (brushChanged)
	{
		DetachBrush();
		hbrush = CreateSolidBrush(fillColor);
		brushChanged = 0;
	}
	if (!brushSelected)
	{
		hbrush = (HBRUSH)SelectObject(hdc, hbrush);
		brushSelected = 1;
	}
}

void Canvas::DetachBrush ()
{
	if (brushSelected)
	{
		hbrush = (HBRUSH)SelectObject(hdc, hbrush);
		brushSelected = 0;
	}
	if (hbrush)
	{
		DeleteObject(hbrush);
		hbrush = 0;
	}
}

void Canvas::SetLineWidth (short n)
{
	if (lineWidth == n)
		return;
	lineWidth = n;
	penChanged = 1;
}

void Canvas::SetLineColor (UColor color)
{
	if (lineColor == color)
		return;
	lineColor = color;
	penChanged = 1;
}

void Canvas::SetFillColor (UColor color)
{
	if (fillColor == color)
		return;
	fillColor = color;
	brushChanged = 1;
}

void Canvas::Line (UPoint p, UPoint q)
{
	ApplyPen();
	MoveToEx(hdc, p.x, p.y, 0);
	LineTo(hdc, q.x, q.y);
}

void Canvas::DrawRect (URect &r)
{
	ApplyPen();
	NoBrush();
	Rectangle(hdc, r.left, r.top, r.right, r.bottom);
}

void Canvas::FillRect (URect &r)
{
	NoPen();
	ApplyBrush();
	Rectangle(hdc, r.left, r.top, r.right + 1, r.bottom + 1);
}

void Canvas::HiliteRect (URect &r)
{
	::FillRect(hdc, &r, GetHiliteBrush());
}

void Canvas::DrawEllipse (URect &r)
{
	ApplyPen();
	NoBrush();
	Ellipse(hdc, r.left, r.top, r.right, r.bottom);
}

void Canvas::FillEllipse (URect &r)
{
	NoPen();
	ApplyBrush();
	Ellipse(hdc, r.left, r.top, r.right, r.bottom);
}

void Canvas::SetClipRect (URect &r)
{
	HRGN hrgn = CreateRectRgnIndirect(&r);
	SelectClipRgn(hdc, hrgn);
	DeleteObject(hrgn);
}


////////////////////////////////////////////////////////////////////////////////
// FileSpec
////////////////////////////////////////////////////////////////////////////////


FileSpec::FileSpec ()
{
	path[0] = 0;
}

void FileSpec::GetFileName (char *str, size_t size)
{
	char u [256];
	if (GetFileTitle(path, u, sizeof u) != 0)
		u[0] = 0;
	strcpy_s(str, size, u);//changed
}

void FileSpec::GetPathName (char *str, size_t size)
{
	strcpy_s(str, size, path);//changed
}

char * FileSpec::FileType ()
{
	static char e[5] = { '.', 0, 0, 0, 0 };
	char u [256];
	if (GetFileTitle(path, u, sizeof u) != 0)
		u[0] = 0;
	char *p = strrchr(u, '.');
	if (p)
		strncpy_s(e, 5, p, 4);
	else
		e[1] = 0;
	AnsiLower(e);
	return e;
}

void FileSpec::SetType (char *t)
{
	if (*t == '.')
		t++;
	char *q = strrchr(path, '\\');
	char *p = strrchr(q ? q + 1 : path, '.');
	if (p)
	{
		strcpy_s(p + 1, 3, t);//changed
	}
	else
	{
		strcat_s(path, FILE_SIZE, ".");//changed
		strcat_s(path, FILE_SIZE, t);//changed
	}
}

////////////////////////////////////////////////////////////////////////////////
// FileDlg
////////////////////////////////////////////////////////////////////////////////


int GetOpenFileSpec (FileSpec &file, char *types, Window *owner)
{
	OPENFILENAME ofn;
	memset(&file, 0, sizeof file);
	memset(&ofn, 0, sizeof ofn);
	ofn.lStructSize = sizeof ofn;
	ofn.hwndOwner = owner ? owner->hwnd : 0;
	ofn.hInstance = hInstance;
	if (types)
	{
		ofn.lpstrFilter = types;
		for (char *p = types; *p; p++)
			if (*p == '|')
				*p = 0;
	}
	else
		ofn.lpstrFilter = "All files\0" "*.*\0";
	ofn.lpstrFile = file.path;
	ofn.nMaxFile = sizeof file.path;
	if (!GetOpenFileName(&ofn))
		return 0;
	AnsiLower(file.path);
	return 1;
}

int GetSaveFileSpec (FileSpec &file, char *name, Window *owner)
{
	OPENFILENAME ofn;
	char str [sizeof file.path];
	if (name)
		strncpy_s(str, sizeof file.path, name, sizeof str);//changed
	else
		str[0] = 0;
	memset(&file, 0, sizeof file);
	memset(&ofn, 0, sizeof ofn);
	ofn.lStructSize = sizeof ofn;
	ofn.hwndOwner = owner ? owner->hwnd : 0;
	ofn.hInstance = hInstance;
	ofn.lpstrFile = str;
	ofn.nMaxFile = sizeof str;
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
	if (!GetSaveFileName(&ofn))
		return 0;
	strcpy_s(file.path, FILE_SIZE, str);//changed
	AnsiLower(file.path);
	return 1;
}


////////////////////////////////////////////////////////////////////////////////
// MiniFW
////////////////////////////////////////////////////////////////////////////////


// hInstance

HINSTANCE hInstance = 0;

HINSTANCE GetInstanceHandle ()
{
	return hInstance;
}

static LPSTR winMainCmdLine = 0;
static int winMainCmdShow = 0;

// UserMessage

int UserMessage (int type, char *title, char *message)
{
	return MessageBox(0, message, title, type);
}

int UserMessage (struct Window *p, int type, char *title, char *message)
{
	return MessageBox(p->hwnd, message, title, type);
}

// UCaret

short UCaret::stdWidth = 2;

UCaret::UCaret (UPoint s, HWND h)
{
	size = s;
	hwnd = h;
	exist = 0;
	visible = 1;
	Realize();
}

UCaret::~UCaret ()
{
	Unrealize();
}

void UCaret::Realize ()
{
	Unrealize();
	if (hwnd == GetFocus())
	{
		exist = 1;
		CreateCaret(hwnd, 0, size.x, size.y);
		SetCaretPos(place.x, place.y);
		if (visible)
			ShowCaret(hwnd);
	}
}

void UCaret::Unrealize ()
{
	if (exist)
	{
		Hide();
		DestroyCaret();
		exist = 0;
	}
}

void UCaret::Move (UPoint p)
{
	place = p;
	if (exist)
		SetCaretPos(place.x, place.y);
}

void UCaret::Show ()
{
	if (visible)
		return;
	visible = 1;
	if (exist)
		ShowCaret(hwnd);
}

void UCaret::Hide ()
{
	if (!visible)
		return;
	visible = 0;
	if (exist)
		HideCaret(hwnd);
}

// Tracker

Tracker::~Tracker ()
{
}

// WindowCreate

static DWORD defaultWindowStyle =
	winBorder|winTitleBar|winCloseBox|winResize|winZoom|winMinimize|winHScroll|winVScroll;

WindowCreate::WindowCreate ()
{
	style = defaultWindowStyle;
	place.x = place.y = sysDefault;
	size.x = size.y = sysDefault;
	title = 0;
}

// Window

Window* Window::focus = 0;
Window* Window::inConstruction = 0;


inline
Window* GetWindow (HWND hwnd)
{
	return (Window*)GetWindowLong(hwnd, 0);
}


Window::Window ()
{
	hwnd = 0;
	theCanvas = 0;
	theCaret = 0;
	defaultWindowProc = DefWindowProc;
	backColor = defaultColor;
	redrawOnResize = 0;
	closingWindow = 0;
	sbar[0].scale = sbar[1].scale = 0;
}

Window::~Window ()
{
}

void Window::Realize (WindowCreate &q)
{
	inConstruction = this;
	theStyle = q.style | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE;
	hwnd = CreateWindow("MiniWindow", q.title, theStyle,
						q.place.x, q.place.y, q.size.x, q.size.y,
						0, 0, hInstance, 0);
	ShowWindow(hwnd, winMainCmdShow);
	UpdateWindow(hwnd);
	/*
	if (theStyle & winHScroll)
		sbar[sbarHorizontal] = new UScrollBar(hwnd, sbarHorizontal);
	if (theStyle & winVScroll)
		sbar[sbarVertical] = new UScrollBar(hwnd, sbarVertical);
	*/
	SetFocus(hwnd);
}

void Window::Unrealize ()
{
	redrawOnResize = 0;
	CaretDelete();
	DestroyWindow(hwnd);
}

int Window::Close ()
{
	if (closingWindow)
		return 1;
	closingWindow = QueryClose();
	if (!closingWindow)
		return 0;
	Unrealize();
	delete this;
	return 1;
}

int Window::QueryClose ()
{
	return 1;
}

void Window::HandleResize ()
{
}

void Window::Draw (Canvas &)
{
}

void Window::ProcessKey (GKeyEvent &)
{
}

void Window::ProcessMouse (GMouseEvent &)
{
}

void Window::AdjustCursor (GMouseEvent &)
{
	SetCursor(LoadCursor(0, IDC_ARROW));
}

void Window::HandleGetFocus ()
{
}

void Window::HandleLoseFocus ()
{
}

void Window::ProcessScroll (GScrollEvent &)
{
}

int Window::MenuCommand (int cmd)
{
	switch (cmd)
	{
		case cmdClose:
			Close();
            return 1;
	}
	return 0;
}

URect Window::GetUserRect ()
{
	URect r;
	GetClientRect(hwnd, &r);
	return r;
}

void Window::Activate ()
{
	SetFocus(hwnd);
}

void Window::SetTitle (char *s)
{
	SetWindowText(hwnd, s);
}

void Window::GetTitle (char *s, int n)
{
	GetWindowText(hwnd, s, n);
}

void Window::Refresh ()
{
	UpdateWindow(hwnd);
}

void Window::Invalidate ()
{
	if (hwnd)
		InvalidateRect(hwnd, 0, 1);
}

void Window::Invalidate (URect &r)
{
	if (hwnd)
		InvalidateRect(hwnd, &r, 1);
}

void Window::Scroll (URect &r, int dx, int dy, int update)
{
	ScrollWindow(hwnd, dx, dy, &r, &r);
	if (update)
		UpdateWindow(hwnd);
}

void Window::SetScrollScale (short kind, int s)
{
	if (s <= 0)
		return;
	sbar[kind].scale = s;
	URect r = GetUserRect();
	int size = (kind == sbarHorizontal) ? Width(r) : Height(r);
	sbar[kind].page = size / s;
	#ifdef WIN32
	SCROLLINFO si;
	si.cbSize = sizeof si;
	si.fMask = SIF_PAGE;
	si.nPage = sbar[kind].page;
	SetScrollInfo(hwnd, kind, &si, this == focus);
	#endif
}

void Window::SetScrollValue (short kind, long u)
{
	SetScrollPos(hwnd, kind, u, this == focus);
}

void Window::SetScrollLimits (short kind, long u, long v)
{
	if (u < v)
	{
		SetScrollRange(hwnd, kind, u, v, this == focus);
		EnableScrollBar(hwnd, kind, ESB_ENABLE_BOTH);
	}
	else
		EnableScrollBar(hwnd, kind, ESB_DISABLE_BOTH);
}

void Window::SetBackColor (UColor color)
{
	if (backColor == color)
		return;
	backColor = color;
	Invalidate();
}

void Window::Track (Tracker &tracker, GMouseEvent &event)
{
	SetCapture(hwnd);
	GMouseEvent e = event;
	tracker.Do(e);
	while (e.what != mouseUp || e.button != event.button)
	{
		Refresh();
		ForceMouseEvent(hwnd);
		if (GetMouseEvent(hwnd, e))
			tracker.Do(e);
	}
	ReleaseCapture();
}

void Window::CaretCreate (UPoint s)
{
	DestroyCaret();
	theCaret = new UCaret(s, hwnd);
}

void Window::CaretMove (UPoint p)
{
	if (theCaret)
		theCaret->Move(p);
}

void Window::CaretShow (Boolean visible)
{
	if (theCaret)
		if (visible)
			theCaret->Show();
		else
			theCaret->Hide();
}

void Window::CaretDelete ()
{
	delete theCaret;
	theCaret = 0;
}

Boolean Window::StartTimer (int n, int interval)
{
	return SetTimer(hwnd, n, interval, 0) != 0;
}

Boolean Window::StopTimer (int n)
{
	return KillTimer(hwnd, n);
}

void Window::ProcessTimer (int)
{
}

void Window::DispatchMenuCommand (int cmd)
{
	MenuCommand(cmd);
}

void Window::SetSize (UPoint s)
{
	if (hwnd)
	{
		RECT r;
		SetRect(&r, 0, 0, s.x, s.y);
		AdjustWindowRect(&r, theStyle, 0);
		SetWindowPos(hwnd, 0, 0, 0, Width(r), Height(r), SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
	}
}

LRESULT Window::WindowProc (HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_SIZE:
			if (redrawOnResize)
				Invalidate();
			if (theStyle & winHScroll)
				SetScrollScale(sbarHorizontal, sbar[sbarHorizontal].scale);
			if (theStyle & winVScroll)
				SetScrollScale(sbarVertical, sbar[sbarVertical].scale);
			HandleResize();
			break;

		case WM_ERASEBKGND:
			if (backColor != defaultColor)
			{
				HBRUSH hbr = CreateSolidBrush(backColor);
				URect r;
				GetClientRect(hwnd, &r);
				FillRect(HDC(wp), &r, hbr);
				DeleteObject(hbr);
				return 1;
			}
			break;

		case WM_PAINT:
			{
				HideCaret(hwnd);
				Canvas z(this, 1);
				theCanvas = &z;
				Draw(z);
				theCanvas = 0;
				ShowCaret(hwnd);
			}
			return 0;

		case WM_SETFOCUS:
			focus = this;
			//if (wp)
			//	UpdateWindow(HWND(wp));
			HandleGetFocus();
			if (theStyle & winHScroll)
				ShowScrollBar(hwnd, SB_HORZ, 1);
			if (theStyle & winVScroll)
				ShowScrollBar(hwnd, SB_VERT, 1);
			/*
			if (theStyle & winHScroll)
				EnableScrollBar(hwnd, SB_HORZ, ESB_ENABLE_BOTH);
			if (theStyle & winVScroll)
				EnableScrollBar(hwnd, SB_VERT, ESB_ENABLE_BOTH);
			*/
			if (theCaret)
				theCaret->Realize();
			break;

		case WM_KILLFOCUS:
			if (theCaret)
				theCaret->Unrealize();
			ShowScrollBar(hwnd, SB_BOTH, 0);
			//EnableScrollBar(hwnd, SB_BOTH, ESB_DISABLE_BOTH);
			HandleLoseFocus();
			UpdateWindow(hwnd);
			break;

		case WM_COMMAND:
			#ifdef WIN32
			if (lp)
				ControlMessage(HWND(lp), LOWORD(wp), HIWORD(wp));
			else if (focus)
				focus->DispatchMenuCommand(LOWORD(wp));
			#else
			if (LOWORD(lp))
				ControlMessage(LOWORD(lp), wp, HIWORD(lp));
			else if (focus)
				focus->DispatchMenuCommand(wp);
			#endif
			return 0;

		case WM_QUERYENDSESSION:
		case WM_CLOSE:

			return Close();

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			//SetFocus(hwnd);

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_MOUSEMOVE:
			{
				GMouseEvent event;
				event.Set(msg, wp, lp);
				AdjustCursor(event);
				ProcessMouse(event);
			}
			return 0;

		case WM_KEYDOWN:
		case WM_CHAR:
			{
				GKeyEvent event;
				if (event.Set(msg, wp, lp))
					ProcessKey(event);
			}
			return 0;

		case WM_HSCROLL:
		case WM_VSCROLL:
			{
				GScrollEvent event;
				event.kind = msg - WM_HSCROLL;
				#ifdef WIN32
				event.action = LOWORD(wp);
				event.value = HIWORD(wp);
				#else
				event.action = wp;
				event.value = LOWORD(lp);
				#endif
				long u = GetScrollPos(hwnd, event.kind);
				switch (event.action)
				{
					case sbarDown:
						u++;
						break;
					case sbarUp:
						u--;
						break;
					case sbarPageDown:
						u += sbar[event.kind].page - 1;
						break;
					case sbarPageUp:
						u -= sbar[event.kind].page - 1;
						break;
					case sbarSet:
					case sbarDrag:
						u = event.value;
						break;
					case sbarTop:
						break;
					case sbarBottom:
						break;
				}
				SetScrollPos(hwnd, event.kind, u, 1);
				event.value = u;
				ProcessScroll(event);
			}
			return 0;

		case WM_TIMER:
			ProcessTimer(wp);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 1;
	}

	//return defaultWindowProc(hwnd, msg, wp, lp);
	return CallWindowProc(WNDPROC(defaultWindowProc), hwnd, msg, wp, lp);
}

void Window::ControlMessage (HWND, WORD, WORD)
{
}

Window* Window::GetActiveWindow ()
{
	return this;
}


// Cursors

void SetCursorShape (int cursor)
{
	static LPCSTR predefined [] = { IDC_ARROW, IDC_IBEAM, IDC_CROSS, IDC_WAIT };
	if (cursor < cursorUser)
		SetCursor(LoadCursor(0, predefined[cursor]));
	else
		SetCursor(LoadCursor(hInstance, MAKEINTRESOURCE(cursor)));
}


// Application

static HACCEL accelerators = 0;

int EventStep ()
{
	MSG msg;
	if (!GetMessage(&msg, 0, 0, 0))
		return 0;
	if (accelerators && Window::focus && TranslateAccelerator(Window::focus->hwnd, accelerators, &msg))
		return 1;
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	return 1;
}

void EventLoop ()
{
	while (EventStep())
		;
}

LRESULT CALLBACK GlueWindowProc (HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	return GetWindow(hwnd)->WindowProc(hwnd, msg, wp, lp);
}

LRESULT CALLBACK LaunchWindowProc (HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	Window::inConstruction->hwnd = hwnd;
	SetWindowLong(hwnd, 0, LONG(Window::inConstruction));
	SetWindowLong(hwnd, GWL_WNDPROC, LONG(GlueWindowProc));
	return SendMessage(hwnd, msg, wp, lp);
}

int RegisterWindowClass ()
{
	WNDCLASS wndclass;
	wndclass.style = CS_DBLCLKS;
	wndclass.lpfnWndProc = LaunchWindowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = sizeof(LONG);
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, "WindowIcon");
	wndclass.hCursor = 0;
	wndclass.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
	wndclass.lpszMenuName = 0;
	wndclass.lpszClassName = "MiniWindow";
	return RegisterClass(&wndclass);
}

void ApplicationStartup (HINSTANCE hInst, HANDLE hPrevInst, LPSTR cmdLine, int cmdShow)
{
	hInstance = hInst;
	winMainCmdLine = cmdLine;
	winMainCmdShow = cmdShow;
	if (!hPrevInst && !RegisterWindowClass())
		exit(EXIT_FAILURE);
	accelerators = LoadAccelerators(hInstance, "Accelerators");
}


