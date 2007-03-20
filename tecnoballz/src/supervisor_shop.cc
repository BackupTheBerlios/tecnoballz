/**
 * @file supervisor_shop.cc 
 * @brief Shop supervisor
 * @date 2007-03-20
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.35 $
 */
/*
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_shop.cc,v 1.35 2007/03/20 22:53:16 gurumeditation Exp $
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
#include "../include/supervisor_shop.h"
#include "../include/handler_resources.h"
#include "../include/supervisor_main_menu.h"
#include "../include/sprite_display_menu.h"

/**
 * Create the shop supervisor
 */
supervisor_shop::supervisor_shop ()
{
  initialise ();
  tiles_ground = new tiles_background ();
  mouse_pointer = new sprite_mouse_pointer ();
  led_indicator = new sprite_object ();
  power_up_capsules = controller_capsules::get_instance ();
  display_text = new display_text_bitmap ();
  popup_menu = new handler_popup_menu ();
  shop_point = 0;
  optioninfo = 0;
  is_already_view_info = false;
  current_price = 0;
  num_of_bought_capsules = 0;
  mouse_x_coord = 0;
  mouse_y_coord = 0;
  get_object = -1;
  pt_get_obj = 0;
  shop_point = 0;
  shoppoint3 = 0;
  blink_capsule = (sprite_capsule *) NULL;

  shop_xmini = 6 * resolution;
  shop_xmaxi = 283 * resolution;
  shop_xmax2 = 51 * resolution;
  shop_ymax1 = 173 * resolution;
  shop_ymax2 = 218 * resolution;

  cadre_offs = 0;
  cadre_flag = 0;
  cadre_ymin = 3 * resolution;
  cadre_ymax = 171 * resolution + cadre_ymin;
  cadre_posx = 292 * resolution;
  ;
  cadre_posy = 0;
  cadre_larg = 21 * resolution;
  cadre_haut = 9 * resolution;
  box_colour = 0;
  angleValue = 0;

  previous_key_code_down = 0;   //last key pressed
  triche_etb = 0;               //input (triche_etb == cheat_code then is_enabled_cheat_mode = 1)
  cheat_code = SDLK_e << 24 | SDLK_t << 16 | SDLK_b << 8 | SDLK_RETURN;
  box_texts = NULL;
}

/**
 * Release the shop supervisor
 */
supervisor_shop::~supervisor_shop ()
{
  delete popup_menu;
  delete display_text;
  delete power_up_capsules;
  if (NULL != led_indicator)
    {
      delete led_indicator;
      led_indicator = (sprite_object *) NULL;
    }
  if (NULL != box_texts)
    {
      delete[](char *) box_texts;
      box_texts = NULL;
    }
  delete mouse_pointer;
  delete tiles_ground;
  liberation ();
}

/**
 * Initialize the shop supervisor
 */
Sint32 supervisor_shop::first_init ()
{

  box_texts =
    resources->load_texts (handler_resources::TEXTS_SHOP, 0,
                           BOX_LENGTH_STRING, 3);
  for (Uint32 i = 0; i < 36; i++)
    {
      printf ("%02d): %s \n", i, box_texts[i]);
    }


  Uint32
  area_num = current_player->get_area_number ();
  Uint32
  level_num = current_player->get_level_number ();
#ifndef SOUNDISOFF

  audio->play_level_music (area_num, level_num);
  audio->play_shop_music (area_num);
#endif

  sprites->reset ();

  /* copy name player into menu text */
  display_text->print_to_string (current_player->get_name (),
                                 box_texts[TEXT_WELCOME]);
  char *
  ptDes;
  const char *
  ptSrc;
  ptDes = current_player->get_name ();
  for (Uint32 i = 0; i < 6; i++)
    {
      shoptext00[8 + i] = ptDes[i];
    }
  //intToASCII (MAX_OF_CAPSULES_BOUGHT, &shoptext63[48], 1);
  //intToASCII (current_player->get_num_of_lifes (),
  //            &info_text1[BOX_LENGTH_STRING * 4 + 5], 1);

  display_text->print_int_to_string (current_player->get_num_of_lifes (),
                                     2, box_texts[TEXT_LIVES_LEFT]);
  display_text->print_int_to_string (MAX_OF_CAPSULES_BOUGHT, 2,
                                     box_texts[TEXT_CANNOT_BUY_MORE]);


  if (area_num > 1)
    {
      const char *
      pPass =
        supervisor_main_menu::get_area_code (area_num, difficulty_level);
      ptDes = &info_text3[1 * BOX_LENGTH_STRING + 10];
      for (Sint32 i = 0; i < 10; i++)
        {
          ptDes[i] = pPass[i];
        }
      ptSrc = &info_text3[0];
    }
  else
    {
      ptSrc = &info_text3[BOX_LENGTH_STRING * 2];
    }
  ptDes = &info_text1[6 * BOX_LENGTH_STRING];
  for (Uint32 i = 0; i < (BOX_LENGTH_STRING * 2); i++)
    {
      ptDes[i] = ptSrc[i];
    }


  ptSrc = &sprite_display_menu::difficulte[(difficulty_level - 1) * 4];
  ptDes = &info_text1[8 * BOX_LENGTH_STRING + 16];
  for (Sint32 i = 0; i < 4; i++)
    ptDes[i] = ptSrc[i];

  resources->load_sprites_bitmap ();

  /* initialize the led indicator */
  if (resolution == 1)
    {
      led_indicator->create_sprite (BOB_LEDSHP, sprites_bitmap, false);
    }
  else
    {
      led_indicator->create_sprite (BOB_LEDSH2, sprites_bitmap, false);
    }
  sprites->add
  (led_indicator);
  led_indicator->enable ();

  /* initialize the power-up capsules  */
  power_up_capsules->create_shop_sprites_list ();
  current_player->clear_shopping_cart ();
  Sint32 *
  tp = coursetemp;
  for (Uint32 i = 0; i < MAX_OF_CAPSULES_BOUGHT; i++)
    {
      *(tp++) = 0;
    }
  sprite_capsule **
  capsules = power_up_capsules->get_sprites_list ();
  drag_sprite = capsules[(MAX_OF_CAPSULES_BOUGHT + 2) - 1 - 1];

  /* initialize the mouse pointer */
  mouse_pointer->create_pointer_sprite (sprites_bitmap);

  //###################################################################
  // intialize the "escape menu"
  //###################################################################
  popup_menu->first_init (sprites_bitmap, 1,    //menu number
                          320 * resolution,     //width of screen (center)
                          1,    //restaure background where leaves
                          1     //initialize color table
                         );

  resources->release_sprites_bitmap ();
  display_text->initialize ();

  /* load bitmap background of the shop */
  bitmap_data *
  bmp = new bitmap_data ();
  bmp->load (handler_resources::BITMAP_SHOP);
  background_screen->blit_surface (bmp, 0, 0, 0, 0, bmp->get_width (),
                                   bmp->get_height ());
  delete
  bmp;
  background_screen->blit_to_surface (game_screen);
  display->bufferCopy ();       //copy buffer memory into the screen


  putthetext (box_texts[TEXT_WELCOME]);
  if (!current_player->is_budget_prices ())
    {
      char *
      str = box_texts[TEXT_WELCOME] + BOX_LENGTH_STRING * 2;
      for (Uint32 i = 0; i < BOX_LENGTH_STRING; i++)
        {
          str[i] = ' ';
        }
    }

  keyboard->set_grab_input (false);
  tiles_ground->set_4_color_palette ();
  return erreur_num;
}

/**
 * The main loop of the shop
 */
Sint32 supervisor_shop::main_loop ()
{
  Sint32
  Ecode = -1;
  display->wait_frame ();

  /* copy the background offscreen to the game offscreen */
  background_screen->blit_to_surface (game_screen, 290 * resolution,
                                      3 * resolution, 26 * resolution,
                                      180 * resolution);


  /* display the 3 lines of text  */
  display_box_text ();

  display_text->draw (game_screen, 263 * resolution, 227 * resolution,
                      current_price, 6);
  display_text->draw (game_screen, 263 * resolution, 183 * resolution,
                      current_player->get_money_amount (), 6);



  display->lock_surfaces ();

  /* display current price and credit */
  /*
     display_text->bufferAff1 (263 * resolution, 227 * resolution,
     current_price, 100000);
     display_text->bufferAff1 (263 * resolution, 183 * resolution,
     current_player->get_money_amount (), 100000);
   */





  end_return = 0;
  sprites->clear ();


  if (!popup_menu->is_enable ())
    {
      pos_select ();
      Sint32
      x = mouse_pointer->get_x_coord ();
      Sint32
      y = mouse_pointer->get_y_coord ();
      if (get_object == -1)     //-1 = not a drag object
        {
          Sint32
          x2,
          y2;
          bool
          mousreleas = keyboard->is_left_button_up (&x2, &y2);
          if (mousreleas)
            {
              Sint32
              shoppoint2 = testkursor (x, y);
              if (shoppoint2 == shop_point)
                {
                  Sint32
                  gadnu = case_types[shoppoint2];
                  faitcourse (gadnu);
                }
            }
          else
            {
              if (!keyboard->is_left_button ())
                {
                  shop_point = testkursor (x, y);
                  current_price = led_moving (shop_point);
                  if (shoppoint3 != shop_point)
                    shoppoint3 = shop_point;
                }
            }

        }
      else
        current_price = led_moving (-1);
      sh_ballade ();
    }

  mouse_pointer->move ();
  if (is_enabled_cheat_mode)
    {
      power_up_capsules->play_animation_in_shop (2);
    }
  else
    {
      power_up_capsules->play_animation_in_shop (1);
    }

  /* display the cursor of the bonus selected in the list on the right */
  draw_select_cursor ();
  sprites->draw ();
  display_capsules_bought ();
  Ecode = popup_menu->execution1 ();

  //###################################################################
  // copy buffer surface to screen surface
  //###################################################################
  display->unlock_surfaces ();
  display->bufferCTab ();

  //###################################################################
  // escape key to quit the game !
  //###################################################################
  if (keyboard->command_is_pressed (handler_keyboard::TOEXITFLAG) ||
      Ecode == handler_popup_menu::WEQUITGAME)
    {
      end_return = LEAVE_TECNOBALLZ;
    }
  if (keyboard->command_is_pressed (handler_keyboard::TOMENUFLAG) ||
      Ecode == handler_popup_menu::EXITTOMENU)
    {
      end_return = MAIN_MENU;
    }

  check_if_enable_cheat ();
  return end_return;
}

/**
 * Display list bonus capsule(s) bought in the shop,
 * on the right of the screen
 */
void
supervisor_shop::display_capsules_bought ()
{
  Sint32 *cart = current_player->get_shopping_cart ();
  sprite_capsule **capsules = power_up_capsules->get_sprites_list ();
  Sint32 pos_y = 4 * resolution;
  for (Uint32 i = 0; i < MAX_OF_CAPSULES_BOUGHT; i++)
    {
      sprite_capsule *capsule = *(capsules++);
      capsule->set_coordinates (294 * resolution, pos_y);
      pos_y = pos_y + 9 * resolution;
      Sint32 id = *(cart++);
      capsule->set_in_shop (id);
    }
}

//------------------------------------------------------------------------------
// return index of the selected bonus, and calculate position of LED indicator
//      input   =>      x: abscissa of the mouse pointer
//              =>      y: ordinate of the mouse pointer
//      output  <=      index: 0 to 24 (-1: disable)
//------------------------------------------------------------------------------
Sint32 supervisor_shop::testkursor (Sint32 x, Sint32 y)
{
  if (x < shop_xmini || x > shop_xmaxi || y > shop_ymax2 ||
      (x > shop_xmax2 && y > shop_ymax1))
    return -1;
  else
    {
      x = (x - (6 * resolution)) / (48 * resolution);
      y = (y / (44 * resolution));
      if (y > 4)
        x = 0;
      Sint32
      i = x + 6 * y;
      mouse_x_coord = (x * (48 * resolution)) + (17 * resolution);

      if (resolution == 1)
        mouse_y_coord = (y * (44 * resolution)) + (36 * resolution);
      else
        mouse_y_coord = (y * (44 * resolution)) + (35 * resolution);
      return i;
    }
}

/**
 * set LED indicator, and gadget indicator
 * @param index index of the selected bonus; 0 to 24 (-1: disable)
      input   =>      index: index of the selected bonus; 0 to 24 (-1: disable)
      output  <=      price: price of the selected bonus 
*/
Sint32 supervisor_shop::led_moving (Sint32 index)
{
  if (index < 0)
    {
      led_indicator->disable ();
      Sint32
      i = 0;
      power_up_capsules->set_overview_capsule (0);
      return i;
    }
  else
    {
      // set gadget indicator
      Sint32
      i = case_types[index];
      power_up_capsules->set_overview_capsule (i);

      /* set LED indicator */
      led_indicator->enable ();
      led_indicator->set_coordinates (mouse_x_coord, mouse_y_coord);
      if (current_player->is_budget_prices ())
        {
          i = 1;
        }
      else
        {
          i = options_prices[index];
        }

      // info already seen at least once ?
      if (index == 10 && is_already_view_info)
        {
          i = 0;
        }

      return i;
    }
}

/**
 * Bought a bonus capsule or an option
 * @param gadnu option number
 */
void
supervisor_shop::faitcourse (Sint32 gadnu)
{
  if (gadnu != GAD_INFORM)
    optioninfo = 0;

  switch (gadnu)
    {

      //###############################################################
      // expand bumper
      //###############################################################
    case GAD_SIZE_P:
      achete_gad (gadnu);
      break;

      //###############################################################
      // fire power 1
      //###############################################################
    case GAD_FIRE01:
      achete_gad (gadnu);
      break;

      //###############################################################
      // fire power 2
      //###############################################################
    case GAD_FIRE02:
      achete_gad (gadnu);
      break;

      //###############################################################
      // extra balls
      //###############################################################
    case GAD_BALLE2:
      achete_gad (gadnu);
      break;

      //###############################################################
      // multi balls
      //###############################################################
    case GAD_BALLE3:
      achete_gad (gadnu);
      break;

      //###############################################################
      // power ball 1
      //###############################################################
    case GAD_POWER1:
      achete_gad (gadnu);
      break;

      //###############################################################
      // power ball 2
      //###############################################################
    case GAD_POWER2:
      achete_gad (gadnu);
      break;

      //###############################################################
      // extra life
      //###############################################################
    case GAD_LIFE_P:
      achete_gad (gadnu);
      break;

      //###############################################################
      // extra life
      //###############################################################
    case GAD_WALL01:
      achete_gad (gadnu);
      break;

      //###############################################################
      // ball size 2
      //###############################################################
    case GAD_SIZE01:
      achete_gad (gadnu);
      break;

      //###############################################################
      // ball size 3
      //###############################################################
    case GAD_SIZE02:
      achete_gad (gadnu);
      break;

      //###############################################################
      // robot bumper option
      //###############################################################
    case GAD_ROBOT1:
      achete_gad (gadnu);
      break;

      //###############################################################
      // balls control option
      //###############################################################
    case GAD_CONTRO:
      achete_gad (gadnu);
      break;

      //###############################################################
      // glue option
      //###############################################################
    case GAD_GLUE00:
      achete_gad (gadnu);
      break;

      //###############################################################
      // somes infos
      //###############################################################
    case GAD_INFORM:
      {
        display_info ();
        return;

        Sint32 arean = current_player->get_area_number ();
        char *ptSrc = &info_text2[0];
        char *ptDes = &info_text1[0];
        for (Sint32 i = 2; i <= 4; i++)
          {
            char *ptTxt = ptSrc;
            if (current_player->get_paddle_alive_counter (i) <= 0)
              {
                ptTxt += BOX_LENGTH_STRING;
                if (arean >= 2 && i == 3)
                  ptTxt += BOX_LENGTH_STRING;
                if (arean >= 3 && i == 2)
                  ptTxt += BOX_LENGTH_STRING;
                if (arean >= 4 && i == 4)
                  ptTxt += BOX_LENGTH_STRING;
                //area 2 => bumper 3
                //area 3 => bumper 3 & 2
                //area 4 & 5 => bumper 3 & 2 & 4
              }
            for (Uint32 j = 0; j < BOX_LENGTH_STRING; j++)
              {
                *(ptDes++) = *(ptTxt++);
              }
            ptSrc += BOX_LENGTH_STRING * 3;
          }


        if (!is_already_view_info)
          {
            if (!decrease_money_amount ())
              {
                break;
              }
          }
        is_already_view_info = true;
        if (optioninfo < 3)
          putthetext (&info_text1[optioninfo * BOX_LENGTH_STRING * 3]);
        else
          {
            if (is_enabled_cheat_mode)
              putthetext (&info_text1
                          [(optioninfo + 2) * BOX_LENGTH_STRING * 3]);
            else
              {
                if (birth_flag)
                  putthetext (&info_text1
                              [(optioninfo + 1) * BOX_LENGTH_STRING * 3]);
                else
                  putthetext (&info_text1
                              [optioninfo * BOX_LENGTH_STRING * 3]);
              }
          }
        optioninfo++;
        optioninfo &= 3;
      }
      break;

      /* rebuild the wall */
    case GAD_REBUIL:
      if (current_player->get_area_number () < 5)
        {
          putthetext (box_texts[TEXT_ONLY_FOR_AREA_5]);
        }
      else
        {
          if (!current_player->is_rebuild_walls ())
            {
              if (decrease_money_amount ())
                {
                  current_player->set_rebuild_walls (true);
                  message_ok ();
                }
            }
        }
      break;

      /* less bricks option */
    case GAD_LESSBR:
      if (current_player->get_less_bricks () <= 0)
        {
          if (decrease_money_amount ())
            {
              current_player->set_less_bricks (10);
              message_ok ();
            }
        }
      break;

      /* exit from the shop */
    case GAD_EXITSH:
      current_player->set_budget_prices (false);
      current_player =
        handler_players::nextplayer (current_player, &end_return, 2);
      break;

    default:
      {
        //############################################################
        // buy a right, up or left bumper
        //############################################################
        if (gadnu >= GAD_BUMP02 && gadnu <= GAD_BUMP04)
          {
            Sint32 i = (gadnu - GAD_BUMP02) / 2 + 2;
            if (current_player->get_paddle_alive_counter (i) <= 0)
              {
                if (decrease_money_amount ())
                  {
                    current_player->set_paddle_alive_counter (i, 3);
                    message_ok ();
                  }
              }
          }
      }
      break;
    }
}

/**
 * Display info message
 */
void
supervisor_shop::display_info ()
{
  if (!is_already_view_info)
    {
      if (!decrease_money_amount ())
        {
          return;
        }
    }
  is_already_view_info = true;

  switch (optioninfo)
    {
    case INFO_PADDLES:
      {
        Uint32 area_num = current_player->get_area_number ();
        Uint32 index = TEXT_PADDLE_RIGHT;
        Uint32 line = 0;
        char *dest = box_texts[TEXT_PADDLES];
        char *src;
        for (Uint32 i = controller_paddles::RIGHT_PADDLE;
             i <= controller_paddles::LEFT_PADDLE; i++)
          {
            src = box_texts[index];
            if (current_player->get_paddle_alive_counter (i) == 0)
              {
                src += BOX_LENGTH_STRING;
                if ((area_num >= 2 && i == controller_paddles::TOP_PADDLE) ||
                    (area_num >= 3 && i == controller_paddles::RIGHT_PADDLE)
                    || (area_num >= 4
                        && i == controller_paddles::LEFT_PADDLE))
                  {
                    src += BOX_LENGTH_STRING;
                  }
              }
            index++;
            line++;
            strncpy (dest, src, BOX_LENGTH_STRING);
            dest += BOX_LENGTH_STRING;
          }
      }
      putthetext (box_texts[TEXT_PADDLES]);
      break;

    case INFO_LIVES:
      putthetext (box_texts[TEXT_LIVES_LEFT]);
      break;

    case INFO_AREA_CODE:
      if (current_player->get_area_number () > 1)
        {
          putthetext (box_texts[TEXT_AREA_CODE]);
        }
      else
        {
          putthetext (box_texts[TEXT_NO_AREA_CODE]);
        }
      break;
    case INFO_END:
    default:
      if (is_enabled_cheat_mode)
        {
          putthetext (box_texts[TEXT_ENABLED_CHEAT_MODE]);
        }
      else if (birth_flag)
        {
          putthetext (box_texts[TEXT_WAITING_CHEAT_MODE]);
        }
      else
        {
          putthetext (box_texts[TEXT_HOPING_HELP]);
        }
      break;
    }
  if (++optioninfo > INFO_END)
    {
      optioninfo = 0;
    }

}

/**
 * Check if a purchase is possible, so decrement the money amount
 * @return true if the money amount could be descreased
 */
bool supervisor_shop::decrease_money_amount ()
{
  if (current_player->decrease_money_amount (current_price))
    {
      return true;
    }
  else
    {
      putthetext (box_texts[TEXT_NOT_ENOUGH_MONEY]);
      return false;
    }
}

/**
 * Purchase a bonus capsule if possible
 */
void
supervisor_shop::achete_gad (Sint32 gadnb)
{
  /* maximum number of capsules reached */
  if (num_of_bought_capsules >= MAX_OF_CAPSULES_BOUGHT)
    {
      putthetext (box_texts[TEXT_CANNOT_BUY_MORE]);
      return;
    }

  //  purchase is possible ?
  if (!decrease_money_amount ())
    {
      return;
    }

  Sint32 *p = current_player->get_shopping_cart ();
  p[num_of_bought_capsules] = gadnb;
  sh_tablept[num_of_bought_capsules] = shop_point;
  sprite_capsule **liste = power_up_capsules->get_sprites_list ();
  sprite_capsule *gadgt = liste[num_of_bought_capsules++];
  gadgt->set_in_shop (gadnb);
  message_ok ();
  current_player->set_cou_nb (num_of_bought_capsules);
}

//------------------------------------------------------------------------------
// change the 3 lines of the text
//------------------------------------------------------------------------------
void
supervisor_shop::message_ok ()
{
  //Sint32 i = shoptextPT[shop_point] * BOX_LENGTH_STRING * 3;
  //char *texte = shoptext12 + i;
  //Sint32 i = shoptextPT[shop_point] * BOX_LENGTH_STRING * 3;
  char *texte = box_texts[shoptextPT[shop_point]];

  putthetext (texte);
}


//------------------------------------------------------------------------------
// pointe les trois lignes a afficher
//------------------------------------------------------------------------------
void
supervisor_shop::putthetext (char *ligne)
{
  text_lines[0] = ligne;
  ligne += BOX_LENGTH_STRING;
  text_lines[1] = ligne;
  ligne += BOX_LENGTH_STRING;
  text_lines[2] = ligne;
}

/**
 * Display the three lines of text in the box in bottom
 */
void
supervisor_shop::display_box_text ()
{
  Uint32 height = display_text->get_char_height ();
  Uint32 x_pos = 60 * resolution;
  Uint32 y_pos = 180 * resolution;
  Uint32 yspac = height + resolution;
  game_screen->clear (0, x_pos, y_pos, 22 * 8 * resolution, 3 * yspac);
  display_text->draw (game_screen, x_pos, y_pos, text_lines[0],
                      BOX_LENGTH_STRING);
  display_text->draw (game_screen, x_pos, y_pos + yspac, text_lines[1],
                      BOX_LENGTH_STRING);
  display_text->draw (game_screen, x_pos, y_pos + yspac * 2, text_lines[2],
                      BOX_LENGTH_STRING);
}

//-------------------------------------------------------------------------------
// drag and drop the gadget
//-------------------------------------------------------------------------------
void
supervisor_shop::sh_ballade ()
{
  if (get_object >= 0)          //pointer to the table "options_prices" (-1 = no drag object)
    {
      if (keyboard->is_left_button ())
        {
          drag_sprite->enable ();
          drag_sprite->set_coordinates (mouse_pointer->get_x_coord (),
                                        mouse_pointer->get_y_coord ());
          if (blink_capsule->is_enabled)
            blink_capsule->is_enabled = 0;
          else
            blink_capsule->is_enabled = 1;
        }
      else
        {


          drag_sprite->disable ();
          blink_capsule->is_enabled = 1;
          Sint32 i = cadre_offs;
          if (i >= 0)
            {
              if (i >= num_of_bought_capsules)
                {
                  i = num_of_bought_capsules - 1;
                }
              Sint32 *p0 = current_player->get_shopping_cart ();
              Sint32 *p1 = courseList;  //source
              Sint32 *p2 = p0 + i;      //destination
              Sint32 *tp = coursetemp;
              Sint32 value = *p1;       //source code
              if (p1 != p2)
                {
                  Sint32 i = 0;
                  do
                    {
                      if (p0 == p1)     //emplacement source ?
                        {
                          p0++;
                          i++;
                        }
                      else
                        {
                          if (p0 == p2) //emplacement destination ?
                            {
                              if (p1 <= p2)     // source <= destination
                                {
                                  if (i++ < MAX_OF_CAPSULES_BOUGHT)
                                    {
                                      *(tp++) = *(p0++);
                                    }
                                }
                              if (i++ < MAX_OF_CAPSULES_BOUGHT)
                                {
                                  *(tp++) = value;
                                }
                            }
                          if (i++ < MAX_OF_CAPSULES_BOUGHT)
                            {
                              *(tp++) = *(p0++);
                            }
                        }
                    }
                  while (i < MAX_OF_CAPSULES_BOUGHT);
                  tp = coursetemp;
                  p0 = current_player->get_shopping_cart ();
                  for (Uint32 i = 0; i < MAX_OF_CAPSULES_BOUGHT; i++)
                    {
                      *(p0++) = *(tp++);
                    }
                }
            }
          else
            {
              Sint32 *p1 = courseList;
              Sint32 *p2 = p1;
              Sint32 gadnu = *p1;       //drop bonus code
              *(p1++) = 0;
              while (*p1 >= 0)
                {
                  *(p2++) = *p1;
                  *(p1++) = 0;
                }
              num_of_bought_capsules--;
              current_player->set_cou_nb (num_of_bought_capsules);

              Sint32 price = 0;
              if (current_player->is_budget_prices ())
                {
                  /* the player collected a chance capsule containing
                   * a budget prices bonus in the previous bricks level.
                   * All the options are thus for the price of 1 in 
                   * the current shop */
                  price = 1;
                }
              else
                {
                  while (case_types[i] != GAD_EXITSH)
                    {
                      if (case_types[i] == gadnu)
                        {
                          price = options_prices[i];
                          break;
                        }
                      i++;
                    }
                }
              current_player->increase_money_amount (price);
            }
          get_object = -1;
        }
    }

  //###################################################################
  // get a object with the mouse
  //###################################################################
  else
    {
      drag_sprite->disable ();
      pt_get_obj = -1;
      if (keyboard->is_left_button ())
        {
          Sint32 i = cadre_offs;
          if (i >= 0)
            {
              Sint32 *p = current_player->get_shopping_cart ();
              sprite_capsule **capsules =
                power_up_capsules->get_sprites_list ();
              courseList = p + i;
              blink_capsule = *(capsules + i);
              get_object = *(sh_tablept + i);
              drag_sprite->clone_from_capsule (blink_capsule);
              blink_capsule->is_enabled = true;
            }
        }
    }
}

//-------------------------------------------------------------------------------
// calculate position of box cursor
//-------------------------------------------------------------------------------
void
supervisor_shop::pos_select ()
{
  Sint32 y = mouse_pointer->get_y_coord () - cadre_ymin;
  Sint32 o = (y / cadre_haut);  //y / 9 (height of cursor)
  y = o * cadre_haut + cadre_ymin;
  if (y < cadre_ymin)
    y = cadre_ymin;
  if (y > cadre_ymax)
    y = cadre_ymax;
  cadre_posy = y;               //cursor y coordinate

  Sint32 x = mouse_pointer->get_x_coord ();
  y = mouse_pointer->get_y_coord ();
  cadre_flag = 0;               //don't display cursor (by default)
  cadre_offs = -1;

  /* at least one bonus capsule? */
  if (num_of_bought_capsules < 1)
    {
      return;
    }

  // calculate max. y coordinate
  Sint32 y2 = num_of_bought_capsules * cadre_haut + cadre_ymin;

  // drag object ? then add a possible position, all in bottom.
  if (get_object >= 0)
    y2 = y2 + cadre_haut;

  if (x >= cadre_posx && y < y2)
    {
      cadre_flag = 1;
      cadre_offs = o;
    }
}

/**
 * Draw the cursor of the bonus selected in the list on the right
 */
void
supervisor_shop::draw_select_cursor ()
{
  if (box_colour++ > 32)
    {
      box_colour = 0;
    }

  Sint32 a = angleValue + 5;
  a &= 511;
  angleValue = a;
  if (cadre_flag > 0)
    {
      Sint16 *s = table_cosL + a;
      a = *s;
      a *= (cadre_larg / 5);
      a >>= 7;
      a = a + cadre_larg / 5;
      Sint32 b = angleValue;
      s = table_cosL + b;
      b = *s;
      b *= (cadre_haut / 5);
      b >>= 7;
      b = b + cadre_haut / 5;
      Sint32 l = cadre_larg - a;
      Sint32 h = cadre_haut - b;
      Sint32 x = cadre_posx + (cadre_larg - l) / 2;
      Sint32 y = cadre_posy + (cadre_haut - h) / 2;

      Sint32 tmpco = 0;
      Sint32 color = box_colour;
      char *pBuff = game_screen->get_pixel_data (x, y);
      Sint32 nextl = game_screen->get_row_size ();
      for (Sint32 v = 0; v < h; v++)
        {
          for (Sint32 h = 0; h < l; h++)
            {
              unsigned char pixel = cyclingtab[color];
              pBuff[h] = pixel;
            }
          if (++tmpco == 5)
            {
              tmpco = 0;
              if (color++ > 32)
                color = 0;
            }
          pBuff += nextl;
        }
    }
}

/**
 * Check if the player enables the cheating mode
 */
void
supervisor_shop::check_if_enable_cheat ()
{
  mouse_pointer->set_frame_period (3);
  if (is_enabled_cheat_mode)
    {
      /* the cheating mode is already enabled */
      return;
    }
  if (!birth_flag)
    {
      return;
    }
  if (!mouse_pointer->get_x_coord () && !mouse_pointer->get_y_coord ())
    {
      mouse_pointer->set_frame_period (20);
      Uint32 code = keyboard->get_key_down_code ();
      if (previous_key_code_down != code && code > 0)
        {
          previous_key_code_down = code;
          triche_etb = triche_etb << 8 | code;
        }
    }
  if (triche_etb == cheat_code)
    {
      is_enabled_cheat_mode = true;
    }
  else
    {
      is_enabled_cheat_mode = false;
    }
}

//==============================================================================
// statics data
//==============================================================================

//-------------------------------------------------------------------------------
// Contenu de chaque case
//-------------------------------------------------------------------------------
Sint32 supervisor_shop::case_types[] =
  {
    GAD_SIZE_P, GAD_FIRE01, GAD_FIRE02, GAD_REBUIL, GAD_BALLE2, GAD_BALLE3,
    GAD_POWER1, GAD_POWER2, GAD_LESSBR, GAD_LIFE_P, GAD_INFORM, GAD_WALL01,
    GAD_BUMP04, GAD_BUMP03, GAD_BUMP02, GAD_SIZE01, GAD_SIZE02, GAD_ROBOT1,
    GAD_CONTRO, GAD_GLUE00, GAD_EXITSH, GAD_EXITSH, GAD_EXITSH, GAD_EXITSH,
    GAD_EXITSH, GAD_EXITSH, GAD_EXITSH, GAD_EXITSH, GAD_EXITSH, GAD_EXITSH};

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
Sint32
supervisor_shop::sh_tablept[MAX_OF_CAPSULES_BOUGHT];

/**
 * Prices of all the available options in the shop 
 */
Uint32
supervisor_shop::options_prices[] = {
                                      60, 75, 150, 350, 25, 50,
                                      250, 500, 400, 450, 10, 75,
                                      100, 100, 100, 60, 75, 100,
                                      60, 150, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0
                                    };

//-------------------------------------------------------------------------------
// texts
//-------------------------------------------------------------------------------
char
supervisor_shop::shoptext00[] =
  { "WELCOME ...... TO THE "
    "  TECNOBALL-Z SHOP    " "                      " "PRICE BONUS IS ENABLE "
  };

/*
//...............................................................................
char
supervisor_shop::shoptext41[] =
  { "SORRY, BUT YOU HAVEN'T" "   GOT ENOUGH MONEY   " "  TO BUY THIS OPTION  "
};
*/

/*
char
supervisor_shop::shoptext63[] =
  { "                     "
"  YOU CAN ONLY BUY   " "      .. GADGETS     " "                     "
};
 
char
supervisor_shop::shoptext56[] =
  { "  SORRY  THIS OPTION  " "   CAN ONLY BE USED   " "      FOR AREA 5      "
};
*/

//...............................................................................
char
supervisor_shop::shoptextPT[] = { 0, 1, 2, 3, 4, 5,   //S+/F1/F2/RW/B2/B3
                                  6, 7, 8, 9, 10, 11,           //P1/P2/LB/L+/??/WA
                                  12, 13, 14, 15, 16, 17,       //BL/BU/BR/S2/S3/RB
                                  18, 19, 20, 20, 20, 20,       //CT/GL/XX/XX/XX/XX
                                  20, 20, 20, 20, 20, 20,       //XX/XX/XX/XX/XX/XX
                                };

/*
char
  supervisor_shop::shoptext12[] =
  { "    AH YEAH IT IS     " "  MUCH BETTER WITH A  " " SUCH LONGER BUMPER   "
    "THIS FIRE IS NOT AS SO" " POWERFULL AS FIRE 1  " "  BUT IT IS CHEAPER   "
    "   BIGGER POWER  TO   " "  DESTROY MORE BRICS  " "                      "
    "   OK, THE RED WALLS  " "   WILL BE REBUILT    " "                      "
    "                      " "  TWO BALLS TO CATCH  " "                      "
    "    THREE BALLS NOW   " " YOU WILL BRAKE MORE  " " AND FASTER THE WALL  "
    "  A VERY STRONG BALL  " "    TO BREAK BRIKS    " "                      "
    "  WITH THAT YOU WILL  " "  BREAK ALL YOU WANT  " "                      "
    " THERE WILL 10 BRICKS " "    LESS TO BREAK     " "                      "
    " -------------------- " "   EXTRA LIFE ADDED   " " -------------------- "
    "                      " "                      " "                      "
    "          OK          " "       FOR  THE       " "         WALL         "
    " OK FOR A LEFT BUMPER " "BUT DON'T FORGET IT IS" " OVER AFTER 3 LEVELS  "
    "  OK FOR  UP  BUMPER  " "BUT DON'T FORGET IT IS" " OVER AFTER 3 LEVELS  "
    "OK  FOR A RIGHT BUMPER" "BUT DON'T FORGET IT IS" " OVER AFTER 3 LEVELS  "
    "                      " "    BIGSIZE  BALLS!   " "                      "
    "                      " "   MAXISIZE  BALLS!   " "                      "
    "          OK          " "       FOR  THE       " "     ROBOT BUMPER!    "
    "          OK          " "       FOR  THE       " " CONTROL BALL OPTION  "
    "          OK          " "       FOR  THE       " "     GLUE  OPTION     "
  "      GO AND SEE      " "        MY LOVE       " "                      "
};
*/

char
supervisor_shop::info_text1[] = {
                                  "RIGHT BUMPER:NO ACTIVE"
                                  "UP BUMPER   :NO ACTIVE"
                                  "LEFT BUMPER :NO ACTIVE"
                                  "     YOU ONLY HAVE    "
                                  "     00 LIVES LEFT    "
                                  "                      "
                                  "                      "
                                  "                      "
                                  "  DIFFICULTY IS ....  "
                                  "I HOPE THIS ASSISTANCE"
                                  "     WILL HELP YOU    "
                                  "                      "
                                  "      TLK  GAMES      "
                                  "      TECNOBALLZ      "
                                  "      TLK  GAMES      "
                                  "  LA CORRUPTION REVET " "   DES  DEGUISEMENTS  " "        INFINIS       "
                                };

char
supervisor_shop::info_text2[] = {
                                  "RIGHT BUMPER:  ACTIVE "
                                  "RIGHT BUMPER:NO ACTIVE"
                                  "  BUY A RIGHT BUMPER  "
                                  "UP BUMPER   :  ACTIVE "
                                  "UP BUMPER   :NO ACTIVE"
                                  "     BUY A UP BUMPER  "
                                  "LEFT[BUMPER :  ACTIVE " "LEFT BUMPER :NO ACTIVE" "  BUY A LEFT BUMPER   "
                                };
char
supervisor_shop::info_text3[] = {
                                  " THE PASSWORD FOR THIS"
                                  "  AREA IS ..........  " " NO AVAILABLE PASSWORD" " FOR THIS AREA.       "
                                };

const unsigned char
supervisor_shop::cyclingtab[] =
  {
    239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
    253, 254, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244,
    243, 242, 241, 240, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
    249, 250, 251, 252, 253, 254, 255
  };
