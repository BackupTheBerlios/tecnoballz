
COSMIC:		MACRO
		move.l	d3,d1
		add.l	d5,d1
		WAITBLITTER
		move.l	d2,(a3)		BLTAPTH
		move.l	d1,(a2)
		move.w	d4,(a0)
		move.l	d3,d1
		add.l	d6,d1
		WAITBLITTER
		move.l	d2,(a3)
		move.l	d1,(a2)
		move.w	d4,(a0)
		move.l	d3,d1
		add.l	d7,d1
		WAITBLITTER
		move.l	d2,(a3)
		move.l	d1,(a2)
		move.w	d4,(a0)
		ENDM

*=========================================================================*
*------------------- Raffiche décor sous les Bricotes --------------------*
*=========================================================================*
* entrée	a5.l	adr. table 't'
*		a6.l	$dff000
choupette:
		*/ test s'il y a une bricote à effacer */
		moveq	#0,d6
		lea	(cote_pnt-t,a5),a2	table bricotes à eff.
		add.w	(brico.clr-t,a5),a2
		move.w	(COTE.FLAG,a2),d0	flag. eff. bricote
		beq	.exit			->  pas de bricote à eff.
		move.w	d6,(COTE.FLAG,a2)
		add.w	#COTE.SIZE,(brico.clr-t,a5)
		cmp.w	#COTE.SIZE*CNP,(brico.clr-t,a5)
		blt.s	.etb_2333
		move.w	d6,(brico.clr-t,a5)
.etb_2333	move.w	(COTE.PT,a2),d5		pointeur sur map-editor (step 1)

		*/* Initialise effacement bricote en mur du haut */*
		lea	(.maxximum,pc),a3
		cmp.w	#1,d0			mur du haut ??
		bne.s	.choupette1		-> non
		move.w	#4*64+1,d4		fenetre blitter
		lea	(sadr_haut-t,a5),a0	table adr. des 12 buffers
		lea	(mpos_haut-t,a5),a1	table pos. des 12 briques
		lea	(bry_x3-t,a5),a2	map-editor mur haut
		add.w	d5,a2
		move.b	d0,(a2)			sauve flag mur haut
		moveq	#2*04,d0		taille plan buffer
		jmp	(a3)

		*/* Initialise effacement bricote en mur droit */*
.choupette1	cmp.w	#2,d0			mur de gauche ??
		bne.s	.choupette2		-> non
		move.w	#16*64+1,d4		fenetre blitter
		lea	(sadr_droit-t,a5),a0	table adr. des 12 buffers
		lea	(mpos_droit-t,a5),a1	table pos. des 12 briques
		lea	(bry_x2-t,a5),a2	map-editor mur droit
		add.w	d5,a2
		move.b	d0,(a2)			sauve flag mur droit
		moveq	#2*16,d0		taille plan buffer
		jmp	(a3)

		*/* Initialise effacement bricote en mur gauche */
.choupette2	move.w	#16*64+1,d4		fenetre blitter
		lea	(sadr_gauche-t,a5),a0	table adr. des 12 buffers
		lea	(mpos_gauche-t,a5),a1	table pos. des 12 briques
		lea	(bry_x1-t,a5),a2	map-editor mur gauche
		add.w	d5,a2
		move.b	d0,(a2)			sauve flag mur gauche
		moveq	#2*16,d0		taille plan buffer

		*/ Initialise le Blitter en copie */
.maxximum:	move.w	#TLIGNE-2,d7
		moveq	#-1,d1
		move.l	#$09F00000,d3
		WAITBLITTER
		move.l	d1,(BLTAFWM,a6)
		move.l	d3,(BLTCON0,a6)
		move.w	d6,(BLTAMOD,a6)
		move.w	d7,(BLTDMOD,a6)

		*/ Calcul adresse écran d'affichage */
		moveq	#0,d3
		add.w	d5,d5
		move.w	(a1,d5),d3		d3=adresse écran relative

		*/ Calcul adresse décor à réafficher */
		add.w	d5,d5
		move.l	(a0,d5),d2

		*/ Initialise registre 680x0 */
		moveq	#14,d1
		move.l	(ecran1-t,a5),d5
		move.l	(ecran2-t,a5),d6
		move.l	(ecran3-t,a5),d7
		move.l	a5,-(a7)
		move.l	a2,a5			> modifie a5 < : adr. map-editor
		lea	(BLTSIZE,a6),a0
		lea	(BLTDPTH,a6),a2
		lea	(BLTAPTH,a6),a3
		lea	(DMACONR,a6),a4
		
		*/ Réaffiche plan 1 (écran 1, 2, et 3) */
		COSMIC

		*/ Réaffiche plan 2 (écran 1, 2, et 3) */
		add.l	d0,d2
		add.l	#TPLAN,d3
		COSMIC

		*/ Init. Blitter effacement */
		WAITBLITTER
		move.w	#$01F0,(BLTCON0,a6)
		move.w	#$0000,(BLTADAT,a6)
		*/ eff. bricote : efface plan 3 (écran 1, 2, et 3) */
		add.l	#TPLAN,d3
		COSMIC

		*/ eff. bricote : test le mur concerné */
		add.l	#TPLAN,d3
		cmp.b	#3,(a5)			mur gauche ?
		beq	.ebc_left		-> oui
		cmp.b	#2,(a5)			mur droit ?
		beq	.ebc_right 		-> oui


		*/ eff. bricote : efface plan 4 (écran 1, 2, et 3) mur haut*
		*/ - mur haut : 3 premières lignes de la bricote -
		move.l	d3,-(a7)
		move	#3*64+1,d4
		WAITBLITTER
		move	#$ffff,(BLTADAT,a6)
		COSMIC

		* mur haut : dernière ligne bricote, + ombre portée par bricote *
		move.w	#4*64+1,d4
		add.l	#3*TLIGNE,d3
		WAITBLITTER
		tst.b	(1,a5)			brique à droite ?
		beq.s	.no_bcy			-> non
		move.w	#$FFF8,(BLTADAT,a6)
.no_bcy		COSMIC

		* mur haut : 3 lignes, ombre portée par la bricote *
		tst.b	(-2,a5)			1er brique de la ligne ?
		bmi.s	.juramiga		-> oui, n'affiche pas car ejecteur
		subq.l	#2,d3
		WAITBLITTER
		move.w	#$FFFF,(BLTADAT,a6)
		tst.b	(-1,a5)			brique à gauche ?
		beq.s	.no_bcy2		-> non
		add.l	#1*TLIGNE,d3
		move.w	#3*64+1,d4
		move.w	#$0007,(BLTADAT,a6)
.no_bcy2	COSMIC
.juramiga	move.w	#4*64+1,d4
		move.l	(a7)+,d3
		bra	.bc_plan5

		* eff. bricote : efface plan 4 (écran 1, 2, et 3) mur droit *
		* mur droit : 3 1eres lignes, ombre portée par bricote précédente - *
.ebc_right	move.w	#3*64+1,d4
		subq.l	#2,d3
		WAITBLITTER
		move.w	#$FFFF,(BLTADAT,a6)
		tst.b	(-1,a5)			brique en haut ?
		beq.s	.no_bc0			-> non
		move.w	#$FFF8,(BLTADAT,a6)
.no_bc0		tst.b	(-2,a5)			1er brique de la rangée ?
		bpl.s	.pusseno2		-> non
		move.w	#$0007,(BLTADAT,a6)	ombre éjecteur haut-droit
.pusseno2	COSMIC

		* mur droit : 3 1eres lignes, ombre portée par bricote précédente *
		addq.l	#2,d3
		WAITBLITTER
		move.w	#$FFC0,(BLTADAT,a6)
		tst.b	(-1,a5)			brique en haut ?
		beq.s	.no_bcy3			-> non
		move.w	#$7FC0,(BLTADAT,a6)
.no_bcy3	COSMIC

		* mur droit : 13 lignes suivantes, ombre portée par la bricote  *
.wno_bcer	add.l	#3*TLIGNE,d3
		move.w	#13*64+1,d4
		subq.l	#2,d3
		WAITBLITTER
		move.w	#$FFFF,(BLTADAT,a6)
		COSMIC

		* mur droit : 13 lignes suivantes, ombre portée par la bricote  *
		addq.l	#2,d3
		WAITBLITTER
		move.w	#$FFC0,(BLTADAT,a6)
		COSMIC

		* mur droit : 3 lignes suivantes, ombre portée par la bricote  *
		add.l	#13*TLIGNE,d3
		move.w	#3*64+1,d4
		subq.l	#2,d3
		WAITBLITTER
		move.w	#$FFFF,(BLTADAT,a6)
		tst.b	(2,a5)			test spécial
		bmi	.wno_bcz2		-> si négatif, dernière bricote (n'affiche pas a cause de l'èjecteur) 
		tst.b	(1,a5)			brique en bas ?
		beq.s	.no_bcx			-> non
		move.w	#$FFFF,(BLTADAT,a6)
.no_bcx		COSMIC

		* mur droit : 3 lignes suivantes, ombre portée par la bricote  *
.wno_bcz2	addq.l	#2,d3
		tst.b	(2,a5)			test spécial
		bmi	.wno_bcz		-> si négatif, dernière bricote (n'affiche pas a cause de l'èjecteur) 
		WAITBLITTER
		tst.b	(1,a5)			brique en bas ?
		bne.s	.wno_bcz		-> oui
		move.w	#$FFC0,(BLTADAT,a6)
		COSMIC
.wno_bcz	sub.l	#16*TLIGNE,d3
		move.w	#16*64+1,d4
		bra	.bc_plan5

		*eff. bricote : efface plan 4 (écran 1, 2, et 3) mur gauche
		*- mur gauche : 3 1ères lignes, ombre portée par bricote précédente -
.ebc_left	move.w	#3*64+1,d4
		WAITBLITTER
		move.w	#$FFFF,(BLTADAT,a6)
		tst.b	(-1,a5)			brique en haut ?
		beq.s	.no_bc			-> non
		move.w	#$FF87,(BLTADAT,a6)
.no_bc		tst.b	(-2,a5)			1er brique de la rangée ?
		bpl.s	.pusseno
		move.w	#$FFF8,(BLTADAT,a6)
.pusseno	COSMIC

		* mur gauche : 13 lignes suivantes, ombre portée par la bricote  -
		add.l	#3*TLIGNE,d3
		move.w	#13*64+1,d4
		WAITBLITTER
		move.w	#$FFFF,(BLTADAT,a6)
		COSMIC

		* mur gauche : 3 lignes suivantes, ombre portée par la bricote  *
		add.l	#13*TLIGNE,d3
		move.w	#3*64+1,d4
		WAITBLITTER
		tst.b	(1,a5)			brique en bas ?
		beq.s	.no_bc2			-> non
		move.w	#$FFF0,(BLTADAT,a6)
.no_bc2		COSMIC
		sub.l	#16*TLIGNE,d3
		move.w	#16*64+1,d4

		* eff. bricote : efface plan 5 (écran 1, 2, et 3) *
.bc_plan5	add.l	#TPLAN,d3
		WAITBLITTER
		move.w	#$0000,(BLTADAT,a6)
		COSMIC

		*/ raz flag map-editor effacement */
		clr.b	(a5)
		move.l	(a7)+,a5		> restitue a5 <
.exit		rts
