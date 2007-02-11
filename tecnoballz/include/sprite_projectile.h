/** 
 * @file sprite_projectile.h 
 * @brief The fire sprite of the paddle into the bricks level
 * @date 2007-02-11
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.10 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_projectile.h,v 1.10 2007/02/11 21:03:24 gurumeditation Exp $
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
#ifndef __SPRITE_PROJECTILE__
#define __SPRITE_PROJECTILE__

class sprite_projectile;

#include "../include/sprite_object.h"
#include "../include/sprite_paddle.h"
#include "../include/controller_bricks.h"
#include "../include/controller_ships.h"
#include "../include/sprite_ship.h"
#include "../include/right_panel_score.h"

class sprite_projectile:public sprite_object
{
	friend class controller_projectiles;
	friend class sprite_money;	  
	friend class sprite_capsule;
	friend class sprite_gem;

	private:
		static const Sint32	maxi_fires = 200;
		static Sint32		total_fire;
		static sprite_projectile*	list_fires[maxi_fires];
		static controller_bricks*	brickObjet;
		static controller_ships*	atomsObjet;
		Sint32				indexSinus;
		Sint32				fire_Xscie;
		Sint32				fire_Yscie;                          
		sprite_paddle*			raquettePT;
		Sint32				fire_power;		// 1 = fire destroys the indestructible-destructibles bricks 
		Sint32				firePowerX;		// fire power 1 or 2

	public:
						sprite_projectile();
						~sprite_projectile();
		void			littleInit(sprite_paddle * raket);
		void			firePower1();
		void			firePower2();
		static void		start_list (controller_bricks * brick, controller_ships * atoms);
		static void		gestionTir();
		static void		hors_ecran();
		static void		anim_fires();
		static void		collision1();
		static void		collision2();
		static void		disable_sprites();
};
#endif
