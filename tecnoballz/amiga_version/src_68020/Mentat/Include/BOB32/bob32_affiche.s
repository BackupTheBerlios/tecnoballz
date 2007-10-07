*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*
*«=»«=»«=»«=»«=»«=»«=»«=»«=» Affiche les BOBs «=»«=»«=»«=»«=»«=»«=»«=»«=»«*
*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*§*
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l	CUSTOM
bob32_affiche:
		move.l	a6,-(a7)
		moveq	#14,d3
.waitblit	btst	d3,(DMACONR,a6)
		bne.s	.waitblit
		move.l #$FFFF0000,(BLTAFWM,a6)
		move.l	(bob32_xbyte-t,a5),a2
		move.l	(bob32_bltcon-t,a5),a3
		move.l	(bob32_ymulu-t,a5),a4
		lea	(BLTCPTH,a6),a6
		move.l	(pt_screen-t,a5),a0
		move.l	(ecran_travail-t,a5),d7
		tst.w	(flag_flip-t,a5)	affiche dans écran 1 ?
		beq	b32.ok_ecran1		-> oui, on y va !

*=*=*=*=*=*=*=*=*=*=*=*= affiche les bobs écran 2 =*=*=*=*=*=*=*=*=*=*=*=*=
		move.l	(bob32_width16-t,a5),a0
		bsr.s	affiche_bob2
		move.l	(bob32_width32-t,a5),a0
		bsr.s	affiche_bob2
		move.l	(bob32_width48-t,a5),a0
		bsr.s	affiche_bob2
		move.l	(bob32_width64-t,a5),a0
		bsr.s	affiche_bob2
		move.l	(bob32_width80-t,a5),a0
		bsr.s	affiche_bob2
		move.l	(bob32_width96-t,a5),a0
		bsr.s	affiche_bob2
		move.l	(bob32_width192-t,a5),a0
		bsr.s	affiche_bob2
		move.l	(bob32_width256-t,a5),a0
		bsr.s	affiche_bob2
		bra	b32.exit

* BOUCLE D'AFFICHAGE ECRAN 2 *
* Entrée	=> d3.w	#14
*		=> d7.l	adr. écran 2
*		=> a0.l	pointeur sur l'adresse de la liste bobs
*		=> a2.l	'bob32_xbyte'
*		=> a3.l	'bob32_bltcon'
*		=> a4.l	'bob32_ymulu'
*		=> a6.l	BLTCPTH
affiche_bob2:
		move.l	a5,-(a7)
		move.l	a7,d6			sauvegarde pointeur pile
		cmp.l	#0,a0
		beq	.stop
.waitb		btst	d3,(DMACONR-BLTCPTH,a6)
		bne.s	.waitb
		move.l	(a0)+,(BLTCMOD-BLTCPTH,a6)
		move.l	(a0)+,(BLTAMOD-BLTCPTH,a6)
		addq.l	#4,a0			saute adr. liste pour écran 1
		move.l	(a0)+,a5		a5=adr. liste rest. décor écr2
		bra	.start
.main		move.w	(a0)+,d4		d4=Nbre de bobs
		move.w	d5,(a5)+		sauve fenêtre blitter
		move.l	a5,a7
		move.w	d4,(a5)+		sauve nbre de bobs
.loop		move.l	(a0)+,a1		a1=adr. structure bob
		tst.l	(a1)+			affiche le bob ?
		beq.s	.noaffiche		-> non
		move.w	(a1)+,d0		d0=pos. x du bob
		moveq	#0,d1
		move.w	(a2,d0*2),d1		d1=(x/8) && $fffe
		move.l	(a3,d0*4),d2		d2=valeur de BLTCON0
		move.w	(a1)+,d0		d0=pos. y du bob
		add.l	(a4,d0*4),d1		d1=d1+y*[taille_ligne_écran]
		move.l	d1,(a5)+		sauve adr. relative du bob
		add.l	d7,d1			+adr. écran = adr. réélle du bob
.waitblitter	btst	d3,(DMACONR-BLTCPTH,a6)
		bne.s	.waitblitter
		move.l	d2,(BLTCON0-BLTCPTH,a6)
		move.l	d1,(a6)+		init. source C (écran)
		move.l	(a1)+,(a6)+		init. source B (gfx)
		move.l	(a1),(a6)+		init. source A (mask)
		move.l	d1,(a6)+		init. destination D (écran)
		move.w	d5,(a6)			init. fenêtre blitter
		lea	(-16,a6),a6
		dbf	d4,.loop
		bra.s	.start
.noaffiche	subq.w	#1,(a7)
		dbf	d4,.loop
.start		move.w	(a0)+,d5		d5=fenêtre Blitter
		bne	.main
		move.w	d5,(a5)			0=fin de la liste restauration
.stop		move.l	d6,a7
		move.l	(a7)+,a5
		rts

*=*=*=*=*=*=*=*=*=*=*=*= affiche les bobs écran 1 =*=*=*=*=*=*=*=*=*=*=*=*=
b32.ok_ecran1	move.l	(bob32_width16-t,a5),a0
		bsr.s	affiche_bob1
		move.l	(bob32_width32-t,a5),a0
		bsr.s	affiche_bob1
		move.l	(bob32_width48-t,a5),a0
		bsr.s	affiche_bob1
		move.l	(bob32_width64-t,a5),a0
		bsr.s	affiche_bob1
		move.l	(bob32_width80-t,a5),a0
		bsr.s	affiche_bob1
		move.l	(bob32_width96-t,a5),a0
		bsr.s	affiche_bob1
		move.l	(bob32_width192-t,a5),a0
		bsr.s	affiche_bob1
		move.l	(bob32_width256-t,a5),a0
		bsr.s	affiche_bob1
b32.exit	move.l	(a7)+,a6
		rts

* BOUCLE D'AFFICHAGE ECRAN 1 *
* Entrée	=> d3.w	#14
*		=> d7.l	adr. écran 1
*		=> a0.l	pointeur sur l'adresse de la liste bobs
*		=> a2.l	'bob32_xbyte'
*		=> a3.l	'bob32_bltcon'
*		=> a4.l	'bob32_ymulu'
*		=> a6.l	BLTCPTH
affiche_bob1:
		move.l	a5,-(a7)
		move.l	a7,d6
		cmp.l	#0,a0
		beq.s	.stop
.waitb		btst	d3,(DMACONR-BLTCPTH,a6)
		bne.s	.waitb
		move.l	(a0)+,(BLTCMOD-BLTCPTH,a6)
		move.l	(a0)+,(BLTAMOD-BLTCPTH,a6)
		move.l	(a0)+,a5		a5=adr. liste rest. décor écr1
		addq.l	#4,a0			saute adr. pour écran 2
		bra	.start
.main		move.w	(a0)+,d4		d4=Nbre de bobs
		move.w	d5,(a5)+		sauve fenêtre blitter
		move.l	a5,a7
		move.w	d4,(a5)+		sauve nbre de bobs
.loop		move.l	(a0)+,a1		a1=adr. structure bob
		tst.l	(a1)+			affiche le bob ?
		beq.s	.noaffiche		-> non
		move.w	(a1)+,d0		d0=pos. x du bob
		moveq	#0,d1
		move.w	(a2,d0*2),d1		d1=(x/8) && $fffe
		move.l	(a3,d0*4),d2		d2=valeur de BLTCON0
		move.w	(a1)+,d0		d0=pos. y du bob
		add.l	(a4,d0*4),d1		d1=d1+y*[taille_ligne_écran]
		move.l	d1,(a5)+		sauve adr. relative du bob
		add.l	d7,d1			+adr. écran = adr. réélle du bob
.waitblitter	btst	d3,(DMACONR-BLTCPTH,a6)
		bne.s	.waitblitter
		move.l	d2,(BLTCON0-BLTCPTH,a6)
		move.l	d1,(a6)+		init. source C (écran)
		move.l	(a1)+,(a6)+		init. source B (gfx)
		move.l	(a1),(a6)+		init. source A (mask)
		move.l	d1,(a6)+		init. destination D (écran)
		move.w	d5,(a6)			init. fenêtre blitter
		lea	(-16,a6),a6
		dbf	d4,.loop
		bra.s	.start
.noaffiche	subq.w	#1,(a7)
		dbf	d4,.loop
.start		move.w	(a0)+,d5		d5=fenêtre Blitter
		bne.s	.main
		move.w	d5,(a5)
.stop		move.l	d6,a7
		move.l	(a7)+,a5
		rts
