*-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-*
*=-=|=-=|=-=* Routine de restauration du décor sous les BOBs *=|=-=|=-=|=-*
*-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-=X=-*
bob32_restore:
		move.l	(pt_screen-t,a5),a0
		moveq	#14,d3
		move.l	(ecran_travail-t,a5),d6
		move.l	(SPECRAN3,a0),d7
		lea	(DMACONR,a6),a1
		lea	(BLTAPTH,a6),a2
		lea	(BLTDPTH,a6),a3
		lea	(BLTSIZE,a6),a4
		moveq	#-1,d0
.wait_bimmer	btst	d3,(a1)
		bne.s	.wait_bimmer
		move.l	d0,(BLTAFWM,a6)
		move.l	#$09F00000,(BLTCON0,a6)

		* ECRAN 1 *
		tst.w	(flag_flip-t,a5)	affiche dans écran 2 ?
		bne.s	.ecran2			-> oui, on y va !
		lea	(bob32_listes16-t,a5),a0
		bsr.s	.restore
		lea	(bob32_listes32-t,a5),a0
		bsr.s	.restore
		lea	(bob32_listes48-t,a5),a0
		bsr.s	.restore
		lea	(bob32_listes64-t,a5),a0
		bsr.s	.restore
		lea	(bob32_listes80-t,a5),a0
		bsr.s	.restore
		lea	(bob32_listes96-t,a5),a0
		bra.s	.restore
		lea	(bob32_listes192-t,a5),a0
		bsr.s	.restore
		lea	(bob32_listes256-t,a5),a0
		bra.s	.restore
		
		* ECRAN 2 *
.ecran2		lea	(bob32_listes16+4-t,a5),a0
		bsr.s	.restore
		lea	(bob32_listes32+4-t,a5),a0
		bsr.s	.restore
		lea	(bob32_listes48+4-t,a5),a0
		bsr.s	.restore
		lea	(bob32_listes64+4-t,a5),a0
		bsr.s	.restore
		lea	(bob32_listes80+4-t,a5),a0
		bsr.s	.restore
		lea	(bob32_listes96+4-t,a5),a0
		bsr.s	.restore
		lea	(bob32_listes192+4-t,a5),a0
		bsr.s	.restore
		lea	(bob32_listes256+4-t,a5),a0

*=*=*=*=*=*=*=*=*=*=* Réaffiche le décor sur les bobs *=*=*=*=*=*=*=*=*=*=*
* Entrée	=> d3.w	#14
*		=> d6.l	adr. écran 1 ou écran 2
*		=> d7.l	adr. écran 3
*		=> a0.l	pointeur sur l'adresse de la liste bobs
*		=> a1.l	DMACONR
*		=> a2.l	BLTAPTH
*		=> a3.l	BLTDPTH
*		=> a4.l	BLTSIZE
.restore	tst.l	(a0)			il existe une liste ?
		beq.s	.stop			-> non
		move.l	(a0),a0
.waitb		btst	d3,(a1)
		bne.s	.waitb
		move.l	(a0)+,(BLTAMOD,a6)
		bra.s	.first
.wf_loop	move.w	(a0)+,d4		d4=nbre de bobs - 1
		bmi.s	.first
.loop		move.l	(a0)+,d0		d0=adr. relative du bob
		move.l	d0,d1
		add.l	d6,d0
		add.l	d7,d1
.waitblitter	btst	d3,(a1)
		bne.s	.waitblitter
		move.l	d1,(a2)			init. source A (écran 3)
		move.l	d0,(a3)			init. source D (écran 1 ou 2)
		move.w	d2,(a4)			init. fenêtre blitter
		dbf	d4,.loop
.first		move.w	(a0)+,d2		d2=fenêtre blitter
		bne.s	.wf_loop		-> oui
.stop		rts
