/** 
 * @file supervisor.h
 * @brief Virtual class for all supervisors 
 * @date 2007-04-10
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor.h,v 1.3 2007/09/12 06:32:48 gurumeditation Exp $
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
  virtual void first_init ();
  virtual Sint32 main_loop ();
};
#endif
