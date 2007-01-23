//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeMissiles.cc"
// created		: ?
// updates		: 2004-10-23
// fonction	: handle weapons of the guards (not the gigablitz!)
//-----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA  02111-1307, USA.
//
//******************************************************************************
#include "../include/zeMissiles.h"
#include "../include/ressources.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
zeMissiles::zeMissiles(tecno_bump *pBump, zexplosion *pexpl)
{
	littleInit();
	max_of_sprites = 48;
	objects_have_shades = false;
	sprite_type_id = BOB_MISSIL;
	ptbumper01 = pBump;
	pexplosion = pexpl;
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
zeMissiles::~zeMissiles()
{
	release_sprites_list();
}

//-----------------------------------------------------------------------------
// trajectory of all fires
//-----------------------------------------------------------------------------
void zeMissiles::execution1()
{
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	tecno_miss *xFire = sprites_list[i];
		xFire->execution1();
	}
}

//-----------------------------------------------------------------------------
// animation, change GFX 
//-----------------------------------------------------------------------------
void zeMissiles::anim_fires()
{
	tecno_miss *xFire = sprites_list[0];
	xFire->animRepete();
	Sint32 o = xFire->litAnimOff();
	Sint32 cycle = o & 0X1;
	if(!cycle) cycle = sprite_object::METHOD_TAB;
	else cycle = sprite_object::METHOD_CC1;
	
	Sint32 t = max_of_sprites;
	for(Sint32 i = 1; i < t; i++)
	{	xFire = sprites_list[i];
		xFire->change_GFX(o);
		//xFire->affCycling = cycle;
		xFire->put_method = cycle;
	}
}

//-----------------------------------------------------------------------------
// weapon's collision with the bumper 
//-----------------------------------------------------------------------------
void zeMissiles::bumper_col()
{ 
	Sint32 bumpy = ptbumper01->get_y_coord();
	Sint32 bumpx = ptbumper01->get_x_coord();
	Sint32 bmpx2 = bumpx + ptbumper01->rakLargeur();
	Sint32 bmpy2 = bumpy + ptbumper01->get_sprite_height();
	for(Sint32 i = 0; i < max_of_sprites; i++)
	{	tecno_miss *xFire = sprites_list[i];
		if(xFire->is_enabled)
		{	Sint32 weapy = xFire->y_coord;
			if (weapy < bmpy2)
			{	Sint32 weapx = xFire->x_coord;      
				if(weapx < bmpx2)
				{	weapy += xFire->sprite_height;
					if(weapy > bumpy)
					{	weapx += xFire->sprite_width;
						if(weapx > bumpx)
						{	xFire->is_enabled = 0;
							if (ptbumper01->getInvncbl() <= 0)
							{	ptbumper01->setInvncbl(100);
								joueurGere->lifesMoins(1);
#ifndef SOUNDISOFF
								audio->play_sound(S_RAKEXPLO);
								audio->play_sound(S_ENLEVVIE);
#endif
								pexplosion->add_explos
									(bumpx + ptbumper01->rakLargeur()/2,
									bumpy + ptbumper01->get_sprite_height()/2);
							}
						}
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// return the first object "tecno_miss" (first simple weapon)
//-----------------------------------------------------------------------------
tecno_miss*	zeMissiles::getWeapOne()
{
	return sprites_list[max_of_sprites - 1];
}

//-----------------------------------------------------------------------------
// new fire required !
//-----------------------------------------------------------------------------
void zeMissiles::newMissile(Sint32 nfire, tecno_gard *pgard)
{ Sint32 i = nfire;
  //printf("zeMissiles::newMissile(() : numero du tir = %ld\n", i);
  switch(i)
  { case 0:
      tir01_init(pgard);
      break;
    case 4:
      tir02_init(pgard);
      break;
    case 8:
      tir03_init(pgard);
      break;
    case 12:
      tir04_init(pgard);
      break;
    case 16:
      tir05_init(pgard);
      break;
    case 20:
      tir06_init(pgard);
      break;
    case 24:
      tir07_init(pgard);
      break;
    case 28:
      tir08_init(pgard);
      break;
    case 32:
      tir09_init(pgard);
      break;
    case 36:
      tir10_init(pgard);
      break;
    case 40:
      tir11_init(pgard);
      break;
  }
}

// *--------------------------- INIT TIR : tir >01< ---------------------------*
void zeMissiles::tir01_init(tecno_gard *pgard)
{
  Sint32 t = max_of_sprites - 1;
  Sint32 gardx = pgard->x_coord + pgard->gard_xcent - (16 * resolution);
  Sint32 gardy = pgard->y_coord;
  Sint32 n = 12;                                                 //12 objets pour ce tir
  Sint32 s = 0;                                                  //pointeur sur la table sinus
  do
  { tecno_miss *xFire = sprites_list[t];
    if(!xFire->is_enabled)
    { xFire->is_enabled = 1;
      xFire->x_coord = tir01_posi[s] * resolution + gardx;
      xFire->tablesinus = tir01_posi;
      xFire->flagDepla1 = s;                                   //pointeur table sinus x
      xFire->flagDepla2 = gardx;                               //sauve position x
      xFire->y_coord = gardy;                               // position y du tir
      gardy = gardy + 5 * resolution;
      //printf("zeMissiles::tir01_init() n=%ld, t=%ld, x=%ld, y=%ld \n", n, t, xFire->x_coord, xFire->y_coord);
      s = s + 5;
      if(n-- == 7)
        gardy = pgard->y_coord;
    }
  } while ( n > 0 && --t >= 0 );
}
// *--------------* tir 02, scie circulaire + support, 10 bobs *---------------*
void zeMissiles::tir02_init(tecno_gard *pgard)
{
  Sint32 t = max_of_sprites - 1;
  Sint32 gardx = pgard->x_coord + pgard->gard_xcent;
  Sint32 gardy = pgard->y_coord + pgard->gard_ycent;
  Sint32 s = 0;
  Sint32 n = 10;                                                 //10 objets pour ce tir
  do
  { tecno_miss *xFire = sprites_list[t];
    if(!xFire->is_enabled)
    { if(n > 3)
      { xFire->is_enabled = 2;
        xFire->flagDepla3 = s;
        s = s + 2;
        xFire->flagDepla1 = gardx;
        xFire->flagDepla2 = gardy;
        xFire->tablesinus = fire_sinus;
        //printf("zeMissiles::tir01_init() %ld, %ld %ld %ld \n", n, xFire->flagDepla1, xFire->flagDepla2, xFire->flagDepla3);
        if(--n == 3)
            gardx = gardx - 10 * resolution;
      }
      else
      { xFire->is_enabled = 3;
        xFire->x_coord = gardx;
        xFire->y_coord = gardy;
        gardx = gardx + 10 * resolution;
        n--;
      }
    }
  } while ( n > 0 && --t >= 0 );
    
}

// *-----------------------*  TIR 03, xevious, 4 bobs  *-----------------------*
void zeMissiles::tir03_init(tecno_gard *pgard)
{
  Sint32 t = max_of_sprites - 1;
  Sint32 gardx = pgard->x_coord + pgard->gard_xcent;
  Sint32 gardy = pgard->y_coord + pgard->gard_ycent;
  Sint32 n = 5;                                                 //5 objets pour ce tir
  do
  { tecno_miss *xFire = sprites_list[t];
    if(!xFire->is_enabled)
    { xFire->ptbumper01 = ptbumper01;
      if(n == 1)
      { xFire->is_enabled = 7;
        xFire->x_coord = gardx;
        xFire->y_coord = gardy;
      }
      else
      { xFire->is_enabled = 6;
        xFire->flagDepla1 = gardx;
        xFire->flagDepla2 = gardy;
        switch (n)
        { case 5:
            xFire->tablesinus = fire_sinus;
            xFire->flagDepla3 = 0;
            break;
          case 4:
            xFire->tablesinus = fire_sinus;
            xFire->flagDepla3 = 28;
            break;
          case 3:
            xFire->tablesinus = tir02_posi;
            xFire->flagDepla3 = 0;
            break;
          case 2:
            xFire->tablesinus = tir02_posi;
            xFire->flagDepla3 = 28;
            break;
        }
      }
      //printf("zeMissiles::tir02_init() %ld, %ld %ld %ld %ld \n", n, xFire->is_enabled, xFire->flagDepla1, xFire->flagDepla2, xFire->flagDepla3);
      n--;
    }
  } while ( n > 0 && --t >= 0 );
}

// *-----------------*TIR 04, scie circulaire II, 7 bobs*----------------------*
void zeMissiles::tir04_init(tecno_gard *pgard)
{
  Sint32 t = max_of_sprites - 1;
  Sint32 gardx = pgard->x_coord + pgard->gard_xcent;
  Sint32 gardy = pgard->y_coord + pgard->gard_ycent;
  Sint32 s = 0;
  Sint32 n = 7;                                                  //7 objets pour ce tir
  const Sint16* ptir = tir04_posi;
  do
  { tecno_miss *xFire = sprites_list[t];
    if(!xFire->is_enabled)
    { xFire->is_enabled = 4;
      xFire->flagDepla1 = gardx;
      xFire->flagDepla2 = gardy;
      xFire->flagDepla3 = s;
      xFire->flagDepla4 = 1;
      xFire->flagDepla5 = 10;
      xFire->flagDepla6 = *(ptir++);
      xFire->flagDepla7 = *(ptir++);
      xFire->tablesinus = fire_sinus;
      s = s + 8;
      n--;
    }
  } while ( n > 0 && --t >= 0 );
}

// **--------------------* TIR 05, automatique 9 coups *---------------------**
void zeMissiles::tir05_init(tecno_gard *pgard)
{
  Sint32 t = max_of_sprites - 1;
  Sint32 gardx = pgard->x_coord + pgard->gard_xcent;
  Sint32 gardy = pgard->y_coord + pgard->gard_ycent;
  Sint32 tempo = 10;
  Sint32 incrx = -5 * resolution;
  Sint32 n = 9;                                                  //9 objets pour ce tir
  do
  { tecno_miss *xFire = sprites_list[t];
    if(!xFire->is_enabled)
    { xFire->is_enabled = 5;
      xFire->x_coord = gardx;
      xFire->y_coord = gardy;
      xFire->flagDepla1 = tempo;
      tempo += 15;
      xFire->flagDepla2 = incrx;
      incrx += resolution;
      xFire->flagDepla3 = 5 * resolution;
      n--;
    }
  } while ( n > 0 && --t >= 0 );
    
}

// **---------------------* TIR 06, scie circulaire III *---------------------**
void zeMissiles::tir06_init(tecno_gard *pgard)
{
  Sint32 t = max_of_sprites - 1;
  Sint32 s = 0;
  Sint32 n = 30;                                                  //30 objets pour ce tir
  do
  { tecno_miss *xFire = sprites_list[t];
    if(!xFire->is_enabled)
    { xFire->is_enabled = 8;
      xFire->ptguardian = pgard;
      xFire->flagDepla3 = s;
      xFire->flagDepla4 = 2 * resolution;
      xFire->tablesinus = ressources::cosinus360;
      xFire->tablecosin = ressources::zesinus360;
      s = s + 12;
      n--;
    }
  } while ( n > 0 && --t >= 0 );
    
}

// **---------------------* TIR 07, scie circulaire IV *---------------------**
void zeMissiles::tir07_init(tecno_gard *pgard)
{
  Sint32 t = max_of_sprites - 1;
  Sint32 gardx = pgard->x_coord + pgard->gard_xcent;
  Sint32 gardy = pgard->y_coord + pgard->gard_ycent;
  Sint32 s = 0;
  Sint32 n = 10;                                                  //10 objets pour ce tir
  do
  { tecno_miss *xFire = sprites_list[t];
    if(!xFire->is_enabled)
    { xFire->is_enabled = 9;
      xFire->flagDepla1 = gardx;
      xFire->flagDepla2 = gardy;
      xFire->flagDepla3 = s;
      xFire->flagDepla4 = 1;
      xFire->tablesinus = ressources::cosinus360;
      xFire->tablecosin = ressources::zesinus360;
      s = s + 36;
      n--;
    }
  } while ( n > 0 && --t >= 0 );
    
}

// **----------------------* TIR 08, scie circulaire V *----------------------**
void zeMissiles::tir08_init(tecno_gard *pgard)
{
  Sint32 t = max_of_sprites - 1;
  Sint32 gardx = pgard->x_coord + pgard->gard_xcent;
  Sint32 gardy = pgard->y_coord + pgard->gard_ycent;
  Sint32 s = 0;
  Sint32 n = 10;                                                  //10 objets pour ce tir
  Sint32 tempo = 20;
  do
  { tecno_miss *xFire = sprites_list[t];
    if(!xFire->is_enabled)
    { xFire->is_enabled = 10;
      xFire->flagDepla1 = gardx;
      xFire->flagDepla2 = gardy;
      xFire->flagDepla3 = s;
      s += 72;
      xFire->flagDepla4 = 0;
      xFire->flagDepla5 = tempo;
      xFire->tablesinus = ressources::cosinus360;
      xFire->tablecosin = ressources::zesinus360;
      n--;
      if(n == 5)
      {  gardy = gardy + 25 * resolution;
         tempo = 1;
      }
    }
  } while ( n > 0 && --t >= 0 );
}

// **--------------------------* TIR 09, triangle *----------------------------*
void zeMissiles::tir09_init(tecno_gard *pgard)
{
  Sint32 t = max_of_sprites - 1;
  Sint32 gardx = pgard->x_coord + pgard->gard_xcent;
  Sint32 gardy = pgard->y_coord + pgard->gard_ycent;
  Sint32 s = 0;
  Sint32 n = 9;                                                  //10 objets pour ce tir
  do
  { tecno_miss *xFire = sprites_list[t];
    if(!xFire->is_enabled)
    { xFire->is_enabled = 11;
      xFire->flagDepla1 = gardx;
      xFire->flagDepla2 = gardy;
      xFire->tablesinus = ressources::cosinus360;
      xFire->tablecosin = ressources::zesinus360;
      xFire->flagDepla3 = s;
      s += 120;
      n--;
      if(n == 6)
      { s = 0;
        gardx += (16 * resolution);
      }
      else
      { if(n == 3)
        { s = 0;
          gardx -= (8 * resolution);
        }
      }
    }
  } while ( n > 0 && --t >= 0 );
}


// **---------------------** tir 10, epee de DAMOCLES **----------------------**
void zeMissiles::tir10_init(tecno_gard *pgard)
{
  Sint32 t = max_of_sprites - 1;
  Sint32 gardx = pgard->x_coord + pgard->gard_xcent - (17 * resolution) ;
  Sint32 gardy = pgard->y_coord + pgard->gard_ycent - (10 * resolution);
  const Sint16 * ptir = tir10_posi;
  Sint32 n = 6;	//6 objets pour ce tir
  do
  { tecno_miss *xFire = sprites_list[t];
    if(!xFire->is_enabled)
    { xFire->is_enabled = 3;
      xFire->x_coord = gardx + (*(ptir++) * xFire->sprite_width);
      xFire->y_coord = gardy + (*(ptir++) * xFire->sprite_height);
      n--;
    }
  } while ( n > 0 && --t >= 0 );
}

// *=-----------------------=* TIR 11, triangle II *=-------------------------=*
void zeMissiles::tir11_init(tecno_gard *pgard)
{
	Sint32 t = max_of_sprites - 1;
	Sint32 gardx = pgard->x_coord + pgard->gard_xcent;
	Sint32 gardy = pgard->y_coord + pgard->gard_ycent;
	Sint32 n = 15;	//15 objets pour ce tir
	Sint32 inc_x = 0;
	Sint32 inc_y = 5 * resolution;
	Sint32 s = 0;
	do
	{	tecno_miss *xFire = sprites_list[t];
		if(!xFire->is_enabled)
		{	xFire->is_enabled = 12;
			xFire->flagDepla1 = gardx;
			xFire->flagDepla2 = gardy;
			xFire->flagDepla3 = s;
			xFire->flagDepla4 = inc_x;
			xFire->flagDepla5 = inc_y;
			xFire->tablesinus = ressources::cosinus360;
			xFire->tablecosin = ressources::zesinus360;
			s += 72;
			n--;
			if(n == 10)
			{	s = 0;
				inc_x = -1 * resolution;
				inc_y = 4 * resolution;
			}
			else
			{	if(n == 5)
				{	s = 0;
					inc_x = 1 * resolution;
					inc_y = 4 * resolution;
				}
			}
		}
	} while ( n > 0 && --t >= 0 );
}


// tir gardien : table sinus ---------------------------------------------------
const Sint16 zeMissiles::tir01_posi[62] =
{	32,32,32,31,31,30,29,28,27,25,
	24,22,21,19,17,16,14,12,11, 9,
	8, 6, 5, 4, 3, 2, 1, 1, 0, 0,
	0, 0, 1, 1, 2, 3, 4, 5, 6, 8,
	9,11,12,14,16,17,19,21,22,24,
	25,27,28,29,30,31,31,32,32,32,
	99,99
};

// tir gardien : table de points d'un cercle de 8 pixels de rayon --------------
const Sint16 zeMissiles::tir02_posi[60] =
{   8,  0,  8, -1,  6, -3,  4, -4,  3, -6,  1, -8,  0, -8,
    0, -8, -1, -8, -3, -6, -4, -4, -6, -3, -8, -1, -8,  0,
   -8,  0, -8,  1, -6,  3, -4,  4, -3,  6, -1,  8,  0,  8,
    0,  8,  1,  8,  3,  6,  4,  4,  6,  3,  8,  1,  8,  0,
   99, 99, 99, 99
};

// tir gardien : table des directions de la roue accelerante -------------------
const Sint16 zeMissiles::tir04_posi[14] =
{	-1, 5, 3, 5, 1, 6, -2, 6, 0, 6, -1, 4, 2, 6
};

//  tir gardien : position des points, epee d'DAMOCLES -------------------------
const Sint16 zeMissiles::tir10_posi[12] =
{  1,0, 0,1, 1,1, 2,1, 1,2, 1,3
};

// anciennement appelle sinus -------------------------------------------------
const Sint16 zeMissiles::fire_sinus[60] =
{	15, -3, 13, -7, 11,-10,  9,-12,  7,-13,  3,-15,  0,-15,
	-3,-15, -6,-14, -9,-12,-11,-10,-13, -7,-14, -4,-15, -1,
	-15,   3,-13,  7,-11, 10, -9, 12, -7, 13, -3, 15,  0, 15,
	3,  15,  6, 14,  9, 12, 11, 10, 13,  7, 14,  4, 15,  1,
	99,  99, 99, 99
};
