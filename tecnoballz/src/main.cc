/** 
 * @file main.cc 
 * @brief The main function is where the program starts execution 
 * @created 2002-08-21 
 * @date 2007-01-28
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.21 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: main.cc,v 1.21 2007/01/30 21:06:04 gurumeditation Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */
#include "../include/mentatCode.h"
#include "../include/handler_display.h"
#include "../include/handler_audio.h"
#include "../include/configfile.h"
configfile oConfigure;

/**
 *  The main function is where the program starts execution
 */
Sint32
main (Sint32 nbArg, char **ptArg)
{

#ifdef __LP64__
  printf ("LP64\n");
#endif

  oConfigure.loadconfig ();
  if (!oConfigure.scanZeArgs (nbArg, ptArg))
    return 0;
  if (mentatCode::is_verbose)
    {
      std::cout << "================================" << std::endl
        << "TecnoballZ starts! " << std::endl 
        << "================================" << std::endl;
    }
  Sint32 error = 0;
  try
    {
      error = mentatCode::first_init (&oConfigure);
      if (error)
        return error;
      error = mentatCode::game_begin ();
      if (error)
        return error;
    }
  catch (std::ios_base::failure& fail) 
    {
      std::cerr << "(!)std::ios_base::failure" << std::endl;
      std::cerr << fail.what() << std::endl;
    }
  catch (...)
    {
      std::cerr << "fatal error" << std::endl;
      mentatCode::desinstall (&oConfigure);
      throw;
    }
  if (mentatCode::is_verbose)
    {
      std::cout << "================================" << std::endl;
    }
  error = mentatCode::desinstall (&oConfigure);
  if (error)
    return error;
  if (mentatCode::is_verbose)
    {
      std::cout << "TecnoballZ is finished! ========" << std::endl;
    }
  oConfigure.saveconfig ();
  return error;
}


/*
 
 a faire
 - utiliser les tilemaps de 16x16 originales dans le definlement menu/gardiens pour eviter
   d'utiliser les tilemaps 32x32 (qui ont ete doubles avec GIMP) refaire donc un mapeditor
   special pour le 640x480
 - gigablitz doit tuer les bouisbouis
 - si une raquette enclenche un multiballes pendant la musique lost, couper cette musique
 - si le joueur perd une balle et qu'il a le bouton droit presse ne pas ejecter la balle
   de suite attendre qu'il relache le bouton
 - si game over supprimer les gems
 - faire les textes du jeu (intro, magasin, jeu) en francais et anglais (fichiers externes)
 - accelerer l'affichage des mini messages si beaucoup de bonus
 
 - ? faire des fonds defilents des tableaux briques 
 - ? mettre les donnees (tableaux, sprites?, ...) au format XML avec tiny xml 
 - ? utiliser kyro comme moteur de sprites 
 - ? mode balle Arkanoid ?

main.cc
mentatCode.cc    => tecnoballz 
list_sprites.cc  => 
runGestion.cc    => supervisor.cc
configfile.cc    => 
lispreader.cc    =>
GFX_bitmap.cc 
GIF_bitMap.cc    => bitmap_data
 
sprite_object  => sprite_object

giga_blitz.cc   => sprite_gigablitz
mousePoint.cc   => sprite_mouse_pointer
technoBall.cc   => sprite_ball
techno_eye.cc   => sprite_eye
techno_gem.cc   => sprite_gem
tecno_boui.cc   => sprite_ship
tecno_bump.cc   => sprite_paddle
tecno_caps.cc   => sprite_money
tecnoDefil.cc   => sprite_fonte_menu
tecno_fire.cc   => sprite_projectile
tecno_gads.cc   => sprite_capsules
tecno_gard.cc   => sprite_guardian
tecno_miss.cc   => sprite_bullet
tecno_text.cc   => sprite_fonte_game
 
ballDirect.cc   => controller_balls
controller_bricks.cc   => controller_bricks
printmoney.cc   => controller_indicators
zeBouiBoui.cc   => controller_ships
zeCapsules.cc   => controller_moneys
zeCongBall.cc   => controller_spheres
zeDefilTxt.cc   => controller_scroll_fontes
zeFireBump.cc   => controller_fires
ze_gadgets.cc   => controller_capsules
zeGameOver.cc   => controller_game_over
zeGemstone.cc   => controller_gems
zeGigaBlit.cc   => controller_gigablitz
zeguardian.cc   => controller_guardians
ze_magneye.cc   => controller_magnetic_eyes
zeMissiles.cc   => controller_bullets
zeMoveText.cc   => controller_level_fontes
zeNewBalls.cc   => controller_balls
zeRaquette.cc   => controller_paddles
zexplosion.cc   => controller_explosions

tableaux_Z.cc   => supervisor_bricks_level
gard_tecno.cc   => supervisor_guards_level
shop_tecno.cc   => supervisor_shop
scrolledit.cc   => supervisor_scrolleditor 
menu_tecno.cc   => supervisor_main_menu

audiomixer.cc   => handler_audio.cc 
clavierMac.cc   => handler_keyboard.cc
ecran_hard.cc   => handler_display.cc 
RAM_killer.cc   => handler_memory.cc
ressources.cc   => handler_resources.cc 
scoretable.cc   => handler_score_table
joueurData.cc   => handler_players
level_data.cc   => handler_levels 
print_text.cc   => handler_print_text 

zeMiniMess.cc   => item_small_messages 
barreScore.cc   => item_score_panel
briqueCote.cc   => items_sides_bricks
ejectBalls.cc   => items_balls_ejectors
escapeMenu.cc   => item_popup_menu 
fond_ecran.cc   => item_background
head_anima.cc   => item_head_anim
lastScroll.cc   => item_vertical_scroll
print_menu.cc   => item_menu_text
score_over.cc   => item_score_table


*/
