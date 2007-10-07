*=========================================================================*
*--==--==--==--==-* Bouton gauche => déclenche les tirs *--==--==--==--==-*
*=========================================================================*
* Entrée	=> a5.l	adr. table 'tlk'
mouse_fire:
*-----------------------* Bouton tir : mode team *------------------------*
		tst.w	(rak.team-t,a5)		mode "team" ?
		beq.s	creve_11		-> non
		cmp.w	#BOUTON_TIR,(mouse0-t,a5)
		bne.s	.creve_111
		lea	(b1-t,a5),a1
		tst.w	(BUMP.FIRE,a1)
		beq.s	.angstlos1
		bsr	left_test
.angstlos1	lea	(b3-t,a5),a1
		tst.w	(BUMP.FIRE,a1)
		beq.s	.creve_111
		bsr	left_test
.creve_111	cmp.w	#BOUTON_TIR,(mouse1-t,a5)
		bne	creve_2
		lea	(b2-t,a5),a1
		tst.w	(BUMP.FIRE,a1)
		beq.s	.angstlos2
		bsr	left_test
.angstlos2	lea	(b4-t,a5),a1
		tst.w	(BUMP.FIRE,a1)
		beq	creve_2
		bra.s	left_test

*-----------------------* bouton tir : mode alone *-----------------------*
creve_11	cmp.w	#BOUTON_TIR,(mouse0-t,a5)
		bne	creve_2	
		lea	(b1-t,a5),a1
		tst.w	(BUMP.FIRE,a1)
		beq.s	.industry1
		bsr.s	left_test
.industry1	lea	(b3-t,a5),a1
		tst.w	(BUMP.FIRE,a1)
		beq.s	.industry2
		bsr.s	left_test
.industry2	lea	(b2-t,a5),a1
		tst.w	(BUMP.FIRE,a1)
		beq.s	.industry3
		bsr.s	left_test
.industry3	lea	(b4-t,a5),a1
		tst.w	(BUMP.FIRE,a1)
		beq	creve_2

*-------------- bouton tir : test si bobs tirs sont libres ---------------*
left_test:	move.l	(BUMP.OBJET,a1),a4
		cmp.w	#64,(rak.large-t,a5)	Tir circulaire ? 
		bne	left_test_a1		-> non
		moveq	#1,d0
		cmp.w	(FIRE.ACTIF,a4),d0
		bne.s	creve_2
		cmp.w	(FIRE.SIZE+FIRE.ACTIF,a4),d0
		bne.s	creve_2
		cmp.w	(2*FIRE.SIZE+FIRE.ACTIF,a4),d0
		bne.s	creve_2
		cmp.w	(3*FIRE.SIZE+FIRE.ACTIF,a4),d0
		bne.s	creve_2
		cmp.w	(4*FIRE.SIZE+FIRE.ACTIF,a4),d0
		bne.s	creve_2
		cmp.w	(5*FIRE.SIZE+FIRE.ACTIF,a4),d0
		bne.s	creve_2
		cmp.w	(6*FIRE.SIZE+FIRE.ACTIF,a4),d0
		bne.s	creve_2
		bra.s	heft1
left_test_a1	tst.w	(FIRE.ACTIF,a4)
		bne.s	creve_2
		tst.w	(FIRE.SIZE+FIRE.ACTIF,a4)
		bne.s	creve_2
		tst.w	(2*FIRE.SIZE+FIRE.ACTIF,a4)
		bne.s	creve_2
		tst.w	(3*FIRE.SIZE+FIRE.ACTIF,a4)
		bne.s	creve_2
		tst.w	(4*FIRE.SIZE+FIRE.ACTIF,a4)
		bne.s	creve_2
		tst.w	(5*FIRE.SIZE+FIRE.ACTIF,a4)
		bne.s	creve_2
		tst.w	(6*FIRE.SIZE+FIRE.ACTIF,a4)
		bne.s	creve_2
heft1		move.w	#3,(BUMP.FIRE,a1)
		move.b	#1,(sound+S_RAK.TIR-t,a5)
creve_2:	rts
