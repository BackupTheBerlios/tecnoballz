***************************************************************************
*----------* DONNEES UTILISEES POUR L'AFFICHAGE DE CARACTERES *-----------*
***************************************************************************
		CNOP	0,4
hedgehog	dc.l	0,0,"PRI4",0	Routine PRINT 4 couleurs
pos.bricks	dc.l	0		Adresse relative écran
pos.score	dc.l	0
pos.lifes	dc.l	0
score.n		dc.w	0		n° caract. score à afficher 
brick.n		dc.w	0		n° caract. nbre brik à afficher 
lifes.n		dc.w	0		n° caract. nbre de vie à afficher

*/ PRIX OPTIONS MAGASINS */
prix0		dc.w	0		prix million
prix1		dc.w	0		prix centaine de millier 
prix2		dc.w	0		prix millier
prix3		dc.w	0		prix centaine
prix4		dc.w	0		prix dizaine
prix5		dc.w	0		prix unité

* CREDIT DE MONNAIES DU JOUEUR *
credit0		dc.w	0		crédit unité
credit1		dc.w	0		crédit dizaine
credit2		dc.w	0		crédit centaine
credit3		dc.w	0		crédit millier
credit4		dc.w	0		crédit centaine de millier 
credit5		dc.w	0		crédit million
credit		dc.w	0		crédit du joueur

* NOMBRE DE BRIQUES A DETRUIRE *
brick0		dc.w	0		nbre brik ascii unité
brick1		dc.w	0		nbre brik ascii dizaine
brick2		dc.w	0		nbre brik ascii centaine

* SCORE DU JOUEUR *
score0		dc.w	0		score ascii unité
score1		dc.w	0		score ascii dizaine
score2		dc.w	0		score ascii centaine
score3		dc.w	0	 	score ascii millier
score4		dc.w	0		score ascii 10aine millier
score5		dc.w	0		score ascii 100aine millier
		CNOP	0,4
life0		dc.w	0	^	nbre vie unité
life1		dc.w	0	!	nbre vie décimal
lev0		dc.w	0	^	n° tablo unité
lev1		dc.w	0	!	n° tablo dizaine

name.x		ds.b	6		nom du joueur en cours de partie
joueur0:	ds.b	6		nom du meilleur joueur
score00:	ds.b	6		score du meilleur joueur

*/ CODE ASCII => CODE J */
convert:
	dc.b	'A','A'-65
	dc.b	'B','B'-65
	dc.b	'C','C'-65
	dc.b	'D','D'-65
	dc.b	'E','E'-65
	dc.b	'F','F'-65
	dc.b	'G','G'-65
	dc.b	'H','H'-65
	dc.b	'I','I'-65
	dc.b	'J','J'-65
	dc.b	'K','K'-65
	dc.b	'L','L'-65
	dc.b	'M','M'-65
	dc.b	'N','N'-65
	dc.b	'O','O'-65
	dc.b	'P','P'-65
	dc.b	'Q','Q'-65
	dc.b	'R','R'-65
	dc.b	'S','S'-65
	dc.b	'T','T'-65
	dc.b	'U','U'-65
	dc.b	'V','V'-65
	dc.b	'W','W'-65
	dc.b	'X','X'-65
	dc.b	'Y','Y'-65
	dc.b	'Z','Z'-65
	dc.b	' ','['-65
	dc.b	'0','\'-65
	dc.b	'1',']'-65
	dc.b	'2','^'-65
	dc.b	'3','_'-65
	dc.b	'4','`'-65
	dc.b	'5','a'-65
	dc.b	'6','b'-65
	dc.b	'7','c'-65
	dc.b	'8','d'-65
	dc.b	'9','e'-65
	dc.b	'!','f'-65
	dc.b	'.','g'-65
	dc.b	"'",'h'-65
	dc.b	':','i'-65
	dc.b	',','j'-65
	dc.b	'-','k'-65
cend:
