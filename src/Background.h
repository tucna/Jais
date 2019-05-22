//-----------------------------------------------------------------
// Background Object
// C++ Header - Background.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Bitmap.h"

//-----------------------------------------------------------------
// Background Class
//-----------------------------------------------------------------
class Background
{
protected:
  // Member Variables
  int       m_iWidth, m_iHeight;
  COLORREF  m_crColor;
  Bitmap*   m_pBitmap;

public:
	// Constructor(s)/Destructor
	Background(int iWidth, int iHeight, COLORREF crColor);
	Background(Bitmap* pBitmap);
	virtual ~Background();

	// General Methods
	virtual void  Update();
	virtual void  Draw(HDC hDC);

	// Accessor Methods
	int GetWidth()  { return m_iWidth; };
	int GetHeight() { return m_iHeight; };
};

//-----------------------------------------------------------------
// Starry Background Class
//-----------------------------------------------------------------
class StarryBackground : Background
{
protected:
  // Member Variables
  int       m_iNumStars;
  int       m_iTwinkleDelay;
  POINT     m_ptStars[100];
  COLORREF  m_crStarColors[100];

public:
  // Constructor(s)/Destructor
          StarryBackground(int iWidth, int iHeight, int iNumStars = 100,
            int iTwinkleDelay = 50);
  virtual ~StarryBackground();

  // General Methods
  virtual void  Update();
  virtual void  Draw(HDC hDC);
};
//-----------------------------------------------------------------
// Tile Background Class
//-----------------------------------------------------------------
class TileBackground : Background
{
private:
	int m_iCols, m_iRows;
	
public:
	// Constructor(s)/Destructor
	TileBackground(Bitmap* bit,int iCols, int iRows); 
	virtual ~TileBackground();

	// General Methods
	virtual void  Draw(HDC hDC);
};
//-----------------------------------------------------------------
// Scroll Background Class
//-----------------------------------------------------------------
class ScrollBackground : Background
{
protected:
  // Member Variables
	int		m_iNumLines;
	int		m_iNumLinesBackUp;
	int		m_iTwinkleDelay,m_iTwinkleTrigger;
	int		m_iPozY;
	char	m_text[250];
	int		m_iPozYText;
	char	*m_data;
	int	 	m_delka;
	bool	m_withText;
  
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

public:
	// Constructor(s)/Destructor
	ScrollBackground(Bitmap* bit,int iWidth, int iHeight, int iNumLines = 3,
            int iTwinkleDelay = 50,char *pSoubor = NULL);
	virtual ~ScrollBackground();

  // General Methods
  	virtual void  Update();
  	virtual void  Draw(HDC hDC);
  	void 	Stop();
  	void 	Run();
  	bool	IsRun();
  	bool	IsTextUp();
  	void 	JumpUp();
};
