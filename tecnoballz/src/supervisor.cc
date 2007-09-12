/** 
 * @file supervisor.cc 
 * @brief Virtual class for all supervisors 
 * @date 2007-02-04
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.4 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor.cc,v 1.4 2007/09/12 06:32:49 gurumeditation Exp $
 *
 * TecnoballZ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * TecnoballZ is distributed in the hope that it will be useful, but
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
  object_init ();
  end_return = 0;
}

void
supervisor::liberation ()
{
  object_free ();
}

void
supervisor::first_init ()
{
}

Sint32
supervisor::main_loop ()
{
  return MAIN_MENU;
}
