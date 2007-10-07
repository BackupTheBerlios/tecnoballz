***************************************************************************
*---------------------------  Affiche un BOB  ----------------------------*
***************************************************************************
* Entrée	=> d0.w Numéro du BOB
*		=> d1.w	Position X
*		=> d2.w	Position Y
*		=> d3.w Animation
bob32.gfx:
		*/ Lit le BOB dans la liste */
		bsr.l	bob32_get.bob
		bne	.no_bob
		move.l	a0,a1			pt/le BOB

		*/ Calcul adresse écran */
		move.l	(pt_screen-t,a5),a0
		mulu	(SP_SLINE,a0),d2
		moveq	#0,d0
		move.w	d1,d0
		lsr.w	#3,d0
		and.w	#$FFFE,d0
		add.l	d0,d2
		add.l	(ecran_travail-t,a5),d2

		*/ Calcul BLTCON0 */		
		and.w	#$F,d1
		ror.w	#4,d1

		*/ Calcul modulos destinations C & D */
		move.w	(SP_SPLAN,a0),d0
		sub.w	(TBOB_WIDTH,a1),d0
		sub.w	(TBOB_WIDTH,a1),d0
		subq.w	#2,d0
.waitblitter	btst	#14,(DMACONR,a6)
		bne.s	.waitblitter
		move.l	#$FFFF0000,(BLTAFWM,a6)
		move.w	d1,(BLTCON1,a6)
		or.w	#$0FCA,d1
		move.w	d1,(BLTCON0,a6)
		move.w	d0,(BLTCMOD,a6)
		move.w	d0,(BLTDMOD,a6)

		*/ Calcul modulos sources A & B */
		move.l	(bob32_pagegfx-t,a5),a0
		move.w	(PG_SPLAN,a0),d0
		sub.w	(TBOB_WIDTH,a1),d0
		sub.w	(TBOB_WIDTH,a1),d0
		subq.w	#2,d0
		move.w	d0,(BLTAMOD,a6)
		move.w	d0,(BLTBMOD,a6)

		*/ Lance le Blitter */
		lea	(TBOB_TABLE,a1),a0
		move.l	(4,a0,d3),(BLTAPTH,a6)
		move.l	(0,a0,d3),(BLTBPTH,a6)
		move.l	d2,(BLTCPTH,a6)
		move.l	d2,(BLTDPTH,a6)
		move.w	(TBOB_WINDOW,a1),(BLTSIZE,a6)
		moveq	#0,d7
.exit		rts
.no_bob:	moveq	#-33,d7			¤ N° de bob inexistant
		bra.s	.exit




