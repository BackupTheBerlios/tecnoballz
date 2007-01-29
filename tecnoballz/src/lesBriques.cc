//******************************************************************************
// copyright (c) 1991-2006 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "lesBriques.cc"
// created	: 1996-11-13
// updates	: 2006-10-03
// fonction	: manage the bricks
//-----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA 02111-1307, USA.
//******************************************************************************
#include "../include/lesBriques.h"
#include "../include/handler_resources.h"
#include "../include/handler_memory.h"
#include "../include/handler_display.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
lesBriques::lesBriques()
{
	littleInit();
	if(bob_ground)
		max_of_sprites = NB_BRICKSH * NB_BRICKSV;
	else
		max_of_sprites = 0;
	sprites_have_shades = true;
	sprite_type_id = BOB_BRICK1;
	brique_pnt = (brickClear *)NULL;
	brikTampon = (char *)NULL;
	mega_table = (brickInfos *)NULL;
	GFX_brique = (bitmap_data *)NULL;
	briqueSave = 0;
	brique_clr = 0;
	less_brick = 0;
	less_count = 10;
	pageBrickH = 112 * resolution;
	pageBrickV = 63 * resolution;
	brickWidth = BRICKWIDTH * resolution;
	brickHeigh = BRICKHEIGH * resolution;
	brick_size = brickWidth * brickHeigh;
	brkyoffset = BRKYOFFSET * resolution;
	brickIndus = 8 * brickHeigh * pageBrickH;

	ombre_deca = 3 * resolution;
	ombre_left = (BRICKHEIGH * resolution) - ombre_deca;
	ombre_yoff = (BRKYOFFSET - BRICKHEIGH) * resolution;
	ombre_top1 = ombre_deca - ombre_yoff;
	
}

//-------------------------------------------------------------------------------
// release the object
//-------------------------------------------------------------------------------
lesBriques::~lesBriques()
{
	if(brique_pnt)
		memory->release((char *)brique_pnt);
	if(mega_table)
		memory->release((char *)mega_table);
	if(brikTampon)
		memory->release((char *)brikTampon);
	if(GFX_brique)
		delete GFX_brique;
	release_sprites_list();
	//mentatKill();
}

//-------------------------------------------------------------------------------
// first initialization
//-------------------------------------------------------------------------------
Sint32 lesBriques::first_init(barreScore *barre, zeCapsules *capsu, ze_gadgets *gadge)
{
	barreObjet = barre;
	caps_objet = capsu;
	gads_objet = gadge;

	// allocate memory for the redraw bricks table
	if(!brique_pnt)
	{	brique_pnt = (brickClear *) memory->alloc
			(MAXBRIKCLR * sizeof(brickClear), 0x54425249);
		error_init(memory->retour_err());
		if(erreur_num) return erreur_num;
	}

	// allocate memory to save background under bricks
	if(!brikTampon)
	{	brikTampon = memory->alloc
			(brick_size * NB_BRICKST, 0x54425249 );
		error_init(memory->retour_err());
		if(erreur_num) return erreur_num;
	}

	// allocate memory for current brick level
	if(!mega_table)
	{	mega_table = (brickInfos *) memory->alloc
			(sizeof(brickInfos) * NB_BRICKST, 0x4D454741);
		error_init(memory->retour_err());
	}

	return erreur_num;
}

// -----------------------------------------------------------------------------
// initialize new level and draw bricks 
// input	=> areaN: number of the area (1 to 5)
// 		=> tablo: number of the level (1 to 12)
//		=> lbrik: 1 = bonus "less brick" (bought into the shop)
// -----------------------------------------------------------------------------
Sint32 lesBriques::initialise(Sint32 areaN, Sint32 tablo, Sint32 lbrik)
{

	if(bob_ground) 
	{
               alloc_sprites_list ();
	/*	sprites_list = (sprite_object **)
			(memory->alloc(sizeof(sprite_object *) * max_of_sprites, 0x4F424A47));
		error_init(memory->retour_err());
		if(erreur_num) return (erreur_num);
            */
	}
	
	if(!GFX_brique)
	{	
		less_brick = lbrik;

	//###################################################################
	// reset restauration list
	//###################################################################
	briqueSave = 0; 
	brique_clr = 0;
	brickClear *briPT = brique_pnt;
	for(Sint32 j = 0; j < MAXBRIKCLR; j++, briPT++)
	{	briPT->balle_posX = 0;
 		briPT->balle_posY = 0;
		briPT->raquettePT = (sprite_paddle *) 0x0;
		briPT->brique_num = 0;
		briPT->briqueFlag = 0;
		briPT->adresseAff = 0;
		briPT->adresseTab = (brickInfos *) 0x0;
	}

	//###################################################################
	// Initialize current brick level
	//###################################################################
	brickInfos *megaT = mega_table;
	Sint32 c = 0;
	Sint32 vacol = 239;
	for(Sint32 j = 0; j < NB_BRICKSV * brkyoffset; j += brkyoffset)
	{	for(Sint32 i = 0; i < NB_BRICKSH * brickWidth; i += brickWidth)
		{	megaT->brique_rel = 0;
			megaT->brique_aff = 0;
			megaT->adresseAff = display->buffer_rel(i, j);
			megaT->briquePosX = 0;
			megaT->briquePosY = 0;
			megaT->brique_num = c++;
			megaT->brickcolor = vacol;
			megaT++;
		}
		if(++vacol > 255)
			vacol = 239;
	}

	//###################################################################
	// select one of 10 sets of bricks
	//###################################################################
	GFXbriques = new bitmap_data();
	GFXbriques->load(handler_resources::RESZEBRICK);
	Sint32 i = hasard_val & 0x0F;
	//i = 7;
	if(i >= 10)
		i = i - 10;
	Sint32 bposy = 0;
	Sint32 bposx = 0;
	if(i >= 5)
	{	bposy = 63 * resolution;
		i = i - 5;
	}
	bposx = i * 112 * resolution;
	
	bposx = 0 * 112 * resolution; //test only
	bposy = 1 * 63 * resolution; //test only
	
	if(is_verbose)
		printf("lesBriques::initialise()  bposx=%i / bposy=%i \n", bposx, bposy);
	GFX_brique = GFXbriques->cut(bposx, bposy, pageBrickH, pageBrickV);
	if(!GFX_brique)
	{	error_init(GFXbriques->retour_err());
		return (erreur_num);
	}
	delete GFXbriques; 

	//###################################################################
	// read one bricks level from the file "tableau.data"
	//###################################################################
	error_init(tabNouveau(areaN, tablo)); 
	if(erreur_num)
		return (erreur_num);

	//###################################################################
	// read somes values for the graphic routine
	//###################################################################
	offsSource = GFX_brique->get_line_modulo(brickWidth);
	offsDestin = display->buffer_mod(brickWidth);
	adr_source = (Sint32 *)GFX_brique->get_pixel_data();
	adr_desti1 = (Sint32 *)display->buffer_adr();
	adr_desti2 = (Sint32 *)display->tampon_adr();

	initpalett();
	}
	sauve_fond();			// save background under bricks	
	bricksShad();			// display bricks shadows
	bricks_aff();			// display bricks
	return (erreur_num);
}

// -----------------------------------------------------------------------------
// return the number of bricks
// -----------------------------------------------------------------------------
Sint32 lesBriques::getbrikCnt()
{ 
	return brickCount;
}

// -----------------------------------------------------------------------------
// recopie un nouveau tableau dans "mega_table"
// input	=> areaN : Numero de l'area (1 a 5) / number of the area (1 to 5)
//			=> tablo : Numero du tableau (1 a 12) / number of the table (1 to 12)
// -----------------------------------------------------------------------------
Sint32 lesBriques::tabNouveau(Sint32 areaN, Sint32 tablo)
{
	//if(is_verbose)
		printf("lesBriques::tabNouveau()  areaN=%i / tablo=%i \n", areaN, tablo);
	// un tableau fait 17 lignes sur 10 colonnes = 170 * 2 = 340 octets  
	// a table makes 17 lines out of 10 columns 
	brickCount = 0;	//reset the number of bricks of the level 
	brickDestr = 0;	//reset the number of destroyed bricks 

	//###################################################################
	// load bricks levels (34000 bytes => 100 levels)
	//###################################################################
	char *tabHd = resources->getResData(handler_resources::RESBLEVELS);
	if(!tabHd)
	{	error_init(E_NORESOUR);
		return (erreur_num);
	}
	if(tablo >= 6) tablo--;
	char *tabPT = tabHd + (SIZEOFAREA * 2 * (areaN - 1)) +
		(LEVEL_SIZE * 2 * (tablo - 1));

	//###################################################################
	// select a level randomly (2 possible levels for a level)
	//###################################################################
	if((hasard_val & 0x001))
		tabPT = tabPT + LEVEL_SIZE * 2 * 50;

	//###################################################################
	// copy all bricks of the level in the structure "mega_table"
	//###################################################################
	brickInfos *megaT = mega_table;
	megaT += (6 * NB_BRICKSH);	// 6 first lines are always empty
	Uint32 bobindex = 6 * NB_BRICKSH;
	for(Sint32 j = 0; j < LEVELHEIGH; j++, megaT += 3, bobindex += 3)
	{	megaT += 3;	// the first 3 columns are always empty 
		bobindex += 3;
		for(Sint32 i = 0; i < LEVELWIDTH; i++, megaT++, bobindex++)
		{	Sint32 adres = 0;
			char pos_y = *(tabPT++);	// position y dans page brique de 0 a 8
			char pos_x = *(tabPT++);	// position x dans page brique de 0 a 12 (step 2) 12*8*2=192
			if(pos_x || pos_y)
			{	//pos_x = 12;	// test only
				//pos_y = 1;	// test only
				megaT->briquePosX = pos_x;	// save X-coordinate into mega_table
				megaT->briquePosY = pos_y;	// save Y-coordinate into mega_table
				adres = GFX_brique->get_offset(pos_x * 8 * resolution, pos_y * brickHeigh);
				if(adres < brickIndus)	// it's a indestructible brick?
					brickCount++;		// not, counter's incremented
			
				if(bob_ground)
				{
					sprite_object *ptbob = new sprite_object();
					ptbob->create_sprite(BOB_BRICK1, GFX_brique, 1, 0);
					sprites_list[bobindex] = ptbob;
					sprites->add(ptbob);
					ptbob->enable();
					ptbob->set_image(2);
				}
			
			
			}
			megaT->brique_rel = adres;
			megaT->brique_aff = adres;
			//printf("%i ", megaT->brique_rel);
		}
		//printf("\n");
	
	}
	if(tabHd)
		memory->release((char*)tabHd);

	//###################################################################
	// Initialize the number of total bricks to destroy 
	//###################################################################
	barreObjet->scoreBrick(brickCount);
	return (erreur_num);
}

// -----------------------------------------------------------------------------
// display bricks shadows
// -----------------------------------------------------------------------------
void lesBriques::bricksShad()
{
	brickInfos *megaT = mega_table;
	for(Sint32 j = ombre_deca; j < NB_BRICKSV * brkyoffset + ombre_deca; j += brkyoffset)
	{	for(Sint32 i = -ombre_deca; i < NB_BRICKSH * brickWidth - ombre_deca; i += brickWidth)
		{	if(megaT->brique_rel)
				display->rectShadow(i, j, brickWidth, brickHeigh);
			megaT++;
		}
	}
}

//------------------------------------------------------------------------------
// display all bricks
//------------------------------------------------------------------------------
void lesBriques::bricks_aff()
{
	brickInfos *megaT = mega_table;
	for(Sint32 j = 0; j < NB_BRICKSV * brkyoffset; j += brkyoffset)
	{	for(Sint32 i = 0; i < NB_BRICKSH * brickWidth; i += brickWidth, megaT++)
		 {	Sint32 pos_x = megaT->briquePosX;	// x >= 0 and x < 14
			Sint32 pos_y = megaT->briquePosY;	// y >= 0 and y < 9
			if(pos_x || pos_y)
			{	pos_x *= 8 * resolution;				// planar -> chunky
				pos_y *= brickHeigh;
				char *srcPT = GFX_brique->get_pixel_data(pos_x, pos_y);
				dsplybrick(srcPT, megaT->adresseAff, megaT->brickcolor);
			}
		}
	}
}

// -----------------------------------------------------------------------------
// display one brick
//	input	=> srcPT : pointer on the graphics of the brick
//			=> adres : relative pointer on the graphics of the "buffer"
//			=> colbr : colormap number (239 to 255)
// -----------------------------------------------------------------------------
void lesBriques::dsplybrick(char *srcPT, Sint32 adres, Sint32 colbr)
{
	Sint32 line1 = offsSource;
	Sint32 line2 = offsDestin;
	char *desP1 = (char *)adr_desti1;	// pointer to the "buffer"
	char *desP2 = (char *)adr_desti2;	// pointer to the "tampon"
	desP1 = desP1 + adres;
	desP2 = desP2 + adres;
	for (Sint32 j = 0 ; j < brickHeigh ; j++)
	{	for (Sint32 i = 0 ; i<brickWidth ; i++ )
		{	char p = *(srcPT++);
			if(p == 0)		// color 0 ?
				p = colbr;	// yes, replace
			*(desP1++) = p;
			*(desP2++) = p;
		}
		srcPT = srcPT + line1;
		desP1 = desP1 + line2;
		desP2 = desP2 + line2;
	}
}
// -----------------------------------------------------------------------------
// initialize the 17 colors of the bricks
// -----------------------------------------------------------------------------
void lesBriques::initpalett()
{
	SDL_Color *palPT = display->paletteAdr();
	SDL_Color *palP1 = palPT + 239;
	Sint32 i = hasard_val;
	i &= 0x0f;
	//Sint32 i = hasard_val & 0x0F;
	if(i >= 10)
		i = i - 10;
	const Uint32 *ptpal = (handler_resources::tabledegas + i * 18);
	for(i = 0; i < 17; i++)
	{	Uint32 vacol = ptpal[i];
		Uint32 vablu = vacol & 0x000000ff;
		Uint32 vagre = vacol & 0x0000ff00;
		vagre = vagre >> 8;
		Uint32 vared = vacol & 0x00ff0000;
		vared = vared >> 16;
		palP1->r = vared;
		palP1->g = vagre;
		palP1->b = vablu;
		palP1++;
	}
	display->enable_palette(palPT);
}
//------------------------------------------------------------------------------
// save background under bricks	
//------------------------------------------------------------------------------
void lesBriques::sauve_fond()
{
	Sint32 *bufPT = (Sint32 *)brikTampon;
	brickInfos *megaT = mega_table;
	Sint32 offs = display->bufferNext() - brickWidth;
	for(Sint32 j = 0; j < NB_BRICKSV * brkyoffset; j += brkyoffset)
	{	for(Sint32 i = 0; i < NB_BRICKSH * brickWidth; i += brickWidth, megaT++)
		{	Sint32 *monPT = (Sint32 *)display->tampon_pos(i, j);
			megaT->briqueFond = bufPT;
			for(Sint32 k = 0; k < brickHeigh; k++, monPT = (Sint32*)((char *)monPT + offs))
			{	for(Sint32 l = 0; l < brickWidth / 4; l++)
				*(bufPT++) = *(monPT++);
			}
		}
	}
}


// -----------------------------------------------------------------------------
// handle "less bricks"
// -----------------------------------------------------------------------------
void lesBriques::lessbricks()
{ 
	if(less_brick > 0)
	{	if(--less_count <= 0)
		{	less_count = 10;
			less_brick--;
			barreObjet->brickMoins(1);
#ifndef SOUNDISOFF
			audio->play_sound(S_TOUBRIK1);
#endif
		}
	}
}

//------------------------------------------------------------------------------
// display a brick or restore the background under the brick
//------------------------------------------------------------------------------
Sint32 lesBriques::brickRemap()
{
	brickClear *briPT = brique_pnt + brique_clr;
	Sint32 adres = briPT->adresseAff;	// adresse affichage relative
	if(!adres) return 0;
	{	brique_clr += 1;				// augmente le pointeur sur la table brique a effacer
		brique_clr &= (MAXBRIKCLR - 1);	// limite le compte a 512 (de 0 a 511)
		briPT->adresseAff = 0;
		brickInfos *megaT = briPT->adresseTab;

		//###############################################################
		// restaure background
		//###############################################################
		if(briPT->briqueFlag)	// 0 = redraw brick / 1 = restore background
		{	Sint32 line2 = offsDestin;
			Sint32 *desP1 = adr_desti1;
			Sint32 *desP2 = adr_desti2;
			desP1 = (Sint32*)((char *)desP1 + adres);
			desP2 = (Sint32*)((char *)desP2 + adres);
			Sint32 *srcPT = megaT->briqueFond;


			megaT->brique_aff = 0;
			for(Sint32 j = 0; j < brickHeigh; j++)
			{	for(Sint32 i = 0; i < brickWidth / 4; i++)
				{	*(desP1++) = *srcPT; 
					*(desP2++) = *(srcPT++);
				}
				desP1 = (Sint32*)((char *)desP1 + line2);
				desP2 = (Sint32*)((char *)desP2 + line2);
			}
			Sint32 j;

			//##########################################################
			// clear drop shadow (if needed)
			//##########################################################

			// 1. bottom
			if((megaT + offBri_BB)->brique_aff)
				j = ombre_yoff;	// brique en bas (2 lignes)
			else
				j = ombre_deca;	// pas de brique en bas (6 lignes)
			Sint32 decal = display->ecran_next(adres, 0, brickHeigh);
			display->clr_shadow(decal, brickWidth - ombre_deca, j);

			// 2. left-bottom
			if((megaT + offBri_BG)->brique_aff)
				j = ombre_yoff;
			else
				j = ombre_deca;
			decal = display->ecran_next(adres, -ombre_deca, brickHeigh);
			display->clr_shadow(decal, ombre_deca, j);

			// 3. left		
			if(!(megaT + offBri_GG)->brique_aff)
			{	decal = display->ecran_next(adres, -ombre_deca, ombre_deca);
				display->clr_shadow(decal, ombre_deca, ombre_left);
			}

			//##########################################################
			// redraw drop shadow (if needed)
			//##########################################################
			
			// 1. exist top brick ? 
			if((megaT + offBri_HH)->brique_aff)
				display->set_shadow(adres, brickWidth - ombre_deca, ombre_top1);

			// Il existe une brique en haut a droite ?
			if((megaT + offBri_HD)->brique_aff)
			{	decal = display->ecran_next(adres, brickWidth - ombre_deca, 0);
				display->set_shadow(decal, ombre_deca, ombre_top1);
			}
			
			// Il existe une brique a droite ?
			if((megaT + offBri_DD)->brique_aff)
			{	decal = display->ecran_next(adres, brickWidth - ombre_deca, ombre_deca);
				display->set_shadow(decal, ombre_deca, ombre_left);
			}

			// destroyed indestructible brick
			if(briPT->balle_posX < 0) 
			{	barreObjet->scoreAjout(100);
#ifndef SOUNDISOFF
				audio->play_sound(S_CASSEIND);
#endif
			}
			else
			{	barreObjet->scoreAjout(20);
				barreObjet->brickMoins(1);
				brickDestr++;
#ifndef SOUNDISOFF
				audio->play_sound(S_TOUBRIK1);
#endif
				if(briPT->balle_posX != 512)
				{	caps_objet->envoieFric(briPT);
					gads_objet->envoieGads(briPT);
				}
			}

		}

		//###############################################################
    	// redraw a new brick
		//###############################################################
    	else
		{	char *gfxad = GFX_brique->get_pixel_data();
			brickInfos *megaT = briPT->adresseTab;
			dsplybrick(gfxad + briPT->brique_num, adres, megaT->brickcolor);
			barreObjet->scoreAjout(10);
#ifndef SOUNDISOFF
		  	audio->play_sound(S_TOUBRIK1);
#endif
			caps_objet->envoieFric(briPT);
    	}
	} // (adresseAff!=0)
	return 1;
}

//------------------------------------------------------------------------------
// clear all bricks (Game Over)
//------------------------------------------------------------------------------
void lesBriques::clr_bricks()
{
	Sint32 save = briqueSave;
	brickClear *briPT = brique_pnt;
	brickInfos *tMega = mega_table;
	tMega += (6 * NB_BRICKSH);	// 6 first lines are always empty
	for(Sint32 j = 0; j < LEVELHEIGH; j++, tMega += 3)
	{	tMega += 3;	// the first 3 columns are always empty 
		for(Sint32 i = 0; i < LEVELWIDTH; i++, tMega++)
		{	brickInfos *megaT = tMega;
			brickClear *briP2 = briPT + save;
			Sint32 v = megaT->brique_rel;
			if(v)
			{	briP2->balle_posX = 512;	// flag brick blitz destroy
				briP2->adresseAff = megaT->adresseAff;
				briP2->adresseTab = megaT;
				megaT->briquePosX = -1;
				megaT->brique_rel = 0;					//RAZ brick code
				briP2->brique_num = megaT->brique_num;	//brick number
				briP2->briqueFlag = 1;					//flag restore background
				save += 1;							// inc. pt restaure table
				save &= (MAXBRIKCLR - 1);
			}
		}
	}
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Sint32	lesBriques::getBkWidth()
{
	return brickWidth;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Sint32	lesBriques::getBkHeigh()
{
	return brickHeigh;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Sint32	lesBriques::getBkIndus()
{
	return brickIndus;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Sint32	lesBriques::getYOffset()
{
	return brkyoffset;
}
