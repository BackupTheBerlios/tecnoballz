//*****************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "main.cc"
// created      : 2002-08-21
// updates      : 2005-01-23
// id		: $Id: main.cc,v 1.8 2007/01/16 21:27:13 gurumeditation Exp $
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
// Place - Suite 330, Boston, MA  02111-1307, USA.
//-----------------------------------------------------------------------------
#include "../include/mentatCode.h"
#include "../include/ecran_hard.h"
#include "../include/audiomixer.h"
#include "../include/configfile.h"
//..............................................................................
configfile oConfigure;

//------------------------------------------------------------------------------
// main function
//------------------------------------------------------------------------------
int main(Sint32 nbArg, char **ptArg)
{ 

#ifdef __LP64__
	printf("LP64\n");
#endif

	oConfigure.loadconfig();
	if(!oConfigure.scanZeArgs(nbArg, ptArg))
		return 0;
	if(mentatCode::is_verbose) 
	{	printf("===========================================================\n");
		printf("                       TECNOBALLZ START\n");
		printf("===========================================================\n");
	}
	Sint32 error = 0;
	error = mentatCode::first_init(&oConfigure);
	if(error) return error;
	error = mentatCode::game_begin();
	if(error) return error;
	if(mentatCode::is_verbose) 
		printf("===========================================================\n");
	error = mentatCode::desinstall(&oConfigure);
	if(error) return error;
	if(mentatCode::is_verbose) 
		printf("END =======================================================\n");
	oConfigure.saveconfig();
	return error;
}


/*
 
sprite_object     => sprite_object

technoBall     => sprite_ball
techno_eye.cc  => sprite_eye
techno_gem.cc  => sprite_gem
tecno_boui.cc  => sprite_ship
tecno_bump.cc  => sprite_pad
tecno_caps.cc  => sprite_money
tecnoDefil.cc  => sprite_fonte_menu
tecno_fire.cc  => sprite_fire
tecno_gads.cc  => sprite_capsules
tecno_gard.cc  => sprite_guardian
tecno_miss.cc  => sprite_bullet
tecno_text.cc  => sprite_fonte_game
 

audiomixer.cc  => audio_handler
clavierMac.cc  => keyboard_handler
ecran_hard.cc  => display_handler
handler_memory.cc  => memory_handler

ballDirect.cc
barreScore.cc
briqueCote.cc
configfile.cc
ejectBalls.cc
escapeMenu.cc
fond_ecran.cc
gard_tecno.cc
GFX_bitmap.cc
GIF_bitMap.cc
giga_blitz.cc
head_anima.cc
joueurData.cc
lastScroll.cc
lesBriques.cc
level_data.cc
lispreader.cc
list_sprites.cc
main.cc
mentatCode.cc
menu_tecno.cc
mousePoint.cc
print_menu.cc
printmoney.cc
print_text.cc
ressources.cc
runGestion.cc
score_over.cc
scoretable.cc
scrolledit.cc
shop_tecno.cc
tableaux_Z.cc
eBouiBoui.cc
zeCapsules.cc
zeCongBall.cc
zeDefilTxt.cc
zeFireBump.cc
ze_gadgets.cc
zeGameOver.cc
zeGemstone.cc
zeGigaBlit.cc
zeguardian.cc
ze_magneye.cc
zeMiniMess.cc
zeMissiles.cc
zeMoveText.cc
zeNewBalls.cc
zeRaquette.cc



*/
 
  
 
 
 
 


