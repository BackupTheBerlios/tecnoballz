/** 
 * @file sprite_object.cc 
 * @brief Draw sprites on the screen 
 * @date 2007-01-23
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.13 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_object.cc,v 1.13 2007/01/23 20:51:30 gurumeditation Exp $
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
#include "../include/sprite_object.h"

/**
 * Create a sprite object
 */
sprite_object::sprite_object ()
{
  ombredecax = handler_display::SHADOWOFFX * resolution;
  ombredecay = handler_display::SHADOWOFFY * resolution;
  ombrepixel = handler_display::SHADOW_PIX;
  ombrepixe4 = handler_display::SHADOWLONG;
  clear_sprite_members ();
}

/**
 * Release a sprite object
 */
sprite_object::~sprite_object ()
{
  release_sprite ();
}

//-----------------------------------------------------------------------------
// free memory used
//-----------------------------------------------------------------------------
void
sprite_object::release_sprite ()
{
  if (memoryflag)
    {
      for (Sint32 i = 0; i < max_of_images; i++)
        {
          Sint16 *memPT = BOBtableP1[i];
          memory->release ((char *) memPT);
          char *memP2 = BOBtableP2[i];
          memory->release ((char *) memP2);
          if (fTableByte)
            {
              memPT = BOBtableP3[i];
              memory->release ((char *) memPT);
            }
        }
      memory->release ((char *) BOBtableP1);
      memory->release ((char *) BOBtableP2);
      if (fTableByte)
        memory->release ((char *) BOBtableP3);
      memory->release ((char *) adresseTAB);    // Tableau des adresses dans la page BOB
      if (tafflignes)
        {
          for (Sint32 i = 0; i < max_of_images; i++)
            {
              bb_afligne *p = tafflignes[i];
              tafflignes[i] = (bb_afligne *) NULL;
              memory->release ((char *) p);
            }
          memory->release ((char *) tafflignes);
          tafflignes = (bb_afligne **) NULL;
        }
    }
  if (releaseGFX && adresseGFX)
    {
      memory->release ((char *) adresseGFX);
      adresseGFX = 0, adresseGFX = NULL;
    }
  mentatKill ();
}

//-----------------------------------------------------------------------------
// reset some values
//-----------------------------------------------------------------------------
void
sprite_object::clear_sprite_members ()
{
  mentatInit ();
  adresseGFX = (char *) NULL;
  adresseTAB = (char **) NULL;
  screen_ptr = (char *) NULL;
  adresseEC2 = (char *) NULL;
  frame_delay = 1;
  max_of_images = 0;
  frame_index = 0;
  is_enabled = 0;
  sprite_height = 0;
  sprite_width = 0;
  BOBtableP1 = (Sint16 **) NULL;
  BOBtableP2 = (char **) NULL;
  BOBtableP3 = (Sint16 **) NULL;
  collision_height = 0;
  collision_width = 0;
  screen_height = 0;
  screen_width = 0;
  frame_period = 1;
  x_coord = 0;
  y_coord = 0;
  display_pos = 0;
  x_maximum = 0;
  y_maximum = 0;
  frame_index_max = 0;
  x_minimum = 0;
  y_minimum = 0;
  frame_index_min = 0;
  offsetDest = 0;
  offsetSrce = 0;
  tabAffich1 = (Sint16 *) 0x0;
  tabAffich2 = (char *) 0x0;
  tabAffich3 = NULL;
  sprite_has_shadow = 0;
  sprite_type_id = 0;
  srceNextLn = 0;
  affligFrst = 0;
  affligLast = 1;
  mirrorVert = 0;
  tafflignes = (bb_afligne **) 0x0;
  memoryflag = 0;
  object_pos = -1;
  affRepeatF = 0;
  indexCycle = 0;
  pt_cycling = &cycling_01[0];
  thecounter = 0;
  put_method = METHOD_MSK;
  releaseGFX = 0;
}

sprite_object & sprite_object::operator= (const sprite_object & sprite)
{
  if (this == &sprite)
    {
      return *this;
    }

  printf ("sprite_object: %i = %i \n", object_pos, sprite.object_pos);

  adresseGFX = sprite.adresseGFX;
  adresseTAB = sprite.adresseTAB;
  screen_ptr = sprite.screen_ptr;
  adresseEC2 = sprite.adresseEC2;
  frame_delay = sprite.frame_delay;
  max_of_images = sprite.max_of_images;
  frame_index = sprite.frame_index;
  is_enabled = sprite.is_enabled;
  sprite_height = sprite.sprite_height;
  sprite_width = sprite.sprite_width;
  BOBtableP1 = sprite.BOBtableP1;
  BOBtableP2 = sprite.BOBtableP2;
  BOBtableP3 = sprite.BOBtableP3;
  collision_height = sprite.collision_height;
  collision_width = sprite.collision_width;
  screen_height = sprite.screen_height;
  screen_width = sprite.screen_width;
  frame_period = sprite.frame_period;
  x_coord = sprite.x_coord;
  y_coord = sprite.y_coord;
  display_pos = sprite.display_pos;
  x_maximum = sprite.x_maximum;
  y_maximum = sprite.y_maximum;
  frame_index_max = sprite.frame_index_max;
  x_minimum = sprite.x_minimum;
  y_minimum = sprite.y_minimum;
  frame_index_min = sprite.frame_index_min;
  offsetSrce = sprite.offsetSrce;
  offsetDest = sprite.offsetDest;
  tabAffich1 = sprite.tabAffich1;
  tabAffich2 = sprite.tabAffich2;
  tabAffich3 = sprite.tabAffich3;
  sprite_has_shadow = sprite.sprite_has_shadow;
  sprite_type_id = sprite.sprite_type_id;
  srceNextLn = sprite.srceNextLn;
  destNextLn = sprite.destNextLn;
  put_method = sprite.put_method;
  fTableByte = sprite.fTableByte;
  memoryflag = 0;
  return *this;
}




//-----------------------------------------------------------------------------
// copy sprite in another 
//-----------------------------------------------------------------------------
void
sprite_object::duplicaBOB (sprite_object * bobPT)
{
  bobPT->adresseGFX = adresseGFX;
  bobPT->adresseTAB = adresseTAB;
  bobPT->screen_ptr = screen_ptr;
  bobPT->adresseEC2 = adresseEC2;
  bobPT->frame_delay = frame_delay;
  bobPT->max_of_images = max_of_images;
  bobPT->frame_index = frame_index;
  bobPT->is_enabled = is_enabled;
  bobPT->sprite_height = sprite_height;
  bobPT->sprite_width = sprite_width;
  bobPT->BOBtableP1 = BOBtableP1;
  bobPT->BOBtableP2 = BOBtableP2;
  bobPT->BOBtableP3 = BOBtableP3;
  bobPT->collision_height = collision_height;
  bobPT->collision_width = collision_width;
  bobPT->screen_height = screen_height;
  bobPT->screen_width = screen_width;
  bobPT->frame_period = frame_period;
  bobPT->x_coord = x_coord;
  bobPT->y_coord = y_coord;
  bobPT->display_pos = display_pos;
  bobPT->x_maximum = x_maximum;
  bobPT->y_maximum = y_maximum;
  bobPT->frame_index_max = frame_index_max;
  bobPT->x_minimum = x_minimum;
  bobPT->y_minimum = y_minimum;
  bobPT->frame_index_min = frame_index_min;
  bobPT->offsetSrce = offsetSrce;
  bobPT->offsetDest = offsetDest;
  bobPT->tabAffich1 = tabAffich1;
  bobPT->tabAffich2 = tabAffich2;
  bobPT->tabAffich3 = tabAffich3;
  bobPT->sprite_has_shadow = sprite_has_shadow;
  bobPT->sprite_type_id = sprite_type_id;
  bobPT->srceNextLn = srceNextLn;
  bobPT->destNextLn = destNextLn;
  bobPT->put_method = put_method;
  bobPT->fTableByte = fTableByte;
  bobPT->memoryflag = 0;
}


/**
 * Enable the sprite
 */
void
sprite_object::enable ()
{
  is_enabled = 1;
}

/**
 * Disable the sprite
 */
void
sprite_object::disable ()
{
  is_enabled = 0;
}

/**
 * Check if the sprite is enable
 * @return true if the sprite is enable
 */
Sint32 sprite_object::is_enable ()
{
  return is_enabled;
}

/**
 * Set a number sprite (normally the number of position in list)
 */
void
sprite_object::set_object_pos (Sint32 num)
{
  object_pos = num;
}

void
sprite_object::set_display_pos (Sint32 num)
{
  display_pos = num;
}

/**
 * Make a simple sprite
 * @param image
 * @param shadow
 */
Sint32
sprite_object::make_sprite (GIF_bitMap * image, Sint32 shadow)
{
  initCommun (image, shadow);
  adresseGFX = image->GFXadresse ();
  return erreur_num;
}

//------------------------------------------------------------------------------
// initialize sprite
// input        => image: bitmap source
//                      => ombre: 1 = shadow
//------------------------------------------------------------------------------
void
sprite_object::initCommun (GIF_bitMap * image, Sint32 shadow)
{
  screen_width = display->get_width ();
  screen_height = display->get_height ();
  srceNextLn = image->GFX_nextLn ();
  destNextLn = display->bufferNext ();
  sprite_has_shadow = shadow;
  screen_ptr = (char *) NULL;
  adresseEC2 = (char *) NULL;
  sprite_width = image->GFXlargeur ();
  sprite_height = image->GFXhauteur ();
  max_of_images = 1;
  collision_height = sprite_height;
  collision_width = sprite_width;
  x_maximum = screen_width - (Sint32) collision_width;
  y_maximum = screen_height - (Sint32) collision_height;
  offsetSrce = image->GFX_modulo (sprite_width);
  offsetDest = display->buffer_mod (sprite_width);
}

//------------------------------------------------------------------------------
// Allocate memory
//------------------------------------------------------------------------------
Sint32
sprite_object::reservBOBt (Sint32 anima)
{
  memoryflag = 1;
  max_of_images = anima;
  // reserve la table de pointeur sur les tables d'affichage

  // tables of offsets and counters (words and bytes)
  BOBtableP1 = (Sint16 **) (memory->alloc (sizeof (Sint16 *) * max_of_images));
  error_init (memory->retour_err ());
  if (erreur_num)
    return erreur_num;

  // tables of data (pixels of the sprite)
  BOBtableP2 = (char **) (memory->alloc (sizeof (char *) * max_of_images));
  error_init (memory->retour_err ());
  if (erreur_num)
    return erreur_num;

  // tables of offsets and counters (byte peer byte)
  if (fTableByte)
    {
      BOBtableP3 =
        (Sint16 **) (memory->alloc (sizeof (Sint16 *) * max_of_images));
      error_init (memory->retour_err ());
      if (erreur_num)
        return erreur_num;
    }

  return erreur_num;
}



/**
 * Create the structure for drawing the sprite 
 * @param BOBnu shape number 1 to n
 * @praam image graphic object (a big image)
 * @param shadow true if it sprite has a shadow, false otherwise 
 * @ftpix 
 */
Sint32
sprite_object::create_sprite (Sint32 BOBnu, GIF_bitMap * image, bool shadow,
                              Sint32 ftpix)
{

  /*printf("sprite_object::initialise(BOBnu=%i, image, shadow=%i, ftpix=%i)\n",
     BOBnu, shadow, ftpix); */
  fTableByte = ftpix;

  if (put_method == METHOD_MSK)
    {
      put_method = METHOD_TAB;
    }
/*
	printf("sprite_object::initialise() BOBnu=%i, image=%x, shadow=%i\n",
		BOBnu, (Sint32)image, shadow);
*/

  // L'ecran dans lequel le BOB est affiche
  sprite_type_id = BOBnu;
  initCommun (image, shadow);
  const bb_describ *descr = zelistBOB[BOBnu];

  /* read sprite caracteristics */
  sprite_height = descr->BB_HAUTEUR;
  sprite_height *= resolution;
  max_of_images = descr->BB_ANIMATE;
  frame_index_max = max_of_images - 1;
  sprite_width = descr->BB_LARGEUR;
  sprite_width *= resolution;
  collision_height = sprite_height;
  collision_width = sprite_width;
  x_maximum = screen_width - sprite_width;
  y_maximum = screen_height - sprite_height;

  //###################################################################
  // mode lines by lines
  //###################################################################
  if (put_method == METHOD_LIN)
    {
      affligFrst = 0;
      affligLast = sprite_height;
      tafflignes = (bb_afligne **)
        (memory->alloc (sizeof (bb_afligne *) * max_of_images, 0x42424242));
      error_init (memory->retour_err ());
      if (erreur_num)
        return (erreur_num);
      for (Sint32 i = 0; i < max_of_images; i++)
        {
          bb_afligne *p = (bb_afligne *)
            (memory->alloc (sizeof (bb_afligne) * sprite_height, 0x4C4C4C4C));
          error_init (memory->retour_err ());
          if (erreur_num)
            return erreur_num;
          tafflignes[i] = p;
        }
    }

  //###################################################################
  // table giving address of each BOBs into BOBs page 
  //###################################################################
  adresseTAB = (char **)
    (memory->alloc (sizeof (char *) * max_of_images, 0x424F4250));
  error_init (memory->retour_err ());
  if (erreur_num)
    return erreur_num;
  offsetSrce = image->GFX_modulo (sprite_width);
  offsetDest = display->buffer_mod (sprite_width);

  //###################################################################
  // allocate pointers list
  //###################################################################
  error_init (reservBOBt (max_of_images));
  if (erreur_num)
    return erreur_num;

  //###################################################################
  // generate the display table
  //###################################################################
  bbPosition *coord = descr->BBPOSITION;
  for (Sint32 i = 0; i < max_of_images; i++)       //loop for each anim
    {
      Sint32 nbreP = 0;         //pixels counter
      Sint32 nbreV = 0;         //counter of number of offsets/counter
      Sint32 pos_x = (Sint32) coord[i].BB_COORDX;
      pos_x *= resolution;
      pos_x *= 16;
      Sint32 pos_y = (Sint32) coord[i].BB_COORDY;
      pos_y *= resolution;

      //###############################################################
      //verify page overflow 
      //###############################################################
      if (pos_y > (image->GFXhauteur () - sprite_height) ||
          pos_x > (image->GFXlargeur () - sprite_width))
        {
          fprintf (stderr,
                   "sprite_object::initialise() BOBnu=%i x2=%i>GFXlargeur=%i "
                   "*AND/OR* y2=%i>GFXhauteur=%i\n",
                   (Sint32) BOBnu, (Sint32) (pos_x + sprite_width),
                   (Sint32) image->GFXlargeur (),
                   (Sint32) (pos_y + sprite_height),
                   (Sint32) image->GFXhauteur ());
          fprintf (stderr,
                   "- pos_x: %i, pos_y:%i, max_of_images:%i " "resolution=%i\n",
                   coord[i].BB_COORDX, coord[i].BB_COORDY, i, resolution);
          return E_GENRIQUE;
        }

      //###############################################################
      // mirror y if request
      //###############################################################
      if (mirrorVert)
        {
          char *gfxHa = image->GFXadresse (pos_x, pos_y);
          char *gfxBa = image->GFXadresse (pos_x, pos_y + sprite_height - 1);
          Sint32 iOffs = image->GFX_nextLn ();
          for (Sint32 j = 0; j < sprite_height / 2; j++)
            {
              for (Sint32 k = 0; k < sprite_width; k++)
                {
                  char c = gfxHa[k];
                  gfxHa[k] = gfxBa[k];
                  gfxBa[k] = c;
                }
              gfxHa += iOffs;
              gfxBa -= iOffs;
            }
        }

      //###################################################################
      //calculation size of the table
      //###################################################################
      Sint32 npixe = 0;         //counter of the pixels
      char *gfxPT = image->GFXadresse (pos_x, pos_y);   //graphic address 
      for (Sint32 j = 0; j < sprite_height; j++)
        {
          for (Sint32 k = 0; k < sprite_width; k++)
            {
              char pixel = *(gfxPT++);  //read the pixel
              if (pixel)        //transparent?
                {
                  npixe++;
                  nbreP++;      //incremente the size of pixel's table 
                }
              else
                {
                  if (npixe > 0)        //at least one pixel ?
                    {
                      npixe = 0;
                      nbreV++;  //incremente the table size
                    }
                }
            }
          if (npixe > 0)
            {
              npixe = 0;
              nbreV++;
            }
          gfxPT += offsetSrce;
        }

      //###################################################################
      // genere the sprite's table for display
      //###################################################################
      char *destP = (char *) memory->alloc
        ((nbreP) * sizeof (char), 0x40424F42);
      error_init (memory->retour_err ());
      if (erreur_num)
        return erreur_num;
      Sint16 *destV = (Sint16 *) memory->alloc
        ((nbreV * 3 + 1) * sizeof (Sint16), 0x40424F42);
      error_init (memory->retour_err ());
      if (erreur_num)
        return erreur_num;
      Sint16 *destW = NULL;
      if (fTableByte)
        {
          destW = (Sint16 *) memory->alloc
            ((nbreV * 2 + 1) * sizeof (Sint16), 0x40424F42);
          error_init (memory->retour_err ());
          if (erreur_num)
            return erreur_num;
        }
      BOBtableP1[i] = destV;    //table of offsets and loops counters
      BOBtableP2[i] = destP;    //table of pixels
      if (fTableByte)
        BOBtableP3[i] = destW;

      // genere the sprite's table for display ...................................
      Sint32 depla = 0;         // offset
      npixe = 0;                // counter of the pixels
      gfxPT = image->GFXadresse (pos_x, pos_y); // graphic address 
      adresseTAB[i] = gfxPT;
      *(destV++) = (Sint16) nbreV;      // Nombre d'occurences
      if (fTableByte)
        *(destW++) = (Sint16) nbreV;    // Nombre d'occurences
      nbreV = 0;                // compteur nombre d'offest et de compteur
      Sint32 nbreO = 0;
      Sint32 flagO = 0;
      for (Sint32 j = 0; j < sprite_height; j++)
        {
          // special display mode line peer line (gigablitz)
          if (put_method == METHOD_LIN)
            {
              bb_afligne *p = tafflignes[i];
              p[j].TABAFFICH1 = destV;
              p[j].TABAFFICH2 = destP;
            }

          flagO = 0;
          for (Sint32 k = 0; k < sprite_width; k++)
            {
              char pixel = *(gfxPT++);  //read the pixel
              if (pixel)        //transparent?
                {
                  *(destP++) = (Sint16) pixel;  //no, save pixel value
                  npixe++;
                }
              else
                {
                  if (npixe > 0)        //at least one pixel ?
                    {
                      if (fTableByte)
                        {
                          *(destW++) = (Sint16) depla;  //previous offset
                          *(destW++) = (Sint16) npixe;  //number of pixel(s)    
                        }
                      *(destV++) = (Sint16) depla;      //save the previous offset
#ifndef BYTES_COPY
                      Sint32 n = npixe >> 2;    //number of word 32 bits
                      *(destV++) = (Sint16) n;  //number of 4x pixel(s)
                      npixe &= 0x003;   //rest 0, 1, 2 or 3 bytes
#else
                      *(destV++) = 0;
#endif
                      *(destV++) = (Sint16) npixe;      //number of pixel(s)
                      npixe = 0;
                      depla = 0;
                      nbreV++;  //COUNTERTAB++
                      flagO = 1;
                    }
                  if (flagO == 0)
                    nbreO++;
                  depla++;
                }
            }                   //width loop
          //***
          if (npixe > 0)
            {
              if (fTableByte)
                {
                  *(destW++) = (Sint16) depla;  //previous offset
                  *(destW++) = (Sint16) npixe;  //number of pixel(s)    
                }
              *(destV++) = (Sint16) depla;
#ifndef BYTES_COPY
              Sint32 n = npixe >> 2;    //number of word 32 bits
              *(destV++) = (Sint16) n;  //number of 4x pixel(s)
              npixe &= 0x003;   //rest 0, 1, 2 or 3 bytes
#else
              *(destV++) = 0;
#endif
              *(destV++) = (Sint16) npixe;      //number of pixel(s)
              npixe = 0;
              nbreV++;          //COUNTERTAB++
              depla = 0;
            }
          gfxPT += offsetSrce;
          depla += offsetDest;

          if (put_method == METHOD_LIN)
            {
              bb_afligne *p = tafflignes[i];
              p[j].COUNTERTAB = nbreV;
              p[j].OFFSETLEFT = nbreO;
              nbreV = 0;
              nbreO = 0;
            }
        }                       //height loop
    }
  tabAffich1 = BOBtableP1[0];   //table of offsets and loops counters
  tabAffich2 = BOBtableP2[0];   //adresse table pour "afficheBOB"
  if (fTableByte)
    tabAffich3 = BOBtableP3[0];
  adresseGFX = adresseTAB[0];   //adresse GFX pour routine "draw()"
  //printf("sprite_object::initialise()\n");
  return (erreur_num);
}

/**
 * Set x and y coordinates of the sprite
 * @param xcoord the x coordinate in pixels
 * @param ycoord the y coordinate in pixels
 */
void
sprite_object::set_coordinates (Sint32 xcoord, Sint32 ycoord)
{
  x_coord = xcoord;
  y_coord = ycoord;
}

/**
 * Set x coordinate of the sprite
 * @param xcoord the x coordinate in pixels
 */
void
sprite_object::set_x_coord (Sint32 xcoord)
{
  x_coord = xcoord;
}

/**
 * Set y coordinate of the sprite
 * @param ycoord the y coordinate in pixels
 */
void
sprite_object::set_y_coord (Sint32 ycoord)
{
  y_coord = ycoord;
}

/**
 * Moving the sprite horizontally 
 * @param xoffset the horizontal offset of displacement
 */
void
sprite_object::move_x (Sint32 xoffset)
{
  x_coord += xoffset;
}

/**
 * Moving the sprite vertically 
 * @param yoffset the vertical offset of displacement
 */
void
sprite_object::move_y (Sint32 yoffset)
{
  y_coord += yoffset;
}

/**
 * Check if the sprite has a shadow
 * @return true if the sprite has a shadow
 */
bool
sprite_object::has_shadow ()
{
  return sprite_has_shadow;
}

/**
 * Get x coordinate
 * @return x coordinate of the sprite
 */
Sint32
sprite_object::get_x_coord ()
{
  return x_coord;
}

/**
 * Get y coordinate
 * @return y coordinate of the sprite
 */
Sint32
sprite_object::get_y_coord ()
{
  return y_coord;
}

/**
 * Set the image to use for this sprite
 */
void
sprite_object::set_image ()
{
  Sint32 index = frame_index;
  adresseGFX = adresseTAB[index];
  tabAffich1 = BOBtableP1[index];
  tabAffich2 = BOBtableP2[index];
  if (fTableByte && BOBtableP3)
    {
      tabAffich3 = BOBtableP3[index]; 
    }
}

/**
 * Set the image to use for this sprite
 * @param index frame index 
 */
void
sprite_object::set_image (Sint32 index)
{
  frame_index = index;
  adresseGFX = adresseTAB[index];
  tabAffich1 = BOBtableP1[index];
  tabAffich2 = BOBtableP2[index];
  if (fTableByte)
    {
      tabAffich3 = BOBtableP3[index];
    }
}

/**
 * Restore background where sprite was displayed
 */
void
sprite_object::restore_background_under_sprite ()
{
  /* if memory address is null, then sprite is not displaying */
  if (NULL == screen_ptr)
    {
      return;
    }

  /* special sprite, restore line by line (gigablitz) */
  if (put_method == METHOD_LIN)
    {
      efface_lin ();
      return;
    }

#ifndef BYTES_COPY
  Sint32 *srcPT = (Sint32 *) restore_ptr;
  Sint32 *adres = (Sint32 *) screen_ptr;
  screen_ptr = (char *) NULL;
  Sint16 *gfxPT = tabAffich1;
  Uint32 t = (Uint32) * (gfxPT++);
  for (Uint32 i = 0; i < t; i++)
    {
      Sint16 o = *(gfxPT++);    //offset
      adres = (Sint32 *) ((char *) adres + o);
      srcPT = (Sint32 *) ((char *) srcPT + o);
      o = *(gfxPT++);           //number of bytes contigus
      for (Sint32 k = 0; k < o; k++)
        {
          *(adres++) = *(srcPT++);
        }
      o = *(gfxPT++);           //number of longword contigus
      char *adreb = (char *) adres;
      char *srcPB = (char *) srcPT;
      for (Sint32 k = 0; k < o; k++)
        {
          *(adreb++) = *(srcPB++);
        }
      adres = (Sint32 *) adreb;
      srcPT = (Sint32 *) srcPB;
    }
#else
  char *srcPT = restore_ptr;
  char *adres = screen_ptr;
  screen_ptr = (char *) NULL;
  Sint16 *gfxPT = tabAffich1;
  Uint32 t = (Uint32) * (gfxPT++);
  for (Uint32 i = 0; i < t; i++)
    {
      Sint16 o = *(gfxPT++);    //offset
      adres += o;
      srcPT += o;
      gfxPT++;
      o = *(gfxPT++);           //number of bytes contigus
      for (Sint32 k = 0; k < o; k++)
        {
          *(adres++) = *(srcPT++);
        }
    }
#endif
}

// -----------------------------------------------------------------------------
// efface le BOB : restaure le decor 
// -----------------------------------------------------------------------------
void
sprite_object::efface_lin ()
{

  bb_afligne *p = tafflignes[frame_index];
  Sint32 l = affligFrSv;
  Sint16 o = p[l].OFFSETLEFT;
  Uint32 t = (Uint32) p[l].COUNTERTAB;
  Uint16 *gfxPT = (Uint16 *) p[l].TABAFFICH1;
  gfxPT++;
#ifndef BYTES_COPY
  Sint32 *srcPT = (Sint32 *) restore_ptr;
  Sint32 *adres = (Sint32 *) screen_ptr;
  screen_ptr = (char *) NULL;
  for (Uint32 i = 0; i < t; i++)
    {
      adres = (Sint32 *) ((char *) adres + o);
      srcPT = (Sint32 *) ((char *) srcPT + o);
      o = *(gfxPT++);           //number of pixels contigus
      for (Sint32 k = 0; k < o; k++)
        *(adres++) = *(srcPT++);
      o = *(gfxPT++);           //number of pixels contigus
      char *adreb = (char *) adres;
      char *srcPB = (char *) srcPT;
      for (Sint32 k = 0; k < o; k++)
        *(adreb++) = *(srcPB++);
      adres = (Sint32 *) adreb;
      srcPT = (Sint32 *) srcPB;
      o = *(gfxPT++);           //offset
    }
  l++;
  for (; l < affligLaSv; l++)
    {
      gfxPT = (Uint16 *) p[l].TABAFFICH1;
      t = (Uint32) p[l].COUNTERTAB;
      for (Uint32 i = 0; i < t; i++)
        {
          o = *(gfxPT++);       //offset
          adres = (Sint32 *) ((char *) adres + o);
          srcPT = (Sint32 *) ((char *) srcPT + o);
          o = *(gfxPT++);       //number of pixels contigus
          for (Sint32 k = 0; k < o; k++)
            *(adres++) = *(srcPT++);
          o = *(gfxPT++);       //number of pixels contigus
          char *adreb = (char *) adres;
          char *srcPB = (char *) srcPT;
          for (Sint32 k = 0; k < o; k++)
            *(adreb++) = *(srcPB++);
          adres = (Sint32 *) adreb;
          srcPT = (Sint32 *) srcPB;
        }
    }
#else
  char *srcPT = restore_ptr;
  char *adres = screen_ptr;
  screen_ptr = (char *) NULL;
  for (Uint32 i = 0; i < t; i++)
    {
      adres = adres + o;
      srcPT = srcPT + o;
      gfxPT++;
      o = *(gfxPT++);           //number of pixels contigus
      for (Sint32 k = 0; k < o; k++)
        *(adres++) = *(srcPT++);
      o = *(gfxPT++);           //offset
    }
  l++;
  for (; l < affligLaSv; l++)
    {
      gfxPT = (Uint16 *) p[l].TABAFFICH1;
      t = (Uint32) p[l].COUNTERTAB;
      for (Uint32 i = 0; i < t; i++)
        {
          o = *(gfxPT++);       //offset
          adres = adres + o;
          srcPT = srcPT + o;
          gfxPT++;
          o = *(gfxPT++);       //number of pixels contigus
          for (Sint32 k = 0; k < o; k++)
            *(adres++) = *(srcPT++);
        }
    }
#endif
}

//-------------------------------------------------------------------------------
// get current animation offset
//-------------------------------------------------------------------------------
Sint32
sprite_object::litAnimOff ()
{
  return frame_index;
}

/**
 * Restore background where sprite's shadow was displayed
 */
void
sprite_object::restore_background_under_shadow ()
{
  if (NULL == adresseEC2)
    {
      return;
    }
#ifndef BYTES_COPY
      Sint32 *srcPT = (Sint32 *) adresseTA2;
      Sint32 *adres = (Sint32 *) adresseEC2;
      adresseEC2 = (char *) NULL;
      Sint16 *gfxPT = tabAffich1;
      Uint32 t = (Uint32) * (gfxPT++);
      for (Uint32 i = 0; i < t; i++)
        {
          Sint16 o = *(gfxPT++);        //offset
          adres = (Sint32 *) ((char *) adres + o);
          srcPT = (Sint32 *) ((char *) srcPT + o);
          o = *(gfxPT++);       //number of pixels contigus
          for (Sint32 k = 0; k < o; k++)
            *(adres++) = *(srcPT++);
          o = *(gfxPT++);       //number of pixels contigus
          char *adreb = (char *) adres;
          char *srcPB = (char *) srcPT;
          for (Sint32 k = 0; k < o; k++)
            *(adreb++) = *(srcPB++);
          adres = (Sint32 *) adreb;
          srcPT = (Sint32 *) srcPB;
        }
#else
      char *srcPT = adresseTA2;
      char *adres = adresseEC2;
      adresseEC2 = (char *) NULL;
      Sint16 *gfxPT = tabAffich1;
      Uint32 t = (Uint32) * (gfxPT++);
      for (Uint32 i = 0; i < t; i++)
        {
          Sint16 o = *(gfxPT++);        //offset
          adres += o;
          srcPT += o;
          gfxPT++;
          o = *(gfxPT++);       //number of pixels contigus
          for (Sint32 k = 0; k < o; k++)
            *(adres++) = *(srcPT++);
        }
#endif
}

/**
 * Draw a sprite into the offscreen
 */
void
sprite_object::draw ()
{
  if (!is_enabled || frame_index >= max_of_images)
    {
      return;
    }
  switch (put_method)
    {
    case METHOD_TAB:
      method_tab ();
      break;
    case METHOD_MSK:
      draw_copy ();
      break;
    case METHOD_LIN:
      afficheLin ();
      break;
    case METHOD_REP:
      afficheRep ();
      break;
    case METHOD_CC1:
      afficheCyc ();
      break;
    case CYCLE_PTAB:
      cycle_ptab ();
      break;
    }
}

//------------------------------------------------------------------------------
// display a sprite (most current metod)
//------------------------------------------------------------------------------
void
sprite_object::method_tab ()
{
  restore_ptr = display->tampon_pos (x_coord, y_coord);
#ifndef BYTES_COPY
  Sint32 *adres = (Sint32 *) display->buffer_pos (x_coord, y_coord);
  screen_ptr = (char *) adres;
  Sint32 *gfxP2 = (Sint32 *) tabAffich2;        //pixels
  Uint16 *gfxP1 = (Uint16 *) tabAffich1;        //offset and loop counter
  Uint32 t = (Uint32) * (gfxP1++);      //height of sprite
  for (Uint32 i = 0; i < t; i++)
    {
      Sint16 o = *(gfxP1++);    //offset
      adres = (Sint32 *) ((char *) adres + o);
      o = *(gfxP1++);           //number of longword contigus
      for (Sint32 k = 0; k < o; k++)
        {
          Sint32 j = *(gfxP2++);
          *(adres++) = j;
        }
      o = *(gfxP1++);           //number of bytes contigus
      char *gfxpb = (char *) gfxP2;
      char *adreb = (char *) adres;
      for (Sint32 k = 0; k < o; k++)
        {
          char j = *(gfxpb++);
          *(adreb++) = j;
        }
      gfxP2 = (Sint32 *) gfxpb;
      adres = (Sint32 *) adreb;
    }
#else
  char *adres = display->buffer_pos (x_coord, y_coord);
  screen_ptr = adres;
  char *gfxP2 = tabAffich2;     //pixels (sprite data)
  Uint16 *gfxP1 = (Uint16 *) tabAffich1;        //offset and loop counter
  Uint32 t = (Uint32) * (gfxP1++);      //height of sprite
  for (Uint32 i = 0; i < t; i++)
    {
      Sint16 o = *(gfxP1++);    //offset
      adres += o;
      gfxP1++;
      o = *(gfxP1++);           //number of pixels contigus
      for (Sint32 k = 0; k < o; k++)
        *(adres++) = *(gfxP2++);
    }
#endif
}

//------------------------------------------------------------------------------
// display a color cycling sprite mask (bumper's fire and guards's fire)
//------------------------------------------------------------------------------
void
sprite_object::afficheCyc ()
{
  indexCycle &= 7;
  Sint32 pixel = pt_cycling[indexCycle++];
  restore_ptr = display->tampon_pos (x_coord, y_coord);
#ifndef BYTES_COPY
  Sint32 *adres = (Sint32 *) display->buffer_pos (x_coord, y_coord);
  screen_ptr = (char *) adres;
  Sint32 *gfxP2 = (Sint32 *) tabAffich2;        //pixels
  Uint16 *gfxP1 = (Uint16 *) tabAffich1;        //offset and loop counter
  Uint32 t = (Uint32) * (gfxP1++);
  for (Uint32 i = 0; i < t; i++)
    {
      Sint16 o = *(gfxP1++);    //offset
      adres = (Sint32 *) ((char *) adres + o);
      o = *(gfxP1++);           //number of pixels contigus
      for (Sint32 k = 0; k < o; k++)
        *(adres++) = pixel;
      o = *(gfxP1++);           //number of pixels contigus
      char *gfxpb = (char *) gfxP2;
      char *adreb = (char *) adres;
      for (Sint32 k = 0; k < o; k++)
        *(adreb++) = pixel;
      gfxP2 = (Sint32 *) gfxpb;
      adres = (Sint32 *) adreb;
    }
#else
  char *adres = display->buffer_pos (x_coord, y_coord);
  screen_ptr = adres;
  Uint16 *gfxP1 = (Uint16 *) tabAffich1;        //offset and loop counter
  Uint32 t = (Uint32) * (gfxP1++);
  for (Uint32 i = 0; i < t; i++)
    {
      Sint16 o = *(gfxP1++);    //offset
      adres += o;
      gfxP1++;
      o = *(gfxP1++);           //number of pixels contigus
      for (Sint32 k = 0; k < o; k++)
        *(adres++) = pixel;
    }
#endif
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void
sprite_object::afficheCC2 ()
{
}

//------------------------------------------------------------------------------
// guards weapons: display mask pixel by pixel with color cylcling
//------------------------------------------------------------------------------
void
sprite_object::cycle_ptab ()
{
  indexCycle &= 7;
  Sint32 pixel = pt_cycling[indexCycle++];
  char *adres = display->buffer_pos (x_coord, y_coord);
  restore_ptr = display->tampon_pos (x_coord, y_coord);
  screen_ptr = (char *) adres;
  char *gfxP2 = tabAffich2;     //pixels
  Uint16 *gfxP3 = (Uint16 *) tabAffich3;        //offset and loop counter
  Uint32 t = (Uint32) * (gfxP3++);
  for (Uint32 i = 0; i < t; i++)
    {
      Sint16 o = *(gfxP3++);    //offset
      adres = adres + o;
      o = *(gfxP3++);           //number of pixels contigus
      for (Sint32 k = 0; k < o; k++)
        {
          char p = *(gfxP2++);
          if (p == 29)
            *(adres++) = pixel;
          else
            *(adres++) = p;
        }
    }
}

//------------------------------------------------------------------------------
//  sprite image will be repeated vertically  (guards's life level) 
//------------------------------------------------------------------------------
void
sprite_object::afficheRep ()
{
  restore_ptr = display->tampon_pos (x_coord, y_coord);
  screen_ptr = display->buffer_pos (x_coord, y_coord);
  Sint32 offsy = 0;
  for (Sint32 r = 0; r < affRepeatF; r++, offsy += sprite_height)
    {
      Uint16 *gfxP1 = (Uint16 *) tabAffich1;    //offsets and loop counter
      Uint32 t = (Uint32) * (gfxP1++);
#ifndef BYTES_COPY
      Sint32 *adres =
        (Sint32 *) display->buffer_pos (x_coord, y_coord + offsy);
      Sint32 *gfxP2 = (Sint32 *) tabAffich2;    //pixels data of the sprite
      for (Uint32 i = 0; i < t; i++)
        {
          Sint16 o = *(gfxP1++);        //offset in bytes
          adres = (Sint32 *) ((char *) adres + o);
          o = *(gfxP1++);       //number of longword contigus
          for (Sint32 k = 0; k < o; k++)
            {
              Sint32 j = *(gfxP2++);
              *(adres++) = j;
            }
          o = *(gfxP1++);       //number of bytes contigus
          char *gfxpb = (char *) gfxP2;
          char *adreb = (char *) adres;
          for (Sint32 k = 0; k < o; k++)
            {
              char j = *(gfxpb++);
              *(adreb++) = j;
            }
          gfxP2 = (Sint32 *) gfxpb;
          adres = (Sint32 *) adreb;
        }
#else
      char *adres = display->buffer_pos (x_coord, y_coord + offsy);
      char *gfxP2 = tabAffich2; //pixels data of the sprite
      for (Uint32 i = 0; i < t; i++)
        {
          Sint16 o = *(gfxP1++);        //offset in bytes
          adres += o;
          gfxP1++;
          o = *(gfxP1++);       //number of pixels contigus
          for (Sint32 k = 0; k < o; k++)
            {
              char j = *(gfxP2++);
              *(adres++) = j;
            }
        }
#endif
    }
}

//------------------------------------------------------------------------------
// display a sprite line by line (for the gigablitz vertical clipping)
//------------------------------------------------------------------------------
void
sprite_object::afficheLin ()
{
  bb_afligne *p = tafflignes[frame_index];
  restore_ptr = display->tampon_pos (x_coord, y_coord + affligFrst);
  affligFrSv = affligFrst;;
  affligLaSv = affligLast;
  Sint32 l = affligFrst;
  Uint32 t = (Uint32) p[l].COUNTERTAB;
  Uint16 *gfxP1 = (Uint16 *) p[l].TABAFFICH1;
  Sint16 o = p[l].OFFSETLEFT;
  gfxP1++;
#ifndef BYTES_COPY
  Sint32 *adres =
    (Sint32 *) display->buffer_pos (x_coord, y_coord + affligFrst);
  Sint32 *gfxP2 = (Sint32 *) p[l].TABAFFICH2;
  screen_ptr = (char *) adres;
  for (Uint32 i = 0; i < t; i++)
    {
      adres = (Sint32 *) ((char *) adres + o);
      o = *(gfxP1++);           //number of pixels contigus
      for (Sint32 k = 0; k < o; k++)
        {
          Sint32 j = *(gfxP2++);
          *(adres++) = j;
        }
      o = *(gfxP1++);           //number of pixels contigus
      char *gfxpb = (char *) gfxP2;
      char *adreb = (char *) adres;
      for (Sint32 k = 0; k < o; k++)
        {
          char j = *(gfxpb++);
          *(adreb++) = j;
        }
      gfxP2 = (Sint32 *) gfxpb;
      adres = (Sint32 *) adreb;
      o = *(gfxP1++);           //offset
    }
  l++;
  for (; l < affligLast; l++)
    {
      t = (Uint32) p[l].COUNTERTAB;
      gfxP1 = (Uint16 *) p[l].TABAFFICH1;
      gfxP2 = (Sint32 *) p[l].TABAFFICH2;
      for (Uint32 i = 0; i < t; i++)
        {
          o = *(gfxP1++);
          adres = (Sint32 *) ((char *) adres + o);
          o = *(gfxP1++);       //number of pixels contigus
          for (Sint32 k = 0; k < o; k++)
            {
              Sint32 j = *(gfxP2++);
              *(adres++) = j;
            }
          o = *(gfxP1++);       //number of pixels contigus
          char *gfxpb = (char *) gfxP2;
          char *adreb = (char *) adres;
          for (Sint32 k = 0; k < o; k++)
            {
              char j = *(gfxpb++);
              *(adreb++) = j;
            }
          gfxP2 = (Sint32 *) gfxpb;
          adres = (Sint32 *) adreb;
        }
    }
#else
  char *adres = display->buffer_pos (x_coord, y_coord + affligFrst);
  char *gfxP2 = p[l].TABAFFICH2;
  screen_ptr = adres;
  for (Uint32 i = 0; i < t; i++)
    {
      adres += o;
      gfxP1++;
      o = *(gfxP1++);           //number of pixels contigus
      for (Sint32 k = 0; k < o; k++)
        {
          char j = *(gfxP2++);
          *(adres++) = j;
        }
      o = *(gfxP1++);           //offset
    }
  l++;
  for (; l < affligLast; l++)
    {
      t = (Uint32) p[l].COUNTERTAB;
      gfxP1 = (Uint16 *) p[l].TABAFFICH1;
      gfxP2 = p[l].TABAFFICH2;
      for (Uint32 i = 0; i < t; i++)
        {
          o = *(gfxP1++);
          adres += o;
          gfxP1++;
          o = *(gfxP1++);       //number of pixels contigus
          for (Sint32 k = 0; k < o; k++)
            {
              char j = *(gfxP2++);
              *(adres++) = j;
            }
        }
    }
#endif
}

//------------------------------------------------------------------------------
// The second main method : display a shadow into the "buffer"
//------------------------------------------------------------------------------
void
sprite_object::afficheSHA ()
{
  if (!is_enabled || !sprite_has_shadow)
    return;
  char j = ombrepixel;
  adresseTA2 =
    display->tampon_pos (x_coord + ombredecax, y_coord + ombredecay);
  Uint16 *gfxPT = (Uint16 *) tabAffich1;
  Uint32 t = (Uint32) * (gfxPT++);
#ifndef BYTES_COPY
  Sint32 q = ombrepixe4;
  Sint32 *adres =
    (Sint32 *) display->buffer_pos (x_coord + ombredecax,
                                    y_coord + ombredecay);
  adresseEC2 = (char *) adres;
  for (Uint32 i = 0; i < t; i++)
    {
      Sint16 o = *(gfxPT++);    //offset
      adres = (Sint32 *) ((char *) adres + o);
      o = *(gfxPT++);           //number of longworld contigus
      for (Sint32 k = 0; k < o; k++)
        *(adres++) |= q;
      o = *(gfxPT++);           //number of byte contigus
      char *adreb = (char *) adres;
      for (Sint32 k = 0; k < o; k++)
        *(adreb++) |= j;
      adres = (Sint32 *) adreb;
    }
#else
  char *adres =
    display->buffer_pos (x_coord + ombredecax, y_coord + ombredecay);
  adresseEC2 = adres;
  for (Uint32 i = 0; i < t; i++)
    {
      Sint16 o = *(gfxPT++);    //offset
      adres += o;
      gfxPT++;
      o = *(gfxPT++);           //number of pixels contigus
      for (Sint32 k = 0; k < o; k++)
        *(adres++) |= j;
    }
#endif
}

//------------------------------------------------------------------------------
// display a sprite into the "tampon" (build the background)
//------------------------------------------------------------------------------
void
sprite_object::affich_MSK ()
{
  restore_ptr = display->tampon_pos (x_coord, y_coord);
  Uint16 *gfxP1 = (Uint16 *) tabAffich1;        //offset and loop counter
  Uint32 t = (Uint32) * (gfxP1++);
#ifndef BYTES_COPY
  Sint32 *adres = (Sint32 *) display->tampon_pos (x_coord, y_coord);
  screen_ptr = (char *) adres;
  Sint32 *gfxP2 = (Sint32 *) tabAffich2;        //pixels
  for (Uint32 i = 0; i < t; i++)
    {
      Sint16 o = *(gfxP1++);    //offset
      adres = (Sint32 *) ((char *) adres + o);
      o = *(gfxP1++);           //number of longwords contigus
      for (Sint32 k = 0; k < o; k++)
        {
          Sint32 j = *(gfxP2++);
          *(adres++) = j;
        }
      o = *(gfxP1++);           //number of bytes contigus
      char *gfxpb = (char *) gfxP2;
      char *adreb = (char *) adres;
      for (Sint32 k = 0; k < o; k++)
        {
          char j = *(gfxpb++);
          *(adreb++) = j;
        }
      gfxP2 = (Sint32 *) gfxpb;
      adres = (Sint32 *) adreb;
    }
#else
  char *adres = display->tampon_pos (x_coord, y_coord);
  screen_ptr = adres;
  char *gfxP2 = tabAffich2;     //pixels data
  for (Uint32 i = 0; i < t; i++)
    {
      Sint16 o = *(gfxP1++);    //offset
      adres += o;
      gfxP1++;
      o = *(gfxP1++);           //number of pixels contigus
      for (Sint32 k = 0; k < o; k++)
        {
          char j = *(gfxP2++);
          *(adres++) = j;
        }
    }
#endif
}

//------------------------------------------------------------------------------
// display a shadow into the "tampon" (build the background)
//------------------------------------------------------------------------------
void
sprite_object::affich_SHA ()
{
  adresseTA2 = display->tampon_pos (x_coord + ombredecax,
                                    y_coord + ombredecay);
  char j = ombrepixel;
  Uint16 *gfxPT = (Uint16 *) tabAffich1;
  Uint32 t = (Uint32) * (gfxPT++);
#ifndef BYTES_COPY
  Sint32 *adres = (Sint32 *) display->tampon_pos (x_coord + ombredecax,
                                                  y_coord + ombredecay);
  adresseEC2 = (char *) adres;
  Sint32 q = ombrepixe4;
  for (Uint32 i = 0; i < t; i++)
    {
      Sint16 o = *(gfxPT++);    //offset
      adres = (Sint32 *) ((char *) adres + o);
      o = *(gfxPT++);           //number of longwords contigus
      for (Sint32 k = 0; k < o; k++)
        *(adres++) |= q;
      o = *(gfxPT++);           //number of bytes contigus
      char *adreb = (char *) adres;
      for (Sint32 k = 0; k < o; k++)
        *(adreb++) |= j;
      adres = (Sint32 *) adreb;
    }
#else
  char *adres = display->tampon_pos (x_coord + ombredecax,
                                     y_coord + ombredecay);
  adresseEC2 = adres;
  for (Uint32 i = 0; i < t; i++)
    {
      Sint16 o = *(gfxPT++);    //offset
      adres += o;
      gfxPT++;
      o = *(gfxPT++);           //number of pixels contigus
      for (Sint32 k = 0; k < o; k++)
        *(adres++) |= j;
    }
#endif
}

/**
 * Draw a sprite into the game offscreen (copy byte to byte, no table)
 */
void
sprite_object::draw_copy ()
{
  if (!is_enabled)
    {
      return;
    }
  char *s = adresseGFX;
  char *d = display->buffer_pos (x_coord, y_coord);
  restore_ptr = display->tampon_pos (x_coord, y_coord);
  screen_ptr = d;
  Sint32 m = srceNextLn;
  Sint32 n = destNextLn;
  Sint32 h = sprite_height;
  Sint32 l = sprite_width;
  for (Sint32 i = 0; i < h; i++)
    {
      for (Sint32 j = 0; j < l; j++)
        {
          /* read the pixel */
          char p = s[j];
          /* black color? */
          if (p != 0)
            {
              /* no, put the pixel */
              d[j] = p;
            }
        }
      s += m;
      d += n;
    }
}

//------------------------------------------------------------------------------
// display a sprite into the "buffer" (copy byte to byte, no table)
//------------------------------------------------------------------------------
void
sprite_object::MSKbitcopy ()
{
  if (!is_enabled)
    return;
  char *s = adresseGFX;
  char *d = display->buffer_pos (x_coord, y_coord);
  restore_ptr = display->tampon_pos (x_coord, y_coord);
  screen_ptr = d;
  Sint32 m = srceNextLn;
  Sint32 n = destNextLn;
  Sint32 h = sprite_height;
  Sint32 l = sprite_width;
  for (Sint32 i = 0; i < h; i++)
    {
      for (Sint32 j = 0; j < l; j++)
        d[j] = s[j];            //copy the pixel
      s += m;
      d += n;
    }
}

//------------------------------------------------------------------------------
// clear a sprite into the "buffer" (copy byte to byte, no table)
//------------------------------------------------------------------------------
void
sprite_object::MSK_bitclr ()
{
  if (NULL == screen_ptr)
    {
      return;
    }
  char *s = restore_ptr;
  char *d = screen_ptr;
  screen_ptr = (char *) NULL;
  Sint32 m = destNextLn;
  Sint32 n = destNextLn;
  Sint32 h = sprite_height;
  Sint32 l = sprite_width;
  for (Sint32 i = 0; i < h; i++)
    {
      for (Sint32 j = 0; j < l; j++)
        d[j] = s[j];            //restore the pixel
      s += m;
      d += n;
    }
}

//-------------------------------------------------------------------------------
// position sprite on the other one
//-------------------------------------------------------------------------------
void
sprite_object::aspire_BOB (sprite_object * bobPT, Sint32 offsX, Sint32 offsY)
{
  x_coord = (bobPT->x_coord) + offsX - (collision_width >> 1);
  y_coord = (bobPT->y_coord) + offsY - (collision_height >> 1);
}

//-------------------------------------------------------------------------------
// position sprite on the other one
//-------------------------------------------------------------------------------
void
sprite_object::aspireBOB2 (sprite_object * bobPT, Sint32 offsX, Sint32 offsY)
{
  x_coord =
    (bobPT->x_coord) + offsX - ((collision_width - bobPT->collision_width) >> 1);
  y_coord =
    (bobPT->y_coord) + offsY - ((collision_height - bobPT->collision_height) >> 1);
}


//-------------------------------------------------------------------------------
// Test de collision entre deux BOBs
//-------------------------------------------------------------------------------
Sint32
sprite_object::collision1 (sprite_object * bobPT)
{
  Sint32 x1 = x_coord;
  Sint32 y1 = y_coord;
  Sint32 x2 = bobPT->x_coord;
  Sint32 y2 = bobPT->y_coord;
  return (x2 + bobPT->collision_width > x1 &&
          x2 - collision_width < x1 &&
          y2 + bobPT->collision_height > y1 && y2 - collision_height < y1);
}

//-------------------------------------------------------------------------------
// Initialise la vitesse d'animation
//-------------------------------------------------------------------------------
void
sprite_object::tempo_init (Sint32 tempo)
{
  frame_delay = tempo;
  frame_period = tempo;
}

/**
 * Set the frame period, the speed of animation
 * @param period time delay before next image
 */
void
sprite_object::set_frame_period (Sint32 period)
{
  frame_period = period;
}

/**
 * The animation is played once 
 * @return true if animation is finished
 */
bool
sprite_object::play_animation_once ()
{
  if (--frame_delay > 0)
    {
      return is_enabled;
    }
  frame_delay = frame_period;
  if (frame_index == frame_index_max)
    {
      frame_index = frame_index_min;
      is_enabled = 0;
    }
  else
    {
      frame_index++;
      set_image (frame_index);
    }
  return is_enabled;
}

/**
 * The animation is played in loop-mode
 */
void
sprite_object::play_animation_loop ()
{
  if (--frame_delay > 0)
    {
      return;
    }
  frame_delay = frame_period;
  if (frame_index == frame_index_max)
    {
      frame_index = frame_index_min;
      set_image (frame_index);
    }
  else
    {
      frame_index++;
      set_image (frame_index);
    }
}

//-------------------------------------------------------------------------------
// Change l'offset du BOB
//-------------------------------------------------------------------------------
void
sprite_object::new_offset (Sint32 numer)
{
  frame_index = numer;
  set_image (numer);
}

/**
 * Clip coordinates of the sprite into offscreen coordinates
 */
void
sprite_object::clip_coordinates ()
{
  if (x_coord < 0)
    {
      x_coord = 0;
    }
  else if (x_coord > screen_width - sprite_width)
    {
      x_coord = screen_width - sprite_width;
    }
  if (y_coord < 0)
    {
      y_coord = 0;
    }
  else if (y_coord > screen_height - sprite_height)
    {
      y_coord = screen_height - sprite_height;
    }
}

/** 
 * get sprite's width 
 * @return the width of the sprite in pixels
 */
Uint32
sprite_object::get_sprite_width ()
{
  return sprite_width;
}

/** 
 * get sprite's height 
 * @return the height of the sprite in pixels
 */
Uint32
sprite_object::get_sprite_height ()
{
  return sprite_height;
}

/** 
 * Get sprite's width for collision
 * @return the width of sprite for the collisions
 */
Uint32
sprite_object::get_collision_width ()
{
  return collision_width;
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
void
sprite_object::initRepeat (Sint32 value)
{
  affRepeatF = value;
  put_method = METHOD_REP;
}

void
sprite_object::set_method (Uint32 vtype)
{
  put_method = vtype;
}

void
sprite_object::set_memGFX (char *pGfx, Sint32 rGfx)
{
  adresseGFX = pGfx;
  releaseGFX = rGfx;
}

//==============================================================================
// MEMBRES STATIQUES 
//==============================================================================

const Sint32
  sprite_object::cycling_01[] =
//{     126, 126, 75, 75, 122, 122, 24, 90};
{ 0x7e7e7e7e, 0x7e7e7e7e, 0x4b4b4b4b, 0x4b4b4b4b,
  0x7a7a7a7a, 0x7a7a7a7a, 0x18181818, 0x5a5a5a5a
};

//{     63, 63, 23, 23, 53, 53, 24, 34};
const Sint32
  sprite_object::cycling_02[] =
  { 0x3f3f3f3f, 0x3f3f3f3f, 0x17171717, 0x17171717, 0x35353535,
  0x35353535, 0x18181818, 0x22222222
};


// LES ATOMS/BOUIBOUIS
bbPosition
BOB_POS000[] = { {0, 70}, {1, 70}, {2, 70}, {3, 70},    //ATOM 1
{4, 70}, {5, 70}, {6, 70}, {7, 70},
{0, 86}, {1, 86}, {2, 86}, {3, 86},     //ATOM 2
{4, 86}, {5, 86}, {6, 86}, {7, 86},
{0, 102}, {1, 102}, {2, 102}, {3, 102}, //ATOM 3
{4, 102}, {5, 102}, {6, 102}, {7, 102},
{0, 118}, {1, 118}, {2, 118}, {3, 118}, //ATOM 4
{4, 118}, {5, 118}, {6, 118}, {7, 118},
{0, 134}, {1, 134}, {2, 134}, {3, 134}, //ATOM 5
{4, 134}, {5, 134}, {6, 134}, {7, 134},
{0, 150}, {1, 150}, {2, 150}, {3, 150}, //ATOM 6
{4, 150}, {5, 150}, {6, 150}, {7, 150},
{0, 166}, {1, 166}, {2, 166}, {3, 166}, //ATOM 7
{4, 166}, {5, 166}, {6, 166}, {7, 166},
{0, 182}, {1, 182}, {2, 182}, {3, 182}, //ATOM 8
{4, 182}, {5, 182}, {6, 182}, {7, 182},
{0, 198}, {1, 198}, {2, 198}, {3, 198}, //ATOM 9
{4, 198}, {5, 198}, {6, 198}, {7, 198},
{0, 214}, {1, 214}, {2, 214}, {3, 214}, //ATOM 10
{4, 214}, {5, 214}, {6, 214}, {7, 214},
{8, 70}, {9, 70}, {10, 70}, {11, 70},   //ATOM 11
{12, 70}, {13, 70}, {14, 70}, {15, 70},
{8, 86}, {9, 86}, {10, 86}, {11, 86},   //ATOM 12
{12, 86}, {13, 86}, {14, 86}, {15, 86},
{8, 102}, {9, 102}, {10, 102}, {11, 102},       //ATOM 13 
{12, 102}, {13, 102}, {14, 102}, {15, 102},
{8, 118}, {9, 118}, {10, 118}, {11, 118},       //ATOM 14
{12, 118}, {13, 118}, {14, 118}, {14, 118},
{8, 134}, {9, 134}, {10, 134}, {11, 134},       //ATOM 15
{12, 134}, {13, 134}, {14, 134}, {15, 134},
{8, 150}, {9, 150}, {10, 150}, {11, 150},       //ATOM 16
{12, 150}, {13, 150}, {14, 150}, {15, 150},
{8, 166}, {9, 166}, {10, 166}, {11, 166},       //ATOM 17
{12, 166}, {13, 166}, {14, 166}, {15, 166},
{8, 182}, {9, 182}, {10, 182}, {11, 182},       //ATOM 18
{12, 182}, {13, 182}, {14, 182}, {15, 182},
{8, 198}, {9, 198}, {10, 198}, {11, 198},       //ATOM 19
{12, 198}, {13, 198}, {14, 198}, {15, 198},
{8, 214}, {9, 214}, {10, 214}, {11, 214},       //ATOM 20
{12, 214}, {13, 214}, {14, 214}, {15, 214},
{0, 54}, {1, 54}, {2, 54}, {3, 54},     //EXPLOSION 1
{4, 54}, {5, 54}, {6, 54}, {7, 54},
{8, 54}, {9, 54}, {10, 54}, {11, 54},   //EXPLOSION 2
{12, 54}, {13, 54}, {14, 54}, {15, 54}
};
bb_describ
BOB_NUM000 = { 16, 16, 22 * 8, BOB_POS000 };

// Ejecteurs ------------------------------------------------------------//
bbPosition
BOB_POS001[] = { {14, 5} };
bb_describ
BOB_NUM001 = { 16, 16, 1, BOB_POS001 };
bbPosition
BOB_POS002[] = { {15, 5} };
bb_describ
BOB_NUM002 = { 16, 16, 1, BOB_POS002 };
bbPosition
BOB_POS003[] = { {16, 5} };
bb_describ
BOB_NUM003 = { 16, 16, 1, BOB_POS003 };
bbPosition
BOB_POS004[] = { {17, 5} };
bb_describ
BOB_NUM004 = { 16, 16, 1, BOB_POS004 };

// Briques -------------------------------------------------------------//
bbPosition
BOB_POS005[] = { {18, 5} };     // bricote verticale
bb_describ
BOB_NUM005 = { 4, 16, 1, BOB_POS005 };
bbPosition
BOB_POS006[] = { {11, 0} };     // bricote horizontale
bb_describ
BOB_NUM006 = { 16, 4, 1, BOB_POS006 };
bbPosition
BOB_POS007[] = { {14, 21} };    // brique du jeu
bb_describ
BOB_NUM007 = { 16, 7, 1, BOB_POS007 };

// Raquettes -----------------------------------------------------------//
bbPosition
BOB_POS008[28] = { {0, 0}, {4, 0}, {8, 0}, {12, 0}, {16, 0}, {20, 0}, {24, 0},
{0, 8}, {4, 8}, {8, 8}, {12, 8}, {16, 8}, {20, 8}, {24, 8},
{0, 16}, {4, 16}, {8, 16}, {12, 16}, {16, 16}, {20, 16}, {24, 16},
{0, 24}, {4, 24}, {8, 24}, {12, 24}, {16, 24}, {20, 24}, {24, 24}
};
bb_describ
BOB_NUM008 = { 64, 8, 28, BOB_POS008 };
bbPosition
BOB_POS009[28] =
  { {0, 32}, {1, 32}, {2, 32}, {3, 32}, {4, 32}, {5, 32}, {6, 32},
{7, 32}, {8, 32}, {9, 32}, {10, 32}, {11, 32}, {12, 32}, {13, 32},
{14, 32}, {15, 32}, {16, 32}, {17, 32}, {18, 32}, {19, 32}, {20, 32},
{21, 32}, {22, 32}, {23, 32}, {24, 32}, {25, 32}, {26, 32}, {27, 32}
};
bb_describ
BOB_NUM009 = { 8, 64, 28, BOB_POS009 };

// 7 gigablitz ---------------------------------------------------------//

bbPosition
BOB_POS021[1] = { {0, 0} };
bb_describ
BOB_NUM021 = { 16, 32, 1, BOB_POS021 };

bbPosition
BOB_POS022[1] = { {0, 32} };
bb_describ
BOB_NUM022 = { 24, 48, 1, BOB_POS022 };

bbPosition
BOB_POS023[1] = { {0, 80} };
bb_describ
BOB_NUM023 = { 32, 64, 1, BOB_POS023 };

bbPosition
BOB_POS024[1] = { {0, 144} };
bb_describ
BOB_NUM024 = { 40, 80, 1, BOB_POS024 };

bbPosition
BOB_POS025[1] = { {0, 224} };
bb_describ
BOB_NUM025 = { 48, 96, 1, BOB_POS025 };

bbPosition
BOB_POS026[1] = { {0, 320} };
bb_describ
BOB_NUM026 = { 56, 112, 1, BOB_POS026 };

bbPosition
BOB_POS027[1] = { {0, 432} };
bb_describ
BOB_NUM027 = { 64, 128, 1, BOB_POS027 };

// Balles -------------------------------------------------------------// 
bbPosition
  BOB_POS010[] = {
  {14, 46},                     // Verte 1
  {1, 497},                     // Grise 1
  {14, 28},                     // Jaune 1
  {15, 37},                     // Verte 2
  {2, 497},                     // Grise 2
  {15, 28},                     // Jaune 2
  {3, 488},                     // Verte 3
  {3, 497},                     // Grise 3
  {14, 37}
};                              // Jaune 3
bb_describ
BOB_NUM010 = { 9, 9, 9, BOB_POS010 };


// Tirs --------------------------------------------------------------//
bbPosition
BOB_POS011[] = { {3, 0},        // TIR 2
{4, 0},
{5, 0},
{6, 0},
{7, 0},                         // TIR 1
{8, 0},
{9, 0},
{10, 0}
};
bb_describ
BOB_NUM011 = { 5, 5, 8, BOB_POS011 };

// Monnaie------------------------------------------------------------//
bbPosition
BOB_POS012[] =
  { {7, 33}, {8, 33}, {9, 33}, {10, 33}, {11, 33}, {12, 33}, {13, 33} };
bb_describ
BOB_NUM012 = { 16, 7, 7, BOB_POS012 };

// Gadgets Bonus/Malus ---------------------------------------------------------
bbPosition
BOB_POS013[] = { {0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}, {6, 5},        //Size+
{0, 12}, {1, 12}, {2, 12}, {3, 12}, {4, 12}, {5, 12}, {6, 12},  //Size-
{0, 19}, {1, 19}, {2, 19}, {3, 19}, {4, 19}, {5, 19}, {6, 19},  //Life+
{0, 26}, {1, 26}, {2, 26}, {3, 26}, {4, 26}, {5, 26}, {6, 26},  //Life-
{0, 33}, {1, 33}, {2, 33}, {3, 33}, {4, 33}, {5, 33}, {6, 33},  //BallPower1
{0, 40}, {1, 40}, {2, 40}, {3, 40}, {4, 40}, {5, 40}, {6, 40},  //BallPower2
{0, 47}, {1, 47}, {2, 47}, {3, 47}, {4, 47}, {5, 47}, {6, 47},  //Balls+2
{7, 5}, {8, 5}, {9, 5}, {10, 5}, {11, 5}, {12, 5}, {13, 5},     //Balls+3
{7, 12}, {8, 12}, {9, 12}, {10, 12}, {11, 12}, {12, 12}, {13, 12},      //Fire1
{7, 19}, {8, 19}, {9, 19}, {10, 19}, {11, 19}, {12, 19}, {13, 19},      //Fire2
{7, 26}, {8, 26}, {9, 26}, {10, 26}, {11, 26}, {12, 26}, {13, 26},      //Glue
{7, 40}, {8, 40}, {9, 40}, {10, 40}, {11, 40}, {12, 40}, {13, 40},      //Inverse
{17, 219}, {18, 219}, {18, 246}, {19, 246}, {19, 261}, {19, 239}, {16, 268},    //BallSize2
{17, 268}, {18, 268}, {19, 268}, {10, 481}, {11, 481}, {12, 481}, {13, 481},    //BallSize3
{14, 392}, {14, 399}, {14, 406}, {14, 413}, {14, 420}, {14, 427}, {14, 434},    //Random
{15, 392}, {15, 399}, {15, 406}, {15, 413}, {15, 420}, {15, 427}, {15, 434},    //Mega
{0, 635}, {1, 635}, {2, 635}, {3, 635}, {4, 635}, {5, 635}, {6, 635},   //WALL
{7, 635}, {8, 635}, {9, 635}, {10, 635}, {11, 635}, {12, 635}, {13, 635},       //ROBOT
{14, 635}, {15, 635}, {16, 635}, {17, 635}, {18, 635}, {19, 635}, {2, 490},     //CONTROLATOR
{16, 205}, {17, 205}, {18, 205}, {16, 212}, {17, 212}, {18, 212}, {16, 219}     //protect
};
bb_describ
BOB_NUM013 = { 16, 7, 7 * 20, BOB_POS013 };

// 43 lettres de 32x32 ---------------------------------------------------------
bbPosition
BOB_POS014[] =
  { {0, 758}, {1, 758}, {2, 758}, {3, 758}, {4, 758}, {5, 758}, {6, 758}, {7,
                                                                           758},
  {8, 758}, {9, 758},
{10, 758}, {11, 758}, {12, 758}, {13, 758}, {14, 758}, {15, 758}, {16, 758},
  {17, 758}, {18, 758}, {19, 758},
{0, 774}, {1, 774}, {2, 774}, {3, 774}, {4, 774}, {5, 774}, {6, 774}, {7,
                                                                       774},
  {8, 774}, {9, 774},
{10, 774}, {11, 774}, {12, 774}, {13, 774}, {14, 774}, {15, 774}, {16, 774},
  {17, 774}, {18, 774}, {19, 774},
{0, 790}, {1, 790}, {2, 790}
};
bb_describ
BOB_NUM014 = { 16, 16, 42, BOB_POS014 };

// LED du magasin --------------------------------------------------------------
bbPosition
BOB_POS015[] = { {13, 0} };
bb_describ
BOB_NUM015 = { 16, 3, 1, BOB_POS015 };
bbPosition
BOB_POS057[] = { {15, 0} };
bb_describ
BOB_NUM057 = { 16, 5, 1, BOB_POS057 };

// GAME OVER -------------------------------------------------------------------
bbPosition
BOB_POS016[] = { {0, 246},      // G x*32 / y*2
{2, 246},                       // A
{4, 246},                       // M
{6, 246},                       // E
{8, 246},                       // O
{10, 246},                      // V
{12, 246},                      // E
{14, 246}                       // R
};
bb_describ
BOB_NUM016 = { 32, 32, 8, BOB_POS016 };

// CARACTERES 32*40 DU MENU (DEFILEMENT TEXTE)----------------------------------
bbPosition
  BOB_POS017[43] = {
  {0, 587}, {1, 587}, {2, 587}, {3, 587}, {4, 587},
  {5, 587}, {6, 587}, {7, 587}, {8, 587}, {9, 587},
  {10, 587}, {11, 587}, {12, 587}, {13, 587}, {14, 587},
  {15, 587}, {16, 587}, {17, 587}, {18, 587}, {19, 587},
  {0, 607}, {1, 607}, {2, 607}, {3, 607}, {4, 607},
  {5, 607}, {6, 607}, {7, 607}, {8, 607}, {9, 607},
  {10, 607}, {11, 607}, {12, 607}, {13, 607}, {14, 607},
  {15, 607}, {16, 607}, {17, 607}, {18, 607}, {19, 607},
  {17, 520}, {18, 520}, {16, 520}
};
bb_describ
BOB_NUM017 = { 16, 20, 43, BOB_POS017 };

// LOGO TECNOBALLZ--------------------------------------------------------------
bbPosition
BOB_POS018[1] = { {0, 520} };
bb_describ
BOB_NUM018 = { 256, 67, 1, BOB_POS018 };

// Pointeur Souris 1 -----------------------------------------------------------
bbPosition
BOB_POS019[8] = { {0, 230}, {1, 230}, {2, 230}, {3, 230},
{4, 230}, {5, 230}, {6, 230}, {7, 230}
};
bb_describ
BOB_NUM019 = { 16, 16, 8, BOB_POS019 };

// Pointeur Souris 2 -----------------------------------------------------------
bbPosition
BOB_POS020[8] = { {8, 230}, {9, 230}, {10, 230}, {11, 230},
{12, 230}, {13, 230}, {14, 230}, {15, 230}
};
bb_describ
BOB_NUM020 = { 16, 16, 8, BOB_POS020 };

// lettre pour afficher la somme d'argent --------------------------------------
bbPosition
BOB_POS028[10] = { {3, 798}, {4, 798}, {5, 798}, {6, 798}, {7, 798},
{8, 798}, {9, 798}, {10, 798}, {11, 798}, {12, 798}
};
bb_describ
BOB_NUM028 = { 8, 8, 10, BOB_POS028 };

// raquette robot en bas -------------------------------------------------------
//bbPosition BOB_POS029[1] = { {16, 444} } ;
bbPosition
BOB_POS029[1] = { {16, 448} };
bb_describ
BOB_NUM029 = { 64, 8, 1, BOB_POS029 };


// *****************************************************************************
// GUARDIANS                                                                   *
// *****************************************************************************

bbPosition
BOB_POS030[1] = { {0, 278} };
bb_describ
BOB_NUM030 = { 64, 104, 1, BOB_POS030 };
bbPosition
BOB_POS031[1] = { {4, 278} };
bb_describ
BOB_NUM031 = { 64, 99, 1, BOB_POS031 };
bbPosition
BOB_POS032[1] = { {8, 278} };
bb_describ
BOB_NUM032 = { 64, 90, 1, BOB_POS032 };
bbPosition
BOB_POS033[1] = { {8, 368} };
bb_describ
BOB_NUM033 = { 64, 92, 1, BOB_POS033 };
bbPosition
BOB_POS034[1] = { {12, 278} };
bb_describ
BOB_NUM034 = { 64, 114, 1, BOB_POS034 };
bbPosition
BOB_POS035[1] = { {16, 278} };
bb_describ
BOB_NUM035 = { 64, 163, 1, BOB_POS035 };
bbPosition
BOB_POS036[1] = { {0, 382} };
bb_describ
BOB_NUM036 = { 32, 16, 1, BOB_POS036 };
bbPosition
BOB_POS037[1] = { {0, 398} };
bb_describ
BOB_NUM037 = { 32, 55, 1, BOB_POS037 };
bbPosition
BOB_POS038[1] = { {0, 453} };
bb_describ
BOB_NUM038 = { 32, 36, 1, BOB_POS038 };
bbPosition
BOB_POS039[1] = { {2, 382} };
bb_describ
BOB_NUM039 = { 32, 52, 1, BOB_POS039 };
bbPosition
BOB_POS040[1] = { {2, 434} };
bb_describ
BOB_NUM040 = { 32, 36, 1, BOB_POS040 };
bbPosition
BOB_POS041[1] = { {4, 377} };
bb_describ
BOB_NUM041 = { 32, 82, 1, BOB_POS041 };
bbPosition
BOB_POS042[1] = { {4, 459} };
bb_describ
BOB_NUM042 = { 32, 47, 1, BOB_POS042 };
bbPosition
BOB_POS043[1] = { {6, 377} };
bb_describ
BOB_NUM043 = { 32, 74, 1, BOB_POS043 };
bbPosition
BOB_POS044[1] = { {6, 451} };
bb_describ
BOB_NUM044 = { 32, 65, 1, BOB_POS044 };
bbPosition
BOB_POS045[1] = { {12, 392} };
bb_describ
BOB_NUM045 = { 32, 85, 1, BOB_POS045 };

// raquette 
bbPosition
BOB_POS046[1] = { {16, 246} };
bb_describ
BOB_NUM046 = { 32, 8, 1, BOB_POS046 };

// tirs des gardiens
bbPosition
BOB_POS047[16] =
  { {19, 141}, {8, 460}, {19, 152}, {9, 460}, {19, 163}, {10, 460}, {19, 174},
  {11, 460},
{19, 185}, {8, 471}, {19, 196}, {9, 471}, {19, 207}, {16, 573}, {19, 218},
  {17, 573}
};
bb_describ
BOB_NUM047 = { 11, 11, 16, BOB_POS047 };


// explosion 1 ****
bbPosition
BOB_POS048[8] = { {0, 54}, {1, 54}, {2, 54}, {3, 54},   //EXPLOSION 1
{4, 54}, {5, 54}, {6, 54}, {7, 54}
};
bb_describ
BOB_NUM048 = { 16, 16, 8, BOB_POS048 };

// explosion 2 ****
bbPosition
BOB_POS049[8] = { {8, 54}, {9, 54}, {10, 54}, {11, 54}, //EXPLOSION 2
{12, 54}, {13, 54}, {14, 54}, {15, 54}
};
bb_describ
BOB_NUM049 = { 16, 16, 8, BOB_POS049 };

// boule argentee (congratulation)
bbPosition
BOB_POS050[8] = { {14, 462} };
bb_describ
BOB_NUM050 = { 25, 26, 1, BOB_POS050 };

// niveau d'energie des gadiens --------------------------------------//
bbPosition
BOB_POS051[] = { {14, 0} };
bb_describ
BOB_NUM051 = { 16, 1, 1, BOB_POS051 };

// gem ---------------------------------------------------------------//
bbPosition
BOB_POS052[] = { {10, 472},     // grey square
{11, 472},                      // green sphere
{18, 574},                      // yellow ring
{19, 574},                      // blue triangle
{3, 480},                       // rhombus
{3, 472}                        // pentagon
};
bb_describ
BOB_NUM052 = { 8, 8, 6, BOB_POS052 };

// bottom wall -------------------------------------------------------//
bbPosition
BOB_POS053[] = { {0, 627} };
bb_describ
BOB_NUM053 = { 192, 8, 1, BOB_POS053 };

bbPosition
BOB_POS054[] = { {4, 730}, {5, 730}, {6, 730}, {7, 730},
{8, 730}, {9, 730}, {10, 730}, {11, 730},
{12, 730}, {13, 730}, {14, 730}, {15, 730},
{16, 730}, {17, 730}, {18, 730}, {19, 730},
};
bb_describ
BOB_NUM054 = { 9, 9, 16, BOB_POS054 };

bbPosition
BOB_POS055[] = { {16, 21}, {17, 21}, {18, 21}, {19, 21}, {16, 25}, {17, 25},
{18, 25}, {19, 25}
};
bb_describ
BOB_NUM055 = { 4, 4, 8, BOB_POS055 };

bbPosition
BOB_POS056[] =
  { {0, 642}, {2, 642}, {4, 642}, {6, 642}, {8, 642}, {10, 642}, {12, 642},
  {14, 642}, {16, 642}, {18, 642},
{0, 671}, {2, 671}, {4, 671}, {6, 671}, {8, 671}, {10, 671}, {12, 671}, {14,
                                                                         671},
  {16, 671}, {18, 671},
{0, 700}, {2, 700}, {4, 700}, {6, 700}, {8, 700}, {10, 700}, {12, 700}, {14,
                                                                         700},
  {16, 700}, {18, 700},
{0, 729}, {2, 729}
};
bb_describ
BOB_NUM056 = { 32, 29, 32, BOB_POS056 };

// bricks
bbPosition
BOB_POS058[] = { {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0},
{0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}, {6, 7},
{0, 14}, {1, 14}, {2, 14}, {3, 14}, {4, 14}, {5, 14}, {6, 14},
{0, 21}, {1, 21}, {2, 21}, {3, 21}, {4, 21}, {5, 21}, {6, 21},
{0, 28}, {1, 28}, {2, 28}, {3, 28}, {4, 28}, {5, 28}, {6, 28},
{0, 35}, {1, 35}, {2, 35}, {3, 35}, {4, 35}, {5, 35}, {6, 35},
{0, 42}, {1, 42}, {2, 42}, {3, 42}, {4, 42}, {5, 42}, {6, 42},
{0, 49}, {1, 49}, {2, 49}, {3, 49}, {4, 49}, {5, 49}, {6, 49},
{0, 56}, {1, 56}, {2, 56}, {3, 56}, {4, 56}, {5, 56}, {6, 56}
};
bb_describ
BOB_NUM058 = { 16, 7, 63, BOB_POS058 };

/*
bbPosition BOB_POS058[] =
{	{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0} };
bb_describ BOB_NUM058 = {16, 7, 7, BOB_POS058};
bbPosition BOB_POS059[] =
{	{0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}, {6, 7}, {7, 7} };
bb_describ BOB_NUM059 = {16, 7, 7, BOB_POS059};
bbPosition BOB_POS060[] =
{	{0, 14}, {1, 14}, {2, 14}, {3, 14}, {4, 14}, {5, 14}, {6, 14}, {7, 14} };
bb_describ BOB_NUM060 = {16, 7, 7, BOB_POS060};
bbPosition BOB_POS061[] =
{	{0, 21}, {1, 21}, {2, 21}, {3, 21}, {4, 21}, {5, 21}, {6, 21}, {7, 21} };
bb_describ BOB_NUM061 = {16, 7, 7, BOB_POS061};
bbPosition BOB_POS062[] =
{	{0, 28}, {1, 28}, {2, 28}, {3, 28}, {4, 28}, {5, 28}, {6, 28}, {7, 28} };
bb_describ BOB_NUM062 = {16, 7, 7, BOB_POS062};
bbPosition BOB_POS063[] =
{	{0, 35}, {1, 35}, {2, 35}, {3, 35}, {4, 35}, {5, 35}, {6, 35}, {7, 35} };
bb_describ BOB_NUM063 = {16, 7, 7, BOB_POS063};
bbPosition BOB_POS064[] =
{	{0, 42}, {1, 42}, {2, 42}, {3, 42}, {4, 42}, {5, 42}, {6, 42}, {7, 42} };
bb_describ BOB_NUM064 = {16, 7, 7, BOB_POS064};
bbPosition BOB_POS065[] =
{	{0, 49}, {1, 49}, {2, 49}, {3, 49}, {4, 49}, {5, 49}, {6, 49}, {7, 49} };
bb_describ BOB_NUM065 = {16, 7, 7, BOB_POS065};
bbPosition BOB_POS066[] =
{	{0, 56}, {1, 56}, {2, 56}, {3, 56}, {4, 56}, {5, 56}, {6, 56}, {7, 56} };
bb_describ BOB_NUM066 = {16, 7, 7, BOB_POS066};
*/

const bb_describ *
  sprite_object::zelistBOB[] = { &BOB_NUM000,   // BouisBouis
  &BOB_NUM001,                  // Ejector
  &BOB_NUM002,                  //
  &BOB_NUM003,                  //
  &BOB_NUM004,                  //
  &BOB_NUM005,                  // Brique verticale
  &BOB_NUM006,                  // Brique horizontale
  &BOB_NUM007,                  // Brique jeu
  &BOB_NUM008,                  // Raquette horizontale
  &BOB_NUM009,                  // Raquette verticale
  &BOB_NUM010,                  // Balls
  &BOB_NUM011,                  // Bumper's fires
  &BOB_NUM012,                  // Capsule of money
  &BOB_NUM013,                  // Gadgets (bonuses or maluses)
  &BOB_NUM014,                  // Lettres (annonce)
  &BOB_NUM015,                  // shop's led (low-res) 
  &BOB_NUM016,                  // Lettres GameOver
  &BOB_NUM017,                  // Lettres defilement menu
  &BOB_NUM018,                  // Logo TecnoballZ
  &BOB_NUM019,                  // Pointeur souris 1
  &BOB_NUM020,                  // Pointeur souris 2
  &BOB_NUM021,                  // GigaBlitz 1
  &BOB_NUM022,                  // GigaBlitz 2
  &BOB_NUM023,                  // GigaBlitz 3
  &BOB_NUM024,                  // GigaBlitz 4
  &BOB_NUM025,                  // GigaBlitz 5
  &BOB_NUM026,                  // GigaBlitz 6
  &BOB_NUM027,                  // GigaBlitz 7
  &BOB_NUM028,                  // Chiffres de 0 a 10 pour afficher le credit
  &BOB_NUM029,                  // Robot bottom bumper (bricks levels)
  &BOB_NUM030,                  // Guard final 1
  &BOB_NUM031,                  // Guard final 2
  &BOB_NUM032,                  // Guard final 3
  &BOB_NUM033,                  // Guard final 4
  &BOB_NUM034,                  // Guard final 5
  &BOB_NUM035,                  // Guard final 6
  &BOB_NUM036,                  // Guard intermediary 1
  &BOB_NUM037,                  // Guard intermediary 1
  &BOB_NUM038,                  // Guard intermediary 2
  &BOB_NUM039,                  // Guard intermediary 2
  &BOB_NUM040,                  // Guard intermediary 3
  &BOB_NUM041,                  // Guard intermediary 3
  &BOB_NUM042,                  // Guard intermediary 4
  &BOB_NUM043,                  // Guard intermediary 4
  &BOB_NUM044,                  // Guard intermediary 5
  &BOB_NUM045,                  // Guard intermediary 5
  &BOB_NUM046,                  // bumper into guards level
  &BOB_NUM047,                  // Guards's weapeon
  &BOB_NUM048,                  // explosion's guard (type 1)  
  &BOB_NUM049,                  // explosion's guard (type 2)
  &BOB_NUM050,                  // boule argentee
  &BOB_NUM051,                  // guards's energy bar 
  &BOB_NUM052,                  // Gem
  &BOB_NUM053,
  &BOB_NUM054,                  //directeur
  &BOB_NUM055,                  //echape menu
  &BOB_NUM056,                  //maget eye
  &BOB_NUM057,                  //shop's led (high-res)
  &BOB_NUM058,                  //brick 
/*	&BOB_NUM059,	//brick 
	&BOB_NUM060,	//brick 
	&BOB_NUM061,	//brick 
	&BOB_NUM062,	//brick 
	&BOB_NUM063,	//brick 
	&BOB_NUM064,	//brick 
	&BOB_NUM065,	//brick 
	&BOB_NUM066	//brick 
*/
};


Sint32
  sprite_object::ombredecax = handler_display::SHADOWOFFX;
Sint32
  sprite_object::ombredecay = handler_display::SHADOWOFFY;
char
  sprite_object::ombrepixel = handler_display::SHADOW_PIX;
Sint32
  sprite_object::ombrepixe4 = handler_display::SHADOWLONG;
