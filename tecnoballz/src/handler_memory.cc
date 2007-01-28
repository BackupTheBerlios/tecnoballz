/** 
 * @file handler_memory.cc 
 * @brief Handler of memory allocation 
 * @created 1996-07-05
 * @date 2007-01-16
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_memory.cc,v 1.2 2007/01/28 21:31:56 gurumeditation Exp $
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
#include "../include/handler_memory.h"

/**
 * Create object
 */
handler_memory::handler_memory ()
{
  mentatInit ();
  total_size = 0;
  free_fail_count = 0;
  maxi_zones = 0;
}

/**
 * Release object
 */
handler_memory::~handler_memory ()
{
  release_all ();
  mentatKill ();
  if (memory_list_base == NULL)
    {
      free ((char *) memory_list_base);
      memory_list_base = NULL;
      memory_list = NULL;
    }
}

/**
 * Initialize the memory table for our malloc() wrapper
 * @param numofzones maximum number of memory zones
 * @return boolean value on success or failure
 */
Sint32
handler_memory::init (Sint32 numof)
{
  num_of_zones = 0;
  max_of_zones = numof;
  total_size = (Sint32) (max_of_zones * sizeof (mem_struct));
  max_size_allocated = total_size;
  memory_list_base = (mem_struct *) malloc (total_size);
  if (memory_list_base == NULL)
    {
      return (erreur_num = E_NOMEMORY);
    }
  memory_list = memory_list_base;
  return E_NO_ERROR;
}

/** 
 * Allocate memory, malloc() wrapper
 * @param size size in bytes to alloc
 * @return pointer to the allocated memory or NULL if an error occurred
 */
char *
handler_memory::alloc (Sint32 size, Uint32 ident)
{
  char *addr;
  size *= double_mem;
  std::cout << "handler_memory::alloc: " << size << std::endl;

  if (num_of_zones >= max_of_zones)
    {
      /* out of zones */
      erreur_num = E_OUT_ZONE;
      fprintf (stderr, "handler_memory.cc::reserveMem() "
               "max. number of zones reached: %i\n", num_of_zones);
      return NULL;
    }
  addr = (char *) malloc ((Sint32) size);
  if (addr == NULL)
    {
      /* allocate error */
      erreur_num = E_NOMEMORY;
      fprintf (stderr, "(!)handler_memory.cc::reserveMem() "
               "out of memory!\n");
      return NULL;
    }
  total_size += size;
  if (total_size > max_size_allocated)
    {
      max_size_allocated = total_size;
    }
  memory_list->addr = addr;
  memory_list->size = size;
  memory_list++;
  if (++num_of_zones > maxi_zones)
    {
      maxi_zones = num_of_zones;
    }
  memset (addr, 0, sizeof (char) * size);
  return addr;
}

/**
 * Deallocates the memory, free() wrapper 
 * @param addr pointer to memory
 **/
void
handler_memory::release (char *addr)
{
  if (addr == NULL)
    {
      return;
    }
  mem_struct *mem_list = memory_list_base;
  for (Sint32 i = 0; i < num_of_zones; i++, mem_list++)
    {
      if (mem_list->addr != addr)
        {
          continue;
        }
      free (addr);
      mem_struct *monPT = mem_list + 1;
      total_size -= mem_list->size;
      num_of_zones--;
      memory_list--;
      while (i < num_of_zones)
      {
        mem_list->addr = monPT->addr;
        mem_list->size = monPT->size;
        i++;
        mem_list++;
        monPT++;
      }
      monPT->addr = NULL;
      monPT->size = 0;
      addr = (char *) NULL;
      break;
    }
  if (addr != NULL)
    {
      free_fail_count++;
#if __WORDSIZE == 64
      fprintf (stderr, "(!)handler_memory::free() "
               "%i) address %x not found!\n",
               free_fail_count, (long) addr);
#else
      fprintf (stderr, "(!)handler_memory::free() "
               "%i) address %x not found!\n",
               free_fail_count, (Sint32) addr);
#endif
    }
}

/**
* Releases all memory allocated
*/
void
handler_memory::release_all ()
{
  if (num_of_zones > 0)
    {
      fprintf (stderr, "handler_memory::release_all() "
               "remain %i not released zones\n", num_of_zones);
      mem_struct *mem_list = memory_list_base;
      for (Sint32 i = 0; i < num_of_zones; i++, mem_list++)
        {
          if (mem_list->addr != NULL)
            {
              fprintf (stderr, "handler_memory::release_all() "
                       "size: %i\n", mem_list->size);
              free (mem_list->addr);
              total_size -= mem_list->size;
              mem_list->addr = NULL;
              mem_list->size = 0;
            }
        }
      fprintf (stderr, "handler_memory::release_all() "
               "remain %i not released zones\n", num_of_zones);
    }
  if (free_fail_count > 0)
    {
      fprintf (stderr, "handler_memory::release_all() "
               "release %i already released addresses!\n", free_fail_count);
    }
  if (is_verbose)
    {
      fprintf (stdout, "handler_memory::release_all() "
               "maximum number of zones memory which "
               "were allocated during the game: %i\n", maxi_zones);
      fprintf (stdout, "handler_memory::release_all(): "
               "maximum of memory which "
               "were allocated during the game: %i\n", max_size_allocated);
    }
}

/**
 * Return total allocate memory
 */
Sint32
handler_memory::get_total_size ()
{
  return total_size;
}
