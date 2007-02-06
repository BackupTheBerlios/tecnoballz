/** 
 * @file supervisor_guards_level.h
 * @brief Guardians level supervisor 
 * @created 2003-01-09
 * @date 2007-02-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.4 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_guards_level.h,v 1.4 2007/02/06 12:26:01 gurumeditation Exp $
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
#ifndef __SUPERVISOR_GUARDS_LEVEL__
#define __SUPERVISOR_GUARDS_LEVEL__
class supervisor_guards_level;
#include "../include/lastScroll.h"
#include "../include/tecno_miss.h"
#include "../include/handler_display.h"
#include "../include/handler_keyboard.h"
#include "../include/supervisor.h"
#include "../include/controller_guardians.h"
#include "../include/zeMissiles.h"
#include "../include/controller_paddles.h"
#include "../include/controller_balls.h"
#include "../include/zeCapsules.h"
#include "../include/ze_gadgets.h"
#include "../include/printmoney.h"
#include "../include/zeMiniMess.h"
#include "../include/zeMiniMess.h"
#include "../include/zeGigaBlit.h"
#include "../include/zexplosion.h"
#include "../include/ballDirect.h"
#include "../include/zeGameOver.h"
#include "../include/zeCongBall.h"
#include "../include/handler_popup_menu.h"
#include "../include/zeMoveText.h"


class supervisor_guards_level:public virtual supervisor
{
  friend class zeMoveText;

private:
  zeGigaBlit * ptGigaBlit;
  lastScroll *defilement;
  controller_guardians *guards;
  zeMissiles *ptMissiles;
  controller_paddles *paddles;
  controller_balls *balls;
  ballDirect *ptBaDirect;
  zeCapsules *ptCapsules;
  ze_gadgets *pt_gadgets;
  printmoney *ptPrntmney;
  zeMiniMess *ptMiniMess;
  zexplosion *pExplosion;
  sprite_object *ptBobMoney;
  sprite_object *ptBob_name;
  tecno_gads *ptBobLifes;
  handler_popup_menu *popup_menu;
  /** Area number from 1 to 5 */
  Uint32 areaNumber;
  /** Current level number 6, 12 or 13 */ 
  Uint32 levelTecno;
  const atariLevel *levelParam;
  Sint32 scrollTemp;            // time before scroll start
  Sint32 scrollType;            // type of background scrolling
  Sint32 scrolSpeed;            // speed of the scrolling
  zeGameOver *ptGameOver;
  zeCongBall *ptCongBall;
  Sint32 isgameover;
  Sint32 count_next;            //temporisation before next level 
  Sint32 tecnwinner;            //1 = end of the game
  zeMoveText *ptMoveText;

private:
  void init_level ();
  void run_scroll ();
  void cheat_keys ();

public:
    supervisor_guards_level ();
   ~supervisor_guards_level ();
  Sint32 first_init ();
  Sint32 main_loop ();
};
#endif
