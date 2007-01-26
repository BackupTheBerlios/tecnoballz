/** 
 * @file sprite_projectile.h 
 * @brief The fire sprite of the paddle into the bricks level
 * @date 2007-01-25
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_projectile.h,v 1.1 2007/01/25 20:33:51 gurumeditation Exp $
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
#ifndef __TECNO_FIRE__
#define __TECNO_FIRE__
//...............................................................................
class sprite_projectile;
//...............................................................................
#include "../include/sprite_object.h"
#include "../include/sprite_paddle.h"
#include "../include/lesBriques.h"
#include "../include/zeBouiBoui.h"
#include "../include/tecno_boui.h"
#include "../include/barreScore.h"
//...............................................................................
class sprite_projectile:public sprite_object
{
	friend class zeFireBump;
	friend class tecno_caps;	  
	friend class tecno_gads;
	friend class techno_gem;

	private:
		static const Sint32	maxi_fires = 200;
		static Sint32		total_fire;
		static sprite_projectile*	list_fires[maxi_fires];
		static lesBriques*	brickObjet;
		static zeBouiBoui*	atomsObjet;
		static barreScore*	scoreObjet;
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
		static void		start_list (lesBriques * brick, zeBouiBoui * atoms,
					barreScore * score);
		static void		gestionTir();
		static void		hors_ecran();
		static void		anim_fires();
		static void		collision1();
		static void		collision2();
		static void		disable_sprites();

};
#endif