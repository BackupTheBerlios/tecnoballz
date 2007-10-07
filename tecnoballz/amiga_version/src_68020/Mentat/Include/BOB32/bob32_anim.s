*									  *
* "bob32_anim"		=> Animation avec d�sactivation du BOB
* "bob32_simple_anim"	=> Animation r�p�t�e
*									  *
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
*�������������* Animation simple avec desactivation du BOB ���������������*
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Entr�e	=> a3.l	Adr. structure bob
* Modifie	(d0/a0)
bob32_anim:
		lea	(BOB.TEMPO,a3),a0
		subq.w	#1,(a0)+		tempo=tempo-1
		bne.s	.noamim			-> tempo > 0
		move.w	(a0)+,(BOB.TEMPO,a3)
		move.w	(a0)+,d0		d0=pt/animation
		cmp.w	(a0)+,d0		pt < au pt maxi ?
		bls.s	.no_maxi		-> oui
		clr.l	(BOB.ACTIF,a3)
		bra.s	.noamim			-> tempo > 0
.no_maxi	addq.w	#8,(BOB.PTANIM,a3)	
		move.l	(BOB.TABLE,a3),a0	a0=table adr. gfx & msk
		add.w	d0,a0
		move.l	(a0)+,(BOB.GFX,a3)
		move.l	(a0),(BOB.MSK,a3)
.noamim		rts

*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
*<O><O><O><O><O><O><O><* Animation simple r�p�t�e *<O><O><O><O><O><O><O><O*
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
* Entr�e	=> a3.l	Adr. structure bob
* Modifie	(d0/a0)
bob32_simple_anim:
		lea	(BOB.TEMPO,a3),a0
		subq.w	#1,(a0)+		tempo=tempo-1
		bne.s	.noamim			-> tempo > 0
		move.w	(a0)+,(BOB.TEMPO,a3)
		move.w	(a0)+,d0		d0=pt/animation
		cmp.w	(a0)+,d0		pt < au pt maxi ?
		bls.s	.no_maxi		-> oui
		moveq	#0,d0
		move.w	d0,(BOB.PTANIM,a3)
.no_maxi	addq.w	#8,(BOB.PTANIM,a3)	
		move.l	(BOB.TABLE,a3),a0	a0=table adr. gfx & msk
		add.w	d0,a0
		move.l	(a0)+,(BOB.GFX,a3)
		move.l	(a0),(BOB.MSK,a3)
.noamim		rts
