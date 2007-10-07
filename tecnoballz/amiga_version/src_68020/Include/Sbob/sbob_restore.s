*»»»»»»»»»»»»»»»»»»				       «««««««««««««««««««*
*-----------------> RESTAURATION DU DECOR SOUS LES BOBS <-----------------*
*»»»»»»»»»»»»»»»»»»				       «««««««««««««««««««*
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l CUSTOM
sbob_restore:
		move.w	(flag_flip-t,a5),d0	flag "double buffering" 0/4
		move.l	(pt_screen-t,a5),a0
		move.l	(ecran_travail-t,a5),d4
		move.l	(SPECRAN3,a0),d5
		move.l	(ecran_travail-t,a5),d6
		move.l	(SPECRAN3,a0),d7
		lea	(DMACONR,a6),a1
		lea	(BLTAPTH,a6),a2
		lea	(BLTDPTH,a6),a3
		lea	(BLTSIZE,a6),a4
		moveq	#-1,d0
.wait_bimmer	btst	#14,(a1)
		bne.s	.wait_bimmer
		move.l	d0,(BLTAFWM,a6)
		move.l	#$09F00000,(BLTCON0,a6)

		* ECRAN 1 *
		tst.w	(flag_flip-t,a5)	affiche dans écran 2 ?
		bne.s	.ecran2			-> oui, on y va !
		lea	(sbob_listes16-t,a5),a0
		bsr.s	.restore
		lea	(sbob_listes32-t,a5),a0
		bsr.s	.restore
		lea	(sbob_listes48-t,a5),a0
		bsr.s	.restore
		lea	(sbob_listes64-t,a5),a0
		bsr.s	.restore
		lea	(sbob_listes80-t,a5),a0
		bsr.s	.restore
		lea	(sbob_listes256-t,a5),a0
		bra.s	.restore
		
		* ECRAN 2 *
.ecran2		lea	(sbob_listes16+4-t,a5),a0
		bsr.s	.restore
		lea	(sbob_listes32+4-t,a5),a0
		bsr.s	.restore
		lea	(sbob_listes48+4-t,a5),a0
		bsr.s	.restore
		lea	(sbob_listes64+4-t,a5),a0
		bsr.s	.restore
		lea	(sbob_listes80+4-t,a5),a0
		bsr.s	.restore
		lea	(sbob_listes256+4-t,a5),a0

*=*=*=*=*=*=*=*=*=*=* Réaffiche le décor sur les bobs *=*=*=*=*=*=*=*=*=*=*
* Entrée	=> d4.l adr. écran 1 ou écran 2 (1ere copie Blitter)
*		=> d5.l adr. écran 3 (1ere copie Blitter)
*		=> d6.l	adr. écran 1 ou écran 2 (2ieme copie Blitter)
*		=> d7.l	adr. écran 3 (2ieme copie Blitter)
*		=> a0.l	pointeur sur la liste de restauration SBOB
*		=> a1.l	DMACONR
*		=> a2.l	BLTAPTH
*		=> a3.l	BLTDPTH
*		=> a4.l	BLTSIZE
.restore	tst.l	(a0)			il existe une liste ?
		beq.s	.stop			-> non
		move.l	(a0),a0			a0=liste de restauration
.waitb		btst	#14,(a1)
		bne.s	.waitb
		move.l	(a0)+,(BLTAMOD,a6)	initialise modulo A & D
.wf_loop	move.l	(a0)+,d3		d3=nbre de SBOBs - 1
		bmi.s	.stop			-> pas de SBOBs à effacer
.loop		move.l	(a0)+,d0		d0=adr. relative du SBOB
		move.l	d0,d1
		add.l	d4,d0			d0=adresse écran 1 ou 2
		add.l	d5,d1			d1=adresse écran 3
		move.w	(a0)+,d2		d2=1ere fenêtre Blitter
.waitblitter	btst	#14,(a1)
		bne.s	.waitblitter
		move.l	d1,(a2)			init. source A (écran 3)
		move.l	d0,(a3)			init. source D (écran 1 ou 2)
		move.w	d2,(a4)			init. fenêtre blitter
		move.w	(a0)+,d2		d2=2ieme fenêtre Blitter
		beq.s	.next			-> une seule fenêtre Blitter
		move.l	(a0),d0			d0=2ieme adr. relative du SBOB
		move.l	d0,d1
		add.l	d6,d0			d1=adresse écran 1 ou 2
		add.l	d7,d1			d1=adresse écran 3
.waitzblitter	btst	#14,(a1)
		bne.s	.waitzblitter
		move.l	d1,(a2)			init. source A (écran 3)
		move.l	d0,(a3)			init. source D (écran 1 ou 2)
		move.w	d2,(a4)			init. fenêtre blitter
.next		addq.l	#4,a0
		dbf	d3,.loop
.stop		rts
