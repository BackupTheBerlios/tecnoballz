//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeNewBalls.h"
// created		: ?
// updates		: 2004-10-10
// fonction	: manage the balls (move and collisions)
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
#ifndef __ZENEWBALLS__
#define __ZENEWBALLS__
//...............................................................................
class zeNewBalls;
//...............................................................................
#include "../include/technoBall.h"
#include "../include/objet_gere.h"
#include "../include/tecno_bump.h"
#include "../include/ejectBalls.h"
#include "../include/lesBriques.h"
#include "../include/briqueCote.h"
#include "../include/head_anima.h"
#include "../include/zeBouiBoui.h"
#include "../include/barreScore.h"
#include "../include/zeguardian.h"
#include "../include/zeCapsules.h"
#include "../include/ze_gadgets.h"
#include "../include/zeMiniMess.h"
#include "../include/zeRaquette.h"
#include "../include/ze_magneye.h"
#include "../include/techno_eye.h"

//...............................................................................
class zeNewBalls:public objet_gere < technoBall >
{
	friend class ejectBalls;
 	friend class technoBall;

	private:
		Sint32			num_erreur;
		Sint32			balle_glue;		//duration of the glue
		Sint16*			balleVites;		// table premiere vitesse
		Sint32			tempoVites;		// temps avant changement vitesse
		Sint32			startCount;		// Temps avant que la balle ne parte
		Sint32			balle_tilt;		// Temps avant que le tilt soit possible
		Sint32			balle_ctrl;		//1 = control ball (left mouse button)
		tecno_bump*		tecBumper1;		//bottom bumper
		tecno_bump*		tecBumper2;		//right bumper
		tecno_bump*		tecBumper3;		//top bumper
		tecno_bump*		tecBumper4;		//left bumper
		tecno_bump*		tec_robot0;		//robot bumper
		ejectBalls*		ejectObjet;
		lesBriques*		brickObjet;
		briqueCote*		bricoObjet;
		head_anima*		gugusObjet;
		zeBouiBoui*		ptBouiBoui;
		barreScore*		ptBarreScr;
		zeguardian*		ptguardian;
		zeCapsules*		ptCapsules;
		ze_gadgets*		pt_gadgets;
		BOB_killer*		ptBob_wall;
		zeMiniMess*		ptMiniMess;
		ze_magneye*		pt_magneye;
		
		private:
			static Sint32	ballEject1[];
			static Sint32	ballEject2[];
			static Sint32	ballEject3[];
			static Sint32	ballEject4[];
			static Sint32	rb0[16];
			static Sint32	rb1[16];
			static Sint32	rb2[16];
			static Sint32	rb3[16];
			static Sint32	rb4[16];
			static Sint32	rb5[16];
			static Sint32	rb6[16];
			static Sint32	rb7[16];
			static Sint32	rb8[16];
			static Sint32*	brick_jump[15];
 
	public:
						zeNewBalls(ejectBalls *eject, lesBriques *brick, 
									briqueCote *brico, head_anima* gugus, 
									zeBouiBoui *atoms, barreScore* score,
									BOB_killer *pwall, zeMiniMess*,
									ze_magneye* pEyes);
						zeNewBalls(zeguardian*, zeCapsules*, ze_gadgets*);
						~zeNewBalls();
		void			init_balle(zeRaquette *raket, Sint32 start, Sint32 glueC, 
							Sint32 speed, Sint32 tiltC, Sint32 table);
		void			vitusBalle();
		void			vitusBall2();
		technoBall*		first_ball();

	private:
		void			vitus_sort();
		void			vitussort2();
		void			vitus_tilt();
		void			vitus_move();
		void			accelerate();
		void			vitusmove2();
		void			vitus_bump();
		void			vitusbump2();
		void			vitusrobot();
		void			vitusEject();
		void			vitus_cote();
		void			vituscote2();
		void			vitusbound();
		void			vitusBrick();
		void			vitusAtoms();
		void			vitus_eyes();
		void			vitusGuard();
		void			vitus_ctrl();
		

	public:
		void			runcontrol();
		void			run_2balls();
		void			run_nballs(Sint32 nball = 0);
		void			run_3balls();
		void			run_power1();
		void			run_power2();
		void			run_size01();
		void			run_size02();
		void			maxi_speed();
		void			time_2tilt();
		void			time2tilt2();
		Sint32			least_glue();

};
#endif
