*>>>>>>>>>>>>>>>>>>>>>>>			   <<<<<<<<<<<<<<<<<<<<<<<*
*»»»»»»»»»»»»»»» ROUTINE AFFICHAGE DES SHADOWS-SCROLLBOB «««««««««««««««««*
*>>>>>>>>>>>>>>>>>>>>>>>			   <<<<<<<<<<<<<<<<<<<<<<<*
* Affichage des SHADOWS-SBOBs sur une écran en défilement vertical.
* Les SBOBs peuvent être affichés en deux opérations Blitter
* Entrée	=> a5.l	Adr. 't'	
*		=> a6.l	CUSTOM
ssha_affiche:
		*/ Calcul le pointeur sur les "last-tables" */
		move.l	a6,-(a7)
		moveq	#0,d5
		move.w	(nu_video-t,a5),d5
		beq.s	.no_change
		sub.w	(s_nligne-t,a5),d5
		neg.w	d5
		lsl.w	#2,d5
.no_change	
		*/ Calcul adresse écran */
		move.l	(ecran_travail-t,a5),d6
		move.l	(ecran_travail-t,a5),d7
		add.l	(ecran_shadow-t,a5),d6
		add.l	(ecran_shadow-t,a5),d7
		move.l	(ssha_xtable-t,a5),a2
		move.l	(ssha_ytable-t,a5),a4
		add.l	d5,a4
.waitblit	btst	#14,(DMACONR,a6)
		bne.s	.waitblit
		move.l	#$FFFF0000,(BLTAFWM,a6)
		lea	(BLTBPTH,a6),a6
		tst.w	(flag_flip-t,a5)	affiche dans écran 1 ?
		beq	ssh.ok_ecran1		-> oui, on y va !

*=*=*=*=*=*=*=*=*=*=*=*= affiche les bobs écran 2 =*=*=*=*=*=*=*=*=*=*=*=*=
		move.l	(ssha_width16-t,a5),a0
		bsr.s	affiche_ssha
		move.l	(ssha_width32-t,a5),a0
		bsr.s	affiche_ssha
		move.l	(ssha_width48-t,a5),a0
		bsr.s	affiche_ssha
		move.l	(ssha_width64-t,a5),a0
		bsr.s	affiche_ssha
		move.l	(ssha_width80-t,a5),a0
		bsr.s	affiche_ssha
		move.l	(ssha_width256-t,a5),a0
		bsr.s	affiche_ssha
		bra	ssh.exit

* BOUCLE D'AFFICHAGE ECRAN 2 *
* Entrée	=> d5.l	SCROLL_ssha
*		=> d6.l adr. écran 2 pour la premère copie Blitter
*		=> d7.l	adr. écran 2 pour la deuxième copie Blitter
*		=> a0.l	pointeur sur l'adresse de la liste sshas
*		=> a2.l	'ssha_xtable'
*		=> a4.l	'ssha_ytable'
*		=> a6.l	BLTBPTH
affiche_ssha:
		move.l	a5,-(a7)
		lea	(.pile,pc),a3
		move.l	a7,(a3)
		cmp.l	#0,a0
		beq	.stop
.waitb		btst	#14,(DMACONR-BLTBPTH,a6)
		bne.s	.waitb
		move.l	(a0)+,(BLTCMOD-BLTBPTH,a6)
		move.l	(a0)+,(BLTAMOD-BLTBPTH,a6)
		addq.l	#4,a0			saute adr. pour écran 1
		move.l	(a0)+,a5		a5=liste rest décor écr1
		move.l	a5,a7
		clr.l	(a5)+			RAZ nbre de BOBS
		bra	.start
.main		move.l	(a0)+,a3		a3=adr. "last_table"
		add.l	d5,a3			+ affline / SP_SLINE && $FFFE
		move.l	(a0)+,d4		d4=nbre de bobs-1
		add.l	d4,(a7)			sauve nbre de bobs-1
		addq.l	#1,(a7)
.loop		move.l	(a0)+,a1		a1=adr. structure bob
		tst.l	(a1)+			affiche le bob ?
		beq.s	.noaffiche		-> non
		move.w	(a1)+,d0		d0=pos. x du bob
		move.l	(a2,d0*8),d1		d1=(x/8) && $fffe
		move.l	d1,d3
		move.l	(4,a2,d0*8),d2		d2=valeur de BLTCON0
		move.w	(a1)+,d0		d0=pos. y du bob
		add.l	(a4,d0*4),d1		d1=d1+y*[taille_ligne_écran]
		move.l	d1,(a5)+		sauve adr. relative du bob
		move.w	(a3,d0*4),(a5)+		sauve 1ère fenêtre Blitter
		add.l	d6,d1			+adr. écran = adr. réélle du bob
		addq.l	#4,a1			saute adr. gfx
.waitblitter	btst	#14,(DMACONR-BLTBPTH,a6)
		bne.s	.waitblitter
		move.l	d2,(BLTCON0-BLTBPTH,a6)
		move.l	d1,(a6)+		init. source B (écran)
		move.l	(a1),(a6)+		init. source A (mask)
		move.l	d1,(a6)+		init. destination D (écran)
		move.w	(a3,d0*4),(a6)		init. fenêtre Blitter
		lea	(-12,a6),a6
		move.w	(2,a3,d0*4),d1		d1=2ième fenêtre Blitter 
		move.w	d1,(a5)+		sauve 2ième fenêtre Blitter
		beq.s	.next
		move.l	d3,(a5)
		add.l	d7,d3			+adr. écran
.wait_blitter	btst	#14,(DMACONR-BLTBPTH,a6)
		bne.s	.wait_blitter
		move.l	d3,(a6)				(BLTBPTH-BLTBPTH,a6)	init. source C (écran)
		move.l	d3,(BLTDPTH-BLTBPTH,a6)	init. destination D (écran)
		move.w	d1,(BLTSIZE-BLTBPTH,a6)	init. fenêtre Blitter
.next		addq.l	#4,a5
		dbf	d4,.loop
		bra.s	.start
.noaffiche	subq.l	#1,(a7)
		dbf	d4,.loop
.start		tst.l	(a0)			"last_table"=0 ?
		bne	.main			-> non
		subq.l	#1,(a7)
.stop		move.l	(.pile,pc),a7
		move.l	(a7)+,a5
		rts
		CNOP	0,4
.pile		dc.l	0

*=*=*=*=*=*=*=*=*=*=*=*= affiche les bobs écran 1 =*=*=*=*=*=*=*=*=*=*=*=*=
ssh.ok_ecran1	move.l	(ssha_width16-t,a5),a0
		bsr.s	affiche_ssha1
		move.l	(ssha_width32-t,a5),a0
		bsr.s	affiche_ssha1
		move.l	(ssha_width48-t,a5),a0
		bsr.s	affiche_ssha1
		move.l	(ssha_width64-t,a5),a0
		bsr.s	affiche_ssha1
		move.l	(ssha_width80-t,a5),a0
		bsr.s	affiche_ssha1
		move.l	(ssha_width256-t,a5),a0
		bsr.s	affiche_ssha1
ssh.exit	move.l	(a7)+,a6
		rts

* BOUCLE D'AFFICHAGE ECRAN 1 *
* Entrée	=> d5.l	SCROLL_ssha
*		=> d6.l adr. écran 1 pour la premère copie Blitter
*		=> d7.l	adr. écran 1 pour la deuxième copie Blitter
*		=> a0.l	pointeur sur l'adresse de la liste sshas
*		=> a2.l	'ssha_xtable'
*		=> a4.l	'ssha_ytable'
*		=> a6.l	BLTBPTH
affiche_ssha1:
		move.l	a5,-(a7)
		lea	(.pile,pc),a3
		move.l	a7,(a3)
		cmp.l	#0,a0
		beq	.stop
.waitb		btst	#14,(DMACONR-BLTBPTH,a6)
		bne.s	.waitb
		move.l	(a0)+,(BLTCMOD-BLTBPTH,a6)
		move.l	(a0)+,(BLTAMOD-BLTBPTH,a6)
		move.l	(a0)+,a5		a5=liste rest décor écr1
		addq.l	#4,a0			saute adr. pour écran 2
		move.l	a5,a7
		clr.l	(a5)+			RAZ nbre de BOBS
		bra	.start
.main		move.l	(a0)+,a3		a3=adr. "last_table"
		add.l	d5,a3			+"SCROLL_ssha"
		move.l	(a0)+,d4		d4=nbre de bobs-1
		add.l	d4,(a7)			sauve nbre de bobs-1
		addq.l	#1,(a7)
.loop		move.l	(a0)+,a1		a1=adr. structure bob
		tst.l	(a1)+			affiche le bob ?
		beq.s	.noaffiche		-> non
		move.w	(a1)+,d0		d0=pos. x du bob
		move.l	(a2,d0*8),d1		d1=(x/8) && $fffe
		move.l	d1,d3
		move.l	(4,a2,d0*8),d2		d2=valeur de BLTCON0
		move.w	(a1)+,d0		d0=pos. y du bob
		add.l	(a4,d0*4),d1		d1=d1+y*[taille_ligne_écran]
		move.l	d1,(a5)+		sauve adr. relative du bob
		move.w	(a3,d0*4),(a5)+		sauve 1ère fenêtre Blitter
		add.l	d6,d1			+adr. écran = adr. réélle du bob
		addq.l	#4,a1			saute adr. gfx
.waitblitter	btst	#14,(DMACONR-BLTBPTH,a6)
		bne.s	.waitblitter
		move.l	d2,(BLTCON0-BLTBPTH,a6)
		move.l	d1,(a6)+		init. source B (écran)
		move.l	(a1),(a6)+		init. source A (mask)
		move.l	d1,(a6)+		init. destination D (écran)
		move.w	(a3,d0*4),(a6)		init. fenêtre Blitter
		lea	(-12,a6),a6
		move.w	(2,a3,d0*4),d1		d1=2ième fenêtre Blitter 
		move.w	d1,(a5)+		sauve 2ième fenêtre Blitter
		beq.s	.next
		move.l	d3,(a5)
		add.l	d7,d3			+adr. écran
.wait_blitter	btst	#14,(DMACONR-BLTBPTH,a6)
		bne.s	.wait_blitter
		move.l	d3,(a6)			(BLTBPTH-BLTBPTH,a6)	init. source C (écran)
		move.l	d3,(BLTDPTH-BLTBPTH,a6)	init. destination D (écran)
		move.w	d1,(BLTSIZE-BLTBPTH,a6)	init. fenêtre Blitter
.next		addq.l	#4,a5
		dbf	d4,.loop
		bra.s	.start
.noaffiche	subq.l	#1,(a7)
		dbf	d4,.loop
.start		tst.l	(a0)			"last_table"=0 ?
		bne	.main			-> non
		subq.l	#1,(a7)
.stop		move.l	(.pile,pc),a7
		move.l	(a7)+,a5
		rts
		CNOP	0,4
.pile		dc.l	0
