//-----------------------------------------------------------------
// Background Object
// C++ Source - Background.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
//#define WINVER 0x0500
#include "Background.h"

//-----------------------------------------------------------------
// Background Constructor(s)/Destructor
//-----------------------------------------------------------------
Background::Background(int iWidth, int iHeight, COLORREF crColor)
{
	// Initialize the member variables
  	m_iWidth = iWidth;
	m_iHeight = iHeight;
  	m_crColor = crColor;
  	m_pBitmap = NULL;
}

Background::Background(Bitmap* pBitmap)
{
  	// Initialize the member variables
  	m_crColor = 0;
  	m_pBitmap = pBitmap;
  	m_iWidth = pBitmap->GetWidth();
  	m_iHeight = pBitmap->GetHeight();
}

Background::~Background()
{
}

//-----------------------------------------------------------------
// Background General Methods
//-----------------------------------------------------------------
void Background::Update()
{
  	// Do nothing since the basic background is not animated
}

void Background::Draw(HDC hDC)
{
  	// Draw the background
  	if (m_pBitmap != NULL)
    	m_pBitmap->Draw(hDC, 0, 0);
  	else
  	{
    	RECT    rect = { 0, 0, m_iWidth, m_iHeight };
    	HBRUSH  hBrush = CreateSolidBrush(m_crColor);
    	FillRect(hDC, &rect, hBrush);
    	DeleteObject(hBrush);
  	}
}

//-----------------------------------------------------------------
// StarryBackground Constructor
//-----------------------------------------------------------------
StarryBackground::StarryBackground(int iWidth, int iHeight, int iNumStars,
  	int iTwinkleDelay) : Background(iWidth, iHeight, 0)
{
  	// Initialize the member variables
  	m_iNumStars = min(iNumStars, 100);
  	m_iTwinkleDelay = iTwinkleDelay;

  	// Create the stars
  	for (int i = 0; i < iNumStars; i++)
  	{
    	m_ptStars[i].x = rand() % iWidth;
    	m_ptStars[i].y = rand() % iHeight;
    	m_crStarColors[i] = RGB(128, 128, 128);
  	}
}

StarryBackground::~StarryBackground()
{
}

//-----------------------------------------------------------------
// StarryBackground General Methods
//-----------------------------------------------------------------
void StarryBackground::Update()
{
  // Randomly change the shade of the stars so that they twinkle
  int iRGB;
  for (int i = 0; i < m_iNumStars; i++)
    if ((rand() % m_iTwinkleDelay) == 0)
    {
      iRGB = rand() % 256;
      m_crStarColors[i] = RGB(iRGB, iRGB, iRGB);
    }
}

void StarryBackground::Draw(HDC hDC)
{
  // Draw the solid black background
  RECT    rect = { 0, 0, m_iWidth, m_iHeight };
  HBRUSH  hBrush = CreateSolidBrush(RGB(0, 0, 0));
  FillRect(hDC, &rect, hBrush);
  DeleteObject(hBrush);

  // Draw the stars
  for (int i = 0; i < m_iNumStars; i++)
    SetPixel(hDC, m_ptStars[i].x, m_ptStars[i].y, m_crStarColors[i]);
}

//-----------------------------------------------------------------
// TileBackground Constructor
//-----------------------------------------------------------------
TileBackground::TileBackground(Bitmap* bit,int iCols, int iRows):
	Background(bit)
{
	m_iCols = iCols;
	m_iRows = iRows;		
}

TileBackground::~TileBackground()
{
}

//-----------------------------------------------------------------
// TileBackground General Methods
//-----------------------------------------------------------------
void  TileBackground::Draw(HDC hDC)
{	
	for (int i=0; i < m_iCols; i++)
		for (int o=0; o < m_iRows; o++)
			m_pBitmap->Draw(hDC,i*160,o*160);
}

//-----------------------------------------------------------------
// ScrollBackground Constructor
//-----------------------------------------------------------------
ScrollBackground::ScrollBackground(Bitmap* bit,int iWidth, int iHeight, int iNumLines,
  int iTwinkleDelay, char *pSoubor) : Background(iWidth, iHeight, 0)
{
  // Initialize the member variables
  m_iNumLines = min(iNumLines, 100);
  m_iTwinkleDelay = m_iTwinkleTrigger = iTwinkleDelay;  
  m_iPozY = 0;
  m_pBitmap = bit;
  m_iPozYText = 480;
  m_data = new char[5000];
  m_delka = 0;
  m_withText = false;
  
  if (pSoubor != NULL)
  	{  
		m_withText = true;
		  HANDLE soubor = CreateFile(
				TEXT(pSoubor),
				GENERIC_READ,
				0,
				NULL,
				OPEN_ALWAYS,
				FILE_ATTRIBUTE_READONLY,
				NULL
		  );
  
    
		  DWORD	dwBytesRead;
		  int Aktual = 0;	
		
			
		  while (ReadFile(soubor, &m_data[Aktual++], 1, &dwBytesRead, NULL) && dwBytesRead != 0)
		  {		
			m_delka++;
		  };
  
			CloseHandle(soubor);

			font = CreateFont(	30,							// Height Of Font
							0,								// Width Of Font
							0,								// Angle Of Escapement
							0,								// Orientation Angle
							FW_BOLD,						// Font Weight
							FALSE,							// Italic
							FALSE,							// Underline
							FALSE,							// Strikeout			
							EASTEUROPE_CHARSET,				// Character Set Identifier
							OUT_TT_PRECIS,					// Output Precision
							CLIP_DEFAULT_PRECIS,			// Clipping Precision
							ANTIALIASED_QUALITY,			// Output Quality
							FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
							"Monotype Corsiva");			// Font Name
			}

}

ScrollBackground::~ScrollBackground()
{
	delete m_data;
	DeleteObject(font);								// Delete The Font
	DeleteObject(oldfont);							// Selects The Font We Want
}

//-----------------------------------------------------------------
// ScrollBackground General Methods
//-----------------------------------------------------------------
void ScrollBackground::Update()
{
  	if ((m_iTwinkleDelay >= 0) && (--m_iTwinkleTrigger <= 0))
  	{
		m_iTwinkleTrigger = m_iTwinkleDelay;	
	
		m_iPozY = m_iPozY - m_iNumLines;  
		m_iPozYText -= m_iNumLines;	
	}
}

void ScrollBackground::Draw(HDC hDC)
{
	if (m_iPozY <= -480) m_iPozY = 0;
	m_pBitmap->Draw(hDC,0,m_iPozY);
	m_pBitmap->Draw(hDC,0,m_iPozY + 480);
	
	if (m_withText)
	{

		oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
		SelectObject(hDC, font);							// Selects The Font We Want

		SetBkMode(hDC,TRANSPARENT);
		SetTextColor(hDC,RGB(0,0,0));
	
		RECT pokus = {25,m_iPozYText,615,480};

		DrawText(hDC,TEXT(m_data),-1,&pokus,DT_WORDBREAK);	
	
		SelectObject(hDC, oldfont);							// Selects The Font We Want
	}
}

void ScrollBackground::Stop()
{
	m_iNumLinesBackUp = m_iNumLines;
	m_iNumLines = 0;
}

bool ScrollBackground::IsRun()
{
	return m_iNumLines > 0 ? true:false;
}

void ScrollBackground::Run()
{
	m_iNumLines = m_iNumLinesBackUp;
}

bool ScrollBackground::IsTextUp()
{
	return m_iPozYText<=10 ? true:false;
}

void ScrollBackground::JumpUp()
{
	m_iNumLinesBackUp = m_iNumLines;
	m_iNumLines = m_iPozYText - 10;
}
