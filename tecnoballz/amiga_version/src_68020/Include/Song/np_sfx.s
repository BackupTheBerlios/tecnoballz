*-------------------------------------------------------------------------*
*               Détermine les bruitages à émettre 	 		  *
*-------------------------------------------------------------------------*
np_sfx:		cmp.w	#1,(musicsound-t,a5)	Bruitages 4 cannaux ?
		beq	z4cannaux

		*/ 1 CANNAL */
		lea	(sound+1-t,a5),a0
		lea	(np0,pc),a1
		lea	(sound-t,a5),a2
		moveq	#1,d1			Commence par le son 1
		moveq	#KOFFER+1,d2
		moveq	#0,d3
.boaa_boaa:	tst.b	(a0)			Son à jouer ?
		bne.s	.flobert		-> oui
.poaa_poaa	addq.l	#1,a0
		addq.w	#1,d1			Son++
		cmp.w	d2,d1			Dernier son ?
		bne.s	.boaa_boaa		-> Non
		rts
.tanpis		move.b	d3,(a0)
		bra.s	.poaa_poaa
.flobert	move.w	d1,d4
		lsl.w	#4,d4
		move.w	(10+xt-t,a5,d4),d4	d4=priorité du son
		tst.w	(NP_WAIT,a1)		prise ?
		bne.s	.papa_1			-> oui
		tst.w	(a1)
		bne.s	.papa_1
		move.w	d1,(a1)
		move.b	d3,(a0)
		move.w	d4,(NP_PRIO,a1)
		bra.s	.poaa_poaa
.papa_1		cmp.w	(NP_PRIO,a1),d4		son prioritaire ?
		bge.s	.tanpis			-> non
		move.w	d1,(a1)			d1=NP_NEWS
		move.b	d3,(a0)
		move.w	d4,(NP_PRIO,a1)
		bra.s	.poaa_poaa

		*/ 4 CANNAUX */
z4cannaux	move.l	a6,-(a7)
		lea	(np0,pc),a1
		lea	(np1,pc),a2
		lea	(np2,pc),a3
		lea	(np3,pc),a4
		lea	(sound+1-t,a5),a0
		lea	(sound-t,a5),a6
		moveq	#1,d1			Commence par le son 1
		moveq	#KOFFER+1,d2
		moveq	#0,d3
.boaa_boaa:	tst.b	(a0)			Son à jouer ?
		bne.s	.flobert		-> oui
.poaa_poaa	addq.l	#1,a0
		addq.w	#1,d1			Son++
		cmp.w	d2,d1			Dernier son ?
		bne.s	.boaa_boaa		-> Non
		move.l	(a7)+,a6
		rts
		*-=- test si voix est prise -=-*
.flobert	move.w	d1,d4
		lsl.w	#4,d4
		move.w	(10+xt-t,a5,d4),d4	d4=priorité du son
		tst.w	(NP_WAIT,a1)		voix 3 prise ?
		bne.s	.papa_1			-> oui
		tst.w	(a1)
		bne.s	.papa_1
		move.w	d1,(a1)
		move.b	d3,(a0)
		move.w	d4,(NP_PRIO,a1)
		bra.s	.poaa_poaa
.papa_1		tst.w	(NP_WAIT,a2)		voix 2 prise ?
		bne.s	.papa_2			-> oui
		tst.w	(a2)
		bne.s	.papa_2
		move.w	d1,(a2)
		move.b	d3,(a0)
		move.w	d4,(NP_PRIO,a2)
		bra.s	.poaa_poaa
.papa_2		tst.w	(NP_WAIT,a3)		voix 1 prise ?
		bne.s	.papa_3			-> oui
		tst.w	(a3)
		bne.s	.papa_3
		move.w	d1,(a3)
		move.b	d3,(a0)
		move.w	d4,(NP_PRIO,a3)
		bra.s	.poaa_poaa
.papa_3		tst.w	(NP_WAIT,a4)		voix 0 prise ?
		bne.s	.papa_4			-> oui
		tst.w	(a4)
		bne.s	.papa_4
		move.w	d1,(a4)
		move.b	d3,(a0)
		move.w	d4,(NP_PRIO,a4)
		bra.s	.poaa_poaa
		*-=- les 4 voix sont prises, test priorité -=-*
.papa_4		cmp.w	(NP_PRIO,a1),d4		son prioritaire ?
		bge.s	.noel_1			-> non
		move.w	(a1),d0			d0=NP_NEWS
		move.b	#1,(a6,d0)
		move.w	d1,(a1)			d1=NP_NEWS
		move.b	d3,(a0)
		move.w	d4,(NP_PRIO,a1)
		bra.s	.poaa_poaa
.noel_1		cmp.w	(NP_PRIO,a2),d4		son prioritaire ?
		bge.s	.noel_2			-> non
		move.w	(a2),d0
		move.b	#1,(a6,d0)
		move.w	d1,(a2)
		move.b	d3,(a0)
		move.w	d4,(NP_PRIO,a2)
		bra	.poaa_poaa
.noel_2		cmp.w	(NP_PRIO,a3),d4		son prioritaire ?
		bge.s	.noel_3			-> non
		move.w	(a3),d0
		move.b	#1,(a6,d0)
		move.w	d1,(a3)
		move.b	d3,(a0)
		move.w	d4,(NP_PRIO,a3)
		bra	.poaa_poaa
.noel_3		cmp.w	(NP_PRIO,a4),d4		son prioritaire ?
		bge	.poaa_poaa		-> non
		move.w	(a4),d0
		move.b	#1,(a6,d0)
		move.w	d1,(a4)
		move.b	d3,(a0)
		move.w	d4,(NP_PRIO,a4)
		bra	.poaa_poaa
