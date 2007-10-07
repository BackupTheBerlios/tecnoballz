*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
*>************************» MACRO DE TECNOBALLZ «************************<*
*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
*-------------------------------------------------------------------------*
*/ ATTEND LE PERE BLITTER */
WAITBLITTER	MACRO
.wait_blitter\@	btst	#14,(DMACONR,a6)
		bne.s	.wait_blitter\@
		ENDM
*-------------------------------------------------------------------------*
GETMESSAGE	MACRO	NUM_MESS
		move.b	#1,(message+\1-t,a5)
		ENDM


*-------------------------------------------------------------------------*
*/ ATTEND LE PERE BLITTER */
* Entrée	=> d7.w	#14 ("BBUSY" de "DMACONR")
*		=> a6.l	DMACONR
WAIT_BLITTER:	MACRO
.wait\@		btst	d7,(a6)
		bne.s	.wait\@
		ENDM
*-------------------------------------------------------------------------*
*/ CHANGE COULEUR DE FOND */
RASTER		MACRO	lcolor
		IFEQ	A_RASTER
		tst.b	(flag.raster-t,a5)
		beq.s	.nocolors\@
		movem.l	a0/a6,-(a7)
		move.l	(pt_screen-t,a5),a0
		move.w	(SBPLCON3,a0),(BPLCON3,a6)
		move.w	\1,(COLOR00,a6)
		movem.l	(a7)+,a0/a6
		ENDC
.nocolors\@
		ENDM

*-------------------------------------------------------------------------*
