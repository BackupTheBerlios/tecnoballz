//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file		: "technoBall.cc"
// created		: ?
// updates		: 2004-10-15
// fonction	: handle ball of game
//-----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place - Suite 330, Boston, MA  02111-1307, USA.
//******************************************************************************
#include "../include/technoBall.h"

//-----------------------------------------------------------------------------
// create the object
//-----------------------------------------------------------------------------
technoBall::technoBall()
{
	BOBprepare();
	littleInit(0, 0, (tecno_bump *)NULL, ballSpeed1, 0);
}

//-----------------------------------------------------------------------------
// release the object
//-----------------------------------------------------------------------------
technoBall::~technoBall()
{
}

//-----------------------------------------------------------------------------
// perform some initializations
//	input	=> start: time before the ball leaves the bumper (first time)
//		=> speed: time before the ball accelerates
//		=> raket: default bumper
//		=> table: speed ball (1 to 4)
//		=> bwgth: width of brick in pixels 
//-----------------------------------------------------------------------------
void technoBall::littleInit(Sint32 start, Sint32 speed,
	tecno_bump *raket, Sint16 *table, Sint32 bwght)
{
	start_init = start;
	speed_init = speed;
	speedBallZ = table;
	BOB_desact();
	colleBallF = 0;
	startCount = 0;
	brickWidth = bwght;
	razingBal0(raket);
}

//-----------------------------------------------------------------------------
// initialize again
//-----------------------------------------------------------------------------
void technoBall::reStarting(tecno_bump * raket)
{
	BOB_active();
	startCount = start_init;
	colleBallF = raket->bumper_num();
	razingBall(raket);
}

//-----------------------------------------------------------------------------
// disable the ball
//-----------------------------------------------------------------------------
void technoBall::goSleeping(tecno_bump * raket)
{
	BOB_desact();
	colleBallF = 0;
	startCount = 0;
	razingBall(raket);
}

//-----------------------------------------------------------------------------
// reset some members
//-----------------------------------------------------------------------------
void technoBall::razingBall(tecno_bump * raket)
{
	razingBal0(raket);
	pointe_GFX();
}

//-----------------------------------------------------------------------------
// reset some members
//-----------------------------------------------------------------------------
void technoBall::razingBal0(tecno_bump * raket)
{
	speedCount = speed_init;
	colLargeur = BALLWIDTH1 * resolution;
	colHauteur = BALLWIDTH1 * resolution;
	directBall = 0;
	save_Dball = 0;
	countDball = 0;
	raket_ball = raket;
	raket_glue = raket;
	speedBallT = speedBallZ;
	collisionT = brikPoint1;
	powerBall1 = 1;
	powerBall2 = brickWidth;
	eject_ball[0] = 0;
	eject_ball[1] = 0;
	eject_ball[2] = 0;
	eject_ball[3] = 0;
	tiltCompte = 0;
	ClBouiBoui = 0;
	balle_rota = 1;
	tempo_rota = 1;
	ball_sizeX = BALL_SIZE1;
	ballPowerX = BALLNORMAL;
	oeilRotate = 0;
	colli_wall = 0;
}

//-----------------------------------------------------------------------------
// initialize a simple ball (sticked on the bumper)
//-----------------------------------------------------------------------------
void technoBall::startBalle(Sint32 large)
{
	BOB_active();
	tecno_bump *raket = raket_glue;
	reStarting(raket);
	position_x = raket->retournePX() + ((large >> 1) - ((colLargeur >> 1) + 1));
	position_y = raket->retournePY() - colHauteur - 1;
}

//-------------------------------------------------------------------------------
// disbable sticked ball
//-------------------------------------------------------------------------------
void technoBall::glueLibere()
{
	colleBallF = 0;
	startCount = 0;
}

//-------------------------------------------------------------------------------
// get speed ball table
//-------------------------------------------------------------------------------
Sint16 *technoBall::donneSpeed(Sint32 speed)
{
	switch (speed)
	{	case 1:
			return ballSpeed1;
		case 2:
			return ballSpeed2;
		case 3:
			return ballSpeed3;
		case 4:
			return ballSpeed4;
	}
	return ballSpeed1;
}

//-------------------------------------------------------------------------------
// return the last touched bumper
//-------------------------------------------------------------------------------
tecno_bump *technoBall::donne_bump()
{
	return (raket_ball);
}

//-------------------------------------------------------------------------------
// clone ball
//-------------------------------------------------------------------------------
void technoBall::duplicate3(technoBall * balle, Sint32 angle)
{
	flag_actif = 1;
	position_x = balle->position_x;
	position_y = balle->position_y;
	directBall = angle;
	colleBallF = 0;
	tiltCompte = 0;
	ball_sizeX = balle->ball_sizeX;
	ballPowerX = balle->ballPowerX;
	colLargeur = balle->colLargeur;
	colHauteur = balle->colHauteur;
	raket_ball = balle->raket_ball;
	raket_glue = balle->raket_glue;
	speedBallT = balle->speedBallT;
	collisionT = balle->collisionT;
	powerBall1 = balle->powerBall1;
	powerBall2 = balle->powerBall2;
	oeilRotate = 0;
	pointe_GFX();
}

//-------------------------------------------------------------------------------
// enable the ball power 1
//-------------------------------------------------------------------------------
void technoBall::ballPower1()
{
	ballPowerX = BALLPOWER1;
	pointe_GFX();
}

//-------------------------------------------------------------------------------
// enable the ball power 2
//-------------------------------------------------------------------------------
void technoBall::ballPower2()
{
	ballPowerX = BALLPOWER2;
	pointe_GFX();
}

//-------------------------------------------------------------------------------
// enable the ball size 2
//-------------------------------------------------------------------------------
void technoBall::ball_size2()
{
	powerBall1 = 2;
	powerBall2 = brickWidth * 2;
	colLargeur = BALLWIDTH2 * resolution;
	colHauteur = BALLWIDTH2 * resolution;
	ball_sizeX = BALL_SIZE2;
	collisionT = brikPoint2;
	pointe_GFX();
}

//-------------------------------------------------------------------------------
// enable the ball size 3
//-------------------------------------------------------------------------------
void technoBall::ball_size3()
{
	powerBall1 = 3;
	powerBall2 = brickWidth * 3;
	colLargeur = BALLWIDTH3 * resolution;
	colHauteur = BALLWIDTH3 * resolution;
	ball_sizeX = BALL_SIZE3;
	collisionT = brikPoint3;
	pointe_GFX();
}

//-------------------------------------------------------------------------------
// set the fastest speed 
//-------------------------------------------------------------------------------
void technoBall::very_speed()
{
	speedBallT = ballSpeed3;
}

//-------------------------------------------------------------------------------
// bricks levels: enable the ball in a ejector 
//-------------------------------------------------------------------------------
void technoBall::ball2eject(Sint32 index, Sint32 otime)
{
	flag_actif = 1;
	index &= 3;
	furaxEject *monPT = furaxTable + index;
	if(monPT->ejectBall1) eject_ball[0] = otime;
	else eject_ball[0] = 0;
	if(monPT->ejectBall2) eject_ball[1] = otime;
	else eject_ball[1] = 0;
	if(monPT->ejectBall3) eject_ball[2] = otime;
	else eject_ball[2] = 0;
	if(monPT->ejectBall4) eject_ball[3] = otime;
	else eject_ball[3] = 0;
	position_x = monPT->position_x;
	position_y = monPT->position_y;
	directBall = 64;	//the ball's motionless 
}

//-------------------------------------------------------------------------------
// initialize ball image
//-------------------------------------------------------------------------------
void technoBall::pointe_GFX()
{
	animOffset = ball_sizeX + ballPowerX;
	change_GFX(animOffset);
}

//-------------------------------------------------------------------------------
// accelerate the ball
//-------------------------------------------------------------------------------
void technoBall::accelerate()
{
	if(--speedCount > 1) return;
	speedCount = speed_init;
	if(speedBallT == ballSpeed1)
	{	speedBallT = ballSpeed2;
		//printf("technoBall::accelerate() : ballSpeed2\n");
		return;
	}
	if(speedBallT == ballSpeed2)
	{	speedBallT = ballSpeed3;
		//printf("technoBall::accelerate() : ballSpeed3\n");
		return;
	}
	if(speedBallT == ballSpeed3)
	{	speedBallT = ballSpeed4;
		//printf("technoBall::accelerate() : ballSpeed4\n");
		return;
	}
	//printf("technoBall::accelerate() : maxi speed\n");
}

//-------------------------------------------------------------------------------
// Liste des points de collision de la balle (avec les briques) ================
//-------------------------------------------------------------------------------
Sint32 technoBall::brikPoint1[8] =
{ 10, 6,                                                       //XMAXIM,MILIEU (balle1)
  6, 0,                                                        //MILIEU,YMINIM
  0, 6,                                                        //XMINIM,MILIEU
  6, 10                                                        //MILIEU,YMAXIM
};
Sint32 technoBall::brikPoint2[8] =
{ 14, 8,                                                       // XMAXIM,MILIEU (balle2)
  8, 0,                                                        // MILIEU,YMINIM
  0, 8,                                                        // XMINIM,MILIEU
  8, 14                                                        // MILIEU,YMAXIM
};
Sint32 technoBall::brikPoint3[8] =
{ 18, 10,                                                      // XMAXIM,MILIEU (balle3)
  10, 0,                                                       // MILIEU,YMINIM
  0, 10,                                                       // XMINIM,MILIEU
  10, 18                                                       // MILIEU,YMAXIM
};

// Table des differentes vitesses de balle
//              16
//            20/\ 12
//          24  ||   08
//        28    ||     04
//      32<=====64=====> 00
//        36    ||    60
//          40  ||  56
//            44\/52 
//              48
//==============================================================================

Sint16 technoBall::ballSpeed1[] =
{	+2, 0,	//00
	+2,-1,	//04
	+2,-2,	//08
	+1,-2,	//12
	 0,-2,	//16
	-1,-2,	//20
	-2,-2,	//24
 	-2,-1,	//28
	-2, 0,	//32
	-2,+1,	//36
	-2,+2,	//40
	-1,+2,	//44
	 0,+2,	//48
 	+1,+2,	//52
 	+2,+2,	//56
	+2,+1,	//60
	0, 0	//64
};
Sint16 technoBall::ballSpeed2[] =
{	3,0,3,-1,2,-2,1,-3,0,-3,-1,-3,-2,-2,-3,-1,-3,
	0,-3,1,-2,2,-1,3,0,3,1,3,2,2,3,1,0,0
};

Sint16 technoBall::ballSpeed3[] =
{	4,0,4,-1,3,-3,1,-4,0,-4,-1,-4,-3,-3,-4,-1,-4,
	0,-4,1,-3,3,-1,4,0,4,1,4,3,3,4,1,0,0
};
	
Sint16 technoBall::ballSpeed4[] =
{	5,0,5,-2,4,-4,2,-5,0,-5,-2,-5,-4,-4,-5,-2,
	-5,0,-5,2,-4,4,-2,5,0,5,2,5,4,4,5,2,0,0
};

furaxEject technoBall::furaxTable[] =
{	{1, 0, 0, 0, 8, 8},		//1 top-left
	{0, 1, 0, 0, 8, 3},		//3 bottom-left
	{0, 0, 1, 0, 3, 3},		//4 bottom-right
	{0, 0, 0, 1, 3, 8}		//2 top-right
};

const Sint32 technoBall::tilt_table[16][16] =
{	{4, 4,  8, 12, 16, 20, 24, 28, 28, 36, 40, 44, 48, 52, 56, 60},	//0	32
	{8, 8,  8, 12, 16, 20, 24, 28, 28, 40, 40, 44, 48, 52, 56, 60},	//4	36
	{4, 4, 12, 12, 16, 20, 24, 28, 28, 36, 44, 44, 48, 52, 56, 60},	//8	40
	{4, 4,  8, 16, 16, 20, 24, 28, 28, 36, 40, 48, 48, 52, 56, 60},	//12	44
	{4, 4,  8, 12, 20, 20, 24, 28, 28, 36, 40, 44, 52, 52, 56, 60},	//16	48
	{4, 4,  8, 12, 16, 24, 24, 28, 28, 36, 40, 44, 48, 56, 56, 60},	//20	52
	{4, 4,  8, 12, 16, 20, 20, 28, 28, 36, 40, 44, 48, 52, 60, 60},	//24	56
	{4, 4,  8, 12, 16, 20, 24, 24, 36, 36, 40, 44, 48, 52, 56, 56},	//28	60
	{4, 4,  8, 12, 16, 20, 24, 28, 28, 36, 40, 44, 48, 52, 56, 60},	//32	0
	{8, 8,  8, 12, 16, 20, 24, 28, 28, 40, 40, 44, 48, 52, 56, 60},	//36	4
	{4, 4, 12, 12, 16, 20, 24, 28, 28, 36, 44, 44, 48, 52, 56, 60},	//40	8
	{4, 4,  8, 16, 16, 20, 24, 28, 28, 36, 40, 40, 48, 52, 56, 60},	//44	12
	{4, 4,  8, 12, 12, 20, 24, 28, 28, 36, 40, 44, 44, 52, 56, 60},	//48	16
	{4, 4,  8, 12, 16, 24, 24, 28, 28, 36, 40, 44, 48, 56, 56, 60},	//52	20
	{4, 4,  8, 12, 16, 20, 20, 28, 28, 36, 40, 44, 48, 52, 52, 60},	//56	24
	{4, 4,  8, 12, 16, 20, 24, 24, 36, 36, 40, 44, 48, 52, 56, 56}	//60	28
};
