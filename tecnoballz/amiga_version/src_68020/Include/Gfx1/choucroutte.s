;==========================================================================
;		MACRO : jeu, efface briques			      *GFX*
;==========================================================================
; entrée	=> d0.l		Adresse écran
;		=> d3.w		Motif de remplissage
;		=> d4.w		Fenêtre blitter
;		=> a6.l		CUSTOM
;		=> BLTCON0	$01F0
;		=> BLTCON1	$0000
;		=> BLTDMOD	TPLAN-2+4*TPLAN
;		=> BLTAFWM	$FFFF
;		=> BLTALWM	$FFFF
;
SORCIER:	MACRO
		WAITBLITTER
		move.w	d3,(BLTADAT,a6)
		move.l	d0,(BLTDPTH,a6)
		move.w	d4,(BLTSIZE,a6)
		ENDM

*=========================================================================*
*------------------> Réaffiche briques/décors briques <-------------------*
*=========================================================================*
* entrée	=> a5.l	adr. table 't'
*		=> a6.l	$dff000
choucroutte:
		lea	(bri_pnt-t,a5),a2	Table des briques à effacer
		lea	(brick.clr-t,a5),a0
		move.w	(a0),d0
		add.w	d0,a2
		moveq	#0,d2
		move.w	(BRI.ADR,a2),d2		Adresse relative écran
		beq	.exit			->  pas de brique a effacer
		add.w	#BRI.SIZE,d0
		and.w	#BRI.MSK,d0
		move.w	d0,(a0)
		move.w	(BRI.NUM,a2),d1		Numéro de la brique
		moveq	#0,d3
		move.w	d3,(BRI.ADR,a2)		RAZ adresse d'affichage

		*/* Initialise le Blitter */*
		move.l	(pt_screen-t,a5),a0
		move.l	(SPECRAN1,a0),d5
		move.l	(SPECRAN2,a0),d6
		move.l	(SPECRAN3,a0),d7
		moveq	#-1,d0
		WAITBLITTER
		move.w	d3,(BLTCON1,a6)
		move.l	d0,(BLTAFWM,a6)

		*/* test si réaf. brik ou décor */*
		tst.w	(BRI.FLAG,a2)		réaf. brik ?
		beq	.zoulou1		-> oui

*-------------------------------------------------------------------------*
*                 Brique disparait : réaffiche décor			  *
*-------------------------------------------------------------------------*
		*/* calcul adr. brik dans le map-editor */*
		lea	(set_table2-t,a5),a4
		move.w	d1,d0
		lsl.w	#2,d0
		add.w	d0,a4
		move.w	d3,(2,a4)		efface code brik

		*/* efface l'ombre portée par la brique */*
		move.w	d3,(BLTAMOD,a6)
		move.w	#$01F0,(BLTCON0,a6)
		move.w	#(TPLAN-2)+4*TPLAN,(BLTDMOD,a6)

		* efface ombre portée en bas verticalement sur 3 lignes *
		move.w	#3*64+1,d4
		tst.w	(RIZ+2,a4)		brique en bas ?
		beq.s	.bibi_1			-> non
		move.w	#1*64+1,d4
.bibi_1		move.w	#$FFF8,d3
		tst.w	(04+2,a4)		brique à droite ?
		bne.s	.titi1			-> oui
		moveq	#-1,d3
.titi1		move.l	d2,d0
		add.l	d7,d0
		add.l	#7*TLIGNE+OPLAN,d0
		SORCIER

		* ombre portée en bas à gauche sur 3 lignes *
		move.w	#3*64+1,d4
		tst.w	(RIZ-4+2,a4)		brique en bas à gauche ?
		beq.s	.bibi_2			-> non
		move.	#1*64+1,d4
.bibi_2		moveq	#$0003,d3
		tst.w	(-4+2,a4)		brique à gauche ?
		bne.s	.titi2			-> oui
		moveq	#-1,d3
.titi2		move.l	d2,d0
		add.l	d7,d0
		add.l	#7*TLIGNE+OPLAN-2,d0
		SORCIER

		* ombre portée à gauche sur 4 lignes *
		move.w	#4*64+1,d4
		tst.w	(-4+2,a4)		brique à gauche ?
		bne.s	.bibi_3			-> oui
		moveq	#-1,d3
		move.l	d2,d0
		add.l	d7,d0
		add.l	#3*TLIGNE+OPLAN-2,d0
		SORCIER

		*-=-* réaffiche décor sous la brique *-=-*
		* plan 4 : ombre venant de droite sur 4 lignes *
.bibi_3		moveq	#-1,d3
		move.l	d2,d0
		add.l	d7,d0
		add.l	#3*TLIGNE+OPLAN,d0
		tst.w	(4+2,a4)		brique à droite ?
		beq.s	.titi3			-> non
		move.w	#$FFF8,d3
.titi3		SORCIER
		
		* plan 4 : ombre venant du haut, haut/gauche sur 2 lignes *
		move.w	#2*64+1,d4
		moveq	#-1,d3
		move.l	d2,d0
		add.l	d7,d0
		add.l	#OPLAN,d0
		tst.w	(-RIZ+2,a4)		brique en haut ?
		bne.s	.titi71			-> oui
		tst.w	(-RIZ+4+2,a4)		brique en haut à droite ?
		beq.s	.titi80			-> non
		move.w	#$FFF8,d3
		bra.s	.titi80
.titi71		moveq	#$0003,d3
		tst.w	(-RIZ+4+2,a4)		brique en haut à droite ?
		beq.s	.titi80			-> non
		moveq	#0,d3
.titi80		SORCIER

		* plan 4 : ligne entre ombre *
		move.w	#1*64+1,d4
		moveq	#-1,d3
		move.l	d2,d0
		add.l	d7,d0
		add.l	#2*TLIGNE+OPLAN,d0
		SORCIER

		* plan 3 : mis à zéro *
		move.w	#7*64+1,d4
		moveq	#0,d3
		move.l	d2,d0
		add.l	d7,d0
		add.l	#TPLAN*2,d0
		SORCIER

		* plan 5 : mis à zéro *
		move.l	d2,d0
		add.l	d7,d0
		add.l	#TPLAN*4,d0
		SORCIER

		* plan 1 : réaff. décor *
		move.l	(gateau-t,a5),a0	adresse sauv. décor
		mulu	#CONORS,d1
		add.l	d1,a0
		move.l	d2,d0
		add.l	d7,d0
		WAITBLITTER
		move.w	#$09F0,(BLTCON0,a6)
		move.l	a0,(BLTAPTH,a6)
		move.l	d0,(BLTDPTH,a6)
		move.w	d4,(BLTSIZE,a6)

		* plan 2 : réaff. décor *
		lea	(CONORS/2,a0),a0
		move.l	d2,d0
		add.l	#TPLAN,d0
		add.l	d7,d0
		WAITBLITTER
		move.l	a0,(BLTAPTH,a6)
		move.l	d0,(BLTDPTH,a6)
		move.w	d4,(BLTSIZE,a6)

		* recopie modif. dans les écr. 1 et 2 *
		move.w	#10*5*64+2,d4
		subq.l	#2,d2
		move.l	d2,d0
		add.l	d7,d0
		move.l	d2,d1
		add.l	d5,d1
		WAITBLITTER
		move.w	#TPLAN-4,(BLTAMOD,a6)
		move.w	#TPLAN-4,(BLTDMOD,a6)
		move.l	d0,(BLTAPTH,a6)
		move.l	d1,(BLTDPTH,a6)
		move.w	d4,(BLTSIZE,a6)
		move.l	d2,d1
		add.l	d6,d1
		WAITBLITTER
		move.l	d0,(BLTAPTH,a6)
		move.l	d1,(BLTDPTH,a6)
		move.w	d4,(BLTSIZE,a6)
	
		* c'est une brique indest. dest. *
		tst.w	(BRI.X,a2)		brik indest. cassé ?
		bpl.s	.lou_ferrigno		-> non
		move.b	#1,(sound+S_CASSE.INDEST-t,a5)
		bsr	venus2
		add.w	#100,(score.life-t,a5)
		bra	.exit

		** incrémente le score **
.lou_ferrigno	bsr	venus1
		bsr	venus1
		add.w	#20,(score.life-t,a5)

		** test si ttes les briks sont détruites **
		bsr	soleil			brique - 1
		addq.w	#1,(brick.destroy-t,a5)
		move.w	(brick.count-t,a5),d2
		subq.w	#1,d2			nbre briks à détruire
		cmp.w	(brick.destroy-t,a5),d2	= nbre briks détruites ?
		bne.s	.yo_mylene		-> non
		bsr	voleur
		
		***** emet le son *****
.yo_mylene:	move.b	#1,(sound+S_TOUCHE.BRIK-t,a5)
		cmp.w	#512,(BRI.X,a2)		GIGABLITZ ?
		beq	.exit
		bsr	no_what			|-> test si envoie monnaie
		bsr	yes_what		|-> test si envoie gadgets
		bra	.exit

		**-=-** brik devient moins résistante : réaf. brik **-=-**
*-------------------------------------------------------------------------*
		* calcul adr. brik dans le map-editor *
.zoulou1:	lea	(set_table-t,a5),a4
		moveq	#0,d3
		move.w	d1,d3			d1=n°brique
		add.l	(bk.pagegfx-t,a5),d3
		add.l	#PG_DATA,d3
		move.w	#7*5*64+1,d0
		move.l	d5,d1
		add.l	d2,d1
		WAITBLITTER
		move.w	#$09F0,(BLTCON0,a6)
		move.w	#TPLANB-2,(BLTAMOD,a6)
		move.w	#TPLAN-2,(BLTDMOD,a6)
		move.l	d3,(BLTAPTH,a6)
		move.l	d1,(BLTDPTH,a6)
		move.w	d0,(BLTSIZE,a6)		écran 1
		move.l	d6,d1
		add.l	d2,d1
		WAITBLITTER
		move.l	d3,(BLTAPTH,a6)
		move.l	d1,(BLTDPTH,a6)
		move	d0,(BLTSIZE,a6)		écran 2
		move.l	d7,d1
		add.l	d2,d1
		WAITBLITTER
		move.l	d3,(BLTAPTH,a6)
		move.l	d1,(BLTDPTH,a6)
		move.w	d0,(BLTSIZE,a6)		écran 3

		*/* incrémente le score */*
		bsr	venus1
		add.w	#10,(score.life-t,a5)

		*** emet le son ***
		move.b	#1,(sound+S_TOUCHE.BRIK-t,a5)
		bsr	no_what			|-> test si envoie monnaie
.exit		rts
		