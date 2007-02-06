/** 
 * @file handler_players.cc 
 * @brief players handler 
 * @date 2007-02-06
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_players.cc,v 1.2 2007/02/06 16:52:13 gurumeditation Exp $
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
#include "../include/handler_players.h"
#include "../include/zeGemstone.h"
#include "../include/briqueCote.h"
#include "../include/zeGemstone.h"

Sint32 handler_players::totalActif = 0;
Sint32 handler_players::joueur_run = 0;
handler_players * handler_players::player_one = NULL;
Sint32 handler_players::best_score = 0;
char handler_players::bestPlayer[6] = { ' ', ' ', ' ', ' ', ' ', ' ' };
handler_players ** handler_players::playerlist = NULL;

/* 
 * Create the players handler
 */
handler_players::handler_players ()
{

  object_init ();

  //###################################################################
  // add a new player
  //###################################################################
  if (!totalActif)
    {
      player_one = this;
      playerNext = this;
      playerPrev = this;
    }
  else
    {
      playerNext = player_one;
      handler_players *avant = player_one->prevPlayer ();
      playerPrev = avant;
      player_one->prevPlayer (this);
      avant->nextPlayer (this);
    }
  totalActif++;

  //###################################################################
  // RAZ members
  //###################################################################
  player_num = totalActif;
  resetvalue ();
  Uint32 i;
  for (i = 0; i < 6; i++)
    nameString[i] = ' ';        //reset name of the player
  nameString[i] = 0;
}

/* 
 * Release the players handler
 */
handler_players::~handler_players ()
{
  totalActif--;
  if (totalActif)
    {
      playerNext->prevPlayer (playerPrev);
      playerPrev->nextPlayer (playerNext);
      if (player_one == this)
        player_one = playerNext;
    }
  else
    player_one = NULL;
  object_free ();
}

//-----------------------------------------------------------------------------
// initialize simple player before a new game
// input        => lifes: number of lifes 
//                      => areaN: area number (1 to 5)
//                      => level: level number (1 to 12)
//                      => monay: credit
//                      => grdPt: level_list of the guards
//-----------------------------------------------------------------------------
void
handler_players::initialise (Sint32 lifes, Sint32 areaN, Sint32 level,
                             Sint32 monay, Sint32 grdPt)
{
  resetvalue ();
  superLifes = lifes;
  areaNumber = areaN;
  levelTecno = level;
  levelTecno = level;
  creditFric = monay;
  guardianPt = grdPt;
}

//-----------------------------------------------------------------------------
// reset some members values
//-----------------------------------------------------------------------------
void
handler_players::resetvalue ()
{
  Uint32 z = 0;
  superScore = z;               //reset score of the player
  score_life = 0;
  areaNumber = 1;               //area number
  levelTecno = 1;               //level number into the area
  superLifes = vieInitial;      //number of lifes
  activation = 1;               //flag = 1, player's active
  RAZ_course ();
  creditFric = 500;             //amount of money 
  for (Uint32 i = 0; i < briqueCote::BRICOTENUM; i++)
    {
      bricotLeft[i] = 1;        //state of the left wall
      bricotRigh[i] = 1;        //state of the right wall
      bricot_top[i] = 1;        //state of the top wall
    }
  bump2Actif = z;               //right bumper disable
  bump3Actif = z;               //top bumper disable
  bump4Actif = z;               //left bumper disable
  rebuild_ok = z;               //reset flag "rebuild wall" option
  less_brick = z;               //reset flag "less brick" option
  life_bonus = z;               //counter of points giving a free life
  paddle_length = 32 * resolution;      //width of the bumper
  bonusPrice = z;               //reset flag "bonus price" option
  guardianPt = 0;
  RAZgemlist ();
}

//-----------------------------------------------------------------------------
// initialize a new name
//-----------------------------------------------------------------------------
void
handler_players::setNewName (char *pName)
{
  for (Uint32 i = 0; i < 6; i++)
    nameString[i] = ' ';
  for (Uint32 i = 0; i < 6; i++)
    {
      char c = pName[i];
      if (!c)
        return;
      if (c >= 'a' && c <= 'z')
        c = c - ('a' - 'A');
      if ((c >= ' ' && c <= '!') ||
          (c >= '-' && c <= '.') ||
          (c >= '0' && c <= ':') || (c >= 'A' && c <= 'Z') || c == '\'')
        nameString[i] = c;
      else
        nameString[i] = ' ';
    }
}

//-----------------------------------------------------------------------------
// return current name
//-----------------------------------------------------------------------------
char *
handler_players::returnName ()
{
  return &nameString[0];
}

//-----------------------------------------------------------------------------
// return area number
//      output  <= areaNumber: area number (1 to 5)
//-----------------------------------------------------------------------------
Sint32
handler_players::getAreaNum ()
{
  return areaNumber;
}

//-----------------------------------------------------------------------------
// return level number
//      output  <= levelTecno: level number (1 to 13)
//-----------------------------------------------------------------------------
Sint32
handler_players::getLevelNu ()
{
  return levelTecno;
}

//-----------------------------------------------------------------------------
// return the number of life(s)
//      output  <= superLifes: number of life(s)
//-----------------------------------------------------------------------------
Sint32
handler_players::getLifeNum ()
{
  return superLifes;
}

//-----------------------------------------------------------------------------
// return bumper width
//-----------------------------------------------------------------------------
Sint32
handler_players::get_paddle_width ()
{
  return paddle_length;
}

//-----------------------------------------------------------------------------
// initialize bumper width
//-----------------------------------------------------------------------------
void
handler_players::setLargeur (Sint32 large)
{
  paddle_length = large;
}

//-----------------------------------------------------------------------------
// return credit value
//-----------------------------------------------------------------------------
Sint32
handler_players::get_credit ()
{
  return creditFric;
}

//-----------------------------------------------------------------------------
// decrease credit
//-----------------------------------------------------------------------------
Sint32
handler_players::sub_credit (Sint32 value)
{
  if (value > creditFric)
    return 0;
  creditFric = creditFric - value;
  return 1;
}

//-----------------------------------------------------------------------------
// increase credit
//-----------------------------------------------------------------------------
void
handler_players::add_credit (Sint32 value)
{
  creditFric = creditFric + value;
}

//-----------------------------------------------------------------------------
// increase score
//-----------------------------------------------------------------------------
void
handler_players::add_scores (Sint32 value)
{
  superScore += value;
  score_life += value;
  if (score_life > 25000)
    {
      lifes_plus (1);
      score_life -= 25000;
    }
}

//-----------------------------------------------------------------------------
// RAZ the list of bonuses bought
//-----------------------------------------------------------------------------
void
handler_players::RAZ_course ()
{
  Sint32 t = NB_OPTIONS;
  Sint32 z = 0;
  Sint32 *p = courseList;
  for (Sint32 i = 0; i < t; i++)
    *(p++) = z;
  courseNmbr = z;               // 0 = number of bonuses bought
  *p = -1;                      // end of list
}

//-----------------------------------------------------------------------------
// return memory pointer to the list of bonuses bought
// output       <= pointer to the start of list
//-----------------------------------------------------------------------------
Sint32 *
handler_players::get_course ()
{
  return courseList;
}

//-----------------------------------------------------------------------------
// return number of bonuses bought
// output       <= number of bonuses
//-----------------------------------------------------------------------------
Sint32
handler_players::get_cou_nb ()
{
  return courseNmbr;
}

//-----------------------------------------------------------------------------
// set number of bonuses bought
//      input   => value: number of bonuses
//-----------------------------------------------------------------------------
void
handler_players::set_cou_nb (Sint32 value)
{
  courseNmbr = value;
}

//-----------------------------------------------------------------------------
// Reset the list of gems collected
//-----------------------------------------------------------------------------
void
handler_players::RAZgemlist ()
{
  for (Sint32 i = 0; i < zeGemstone::NUMBER_GEM; i++)
    gemmeActif[i] = 0;          //states of the 6 gems
  gemmeNombr = 0;
}

//-----------------------------------------------------------------------------
// verify if the 6 gemstones are collected
//-----------------------------------------------------------------------------
Sint32
handler_players::gem_enable (Sint32 gemNu)
{
  gemmeActif[gemNu] = 1;
  for (Sint32 i = 0; i < zeGemstone::NUMBER_GEM; i++)
    if (!gemmeActif[i])
      return 0;
  RAZgemlist ();
  return 1;

}

//-----------------------------------------------------------------------------
// return the state of one of six gemstones
//-----------------------------------------------------------------------------
Sint32
handler_players::gem_is_set (Sint32 gemNu)
{
  return gemmeActif[gemNu];
}

//-----------------------------------------------------------------------------
// return the state of one of the bumper
// input        => bumpN: bumper number (2 = right, 3 = top, or 4 left)
// output       <= 0(disable) or > 1 (enable)
//-----------------------------------------------------------------------------
Sint32
handler_players::get_bumpOn (Sint32 bumpN)
{
  switch (bumpN)
    {
    case 2:
      return bump2Actif;
      break;
    case 3:
      return bump3Actif;
      break;
    default:
      return bump4Actif;
      break;
    }
}

//-----------------------------------------------------------------------------
// modify the state of one of the bumper
// input        => bumpN: bumper number (2 = right, 3 = top, or 4 left)
//                      => value: 0 (disable) or > 1 (enable)
//-----------------------------------------------------------------------------
void
handler_players::set_bumpOn (Sint32 bumpN, Sint32 value)
{
  switch (bumpN)
    {
    case 2:
      bump2Actif = value;
      break;
    case 3:
      bump3Actif = value;
      break;
    case 4:
      bump4Actif = value;
      break;
    }
}

//-----------------------------------------------------------------------------
// initialize state "less bricks" option
//-----------------------------------------------------------------------------
void
handler_players::set_lessBk (Sint32 value)
{
  less_brick = value;
}

//-----------------------------------------------------------------------------
// return state "less bricks" option
//-----------------------------------------------------------------------------
Sint32
handler_players::get_lessBk ()
{
  return less_brick;
}

//-----------------------------------------------------------------------------
// initialize state "bonus price" option
//-----------------------------------------------------------------------------
void
handler_players::set_Bprice (Sint32 value)
{
  bonusPrice = value;
}

//-----------------------------------------------------------------------------
// return state "bonus price" option
//-----------------------------------------------------------------------------
Sint32
handler_players::get_Bprice ()
{
  return bonusPrice;
}


//-----------------------------------------------------------------------------
// initialize state of "rebuild wall" option
//-----------------------------------------------------------------------------
void
handler_players::setRebuild (Sint32 build)
{
  rebuild_ok = build;
}

//-----------------------------------------------------------------------------
// return state of "rebuild wall" option
//-----------------------------------------------------------------------------
Sint32
handler_players::getRebuild ()
{
  return rebuild_ok;
}

//-----------------------------------------------------------------------------
// return state of the wall of left (small bricks)
//-----------------------------------------------------------------------------
char *
handler_players::getBriLeft ()
{
  return bricotLeft;
}

//-----------------------------------------------------------------------------
// return state of the wall of right (small bricks)
//-----------------------------------------------------------------------------
char *
handler_players::getBriRigh ()
{
  return bricotRigh;
}

//-----------------------------------------------------------------------------
// return state of the wall of top (small bricks)
//-----------------------------------------------------------------------------
char *
handler_players::getBri_top ()
{
  return bricot_top;
}

//-----------------------------------------------------------------------------
// is the lastest level of tecnoballz?
//      output <= 1: end of game :-)
//-----------------------------------------------------------------------------
Sint32
handler_players::zlastlevel ()
{
  if (areaNumber >= 5 && levelTecno >= 13)
    return 1;
  else
    return 0;
}

//-----------------------------------------------------------------------------
// next level
//      output <= 1: end of game :-)
//-----------------------------------------------------------------------------
Sint32
handler_players::next_level (Sint32 grdNx)
{
  Sint32 r = 0;
  if (is_verbose)
    printf
      ("handler_players::next_level() areaNumber=%i, levelTecno=%i grdNx=%i guardianPt =%i\n",
       areaNumber, levelTecno, grdNx, guardianPt);
  if (areaNumber == 5 && levelTecno == 13)
    {
      areaNumber = 1;
      levelTecno = 1;
      r = 1;                    //end of game
      guardianPt = 0;
    }
  else
    {
      if (areaNumber == 5 && levelTecno == 12)
        {
          levelTecno++;
          guardianPt += grdNx;
        }
      else
        {
          levelTecno++;
          if (levelTecno == 13)
            {
              areaNumber++;
              levelTecno = 1;
              guardianPt += grdNx;
            }
          if (levelTecno == 7)
            guardianPt += grdNx;
        }
    }
  if (is_verbose)
    printf
      ("handler_players::next_level() areaNumber=%i, levelTecno=%i,  guardianPt=%i\n",
       areaNumber, levelTecno, guardianPt);
  return r;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
Sint32
handler_players::level2jump ()
{
  Sint32 rcode = 4;             //menu
  if (levelTecno == 6 || levelTecno == 12 || levelTecno == 13)
    rcode = 3;                  //guard level
  else
    {
      if (levelTecno > 0 && levelTecno < 12)
        rcode = 2;              //shop phase
    }
  return rcode;
}

//-----------------------------------------------------------------------------
// return previous player
//-----------------------------------------------------------------------------
handler_players *
handler_players::prevPlayer ()
{
  return playerPrev;
}

//-----------------------------------------------------------------------------
// return next player
//-----------------------------------------------------------------------------
handler_players *
handler_players::nextPlayer ()
{
  return playerNext;
}

//-----------------------------------------------------------------------------
// set next player
//-----------------------------------------------------------------------------
void
handler_players::nextPlayer (handler_players * gamer)
{
  playerNext = gamer;
}

//-----------------------------------------------------------------------------
// set previous player
//-----------------------------------------------------------------------------
void
handler_players::prevPlayer (handler_players * gamer)
{
  playerPrev = gamer;
}

//-----------------------------------------------------------------------------
// return area number
//      output  <=      areaNumber: 1 to 5
//-----------------------------------------------------------------------------
Sint32
handler_players::getareaNum ()
{
  return areaNumber;
}

//-----------------------------------------------------------------------------
// get pointer to "level_list" of the guards
//-----------------------------------------------------------------------------
Sint32
handler_players::getGuardPt ()
{
  return guardianPt;
}

//-----------------------------------------------------------------------------
// set pointer to "level_list" of the guards
//-----------------------------------------------------------------------------
void
handler_players::setGuardPt (Sint32 grdPt)
{
  guardianPt = grdPt;
}

//-----------------------------------------------------------------------------
// extra lifes
//      input   => value: number of life(s)
//-----------------------------------------------------------------------------
void
handler_players::lifes_plus (Sint32 value)
{
  superLifes += value;
}

//-----------------------------------------------------------------------------
// decrease number of life
// retra: number of life(s)
//-----------------------------------------------------------------------------
Sint32
handler_players::lifesMoins (Sint32 retra)
{
  superLifes -= retra;
  if (superLifes > 0)
    return 1;
  else
    {
      superLifes = 0;
      return 0;
    }
}

//-----------------------------------------------------------------------------
// reset number of lifes
//-----------------------------------------------------------------------------
void
handler_players::lifesReset ()
{
  superLifes = 0;
}

//===============================================================================
// static methods
//===============================================================================

//-----------------------------------------------------------------------------
// static: return next player
//      input   => gamer: current "handler_players" object
//                      => rcode: pointer to "end_return"
//                      => vactu: current phase (1, 2, or 3) 
//                      => grdNx: pointer to "level_list" of the guards (NULL by default)
// output <= next "handler_players" object
//-----------------------------------------------------------------------------
handler_players *
handler_players::nextplayer (handler_players * gamer, Sint32 * rcode,
                             Sint32 vactu, Sint32 grdNx)
{
  Sint32 start = gamer->player_num;
  Sint32 index = start;
  if (vactu != 2)
    gamer->next_level (grdNx);
  for (Sint32 i = 0; i < totalActif; i++)
    {
      if (++index > totalActif)
        index = 1;
      handler_players *pPlay = playerlist[index - 1];
      if (pPlay->superLifes > 0)
        {
          *rcode = pPlay->level2jump ();
          if (pPlay->player_num <= start && *rcode == 2 && vactu == 2)
            *rcode = 1;
          // - 1: bricks levels
          // - 2: shop
          // - 3: guards levels
          // - 4: menu
          if (pPlay->player_num > start && vactu != 2)
            if (*rcode == 2)
              *rcode = 1;
          return pPlay;
        }
    }
  *rcode = 4;                   //improbable
  return playerlist[0];
}

//-----------------------------------------------------------------------------
// static: return first player
//-----------------------------------------------------------------------------
handler_players *
handler_players::firstGamer ()
{
  return player_one;
}

//-----------------------------------------------------------------------------
// static: initialize the number maximum of players
//      input   => total: number maximum of players (always 6)
//-----------------------------------------------------------------------------
handler_players *
handler_players::joueursADD (Sint32 total)
{
  Sint32 t = total;
  playerlist =
    (handler_players **) memory->alloc (t * sizeof (handler_players *),
                                        0x504C4159);
  if (!playerlist)
    return 0;
  for (Sint32 i = 0; i < t; i++)
    playerlist[i] = new handler_players ();
  return player_one;
}

//-----------------------------------------------------------------------------
// static: release all objects players
//-----------------------------------------------------------------------------
void
handler_players::joueursRAZ ()
{
  Sint32 t = totalActif;
  for (Sint32 i = 0; i < t; i++)
    delete player_one;
  if (playerlist)
    {
      memory->release ((char *) playerlist);
      playerlist = NULL;
    }
}
