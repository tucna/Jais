//-----------------------------------------------------------------
// Main 
// C++ Source - Main.cpp
//-----------------------------------------------------------------

/*
Chyby
- delo muze vystrelit bezprostredne dvakrát za sebou
- nefungují kolize pøi nastavení vlastnosti Z u spritu
*/

/*	Sprite který vytvoøí jiný sprite pøi svém zániku	
 * musí mít stejný ZOrder jako právì mazaný sprite (?)
 */
 

/* 
Vylepseni
- pridat bonusy (rychlost strel, rychlost vozitka)

*/ 


//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "main.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------

BOOL GameInitialize(HINSTANCE hInstance)
{
	// Create the game engine
	_pGame = new GameEngine(hInstance, TEXT("Jais"), TEXT("Jais"), IDI_SKELETON, IDI_SKELETON_SM);
	if (_pGame == NULL)	return FALSE;
  
  	// Set the frame rate
  	_pGame->SetFrameRate(50);
  	
  	_hInstance = hInstance;
  	
  	return TRUE;
}

void GameStart(HWND hWindow)
{
	HDC hDC = GetDC(hWindow);	

  	_hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
  	_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow),_pGame->GetWidth(), _pGame->GetHeight());
  	SelectObject(_hOffscreenDC, _hOffscreenBitmap);

	// Seed the random number generator
	srand(GetTickCount());
	
	// Bitmaps
	_pozadi = new Bitmap(hDC,IDB_POZADI,_hInstance);
	_obrDelo1 = new Bitmap(hDC,IDB_DELO1,_hInstance);
	_obrDelo2 = new Bitmap(hDC,IDB_DELO2,_hInstance);
	_obrDelo3 = new Bitmap(hDC,IDB_DELO3,_hInstance);
	_obrDelo4 = new Bitmap(hDC,IDB_DELO4,_hInstance);
	_obrRaketa1 = new Bitmap(hDC,IDB_RAKETA1,_hInstance);
	_obrRaketa2 = new Bitmap(hDC,IDB_RAKETA2,_hInstance);
	_obrRaketa3 = new Bitmap(hDC,IDB_RAKETA3,_hInstance);
	_obrRaketa4 = new Bitmap(hDC,IDB_RAKETA4,_hInstance);
	_obrExploze = new Bitmap(hDC,IDB_EXPLOZE,_hInstance);
	_obrBigExploze = new Bitmap(hDC,IDB_BIGEXPLOZE,_hInstance);
	_obrAuto = new Bitmap(hDC,IDB_AUTO,_hInstance);
	_obrStrela = new Bitmap(hDC,IDB_STRELA,_hInstance);
	_obrExpStrela = new Bitmap(hDC,IDB_EXPSTRELA,_hInstance);
	_obrBonusy = new Bitmap(hDC,IDB_BONUSY,_hInstance);
	_obrFinish = new Bitmap(hDC,IDB_FINISH,_hInstance);
	_obrJet = new Bitmap(hDC,IDB_JET,_hInstance);
	_obrSplash = new Bitmap(hDC,IDB_SPLASH,_hInstance);
	_obrVlajky = new Bitmap(hDC,IDB_VLAJKY,_hInstance);
	_obrJetFire = new Bitmap(hDC,IDB_JETFIRE,_hInstance);
	_obrKamen = new Bitmap(hDC,"");
	_obrAB = new Bitmap(hDC,"");
	_obrMitkins = new Bitmap(hDC,IDB_MITKINS,_hInstance);
	_obrStoryFirst = new Bitmap(hDC,IDB_STORYFIRST,_hInstance);
	_obrStorySecond = new Bitmap(hDC,IDB_STORYSECOND,_hInstance);
	_obrJoe = new Bitmap(hDC,IDB_JOE,_hInstance);
	_obrLevel = new Bitmap(hDC,IDB_LEVEL,_hInstance);
	
	// Tiles
	_tile1 = new Bitmap(hDC,"");
	
	// Levels
	_level1 = new TileBackground(_tile1,4,3);
		
	// Sprites
	_flagCZ = new Sprite(_obrVlajky);
    _flagCZ->SetNumFrames(2,2);
	_flagCZ->SetPosition(10,10);
    _flagCZ->SetFramesRange(1,1,1);
    _pGame->AddSprite(_flagCZ);
    
	_flagUS = new Sprite(_obrVlajky);
    _flagUS->SetNumFrames(2,2);
	_flagUS->SetPosition(40,8);
    _flagUS->SetFramesRange(1,1,2);
    _pGame->AddSprite(_flagUS);    
    
	RECT miArea = {-100,-100,940,560};    	
	_joe = new Sprite(_obrJoe,miArea);
    _joe->SetNumFrames(8);
	_joe->SetPosition(50,230);
    _joe->SetFramesRange(1,1);
    
	_mitkins = new Sprite(_obrMitkins,miArea);
    _mitkins->SetNumFrames(8,3);
	_mitkins->SetPosition(680,230);
    _mitkins->SetFramesRange(1,8,1);
    _mitkins->SetVelocity(-2,0);

	_A = new Sprite(_obrAB);
    _A->SetNumFrames(2);
	_A->SetPosition(20,420);
    _A->SetFramesRange(1,1);
    _A->SetZOrder(1);

	_B = new Sprite(_obrAB);
    _B->SetNumFrames(2);
	_B->SetPosition(570,30);
    _B->SetFramesRange(2,2);
    _B->SetZOrder(1);
    
	// SDL
    g_win_flags = SDL_HWSURFACE|SDL_DOUBLEBUF;
	g_volume = 120;


	g_volume_left = 128;
	g_sound_distance = 128;
	g_reverse_stereo = false;

	_mSounds = NULL;
	_iCountSounds = -1;

	InitSDL();

	_musicMenu = Mix_LoadMUS(".\\snd\\music.ogg");
	_mSounds = Mix_LoadWAV(".\\snd\\exploze.wav");
	
	Mix_VolumeMusic(g_volume);
	Mix_PlayMusic(_musicMenu,-1);

	Mix_VolumeChunk(_mSounds,50);
	
	// Language	
	CreateTable("cz.dat");
	
	// Fonts
	_storyFont = CreateFont(20,							// Height Of Font
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
						"Curlz MT");					// Font Name
						
	_gameFont = CreateFont(80,							// Height Of Font
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
						"Curlz MT");					// Font Name
						
	_font = CreateFont(30,								// Height Of Font
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
						"Curlz MT");					// Font Name						

	_oldfont = (HFONT)SelectObject(hDC, _font);
	AddFontResource("CURLZ___.TTF");	

	// Menu	
	_titulky = Translate("STRTIT001")+": Pavel Vlašánek, "+Translate("STRTIT002")+": Pavel Vlašánek, " + Translate("STRTIT003")+": Roman Antala";	
	
	_horniMenu = new TextMenu();	
	_horniMenu->AddItem(430,7,Translate("STRMENU02"));
	_horniMenu->AddItem(550,4,Translate("STRMENU03"));
	
	_nastaveniMenu = new TextMenu();
	_nastaveniMenu->AddItem(290,250,"<");
	_nastaveniMenu->AddItem(355,250,">");
	_nastaveniMenu->AddItem(290,280,"<");
	_nastaveniMenu->AddItem(355,280,">");
	_nastaveniMenu->AddItem(205,360,Translate("STRMENU04"));
	_nastaveniMenu->AddItem(305,360,Translate("STRMENU05"));
	
	_hlavniMenu = new TextMenu(hDC,35,"Curlz MT");
	
	RECT oblast = {210,250,385,285};
	_hlavniMenu->AddItem(&oblast," - " + Translate("STRMENU07") + " - ");
	
	RECT oblast1 = {210,300,385,335};
	_hlavniMenu->AddItem(&oblast1," - " + Translate("STRMENU06") + " - ");
	
	RECT oblast2 = {210,350,385,385};	
	_hlavniMenu->AddItem(&oblast2," - " + Translate("STRMENU08") + " - ");

	for(int i=0;i<5;i++)
	{
		_kameny[i] = new Sprite(_obrKamen);
		_kameny[i]->SetNumFrames(1);
    		_kameny[i]->SetFramesRange(1,1);
    		_kameny[i]->SetZOrder(0);
	}
	
	_kameny[0]->SetPosition(80,325);
	_kameny[1]->SetPosition(75,150);
	_kameny[2]->SetPosition(10,20);
	_kameny[3]->SetPosition(250,200);
	_kameny[4]->SetPosition(530,120);

	for(int i=0;i<_pocetDelNahore;i++)
		_dela[i] = new Delo(_obrDelo1,_obrRaketa1,105+(i*100),30,i,nahore);

	for(int i=_pocetDelNahore;i<(_pocetDelNahore+_pocetDelDole) ;i++)
		_dela[i] = new Delo(_obrDelo2,_obrRaketa2,155+( (i-_pocetDelNahore)*100),430,i,dole);

	for(int i=_pocetDelNahore+_pocetDelDole;i<(_pocetDelNahore+_pocetDelDole+_pocetDelVlevo);i++)
		_dela[i] = new Delo(_obrDelo3,_obrRaketa3,30,70+( (i-(_pocetDelNahore+_pocetDelDole))*110),i,vlevo);

	for(int i=_pocetDelNahore+_pocetDelDole+_pocetDelVlevo;i<(_pocetDelNahore+_pocetDelDole+_pocetDelVlevo+_pocetDelVpravo);i++)
		_dela[i] = new Delo(_obrDelo4,_obrRaketa4,585,120+( (i-(_pocetDelNahore+_pocetDelDole+_pocetDelVlevo))*110),i,vpravo);
}

void GameEnd()
{	
	// Cleanup the offscreen device context and bitmap
  	DeleteObject(_hOffscreenBitmap);
  	DeleteDC(_hOffscreenDC);  

  	// Cleanup the bitmaps	
	delete _pozadi;
	delete _obrDelo1;
	delete _obrDelo2;
	delete _obrDelo3;
	delete _obrDelo4;
	delete _obrRaketa1;
	delete _obrRaketa2;
	delete _obrRaketa3;
	delete _obrRaketa4;
	delete _obrExploze;
	delete _obrAuto;
	delete _obrStrela;
	delete _obrExpStrela;
	delete _obrBigExploze;
	delete _obrBonusy;
	delete _obrFinish;
	delete _obrJet;
	delete _obrSplash;
	delete _obrJetFire;
	delete _obrMitkins;
	delete _obrJoe;
	delete _obrStoryFirst;
	delete _obrStorySecond;	
	delete _obrLevel;
	delete _obrKamen;
	delete _obrAB;
	
	delete _tile1;
	
	delete _level1;
	
	Mix_CloseAudio();

	delete _horniMenu;
	delete _nastaveniMenu;
	delete _hlavniMenu;
	
	delete _obrVlajky;
	
	DeleteObject(_font);
	DeleteObject(_storyFont);
	DeleteObject(_oldfont);
	
  	// Cleanup the sprites and game engine
  	_pGame->CleanupSprites();
  	
	delete _pGame;
	
	RemoveFontResource("ravie.ttf");

	SDL_Quit();	
    exit(0);
}

void GameActivate(HWND hWindow)
{
}

void GameDeactivate(HWND hWindow)
{
}

void GamePaint(HDC hDC)
{
	switch (_stavHry)
	{		
		case pribeh:
			if ( _stavPribehu < 10) _obrStoryFirst->Draw(hDC,0,0);
				else _obrStorySecond->Draw(hDC,0,0);
			break;
		case hra:
			if (_mise == 1) _obrLevel->Draw(hDC,0,0);
				else _level1->Draw(hDC);
			break;			
		default:
			_pozadi->Draw(hDC,0,0);
			break;
	}
		
	_pGame->DrawSprites(hDC); 
	
	switch (_stavHry)
	{
		case menu:			
			DrawSplashScreen(hDC); 
			break;			
		case nastaveni:			
			DrawSettingsScreen(hDC);
			break;
		case pribeh:
			DrawStoryScreen(hDC);
			break;
		case hra:
			DrawGameScreen(hDC);
			break;
		case konec:
			break;
		case volna_jizda:
			break;			
	}
}

void GameCycle()
{
	// Update the sprites
	_pGame->UpdateSprites();

	// Obtain a device context for repainting the game
	HWND  hWindow = _pGame->GetWindow();
	HDC   hDC = GetDC(hWindow);	
	int	  nahDelo;

	// Paint the game to the offscreen device context
	GamePaint(_hOffscreenDC);

	// Blit the offscreen bitmap to the game screen
	BitBlt(hDC, 0, 0, _pGame->GetWidth(), _pGame->GetHeight(),_hOffscreenDC, 0, 0, SRCCOPY);


	// Cleanup
	ReleaseDC(hWindow, hDC);
	
	switch (_stavHry)
	{
		case menu:
			if (rand()%100 == 0)
			{
				RECT rcBounds = {-100,-100,750,580};
				int pozice = (rand()%350)+40;
			
    			Sprite* pSprite = new Sprite(_obrJet,rcBounds,BA_DIE);
    			pSprite->SetNumFrames(1);
    			pSprite->SetFramesRange(1,1);
    			pSprite->SetPosition(-100,pozice) ;
    			pSprite->SetVelocity(11,0);
    			pSprite->SetZOrder(5);
    			_pGame->AddSprite(pSprite);				
			}			
			break;
		case volna_jizda:
			nahDelo = rand()%100;
			
			// Zobrazeni bonusu
			if ( (nahDelo == 10) && (!_bBonus) )
			{
				_bonus = new Sprite(_obrBonusy);
				_bonus->SetNumFrames(3);

//				if ( (rand()%2) == 1) _bonus->SetFramesRange(2,2);
//					else _bonus->SetFramesRange(1,1);
					
				int cislo = (rand()%3) + 1;
				_bonus->SetFramesRange(cislo,cislo);				
				
				_bonus->SetPosition( rand()%490 + 60,rand()%340 + 60);
				_pGame->AddSprite(_bonus);
				_bBonus = true;
			}

			if ( (nahDelo < _pocetDel)  && ( _dela[nahDelo]->GetPuvodniStav()) && (!_dela[nahDelo]->GetZniceno()) )
			{
		 		if (_iPocetRaket<4)
				{
					++_iPocetRaket;
					_dela[nahDelo]->SetVysunout(true);
				}
			}
				
			if (_auto->IsInBonus(B_STRELY))
				if (++_iCasovac / _pGame->GetFrameDelay() == 20) 
				{
					_auto->OdstranBonus(B_STRELY);
					_iCasovac = 0;
				}					
			break;
		default:
			break;
	}	
		
	CheckujDela();	
}

void HandleKeys()
{
	if (++_iEscDelay>8 && GetAsyncKeyState(VK_ESCAPE) < 0)
		{
			switch (_stavHry)
			{
				case pribeh:
					_stavHry = hra;
					break; 
				default: 
					_stavHry = menu;
			}
			_iEscDelay = 0; 
		}
	
	if ( _bExistCar && ++_iInputDelay > 2)
		{
			if (GetAsyncKeyState(VK_UP) == 0 && GetAsyncKeyState(VK_DOWN) == 0) _auto->Setrvacnost();

			if (GetAsyncKeyState(VK_UP) < 0) _auto->Pohyb(plyn);
			if (GetAsyncKeyState(VK_DOWN) < 0) _auto->Pohyb(brzda);
			if (GetAsyncKeyState(VK_RIGHT) < 0) _auto->Pohyb(doprava);
			if (GetAsyncKeyState(VK_LEFT) < 0) _auto->Pohyb(doleva);
			_iInputDelay = 0;
		}

	if (_stavHry == pribeh && ++_iPribehDelay>7 && GetAsyncKeyState(VK_RETURN) < 0)
		{
			if (_stavPribehu == 27) _stavHry = hra;
			 _stavPribehu++;
			_iPribehDelay = 0;
		}

	if (_stavHry == hra && ++_iHraDelay>7 && GetAsyncKeyState(VK_RETURN) < 0)
		{
			 _mise++;
			_iHraDelay = 0;
		}
		
 	if (_stavHry == volna_jizda && ++_iFireDelay > _auto->GetRychlostStrel() && GetAsyncKeyState(VK_CONTROL) < 0)
 		{
			_auto->Vystrel();	
		 	_iFireDelay = 0;
		}
}

void SpriteDying(Sprite* pSprite)
{

  	if (pSprite->GetBitmap() == _obrRaketa1 || pSprite->GetBitmap() == _obrRaketa2 || pSprite->GetBitmap() == _obrRaketa3 || pSprite->GetBitmap() == _obrRaketa4)
  		{
    		// Play the explosion sound
			Mix_PlayChannel(-1,_mSounds,0);
			--_iPocetRaket;

			if (!pSprite->GetCollisionSprite())
				{
    				// Create an explosion sprite at the meteor's position
    				RECT rcPos = pSprite->GetPosition();
    
    				Sprite* pSprite = new Sprite(_obrExploze);
    				pSprite->SetNumFrames(8,1,TRUE);
    				pSprite->SetPosition(rcPos.left-5, rcPos.top); //28
					pSprite->SetFramesRange(1,8);
					pSprite->SetZOrder(1);
    				_pGame->AddSprite(pSprite);
				}
				    
			_dela[pSprite->GetIndex()]->SetZasunout(true);			
  		}


  	if (pSprite->GetBitmap() == _obrAuto)
  		{
    		RECT rcPos;
      		rcPos = pSprite->GetPosition();
      		
    		Sprite* pSprite = new Sprite(_obrBigExploze);
    		pSprite->SetNumFrames(8,1,TRUE);
    		pSprite->SetPosition(rcPos.left, rcPos.top);
    		pSprite->SetFramesRange(1,8);
    		pSprite->SetZOrder(2);
    		_pGame->AddSprite(pSprite);
		}

   	if (pSprite->GetBitmap() == _obrStrela) 
 		{
    		RECT rcPos;
      		rcPos = pSprite->GetPosition();

    		Sprite* pSprite = new Sprite(_obrExpStrela);
    		pSprite->SetNumFrames(4,1,TRUE);
    		pSprite->SetPosition(rcPos.left-4, rcPos.top-4);
    		pSprite->SetFramesRange(1,4);
    		_pGame->AddSprite(pSprite);
		}
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
	if(_stavHry == menu)
		{	
			if (_horniMenu->IsMouseUp(x,y) == 1 && bLeft) GameEnd();
			if (_flagCZ->IsPointInside(x,y))
				{
					CreateTable("cz.dat");
					RefreshCaptions();
				}
			if (_flagUS->IsPointInside(x,y))
				{
					CreateTable("us.dat");
					RefreshCaptions();
				}	

			if (_stavHry == menu)
				{
					if (_hlavniMenu->IsMouseUp(x,y) == 1 && bLeft) 
						{
							_hlavniMenu->SetAllItems_color(RGB(30,30,30));
							NewFreeGame();
						}
				}
		}
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
	if(_stavHry == menu || _stavHry == nastaveni)
		{	
			if (_flagCZ->IsPointInside(x,y)) _flagCZ->SetFramesRange(2,2,1);
				else _flagCZ->SetFramesRange(1,1,1);

			if (_flagUS->IsPointInside(x,y)) _flagUS->SetFramesRange(2,2,2);
				else _flagUS->SetFramesRange(1,1,2);
		
			int pom = _horniMenu->IsMouseUp(x,y);
	
			if (pom > -1) _horniMenu->SetItem_color(pom,RGB(100,0,0),RGB(30,30,30));
				else _horniMenu->SetAllItems_color(RGB(30,30,30));
				
			if (_stavHry == nastaveni)
				{
					int pom = _nastaveniMenu->IsMouseUp(x,y);
	
					if (pom > -1) _nastaveniMenu->SetItem_color(pom,RGB(100,0,0),RGB(30,30,30));
						else _nastaveniMenu->SetAllItems_color(RGB(30,30,30));
				}

			if (_stavHry == menu)
				{
					int pom = _hlavniMenu->IsMouseUp(x,y);
	
					if (pom > -1) _hlavniMenu->SetItem_color(pom,RGB(100,0,0),RGB(30,30,30));
						else _hlavniMenu->SetAllItems_color(RGB(30,30,30));
				}
		}
}

void HandleJoystick(JOYSTATE jsJoystickState)
{
}


BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
	Bitmap* pHitter = pSpriteHitter->GetBitmap();
  	Bitmap* pHittee = pSpriteHittee->GetBitmap();
  	
  	if ( (pHitter == _obrKamen) && (pHittee == _obrAuto) )
  	{
  		_auto->Zastav();
  		return true;
  	}
  	
	if ( (pHitter == _obrAuto) && (pHittee == _obrRaketa1 || pHittee == _obrRaketa2 || pHittee == _obrRaketa3 || pHittee == _obrRaketa4)  )
	  	{
			if (_auto->IsInBonus(B_STIT)) 
				{
					pSpriteHittee->Kill();
					_auto->OdstranBonus(B_STIT);
				}
				else
					{				
						pSpriteHittee->SetCollisionSprite(true);
						
						pSpriteHitter->Kill();
						pSpriteHittee->Kill(); 
			
						_stavHry = menu;
//						Mix_FadeOutMusic(2000);
						
						_flagCZ = new Sprite(_obrVlajky);
					    _flagCZ->SetNumFrames(2,2);
						_flagCZ->SetPosition(10,10);
					    _flagCZ->SetFramesRange(1,1,1);
					    _pGame->AddSprite(_flagCZ);    		

						_flagUS = new Sprite(_obrVlajky);
					    _flagUS->SetNumFrames(2,2);
						_flagUS->SetPosition(40,8);
					    _flagUS->SetFramesRange(1,1,2);
					    _pGame->AddSprite(_flagUS);
					    
//						Mix_PlayMusic(_musicMenu,-1);
						_bExistCar = false;
					}
		}

	if ( (pHitter == _obrAuto) && (pHittee == _obrBonusy)  )
	  	{
			switch (pSpriteHittee->GetCurFrame())
			{
				case 0:			
					_auto->PridejBonus(B_STIT);									
					break;
				case 1:
					_auto->PridejBonus(B_STRELY);
					break;
				case 2:
					_auto->PridejBonus(B_VICESTREL);
					break;
			}

			_bBonus = false;
			pSpriteHittee->Kill();
		}


	if ( (pHittee == _obrStrela) && ( (pHitter == _obrDelo1) || (pHitter == _obrDelo2) || (pHitter == _obrDelo3) || (pHitter == _obrDelo4)) ) 
	  	{
			if (!_dela[pSpriteHitter->GetIndex()]->GetPuvodniStav())
				{					
					if (_dela[pSpriteHitter->GetIndex()]->GetZniceno() == false)
						if (--_iCurPocetDel == 0) _stavHry = konec;
						
					_dela[pSpriteHitter->GetIndex()]->SetZniceno(true);
				}
				
			pSpriteHittee->Kill(); 
		}

	if ((pHittee == _obrStrela) && (pHitter == _obrRaketa1 || pHitter == _obrRaketa2 || pHitter == _obrRaketa3 || pHitter == _obrRaketa4)) 
		{
			pSpriteHittee->SetCollisionSprite(true);
			pSpriteHittee->Kill();
		}
	if ((pHitter == _obrStrela) && (pHittee == _obrRaketa1 || pHittee == _obrRaketa2 || pHittee == _obrRaketa3 || pHittee == _obrRaketa4)) 
		{
			pSpriteHitter->SetCollisionSprite(true);
			pSpriteHitter->Kill(); 
		}		
		
	return false;
}

// Main methods
void CheckujDela()
{	
	for(int i=0;i<_pocetDel;i++)
		{
			if  (_dela[i]->GetVysunout()) 	_dela[i]->VysunDelo();
			if  (_dela[i]->GetStrelba()) 	_dela[i]->Vystrel();
			if  (_dela[i]->GetZasunout())	_dela[i]->ZasunDelo();
			if  (_dela[i]->GetZniceno())	_dela[i]->ZnicDelo();
		}

}

void DrawSplashScreen(HDC hDC)
{
	_obrSplash->Draw(hDC,0,0,TRUE);

	SelectObject(hDC, _font);

	SetBkMode(hDC,TRANSPARENT);
	SetTextColor(hDC,RGB(0,0,0));
	
	TextOut(hDC, (_XTitulky -= 2) < -800 ? _XTitulky = 650 : _XTitulky = _XTitulky ,440,TEXT(_titulky.data()),_titulky.length());
			
	_horniMenu->Draw(hDC);
	_hlavniMenu->Draw(hDC);	
}

void DrawStoryScreen(HDC hDC)
{	
	RECT joeTalk;
	RECT mitkinsTalk;
	
	if (_stavPribehu<12)
		{
			joeTalk.left = 50;
			joeTalk.top = 170;
			joeTalk.right = 170;
			joeTalk.bottom = 370;
			
			mitkinsTalk.left = 180;
			mitkinsTalk.top = 170;
			mitkinsTalk.right = 280;
			mitkinsTalk.bottom = 370;
		}
	else
		{
			joeTalk.left = 180;
			joeTalk.top = 130;
			joeTalk.right = 280;
			joeTalk.bottom = 200;
			
			mitkinsTalk.left = 180;
			mitkinsTalk.top = 270;
			mitkinsTalk.right = 280;
			mitkinsTalk.bottom = 370;
		}
	
	SelectObject(hDC, _storyFont);
	
	switch (_stavPribehu)
	{
		case 0:
			DrawText(hDC,TEXT(Translate("STRSTO001").data()),-1,&joeTalk,DT_VCENTER|DT_WORDBREAK);
			break;
		case 1:
			_pGame->AddSprite(_mitkins);
			_stavPribehu++;
			break;
		case 2:
			if (_mitkins->GetPosition().left == 200)
				{
					_mitkins->SetVelocity(0,0);
					_mitkins->SetFramesRange(1,1,1);
					_stavPribehu++;
				}			
			break;
		case 3:
			_mitkins->SetPosition(200,230);
			_mitkins->SetVelocity(0,0);
			_mitkins->SetFramesRange(1,1,1);
			
			DrawText(hDC,TEXT(Translate("STRSTO002").data()),-1,&mitkinsTalk,DT_VCENTER|DT_WORDBREAK);
			break;
		case 4:
			DrawText(hDC,TEXT(Translate("STRSTO003").data()),-1,&joeTalk,DT_VCENTER|DT_WORDBREAK);
			break;
		case 5:
			DrawText(hDC,TEXT(Translate("STRSTO004").data()),-1,&mitkinsTalk,DT_VCENTER|DT_WORDBREAK);
			break;
		case 6:
			DrawText(hDC,TEXT(Translate("STRSTO005").data()),-1,&mitkinsTalk,DT_VCENTER|DT_WORDBREAK);
			break;
		case 7:
			_mitkins->SetFramesRange(1,4,2);
			_stavPribehu++;
			break;
		case 8:
			if (_mitkins->GetCurFrame() == 3)
				{
					_mitkins->SetVelocity(2,0);
					_mitkins->SetFramesRange(1,8,1);

					_joe->SetVelocity(2,0);
					_joe->SetFramesRange(1,8);
					_stavPribehu++;		
				}
			break;
		case 9:
			if (_joe->GetPosition().left > 650)
				{
					_mitkins->SetPosition(-20,240);
					_joe->SetPosition(-20,210);
					_stavPribehu++;
				}
			break;
		case 10:
			_mitkins->SetPosition(-20,240);
			_joe->SetPosition(-20,210);
			_stavPribehu++;
			break;
		case 11:							
			if (_mitkins->GetPosition().left > 250)
				{
					_mitkins->SetVelocity(0,0);
					_mitkins->SetFramesRange(1,1,1);
					
					_joe->SetVelocity(0,0);
					_joe->SetFramesRange(1,1);
					_stavPribehu++;
				}
			break;
		case 12:
			_mitkins->SetVelocity(0,0);
			_mitkins->SetFramesRange(1,1,1);
			_mitkins->SetPosition(250,240);
			
			_joe->SetVelocity(0,0);
			_joe->SetFramesRange(1,1);
			_joe->SetPosition(250,210);
			
			DrawText(hDC,TEXT(Translate("STRSTO006").data()),-1,&mitkinsTalk,DT_VCENTER|DT_WORDBREAK);			
			break;
		case 13:
			DrawText(hDC,TEXT(Translate("STRSTO007").data()),-1,&joeTalk,DT_VCENTER|DT_WORDBREAK);			
			break;
		case 14:
			DrawText(hDC,TEXT(Translate("STRSTO008").data()),-1,&mitkinsTalk,DT_VCENTER|DT_WORDBREAK);			
			break;
		case 15:
			DrawText(hDC,TEXT(Translate("STRSTO009").data()),-1,&mitkinsTalk,DT_VCENTER|DT_WORDBREAK);			
			break;
		case 16:
			DrawText(hDC,TEXT(Translate("STRSTO010").data()),-1,&mitkinsTalk,DT_VCENTER|DT_WORDBREAK);			
			break;
		case 17:
			DrawText(hDC,TEXT(Translate("STRSTO011").data()),-1,&mitkinsTalk,DT_VCENTER|DT_WORDBREAK);			
			break;
		case 18:
			DrawText(hDC,TEXT(Translate("STRSTO012").data()),-1,&mitkinsTalk,DT_VCENTER|DT_WORDBREAK);			
			break;
		case 19:
			DrawText(hDC,TEXT(Translate("STRSTO013").data()),-1,&mitkinsTalk,DT_VCENTER|DT_WORDBREAK);			
			break;
		case 20:
			DrawText(hDC,TEXT(Translate("STRSTO014").data()),-1,&mitkinsTalk,DT_VCENTER|DT_WORDBREAK);			
			break;
		case 21:
			DrawText(hDC,TEXT(Translate("STRSTO015").data()),-1,&mitkinsTalk,DT_VCENTER|DT_WORDBREAK);			
			break;
		case 22:
			DrawText(hDC,TEXT(Translate("STRSTO016").data()),-1,&joeTalk,DT_VCENTER|DT_WORDBREAK);			
			break;
		case 23:
			DrawText(hDC,TEXT(Translate("STRSTO017").data()),-1,&joeTalk,DT_VCENTER|DT_WORDBREAK);			
			break;
		case 24:
			DrawText(hDC,TEXT(Translate("STRSTO018").data()),-1,&joeTalk,DT_VCENTER|DT_WORDBREAK);			
			break;
		case 25:
			DrawText(hDC,TEXT(Translate("STRSTO019").data()),-1,&mitkinsTalk,DT_VCENTER|DT_WORDBREAK);			
			break;
		case 26:
			DrawText(hDC,TEXT(Translate("STRSTO020").data()),-1,&mitkinsTalk,DT_VCENTER|DT_WORDBREAK);			
			break;
		case 27:
			DrawText(hDC,TEXT(Translate("STRSTO021").data()),-1,&mitkinsTalk,DT_VCENTER|DT_WORDBREAK);			
			break;
	}
}

void DrawGameScreen(HDC hDC)
{
	RECT nadpis = {0,0,640,200};
	RECT popis = {0,320,640,500};
	
	switch (_mise)
	{
		case 0:
			_pGame->CleanupSprites();
			_mise++;
			break;
		case 1:
			SelectObject(hDC,_gameFont);	
			DrawText(hDC,TEXT(Translate("STRGAME00").data()),-1,&nadpis,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
			SelectObject(hDC,_font);	
			DrawText(hDC,TEXT(Translate("STRGAME01").data()),-1,&popis,DT_VCENTER|DT_SINGLELINE|DT_CENTER);			
			break;
		case 2:
		{
			for(int i=0;i<5;i++) _pGame->AddSprite(_kameny[i]);
			RECT okraj = {0,0,640,480};
			_auto = new Auto(28,430,_obrAuto,_obrStrela,&okraj);
			_bExistCar = true;
			_pGame->AddSprite(_A);
			_pGame->AddSprite(_B);
			_mise++;
		}
			break;
		case 3:
  			time ( &rawtime );
  			timeinfo = localtime ( &rawtime );
  			strftime (buffer,3,"%S",timeinfo);

			TextOut(hDC, 150 ,25,buffer,2);			
			break;
	}
	
}

void DrawSettingsScreen(HDC hDC)
{
	_obrSplash->Draw(hDC,0,0,TRUE);

	SelectObject(hDC, _font);

	SetBkMode(hDC,TRANSPARENT);
	SetTextColor(hDC,RGB(0,0,0));	
	
	TextOut(hDC, (_XTitulky -= 2) < -800 ? _XTitulky = 650 : _XTitulky = _XTitulky ,440,TEXT(_titulky.data()),_titulky.length());

	_nastaveniMenu->Draw(hDC);
	_horniMenu->Draw(hDC);

	RECT pokus1 = {200,247,280,273};
	DrawText(hDC,Translate("STRTIT003").data(),-1,&pokus1,DT_CENTER);

	RECT pokus2 = {200,277,280,302};
	DrawText(hDC,Translate("STRTIT004").data(),-1,&pokus2,DT_CENTER);


	char po[2];
	string lol;
	RECT pokus3 = {325,249,350,279};
	
//	itoa(g_volume/10,po,10);	
	lol = po;
//	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
//	FillRect(hDC,&pokus3,brush);

	
	DrawText(hDC,lol.data(),-1,&pokus3,DT_CENTER|DT_VCENTER);
//	TextOut(hDC, 332 ,250,lol.data(),lol.length());
}


void NewFreeGame()
{
	_iCurPocetDel = _pocetDel;
	_stavHry = volna_jizda;
	
	_iPocetRaket = 0;
	RECT okraje = {50,55,585,430};	
	
	
//	Mix_VolumeMusic(g_volume);
//	Mix_PlayMusic(_musicGame, -1);

	_auto = new Auto(320,240,_obrAuto,_obrStrela,&okraje);
	_bExistCar = true;
	
	for(int i=0;i<_pocetDel;i++)
		_dela[i]->ObnovDelo();

	_flagCZ->Kill();
	_flagUS->Kill();
}

void NewGame()
{
	_stavHry = pribeh;
	_flagCZ->Kill();
	_flagUS->Kill();
}

int Approach(int what, int where)
{
	if (what>where) what--;
	if (what<where) what++;
	return what; 
}


string Translate(string t)
{
	return _TextMap[t];
}

void CreateTable(char* p)
{
	_TextMap.clear();
	
	ifstream os(p);
	
	string _key,_pom;
	char _data[300];
	
	os>>_key;

	while (os != 0)
	{
		os>>_pom;
		os.getline(_data,300);
		
		_pom += _data;	
		_TextMap[_key] = _pom;
		
		os>>_key;	
	}
	
	os.close();
}

void RefreshCaptions()
{
	_horniMenu->SetItem_caption(0,Translate("STRMENU02"));
	_horniMenu->SetItem_caption(1,Translate("STRMENU03"));
	_titulky = Translate("STRTIT001")+": Pavel Vlašánek, "+Translate("STRTIT002")+": Pavel Vlašánek, " + Translate("STRTIT003")+": Roman Antala";		
	
	_hlavniMenu->SetItem_caption(0," - " + Translate("STRMENU07") + " - ");
	_hlavniMenu->SetItem_caption(1," - " + Translate("STRMENU06") + " - ");
	_hlavniMenu->SetItem_caption(2," - " + Translate("STRMENU08") + " - ");
}

//-----------------------------------------------------------------
// SDL
//-----------------------------------------------------------------
bool InitSDL()
{
	// Inicializace SDL
	if(SDL_Init(SDL_INIT_AUDIO) == -1)
	{
		return false;
	}
	
	// Audio
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,	MIX_DEFAULT_FORMAT, 2, 2048) == -1)   //2048
	{
		return false;
	}
	
	return true;
}


//-----------------------------------------------------------------
// Class Auto
//-----------------------------------------------------------------
Auto::Auto(int x, int y, Bitmap* b, Bitmap* s, RECT* o)
{
	b_strela = s;
	
//	RECT okraje = {50,55,585,430};	
	
	i_uhel = 1;
	_iRadek = 1;
	_iRychlostStrel = 9;
	
	s_auto = new Sprite(b,*o);
	s_auto->SetPosition(x,y);
	s_auto->SetNumFrames(8,2);
	s_auto->SetFramesRange(1,1,_iRadek);
	s_auto->SetZOrder(2);
	_pGame->AddSprite(s_auto);
}

Auto::~Auto()
{
}

void Auto::Setrvacnost()
{
	int _rychlostX = (int)s_auto->GetVelocity().x;
	int _rychlostY = (int)s_auto->GetVelocity().y;
	
	if (_rychlostX < 0) ++_rychlostX;
	if (_rychlostX > 0) --_rychlostX;

	if (_rychlostY < 0) ++_rychlostY;
	if (_rychlostY > 0) --_rychlostY;
	
	s_auto->SetVelocity(_rychlostX,_rychlostY);
//	s_auto->SetVelocity( min((int)s_auto->GetVelocity().x+1,0),min((int)s_auto->GetVelocity().y+1,0) );
}

void Auto::Pohyb(ePohyb kam)
{
	RECT position = s_auto->GetPosition();
	POINT rychlost = s_auto->GetVelocity();	
	
	switch (kam)
	{
		case doprava:
				if (++i_uhel > 8) i_uhel = 1;
//				s_auto->SetVelocity(Approach(rychlost.x,0),Approach(rychlost.y,0));
			break;			
		case doleva:
				if (--i_uhel < 1) i_uhel = 8;
//				s_auto->SetVelocity(Approach(rychlost.x,0),Approach(rychlost.y,0));
			break;
		case plyn:
				switch (i_uhel)
				{
					case 1:
							s_auto->SetVelocity(Approach(rychlost.x,0), max((int)rychlost.y-1,-8));
  							e_kam = nahoru;
						break;
					case 2:
							s_auto->SetVelocity(min((int)rychlost.x+1,8),max((int)rychlost.y-1,-8)); //nastavit -2 u x nebo y
  							e_kam = nahoru;
						break;
					case 3:
							s_auto->SetVelocity(min((int)rychlost.x+1,8),Approach(rychlost.y,0));
  							e_kam = dolu;
						break;
					case 4:
							s_auto->SetVelocity(min((int)rychlost.x+1,8),min((int)rychlost.y+1,8));
  							e_kam = dolu;
						break;
					case 5:
							s_auto->SetVelocity(Approach(rychlost.x,0),min((int)rychlost.y+1,8));
  							e_kam = dolu;
						break;
					case 6:
							s_auto->SetVelocity(max((int)rychlost.x-1,-8),min((int)rychlost.y+1,8));
  							e_kam = dolu;
						break;
					case 7:
							s_auto->SetVelocity(max((int)rychlost.x-1,-8),Approach(rychlost.y,0));
  							e_kam = nahoru;
						break;
					case 8:
							s_auto->SetVelocity(max((int)rychlost.x-1,-8),max((int)rychlost.y-1,-8));
  							e_kam = nahoru;
						break;
				}
			break;
		case brzda:
				switch (i_uhel)
				{
					case 1:
							s_auto->SetVelocity(Approach(rychlost.x,0), min((int)rychlost.y+1,8));
  							e_kam = nahoru;
						break;
					case 2:
							s_auto->SetVelocity(max((int)rychlost.x-1,-8),min((int)rychlost.y+1,8));
  							e_kam = nahoru;
						break;
					case 3:
							s_auto->SetVelocity(max((int)rychlost.x-1,-8),Approach(rychlost.y,0));
  							e_kam = dolu;
						break;
					case 4:
							s_auto->SetVelocity(max((int)rychlost.x-1,-8),max((int)rychlost.y-1,-8));
  							e_kam = dolu;
						break;
					case 5:
							s_auto->SetVelocity(Approach(rychlost.x,0),max((int)rychlost.y-1,-8));
  							e_kam = dolu;
						break;
					case 6:
							s_auto->SetVelocity(min((int)rychlost.x+1,8),max((int)rychlost.y-1,-8));
  							e_kam = dolu;
						break;
					case 7:
							s_auto->SetVelocity(min((int)rychlost.x+1,8),Approach(rychlost.y,0));
  							e_kam = nahoru;
						break;
					case 8:
							s_auto->SetVelocity(min((int)rychlost.x+1,8),min((int)rychlost.y+1,8));
  							e_kam = nahoru;
						break;
				}
			break;
	}

	s_auto->SetFramesRange(i_uhel,i_uhel,_iRadek);
}

void Auto::Vystrel()
{
	RECT okraje = {40,40,600,440};
	RECT pozAuto = s_auto->GetPosition();
	
	Sprite* pSprite = new Sprite(b_strela,okraje,BA_DIE);

	switch (i_uhel)
	{
		case 1:
				pSprite->SetPosition(pozAuto.left+14,pozAuto.top); //14 0
				pSprite->SetVelocity(0,-10);
			break;
		case 2:
				pSprite->SetPosition(pozAuto.right-8,pozAuto.top+5); //-8 5
				pSprite->SetVelocity(10,-10);  //0 -10
			break;
		case 3:
				pSprite->SetPosition(pozAuto.right,pozAuto.top+13);
				pSprite->SetVelocity(10,0);
			break;
		case 4:
				pSprite->SetPosition(pozAuto.right-10,pozAuto.bottom-10);
				pSprite->SetVelocity(10,10);  //0 -10
			break;
		case 5:
				pSprite->SetPosition(pozAuto.left+13,pozAuto.bottom-7);
				pSprite->SetVelocity(0,10);
			break;
		case 6:
				pSprite->SetPosition(pozAuto.left,pozAuto.bottom-10);
				pSprite->SetVelocity(-10,10);  //0 -10
			break;
		case 7:
				pSprite->SetPosition(pozAuto.left-5,pozAuto.top+13);
				pSprite->SetVelocity(-10,0);
			break;
		case 8:
				pSprite->SetPosition(pozAuto.left+2,pozAuto.top+2);
				pSprite->SetVelocity(-10,-10);  //0 -10
			break;
	}
		
//	pSprite->SetZOrder(1);
	pSprite->SetNumFrames(1);
	pSprite->SetFramesRange(1,1);
	
	if (_sBonusy.count(B_STRELY))
		{
			Sprite* pSprite1 = new Sprite(b_strela,okraje,BA_DIE);
			Sprite* pSprite2 = new Sprite(b_strela,okraje,BA_DIE);
			
			pSprite1->SetPosition(pSprite->GetPosition());
			pSprite1->SetNumFrames(1);
			pSprite1->SetFramesRange(1,1);

			pSprite2->SetPosition(pSprite->GetPosition());
			pSprite2->SetNumFrames(1);
			pSprite2->SetFramesRange(1,1);

			switch (i_uhel)
			{
				case 1:
						pSprite1->SetVelocity(-10,-10);
						pSprite2->SetVelocity(10,-10);
					break;
				case 2:
						pSprite1->SetVelocity(0,-10);
						pSprite2->SetVelocity(10,0);
					break;
				case 3:
						pSprite1->SetVelocity(10,-10);
						pSprite2->SetVelocity(10,10);
					break;
				case 4:
						pSprite1->SetVelocity(10,0);
						pSprite2->SetVelocity(0,10);
					break;
				case 5:
						pSprite1->SetVelocity(10,10);
						pSprite2->SetVelocity(-10,10);
					break;
				case 6:
						pSprite1->SetVelocity(0,10);
						pSprite2->SetVelocity(-10,0);
					break;
				case 7:
						pSprite1->SetVelocity(-10,10);
						pSprite2->SetVelocity(-10,-10);
					break;
				case 8:
						pSprite1->SetVelocity(-10,0);
						pSprite2->SetVelocity(0,-10);
					break;
			}
			
			_pGame->AddSprite(pSprite1);
			_pGame->AddSprite(pSprite2);
		}	
	
	_pGame->AddSprite(pSprite);
}

void Auto::PridejBonus(BONUS b)
{
	_sBonusy.insert(b);
	
	switch (b)
	{
		case B_STIT:
			s_auto->SetFramesRange(i_uhel,i_uhel,2);
			_iRadek = 2;
			break;
		case B_VICESTREL:
			_iRychlostStrel = 4;
			break;
	}
}

void Auto::OdstranBonus(BONUS b)
{
	_sBonusy.erase(b);
	
	switch (b)
	{
		case B_STIT:
			s_auto->SetFramesRange(i_uhel,i_uhel,1);
			_iRadek = 1;
			break;
	}
}

//-----------------------------------------------------------------
// Class Delo
//-----------------------------------------------------------------
Delo::~Delo()
{
	delete	b_raketa;
}

Delo::Delo(Bitmap* d,Bitmap* r,int pozX,int pozY,int index, ePoziceDela pozDela)
{
	X = pozX;
	Y = pozY;
	
	strelba = false;
	zasunout = false;
	vysunout = false;
	zniceno = false;
	puvodniStav = true;
	
	dalsi = false;
	
	kdeJeDelo = pozDela;
		                                                                                        
	m_delo = new Sprite(d);	
	m_delo->SetNumFrames(9);
	m_delo->SetFramesRange(1,1);
	m_delo->SetPosition(pozX,pozY);  //282, 30
	m_delo->SetIndex(index);
	_pGame->AddSprite(m_delo);	

	b_raketa = r;
	i_cisloDela = index;
}

void Delo::Vystrel()
{
	RECT okraje = {40,40,600,440};
	
	Sprite* pSprite = new Sprite(b_raketa,okraje,BA_DIE);
	
	if (GetKdeJeDelo() == nahore)
		{
			pSprite->SetPosition(X+4,Y+15);  //276,114  295,33
			pSprite->SetVelocity(0,10);
		}
	if (GetKdeJeDelo() == dole)
		{
			pSprite->SetPosition(X+4,Y-15);  //276,114  295,33
			pSprite->SetVelocity(0,-10);
		}
	if (GetKdeJeDelo() == vlevo)
		{
			pSprite->SetPosition(X+15,Y+4);  //276,114  295,33
			pSprite->SetVelocity(10,0);
		}

	if (GetKdeJeDelo() == vpravo)
		{
			pSprite->SetPosition(X-15,Y+4);  //276,114  295,33
			pSprite->SetVelocity(-10,0);
		}
		
	pSprite->SetNumFrames(1);
	pSprite->SetFramesRange(1,1);
	pSprite->SetIndex(i_cisloDela);
	pSprite->SetZOrder(1);
	_pGame->AddSprite(pSprite);
	
	strelba = false;
}

void Delo::ZasunDelo()
{
	if (m_delo->GetCurFrame() == 3) m_delo->SetFramesRange(5,8);
	if (m_delo->GetCurFrame() == 7) 
		{
			m_delo->SetFramesRange(1,1);
			zasunout = false;
			strelba = false;
			
			puvodniStav = true;
		}	
}

void Delo::VysunDelo()
{
	puvodniStav = false;
	
	/******************CHYBA****************************************************
	Pokud uzivatel stiskne ENTER a zapocne nova hra jeste pred tim, nez
	vybouchnou vsechny vystrelene rakety nastane chyba - dela se obnovi
	a vse se uvede do puvodniho stavu. Ovsem po vybuchu rakety se vlastnost
	"zasunout" u konkretniho dela nastavi na "true". Z toho duvodu	je
	treba dopsat na tomto miste "zasunout = false"
	***************************************************************************/

	zasunout = false;
	
	if (m_delo->GetCurFrame() == 0) m_delo->SetFramesRange(1,4);
	if (m_delo->GetCurFrame() == 3) 
		{
			m_delo->SetFramesRange(4,4);
			strelba = true;			
			vysunout = false;
		}
}

void Delo::ZnicDelo()
{
	m_delo->SetFramesRange(9,9);
}

void Delo::SetZasunout(bool b)
{
	zasunout = b;
}

void Delo::SetStrelba(bool b)
{
	strelba = b;
}

void Delo::ObnovDelo()
{
	m_delo->SetFramesRange(1,1);
	strelba = false;
	zasunout = false;
	vysunout = false;
	zniceno = false;
	puvodniStav = true;
	
	dalsi = false;
}
