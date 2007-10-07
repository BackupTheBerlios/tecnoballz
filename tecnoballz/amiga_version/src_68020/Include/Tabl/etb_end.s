***************************************************************************
*************************TEST SI GAMEOVER TABLEAU**************************
***************************************************************************
* Entrée	-> a5.l	adr. 't'
etb_end:	
		tst.w	(gameover-t,a5)
		beq.s	.etb_le_dieu
		moveq	#0,d0
		move.w	d0,(brick0-t,a5)
		move.w	d0,(brick1-t,a5)
		move.w	d0,(brick2-t,a5)
		cmp.w	#1,(gameover-t,a5)
		bne.s	.jaime
		bsr	clear_allbricks
		bsr	yadelabu
		bsr.l	np_end
.jaime		addq.w	#1,(gameover-t,a5)
		cmp.w	#10,(gameover-t,a5)
		blt.s	.etb_le_dieu
		tst.w	(tableau.end-t,a5)
		bne.s	.zoblok
		lea	(bri_pnt-t,a5),a2	table briks à eff.
		add	(brick.clr-t,a5),a2
		move.w	(a2),d2			adr. aff. brik
		bne.s	.etb_le_dieu
.zoblok		tst.w	(blitz.haut-t,a5)	gigablitz en cours ?
		bne.s	.etb_le_dieu		-> oui
		move.w	#9,(rout-t,a5)		>flag prépare gameover<
		bsr.l	pointeplayer
		bsr.l	xcopy			-> sauve le score ds liste joueur
		move.l	#irq,(a7)
.etb_le_dieu	rts

		*/ Supprime les balles */
yadelabu	move.l	(pt_balle-t,a5),a4
		moveq	#0,d0
		moveq	#NBALLE-1,d1
.super_etb	move.w	d0,(BALLE.ACTIF,a4)
		move.w	d0,(BALLE.GLUE,a4)
		lea	(BALLE.SIZE,a4),a4
		dbf	d1,.super_etb

		*/ Supprime les tirs */
		move.l	(pt_objet-t,a5),a4
		moveq	#NTIR*4-1,d1
.super_etb1	move.w	d0,(FIRE.ACTIF,a4)
		lea	(FIRE.SIZE,a4),a4
		dbf	d1,.super_etb1

		lea	(b1-t,a5),a4
		moveq	#3,d1
.super_etb2	move.w	d0,(BUMP.ACTIF,a4)
		move.l	(BUMP.BOB,a4),a1
		move.l	d0,(BOB.ACTIF,a1)
		lea	(BUMP.SIZE,a4),a4
		dbf	d1,.super_etb2

		move.l	(pt_capsule-t,a5),a4
		moveq	#NMONNAIE-1,d1
.super_etb3	move.w	d0,(a4)
		lea	(CAP.SIZE,a4),a4
		dbf	d1,.super_etb3

		move.l	(pt_gadget-t,a5),a4
		moveq	#NGADGET-1,d1
.super_etb4	move.w	d0,(a4)
		lea	(GAD.SIZE,a4),a4
		dbf	d1,.super_etb4
		rts


