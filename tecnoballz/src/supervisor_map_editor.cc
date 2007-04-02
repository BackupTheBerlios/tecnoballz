/** 
 * @file supervisor_map_editor.cc 
 * @brief The tile map editor for the menu and guardians levels 
 * @created 2004-09-13 
 * @date 2007-04-02
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.11 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_map_editor.cc,v 1.11 2007/04/02 16:27:04 gurumeditation Exp $
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
  map_width = 0;
  is_right_button_down = false;
  box_colour = 0;
  tiles_brush = NULL;
  brush_bitmap = (bitmap_data *) NULL;
  is_left_button_down = false;
  brush_posx = 0;
  brush_posy = 0;
  brush_width = 0;
  brush_height = 0;
  is_s_key_down = false;
  first_init ();
}

/**
 * Release the tile map editor
 */
supervisor_map_editor::~supervisor_map_editor ()
{
  if (NULL != mouse_pointer)
    {
       delete mouse_pointer;
       mouse_pointer = NULL;
    }
  if (NULL != tiles_map) 
    {
      delete tiles_map;
      tiles_map = NULL;
    }
  if (NULL != map_selection)
    {
      delete map_selection;
    }
  if (NULL != tiles_selection)
    {
      delete tiles_selection;
    }
  if (NULL != tiles_brush)
    {
      delete[]tiles_brush;
      tiles_brush = NULL;
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
      map_selection = new selected_region;
      tiles_selection = new selected_region;
    }
  catch (std::bad_alloc &)
  {
    std::
      cerr << "(!)supervisor_map_editor::first_init() "
      "not enough memory to allocate " <<
      " 'selected_region' structure!" << std::endl;
    throw;
  }
  current_selection = map_selection;


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
  tiles_bitmap = tiles_map->get_bitmap ();

  tile_width = tiles_map->get_tiles_width ();
  tile_mask1 = 0xffffffff ^ (tile_width - 1);
  tile_mask2 = ~tile_mask1;
  map_width = tiles_map->get_map_width();

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

  map_selection->y_offset = tiles_map->get_y_coord ();
  tiles_selection->y_offset = titlesPosy;

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
  current_selection = map_selection;
  Sint32 speed = get_speed ();
  /* draw tiles map on the screen */
  tiles_map->scrolling1 (speed); 
  select_rectangle ();
  highlight_selection ();
  draw_brush ();
}

/**
 * Create a brush from map editor
 */
void
supervisor_map_editor::map_to_brush ()
{
  if (is_verbose) 
  {
    std::cout << "supervisor_map_editor::map_to_brush() (" <<
        current_selection->x1 << ", " << current_selection->y1 << "," << 
        current_selection->x2 << ", " << current_selection->y2 << std::endl;
  }

  /*
   * Allocate memory for tiles brush
   */
  alloc_tilesmap_brush (current_selection->number_of_raws, current_selection->number_of_cols);
  Sint32 ycoord = current_selection->y1;
  ycoord = (ycoord / tiles_map->tile_height) + 0;
  ycoord *= map_width;
  ycoord += (current_selection->x1 / tiles_map->tile_width);
  Uint16 *map = tiles_map->map_tiles + ycoord;
  Uint16 *ptBrh = tiles_brush;
  for (Uint32 y = 0; y < current_selection->number_of_raws; y++)
    {
      for (Uint32 x = 0; x < current_selection->number_of_cols; x++)
        {
          *(ptBrh++) = map[x];
        }
      map += map_width;
    }
  alloc_brush ();
}

/**
 * View tiles mode
 */ 
void
supervisor_map_editor::view_tiles ()
{
  current_selection = tiles_selection;
  Sint32 speed = get_speed ();
  Sint32 y_max = tiles_bitmap->get_height () - screen_height;

  titlesPosy = titlesPosy + speed;
  if (titlesPosy < 0)
    {
      titlesPosy = 0;
    }
  else if (titlesPosy > y_max)
    {
      titlesPosy = y_max;
    }
  tiles_bitmap->copyBuffer (0, titlesPosy, 0, 0, screen_width, screen_height);
  select_rectangle ();
  highlight_selection ();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void
supervisor_map_editor::tiles_to_brush ()
{
  printf ("supervisor_map_editor::tiles_to_brush() : [%i, %i, %i, %i]\n",
          current_selection->x1, current_selection->y1,
          current_selection->x2, current_selection->y2);


  Sint32 o =
    (current_selection->y1 / tile_width) * map_width +
    (current_selection->x1 / tile_width);

  /* allocate tilesmap for the brush */
  alloc_tilesmap_brush (current_selection->number_of_raws, current_selection->number_of_cols);

  Uint16 *ptBrh = tiles_brush;
  for (Uint32 y = 0; y < current_selection->number_of_raws; y++)
    {
      Sint32 p = o;
      for (Uint32 x = 0; x < current_selection->number_of_cols; x++)
        {
          *(ptBrh++) = p;
          p++;
        }
      o += map_width;
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
        case SHOW_TILES:
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
  Uint32 mousY = keyboard->get_mouse_y ();
  if (mousY > 0 && mousY < 8 * resolution)
    {
      speed = -16 * resolution;
    }
  if (mousY >= 8 * resolution && mousY < 16 * resolution)
    {
      speed = -8 * resolution;
    }
  if (mousY >= 16 * resolution && mousY < 24 * resolution)
    {
      speed = -4 * resolution;
    }
  if (mousY <= screen_height - 16 * resolution
      && mousY > screen_height - 24 * resolution)
    {
      speed = 4 * resolution;
    }
  if (mousY <= screen_height - 8 * resolution
      && mousY > screen_height - 16 * resolution)
      {
        speed = 8 * resolution;
      }
  if (mousY < screen_height && mousY > screen_height - 8 * resolution)
      {
        speed = 16 * resolution;
      }
  return speed;
}

/**
 * Handle rectangle selection with the right mouse button
 */
void
supervisor_map_editor::select_rectangle ()
{
  /* reads coordinates if the right mouse button has just been pressed */
  bool is_right_down = keyboard->is_right_button ();
  if (is_right_down && !is_right_button_down)
    {
      is_right_button_down = true;
      current_selection->x1 = keyboard->get_mouse_x ();
      current_selection->y1 =
        current_selection->y_offset + keyboard->get_mouse_y ();
      current_selection->x1 &= tile_mask1;
      current_selection->y1 &= tile_mask1;
      if (NULL != brush_bitmap)
        {
          delete brush_bitmap;
          brush_bitmap = (bitmap_data *) NULL;
        }
    }

  if (is_right_button_down)
    {
      current_selection->x2 = keyboard->get_mouse_x ();
      current_selection->y2 =
        keyboard->get_mouse_y () + current_selection->y_offset;
      if (current_selection->x2 & tile_mask2)
	{
	  current_selection->x2 += tile_width;
	}
      if (current_selection->y2 & tile_mask2)
	{
	  current_selection->y2 += tile_width;
	}
      current_selection->x2 &= tile_mask1;
      current_selection->y2 &= tile_mask1;
      if (current_selection->x2 >= current_selection->x1 &&
          current_selection->x2 - current_selection->x1 < tile_width)
	{
          current_selection->x2 = current_selection->x1 + tile_width;
	}
      if (current_selection->x2 < current_selection->x1 &&
          current_selection->x1 - current_selection->x2 < tile_width)
	{
	  current_selection->x2 = current_selection->x1 - tile_width;
	}
      if (current_selection->y2 >= current_selection->y1 &&
          current_selection->y2 - current_selection->y1 < tile_width)
	{
	  current_selection->y2 = current_selection->y1 + tile_width;
	}
      if (current_selection->y2 < current_selection->y1 &&
          current_selection->y1 - current_selection->y2 < tile_width)
	{
	  current_selection->x2 = current_selection->x1 - tile_width;
	}
      if (current_selection->y1 < current_selection->y2
          && current_selection->y2 - current_selection->y1 >
          (screen_height / 2))
        current_selection->y2 = current_selection->y1 + (screen_height / 2);
      if (current_selection->y1 > current_selection->y2
          && current_selection->y1 - current_selection->y2 >
          (screen_height / 2))
        current_selection->y2 = current_selection->y1 - (screen_height / 2);
    }

  if (is_right_button_down && is_right_down)
    {
      current_selection->box_typeID = 2;
    }
  else
    {
      /* right mouse button relased */
      if (!is_right_down && is_right_button_down)
        {
          is_right_button_down = false;
          current_selection->box_typeID = 1;
          printf ("supervisor_map_editor::select_rectangle() / relased\n");

          if (current_selection->x1 > current_selection->x2)
            {
              Sint32 x = current_selection->x1;
              current_selection->x1 = current_selection->x2;
              current_selection->x2 = x;
            }
          if (current_selection->y1 > current_selection->y2)
            {
              Sint32 y = current_selection->y1;
              current_selection->y1 = current_selection->y2;
              current_selection->y2 = y;
            }


          current_selection->number_of_cols =
            (current_selection->x2 - current_selection->x1) / tile_width;
          current_selection->number_of_raws =
            (current_selection->y2 - current_selection->y1) / tile_width;


          switch (view_mode)
            {
            case SHOW_TILES:
              tiles_to_brush ();
              break;
            case SHOW_MAP:
            default:
              map_to_brush ();
              break;
            }

        }
    }

}

/**
 * Draw highlighting selection
 */
void
supervisor_map_editor::highlight_selection ()
{
  char *pBuff;
  Sint32 tmpco = 0;

  if (current_selection->x2 == current_selection->x1 ||
      current_selection->y2 == current_selection->y1)
    {
      current_selection->box_typeID = 0;
    }
  if (0 == current_selection->box_typeID)
    {
      return;
    }

  Uint32 x1 = current_selection->x1;
  Uint32 x2 = current_selection->x2;
  Uint32 y1 = current_selection->y1 - current_selection->y_offset;
  Uint32 y2 = current_selection->y2 - current_selection->y_offset;

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


  //char *ptBuf = game_screen->get_pixel_data(x1, y2);

  Sint32 width = x2 - x1;
  Sint32 heigh = y2 - y1;

  /*printf("supervisor_map_editor::highlight_selection() : [%i, %i, %i, %i]\n", 
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
 * Allocate a tilesmap brush of the required size
 * @param number_of_raws number of raws of tiles
 * @param number_of_cols number of cols of tiles
 */
void
supervisor_map_editor::alloc_tilesmap_brush (Uint32 number_of_raws, Uint32 number_of_cols)
{
  if (NULL != tiles_brush)
    {
      delete[]tiles_brush;
    }
  Uint32 size = number_of_raws * number_of_cols;
  try
    {
      tiles_brush = new Uint16[size];
    }
  catch (std::bad_alloc &)
  {
    std::
      cerr << "(!)supervisor_map_editor::alloc_tilesmap_brush() "
      "not enough memory to allocate " <<
       size << " Uint16!" << std::endl;
    throw;
  }
}

/**
 * Allocate a bitmap brush of the required size
 */
void
supervisor_map_editor::alloc_brush ()
{
  if (NULL != brush_bitmap)
    {
      delete brush_bitmap;
    }
  brush_bitmap = tiles_map->alloc_brush (tiles_brush, current_selection->number_of_cols, current_selection->number_of_raws);
  brush_width = current_selection->number_of_cols;
  brush_height = current_selection->number_of_raws;
  return;
}

/**
 * Draw the current brush
 */
void
supervisor_map_editor::draw_brush ()
{
  if (NULL == brush_bitmap)
    {
      return;
    }
  Sint32 pos_x = keyboard->get_mouse_x ();
  Sint32 pos_y = keyboard->get_mouse_y ();
  pos_x &= tile_mask1;
  pos_y &= tile_mask1;
  if (pos_x > screen_width - brush_bitmap->get_width ())
    {
      pos_x = screen_width - brush_bitmap->get_width ();
    }
  if (pos_y > screen_height - brush_bitmap->get_height ())
    {
      pos_y = screen_height - brush_bitmap->get_height ();
    }
  Sint32 scrlY = tiles_map->get_y_coord ();
  bool is_left_down = keyboard->is_left_button ();
  if (is_left_down && !is_left_button_down)
    {
      is_left_button_down = true;
      brush_posx = pos_x;
      brush_posy = pos_y;
    }
  if (is_left_down && is_left_button_down)
    {
      pos_x = brush_posx;
      pos_y = brush_posy;
    }
  else
    {
      if (!is_left_down && is_left_button_down)
        {
          is_left_button_down = false;
          printf
            ("supervisor_map_editor::draw_brush() : brush_posx:%i / brush_posy:%i\n",
             brush_posx, brush_posy);


          Sint32 i = scrlY + brush_posy;


          printf
            ("supervisor_map_editor::draw_brush() : scrlY:%i /  i :%i motifhaute:%i\n",
             scrlY, i, tiles_map->tile_height);

          i = (i / tiles_map->tile_height) + 0;
          i *= map_width;
          Uint16 *brush = tiles_brush;
          Uint16 *table = tiles_map->map_tiles + i;
          Uint16 *t_end =
            tiles_map->map_tiles +
            (tilesmap_scrolling::MAP_HEIGHT * map_width);
          table += (brush_posx / tiles_map->tile_width);

          printf
            ("supervisor_map_editor::draw_brush() : (table - carteFirst):%i /  i:%i\n",
             (table - tiles_map->map_tiles), i);


          for (i = 0; i < brush_height; i++)
            {
              if (table > t_end)
		{
                  table -=
                    (tilesmap_scrolling::MAP_HEIGHT * map_width);
		}

              for (Uint32 j = 0; j < brush_width; j++)
		{
                  table[j] = *(brush++);
		}
              table += map_width;
            }
          //###################################################################
          // copy a height of the screen (for scrolling rotation)
          //###################################################################
          table = tiles_map->map_tiles;
          i =
            (tilesmap_scrolling::MAP_HEIGHT * map_width);
          Sint32 tsupp =
            (display->get_height () / tiles_map->tile_height) * 2;
          for (Uint32 j = 0; j < (tsupp * map_width); j++)
            tiles_map->map_tiles[i++] = table[j];
        }
    }
  brush_bitmap->copyBuffer (0, 0, pos_x, pos_y - (scrlY & tile_mask2), -1, -1);
}

Sint32
supervisor_map_editor::saveTheMap ()
{
  Sint32 zsize =
    tilesmap_scrolling::MAP_HEIGHT * map_width;

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
