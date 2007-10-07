*=========================================================================*
*------------------ Les 2 boutons en même temps => Tilt ------------------*
*=========================================================================*
* entrée	=> a5.l	adr. table 't'
tilt:
		clr.b	(sound+S_TECNO-t,a5)
		cmp.w	#BOUTON_TILT,(mouse0-t,a5)
		bne.s	.skrov2
		move.w	(balle_tilt-t,a5),d0
		addq.w	#1,d0
		move.w	(hasard-t,a5),d1
		lsl.w	#2,d1
		moveq	#60,d2
		and.w	d2,d1
		move.l	(pt_balle-t,a5),a3
		moveq	#NBALLE-1,d7
.loop		tst.w	(BALLE.ACTIF,a3)	Balle active ?
		beq.s	.skrov			-> Non
		tst.l	(BALLE.EJECT1,a3)	Balle est dans un coin ?
		bne.s	.skrov			-> Oui
		tst.l	(BALLE.EJECT3,a3)	Balle est dans un coin ?
		bne.s	.skrov			-> Oui
		cmp.w	(BALLE.TILT,a3),d0
		bne.s	.skrov
		clr.w	(BALLE.TILT,a3)
		move.b	#1,(sound+S_TECNO-t,a5)
		lea	(2+BALLE.DIRECT,a3),a0
		cmp.w	(a0),d1			Même direction ?
		bne.s	.krov2			-> Non
		move.w	(a0),d1
		addq.w	#8,d1
.krov2		and.w	d2,d1
		move.w	d1,(a0)			Nouvelle direction
		addq.w	#4,d1
.skrov		lea	(BALLE.SIZE,a3),a3
		dbf	d7,.loop
.skrov2
		*/ Déplace l'écran */
		tst.b	(sound+S_TECNO-t,a5)	tilt effectué ?
		beq.s	.exit				-> non
		move.w	#10,(rastx-t,a5)
.exit		move.w	(rastx-t,a5),d0
		beq.s	.stop
		subq.w	#1,d0
		move.w	d0,(rastx-t,a5)
.stop		move.l	(pt_screen-t,a5),a0
		sub.w	(SCOORDY,a0),d0
		neg.w	d0
		move.w	(flag_flip-t,a5),d1
		move.l	(copper.diwstrt-t,a5,d1),a1
		move.b	d0,(a1)
		add.w	(SNLIGNE,a0),d0
		move.b	d0,(4,a1)
		rts

nono
		clr.b	(sound+S_TECNO-t,a5)
		cmp.w	#BOUTON_TILT,(mouse0-t,a5)
		bne	.exit
		move.w	(balle_tilt-t,a5),d0	temps avant tilt
		addq.w	#1,d0
		move.w	(hasard-t,a5),d1
		and.w	#$F,d1
		lsl.w	#2,d1
		moveq	#4,d2
		moveq	#12,d3
		moveq	#0,d6
		move.l	(pt_balle-t,a5),a3
		moveq	#NBALLE-1,d4
.bitarts	cmp.w	(BALLE.TILT,a3),d0
		bne.s	.krov
		move.w	d6,(BALLE.TILT,a3)
		tst.w	(BALLE.ACTIF,a3)	Balle active ?
		beq.s	.krov				-> Non
		move.b	#1,(sound+S_TECNO-t,a5)
		tst.l	(BALLE.EJECT1,a3)	Balle est dans un coin ?
		bne.s	.krov				-> Oui
		tst.l	(BALLE.EJECT3,a3)	Balle est dans un coin ?
		bne.s	.krov				-> Oui
		cmp.w	(2+BALLE.DIRECT,a3),d1	Même direction ?
		bne.s	.krov2				-> Non
		move.w	(2+BALLE.DIRECT,a3),d1
		cmp.w	d2,d1
		bne.s	.banan
		move.w	d3,d1
		bra.s	.krov2
.banan		move.w	d2,d1
.krov2		move.w	d1,(2+BALLE.DIRECT,a3)	Nouvelle direction
.krov		lea	(BALLE.SIZE,a3),a3
.exit		dbf	d4,.bitarts
		
