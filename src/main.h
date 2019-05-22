//-----------------------------------------------------------------
// Main                   
// C++ Header - Main. h          
//----------------------------------------------------------------- 

#pragma once
       
//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <map>
#include <set>
#include <fstream>
#include <windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Background.h"
#include "TextMenu.h"


#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <time.h>


using namespace std;


//-----------------------------------------------------------------
// Custom Data Types
//-----------------------------------------------------------------
enum ePohyb{plyn,brzda,doleva,doprava};
enum eSmer{nahoru,dolu};
enum ePoziceDela{nahore,dole,vlevo,vpravo};
enum eStavHry{menu,nastaveni,volna_jizda,konec,pribeh,hra};

typedef map<string,string> TPole;

typedef WORD	BONUS;
const BONUS		B_STIT		= 0,
        		B_STRELY	= 1,
        		B_VICESTREL	= 2;

//-----------------------------------------------------------------
// Class Auto
//-----------------------------------------------------------------
class Auto
{
private:
	Sprite*		s_auto;
	Bitmap*		b_strela;
	int			i_uhel; //snimek natoceni auta
	eSmer		e_kam;
	int			_iRadek;
	set<BONUS>	_sBonusy;
	int			_iRychlostStrel;
	
public:
	// Constructor(s)/Destructor
	Auto(int,int,Bitmap*, Bitmap*, RECT*);
	~Auto();
  	// General Methods
  	void Pohyb(ePohyb);
  	void Setrvacnost();
  	void Vystrel();
	bool IsInBonus(BONUS b)		{if (_sBonusy.count(b)) return true; else return false;};
	void Zastav()				{s_auto->SetVelocity(0,0);};
  	
	// Acces Methods
  	void SetKam(eSmer e) 		{e_kam = e;};
  	void PridejBonus(BONUS b);
  	void OdstranBonus(BONUS b);
  	int	GetRychlostStrel()		{return _iRychlostStrel;};

  	eSmer GetKam() {return e_kam;};
};

//-----------------------------------------------------------------
// Class Delo
//-----------------------------------------------------------------
class Delo
{
private:
  	// Member Variables
  	Sprite* 		m_delo;
  	Bitmap*			b_raketa;
  	int				X,Y;
  	bool			strelba,zasunout,vysunout;
  	int				i_cisloDela;
  	bool			zniceno;
  	bool			dalsi;
  	bool			puvodniStav;
  	ePoziceDela 	kdeJeDelo;
  
public:
	// Constructor(s)/Destructor
	Delo(Bitmap*,Bitmap*, int, int, int, ePoziceDela);
  	~Delo();
  	// General Methods
  	void Vystrel();
  	void ZasunDelo();
  	void VysunDelo();
  	void ZnicDelo();
  	void ObnovDelo();
	  
	// Acces Methods
  	bool GetZasunout()			{return	zasunout;};
  	bool GetStrelba()			{return strelba;};
  	bool GetVysunout() 			{return vysunout;};
  	bool GetPuvodniStav() 		{return puvodniStav;};
  	bool GetZniceno() 			{return zniceno;};
  	ePoziceDela GetKdeJeDelo()	{return kdeJeDelo;};  	
  	
	void SetZasunout(bool);
	void SetStrelba(bool);  	
	void SetVysunout(bool b) 	{vysunout = b;};
	void SetPuvodniStav(bool b) {puvodniStav = b;};
	void SetZniceno(bool b) 	{zniceno = b;};
};

//-----------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------
GameEngine* 	_pGame;

TPole 			_TextMap;

HINSTANCE		_hInstance;	

HDC             _hOffscreenDC;
HBITMAP         _hOffscreenBitmap;

const int		_pocetDelNahore = 5;
const int		_pocetDelDole = 4;
const int		_pocetDelVlevo = 4;
const int		_pocetDelVpravo = 3;
const int		_pocetDel = _pocetDelNahore + _pocetDelDole + _pocetDelVlevo + _pocetDelVpravo;

int				_iCurPocetDel = _pocetDel;
int				_iInputDelay = 0;
int				_iFireDelay = 0;
int				_iPribehDelay = 0;
int				_iEscDelay = 0;
int				_iHraDelay = 0;
int 			_iPocetRaket = 0;
int				_iCasovac = 0;

bool			_bBonus = false;
bool			_bExistCar = false;

//-----------------------------------------------------------------
// World Variables
//-----------------------------------------------------------------
Bitmap*			_pozadi;  	
Bitmap*			_obrDelo1;
Bitmap*			_obrDelo2;
Bitmap*			_obrDelo3;
Bitmap*			_obrDelo4;
Bitmap*			_obrRaketa1;
Bitmap*			_obrRaketa2;
Bitmap*			_obrRaketa3;
Bitmap*			_obrRaketa4;
Bitmap*			_obrExploze;
Bitmap*			_obrAuto;
Bitmap*			_obrStrela;
Bitmap*			_obrExpStrela;
Bitmap*			_obrBigExploze;
Bitmap*			_obrBonusy;
Bitmap*			_obrSplash;
Bitmap*			_obrFinish;
Bitmap*			_obrJet;
Bitmap*			_obrVlajky;
Bitmap*			_obrJetFire;
Bitmap*			_obrJoe;
Bitmap*			_obrStoryFirst;
Bitmap*			_obrStorySecond;
Bitmap*			_obrMitkins;
Bitmap*			_obrLevel;
Bitmap*			_obrKamen;
Bitmap*			_obrAB;
Bitmap*			_tile1;

TileBackground*	_level1;

Sprite*			_delo1;
Sprite*			_raketa1;
Sprite*			_bonus;
Sprite*			_flagCZ;
Sprite*			_flagUS;
Sprite*			_joe;
Sprite*			_mitkins;
Sprite*			_kameny[5];
Sprite*			_A;
Sprite*			_B;

HFONT			_font;
HFONT			_storyFont;
HFONT			_gameFont;
HFONT			_oldfont;

string			_titulky;
int				_XTitulky = 650;
eStavHry		_stavHry = menu;
int				_stavPribehu = 0;
int				_mise = 0;

Delo*			_dela[_pocetDel];
Auto*			_auto;
TextMenu*		_horniMenu;
TextMenu*		_nastaveniMenu;
TextMenu*		_hlavniMenu;

//------------------------------------------------------------------------------
		Uint8 				g_win_flags;
		SDL_Surface*		g_screen;				// Surface okna
		Mix_Chunk* 			_mSounds;
		Mix_Music*			_musicGame;
		Mix_Music*			_musicMenu;
		
		int 			_iCountSounds;
		int 			g_volume ;				// Hlasitost
		int 			g_volume_left;			// Hlasitost leveho kanalu
		int 			g_sound_distance;		// Vzdalenost od zdroje zvuku
		bool 			g_reverse_stereo;		// Prohodi levy a pravy kanal
//------------------------------------------------------------------------------

//-----------------------
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [3];
//-----------------------

void CheckujDela();
void NewFreeGame();
void NewGame();
void DrawSplashScreen(HDC);
void DrawSettingsScreen(HDC);
void DrawStoryScreen(HDC);
void DrawGameScreen(HDC);
void RefreshCaptions();

int Approach(int what,int where);

string Translate(string t);
void CreateTable(char* p);

bool InitSDL();
