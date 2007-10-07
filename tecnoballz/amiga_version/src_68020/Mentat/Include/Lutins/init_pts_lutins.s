*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--
*ð********* INITIALISE LES POINTEURS LUTINS DE LA LISTE COPPER **********ð*
*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--
* Entrée	=> a5.l	Adr. 't'
init_pts_lutins:
		move.l	(pt_screen-t,a5),a0
		move.l	(SALUTIN1,a0),d0
		beq.s	.exit
		move.l	d0,a1
		tst.w	(NBCOPPER,a0)		une seule liste Copper ?
		beq.s	.one_copperlist		-> oui
		move.w	(flag_flip-t,a5),d0	d0=flag "double buff." 0 ou 4
		lea	(SALUTIN1,a0),a1
		move.l	(a1,d0),a1
.one_copperlist	addq.l	#2,a1
		lea	(table_lutins-t,a5),a2	a2=Adr. table des 8 adresses
		moveq	#8*2-1,d0
.loop		move.w	(a2)+,(a1)
		addq.l	#4,a1
		dbf	d0,.loop
.exit		rts

********** Initialise les pointeurs lutins des 2 listes Copper ************
* Entrée	=> a5.l	Adr. 't'
init_pts_lutins2:
		move.l	(pt_screen-t,a5),a0
		move.l	(SALUTIN1,a0),d0
		beq.s	.exit
		move.l	d0,a1
		move.l	d0,a2
		tst.w	(NBCOPPER,a0)		une seule liste Copper ?
		beq.s	.one_copperlist		-> oui
		move.l	(SALUTIN2,a0),a2
.one_copperlist	addq.l	#2,a1
		addq.l	#2,a2
		lea	(table_lutins-t,a5),a3	a3=Adr. table des 8 adresses
		moveq	#8*2-1,d0
.loop		move.w	(a3),(a1)
		move.w	(a3)+,(a2)
		addq.l	#4,a1
		addq.l	#4,a2
		dbf	d0,.loop
.exit		rts
