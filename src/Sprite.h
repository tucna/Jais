//-----------------------------------------------------------------
// Sprite Object
// C++ Header - Sprite.h
//-----------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Bitmap.h"

//-----------------------------------------------------------------
// Custom Data Types
//-----------------------------------------------------------------
typedef WORD        SPRITEACTION;
const SPRITEACTION  SA_NONE      = 0x0000L,
                    SA_KILL      = 0x0001L,
                    SA_ADDSPRITE = 0x0002L,
                    SA_ERASE	 = 0x0003L;

typedef WORD        BOUNDSACTION;
const BOUNDSACTION  BA_STOP   = 0,
                    BA_WRAP   = 1,
                    BA_BOUNCE = 2,
                    BA_DIE    = 3;

//-----------------------------------------------------------------
// Sprite Class
//-----------------------------------------------------------------
class Sprite
{
protected:
  // Member Variables
  Bitmap*       m_pBitmap;
  
  int			m_iFrom,m_iTo;
  
  int           m_iNumFramesX, m_iNumFramesY, m_iCurFrame, m_iPositionFramesY;  
  int           m_iFrameDelay, m_iFrameTrigger;
  RECT          m_rcPosition,
                m_rcCollision;
  POINT         m_ptVelocity;
  int           m_iZOrder;
  RECT          m_rcBounds;
  BOUNDSACTION  m_baBoundsAction;
  BOOL          m_bHidden;
  BOOL          m_bDying;
  BOOL          m_bOneCycle;
  BOOL			m_bEnabled;
  BOOL			m_bErase;
  
  BOOL			m_bLastFrame;
  bool			m_bCollisionSprite;
  
  BOOL 			m_bIsChosen;
  
  int			angle;
  int			m_iIndex;

  // Helper Methods
  void          UpdateFrame();
  virtual void  CalcCollisionRect();

public:
  // Constructor(s)/Destructor
  Sprite();
  Sprite(Bitmap* pBitmap);
  Sprite(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
  Sprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
  virtual ~Sprite();

  // General Methods
  virtual SPRITEACTION  Update();
  virtual Sprite*       AddSprite();
  void                  Draw(HDC hDC);
  BOOL                  IsPointInside(int x, int y);
  BOOL                  TestCollision(Sprite* pTestSprite);
  void                  Kill()      { m_bDying = TRUE; m_bEnabled = false; };
  void                  Erase()     { m_bErase = TRUE; m_bEnabled = false; };

  // Accessor Methods
  Bitmap* 	GetBitmap()               { return m_pBitmap; };
  void    	SetNumFrames(int iNumFramesX, int iNumFramesY = 1, BOOL bOneCycle = FALSE);
  void    	SetFrameDelay(int iFrameDelay) { m_iFrameDelay = iFrameDelay; };
  RECT&   	GetPosition()             { return m_rcPosition; };
  void    	SetPosition(int x, int y);
  void    	SetPosition(POINT ptPosition);
  void    	SetPosition(RECT& rcPosition);
  void		SetFramesRange(int iFrom,int iTo, int iYLine = 1)	{ m_iFrom = iFrom - 1; m_iTo = iTo;  m_iCurFrame = m_iFrom;  m_iPositionFramesY = iYLine;} ;
  int		GetFrom()		{return m_iFrom;};
  int		GetTo()			{return m_iTo;};
  int		GetLine()		{return m_iPositionFramesY;};
  void    	OffsetPosition(int x, int y);
  void		SetChosen(bool s)		{m_bIsChosen = s;};
  RECT&   	GetCollision()            { return m_rcCollision; };
  POINT   	GetVelocity()             { return m_ptVelocity; };
  void    	SetVelocity(int x, int y);
  void    	SetVelocity(POINT ptVelocity);
  BOOL    	GetZOrder()               { return m_iZOrder; };
  void    	SetZOrder(int iZOrder)    { m_iZOrder = iZOrder; };
  void    	SetBounds(RECT& rcBounds) { CopyRect(&m_rcBounds, &rcBounds); };
  void    	SetBoundsAction(BOUNDSACTION ba) { m_baBoundsAction = ba; };
  BOOL    	IsHidden()                { return m_bHidden; };
  void    	SetHidden(BOOL bHidden)   { m_bHidden = bHidden; if (m_bHidden == true) m_bEnabled = false; else m_bEnabled = true;};
  int     	GetWidth()				{ return (m_pBitmap->GetWidth() / m_iNumFramesX); };
  int     	GetHeight()				{ return (m_pBitmap->GetHeight() / m_iNumFramesY); };
  void		SetEnabled(bool tf)		{ m_bEnabled = tf; };
  void		SetBitmap(Bitmap* pBitmap);
  bool		IsEnabled()		{return m_bEnabled;};
  bool		IsEndAnimation()	{return m_bLastFrame;};
  bool		IsChosen()		{return m_bIsChosen;};
  void		SetAngle(int a)		{angle = a;};
  int		GetAngle()			{return angle;};  
  void		SetCollisionSprite(bool a) {m_bCollisionSprite = a;};
  bool		GetCollisionSprite()	   {return m_bCollisionSprite;};  
  int		GetCurFrame()			{return m_iCurFrame;};
  void		SetIndex(int a)			{m_iIndex = a;};
  int		GetIndex()				{return m_iIndex;};
};

//-----------------------------------------------------------------
// Sprite Inline Helper Methods
//-----------------------------------------------------------------
inline void Sprite::UpdateFrame()
{
  if ((m_iFrameDelay >= 0) && (--m_iFrameTrigger <= 0))
  {
    // Reset the frame trigger;
    m_iFrameTrigger = m_iFrameDelay;    
//    m_bLastFrame = false;
    
    // Increment the frame
    if (++m_iCurFrame >= m_iTo)
    {
      // If it's a one-cycle frame animation, kill the sprite

      if (m_bOneCycle)  m_bDying = TRUE;
		else
			{	
				m_iCurFrame = m_iFrom;
				m_bLastFrame = true;
			}
    }
    else m_bLastFrame = false;
  }
}

inline void Sprite::CalcCollisionRect()
{
  int iXShrink = (m_rcPosition.left - m_rcPosition.right) / 12;
  int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / 12;
  CopyRect(&m_rcCollision, &m_rcPosition);
  InflateRect(&m_rcCollision, iXShrink, iYShrink);
}

//-----------------------------------------------------------------
// Sprite Inline General Methods
//-----------------------------------------------------------------
inline BOOL Sprite::TestCollision(Sprite* pTestSprite)
{
  RECT& rcTest = pTestSprite->GetCollision();
  return (m_rcCollision.left <= rcTest.right &&
         rcTest.left <= m_rcCollision.right &&
         m_rcCollision.top <= rcTest.bottom &&
         rcTest.top <= m_rcCollision.bottom);
}

inline BOOL Sprite::IsPointInside(int x, int y)
{
  	POINT ptPoint;
  	ptPoint.x = x;
  	ptPoint.y = y;
  
	return PtInRect(&m_rcPosition, ptPoint);
}

//-----------------------------------------------------------------
// Sprite Inline Accessor Methods
//-----------------------------------------------------------------
inline void Sprite::SetNumFrames(int iNumFramesX, int iNumFramesY, BOOL bOneCycle)
{
	// Set the number of frames and the one-cycle setting
  	m_iNumFramesX = iNumFramesX;
  	m_iNumFramesY = iNumFramesY;
  
  	m_iPositionFramesY = 1;    
  
  	m_bOneCycle = bOneCycle;

  	// Recalculate the position
  	RECT rect = GetPosition();

  	rect.right = rect.left + ((rect.right - rect.left) / iNumFramesX);
  	rect.bottom = rect.top + ((rect.bottom- rect.top) / iNumFramesY);

  	SetPosition(rect);
}

inline void Sprite::SetPosition(int x, int y)
{
  	OffsetRect(&m_rcPosition, x - m_rcPosition.left, y - m_rcPosition.top);
  	CalcCollisionRect();
}

inline void Sprite::SetPosition(POINT ptPosition)
{
	OffsetRect(&m_rcPosition, ptPosition.x - m_rcPosition.left, ptPosition.y - m_rcPosition.top);
	CalcCollisionRect();
}

inline void Sprite::SetPosition(RECT& rcPosition)
{
  	CopyRect(&m_rcPosition, &rcPosition);
  	CalcCollisionRect();
}

inline void Sprite::OffsetPosition(int x, int y)
{
  	OffsetRect(&m_rcPosition, x, y);
  	CalcCollisionRect();
}

inline void Sprite::SetVelocity(int x, int y)
{
  	m_ptVelocity.x = x;
  	m_ptVelocity.y = y;
}

inline void Sprite::SetVelocity(POINT ptVelocity)
{
  	m_ptVelocity.x = ptVelocity.x;
  	m_ptVelocity.y = ptVelocity.y;
}
