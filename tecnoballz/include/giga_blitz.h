//*****************************************************************************
// copyright (c) 1991-2002 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "giga_blitz.h"
// created      : ?
// updates      : 2002-11-23
// fonctions    : Implementation de l'objet gigablitz
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
//*****************************************************************************

#ifndef __GIGA_BLITZ__
#define __GIGA_BLITZ__

class giga_blitz;

#include "../include/BOB_killer.h"
#include "../include/objet_gere.h"
class giga_blitz:public BOB_killer
{
  private:
    
  public:
                          giga_blitz();
                         ~giga_blitz();
  
  

};

#endif
