//-----------------------------------------------------------------
// Menu Object
// C++ Header - World.h
//-----------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Bitmap.h"
#include "Sprite.h"
#include "GameEngine.h"
#include "Resource.h"

//-----------------------------------------------------------------
// Custom Data Types
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// World Class
//-----------------------------------------------------------------
class World
{
public:
  	// Member Variables  	
	    	

  	// Constructor(s)/Destructor
  	World();   // pozadi se v MAIN dotvori pres CREATE
  	~World();
  	
  	// General Methods
  	void VykresliSvet(HDC);
  	void Vystrel();
  	
  	// Accessor Methods
};
