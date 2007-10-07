***************************************************************************
****************************AFFICHE LES CREDITS****************************
***************************************************************************
pub3:
		move.w	(score.n-t,a5),d0	n° du caractère à afficher
		lea	(credit5-t,a5),a0	pt/100aine de millier
		sub.w	d0,a0
		sub.w	d0,a0
		move.w	(a0),d1
		move.l	(fontex.pagegfx-t,a5),a3
		moveq	#0,d3
		move.w	(PG_SPLAN,a3),d3
		lea	(PG_DATA,a3,d1),a3
		move.l	(rak.lutin0.4-t,a5),a0
		move.l	(rak.lutin1.4-t,a5),a1
		lea	(2+16,a0,d0),a0
		lea	(2+16,a1,d0),a1
		moveq	#0,d0
		moveq	#7,d1
.bree		move.b	(a3),(a0)		Plan 1
		addq.l	#8,a0
		add.l	d3,a3
		move.b	(a3),(a0)		Plan 2
		addq.l	#8,a0
		add.l	d3,a3
		move.b	(a3),(a1)		Plan 3
		addq.l	#8,a1
		add.l	d3,a3
		move.b	(a3),(a1)		Plan 4
		addq.l	#8,a1
		add.l	d3,a3
		dbf	d1,.bree

		*/ Animation */
		lea	(sscap-t,a5),a3
		move.l	(rak.lutin0.4-t,a5),a0
		move.l	(rak.lutin1.4-t,a5),a1
		bsr.s	.techno
		tst.w	(rak.inver-t,a5)	Inverseur en cours ?
		beq	.crooo			-> non
		lea	(ssinv-t,a5),a3
		move.l	(rak.lutin2.2-t,a5),a0
		move.l	(rak.lutin3.2-t,a5),a1
.techno		lea	(16,a0),a0
		lea	(16,a1),a1
		move.l	a0,(SS.LUTIN0,a3)
		move.l	a1,(SS.LUTIN1,a3)
		move.l	(SS.ADR,a3),a0
		lea	(SS.PT,a3),a2
		add.l	(a2)+,a0
		subq.w	#1,(a2)
		bne.s	.goto
		move.w	(SS.INIT,a3),(a2)
		move.l	(SS.TAILLE2,a3),d0
		add.l	d0,-(a2)
		move.l	(SS.SIZ,a3),d0
		cmp.l	(a2),d0
		bne.s	.goto
		clr.l	(a2)
.goto		move.l	a0,a1
		add.l	(SS.TAILLE,a3),a1
		move.w	(SS.HEIGHT,a3),d1
		move.l	(SS.LUTIN0,a3),a2
		move.l	(SS.LUTIN1,a3),a3
.lala		move.w	(a0)+,(a2)
		addq.l	#8,a2
		move.w	(a0)+,(a2)
		addq.l	#8,a2
		move.w	(a1)+,(a3)
		addq.l	#8,a3
		move.w	(a1)+,(a3)
		addq.l	#8,a3
		dbf	d1,.lala
.crooo		rts
