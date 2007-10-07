***************************************************************************
********************ADRESSES UTILISEES PAR LES MESSAGES********************
***************************************************************************
* TAILLE MESSAGE	64 pixels / 6 lignes / 5 plans
		CNOP	0,4
mess.fond	dc.l	0,0,"REST",MEMF_CHIP
mess.pos	dc.l	0	Adresse relative écran du message
mess.mod	dc.l	0	Modulo A & D (affichage bande de 64 pixels)
mess.req	dc.l	0	Pointeur sur le message
mess.win	dc.w	0	Fenêtre Blitter
mess.pause	dc.w	0	Pause message si <5 alors on efface
mess.pt		dc.w	0	pt/lettre à afficher

*/ Messages apparaissant sur l'écran de jeu */
* A à Z	: A à Z
* [	: espace
* \	: point d'exclamation
* ]	: point d'interrogation
* ^	: point		 
mes0	dc.b	'[[[[[[[[[[[[[[[[' 	 0
	dc.b	'[ARE[YOU[READY[]'	 1
	dc.b	'[YEAH[[YOU[WERE\'	 2
	dc.b	'[[[[[[[[[[[[[[[['	 3
	dc.b	'[[[SIMPLE[FIRE[['	 4
	dc.b	'[[[COOL[FIRE[[\['	 5
	dc.b	'[[COOL[GUY[[\\[['	 6
	dc.b	'[[[[[[[[[[[[[[[['	 7
	dc.b	'[[[[[[[[[[[[[[[['	 8
	dc.b	'[[[[[[[[[[[[[[[['	 9
	dc.b	'[YOU[LOST[A[LIFE'	10
	dc.b	'[YOU[WON[A[LIFE\'	11
	dc.b	'[[[[[[[[[[[[[[[['	12
	dc.b	'[[[[[[[[[[[[[[[['	13
	dc.b	'[[[[TOO[LATE^^^['	14
	dc.b	'[BONUS[PRICE[[^['	15
	dc.b	'[[GLUE[[OPTION[['	16
	dc.b	'[[NEXT[LEVEL^^^['	17
	dc.b	'[[[MALUS[SIZE[[['	18
	dc.b	'[[[BONUS[SIZE[[['	19
	dc.b	'[[BONUS[BALLS[[['	20
	dc.b	'[[[MULTIBALLS[[['	21
	dc.b	'[[[POWERBALLS[[['	22
	dc.b	'[MEGAPOWERBALLS['	23
	dc.b	'[INVERSOR[MALUS['	24
	dc.b	'[SPEEDBALLS[MAXI'	25
	dc.b	'[[BUMPER[RIGHT[['	26
	dc.b	'[[[BUMPER[[UP[[['	27
	dc.b	'[[BUMPER[[LEFT[['	28
	dc.b	'[BIGSIZE[[BALLS['	29
	dc.b	'[MAXISIZE[BALLS['	30
	dc.b	'[[MEGA[OPTIONS[['	31
	dc.b	'[[WALL[[ENABLE[['	32
	dc.b	'[[ROBOT[ENABLE[['	33
	dc.b	'[CONTROL[[BALLS['	34
message		ds.b	40
mg2

*/ position des lettres des messages avec décalage */
mes_love:	dc.w	0,0,0,0,0,0,0,0
		dc.w	0,0,0,0,0,0,0,0
		dc.w	0,0,0,0,0,0,0,0
		dc.w	0,0,0,0,0,0,0,0

*/ position des 6 lignes des messages à effacer une par une */
mes_rikiki:	dc.l	0,0
		dc.l	0,0
		dc.l	0,0
		dc.l	0,0
		dc.l	0,0
		dc.l	0,0
