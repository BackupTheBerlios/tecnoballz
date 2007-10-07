*=========================================================================*
*»»»»»»»»»»»»»»»»»Routine de déplacement lettres GAMEOVER«««««««««««««««««*
*=========================================================================*
* Entrée	=> a5.l	adr. table 't'
oo_aventure:
		lea	(.mouton_bleu,pc),a0
		move.w	(go.dep-t,a5),d0	n° routine déplacement
		move.l	(a0,d0),a0
		add.l	a5,a0
		jmp	(a0)
.mouton_bleu	dc.l	aventure1-t		00 déplacement sinus GAME-OVER
		dc.l	aventure2-t		04 déplacement x a zero
		dc.l	aventure3-t		08 déplacement y final
		dc.l	aventure4-t		12 déplacement x final

********************/ déplacement 8 lettres en sinus /*********************
* entrée	=> a5.l	adr. table 't'
aventure1:	
		lea	(go.list-t,a5),a3
		moveq	#NLETTRE-1,d7
		lea	(sinus_over-t,a5),a0
		move.w	#zeus_over-sinus_over,d5
		moveq	#0,d6
		moveq	#32,d4
.z_tsee0	move.l	(a3)+,a4
		addq.w	#1,(BOB.POSX1,a4)
		addq.w	#1,(BOB.POSY1,a4)
		cmp.w	(BOB.POSX1,a4),d5
		bhi.s	.z_tsee1
		move.w	d6,(BOB.POSX1,a4)
.z_tsee1	cmp.w	(BOB.POSY1,a4),d5
		bhi.s	.z_tsee2
		move.w	d6,(BOB.POSY1,a4)
.z_tsee2	move.l	d6,(BOB.POSX,a4)
		move.w	(BOB.POSX1,a4),d0
		move.b	(a0,d0),(1+BOB.POSX,a4)
		move.w	(BOB.POSY1,a4),d0
		move.b	(a0,d0),(1+BOB.POSY,a4)
		add.w	d4,(BOB.POSX,a4)
		add.w	d4,(BOB.POSY,a4)
		dbf	d7,.z_tsee0
		subq.w	#1,(go.tempo-t,a5)
		bpl.s	.z_tsee3
		move.w	#4,(go.dep-t,a5)		routine dépl. y final
.z_tsee3	rts

******************Lettres se déplacent en x sur les bords******************
* entrée	=> a5.l	adr. table 't'
aventure2:	
		lea	(go.list-t,a5),a3
		moveq	#NLETTRE-1,d7
		lea	(sinus_over-t,a5),a0
		move.w	#zeus_over-sinus_over,d5
		moveq	#0,d6
		moveq	#32,d4
.zz_tzee0	move.l	(a3)+,a4
		addq.w	#1,(BOB.POSY1,a4)
		cmp.w	(BOB.POSY1,a4),d5
		bhi.s	.zz_tzee2
		move.w	d6,(BOB.POSY1,a4)
.zz_tzee2	move.w	d6,(BOB.POSY,a4)
		move.w	(BOB.POSY1,a4),d0
		move.b	(a0,d0),(1+BOB.POSY,a4)
		add.w	d4,(BOB.POSY,a4)
		dbf	d7,.zz_tzee0

		*- déplace les lettres G,A,M,E -*
		moveq	#0,d5
		lea	(go.list-t,a5),a3
		moveq	#(NLETTRE/2)-1,d7
		moveq	#32,d0
.flane1		move.l	(a3)+,a4
		lea	(BOB.POSX,a4),a4
		cmp.w	(a4),d0
		bge.s	.lane1
		moveq	#1,d5
		subq.w	#1,(a4)
		bra.s	.rane1
.lane1		move.w	d0,(a4)
.rane1		dbf	d7,.flane1

		*- déplace les lettres O,V,E,R -*
		moveq	#(NLETTRE/2)-1,d7
		move.w	#192,d0
.flane2		move.l	(a3)+,a4
		lea	(BOB.POSX,a4),a4
		cmp.w	(a4),d0
		bls.s	.lane2
		addq.w	#1,(a4)
		moveq	#1,d5
		bra.s	.rane2
.lane2		move.w	d0,(a4)
.rane2		dbf	d7,.flane2

 		*- test si lettres sont arrivées -*
		tst.w	d5
		bne.s	.zz_tzee77 
		move.w	#8,(go.dep-t,a5)		routine dépl. x final
.zz_tzee77	rts

****************Lettres se déplacent en y vers. pos. final*****************
* Entrée	=> a5.l	adr. table 't'
aventure3:	
		lea	(go.list-t,a5),a3
		lea	(sinus_over-t,a5),a0
		move.w	#zeus_over-sinus_over,d5
		moveq	#0,d6
		moveq	#0,d3
		moveq	#32,d4
		moveq	#NLETTRE/2-1,d7
.jz_tzee0	move.l	(a3)+,a4
		addq.w	#1,(BOB.POSY1,a4)
		cmp.w	(BOB.POSY1,a4),d5
		bhi.s	.jz_tzee1
		move.w	d6,(BOB.POSY1,a4)
.jz_tzee1	cmp.w	(BOB.POSY,a4),d4
		blt.s	.jz_tzee2
		addq.w	#1,d3
		move.w	d4,(BOB.POSY,a4)
		bra.s	.jz_tzee3
.jz_tzee2	move.w	d6,(BOB.POSY,a4)
		move.w	(BOB.POSY1,a4),d0
		move.b	(a0,d0),(1+BOB.POSY,a4)
		add.w	d4,(BOB.POSY,a4)
.jz_tzee3	dbf	d7,.jz_tzee0
		moveq	#32,d4
		moveq	#NLETTRE/2-1,d7
.mz_tzee0	move.l	(a3)+,a4
		addq.w	#1,(BOB.POSY1,a4)
		cmp.w	(BOB.POSY1,a4),d5
		bhi.s	.mz_tzee1
		move.w	d6,(BOB.POSY1,a4)
.mz_tzee1	cmp.w	#191,(BOB.POSY,a4)
		blt.s	.mz_tzee2
		addq.w	#1,d3
		move.w	#192,(BOB.POSY,a4)
		bra.s	.mz_tzee3
.mz_tzee2	move.w	d6,(BOB.POSY,a4)
		move.w	(BOB.POSY1,a4),d0
		move.b	(a0,d0),(1+BOB.POSY,a4)
		add.w	d4,(BOB.POSY,a4)
.mz_tzee3	dbf	d7,.mz_tzee0

 		*- test si lettres sont arrivées -*
		cmp.w	#8,d3
		bne.s	.mz_tzee888 
		move.w	#12,(go.dep-t,a5)
.mz_tzee888	rts

*************** Lettres se déplacent en x vers. pos. final ****************
* Entrée	=> a5.l	adr. table 't'
aventure4:	
		* déplace les lettres G,A,M,E *
		lea	(go.list-t,a5),a3
		moveq	#35,d5
		moveq	#(NLETTRE/2)-1,d7
.flone1		move.l	(a3)+,a4
		lea	(BOB.POSX,a4),a4
		cmp.w	(a4),d5
		beq.s	.xlon
		bge.s	.xlone1
		subq.w	#2,(a4)
.xlone1		addq.w	#1,(a4)
.xlon		add.w	#54,d5
		dbf	d7,.flone1

		*- déplace les lettres O,V,E,R -*
		moveq	#35,d5
		moveq	#(NLETTRE/2)-1,d7
.ylone1		move.l	(a3)+,a4
		lea	(BOB.POSX,a4),a4
		cmp.w	(a4),d5
		beq.s	.ylon
		bge.s	.lylone1
		subq.w	#2,(a4)
.lylone1	addq.w	#1,(a4)
.ylon		add.w	#54,d5
		dbf	d7,.ylone1
		rts
