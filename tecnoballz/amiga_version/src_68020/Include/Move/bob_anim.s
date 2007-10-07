*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*************************** Animation des BOBS ****************************
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
* Entrée	=> a5.l	Adr. 't'
bob_anim:

		*/ BALLES */
		move.l	(pt_balle-t,a5),a1
		move.l	(BOB.TABLE,a1),d1	d1=table adr. gfx & msk
		moveq	#NBALLE-1,d4
.loop1		move.w	(BOB.WIDTH,a1),d2
		subq.w	#5,d2
		mulu	#12,d2
		move.w	(BALLE.PT,a1),d0		2 / 4 / 6
		subq.w	#2,d0
		add.w	d0,d0 
		add.w	d0,d0 				0 / 8 / 16
		add.w	d2,d0
		move.w	d0,(BOB.PTANIM,a1)
		move.l	d1,a0			a0=table adr. gfx & msk
		add.w	d0,a0
		move.l	(a0)+,(BOB.GFX,a1)
		move.l	(a0),(BOB.MSK,a1)
		lea	(BALLE.SIZE,a1),a1
		dbf	d4,.loop1

		*/ MONNAIE */
		move.l	(pt_capsule-t,a5),a3
		moveq	#NMONNAIE-1,d4
.loop2		bsr.l	bob32_simple_anim
		lea	(CAP.SIZE,a3),a3
		dbf	d4,.loop2

		*/ GADGET */
		move.l	(pt_gadget-t,a5),a4
		moveq	#NGADGET-1,d4
.loop3		subq.w	#1,(GAD.TEMPO,a4)	tempo=tempo-1
		bne.s	.noamim			-> tempo > 0
		move.w	#3,(GAD.TEMPO,a4)
		move.w	(GAD.PTANIM,a4),d0	d0=pt/animation
		cmp.w	#7*8,d0			pt < au pt maxi ?
		bne.s	.no_maxi		-> oui
		moveq	#0,d0
		move.w	d0,(GAD.PTANIM,a4)
.no_maxi	addq.w	#8,(GAD.PTANIM,a4)	
		move.l	(BOB.TABLE,a4),a0	a0=table adr. gfx & msk
		add.w	d0,a0
		add.w	(GAD.PTBOB,a4),a0	pt/le bonus
		move.l	(a0)+,(BOB.GFX,a4)
		move.l	(a0),(BOB.MSK,a4)
.noamim		lea	(GAD.SIZE,a4),a4
		dbf	d4,.loop3

		*/ TIR */
		move.l	(pt_objet-t,a5),a4
		move.l	(BOB.TABLE,a4),d1	d1=table adr. gfx & msk
		moveq	#4*8,d2
		moveq	#NTIR*4-1,d4
.loop4		tst.w	(FIRE.ACTIF,a4)
		beq.s	.nonono
		subq.w	#1,(FIRE.TEMPO,a4)	tempo=tempo-1
		bne.s	.nonono			-> tempo > 0
		move.w	#6,(FIRE.TEMPO,a4)
		move.w	(FIRE.PTANIM,a4),d0	d0=pt/animation
		cmp.w	d2,d0			pt < au pt maxi ?
		bne.s	.no_maxi2		-> oui
		moveq	#0,d0
		move.w	d0,(FIRE.PTANIM,a4)
.no_maxi2	addq.w	#8,(FIRE.PTANIM,a4)	
		move.l	d1,a0			a0=table adr. gfx & msk
		add.w	d0,a0
		add.w	(FIRE.PTBOB,a4),a0	pt/le bonus
		move.l	(a0)+,(BOB.GFX,a4)
		move.l	(a0),(BOB.MSK,a4)
.nonono		lea	(FIRE.SIZE,a4),a4
		dbf	d4,.loop4
		rts
