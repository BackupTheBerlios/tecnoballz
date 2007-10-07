*=========================================================================*
*                   Initialisation des 6 listes joueurs			  *
*=========================================================================*
* Entrée	=> a5.l	adr. table 't'
raton_laveur:	
		move.w	(area-t,a5),d5		N° area de 1 à 5
		subq.w	#1,d5
		lea	(4+liste_joueur-t,a5),a2
		moveq	#NJOUEUR-1,d2
.criterium	move.l	(a2)+,a0

		*-=- RAZ liste des options  -=-*
		lea	(J.COURSE,a0),a1
		moveq	#0,d0
		moveq	#NOPTIONS-1,d1
.rasputin	move.b	d0,(a1)+
		dbf	d1,.rasputin
		move.w	d0,(J.OPT,a0)

		*-=- RAZ le crédit -=-*
		lea	(J.CREDIT0,a0),a1
		moveq	#0,d0
		moveq	#6-1,d1
.xrasputin	move.w	d0,(a1)+
		dbf	d1,.xrasputin

		*-=- Active le flag de chaque brique de côté -=-*
		lea	(J.MLEFT,a0),a1
		moveq	#1,d0
		move.w	#BRICOT.NUM*3-1,d1
.rasputim	move.b	d0,(a1)+
		dbf	d1,.rasputim

		*-=- Donnne 500 crédit au départ -=-*
		move.w	#5,(J.CREDIT2,a0)	Crédit centaine = 5
		move.w	#500,(J.TOTAL,a0)

		*-=- raz le score -=-*
		moveq	#0,d0
		move.l	d0,(J.SCORE,a0)
		move.w	d0,(4+J.SCORE,a0)

		*-=- met le n° area à 1 -=-*
		move.b	#1,(J.AREA,a0)

		*-=- RAZ n° du tableau -=-*
		move.b	d0,(J.LEV,a0)
		move.b	d0,(1+J.LEV,a0)

		*-=- RAZ le nbre de vie -=-*
		move.b	d0,(J.LIFE,a0)	
		move.b	d0,(1+J.LIFE,a0)

		*-=- RAZ les flags des raquettes droite/haut/gauche -=-*
		move.w	d0,(J.BUMP2,a0)
		move.w	d0,(J.BUMP3,a0)
		move.w	d0,(J.BUMP4,a0)

		*-=- RAZ flag 'rebuild mur' -=-*
		move.w	d0,(J.REBUILD,a0)

		*-=- RAZ flag 'less bricks' -=-*
		move.w	d0,(J.LESS,a0)

		*-=- RAZ compteur points vie gratuite -=-*
		move.w	d0,(J.BONUS,a0)

		*-=- Init. largeur de la raquette -=-*
		move.w	#32,(J.LARGE,a0)

		*-=- RAZ PRICE BONUS -=-*
		move.w	d0,(J.PRIX,a0)

		*-=- RAZ état des gemmes *-=-
		lea	(J.GEMME,a0),a1
		move.l	d0,(a1)+
		move.w	d0,(a1)
		move.w	d0,(J.GEMMEX,a0)
		dbf	d2,.criterium
		rts
