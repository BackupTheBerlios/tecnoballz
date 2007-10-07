***************************************************************************
* ADRESSES UTILISEES PAR LES ROUTINES SHADOW
***************************************************************************

		CNOP	0,4
shado_xbyte	dc.l	0,0,"XBYs",0	table des pos. x en octets (évite x/8 & $fffe)
shado_bltcon	dc.l	0,0,"BLTs",0	table des valeurs de bltcon
shado_ymulu	dc.l	0,0,"YMUs",0	table mulu (évite y*SLINE)


*»»»»»»»» Table de pointeurs / listes affichage "shado_widthXX" ««««««««««*
* ¶ Premier  pointeur sur le 0.w de fin de liste d'affichage
* ¶ Deuxième pointeur sur la l	ste des fenêtres blitters
*		a+0	nbre de fenêtre blitter
*		a+2	fenêtre blitter 1
*		a+6	pt/liste aff. 1ere structure bob + 2 avec cette fb
*		a+10	pt/liste aff. dernière struct. bob + 4 avec cette fb
shado_pt16	dc.l	0,0
shado_pt32	dc.l	0,0
shado_pt48	dc.l	0,0
shado_pt64	dc.l	0,0
shado_pt80	dc.l	0,0
shado_pt96	dc.l	0,0
shado_pt112	dc.l	0,0
shado_pt128	dc.l	0,0
shado_pt144	dc.l	0,0
shado_pt160	dc.l	0,0
shado_pt176	dc.l	0,0
shado_pt192	dc.l	0,0
shado_pt208	dc.l	0,0
shado_pt224	dc.l	0,0
shado_pt240	dc.l	0,0
shado_pt256	dc.l	0,0

*»»»»»»»»»»»»»» Pointeurs / listes de restauration du décor ««««««««««««««*
* Disposition d'une structure 'shado_listesXX' :
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
shado_listes16:	dc.l	0,0		liste écran1, liste écran 2
shado_listes32:	dc.l	0,0
shado_listes48:	dc.l	0,0
shado_listes64:	dc.l	0,0
shado_listes80:	dc.l	0,0
shado_listes96:	dc.l	0,0
shado_listes112	dc.l	0,0
shado_listes128	dc.l	0,0
shado_listes144	dc.l	0,0
shado_listes160	dc.l	0,0
shado_listes176	dc.l	0,0
shado_listes192	dc.l	0,0
shado_listes208	dc.l	0,0
shado_listes224	dc.l	0,0
shado_listes240	dc.l	0,0
shado_listes256	dc.l	0,0

*»»»»»» Liste des pointeurs sur les structures des bobs à afficher «««««««*
* disposition d'une structure 'shado_width??' *
* a	* modulo C, B, A, D
* a+8	* adresse de 'shado_listes??' écran 1
* a+12	* adresse de 'shado_listes??' écran 2
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
shado_width16:	dc.l	0
shado_width32:	dc.l	0
shado_width48:	dc.l	0
shado_width64:	dc.l	0
shado_width80:	dc.l	0
shado_width96:	dc.l	0
shado_width112	dc.l	0
shado_width128	dc.l	0
shado_width144	dc.l	0
shado_width160	dc.l	0
shado_width176	dc.l	0
shado_width192	dc.l	0
shado_width208	dc.l	0
shado_width224	dc.l	0
shado_width240	dc.l	0
shado_width256:	dc.l	0
