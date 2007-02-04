/** 
 * @file supervisor.h
 * @brief Virtual class for all supervisors 
 * @date 2007-02-04
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor.h,v 1.1 2007/02/04 20:17:32 gurumeditation Exp $
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
#ifndef __SUPERVISOR__
#define __SUPERVISOR__
#include "../include/tecnoballz.h"
class supervisor:public virtual tecnoballz
{
protected:
  Sint32 end_return;

public:
  supervisor ();
  virtual ~ supervisor ();
  void initialise ();
  void liberation ();
  virtual Sint32 first_init ();
  virtual Sint32 main_loop ();
};
#endif
