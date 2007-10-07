*X***************X**************************************X****************X*
*X***************X* INITIALISE DIFFICULTE DES TABLEAUX *X****************X*
*X***************X**************************************X****************X*
* Entrée	=> a5.l	Adr. 't'
init_level:
		move.w	(hard-t,a5),d7
		addq.w	#1,d7
		move.w	(area-t,a5),d0		n° area de 1 à 5
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

		*/ Apparition des Atoms */
		moveq	#0,d0
		move.w	(PA.ATOM1,a0),d0
		divu	d7,d0
		move.w	d0,(atom1.app-t,a5)	-084 temps d'apparition atom00

		moveq	#0,d0
		move.w	(PA.ATOM2,a0),d0
		divu	d7,d0
		move.w	d0,(atom2.app-t,a5)	-086 temps d'apparition atom01

		moveq	#0,d0
		move.w	(PA.ATOM3,a0),d0
		divu	d7,d0
		move.w	d0,(atom3.app-t,a5)	-088 temps d'apparition atom02

		moveq	#0,d0
		move.w	(PA.ATOM4,a0),d0
		divu	d7,d0
		move.w	d0,(atom4.app-t,a5)	-090 temps d'apparition atom03

		moveq	#0,d0
		move.w	(PA.REAPP,a0),d0
		divu	d7,d0
		move.w	d0,(atom.reapp-t,a5)	-092 temps de réapparition

		*/ Résistance des atoms */
		move.w	(PA.RESIS,a0),d0
		mulu	d7,d0
		move.w	d0,(atom.resist-t,a5) 	-094 résistance des atoms

		*/ Fréquence des lachés des Bonus */
		moveq	#0,d0
		move.b	(PA.FREK1,a0),d0
		divu	d7,d0
		move.w	d0,(malus.frek-t,a5)	-096 fréquence de laché de malus

		*/ Fréquence des lachés des Monnaies */
		moveq	#0,d0
		move.b	(PA.FREK2,a0),d0
		mulu	d7,d0
		move.w	d0,(monai.frek-t,a5)		-106 fréquence laché de monnaie

		move.l	(PA.MALUS,a0),(table.malus-t,a5) -100 table malus
		add.l	d5,(table.malus-t,a5)
		move.l	(PA.SPEED,a0),(balle_speed-t,a5) -104 première vitesse de balle
		add.l	d5,(balle_speed-t,a5)

		moveq	#0,d0
		move.w	(PA.COUNT,a0),d0
		divu	d7,d0
		move.w	d0,(balle_scount-t,a5) -108 temps avant que la balle accellère

		moveq	#0,d0
		move.w	(PA.TGLUE,a0),d0
		divu	d7,d0
		move.w	d0,(balle_glue-t,a5)	-112 temps que dure la colle

		move.w	(PA.START,a0),(balle_start-t,a5) -110 temps avant que balle parte
		move.w	(PA.TTILT,a0),(balle_tilt-t,a5)	-280 temps avant tilt possible
		rts


