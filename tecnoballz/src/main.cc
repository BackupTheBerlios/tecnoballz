//*****************************************************************************
// copyright (c) 1991-2004 TLK Games all rights reserved
//-----------------------------------------------------------------------------
// file         : "main.cc"
// created      : 2002-08-21
// updates      : 2004-10-11
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
// Place - Suite 330, Boston, MA  02111-1307, USA.
//-----------------------------------------------------------------------------


//------------------
// TOUT DOUX LISTE :
//------------------

// *******
// GENERAL
// *******
// - essayer de faire des balles plus jolies
// - faire le automake / autoconf
// - CTRL + S : son uniquement

// *****
// MENU
// ****

// ********
// TABLEAUX
// ********
// [?] refaire une reselection des fonds
// [?] finir de retoucher les briques
// [?] figer le degrade des briques pour les harmoniser avec le fond 
//		(figer aussi le jeu de brique ?)
// [?] faire un directeur de balle + grand (64*64 au lieu de 32*32)

// *******
// MAGASIN
// *******

// *******
// GARDIEN
// *******
//

//*****************************************************************************
#include "../include/mentatCode.h"
#include "../include/ecran_hard.h"
#include "../include/audiomixer.h"
//#include "SDL/SDL.h"
//..............................................................................
Sint32 scanZeArgs(Sint32 _iNombreArgument, char **ptArg);

//------------------------------------------------------------------------------
// main function
//------------------------------------------------------------------------------
int main(Sint32 nbArg, char **ptArg)
{ 
	if(!scanZeArgs(nbArg, ptArg))
		return 0;
	if(mentatCode::is_verbose) 
	{	printf("===========================================================\n");
		printf("                       TECNOBALLZ START\n");
		printf("===========================================================\n");
	}
	Sint32 error = 0;
	error = mentatCode::first_init();
	if(error) return error;
	error = mentatCode::game_begin();
	if(error) return error;
	if(mentatCode::is_verbose) 
		printf("===========================================================\n");
	error = mentatCode::desinstall();
	if(error) return error;
	if(mentatCode::is_verbose) 
		printf("END =======================================================\n");
	return error;
}

//------------------------------------------------------------------------------
// scan_arguments       : analyse les arguementsé 
// _iNombreArgument     : taille de la chaine d'argument
// ptArg          : pointeur sur le pointeur contenat la dite chaine
//------------------------------------------------------------------------------
Sint32 scanZeArgs(Sint32 nbArg, char **ptArg)
{
	//printf("scran %i args\n", nbArg);
	Sint32 _iIndex;
	for(_iIndex = 1; _iIndex < nbArg; _iIndex++)
	{	
		// arguments commence tous par un tiret
		if(*ptArg[_iIndex] != '-')
			continue;
		
	
		if(!strcmp(ptArg[_iIndex], "-h") ||
			!strcmp(ptArg[_iIndex], "--help"))
		{	printf("\noptions:\n");
			printf("-h, --help     print Help (this message) and exit\n");
			printf("--version      print version information and exit\n");
			printf("--full         full screen\n");
			printf("--320          game run in a 320*200 window\n");
			printf("--verbose      verbose mode\n");
			printf("--nosound      force no sound\n");
			printf("--------------------------------------------------------------\n");
			printf("keys recognized during the game:\n");
			printf("CTRL+S         enable/disable sound\n");
			printf("CTRL+D         enable/disable music\n");
			printf("CTRL+F         enable/disable sound effects\n");
			printf("CTRL+X         finish the play current\n");
			printf("CTRL+Q         return to the main menu\n");
			printf("CTRL+ESC       quit TecnoballZ\n");
			printf("P              enable/disable pause\n");
			printf("F              full screen/window mode\n");
			printf("\n");
			return 0;
		}
	
		if(!strcmp(ptArg[_iIndex], "--version"))
		{	printf(TECNOBALLZ_VERSION);
			printf("\n");
			printf("copyright (c) 1991-2004 TLK Games\n");
			printf("website: http://linux.tlk.fr\n");
			return 0;
		}

		if(!strcmp(ptArg[_iIndex], "--full"))
		{	ecran_hard::optionfull = -1;
			continue;
		}

		if(!strcmp(ptArg[_iIndex], "--window"))
		{	ecran_hard::optionfull = 0;
			continue;
		}
		
		if(!strcmp(ptArg[_iIndex], "--verbose") ||
			!strcmp(ptArg[_iIndex], "-v"))
		{	mentatCode::is_verbose = -1; 
			continue;
		}
		
		if(!strcmp(ptArg[_iIndex], "--320"))
		{	mentatCode::resolution = 1;
			continue;
		}

		if(!strcmp(ptArg[_iIndex], "--nosync"))
		{	ecran_hard::optionsync = 0;
			continue;
		}
#ifndef SOUNDISOFF		
		if(!strcmp(ptArg[_iIndex], "--sound"))
		{	audiomixer::audioactif = -1; 
			continue;
		}
		if(!strcmp(ptArg[_iIndex], "--nosound"))
		{	audiomixer::audioactif = 0; 
			continue;
		}
#endif
		// use 4 colors background in 640x480
		if(!strcmp(ptArg[_iIndex], "--bg4"))
		{	mentatCode::bg4_colors = 1; 
			continue;
		}

		//start at brick or guard level
		if(!strcmp(ptArg[_iIndex], "--guard"))
		{	mentatCode::arg_jumper = 3; 
			continue;
		}
		if(!strcmp(ptArg[_iIndex], "--brick"))
		{	mentatCode::arg_jumper = 1; 
			continue;
		}
		
	}
	return 1;
}


/* SOURCE ASSEMBLEUR 680x20 AMIGA

GARD/AFFICHE_.S affiche_dalles()      : affiche toutes les dalles à l'écran (scrolling)
GARD/INIT.S     init.sc_mapadr()      : Initialise la table "sc_mapadr" 
MOVE/GERE_BAL.S gere_balle:
                 -> collision_robot() : Collision Balles & le Robot 
                 -> speedball()       : Augmente les vitesses des balles   
                 -> vitus()           : test balle sort ecran / collisions avec la raquettes / colle / ejecteur
                 -> dancer()          : Gere les compteurs tilt (baillement + sonnerie)

PLAY2/TERUPT1.S                       -> jeu tableau

GFX1/TOTAL_RE.S                       -> total_recal()     : Gestion de la tête


PLAY2/ORIGINAL/SOURCES/MOVE/CHUNKY.S   -> chunky() Gère la barre témoin du GigaBlitz
                                                 Déplacement du GigaBlitz

PLAY2/ORIGINAL/SOURCES/MOVE/GAD_RE.S   -> gad_re() Collisions balles et briques 

PLAY2/ORIGINAL/SOURCES/GFX1/CHOUCROU.S -> choucroutte() Réaffiche briques/décors briques 

PLAY2/ORIGINAL/SOURCES/MOVE/ATOM_COL.S -> Collisions Balles/Atoms
PLAY2/ORIGINAL/SOURCES/MOVE/MOUSE_BL.S -> 
PLAY2/ORIGINAL/SOURCES/MOVE/BP_DEPL1.S -> deplacement des raquettes
PLAY2/ORIGINAL/SOURCES/MOVE/GERE_ROB.S -> 
INIT/INIT_RAK.S                        -> INITIALISATION DES RAQUETTES-BUMPERS

PRINT/PUB3.S                           -> AFFICHE LES CREDITS (dans les sprites)

MOVE/YES_WHAT.S                        -> Test si envoie Bonus/Malus
MOVE/NO_WHAT.S                         -> Test si envoie capsules de monnaie
MOVE/MSDOS_BE.S                        -> deplacement des capsules monnaie	(collision)
MOVE/MOUSE_BL.S                        -> Bouton gauche & droit => gère compteur Blitz
MOVE/GERE_BAL.S                        -> Gestion des balles, colle, rebond raquette, coins
*/




/*
==================================================================================================
==================================================================================================
==================================================================================================
==================================================================================================
==================================================================================================
==================================================================================================
 info locals
 Affiche les variables locals de la "frame" selectionnee, chacune separees par
 une ligne. Ce sont toutes les variables (declaree comme statiques ou
 automatiques) accessible a ce point de l'execution de la "frame" selectionnee

 l briqueCote::execution1()  
 Affiche les Lignes à partir du commencement de la fonction "briqueCote::execution1()"
 
 disassemble briqueCote::execution1
 
 x
 Vous pouvez utiliser la commande x (pour "examiner") pour examiner la memoire
 dans un des plusieurs formats, indépendamment des types de données de votre
 programme.
 
 x/nfu addr
 x addr
 x 
 
 n, f et u sont tous des paramètres facultatifs qui spécifient la quantite de 
 mémoire a afficher et comment la formater; addr est une expression donnant 
 le debut de adresse memoire que vous voulez afficher. Si vous employez les 
 valeurs par defauts pour nfu, vous n'avez pas besoin de taper le slash '/'.
 Plusieurs commandes mettent des défauts commodes pour addr.
 
 n, le compte de répétition
 Le compte de répétition est un entier décimal; le défaut est 1. Il indique
 la quantité de mémoire (comptant par des unités de u) à afficher.

 f, le format d'affichage
 Le format d'affichage est un des formats employés par "print", ou 's' 
 (pour une chaîne terminée par zéro) ou 'i' (pour instruction machine). 
 La valeur par défaut est 'x' (hexadécimal) au début, ou le
 format du dernier format utilisé pour x ou avec "print".

 u, la taille unitaire
    la taille unitaire est soit
  b octets 
  h demi mots (deux octets). 
  w mots (qautre octets). C'est la valeur initiale par défaut
  g double mots (huit octets). 

x/7 0x8183cf0
 
*/
