//*****************************************************************************
// copyright (c) 1991-2005 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "ecran_hard.cc"
// created	: 2002-08-17
// updates	: 2005-01-10
// id		: $Id: ecran_hard.cc,v 1.9 2007/01/18 17:09:53 gurumeditation Exp $
//-----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA 02111-1307, USA.
//*****************************************************************************
#include "../include/ecran_hard.h"
#include "../include/handler_keyboard.h"
#include "../include/audiomixer.h"
#include "../include/ressources.h"

char 	ecran_hard::nomfenetre[25] = "TecnoballZ by TLK Games\0";
bool	ecran_hard::optionfull = false;
bool	ecran_hard::optionsync = true;

//------------------------------------------------------------------------------
// create the object
//------------------------------------------------------------------------------
	ecran_hard::ecran_hard()
{
	sdl_screen = (SDL_Surface *)NULL;	// pointer to a SDL surface, the main screen
	bufSurface = (SDL_Surface *)NULL;	// pointer to the surface for display
	tamSurface = (SDL_Surface *)NULL;	// pointer to the surface for restaure 
	fps_totale = 0;
	fpscounter = 0;
	tiltoffset = 0;
	speed_game = 20;	//1000 / 50
	//speed_game = 17;	//1000 / 60
	//speed_game = 12;	//1000 / 85
	wait_inter = 10;	// interval
	wait_count = 0;
	wait_total = 0;
	VBL_switch = 0;

/*
gameSpeed = 0.5;
last_time = 0;
fps = 50.0;
targetAdj = 1.0;
*/
}

//------------------------------------------------------------------------------
// release the object
//------------------------------------------------------------------------------
	ecran_hard::~ecran_hard()
{
	if(!bufSurface)
	{	SDL_FreeSurface(bufSurface);
		bufSurface = (SDL_Surface *)NULL;;
	}
	if(!tamSurface)
	{	SDL_FreeSurface(tamSurface);
		tamSurface = (SDL_Surface *)NULL;;
	}
	SDL_Quit();
}

//------------------------------------------------------------------------------
// initialize the screen SDL
//------------------------------------------------------------------------------
Sint32 ecran_hard::ecran_init()
{
	
	if (init_video() != E_NO_ERROR)
		return E_SDLERROR;
	//SDL_informations();

	//###################################################################
	// allocate "buffer" surface
	//###################################################################
	bufLargeur = ecranlarge;
	bufHauteur = ecranhaute + offsetplus * 2 ;
	bufSurface = SDL_CreateRGBSurface(SDL_ANYFORMAT, bufLargeur, bufHauteur,
										bitspixels, 0xf00, 0x0f0, 0x00f, 0x00);
	if(!bufSurface)
	{	fprintf(stderr, "ecran_hard::ecran_init() : SDL_CreateRGBSurface " \
			"error: %s\n", SDL_GetError());
		return E_SDLERROR;
	}
	buf_nextLn = bufSurface->pitch;
	bufAdresse = (char *)bufSurface->pixels + buf_nextLn * offsetplus ;
	bufProfond = bufSurface->format->BytesPerPixel;

	//###################################################################
	// allocate "tampon" surface
	//###################################################################
	tamSurface = SDL_CreateRGBSurface(SDL_ANYFORMAT, bufLargeur, bufHauteur, bitspixels, 0xf00, 0x0f0, 0x00f, 0x00);
	if(!tamSurface)
	{	fprintf(stderr, "ecran_hard::ecran_init() : SDL_CreateRGBSurface " \
			"error: %s\n", SDL_GetError());
		return E_SDLERROR;
	}
	tam_nextLn = tamSurface->pitch;
	tamAdresse = (char *)tamSurface->pixels + tam_nextLn * offsetplus;

	datepreced = SDL_GetTicks();
	dateactuel = SDL_GetTicks();
	return 0;
}

//------------------------------------------------------------------------------
// initialize the video SDL
//------------------------------------------------------------------------------
Sint32 ecran_hard::init_video()
{
	ecranlarge = 320 * resolution;
	ecranhaute = 240 * resolution;
	offsetplus = 64 * resolution;
	if(is_verbose)
		fprintf(stdout,	"ecran_hard::init_video() %i * %i \n",
			ecranlarge,ecranhaute);

	//###################################################################
	// initialize SDL screen
	//###################################################################
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE | SDL_INIT_AUDIO) < 0)
	{	fprintf(stderr, "ecran_hard::ecran_init() : sdl_init: %s",
			SDL_GetError());
		return (erreur_num = E_SDLERROR);
	}

	//###################################################################
	// test if the video mode is available
	//###################################################################
	Uint32 flag = SDL_ANYFORMAT;
	if(optionfull)
		flag = flag | SDL_FULLSCREEN;
	Uint32 bpp = SDL_VideoModeOK(ecranlarge, ecranhaute, bitspixels, flag);
	if(!bpp)
	{	fprintf(stderr,
			"ecran_hard::init_video() : SDL_VideoModeOK() Mode not available : %s", 
			SDL_GetError());
		return (erreur_num = E_SDLERROR);
	}

	//###################################################################
	// initialize the video mode
	//###################################################################
	sdl_screen = SDL_SetVideoMode(ecranlarge, ecranhaute, bitspixels, flag);
	if(!sdl_screen)
	{	fprintf(stderr, "ecran_hard::init_video() : SDL_SetVideoMode() failed: %s\n", 
			SDL_GetError());
		return (erreur_num = E_SDLERROR);
	}
	SDL_WM_SetCaption(nomfenetre, nomfenetre);
	
#ifdef TU_TRICHES
	SDL_ShowCursor(SDL_ENABLE);
#else
	SDL_ShowCursor(SDL_DISABLE);
#endif
	SDL_EnableUNICODE(1);
	return E_NO_ERROR;	
}

//------------------------------------------------------------------------------
// return the screen's width 
//	output <= the width of the screen in pixels
//------------------------------------------------------------------------------
Sint32 ecran_hard::screenwdth()
{
	return (Sint32)(sdl_screen->w);
}

//------------------------------------------------------------------------------
// return the screen's height 
//	output <= the height of the screen in lines
//------------------------------------------------------------------------------
Sint32 ecran_hard::screenhght()
{
	return (Sint32)(sdl_screen->h);
}

//------------------------------------------------------------------------------
// lock buffer & tampon surfaces
//------------------------------------------------------------------------------
Sint32 ecran_hard::verouiller()
{
	if(SDL_LockSurface(bufSurface))
	{	fprintf(stderr, "ecran_hard::verouiller(): SDL_LockSurface return %s\n",
			SDL_GetError());
		return E_SDLERROR;
	}
	if(SDL_LockSurface(tamSurface))
	{	fprintf(stderr, "ecran_hard::verouiller(): SDL_LockSurface return %s\n",
			SDL_GetError());
		return E_SDLERROR;
	}
	return E_NO_ERROR;
}

//------------------------------------------------------------------------------
// unlock buffer & tampon surfaces
//------------------------------------------------------------------------------
Sint32 ecran_hard::deverouill()
{
	SDL_UnlockSurface(bufSurface);
	SDL_UnlockSurface(tamSurface);
	return E_NO_ERROR;
}

// -----------------------------------------------------------------------------
// affiche des informations sur SDL
// -----------------------------------------------------------------------------
Sint32 ecran_hard::SDL_informations()
{
	const SDL_VideoInfo *vi;
	char namebuf[32] = { "123456789012345678901234567890\0" };
	SDL_Rect **modes;
	Sint32 i;
	modes = SDL_ListModes(NULL, SDL_FULLSCREEN | SDL_HWSURFACE);
	if(modes == (SDL_Rect **) 0)
	{	printf("No modes available!\n");
		return 1;
	}
	if(modes == (SDL_Rect **) - 1)
		printf("All resolutions available.\n");
	else
	{	printf("Available Modes\n");
		for(i = 0; modes[i]; ++i)
			printf(" %d x %d\n", modes[i]->w, modes[i]->h);
	}
	SDL_WM_SetCaption("TecnoballZ", 0);
	vi = SDL_GetVideoInfo();
	printf("The number of bits used to represent each pixel in a surface.Usually 8, 16, 24 or 32. BitsPerPixel=%d \n", vi->vfmt->BitsPerPixel);
	printf("The number of bytes used to represent each pixel in a surface. Usually one to four BytesPerPixel=%d\n", vi->vfmt->BytesPerPixel);
	printf("Rmask=%i Gmask=%i Bmask=%i Amask=%i \n", vi->vfmt->Rmask, vi->vfmt->Gmask, vi->vfmt->Bmask, vi->vfmt->Amask);
	printf("Rshift=%i Gshift=%i Bshift=%i Ashift=%i \n", vi->vfmt->Rshift, vi->vfmt->Gshift, vi->vfmt->Bshift, vi->vfmt->Ashift);
	printf("Rloss=%i Gloss=%i Bloss=%i Aloss=%i \n", vi->vfmt->Rloss, vi->vfmt->Gloss, vi->vfmt->Bloss, vi->vfmt->Aloss);
	printf("Pixel value of transparent pixels. colorkey=%i \n", vi->vfmt->colorkey);
	printf("Overall surface alpha value. alpha=%i \n", vi->vfmt->alpha);
	printf("Is it possible to create hardware surfaces? hw_available=%i \n", vi->hw_available);
	printf("Is there a window manager available wm_available=%i \n", vi->wm_available);
	printf("Are hardware to hardware blits accelerated? blit_hw=%i \n", vi->blit_hw);
	printf("Are hardware to hardware colorkey blits accelerated? blit_hw_CC=%i \n", vi->blit_hw_CC);
	printf("Are hardware to hardware alpha blits accelerated? blit_hw_A=%i\n", vi->blit_hw_A);
	printf("Are software to hardware blits accelerated? blit_sw=%i \n", vi->blit_sw);
	printf("Are software to hardware colorkey blits accelerated? blit_sw_CC=%i \n", vi->blit_sw_CC);
	printf("Are software to hardware alpha blits accelerated? blit_sw_A=%i \n",
		vi->blit_sw_A);
	printf("Are color fills accelerated? blit_fill=%i \n", vi->blit_fill);
	printf("Total amount of video memory in Kilobytes. video_mem=%i\n",
		vi->video_mem);
	char *c = SDL_VideoDriverName(namebuf, 32);
	printf("the name of the video driver=%s\n", c);
	return 0;
}


//-------------------------------------------------------------------------------
// switch fullscreen or windows mode
//-------------------------------------------------------------------------------
void ecran_hard::fullscreen()
{
	if(keyboard->command_is_pressed(handler_keyboard::FULLSCFLAG) &&
		keyboard->get_input_cursor_pos() < 0)
	{	if(optionfull) optionfull = false;
		else optionfull = true;
		ecran_gere->init_video();
		ecran_gere->palette_go(ze_palette);
		//keyfscreen = 0;
	}
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
void ecran_hard::waitVBlank()
{
/*
	if(keyboard->command_is_pressed(handler_keyboard::WAITVBLOFF))
	{
		VBL_switch++;
		if(VBL_switch > 2) VBL_switch = 0;
	}
*/
	waitVBLtec();
/*
	switch(VBL_switch)
	{
		case 0:
			waitVBLchr();
			break;
		case 1:
			waitVBLtec();
			break;
		default:
			break;
	}
*/	
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
void ecran_hard::waitVBLchr()
{
/*
	keyboard->read_events();
	fullscreen();
#ifndef SOUNDISOFF
	ptAudiomix->execution1();
#endif
	gameFrame++;
	Uint32 delay = 32-(int)(24.0 * gameSpeed);
	SDL_Delay(delay);
	printf("waitVBLchr %i\n", delay);
	if(!(gameFrame%10)) return;
	Uint32 now_time = SDL_GetTicks();
	if(last_time)
		fps = (10.0/(now_time - last_time))*1000.0;
	last_time = now_time;
	if(gameFrame >= 400) return;
	if(fps < 48.0 && gameSpeed < 1.0)
		gameSpeed += 0.02;
	else if(gameFrame > 20)
	{	float tmp = 50.0 / fps;
		tmp = 0.8*targetAdj + 0.2*tmp;
		targetAdj = floor(100.0*(tmp+0.005))/100.0;
	}
	return;
*/
}

//-------------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------------
void ecran_hard::waitVBLtec()
{
	Uint32 durat = 0;
	countframe++;
	durat = SDL_GetTicks() - datepreced;
	optionsync = true;
	if(optionsync)
	{	wait_total += durat;
		if(--wait_count <= 0)
		{	//wait_diffv = durat;
 			wait_value = ((speed_game * wait_inter) - wait_total ) / wait_inter;
			wait_count = wait_inter;
			wait_total = 0;
			if(wait_value <= 0)
				wait_value = 1;
		}
		if(wait_value > 0)
		{	//fprintf(stdout, "waitVBLtec %i\n", wait_value);
			SDL_Delay(wait_value);
		}
	}
	fpscounter++;
	fps_totale = fps_totale + durat;
	if(fpscounter >= 100)
	{	
		if (fps_totale != 0) {      // fix crash : divide by zero !
			framepeers = 1000 * fpscounter / fps_totale;
		}
		fpscounter = 0;
		fps_totale = 0;
	}
	datepreced = SDL_GetTicks();
	keyboard->read_events();
	fullscreen();
#ifndef SOUNDISOFF
	ptAudiomix->execution1();
#endif
	return;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Sint32 ecran_hard::get_framepee()
{
	return framepeers;
}


//------------------------------------------------------------------------------
// get time since last call
//------------------------------------------------------------------------------
inline Sint32 ecran_hard::retour_temps()
{
	Sint32 d;
	dateactuel = SDL_GetTicks(); //Get the number of milliseconds since the SDL library initialization.
	if(dateactuel == datepreced)
		SDL_Delay(1);
	dateactuel = SDL_GetTicks();
	d = dateactuel - datepreced;
	datepreced = dateactuel;
	return d;
}

//------------------------------------------------------------------------------
// reset the timer
//------------------------------------------------------------------------------
inline void ecran_hard::mise_a_zero_timer()
{
	datepreced = SDL_GetTicks();
}

//------------------------------------------------------------------------------
// buffer: return pointer to the buffer memory 
//------------------------------------------------------------------------------
char *ecran_hard::buffer_adr()
{
	return bufAdresse;
}

//------------------------------------------------------------------------------
// buffer: convert (x,y) to pointer into the buffer memory
//	input	=> pos_x: x coordinate
//			=> pos_y: y coordinate
//------------------------------------------------------------------------------
char *ecran_hard::buffer_pos(Sint32 pos_x, Sint32 pos_y)
{
	return (bufAdresse + pos_y * buf_nextLn + pos_x);
}

//------------------------------------------------------------------------------
// buffer: clear the entirety of the buffer memory 
//------------------------------------------------------------------------------
void ecran_hard::buffer_RAZ(Sint32 pixel)
{
	char *d = bufAdresse;
	char p = (char)pixel;
	Sint32 o = buf_nextLn;
	Sint32 l = bufLargeur * bufProfond;
	Sint32 h = bufHauteur;
	for(Sint32 j = 0; j < h; j++, d += o)
	{	for(Sint32 i = 0; i < l; i++)
			d[i] = p;
	}
}

//------------------------------------------------------------------------------
// buffer: clear a zone of the buffer memory 
//	input	=> pixel: pixel color
//			=> x_pos: x coordinate
//			=> y_pos: y coordinate
//			=> width: zone width
//			=> heigh: zone height
//------------------------------------------------------------------------------
void ecran_hard::buffer_RAZ(Sint32 pixel, Sint32 x_pos, Sint32 y_pos,
								Sint32 width, Sint32 heigh)
{
	char *ptMem = buffer_pos(x_pos, y_pos);
	Sint32 a = width;
	Sint32 b = heigh;
	Sint32 n = buf_nextLn;
	for(Sint32 j = 0; j < b; j++, ptMem += n)
	{	for(Sint32 i = 0; i < a; i++)
			ptMem[i] = pixel;
	}
}

//------------------------------------------------------------------------------
// buffer: return size of line in bytes
//------------------------------------------------------------------------------
Sint32 ecran_hard::bufferNext()
{
	return buf_nextLn;
}

//------------------------------------------------------------------------------
// buffer: return offset in bytes
//------------------------------------------------------------------------------
Sint32 ecran_hard::buffer_rel(Sint32 pos_x, Sint32 pos_y)
{
	return (pos_y * buf_nextLn + pos_x);
}

//------------------------------------------------------------------------------
// buffer: return modulo
//	input	=> width: width 
//------------------------------------------------------------------------------
Sint32 ecran_hard::buffer_mod(Sint32 width)
{
	return (Sint32)(buf_nextLn - (width * bufProfond));
}

//------------------------------------------------------------------------------
// buffer: display into the buffer
//------------------------------------------------------------------------------
void ecran_hard::buffer_GFX(char *srcPT, Sint32 large, Sint32 haute)
{
	genericGFX(srcPT, large, haute, large, bufAdresse, bufLargeur,
					bufHauteur, buf_nextLn);
}

//------------------------------------------------------------------------------
// tampon: return pointer to the buffer memory 
//------------------------------------------------------------------------------
char *ecran_hard::tampon_adr()
{
	return (tamAdresse);
}

//------------------------------------------------------------------------------
// tampon: convert (x,y) to pointer into the buffer memory
//	input	=> pos_x: x coordinate
//			=> pos_y: y coordinate
//------------------------------------------------------------------------------
char *ecran_hard::tampon_pos(Sint32 pos_x, Sint32 pos_y)
{
	return (tamAdresse + pos_y * tam_nextLn + pos_x);
}

//------------------------------------------------------------------------------
// tampon: clear the entirety of the tampon memory 
//------------------------------------------------------------------------------
void ecran_hard::tampon_RAZ(Sint32 pixel)
{
	char *d = tamAdresse;
	char p = (char)pixel;
	Sint32 o = tam_nextLn;
	Sint32 l = bufLargeur * bufProfond;
	Sint32 h = bufHauteur;
	for(Sint32 j = 0; j < h; j++, d += o)
	{	for(Sint32 i = 0; i < l; i++)
			d[i] = p;
	}
}

//------------------------------------------------------------------------------
// tampon: clear a zone of the tampon memory 
//------------------------------------------------------------------------------
void ecran_hard::tampon_RAZ(Sint32 pixel, Sint32 x_pos, Sint32 y_pos,
								Sint32 width, Sint32 heigh)
{
	char *ptMem = tampon_pos(x_pos, y_pos);
	Sint32 a = width;
	Sint32 b = heigh;
	Sint32 n = tam_nextLn;
	for(Sint32 j = 0; j < b; j++, ptMem += n)
	{	for(Sint32 i = 0; i < a; i++)
			ptMem[i] = pixel;
	}
}

//------------------------------------------------------------------------------
// tampon: return size of line in bytes
//------------------------------------------------------------------------------
Sint32 ecran_hard::tamponNext()
{
	return tam_nextLn;
}

//------------------------------------------------------------------------------
// tampon: return offset in bytes
//------------------------------------------------------------------------------
Sint32 ecran_hard::tampon_rel(Sint32 pos_x, Sint32 pos_y)
{
	return (pos_y * tam_nextLn + pos_x);
}

//------------------------------------------------------------------------------
// tampon: return modulo
//------------------------------------------------------------------------------
Sint32 ecran_hard::tampon_mod(Uint32 large)
{
	return (Sint32)(tam_nextLn - (large * bufProfond));
}

//------------------------------------------------------------------------------
// tampon memory: 
//------------------------------------------------------------------------------
void ecran_hard::tampon_GFX(SDL_Surface * srcPT, Sint32 large, Sint32 haute)
{
	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	r.w = large;
	r.h = haute;
	Sint32 v = SDL_BlitSurface(srcPT, &r, tamSurface, &r);
	if(v < 0)
		fprintf(stderr,
			"ecran_hard::tampon_GFX() BlitSurface error: %s\n",
			SDL_GetError());
}

//------------------------------------------------------------------------------
// copy the "tampon" memory in the "buffer" memory
//------------------------------------------------------------------------------
void ecran_hard::tamponBuff()
{
	char *s = (char *)tamSurface->pixels;
	char *d = (char *)bufSurface->pixels;
	Sint32 n = tam_nextLn;
	Sint32 o = buf_nextLn;
	Sint32 l = bufLargeur * bufProfond;
	Sint32 h = bufHauteur;
	for(Sint32 j = 0; j < h; j++, s += n, d += o)
	{	for(Sint32 i = 0; i < l; i++)
			d[i] = s[i];
	}
}

//------------------------------------------------------------------------------
// copy a part of the "tampon" in the "buffer"
//	input	=> pos_x: x coordinate
//			=> pos_y: y coordinate
//			=> large: width
//			=> haute: height
//------------------------------------------------------------------------------
void ecran_hard::tamponBuff(Sint32 pos_x, Sint32 pos_y, Sint32 large, Sint32 haute)
{
#ifndef BYTES_COPY	
	Sint32 *s = (Sint32 *)(tamAdresse + (pos_y * tam_nextLn + pos_x));
	Sint32 *d = (Sint32 *)(bufAdresse + (pos_y * buf_nextLn + pos_x));
	Sint32 n = tam_nextLn >> 2;
	Sint32 o = buf_nextLn >> 2;
	Sint32 l = large >> 2;
#else
	char *s = tamAdresse + (pos_y * tam_nextLn + pos_x);
	char *d = bufAdresse + (pos_y * buf_nextLn + pos_x);
	Sint32 n = tam_nextLn;
	Sint32 o = buf_nextLn;
	Sint32 l = large;
#endif
	Sint32 h = haute;
	for(Sint32 j = 0; j < h; j++)
	{	for(Sint32 i = 0; i < l; i++)
			d[i] = s[i];
		s = s + n;
		d = d + o;
	}
}

//------------------------------------------------------------------------------
// buffer & tampon: return height of there in pixels
//------------------------------------------------------------------------------
Sint32 ecran_hard::bufferHaut()
{
	return bufHauteur;
}

//------------------------------------------------------------------------------
// buffer & tampon: return width of there in bytes
//------------------------------------------------------------------------------
Sint32 ecran_hard::bufferLarg()
{
	return bufLargeur;
}

//------------------------------------------------------------------------------
// buffer & tampon: convert (x,y) to offset
//	input	=> zbase: start offset
//			=> offsx: x coordinate
//			=> offsy: y coordinate
//------------------------------------------------------------------------------
Sint32 ecran_hard::ecran_next(Sint32 zbase, Sint32 offsx, Sint32 offsy)
{
	return (zbase + offsy * buf_nextLn + offsx);
}

//-------------------------------------------------------------------------------
// initialize color palette for the current screen
//-------------------------------------------------------------------------------
void ecran_hard::palette_go(unsigned char *palPT)
{
	unsigned char *p = palPT;
	SDL_Color *color = &ze_palette[0];
	Uint32 k = 0;
	for(Sint32 i = 0; i < 256; i++)
	{	color->r = p[k++];
		color->g = p[k++];
		color->b = p[k++];
		color++;
	}
	SDL_SetPalette(bufSurface, SDL_LOGPAL | SDL_PHYSPAL, ze_palette, 0, 256);
	SDL_SetPalette(sdl_screen, SDL_LOGPAL | SDL_PHYSPAL, ze_palette, 0, 256);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ecran_hard::palette_go(SDL_Color * palPT)
{
	SDL_SetPalette(bufSurface, SDL_LOGPAL | SDL_PHYSPAL, palPT, 0, 256);
	SDL_SetPalette(sdl_screen, SDL_LOGPAL | SDL_PHYSPAL, palPT, 0, 256);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
SDL_Color *ecran_hard::paletteAdr()
{
	return (ze_palette);
}

//------------------------------------------------------------------------------
// buffer memory: copy buffer to screen
//------------------------------------------------------------------------------
void ecran_hard::bufferCopy()
{
	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	r.w = 640;
	r.h = 480;
	Sint32 v = SDL_BlitSurface(bufSurface, &r, sdl_screen, &r);
	if(v < 0)
		fprintf(stderr, "ecran_hard::bufferCopy() BlitSurface error: %s\n",
			SDL_GetError());
}

//-----------------------------------------------------------------------------
// buffer memory: copy buffer to screen
//------------------------------------------------------------------------------
void ecran_hard::bufferCTab()
{
	SDL_Rect rsour;
	rsour.x = 0;
	rsour.y = offsetplus + tiltoffset;
	rsour.w = 640;
	rsour.h = offsetplus + 480 + tiltoffset;

	SDL_Rect rdest;
	rdest.x = 0;
	rdest.y = 0;
	rdest.w = 640;
	rdest.h = 480;
 
	Sint32 v = SDL_BlitSurface(bufSurface, &rsour, sdl_screen, &rdest);
	if(v < 0)
		fprintf(stderr, "ecran_hard::bufferCTab() : BlitSurface error: %s\n",
			SDL_GetError());
	SDL_UpdateRect(sdl_screen, 0, 0, sdl_screen->w, sdl_screen->h);
	if(tiltoffset>0)
		tiltoffset--;
}

//-----------------------------------------------------------------------------
// buffer memory: copy a rectangular zone to screen
//------------------------------------------------------------------------------
void ecran_hard::bufferCTab(Sint32 xpos1, Sint32 ypos1, Sint32 xpos2, Sint32 ypos2 )
{
	SDL_Rect r;
	r.x = xpos1;
	r.y = ypos1;
	r.w = xpos2;
	r.h = ypos2;
	Sint32 v = SDL_BlitSurface(bufSurface, &r, sdl_screen, &r);
	if(v < 0)
		fprintf(stderr,
			"ecran_hard::bufferCTab() BlitSurface error: %s\n", SDL_GetError());
	SDL_UpdateRect(sdl_screen, 0, 0, sdl_screen->w, sdl_screen->h);
}

//-------------------------------------------------------------------------------
// tampon memory: display shadow box
//-------------------------------------------------------------------------------
void ecran_hard::rectShadow(Sint32 pos_x, Sint32 pos_y, Sint32 width, Sint32 heigh)
{
	char k = (char)(0x80);
	Sint32 l = width;
	Sint32 m = pos_y + heigh;
	for(Sint32 j = pos_y; j < m; j++)
	{	char *monPT = tampon_pos(pos_x, j);
		for(Sint32 i = 0; i < l; i++)
			*(monPT)++ |= k;
	}
}

//-------------------------------------------------------------------------------
// buffer & tampon: clear shadow box
//	input	=>	offst: offset adresse ecran
//				large: width box in pixels
//				haute: height box en pixels
//-------------------------------------------------------------------------------
void ecran_hard::clr_shadow(Sint32 offst, Sint32 large, Sint32 haute)
{
	char zmask = 0x7F;
	char *bffer = bufAdresse + offst;
	char *tmpon = tamAdresse + offst;
	Sint32 a = large;
	Sint32 b = haute;
	Sint32 n = buf_nextLn - a;
	for(Sint32 j = 0; j < b; j++, bffer += n, tmpon += n)
	{	for(Sint32 i = 0; i < a; i++)
		{	char pixel = *bffer;
			pixel &= zmask;
			*(bffer++) = pixel;
			pixel = *tmpon;
			pixel &= zmask;
			*(tmpon++) = pixel;
		}
	}
}

//-------------------------------------------------------------------------------
// buffer & tampon: clear shadow box
// 	input	=> _iPosX:
//			=> _iPosY:
//			=> _iLarg: width box in pixels
//			=> _iHaut: height box en pixels
//-------------------------------------------------------------------------------
void ecran_hard::clr_shadow(Sint32 _iPosX, Sint32 _iPosY, Sint32 _iLarg,
								Sint32 _iHaut)
{
	char zmask = 0x7F;
	char *bffer = tampon_pos(_iPosX, _iPosY);
	char *tmpon = buffer_pos(_iPosX, _iPosY);
	Sint32 a = _iLarg;
	Sint32 b = _iHaut;
	Sint32 n = buf_nextLn - a;
	for(Sint32 j = 0; j < b; j++, bffer += n, tmpon += n)
	{	for(Sint32 i = 0; i < a; i++)
		{	char pixel = *bffer;
			pixel &= zmask;
			*(bffer++) = pixel;
			pixel = *tmpon;
			pixel &= zmask;
			*(tmpon++) = pixel;
		}
	}
}

//------------------------------------------------------------------------------
// buffer & tampon: display shadow box
//	input	=> offst: buffer offset in bytes
//			=> large: width box in pixels
//			=> haute: height box en pixels
//------------------------------------------------------------------------------
void ecran_hard::set_shadow(Sint32 offst, Sint32 large, Sint32 haute)
{
	char zmask = (char)(0x80);
	char *bffer = bufAdresse + offst;
	char *tmpon = tamAdresse + offst;
	Sint32 a = large;
	Sint32 b = haute;
	Sint32 n = buf_nextLn - a;
	for(Sint32 j = 0; j < b; j++, bffer += n, tmpon += n)
	{	for(Sint32 i = 0; i < a; i++)
		{	char pixel = *bffer;
			pixel |= zmask;
			*(bffer++) = pixel;
			pixel = *tmpon;
			pixel |= zmask;
			*(tmpon++) = pixel;
		}
	}
}

//------------------------------------------------------------------------------
// buffer: copy an image of 32 width pixels into buffer memory 
//------------------------------------------------------------------------------
void ecran_hard::buf_affx32(char *srcPT, char *desPT, Sint32 width, Sint32 heigh)
{
	Sint32 *d = (Sint32 *)desPT;
	Sint32 *s = (Sint32 *)srcPT;
	Sint32 j = heigh;
	Sint32 ms = width;
	Sint32 md = buf_nextLn;
	for(Sint32 i = 0; i < j; i++)
	{	d[0] = s[0];
		d[1] = s[1];
		d[2] = s[2];
		d[3] = s[3];
		d[4] = s[4];
		d[5] = s[5];
		d[6] = s[6];
		d[7] = s[7];
		s = (Sint32 *)((char *)s + ms);
		d = (Sint32 *)((char *)d + md);
	}
}

//------------------------------------------------------------------------------
// buffer: copy an image of 64 width pixels into buffer memory
//------------------------------------------------------------------------------
void ecran_hard::buf_affx64(char *srcPT, char *desPT, Sint32 width, Sint32 heigh)
{
	double *d = (double *)desPT;
	double *s = (double *)srcPT;
	Sint32 j = heigh;
	Sint32 ms = width;
	Sint32 md = buf_nextLn;
	for(Sint32 i = 0; i < j; i++)
	{	d[0] = s[0];
		d[1] = s[1];
		d[2] = s[2];
		d[3] = s[3];
		d[4] = s[4];
		d[5] = s[5];
		d[6] = s[6];
		d[7] = s[7];
		s = (double *)((char *)s + ms);
		d = (double *)((char *)d + md);
	}
}

//------------------------------------------------------------------------------
// copy an image
//------------------------------------------------------------------------------
void ecran_hard::genericGFX(char *sAdre, Sint32 sLarg, Sint32 sHaut, Sint32 sNext,
								char *dAdre, Sint32 dLarg, Sint32 dHaut, Sint32 dNext)
{
	Sint32 h, l;
	Sint32 a = sNext;
	Sint32 b = dNext;
	if(sLarg > dLarg)
		l = sLarg;
	else
		l = dLarg;
	if(sHaut > dHaut)
		h = dHaut;
	else
		h = sHaut;
	char *d = dAdre;
	char *s = sAdre;
	for(Sint32 i = 0; i < h; i++, s += a, d += b)
	{	for(Sint32 j = 0; j < l; j++)
			d[j] = s[j];
	}
}

//------------------------------------------------------------------------------
// shift the screen of 10 or 20 pixels upwards
//------------------------------------------------------------------------------
void ecran_hard::tiltscreen()
{
	tiltoffset = 10 * resolution;
}

//------------------------------------------------------------------------------
// select colour gradation
//------------------------------------------------------------------------------
void ecran_hard::gradation1()
{
	SDL_Color *palPT = ecran_gere->paletteAdr();
	SDL_Color *palP1 = palPT + 239;
	Sint32 i = hasard_val & 0x0F;
	if(i >= 10)
		i = i - 10;
	const Uint32 *ptpal = (ressources::tabledegas + i * 18);
	for(i = 0; i < 17; i++)
	{	Uint32 vacol = ptpal[i];
		Uint32 vablu = vacol & 0x000000ff;
		Uint32 vagre = vacol & 0x0000ff00;
		vagre = vagre >> 8;
		Uint32 vared = vacol & 0x00ff0000;
		vared = vared >> 16;
		palP1->r = vared;
		palP1->g = vagre;
		palP1->b = vablu;
		palP1++;
	}
	ecran_gere->palette_go(palPT); 
}
