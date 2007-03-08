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
 * $Id: handler_high_score.cc,v 1.1 2007/03/08 17:41:52 gurumeditation Exp $
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
#include "../include/handler_high_score.h"
#include "../include/sprite_display_menu.h"
#include "../include/handler_resources.h"
#include "../include/handler_players.h"

handler_high_score * handler_high_score::high_score_singleton = NULL;

/**
 * Create a high score handler 
 */
handler_high_score::handler_high_score ()
{
  the_scores = (score_list **) NULL;
  scores_table_size =
    (sizeof (Uint32) * 3 + 8) * MAX_OF_HIGH_SCORES * MAX_OF_DIFFICULTY_LEVELS +
    sizeof (Uint32);
  first_init ();
}

/**
 * Get the object instance
 * handler_high_score is a singleton
 * @return the handler_high_score object 
 */
handler_high_score *
handler_high_score::get_instance ()
{
  if (NULL == high_score_singleton)
    {
      high_score_singleton = new handler_high_score ();
    }
  return high_score_singleton;
}

/**
 * Release high score handler 
 */
handler_high_score::~handler_high_score ()
{
  if (NULL == the_scores)
    {
      return;
    }
  for (Uint32 i = 0; i < MAX_OF_DIFFICULTY_LEVELS; i++)
    {
      if (NULL != the_scores[i])
        {
          //memory->release ((char *) the_scores[i]);
          delete[](char *)the_scores[i];
          the_scores[i] = NULL;
        }
    }
  //memory->release ((char *) the_scores);
  delete[](char *)the_scores;
  the_scores = NULL;
}

/**
 * Perform some initializations
 */
void
handler_high_score::first_init ()
{
  try
  {
  // allocate memory for the 4 pointers to the 4 scores tables
  the_scores = new score_list*[MAX_OF_DIFFICULTY_LEVELS];
  /*
    (score_list **) memory->alloc (sizeof (score_list *) *
                                   MAX_OF_DIFFICULTY_LEVELS);
  error_init (memory->retour_err ());
  if (erreur_num)
    return erreur_num;
    */

  //fill score table
  for (Uint32 i = 0; i < MAX_OF_DIFFICULTY_LEVELS; i++)
    {
      /*
      the_scores[i] =
        (score_list *) memory->alloc (sizeof (score_list) * MAX_OF_HIGH_SCORES);
      error_init (memory->retour_err ());
      if (erreur_num)
        return erreur_num;
        */
      the_scores[i] = new score_list[MAX_OF_HIGH_SCORES];

      score_list *score = the_scores[i];
      for (Uint32 j = 0; j < MAX_OF_HIGH_SCORES; j++)
        {
          score[j].score_area = 1;
          score[j].scoreLevel = 1;
          score[j].scoreValue = 1000 - j * 100;
          score[j].playerName[0] = 'T';
          score[j].playerName[1] = ' ';
          score[j].playerName[2] = 'L';
          score[j].playerName[3] = ' ';
          score[j].playerName[4] = 'K';
          score[j].playerName[5] = ' ';
          score[j].playerName[6] = 0;
          score[j].playerName[7] = 0;   //SPARC unaligned memory access
        }
    }
  }
  catch (std::bad_alloc &)
  {
    std::cerr << "(!)handler_high_score::first_init() " <<
      "not enough memory to allocate high score table!" << std::endl;
    throw;
  }
  if (!load_high_score ())
    {
      save_high_score ();
    }
}

/**
 * Load high score file and copy data in high score structure
 * @return true if the high score file is successfully loaded or false otherwise
 */
bool
handler_high_score::load_high_score ()
{
  char *file_data = resources->load_high_score_file ();
  if (NULL == file_data)
    {
      return false;
    }
      Uint32 file_size = resources->get_filesize_loaded ();
      if (file_size != scores_table_size)
        {
          std::cerr << "(!)handler_high_score::load_high_score() " <<
            " bad file size, " << file_size << " byte(s) instead" <<
            scores_table_size << "bytes!" << std::endl;
          delete[]file_data;
          return false;
        }
      Uint32 ckVal;
      big_endian_to_int ((Uint32 *) file_data, &ckVal);
      Uint32 *pSelf = (Uint32 *) file_data;
      Uint32 value = calculate_checksum (pSelf + 1,
                                 (scores_table_size -
                                  sizeof (Uint32)) / sizeof (Uint32));
      if (value != ckVal)
        {
          std::cerr << "(!)handler_high_score::load_high_score() " <<
            " bad checksum, " << value << " instead " <<
            *pSelf << std::endl;
          delete[]file_data;
          return false;
        }

      // copy data into the structures
      char *pTemp = file_data + sizeof (Uint32);
      for (Uint32 i = 0; i < MAX_OF_DIFFICULTY_LEVELS; i++)
        {
          score_list *score = the_scores[i];
          for (Uint32 j = 0; j < MAX_OF_HIGH_SCORES; j++)
            {
              Uint32 *ptM32 = (Uint32 *) pTemp;
              big_endian_to_int (ptM32++, &score[j].score_area);
              big_endian_to_int (ptM32++, &score[j].scoreLevel);
              big_endian_to_int (ptM32++, &score[j].scoreValue);
              pTemp = (char *) ptM32;
              for (Uint32 k = 0; k < 8; k++)
                {
                  score[j].playerName[k] = *(pTemp++);
                }
            }
        }
      delete[]file_data;
      return true;
}

/**
 * Save the high score
 */
void
handler_high_score::save_high_score ()
{
  char *file_data = new char[scores_table_size];

  /*
  char *file_data = memory->alloc (scores_table_size, 0x53434F52);
  error_init (memory->retour_err ());
  if (erreur_num)
    return erreur_num;
  */
  Uint32 *pSelf = (Uint32 *) file_data;

  char *pTemp = file_data + sizeof (Uint32);
  for (Uint32 i = 0; i < MAX_OF_DIFFICULTY_LEVELS; i++)
    {
      score_list *score = the_scores[i];
      for (Uint32 j = 0; j < MAX_OF_HIGH_SCORES; j++)
        {
          Uint32 *ptM32 = (Uint32 *) pTemp;
          int_to_big_endian (&score[j].score_area, ptM32++);
          int_to_big_endian (&score[j].scoreLevel, ptM32++);
          int_to_big_endian (&score[j].scoreValue, ptM32++);
          pTemp = (char *) ptM32;
          for (Uint32 k = 0; k < 8; k++)
            {
              *(pTemp++) = score[j].playerName[k];
            }
        }
    }

  //calculate checksum value
  Uint32 ckVal = calculate_checksum (pSelf + 1,
                             (scores_table_size -
                              sizeof (Uint32)) / sizeof (Uint32));
  int_to_big_endian (&ckVal, pSelf);
  resources->save_high_score_file (file_data, scores_table_size);
  //memory->release (file_data);
  delete[]file_data;
  //return 0;
}

/**
 * Calculate checksum value
 * @param addr pointer to the data
 * @param data_size size of the data in long words
 * @return checksum value
 */
Uint32
handler_high_score::calculate_checksum (Uint32 *addr, Uint32 data_size)
{
  Uint32 value = 0;
  for (Uint32 i = 0; i < data_size; i++)
    {
      Uint32 long_word;
      big_endian_to_int (addr++, &long_word);
      value |= long_word;
    }
  return value;
}

//------------------------------------------------------------------------------
// check if the current player can be inserted in the scores table
//------------------------------------------------------------------------------
Sint32
handler_high_score::test_score ()
{
  Sint32 fhigh = test_score (&current_player->player_name[0],
                             current_player->score_value,
                             current_player->level_number,
                             current_player->area_number);
  if (fhigh)
    {
      sort_score (difficulty_level);
      save_high_score ();
    }
  return fhigh;
}

//------------------------------------------------------------------------------
// insert a player in the score table if this player get a good score
//      input   => pName: name of player
//                      => vScre: score
//                      => nLevl: level number
//                      => nArea: area number
//      output  <=      1: the player is in the scores table
//------------------------------------------------------------------------------
Sint32
handler_high_score::test_score (char *pName, Uint32 vScre, Uint32 nLevl,
                                Uint32 nArea)
{
  //###################################################################
  // exit if cheat!
  //###################################################################
  if (cheat_flag || birth_flag)
    return 0;

  //###################################################################
  // verify if the name exists
  //###################################################################
  score_list *score = the_scores[difficulty_level - 1];
  for (Uint32 i = 0; i < MAX_OF_HIGH_SCORES; i++)
    {
      if (chaine_cmp (pName, score[i].playerName, 6))
        {
          if (vScre > score[i].scoreValue)
            {
              score[i].scoreValue = vScre;
              score[i].scoreLevel = nLevl;
              score[i].score_area = nArea;
              return 1;
            }
          else
            return 0;
        }
    }

  for (Uint32 i = 0; i < MAX_OF_HIGH_SCORES; i++)
    {
      if (vScre > score[i].scoreValue)
        {
          i = MAX_OF_HIGH_SCORES - 1;
          score[i].scoreValue = vScre;
          score[i].scoreLevel = nLevl;
          score[i].score_area = nArea;
          char *p = score[i].playerName;
          for (Sint32 j = 0; j < 6; j++)
            p[j] = pName[j];
          if (is_verbose)
            printf ("handler_high_score::test_score() i=%i name: %s\n",
                    i, score[i].playerName);
          return 1;
        }
    }
  return 0;
}

//------------------------------------------------------------------------------
// sort scores table
//------------------------------------------------------------------------------
void
handler_high_score::sort_score (Uint32 nHard)
{
  if (is_verbose)
    printf ("handler_high_score::sort_score %i\n", nHard);
  Uint32 fExit;
  score_list *score = the_scores[nHard - 1];
  do
    {
      fExit = 0;
      for (Uint32 i = 0; i < (MAX_OF_HIGH_SCORES - 1); i++)
        {
          if (score[i].scoreValue < score[i + 1].scoreValue)
            {
              Uint32 tempo = score[i].scoreValue;
              score[i].scoreValue = score[i + 1].scoreValue;
              score[i + 1].scoreValue = tempo;

              tempo = score[i].scoreLevel;
              score[i].scoreLevel = score[i + 1].scoreLevel;
              score[i + 1].scoreLevel = tempo;

              tempo = score[i].score_area;
              score[i].score_area = score[i + 1].score_area;
              score[i + 1].score_area = tempo;

              for (Uint32 j = 0; j < 6; j++)
                {
                  tempo = score[i].playerName[j];
                  score[i].playerName[j] = score[i + 1].playerName[j];
                  score[i + 1].playerName[j] = tempo;
                }
              fExit = 1;
            }
        }
    }
  while (fExit);
}

//------------------------------------------------------------------------------
// return pointer to the scores list structure
//------------------------------------------------------------------------------
score_list *
handler_high_score::getScrList ()
{
  return the_scores[difficulty_level - 1];
}

//------------------------------------------------------------------------------
// return the player name
//------------------------------------------------------------------------------
char *
handler_high_score::bestPlayer ()
{
  score_list *score = the_scores[difficulty_level - 1];
  return &score[0].playerName[0];
}

//------------------------------------------------------------------------------
// return the best player score
//------------------------------------------------------------------------------
Uint32
handler_high_score::best_score ()
{
  score_list *score = the_scores[difficulty_level - 1];
  return score[0].scoreValue;
}
