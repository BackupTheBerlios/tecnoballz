/** 
 * @file supervisor_map_editor.cc 
 * @brief The tile map editor for the menu and guardians levels 
 * @created 2004-09-13 
 * @date 2007-03-21
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.8 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_map_editor.cc,v 1.8 2007/03/31 10:55:03 gurumeditation Exp $
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
#include "../include/supervisor_map_editor.h"
#include "../include/handler_resources.h"
#include "../include/list_sprites.h"
#include "../include/handler_keyboard.h"
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

/**
 * Create the tile map editor
 */
supervisor_map_editor::supervisor_map_editor ()
{
  initialise ();
  /*  vertical background scrolling */
  tiles_map = new tilesmap_scrolling ();
  mouse_pointer = new sprite_mouse_pointer ();
  view_mode = SHOW_MAP;
  is_space_key_down = false;
  titlesPosy = 0;
  flag_press = 0;
  box_colour = 0;
  tiles_brush = NULL;
  brush_bitmap = (bitmap_data *) NULL;
  flag_press = 0;
  flagPress2 = 0;
  brush_posx = 0;
  brush_posy = 0;
  brushWidth = 0;
  brushHeigh = 0;
  is_s_key_down = false;
  first_init ();
}

/**
 * Release the tile map editor
 */
supervisor_map_editor::~supervisor_map_editor ()
{
  delete mouse_pointer;
  delete tiles_map;
  if (NULL != pt_select1)
    {
      delete pt_select1;
    }
  if (NULL != pt_select2)
    {
      delete pt_select2;
    }
  if (tiles_brush)
    {
      delete[]tiles_brush;
      tiles_brush = NULL;
      //memory->release ((char *) tiles_brush);
    }
  if (NULL != brush_bitmap)
    {
      delete brush_bitmap;
      brush_bitmap = NULL;
    }
  liberation ();
}

/**
 * Perform some initializations
 */
Sint32
supervisor_map_editor::first_init ()
{

  try
    {
      pt_select1 = new selectinfo;
      pt_select2 = new selectinfo;
    }
  catch (std::bad_alloc &)
  {
    std::
      cerr << "(!)supervisor_map_editor::first_init() "
      "not enough memory to allocate " <<
      " 'selectinfo' structure!" << std::endl;
    throw;
  }
  pt_select0 = pt_select1;


  screen_height = display->get_height ();
  screen_width = display->get_width ();

  sprites->reset ();
  resources->load_sprites_bitmap ();
  mouse_pointer->create_pointer_sprite (sprites_bitmap);
  resources->release_sprites_bitmap ();


  //Sint32        edmap = tilesmap_scrolling::MAPED_MENU;
  //Sint32        edmap = tilesmap_scrolling::MAPED_GARD;
  Sint32 edmap = tilesmap_scrolling::MAPED_CONG;

  tiles_map->initialize (tilesmap_scrolling::TILES_COLOR_MENU, edmap);
  ptrGBitMap = tiles_map->get_bitmap ();

  tile_width = tiles_map->get_tiles_width ();
  tile_mask1 = 0xffffffff ^ (tile_width - 1);
  tile_mask2 = ~tile_mask1;

  display->gradation1 ();
  return 0;
}

/** 
 * Main loop
 */
Sint32
supervisor_map_editor::main_loop ()
{
  display->wait_frame ();

  pt_select1->boxOffsetY = tiles_map->get_y_coord ();
  pt_select2->boxOffsetY = titlesPosy;

  switch (view_mode)
    {
    case SHOW_TILES:
      view_tiles ();
      break;
    case SHOW_MAP:
    default:
      view_map_editor ();
      break;
    }

  display->lock_surfaces ();
  mouse_pointer->move ();
  sprites->draw ();


  //###################################################################
  // copy whole buffer surface into screen surface
  //###################################################################
  display->unlock_surfaces ();
  display->bufferCTab ();

  /* [ctrl] + escape key to leave! */
  if (keyboard->command_is_pressed (handler_keyboard::TOEXITFLAG))
    {
      end_return = LEAVE_TECNOBALLZ;
    }

  check_keys ();
  /* back to menu */
  if (keyboard->key_is_pressed (SDLK_F10))
    {
      end_return = MAIN_MENU;
    }


  /* save the map */
  if (keyboard->key_is_pressed (SDLK_s) && !is_s_key_down)
    {
      is_s_key_down = true;
    }
  if (keyboard->key_is_released (SDLK_s) && is_s_key_down)
    {
      is_s_key_down = false;
      saveTheMap ();
    }
  return end_return;
}

/**
 * Display map editor view
 */
void
supervisor_map_editor::view_map_editor ()
{
  pt_select0 = pt_select1;
  Sint32 speed = get_speed ();
  /* draw tiles map on the screen */
  tiles_map->scrolling1 (speed); 
  select_box ();
  drawingBox ();
  brush_draw ();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void
supervisor_map_editor::map_to_brush ()
{
  printf ("supervisor_map_editor::map_to_brush() : [%i, %i, %i, %i]\n",
          pt_select0->box_pos_x1, pt_select0->box_pos_y1,
          pt_select0->box_pos_x2, pt_select0->box_pos_y2);

  //###################################################################
  // allocate map memory
  //###################################################################

  /*
   * Allocate memory for tiles brush
   */
  if (NULL != tiles_brush)
    {
      delete[]tiles_brush;
    }
  Uint32 size = pt_select0->box_height * pt_select0->box_widthT;
  try
    {
      tiles_brush = new Uint16[size];
    }
  catch (std::bad_alloc &)
  {
    std::
      cerr << "(!)supervisor_map_editor::map_to_brush() "
      "not enough memory to allocate " <<
       size << " Uint16!" << std::endl;
    throw;
  }

/*
  if (tiles_brush)
    memory->release ((char *) tiles_brush);
  tiles_brush = (Uint16 *) memory->alloc
    (sizeof (Uint16) * pt_select0->box_height * pt_select0->box_widthT,
     0x4D454741);
  error_init (memory->retour_err ());
  if (erreur_num)
    return;
   */

  //Sint32 scrlY = tiles_map->returnPosy();
  Sint32 i = pt_select0->box_pos_y1;
  i = (i / tiles_map->tile_height) + 4;
  i *= tilesmap_scrolling::MAP_WIDTH;
  i += (pt_select0->box_pos_x1 / tiles_map->tile_width);

  Uint16 *carte = tiles_map->map_tiles + i;
  //Uint16* carte = tiles_map->map_tiles;
  Uint16 *ptBrh = tiles_brush;
  for (Uint32 y = 0; y < pt_select0->box_height; y++)
    {
      for (Uint32 x = 0; x < pt_select0->box_widthT; x++)
        {
        *(ptBrh++) = carte[x];
        }
      carte += tilesmap_scrolling::MAP_WIDTH;
    }
  alloc_brush ();
}

//------------------------------------------------------------------------------
// tiles wiew mode
//------------------------------------------------------------------------------
void
supervisor_map_editor::view_tiles ()
{
  pt_select0 = pt_select2;
  Sint32 speed = get_speed ();
  Sint32 y_max = ptrGBitMap->get_height () - screen_height;

  titlesPosy = titlesPosy + speed;
  if (titlesPosy < 0)
    titlesPosy = 0;
  if (titlesPosy > y_max)
    titlesPosy = y_max;

  //printf("y_max :%i / titlesPosy: %i\n", y_max, titlesPosy);
  ptrGBitMap->copyBuffer (0, titlesPosy, 0, 0, screen_width, screen_height);
  select_box ();
  drawingBox ();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void
supervisor_map_editor::tile2brush ()
{
  printf ("supervisor_map_editor::tile2brush() : [%i, %i, %i, %i]\n",
          pt_select0->box_pos_x1, pt_select0->box_pos_y1,
          pt_select0->box_pos_x2, pt_select0->box_pos_y2);


  Sint32 o =
    (pt_select0->box_pos_y1 / tile_width) * tilesmap_scrolling::MAP_WIDTH +
    (pt_select0->box_pos_x1 / tile_width);

  //###################################################################
  // allocate map memory
  //###################################################################
  if (tiles_brush)
    memory->release ((char *) tiles_brush);
  tiles_brush = (Uint16 *) memory->alloc
    (sizeof (Uint16) * pt_select0->box_height * pt_select0->box_widthT,
     0x4D454741);
  error_init (memory->retour_err ());
  if (erreur_num)
    return;


  Uint16 *ptBrh = tiles_brush;
  for (Sint32 y = 0; y < pt_select0->box_height; y++)
    {
      Sint32 p = o;
      for (Sint32 x = 0; x < pt_select0->box_widthT; x++)
        {                       //printf("%i ", p);
          *(ptBrh++) = p;
          p++;
        }
      o += tilesmap_scrolling::MAP_WIDTH;
      //printf("\n");
    }
  alloc_brush ();
}




/**
 * Check keyboard keys
 */
void
supervisor_map_editor::check_keys ()
{
  if (keyboard->key_is_pressed (SDLK_SPACE))
    {
      is_space_key_down = true;
    }
  if (keyboard->key_is_released (SDLK_SPACE) && is_space_key_down)
    {
      switch (view_mode)
        {
        case SHOW_MAP:
          view_mode = SHOW_TILES;
          break;
        default:
          view_mode = SHOW_MAP;
          break;
        }
      is_space_key_down = false;
    }
}

/**
 * Determine vertical scrolling speed
 * @return srolling speed
 */
Sint32
supervisor_map_editor::get_speed ()
{
  Sint32 speed = 0;
  Sint32 mousY = keyboard->get_mouse_y ();
  if (mousY > 0 && mousY < 8 * resolution)
    speed = -16 * resolution;
  if (mousY >= 8 * resolution && mousY < 16 * resolution)
    speed = -8 * resolution;
  if (mousY >= 16 * resolution && mousY < 24 * resolution)
    speed = -4 * resolution;
  if (mousY <= screen_height - 16 * resolution
      && mousY > screen_height - 24 * resolution)
    speed = 4 * resolution;
  if (mousY <= screen_height - 8 * resolution
      && mousY > screen_height - 16 * resolution)
    speed = 8 * resolution;
  if (mousY < screen_height && mousY > screen_height - 8 * resolution)
    speed = 16 * resolution;
  return speed;
}


//------------------------------------------------------------------------------
// handle selection (right mouse button)
//------------------------------------------------------------------------------
void
supervisor_map_editor::select_box ()
{


  //Sint32 presL = keyboard->is_left_button();
  Sint32 presR = keyboard->is_right_button ();

  //##############################################################
  // read y where is pressed 
  //##############################################################
  if (presR && !flag_press)
    {
      flag_press = 1;
      pt_select0->box_pos_x1 = keyboard->get_mouse_x ();
      pt_select0->box_pos_y1 =
        pt_select0->boxOffsetY + keyboard->get_mouse_y ();
      pt_select0->box_pos_x1 &= tile_mask1;
      pt_select0->box_pos_y1 &= tile_mask1;
      if (brush_bitmap)
        {
          delete brush_bitmap;
          brush_bitmap = (bitmap_data *) NULL;
        }
    }


  if (flag_press)
    {
      pt_select0->box_pos_x2 = keyboard->get_mouse_x ();
      pt_select0->box_pos_y2 =
        keyboard->get_mouse_y () + pt_select0->boxOffsetY;

      if (pt_select0->box_pos_x2 & tile_mask2)
        pt_select0->box_pos_x2 += tile_width;
      if (pt_select0->box_pos_y2 & tile_mask2)
        pt_select0->box_pos_y2 += tile_width;

      pt_select0->box_pos_x2 &= tile_mask1;
      pt_select0->box_pos_y2 &= tile_mask1;
      if (pt_select0->box_pos_x2 >= pt_select0->box_pos_x1 &&
          pt_select0->box_pos_x2 - pt_select0->box_pos_x1 < tile_width)
        pt_select0->box_pos_x2 = pt_select0->box_pos_x1 + tile_width;

      if (pt_select0->box_pos_x2 < pt_select0->box_pos_x1 &&
          pt_select0->box_pos_x1 - pt_select0->box_pos_x2 < tile_width)
        pt_select0->box_pos_x2 = pt_select0->box_pos_x1 - tile_width;

      if (pt_select0->box_pos_y2 >= pt_select0->box_pos_y1 &&
          pt_select0->box_pos_y2 - pt_select0->box_pos_y1 < tile_width)
        pt_select0->box_pos_y2 = pt_select0->box_pos_y1 + tile_width;

      if (pt_select0->box_pos_y2 < pt_select0->box_pos_y1 &&
          pt_select0->box_pos_y1 - pt_select0->box_pos_y2 < tile_width)
        pt_select0->box_pos_x2 = pt_select0->box_pos_x1 - tile_width;



      if (pt_select0->box_pos_y1 < pt_select0->box_pos_y2
          && pt_select0->box_pos_y2 - pt_select0->box_pos_y1 >
          (screen_height / 2))
        pt_select0->box_pos_y2 = pt_select0->box_pos_y1 + (screen_height / 2);

      if (pt_select0->box_pos_y1 > pt_select0->box_pos_y2
          && pt_select0->box_pos_y1 - pt_select0->box_pos_y2 >
          (screen_height / 2))
        pt_select0->box_pos_y2 = pt_select0->box_pos_y1 - (screen_height / 2);


    }


  if (flag_press && presR)
    {
      pt_select0->box_typeID = 2;
      //printf("supervisor_map_editor::select_box() / pressed\n");
    }
  else
    {

      //###############################################################
      // left mouse button relased
      //###############################################################
      if (!presR && flag_press)
        {
          flag_press = 0;
          pt_select0->box_typeID = 1;
          printf ("supervisor_map_editor::select_box() / relased\n");

          if (pt_select0->box_pos_x1 > pt_select0->box_pos_x2)
            {
              Sint32 x = pt_select0->box_pos_x1;
              pt_select0->box_pos_x1 = pt_select0->box_pos_x2;
              pt_select0->box_pos_x2 = x;
            }
          if (pt_select0->box_pos_y1 > pt_select0->box_pos_y2)
            {
              Sint32 y = pt_select0->box_pos_y1;
              pt_select0->box_pos_y1 = pt_select0->box_pos_y2;
              pt_select0->box_pos_y2 = y;
            }


          pt_select0->box_widthT =
            (pt_select0->box_pos_x2 - pt_select0->box_pos_x1) / tile_width;
          pt_select0->box_height =
            (pt_select0->box_pos_y2 - pt_select0->box_pos_y1) / tile_width;


          switch (view_mode)
            {
            case SHOW_TILES:
              tile2brush ();
              break;
            case SHOW_MAP:
            default:
              map_to_brush ();
              break;
            }

        }
    }

}


//------------------------------------------------------------------------------
// draw selection
//------------------------------------------------------------------------------
void
supervisor_map_editor::drawingBox ()
{
  char *pBuff;
  Sint32 tmpco = 0;

  if (pt_select0->box_pos_x2 == pt_select0->box_pos_x1 ||
      pt_select0->box_pos_y2 == pt_select0->box_pos_y1)
    {
      pt_select0->box_typeID = 0;
    }
  if (!pt_select0->box_typeID)
    return;

  Sint32 x1 = pt_select0->box_pos_x1;
  Sint32 x2 = pt_select0->box_pos_x2;
  Sint32 y1 = pt_select0->box_pos_y1 - pt_select0->boxOffsetY;
  Sint32 y2 = pt_select0->box_pos_y2 - pt_select0->boxOffsetY;

  if (x1 > x2)
    {
      Sint32 x = x1;
      x1 = x2;
      x2 = x;
    }
  if (y1 > y2)
    {
      Sint32 y = y1;
      y1 = y2;
      y2 = y;
    }

  if (box_colour++ > 32)
    box_colour = 0;
  Sint32 color = box_colour;


  //char *ptBuf = game_screen->get_pixel_data(box_pos_x1, box_pos_y1);

  Sint32 width = x2 - x1;
  Sint32 heigh = y2 - y1;

  /*printf("supervisor_map_editor::drawingBox() : [%i, %i, %i, %i]\n", 
     x1, y1, x2, y2); */


  // top
  if (y1 >= 0 && y1 < screen_height)
    {
      pBuff = game_screen->get_pixel_data (x1, y1);
      tmpco = 0;
      for (Sint32 i = 0; i < width; i++)
        {
          unsigned char pixel = cyclingtab[color];
          pBuff[i] = pixel;
          if (++tmpco == 5)
            {
              tmpco = 0;
              if (color++ > 32)
                color = 0;
            }
        }
    }

  // right
  Sint32 nextl = game_screen->get_row_size ();
  pBuff = game_screen->get_pixel_data (x2 - 1, y1 + 1);
  for (Sint32 i = 1; i < heigh; i++)
    {
      unsigned char pixel = cyclingtab[color];
      if (y1 + i >= 0 && y1 + i < screen_height)
        *pBuff = pixel;
      if (++tmpco == 5)
        {
          tmpco = 0;
          if (color++ > 32)
            color = 0;
        }
      pBuff += nextl;
    }

  // bottom
  if (y2 >= 0 && y2 < screen_height)
    {
      pBuff = game_screen->get_pixel_data (x1, y2);
      for (Sint32 i = width - 1; i >= 0; i--)
        {
          unsigned char pixel = cyclingtab[color];
          pBuff[i] = pixel;
          if (++tmpco == 5)
            {
              tmpco = 0;
              if (color++ > 32)
                color = 0;
            }
        }
    }

  // left
  pBuff = game_screen->get_pixel_data (x1, y2 - 1);
  for (Sint32 i = 1; i < heigh; i++)
    {
      unsigned char pixel = cyclingtab[color];
      if (y2 - i >= 0 && y2 - i < screen_height)
        *pBuff = pixel;
      if (++tmpco == 5)
        {
          tmpco = 0;
          if (color++ > 32)
            color = 0;
        }
      pBuff -= nextl;
    }
}

/**
 * Allocate a brush of the required size
 */
void
supervisor_map_editor::alloc_brush ()
{
  if (NULL != brush_bitmap)
    {
      delete brush_bitmap;
    }
  brush_bitmap = tiles_map->alloc_brush (tiles_brush, pt_select0->box_widthT, pt_select0->box_height);
  brushWidth = pt_select0->box_widthT;
  brushHeigh = pt_select0->box_height;
  return;
}

//------------------------------------------------------------------------------
// display brush 
//------------------------------------------------------------------------------
void
supervisor_map_editor::brush_draw ()
{
  if (!brush_bitmap)
    return;
  Sint32 pos_x = keyboard->get_mouse_x ();
  Sint32 pos_y = keyboard->get_mouse_y ();
  pos_x &= tile_mask1;
  pos_y &= tile_mask1;
  if (pos_x > screen_width - brush_bitmap->get_width ())
    pos_x = screen_width - brush_bitmap->get_width ();
  if (pos_y > screen_height - brush_bitmap->get_height ())
    pos_y = screen_height - brush_bitmap->get_height ();


  Sint32 scrlY = tiles_map->get_y_coord ();
  Sint32 presL = keyboard->is_left_button ();
  if (presL && !flagPress2)
    {
      flagPress2 = 1;
      brush_posx = pos_x;
      brush_posy = pos_y;
    }

  if (presL && flagPress2)
    {
      pos_x = brush_posx;
      pos_y = brush_posy;
    }
  else
    {
      if (!presL && flagPress2)
        {
          flagPress2 = 0;
          printf
            ("supervisor_map_editor::brush_draw() : brush_posx:%i / brush_posy:%i\n",
             brush_posx, brush_posy);


          Sint32 i = scrlY + brush_posy;


          printf
            ("supervisor_map_editor::brush_draw() : scrlY:%i /  i :%i motifhaute:%i\n",
             scrlY, i, tiles_map->tile_height);

          i = (i / tiles_map->tile_height) + 4;
          i *= tilesmap_scrolling::MAP_WIDTH;
          Uint16 *brush = tiles_brush;
          Uint16 *table = tiles_map->map_tiles + i;
          Uint16 *t_end =
            tiles_map->map_tiles +
            (tilesmap_scrolling::MAP_HEIGHT * tilesmap_scrolling::MAP_WIDTH);
          table += (brush_posx / tiles_map->tile_width);

          printf
            ("supervisor_map_editor::brush_draw() : (table - carteFirst):%i /  i:%i\n",
             (table - tiles_map->map_tiles), i);


          for (i = 0; i < brushHeigh; i++)
            {
              if (table > t_end)
                table -=
                  (tilesmap_scrolling::MAP_HEIGHT *
                   tilesmap_scrolling::MAP_WIDTH);

              for (Sint32 j = 0; j < brushWidth; j++)
                table[j] = *(brush++);
              table += tilesmap_scrolling::MAP_WIDTH;
            }
          //###################################################################
          // copy a height of the screen (for scrolling rotation)
          //###################################################################
          table = tiles_map->map_tiles;
          i =
            (tilesmap_scrolling::MAP_HEIGHT * tilesmap_scrolling::MAP_WIDTH);
          Sint32 tsupp =
            (display->get_height () / tiles_map->tile_height) * 2;
          for (Uint32 j = 0; j < (tsupp * tilesmap_scrolling::MAP_WIDTH); j++)
            tiles_map->map_tiles[i++] = table[j];
        }
    }

  brush_bitmap->copyBuffer (0, 0, pos_x, pos_y - (scrlY & tile_mask2), -1, -1);



}

Sint32
supervisor_map_editor::saveTheMap ()
{
  Sint32 zsize =
    tilesmap_scrolling::MAP_HEIGHT * tilesmap_scrolling::MAP_WIDTH;

  Sint32 msize = zsize * sizeof (Uint16);
  Uint16 *carte = (Uint16 *) memory->alloc (msize, 0x54425249);
  error_init (memory->retour_err ());
  if (erreur_num)
    return (erreur_num);

  //unsigned char* ptSrc = (unsigned char *)tiles_map->map_tiles;
  Uint16 *ptSrc = (Uint16 *) tiles_map->map_tiles;
  unsigned char *ptDes = (unsigned char *) carte;
  for (Sint32 i = 0; i < zsize; i++)
    {
      Uint16 codem = *ptSrc;
      codem = codem << 2;
      ptDes[1] = codem;
      codem = codem >> 8;
      ptDes[0] = codem;
      ptSrc++;
      ptDes += 2;
    }


  char *fnamescore = "edmap.data";

  umask (0002);
  Sint32 fhand = open (fnamescore, O_WRONLY | O_CREAT, 00666);
  if (fhand == -1)
    {
      fprintf (stderr,
               "supervisor_map_editor::saveTheMap(): file:%s / error:%s\n",
               fnamescore, strerror (errno));
      memory->release ((char *) ptDes);
      return 0;
    }
  write (fhand, carte, msize);
  if (close (fhand) == -1)
    {
      fprintf (stderr,
               "supervisor_map_editor::saveTheMap(): file:%s / error:%s\n",
               fnamescore, strerror (errno));
      memory->release ((char *) ptDes);
      return 0;
    }
  memory->release ((char *) carte);
  printf ("supervisor_map_editor::saveTheMap() : %s file was saved\n",
          fnamescore);
  return erreur_num;
}

const unsigned char
  supervisor_map_editor::cyclingtab[] =
  { 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
  253, 254, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244,
  243, 242, 241, 240, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
  249, 250, 251, 252, 253, 254, 255
};
