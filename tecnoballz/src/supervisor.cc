/** 
 * @file supervisor.cc 
 * @brief Virtual class for all supervisors 
 * @date 2007-02-04
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor.cc,v 1.1 2007/02/04 20:17:32 gurumeditation Exp $
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
#include "../include/supervisor.h"

/**
 * Create supervisor object
 */
supervisor::supervisor ()
{
  initialise ();
}

/**
 * Release supervisor object
 */
supervisor::~supervisor ()
{
  liberation ();
}

void
supervisor::initialise ()
{
  mentatInit ();
  end_return = 0;
}

void
supervisor::liberation ()
{
  mentatKill ();
}

Sint32
supervisor::first_init ()
{
  return erreur_num;
}

Sint32
supervisor::main_loop ()
{
  return erreur_num;
}
