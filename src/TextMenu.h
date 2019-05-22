//-----------------------------------------------------------------
// Text menu Object
// C++ Header - TextMenu.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include <string>
#include <vector>

using namespace std;
//-----------------------------------------------------------------
// Custom Data Types
//-----------------------------------------------------------------
struct item
{
  RECT		area;
  string	caption;
  COLORREF	color;
};

//-----------------------------------------------------------------
// Text menu Class
//-----------------------------------------------------------------
class TextMenu
{
	private:
		vector<item>	m_items;
		HFONT			m_font;
		
	public:
	  	// Constructor(s)/Destructor
		TextMenu(HDC,int,string);
		TextMenu(HDC,HFONT*);
		TextMenu();
		~TextMenu();
		
  		// General Methods
		void 	Draw(HDC);
		int		IsMouseUp(int,int);				//vrací èíslo bunky pod kurzorem
		void	AddItem(int,int,string);
		void	AddItem(RECT*,string);

  		// Accessor Methods
		void	SetItem_caption(int,string);
		void	SetItem_color(int, COLORREF, COLORREF);

		void	SetAllItems_color(COLORREF);		
};

