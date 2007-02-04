/** 
 * @file handler_memory.h
 * @brief Handler of memory allocation 
 * @created 1996-07-05
 * @date 2007-01-15
 * @copyright 1998-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1998-2007 TLK Games all rights reserved
 * $Id: handler_memory.h,v 1.2 2007/02/04 17:10:16 gurumeditation Exp $
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
#ifndef __HANDLER_MEMORY__
#define __HANDLER_MEMORY__
class handler_memory;
#include "tecnoballz.h"

/** Memory-block description */
typedef struct
{
  /* pointer to memory zone */
  char *addr;
  /* size of memory zone in bytes */
  Sint32 size;
}
mem_struct;

class handler_memory:public virtual tecnoballz
{
private:
  mem_struct * memory_list_base;
  mem_struct *memory_list;
  /** Maximum number of memory zones */
  Sint32 max_of_zones;
  /** Current number of memory zones */
  Sint32 num_of_zones;
  /** Memory currently occupied */
  Sint32 total_size;
  /** Maximum memory size reached */
  Sint32 max_size_allocated;
  Sint32 free_fail_count;
  /** Maximum number of zones memory which were allocated */
  Sint32 maxi_zones;

public:
    handler_memory ();
   ~handler_memory ();
  Sint32 init (Sint32 nombr);
  char *alloc (Sint32 size, Uint32 id = 0);
  void release (char *addr);
  Sint32 get_total_size ();
private:
  void release_all ();
};
extern handler_memory *memory;
#endif
