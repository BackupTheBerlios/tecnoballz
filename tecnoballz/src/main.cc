/** 
 * @file main.cc 
 * @brief The main function is where the program starts execution 
 * @created 2002-08-21 
 * @date 2007-02-10
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.38 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: main.cc,v 1.38 2007/02/10 17:06:04 gurumeditation Exp $
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
#include "../include/tecnoballz.h"
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
  oConfigure.loadconfig ();
  if (!oConfigure.scanZeArgs (nbArg, ptArg))
    return 0;
  if (tecnoballz::is_verbose)
    {
      std::cout << "================================" << std::endl
        << "TecnoballZ starts! " << std::endl 
        << "================================" << std::endl;
    }
  Sint32 error = 0;
  try
    {
      error = tecnoballz::first_init (&oConfigure);
      if (error)
        return error;
      error = tecnoballz::game_begin ();
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
      tecnoballz::release_all_objects (&oConfigure);
      throw;
    }
  if (tecnoballz::is_verbose)
    {
      std::cout << "================================" << std::endl;
    }
  tecnoballz::release_all_objects (&oConfigure);
  if (tecnoballz::is_verbose)
    {
      std::cout << "TecnoballZ is finished! ========" << std::endl;
    }
  oConfigure.saveconfig ();
  return error;
}


/*
 
 a faire
 ** bug tir raquette, un tir de la raquette du bas n'est pas anime comme les autres
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

main
*mentatCode => tecnoballz 
*liste_BOBs => list_sprites 
*supervisor => supervisor
configfile  => 
lispreader  =>
*GFX_bitmap 
*GIF_bitMap => bitmap_data
 
*BOB_killer  => sprite_object

*giga_blitz  => sprite_gigablitz
*mousePoint  => sprite_mouse_pointer
*technoBall  => sprite_ball
*techno_eye  => sprite_eye
*techno_gem  => sprite_gem
*tecno_boui  => sprite_ship
*tecno_bump  => sprite_paddle
*tecno_caps  => sprite_money
*tecnoDefil  => sprite_fonte_menu
*tecno_fire  => sprite_projectile
*tecno_gads  => sprite_capsule
*tecno_gard  => sprite_guardian
*tecno_miss  => sprite_bullet
*tecno_text  => sprite_fonte_game
 
*ballDirect  => controller_viewfinders
*lesBriques  => controller_bricks
printmoney   => controller_indicators
*zeBouiBoui  => controller_ships
*zeCapsules  => controller_moneys
*zeCongBall  => controller_spheres
zeDefilTxt   => controller_scroll_fontes
*zeFireBump  => controller_fires
*ze_gadgets  => controller_capsules
*zeGameOver  => controller_game_over
*zeGemstone  => controller_gems
*zeGigaBlit  => controller_gigablitz
*zeguardian  => controller_guardians
ze_magneye   => controller_magnetic_eyes
*zeMissiles  => controller_bullets
zeMoveText   => controller_level_fontes
*zeNewBalls  => controller_balls
*zeRaquette  => controller_paddles
*zexplosion  => controller_explosions

*tableaux_Z  => supervisor_bricks_level
*gard_tecno  => supervisor_guards_level
*shop_tecno  => supervisor_shop
*menu_tecno  => supervisor_supervisor_map_editoror 
*scrolledit  => supervisor_main_menu

*escapeMenu   => item_popup_menu 
*audiomixer   => handler_audio 
*clavierMac   => handler_keyboard
*ecran_hard   => handler_display 
              => offscreen_surface
*RAM_killer   => handler_memory
*ressources   => handler_resources 
scoretable   => handler_score_table
*joueurData  => handler_players
level_data   => handler_levels 
print_text   => handler_print_text 

zeMiniMess   => item_small_messages 
barreScore   => score_panel_right
briqueCote   => items_sides_bricks
ejectBalls   => items_balls_ejectors
*fond_ecran   => tiles_background
head_anima   => item_head_anim
lastScroll   => tilesmap_scrolling 
print_menu   => item_menu_text
score_over   => item_score_table


*/
