***************************************************************************
*************************EFFACE TOUTES LES BRIQUES*************************
***************************************************************************
* Entrée	=> a5.l	Adr. 't'
clear_allbricks:
		lea	(bri_pnt-t,a5),a2
		move.w	(brick.save-t,a5),d2
		add.w	d2,a2
		lea	(2+set_table-t,a5),a0
		lea	(2+set_guide-t,a5),a1
		moveq	#0,d0
		moveq	#1,d1
		move.w	#BRI.SIZE*BNP,d3
		moveq	#BRI.SIZE,d4
		move.w	#512,d5
		move.w	#BK.X*BK.Y-1,d6
.loop		tst.w	(a0)
		beq.s	.nobrick
		move.w	d0,(a0)			Efface code brique
		move.w	d1,(BRI.FLAG,a2)	Flag réaafiche décor
		move.w	(-2,a0),(BRI.NUM,a2)	Numéro de la brique
		move.w	(a1),(BRI.ADR,a2)	Adresse relative
		move.w	d5,(BRI.X,a2)		flag briks détruit au mégablizt
		add.w	d4,a2
		add.w	d4,d2
		cmp.w	d3,d2
		blt.s	.nobrick
		moveq	#0,d2
		lea	(bri_pnt-t,a5),a2
.nobrick	addq.l	#4,a0
		addq.l	#4,a1
		dbf	d6,.loop
		move.w	d2,(brick.save-t,a5)
		rts
