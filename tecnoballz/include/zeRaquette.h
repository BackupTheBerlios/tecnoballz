//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeRaquette.h"
// created		: ?
// updates		: 2004-08-30
// fonction	: manage the bumpers
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
//*****************************************************************************
#ifndef __ZERAQUETTE__
#define __ZERAQUETTE__
//...............................................................................
class zeRaquette;
//...............................................................................
#include "../include/sprite_paddle.h"
#include "../include/barreScore.h"
#include "../include/objects_list.h"
#include "../include/zeNewBalls.h"
#include "../include/zeGigaBlit.h"
//...............................................................................

class zeRaquette:public objects_list < sprite_paddle >
{
	friend class sprite_paddle;

	private:
		Sint32			width_mini;	//bumper mini width (32 or 64)
		Sint32			width_maxi;	//bumper maxi width (64 or 128)
		//Sint32			width_deca; //3 or 4; offset between 2 size (2^3)
	
		Sint32			bumperMini;
		Sint32			bumperMaxi;
 		Sint32			bumperYhau;
		Sint32			bumperYbas;
		Sint32			bumperXgau;
		Sint32			bumperXdro;
		list_sprites*		objet_draw;
		Sint32			raket_team;		//0=1 player ; 1=2 players (no implemented)
		Sint32			raketLarge;		//width bumper
		Sint32			raketDepla;		//sens de deplacement 1 <- 0 -> 2
		Sint32			rakVgauche;		//vitesse gauche
		Sint32			rakVdroite;		//vitesse droite
		Sint32			balle_glue;		//temps de la colle
		Sint32			rak_invers;		//if > 0 reverse bumpers
		sprite_paddle*		tecBumper1;
		sprite_paddle*		tecBumper2;
		sprite_paddle*		tecBumper3;
		sprite_paddle*		tecBumper4;
		sprite_paddle*		tec_robot0;
		barreScore*		ptBarreScr;
		zeGigaBlit*		ptGigaBlit;
		zeNewBalls*		ptNewBalls;


		static Sint32	ballePets1[14];
		static Sint32	ballePets2[14];
		static Sint32	ballePets3[14];
		static Sint32	ballePets4[14];
		
		static const Sint32	rb09[16];
		static const Sint32	rb10[16];
		static const Sint32	rb11[16];
		static const Sint32	rb12[16];
		static const Sint32	rb13[16];
		static const Sint32	rb14[16];
		static const Sint32	rb15[16];
		static const Sint32	rb16[16];
		static const Sint32	rb17[16];
		static const Sint32* midi1_left[];
		static const Sint32* midi1Right[];
		
		static const Sint32	rb18[];
		static const Sint32	rb19[];
		static const Sint32	rb20[];
		static const Sint32	rb21[];
		static const Sint32	rb22[];
		static const Sint32	rb23[];
		static const Sint32	rb24[];
		static const Sint32	rb25[];
		static const Sint32	rb26[];
		static const Sint32*	midi2_left[];
		static const Sint32*	midi2Right[];


		static const Sint32	rb27[];
		static const Sint32	rb28[];
		static const Sint32	rb29[];
		static const Sint32	rb30[];
		static const Sint32	rb31[];
		static const Sint32	rb32[];
		static const Sint32	rb33[];
		static const Sint32	rb34[];
		static const Sint32	rb35[];
		static const Sint32*	midi3_left[];
		static const Sint32*	midi3Right[];

		static const Sint32	rb36[];
		static const Sint32	rb37[];
		static const Sint32	rb38[];
		static const Sint32	rb39[];
		static const Sint32	rb40[];
		static const Sint32	rb41[];
		static const Sint32	rb42[];
		static const Sint32	rb43[];
		static const Sint32	rb44[];
		static const Sint32*	midi4_left[];
		static const Sint32*	midi4Right[];



		

	public:
							zeRaquette();
							zeRaquette(Sint32 nBob);
							~zeRaquette();
		Sint32				init_fires();
		Sint32				init_liste();
		void				lache_tirs();
		void				lacheBalle();
		void				lacheBall2();
		void				free_balls();
		void				bp_deplace();
		void				bp_deplac2();
		void				initBumper(barreScore *score, zeGigaBlit *blitz,
									zeNewBalls* balls);
		Sint32				init_robot();
		void				activrobot();
		void				deactrobot();
		void				move_robot();
		sprite_paddle*			demandeRak(Sint32);
		void				maxi_bumps();
		void				incremente();
		void				decremente();
		void				initinvers(Sint32);
		Sint32				get_invers();
		void				bumpersOff();
};
#endif
