*////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
*//////////////////////// Collisions Balles/Atoms \\\\\\\\\\\\\\\\\\\\\\\\*
*////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
* Entrée	=> a5.l	Adr. 't'
atom_collision1:
		move.l	a6,-(a7)
		lea	(gemeau-t,a5),a6
		move.w	(hasard-t,a5),d3
		and.w	#$F,d3
		lsl.w	#2,d3			*4
		move.w	#NBALLE-1,d6
		lea	(ghost_trajec-t,a5),a1
		move.l	(pt_atom-t,a5),a2
		move.l	(pt_balle-t,a5),a3
.loop		tst.w	(a3)			balle active ?
		beq	.no_col_ghost		-> non
		tst.w	(BALLE.ATOM,a3)		collision activé ?
		bne	.no_col_ghost		-> non
		move.w	(BOB.WIDTH,a3),d0
		subq.w	#2,d0
		move.w	d0,d1
		move.w	(BALLE.X,a3),d4		x balle
		add.w	d4,d0
		sub.w	#10,d4
		move.w	(BALLE.Y,a3),d5		y balle
		add.w	d5,d1
		sub.w	#13,d5
		move.l	a2,a4
		moveq	#NATOM-1,d7
.loop_automne	tst.w	(ATOM.ACTIF,a4)		Atom actif ?
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
		bsr	venus2			atom touché score + 100
		add.w	#100,(score.life-t,a5)
		move.w	(BALLE.PT,a3),d2	puissance balle 2, 4, ou 6
		add.w	d2,d2
		sub.w	d2,(ATOM.POWER,a4)	Diminue resistane atom
		bpl.s	.no_dead		-> encore vivant
		move.l	(ATOM.X,a4),(a6)+
		move.l	(BALLE.RAK,a3),(a6)+
		move.w	(atom.reapp-t,a5),(ATOM.ACTIF,a4)
		move.w	(atom.resist-t,a5),(ATOM.POWER,a4)
		move.b	#1,(sound+S_ATOM.EXPLO-t,a5)
		clr.w	(BOB.PTANIM,a4)
		move.w	#1,(ATOM.EXPLO,a4)
		move.w	(ATOM.OEXP,a4),(ATOM.OFF,a4)
.no_dead	move.w	d3,(2+BALLE.DIRECT,a3)	nvelle direct. balle
		move.l	(a1,d3),(ATOM.INC,a4)
.loop_ghost	lea	(ATOM.SIZE,a4),a4
		dbf	d7,.loop_automne
.no_col_ghost	lea	(BALLE.SIZE,a3),a3
		dbf	d6,.loop
		move.l	a6,(gemelle-t,a5)
		move.l	(a7)+,a6
		rts
