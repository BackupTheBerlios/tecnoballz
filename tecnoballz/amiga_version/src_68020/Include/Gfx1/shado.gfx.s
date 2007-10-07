***************************************************************************
*--------------------------  Affiche une SHADOW --------------------------*
***************************************************************************
* Entrée	=> d0.w Numéro du BOB
*		=> d1.w	Position X
*		=> d2.w	Position Y
shado.gfx:
		*/ Lit le BOB dans la liste */
		bsr.l	bob32_get.bob
		bne	.no_bob
		move.l	a0,a1			pt/le BOB

		*/ Calcul adresse écran */
		move.l	(pt_screen-t,a5),a0
		mulu	(SP_SLINE,a0),d2
		move.w	(SP_SPLAN,a0),d0
		mulu	#NOPLAN,d0
		add.l	d0,d2
		moveq	#0,d0
		move.w	d1,d0
		lsr.w	#3,d0
		and.w	#$FFFE,d0
		add.l	d0,d2
		add.l	(ecran_travail-t,a5),d2

		*/ Calcul BLTCON0 */		
		and.w	#$F,d1
		ror.w	#4,d1
		or.w	#$0D0C,d1
		swap	d1
		clr.w	d1

		*/ Calcul modulos destinations B & D */
		move.w	(SP_SLINE,a0),d0
		sub.w	(TBOB_WIDTH,a1),d0
		sub.w	(TBOB_WIDTH,a1),d0
		subq.w	#2,d0
.waitblitter	btst	#14,(DMACONR,a6)
		bne.s	.waitblitter
		move.l	#$FFFF0000,(BLTAFWM,a6)
		move.l	d1,(BLTCON0,a6)
		move.w	d0,(BLTBMOD,a6)
		move.w	d0,(BLTDMOD,a6)

		*/ Calcul modulos source A */
		move.l	(bob32_pagegfx-t,a5),a0
		move.w	(PG_SLINE,a0),d0
		sub.w	(TBOB_WIDTH,a1),d0
		sub.w	(TBOB_WIDTH,a1),d0
		subq.w	#2,d0
		move.w	d0,(BLTAMOD,a6)

		*/ Lance le Blitter */
		lea	(TBOB_TABLE,a1),a0
		move.l	(4.w,a0),(BLTAPTH,a6)
		move.l	d2,(BLTBPTH,a6)
		move.l	d2,(BLTDPTH,a6)
		move.w	(TBOB_WINDOW2,a1),(BLTSIZE,a6)
		moveq	#0,d7
.exit		rts
.no_bob:	moveq	#-33,d7			¤ N° de bob inexistant
		bra.s	.exit
