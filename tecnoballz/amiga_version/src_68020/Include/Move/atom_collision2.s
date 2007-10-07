*////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
*///////////////////////// Collisions Tirs&Atoms \\\\\\\\\\\\\\\\\\\\\\\\\*
*////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
* Entrée	=> a5.l	Adr. 't'
atom_collision2:
		move.l	a6,-(a7)
		move.l	(gemelle-t,a5),a6
		move.w	(fire.flag-t,a5),d3	puissance tir 0 ou 1
		addq.w	#1,d3
		add.w	d3,d3
		move.w	#NTIR*4-1,d6
		move.l	(pt_atom-t,a5),a2
		move.l	(pt_objet-t,a5),a3
.loop		tst.w	(a3)			tir actif ?
		beq	.no_col_ghost		-> non
		move.w	(FIRE.X,a3),d4
		move.w	d4,d0
		addq.w	#3,d0			5-2
		sub.w	#10,d4
		move.w	(FIRE.Y,a3),d5
		move.w	d5,d1
		addq.w	#3,d1			5-2
		sub.w	#13,d5
		move.l	a2,a4
		moveq	#NATOM-1,d7
.loop_automne	tst.w	(ATOM.ACTIF,a4)		atom actif ?
		bne.s	.loop_ghost		-> non
		move.l	(ATOM.X,a4),d2
		cmp.w	d1,d2
		bhi.s	.loop_ghost
		sub.w	d5,d2
		blt.s	.loop_ghost
		swap	d2
		cmp.w	d0,d2
		bhi.s	.loop_ghost
		sub.w	d4,d2
		blt.s	.loop_ghost
		move.b	#1,(sound+S_TOUCHE.ATOM-t,a5)
		bsr	venus2
		add.w	#100,(score.life-t,a5)
		move.l	(FIRE.RAK,a3),a0	struct. raqu. tireuse
		cmp.w	#64,(rak.large-t,a5)	big raquette ?
		bne.s	.abrutit		-> non
		cmp.w	#1,(FIRE.ACTIF,a3)	scie circulaire fixée ?
		beq.s	.forfavor		-> oui
.abrutit	clr.w	(FIRE.ACTIF,a3)		flag tir bob non actif
		move.w	(BUMP.X,a0),(FIRE.X,a3)
		move.w	(BUMP.Y,a0),(FIRE.Y,a3)
.forfavor	sub.w	d3,(ATOM.POWER,a4)	déc. rest. atom
		bpl.s	.loop_ghost		-> encore vivant
		move.l	(ATOM.X,a4),(a6)+
		move.l	(FIRE.RAK,a3),(a6)+
		move.w	(atom.reapp-t,a5),(ATOM.ACTIF,a4)
		move.w	(atom.resist-t,a5),(ATOM.POWER,a4)
		move.b	#1,(sound+S_ATOM.EXPLO-t,a5)
		move.w	#1,(ATOM.EXPLO,a4)
		move.w	(ATOM.OEXP,a4),(ATOM.OFF,a4)
		clr.w	(BOB.PTANIM,a4)
.loop_ghost	lea	(ATOM.SIZE,a4),a4
		dbf	d7,.loop_automne
.no_col_ghost	lea	(FIRE.SIZE,a3),a3
		dbf	d6,.loop
		bsr	moulu
		move.l	(a7)+,a6
		rts

*-----------*  Si des Atoms détruits alors envoie recompense  *-----------*
moulu:	
		move.w	(hasard-t,a5),d5
		moveq	#0,d6
		moveq	#NATOM-1,d7
		lea	(gemeau-t,a5),a6
.loop		move.l	(a6)+,d0
		beq.s	.exit
		move.l	d6,(-4,a6)
		move.l	(a6)+,a1
		move.w	d5,d2
		and.w	#$F,d2
		jsr	([.list,pc,d2*4])
.next		add.w	d7,d5
		dbf	d7,.loop
.exit		or.w	d5,(hasard-t,a5)
		rts
		CNOP	0,4
.list:		dc.l	add_gemme	0
		dc.l	.cap		1
		dc.l	.malus		2
		dc.l	.cap		3
		dc.l	.malus		4
		dc.l	.malus		5
		dc.l	.cap		6
		dc.l	.malus		7
		dc.l	.malus		8
		dc.l	.malus		9
		dc.l	.cap		10
		dc.l	.malus		11
		dc.l	.malus		12
		dc.l	.malus		13
		dc.l	.malus		14
		dc.l	.cap		15
*------------------------> On envoie une monnaie <------------------------*
.cap		move.l	(pt_capsule-t,a5),a0
		moveq	#NMONNAIE-1,d2
.capsloop	tst.w	(a0)
		beq.s	.etb_yoyo
		lea	(CAP.SIZE,a0),a0
		dbf	d2,.capsloop
		rts
.etb_yoyo	move.l	d0,(CAP.X,a0)
		move.w	#1,(CAP.ACTIF,a0)
		move.l	(BUMP.MONNAIE,a1),(CAP.DEP,a0)
		add.w	d2,d5
		move.w	(hasard2-t,a5),d0
		add.w	d2,d0
		and.w	#$3,d0
		lea	(cap.val-t,a5,d0*4),a1
		move.w	(a1)+,(CAP.VAL,a0)
		move.w	(a1),(CAP.SPEED,a0)
		rts
*-------------------------> On envoie un Malus <---------------------------*
.malus:		move.l	(pt_gadget-t,a5),a0
		moveq	#NGADGET-1,d2
.xloop		tst.w	(a0)
		beq.s	.toto_what
		lea	(GAD.SIZE,a0),a0
		dbf	d2,.xloop
		rts
.toto_what	move.w	#1,(a0)
		move.l	d0,(GAD.X,a0)
		move.l	a1,(GAD.RAK,a0)
		move.l	(BUMP.GADGET,a1),(GAD.DEP,a0)
		add.w	d2,d5
		move.w	(hasard2-t,a5),d0
		add.w	d2,d0
		and.w	#63,d0
		move.l	(table.malus-t,a5),a1
		move.w	(a1,d0*2),d0
		move.w	d0,(GAD.NUM,a0)
		move.w	(pascal-t,a5,d0),(GAD.PTBOB,a0)
		rts
