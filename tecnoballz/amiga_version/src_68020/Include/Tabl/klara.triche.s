***************************************************************************
*-----------------------* GADGETSTRICHES (hehe!) *------------------------*
***************************************************************************
* Entrée	=> a5.l	Adr. 't'
klara.triche:
		cmp.l	#ETB,(triche-t,a5)
		bne.s	.klara_end
		move.w	(gadget.triche-t,a5),d0
		bmi.s	.klara_end
		move.w	#-1,(gadget.triche-t,a5)
		moveq	#0,d6
		lea	(b1-t,a5),a1
		move.l	(bump.triche-t,a5),d1
		beq.s	.amm
		move.l	d1,a1
		tst.w	(BUMP.ACTIF,a1)
		bne.s	.amm
		lea	(b1-t,a5),a1
.amm		jmp	([gad_jump1,pc,d0*4])
.klara_end	rts
