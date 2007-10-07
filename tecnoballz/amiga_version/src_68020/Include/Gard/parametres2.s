*-------------------------------------------------------------------------*
*                          Table des paramètres				  *
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	adr. table 't'
parametres2:
		IFEQ	A_TAB
		move.w	(hard-t,a5),d7
		addq.w	#1,d7
		move.w	(area-t,a5),d0		N° area de 1 à 5
		subq.w	#1,d0			n° area = n° area - 1
		mulu	#12*4,d0		12 levels par area
		move.w	(level-t,a5),d1		n° level de 1 à 12
		subq.w	#1,d1
		add.w	d1,d1
		add.w	d1,d1			n° level * 4
		add.w	d1,d0
		lea	(giga_amiga-t,a5),a0	table pt/paramètres des levels
		move.l	(a0,d0),a0		pt/table paramètre
		move.l	a5,d5
		add.l	d5,a0
		move.l	(a0),(balle_speed-t,a5)
		add.l	d5,(balle_speed-t,a5)
		move.w	(4,a0),(balle_start-t,a5)
		move.w	(6,a0),(balle_tilt-t,a5)
		move.w	(8,a0),(gd.stemp-t,a5)
		move.w	(10,a0),d0		d0=fréquence laché gadgets
		mulu	d7,d0
		move.w	d0,(malus.frek-t,a5)
		move.l	(12,a0),(table.malus-t,a5)
		add.l	d5,(table.malus-t,a5)
		ENDC
		IFNE	A_TAB
		lea	(speed4-t,a5),a0
		move.l	a0,(balle_speed-t,a5)
		move.w	#10*50,d0
		move.w	d0,(balle_start-t,a5)
		move.w	d0,(balle_tilt-t,a5)
		clr.w	(gd.stemp-t,a5)
		move.w	#10,(malus.frek-t,a5)
		lea	(xcourse-t,a5),a0
		move.l	a0,(table.malus-t,a5)
		ENDC
		clr.w	(malus.step-t,a5)
		rts
