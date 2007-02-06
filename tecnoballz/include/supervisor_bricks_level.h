/** 
 * @file supervisor_bricks_level.cc 
 * @brief Bricks levels supervisor 
 * @date 2007-02-04
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.6 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_bricks_level.h,v 1.6 2007/02/06 20:41:33 gurumeditation Exp $
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
#ifndef __SUPERVISOR_BRICKS_LEVEL__
#define __SUPERVISOR_BRICKS_LEVEL__

class supervisor_bricks_level;

#include "../include/handler_display.h"
#include "../include/handler_audio.h"
#include "../include/sprite_object.h"
#include "../include/bitmap_data.h"
#include "../include/list_sprites.h"
#include "../include/handler_memory.h"
#include "../include/objects_list.h"
#include "../include/supervisor.h"
#include "../include/zeBouiBoui.h"
#include "../include/sprite_ship.h"
#include "../include/handler_keyboard.h"
#include "../include/tiles_background.h"
#include "../include/controller_paddles.h"
#include "../include/briqueCote.h"
#include "../include/barreScore.h"
#include "../include/handler_display.h"
#include "../include/ejectBalls.h"
#include "../include/controller_bricks.h"
#include "../include/controller_balls.h"
#include "../include/zeCapsules.h"
#include "../include/controller_capsules.h"
#include "../include/controller_gigablitz.h"
#include "../include/handler_players.h"
#include "../include/zeMoveText.h"
#include "../include/head_anima.h"
#include "../include/printmoney.h"
#include "../include/zeGameOver.h"
#include "../include/zeMiniMess.h"
#include "../include/level_data.h"
#include "../include/zeGemstone.h"
#include "../include/ballDirect.h"
#include "../include/handler_popup_menu.h"
#include "../include/ze_magneye.h"

class supervisor_bricks_level:public virtual supervisor
{
  friend class tiles_background;
  friend class zeCapsules;
  friend class controller_balls;
  friend class controller_paddles;
  friend class controller_capsules;
  friend class zeMoveText;
  friend class zeBouiBoui;
  friend class sprite_paddle;

private:
  zeMiniMess * ptMiniMess;
  printmoney *ptPrntmney;
  briqueCote *gereBricot;
  head_anima *tete_gugus;
  controller_bricks *bricks;
  tiles_background *tiles_ground;
  ejectBalls *gereEjects;
  controller_balls *gereBalles;
  ballDirect *ptBaDirect;
  controller_paddles *paddles;
  barreScore *tecZ_barre;
  zeBouiBoui *les_atomes;
  ze_magneye *pt_magneye;
  zeCapsules *gereCapsul;
  controller_capsules *gereGadget;
  zeGemstone *ptGemstone;
  zeMoveText *gere_texte;
  controller_gigablitz *ptGigaBlit;
  zeGameOver *ptGameOver;
  sprite_object *BottomWall;
  sprite_object *ptBobMoney;
  handler_popup_menu *popup_menu;
  sprite_capsule *ptBobRever;
  /** Area number from 1 to 2 */
  Sint32 areaNumber;
  /** Level number from 1 to 12 */
  Sint32 levelTecno;
  const amigaLevel *levelParam;
  Sint32 next_level;
  /** Time delay before next level */
  Sint32 count_next;
  Sint32 isgameover;

  Sint32 devel_keyx;            //next backgound
  Sint32 devel_keyw;            //previous backgound

  Sint32 indexbgrnd;
public:
    supervisor_bricks_level ();
   ~supervisor_bricks_level ();
  Sint32 first_init ();
  Sint32 main_loop ();

private:
  void init_level ();
  void changebkgd ();
  Sint32 background (Sint32 nbkdg = -1);
};
#endif
