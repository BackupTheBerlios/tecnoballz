*=========================================================================*
**************************** Affiche Inverseur ****************************
*=========================================================================*
* entrée	=> a5.l	adr. table 't'
pub1:
		tst.w	(rak.inver-t,a5)	Inverseur en cours ?
		beq	.crooo			-> non
		moveq	#BOB.MONNAI,d0
		lea	(sscap-t,a5),a3
		subq.w	#1,(rak.inver-t,a5)	Décrémente compteur
		subq.w	#1,(inv50-t,a5)
		bne.s	.plokama
		move.w	#2,(inv50-t,a5)
		moveq	#0,d6
		subq.w	#1,(inv0-t,a5)		unité - 1
		bpl.s	.plokama
		move.w	#9,(inv0-t,a5)
		subq.w	#1,(inv1-t,a5)
		bpl.s	.plokama
		move.w	d6,(inv0-t,a5)
		move.w	d6,(inv1-t,a5)
.plokama	
		*/ AFFICHE CHIFFRES DANS LISTE LUTINS */
		move.l	(rak.lutin2.2-t,a5),a0
		move.l	(rak.lutin3.2-t,a5),a1
		lea	(16+2,a0),a0
		lea	(16+2,a1),a1
		move.l	(fontex.pagegfx-t,a5),a3
		moveq	#0,d3
		move.w	(PG_SPLAN,a3),d3
		move.w	(inv1-t,a5),d0
		move.w	(inv0-t,a5),d1
		lea	(PG_DATA,a3,d0),a2
		lea	(PG_DATA,a3,d1),a3
		moveq	#0,d0
		moveq	#7,d1

.bree		move.b	(a2),(a0)+
		move.b	(a3),(a0)+
		move.w	d0,(a0)+
		move.l	d0,(a0)+
		add.l	d3,a2
		add.l	d3,a3

		move.b	(a2),(a0)+
		move.b	(a3),(a0)+
		move.w	d0,(a0)+
		move.l	d0,(a0)+
		add.l	d3,a2
		add.l	d3,a3

		move.b	(a2),(a1)+
		move.b	(a3),(a1)+
		move.w	d0,(a1)+
		move.l	d0,(a1)+
		add.l	d3,a2
		add.l	d3,a3

		move.b	(a2),(a1)+
		move.b	(a3),(a1)+
		move.w	d0,(a1)+
		move.l	d0,(a1)+
		add.l	d3,a2
		add.l	d3,a3

		dbf	d1,.bree
.crooo		rts
