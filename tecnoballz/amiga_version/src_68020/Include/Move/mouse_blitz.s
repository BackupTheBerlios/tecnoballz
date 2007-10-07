*=========================================================================*
*              Bouton gauche & droit => gère compteur Blitz		  *
*=========================================================================*
* entrée	=> a5.l	adr. table 't'
mouse_blitz:
		tst.w	(blitz.haut-t,a5)	GigaBlitz en cours ?
		bne.s	.creve_blitz		-> oui
		cmp.w	#BOUTON_BLITZ,(mouse0-t,a5)
		bne.s	.creve_blitz
		move.l	(pt_balle-t,a5),a3
		lea	(BALLE.GLUE,a3),a3
		moveq	#NBALLE-1,d0
.loop		tst.w	(a3)			balle collée ?
		bne.s	.creve_blitz		-> oui
		lea	(BALLE.SIZE,a3),a3
		dbf	d0,.loop
		tst.w	(blitz.count-t,a5)
		beq.s	.creve_1
		subq.w	#1,(blitz.tempo-t,a5)
		bne.s	.creve_1
		move.w	#TEMPO.BLITZ,(blitz.tempo-t,a5)
		subq.w	#1,(blitz.count-t,a5)
		bra.s	.creve_1
.creve_blitz	cmp.w	#NLINE.BLITZ-1,(blitz.count-t,a5)
		beq.s	.creve_1
		move.w	#TEMPO.BLITZ,(blitz.tempo-t,a5)
		addq.w	#1,(blitz.count-t,a5)
.creve_1	rts
