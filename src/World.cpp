//-----------------------------------------------------------------
// Menu Object
// C++ Source - World.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "World.h"

//-----------------------------------------------------------------
// World Constructor(s)/Destructor
//-----------------------------------------------------------------
World::World()
{	
	_pozadi = new Bitmap();
	_obrDelo1 = new Bitmap();
	_obrRaketa1 = new Bitmap();
	_obrExploze = new Bitmap();
	
//	_duse = new SpriteList(_postavyM,3,2,45,200,200,0);
}


World::~World()
{
	delete _pozadi;
	delete _delo1;
//	delete _raketa1;
	delete _obrDelo1;
	delete _obrRaketa1;
	delete _obrExploze;
}

//-----------------------------------------------------------------
// World General Methods
//-----------------------------------------------------------------
void World::VykresliSvet(HDC hDC)
{
	_pozadi->Draw(hDC,0,0);

//	_delo1->Draw(hDC);
//	_raketa1->Draw(hDC);
	
	if (  _delo1->IsEndAnimation() )  {_delo1->SetFramesRange(7,7);Vystrel(); }

//	if (  ((rand() % 50) == 0) && ( _delo1->IsEndAnimation()) )  _delo1->SetFramesRange(1,8);
//		else if ( _delo1->IsEndAnimation() )  
}

void World::Vystrel()
{
	_raketa1->SetHidden(false);
	_raketa1->SetVelocity(0,10);
}
//-----------------------------------------------------------------
// World Accessor Methods
//-----------------------------------------------------------------
