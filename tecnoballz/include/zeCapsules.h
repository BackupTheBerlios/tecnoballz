//******************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "zeCapsules.h"
// created		: ?
// updates		: 2004-04-12
// fonction	: Manage the capsules of money
//-----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA  02111-1307, USA.
//
//******************************************************************************
#ifndef __ZECAPSULES__
#define __ZECAPSULES__
//...............................................................................
class zeCapsules;
//...............................................................................
#include "../include/liste_BOBs.h"
#include "../include/objet_gere.h"
#include "../include/ecran_hard.h"
#include "../include/barreScore.h"
#include "../include/tecno_caps.h"
#include "../include/barreScore.h"
#include "../include/tableaux_Z.h"
#include "../include/printmoney.h"
#include "../include/technoBall.h"
//...............................................................................
class zeCapsules:public objet_gere < tecno_caps >
{
	private:
		Sint32			frequenceX;
		Sint32			zeCompteur;
		barreScore*		ptbarreScr;
		printmoney*		ptPrntmney;

	public:
						zeCapsules();
						~zeCapsules();
		void			initialise(Sint32 frequ, barreScore *score,
  							printmoney *money);
		void			initialise(Sint32 frequ, printmoney *money);
		void			envoieFric(brickClear * briPT);
		void			send_money(technoBall *pball);
		void			send_money(tecno_fire *pfire);
		void			envoieFric(technoBall *pball);
		void			bouge_fric();
		void			bougefric2();
};
#endif
