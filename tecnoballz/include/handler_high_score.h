/** 
 * @file handler_high_score.cc 
 * @brief high score handler 
 * @created 2004-04-30 
 * @date 2007-03-08
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.1 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_high_score.h,v 1.1 2007/03/08 17:41:52 gurumeditation Exp $
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
#ifndef __HANDLER_HIGH_SCORE__
#define __HANDLER_HIGH_SCORE__

class handler_high_score;

#include "../include/tecnoballz.h"

typedef struct
{
  char playerName[8];           //name of player
  Uint32 scoreValue;
  Uint32 score_area;
  Uint32 scoreLevel;
} score_list;

class handler_high_score:public virtual tecnoballz
{
public:
  static const Uint32 MAX_OF_HIGH_SCORES = 10;

private:
  static handler_high_score *high_score_singleton;
  score_list ** the_scores;

private:
  Uint32 scores_table_size;
  Sint32 test_score (char *pName, Uint32 vScre, Uint32 nLevl, Uint32 nArea);
  void sort_score (Uint32 nHard);
  bool load_high_score ();
  void save_high_score ();
  Uint32 calculate_checksum (Uint32 * pBuff, Uint32 bsize);
  void first_init ();
private:
    handler_high_score ();
public:
    ~handler_high_score ();
  static handler_high_score *get_instance ();
  Sint32 test_score ();
  score_list *getScrList ();
  char *bestPlayer ();
  Uint32 best_score ();
};
#endif
