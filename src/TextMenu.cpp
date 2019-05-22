//-----------------------------------------------------------------
// Text menu Object
// C++ Source - TextMenu.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "TextMenu.h"

//-----------------------------------------------------------------
// Text menu Constructor(s)/Destructor
//-----------------------------------------------------------------
TextMenu::TextMenu(HDC hDC,int fontHeight, string fontName)
{
	m_font = CreateFont(fontHeight,						// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight FW_BOLD
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout			
						DEFAULT_CHARSET,				// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						fontName.c_str());				// Font Name
						
	SelectObject(hDC, m_font);
}

TextMenu::TextMenu(HDC hDC,HFONT* f)
{
	m_font = *f;
	SelectObject(hDC, f);
}

TextMenu::TextMenu()
{
}

TextMenu::~TextMenu()
{
	DeleteObject(m_font);
}

void TextMenu::Draw(HDC hDC)
{			
	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
	
//	SelectObject(hDC, m_font);

	SetBkMode(hDC,TRANSPARENT);

	for (int i=0;i<m_items.size();i++)
		{
			SetTextColor(hDC,m_items[i].color);
//			FillRect(hDC,&m_items[i].area,brush);
			DrawText(hDC,TEXT(m_items[i].caption.data()),-1,&m_items[i].area,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}

	SetTextColor(hDC,RGB(30,30,30));
}

void TextMenu::SetItem_caption(int id,string cap)
{
	m_items[id].caption = cap;
	m_items[id].color = RGB(30,30,30);
}

void TextMenu::AddItem(int x, int y, string cap)
{
	RECT pom;	
	
	pom.left = x;
	pom.top = y;
	pom.right =  cap.length()*10 + x + 20; //x + 100
	pom.bottom = y + 25;
	
	item h;
	
	h.area = pom;
	h.caption = cap;
	h.color = RGB(30,30,30);
	
	m_items.push_back(h);
}

void TextMenu::AddItem(RECT* are,string cap)
{
	item h;
	
	h.area = *are;
	h.caption = cap;
	h.color = RGB(30,30,30);
	
	m_items.push_back(h);
}

void TextMenu::SetItem_color(int id, COLORREF col, COLORREF otherCol)
{
	for(int i=0;i<m_items.size();i++)
		if (i == id) m_items[i].color = col;
			else m_items[i].color = otherCol;
}

int TextMenu::IsMouseUp(int mx, int my)
{
	for (int i=0;i<m_items.size();i++)
		if ( my > m_items[i].area.top && my < m_items[i].area.bottom 
		    && mx > m_items[i].area.left && mx < m_items[i].area.right) return i;
		
	return -1;
}

void TextMenu::SetAllItems_color(COLORREF col)
{
	for(int i=0;i<m_items.size();i++)
		m_items[i].color = col;
}

