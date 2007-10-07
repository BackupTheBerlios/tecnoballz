*»»»»»»»»»»»»»»»                                        ««««««««««««««««««*
*»»»»»»»»»»»»»» ADRESSES UTILISEES PAR LES ROUTINES SBOB «««««««««««««««««*
*»»»»»»»»»»»»»»»                                        ««««««««««««««««««*
		CNOP	0,4
sbob_xtable	dc.l	0,0,"XBOB",0	Pos. x octets + valeur de BPLCON1 & 2
sbob_ytable	dc.l	0,0,"YBOB",0	Adresse relative des lignes de la page écran
ssha_xtable	dc.l	0,0,"XBOB",0	Pos. x octets + valeur de BPLCON1 & 2
ssha_ytable	dc.l	0,0,"YBOB",0	Adresse relative des lignes de la page écran
sbob_pagegfx	dc.l	0,0,"PBOB"	Page des  SBOBs
		dc.l	MEMF_CHIP
sbob_pagemsk	dc.l	0,0,"MBOB"	Page Masques des SBOBs
		dc.l	MEMF_CHIP
sbob_liste	dc.l	0		Liste de tous les SBOBs
sbob_struct	dc.l	0,0,"SBOB",0	Structures SBOBs

*»»»»»»»» Table de pointeurs / listes affichage "sbob_widthXX" «««««««««««*
* ¶ "sbob_pt16"   =>  pointeur sur la fin de liste d'affichage "sbob_widthXX"
* ¶ "sbob_pt16+4" => pointeur sur la liste des "last_table"
* STRUCTURE D'UNE LISTE "LAST TABLE" 
*		a+0	nbre de "last_table" de 0 à N
*		a+4	"last_table1"
*		a+8	"sbob_widthXX" adressse début liste affichage avec
*			cette "last_table" + 4 octets (on saute la 
*			"last_table)
*		a+12	"sbob_widthXX" adressse fin liste affichage avec
*			cette "last_table"
SBOB_PTSIZE	=12
sbob_pt16	dc.l	0,0
sbob_pt32	dc.l	0,0
sbob_pt48	dc.l	0,0
sbob_pt64	dc.l	0,0
sbob_pt80	dc.l	0,0
sbob_pt96	dc.l	0,0
sbob_pt112	dc.l	0,0
sbob_pt128	dc.l	0,0
sbob_pt144	dc.l	0,0
sbob_pt160	dc.l	0,0
sbob_pt176	dc.l	0,0
sbob_pt192	dc.l	0,0
sbob_pt208	dc.l	0,0
sbob_pt224	dc.l	0,0
sbob_pt240	dc.l	0,0
sbob_pt256	dc.l	0,0
vsbob_pt16	dc.l	0,0		idem mais pour les SBOBs verticaux
vsbob_pt32	dc.l	0,0
vsbob_pt48	dc.l	0,0
vsbob_pt64	dc.l	0,0
vsbob_pt80	dc.l	0,0
vsbob_pt96	dc.l	0,0
ssha_pt16	dc.l	0,0		idem mais pour les ombres
ssha_pt32	dc.l	0,0
ssha_pt48	dc.l	0,0
ssha_pt64	dc.l	0,0
ssha_pt80	dc.l	0,0
		ds.l	10*2
ssha_pt256	dc.l	0,0

*»»»»»»»»»»»»»» Pointeurs / listes de restauration du décor ««««««««««««««*
* Disposition d'une structure 'sbob_listesXX' :
* a	* modulos A & D
* a+4	* nbre de SBOB(s)-1
* a+8	* première adresse relative d'affichage du SBOB 1
* a+12  * première fenêtre Blitter du SBOB 1
* a+14  * deuxième fenêtre Blitter du SBOB 1 (si=0 alors un seul affichage)
* a+16  * deuxième adresse relative d'affichage du SBOB 1
* a+20	* première adresse relative d'affichage du SBOB 2
* a+24  * première fenêtre Blitter du SBOB 2
* a+26  * deuxième fenêtre Blitter du SBOB 2 (si=0 alors un seul affichage)
* a+28  * deuxième adresse relative d'affichage du SBOB 2
* etc...
sbob_listes16:	dc.l	0,0		liste écran1, liste écran 2
sbob_listes32:	dc.l	0,0
sbob_listes48:	dc.l	0,0
sbob_listes64:	dc.l	0,0
sbob_listes80:	dc.l	0,0
sbob_listes96	dc.l	0,0
sbob_listes112	dc.l	0,0
sbob_listes128	dc.l	0,0
sbob_listes144	dc.l	0,0
sbob_listes160	dc.l	0,0
sbob_listes176	dc.l	0,0
sbob_listes192	dc.l	0,0
sbob_listes208	dc.l	0,0
sbob_listes224	dc.l	0,0
sbob_listes240	dc.l	0,0
sbob_listes256:	dc.l	0,0
vsbob_listes16	dc.l	0,0
vsbob_listes32	dc.l	0,0
vsbob_listes48	dc.l	0,0
vsbob_listes64	dc.l	0,0
vsbob_listes80	dc.l	0,0
vsbob_listes96	dc.l	0,0
ssha_listes16:	dc.l	0,0		liste écran1, liste écran 2
ssha_listes32:	dc.l	0,0
ssha_listes48:	dc.l	0,0
ssha_listes64:	dc.l	0,0
ssha_listes80:	dc.l	0,0
		ds.l	10*2
ssha_listes256:	dc.l	0,0

*»»»»»» Liste des pointeurs sur les structures des SBOBs à afficher «««««««*
* disposition d'une structure 'sbob_width??' *
* a	* modulo C, B, A, D
* a+8	* adresse de 'sbob_listes??' écran 1
* a+12	* adresse de 'sbob_listes??' écran 2
* a+16	* adresse de la "last_table" (contient des fenêtres Blitter)
* a+20	* nbre de SBOBs-1 avec cette "last_table"
* a+24	* adresse structure du SBOB1
* a+28	* adresse structure du SBOB2
* a+32	* adresse structure du SBOB3
* etc...
* a+n	* adresse de la  "last_table"
* a+n+4	* nbre de SBOBs-1 avec cette "last_table"
* a+n+8	* adresse structure du SBOB1
* a+n+16* adresse structure du SBOB2
* etc...
* si "last_table" = 0 alors fin de la liste
sbob_width16:	dc.l	0
sbob_width32:	dc.l	0
sbob_width48:	dc.l	0
sbob_width64:	dc.l	0
sbob_width80:	dc.l	0
		ds.l	10
sbob_width256:	dc.l	0
vsbob_width16:	dc.l	0
vsbob_width32:	dc.l	0
vsbob_width48:	dc.l	0
vsbob_width64:	dc.l	0
vsbob_width80:	dc.l	0
vsbob_width96:	dc.l	0
ssha_width16:	dc.l	0
ssha_width32:	dc.l	0
ssha_width48:	dc.l	0
ssha_width64:	dc.l	0
ssha_width80:	dc.l	0
		ds.l	10
ssha_width256:	dc.l	0

****************************** STRUCTURE SBOB *****************************
		RSRESET
SBOB:		rs	0
SBOB.ACTIF	rs.l	1 ^	0=n'affiche pas le bob 
SBOB.POSX	rs.w	1 ^	position x du SBOB en pixels
SBOB.POSY	rs.w	1 ^	position y du SBOB en lignes
SBOB.GFX	rs.l	1 ^	adresse du dessin du SBOB
SBOB.MSK	rs.l	1 ^	adresse du masque du SBOB
SBOB.PT		rs.l	1 ^	adr. de la structure SBOB
SBOB.NUM	rs.w	1 !	numéro du bob
SBOB.SPEED	rs.w	1	Vitesse du SBOB
SBOB.FREE	rs.w	1	0 = SBOB libre
SBOB.TEMPO	rs.w	1 ^	temporisation en 50ieme de seconde
SBOB.INITTEMPO	rs.w	1 ^	valeur initiale tempo. en 50ieme de sec.
SBOB.PTANIM	rs.w	1 ^	pointeur sur l'animation de 8 en 8
SBOB.PTMAXI	rs.w	1 ^	valeur maximum de PTANIM
SBOB.OFFSET	rs.w	1 ^	offset de déplacement 8 ou -8
SBOB.TABLE	rs.l	1 !	adr. de la table des gfx & msk de chaque anim.
SBOB.WIDTH	rs.w	1	largeur du SBOB en pixels
SBOB.HEIGHT	rs.w	1	hauteur du SBOB en lignes
SBOB.POSX1	rs.w	1 ^
SBOB.POSY1	rs.w	1 ^
SBOB.POSX2	rs.w	1 ^
SBOB.POSY2	rs.w	1 !
SBOB.SIZE	rs.w	0

*»»»»»»»»»»»»»»»»»»»»»»»»»»»» Table des SBOBs ««««««««««««««««««««««««««««*
* La table SBOB (sbob_liste) contient les paramètres de chaque SBOB.
* Elle est inialisé par la routine 'sbob_init_liste'
		RSRESET
TSBOB		rs	0	
TSBOB_WINDOW	rs.w	1	ø	Fenêtre blitter du SBOB dessin
TSBOB_WINDOW2	rs.w	1		Fenêtre blitter du SBOS ombres
		rs.w	1		* inutilisé *
TSBOB_WIDTH	rs.w	1		Largeur en mots du SBOB
TSBOB_HEIGHT	rs.w	1		Hauteur du SBOBs en lignes
TSBOB_LARGE	rs.w	1		Largeur en pixels du SBOB
TSBOB_HAUT	rs.w	1		Hauteur en lignes du SBOB
TSBOB_NUMBER	rs.w	1		Nombre d'animation - 1
TSBOB_LASTTABLE	rs.l	1	ø	LastTable des dessins
TSBOB_LASTSHADO	rs.l	1	ø	LastTable des ombres
TSBOB_TABLE	rs.l	1		table des adr. des gfx et des msk

* Début de la table SBOB
* a	*	'SBOB' = indentificteur ou 'IBOB' si table initialisée
* a+2	*	nombre de SBOBs - 1
* a+4	*	adresse du SBOB numéro 1 - a (relogée par sbob_init_liste)
* a+8	*	adresse du SBOB numéro 2 - a
* a+12	*	adresse du SBOB numéro 3 - a
* etc...

* Exemple d'un SBOB non initialisé
* b1	* W	0
* b1+02 * W	0
* b1+04 * W	0
* b1+06	* W	largeur en mots
* b1+08	* W	hauteur en lignes
* b1+10	* W	largeur en pixels (collision)
* b1+12 * W	hauteur en lignes (collision)
* b1+14	* W 	nombre d'animation(s)-1
* b1+16 * L	0
* b1+20	* L	0
* b1+24	* W	position x en mots du SBOB anim. 1
* b1+26	* W	position y en lignes du SBOB anim. 1
* b1+28	* L	0
* b1+32	* W	position x en mots SBOB anim. 2
* b1+34	* W	position y en lignes du SBOB anim. 2
* b1+36	* L	0
* etc...

* Exemple d'un SBOB initialisé
* b1	* W	Fenêtre Blitter GFX   (tous les plans)
* b1+02 * W	Fenêtre Blitter Ombre (un seul plan)
* b1+04 * W	*Inutilisé*
* b1+06	* W	largeur en mots
* b1+08	* W	hauteur en lignes
* b1+10	* W	largeur en pixels (collision)
* b1+12 * W	hauteur en lignes (collision)
* b1+14	* W 	nombre d'animation(s)-1
* b1+16 * L	LastTable des dessins
* b1+20	* L	LastTable des ombres
* b1+24	*	adresse du dessin du SBOB anim. 1
* b1+28	*	adresse du masque du SBOB anim. 1
* b1+32	*	adresse du dessin du SBOB anim. 1
* b1+36	*	adresse du masque du SBOB anim. 1
* etc...
