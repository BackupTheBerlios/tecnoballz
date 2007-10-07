***************************************************************************
* ADRESSES UTILISEES PAR LES ROUTINES BOBS
***************************************************************************
		CNOP	0,4
bob32_xbyte	dc.l	0,0,"XBYT",0	table des pos. x en octets (évite x/8 & $fffe)
bob32_bltcon	dc.l	0,0,"BLTC",0	table des valeurs de bltcon
bob32_ymulu	dc.l	0,0,"YMUL",0	table mulu (évite y*SLINE)
bob32_pagegfx	dc.l	0,0,"PBOB",2	adr. page gfx avec tous les bobs (raw-blitter)
bob32_pagemsk	dc.l	0,0,"MBOB",2	adr. page msk des bobs (raw-blitter)
bob32_liste	dc.l	0		liste de tous les bobs
bob32_struct	dc.l	0,BOB.SIZE*BOB32_MAXI,"SBOB",0	structures bobs

*»»»»»»»» Table de pointeurs / listes affichage "bob32_widthXX" ««««««««««*
* ¶ Premier  pointeur sur le 0.w de fin de liste d'affichage
* ¶ Deuxième pointeur sur la liste des fenêtres blitters
*		a+0	nbre de fenêtre blitter
*		a+2	fenêtre blitter 1
*		a+6	pt/liste aff. 1ere structure bob + 2 avec cette fb
*		a+10	pt/liste aff. dernière struct. bob + 4 avec cette fb
bob32_pt16	dc.l	0,0
bob32_pt32	dc.l	0,0
bob32_pt48	dc.l	0,0
bob32_pt64	dc.l	0,0
bob32_pt80	dc.l	0,0
bob32_pt96	dc.l	0,0
bob32_pt112	dc.l	0,0
bob32_pt128	dc.l	0,0
bob32_pt144	dc.l	0,0
bob32_pt160	dc.l	0,0
bob32_pt176	dc.l	0,0
bob32_pt192	dc.l	0,0
bob32_pt208	dc.l	0,0
bob32_pt224	dc.l	0,0
bob32_pt240	dc.l	0,0
bob32_pt256	dc.l	0,0
vbob32_pt16	dc.l	0,0		idem mais pour les bobs verticaux
vbob32_pt32	dc.l	0,0
vbob32_pt48	dc.l	0,0
vbob32_pt64	dc.l	0,0
vbob32_pt80	dc.l	0,0
vbob32_pt96	dc.l	0,0

*»»»»»»»»»»»»»» Pointeurs / listes de restauration du décor ««««««««««««««*
* Disposition d'une structure 'bob32_listesXX' :
* a	* modulos A, D
* a+4	* fenêtre blitter
* a+6	* nbre de bobs-1 avec cette fenêtre blitter
* a+8	* adresse relative d'affichage du bob 1
* a+12	* adresse relative d'affichage du bob 2
* a+16	* adresse relative d'affichage du bob 3
* etc...
* a+n	* fenêtre blitter
* a+n+2	* nbre de bobs-1 avec cette fenêtre blitter
* a+n+4	* adresse relative d'affichage du bob 1
* a+n+8	* adresse relative d'affichage du bob 1
* etc...
* si fenêtre blitter = 0 alors fin de la liste
bob32_listes16:	dc.l	0,0		liste écran1, liste écran 2
bob32_listes32:	dc.l	0,0
bob32_listes48:	dc.l	0,0
bob32_listes64:	dc.l	0,0
bob32_listes80:	dc.l	0,0
bob32_listes96:	dc.l	0,0
bob32_listes112	dc.l	0,0
bob32_listes128	dc.l	0,0
bob32_listes144	dc.l	0,0
bob32_listes160	dc.l	0,0
bob32_listes176	dc.l	0,0
bob32_listes192	dc.l	0,0
bob32_listes208	dc.l	0,0
bob32_listes224	dc.l	0,0
bob32_listes240	dc.l	0,0
bob32_listes256	dc.l	0,0
vbob32_listes16	dc.l	0,0
vbob32_listes32	dc.l	0,0
vbob32_listes48	dc.l	0,0
vbob32_listes64	dc.l	0,0
vbob32_listes80	dc.l	0,0
vbob32_listes96	dc.l	0,0

*»»»»»» Liste des pointeurs sur les structures des bobs à afficher «««««««*
* disposition d'une structure 'bob32_width??' *
* a	* modulo C, B, A, D
* a+8	* adresse de 'bob32_listes??' écran 1
* a+12	* adresse de 'bob32_listes??' écran 2
* a+16	* fenêtre blitter
* a+18	* nbre de bobs-1 avec cette fenêtre blitter
* a+20	* adresse structure du bob1
* a+24	* adresse structure du bob2
* a+28	* adresse structure du bob3
* etc...
* a+n	* fenêtre blitter
* a+n+2	* nbre de bobs-1 avec cette fenêtre blitter
* a+n+4	* adresse structure du bob1
* etc...
* si fenêtre blitter = 0 alors fin de la liste
bob32_width16:	dc.l	0
bob32_width32:	dc.l	0
bob32_width48:	dc.l	0
bob32_width64:	dc.l	0
bob32_width80:	dc.l	0
bob32_width96:	dc.l	0
bob32_width112	dc.l	0
bob32_width128	dc.l	0
bob32_width144	dc.l	0
bob32_width160	dc.l	0
bob32_width176	dc.l	0
bob32_width192	dc.l	0
bob32_width208	dc.l	0
bob32_width224	dc.l	0
bob32_width240	dc.l	0
bob32_width256:	dc.l	0
vbob32_width16:	dc.l	0
vbob32_width32:	dc.l	0
vbob32_width48:	dc.l	0
vbob32_width64:	dc.l	0
vbob32_width80:	dc.l	0
vbob32_width96:	dc.l	0

****************************** STRUCTURE BOB ******************************
		IFNE	FLAG_BOBSTRUCT
		RSRESET
BOB:		rs	0
BOB.ACTIF	rs.l	1 ^	0=n'affiche pas le bob 
BOB.POSX	rs.w	1 ^	position x du bob en pixels
BOB.POSY	rs.w	1 ^	position y du bob en lignes
BOB.GFX		rs.l	1 ^	adresse du dessin du bob
BOB.MSK		rs.l	1 ^	adresse du masque du bob
BOB.PT		rs.l	1 ^	adr. de la structure bob
BOB.NUM		rs.w	1 !	numéro du bob
BOB.SPEED	rs.w	1	Vitesse du BOB
BOB.FREE	rs.w	1	0 = bob libre
BOB.TEMPO	rs.w	1 ^	temporisation en 50ieme de seconde
BOB.INITTEMPO	rs.w	1 ^	valeur initiale tempo. en 50ieme de sec.
BOB.PTANIM	rs.w	1 ^	pointeur sur l'animation de 8 en 8
BOB.PTMAXI	rs.w	1 ^	valeur maximum de PTANIM
BOB.OFFSET	rs.w	1 ^	offset de déplacement 8 ou -8
BOB.TABLE	rs.l	1 !	adr. de la table des gfx & msk de chaque anim.
BOB.WIDTH	rs.w	1	largeur du bob en pixels
BOB.HEIGHT	rs.w	1	hauteur du bob en lignes
BOB.POSX1	rs.w	1 ^
BOB.POSY1	rs.w	1 ^
BOB.POSX2	rs.w	1 ^
BOB.POSY2	rs.w	1 !
BOB.SIZE	rs	0
		ENDC

*»»»»»»»»»»»»»»»»»»»»»»»»»»»» Table des bobs «««««««««««««««««««««««««««««*
* La table bob (bob32_liste) contient les paramètres de chaque bob.
* Elle est inialisé par la routine 'bob32_init_liste
		RSRESET
TBOB:		rs	0
TBOB_WINDOW	rs.w	1	ø	fenêtre blitter du bob
TBOB_WINDOW2	rs.w	1		fenêtre blitter du bob ombres
TBOB_WIDTH	rs.w	1		largeur en mots du bob
TBOB_HEIGHT	rs.w	1		hauteur du bobs en lignes
TBOB_NUMBER	rs.w	1		nombre d'animation - 1
TBOB_LARGE	rs.w	1		largeur du BOB en pixels
TBOB_TABLE	rs.l	1		table des adr. des gfx et des msk

* Début de la table bob
* a	*	'SBOB' = indentifiacteur ou 'IBOB' si table initialisée
* a+2	*	nombre de bobs - 1
* a+4	*	adresse du bob numéro 1 - a (relogée par bob32_init_liste)
* a+8	*	adresse du bob numéro 2 - a
* a+12	*	adresse du bob numéro 3 - a
* etc...

* Exemple d'un bob non initialisé
* b1	*	0
* b1+02	*	largeur en mots
* b1+04	*	hauteur en lignes
* b1+06	*	nombre d'animation(s)-1
* b1+08	*	position x en mots bob anim. 1
* b1+10	*	position y en lignes du bob anim. 1
* b1+12	*	0,0
* b1+16	*	position x en mots bob anim. 2
* b1+18	*	position y en lignes du bob anim. 2
* b1+20	*	0,0
* etc...

* Exemple d'un bob initialisé
* b1	*	fenêtre blitter
* b1+02	*	largeur en mots
* b1+04	*	hauteur en lignes
* b1+06	*	nombre d'animation(s)-1
* b1+08	*	adresse du desssin du bob anim. 1
* b1+12	*	adresse du masque du bob anim. 1
* b1+16	*	adresse du desssin du bob anim. 1
* b1+20	*	adresse du masque du bob anim. 1
* etc...
