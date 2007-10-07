*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*X<<<<<<<<<<<<<<<<<<|< AFFICHE LE FOND DE 4 COULEURS >|>>>>>>>>>>>>>>>>>>X*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l CUSTOM
init_fond:
		bsr	init.set_guide
		bsr	choice.color
		bsr	affiche_fond
		bne.s	.exit
		bsr	save_fond_bricks
		bne.s	.exit
		moveq	#0,d7
.exit		rts

*-------------------------------------------------------------------------*
*--------------------*/ Calcul la table 'set_guide' /*--------------------*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	adr. 't'.
init.set_guide:
		move.l	(pt_screen-t,a5),a0
		lea	(set_guide-t,a5),a1
		moveq	#BK.POSY,d2
		mulu	(SP_SLINE,a0),d2
		moveq	#BK.HEIGHT+1,d3
		mulu	(SP_SLINE,a0),d3
		moveq	#BK.Y-1,d0
.loop_y		move.l	d2,d4
		;addq.l	#BK.POSX/8,d4
		moveq	#BK.X-1,d1
.loop_x		move.l	d4,(a1)+
		addq.l	#2,d4
		dbf	d1,.loop_x
		add.l	d3,d2
		dbf	d0,.loop_y
		rts

*-------------------------------------------------------------------------*
*--------------* Choisis une des 28 palettes pour le fond *---------------*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	adr. 't'.
choice.color:
		*/ Choisis les couleurs */
		move.l	(map60.color-t,a5),a0
		move.w	(avo-t,a5),d0		n° couluer de fond à utiliser
		tst.w	(rout-t,a5)		Editeur ?
		bne.s	.xxok			-> oui
		move.w	(hasard-t,a5),d0
		add.w	(VHPOSR,a6),d0
		add.w	(VPOSR,a6),d0
		and.w	#31,d0
		cmp.w	#28,d0			d0 entre 0 & 27
		blt.s	.xxok
		and.w	#15,d0
.xxok		lsl.w	#4,d0
		add.w	d0,a0
		move.l	(pt_screen-t,a5),a1
		lea	(SCOLOR_TABLE,a1),a1
		lea	(8*4,a1),a2
		moveq	#3,d3
.zodkdiso	move.l	(a0),d2
		bsr	aga_color
		move.w	d0,(a2)+
		move.w	d1,(a2)+
		move.l	(a0)+,d2
		lsr.l	#1,d2
		and.l	#$007F7F7F,d2
		bsr	aga_color
		move.w	d0,(a1)+
		move.w	d1,(a1)+
		dbf	d3,.zodkdiso
		rts

*-------------------------------------------------------------------------*
*-----------------------* Affiche un des 60 fonds *-----------------------*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	adr. 't'.
*		=> CUSTOM
affiche_fond:
		*/ Décompacte la page graphique des 60 fonds */
		sub.l	a2,a2
		lea	(map60.pp-t,a5),a3
		lea	(map60.pagegfx-t,a5),a4
		bsr	decode.pagepp
		bne	.exit

		*/ Init Registres */
		lea	(map60.pagegfx-t,a5),a4
		move.l	(RESA.ADR,a4),a4
		move.l	(pt_screen-t,a5),a0
		move.w	(PG_SLINE,a4),d0
		subq.w	#MAP60.LARG,d0
		swap	d0
		move.w	(SP_SLINE,a0),d0
		subq.w	#MAP60.LARG,d0
		move.w	(SP_SLINE,a0),d3
		mulu	#MAP60.HAUT,d3		d3=Offset prochaine ligne
		move.w	#MAP60.HAUT*64+MAP60.LARG/2,d2
		move.l	(SPECRAN1,a0),a2
		moveq	#0,d5
		moveq	#0,d6
		move.w	(PG_SPLAN,a4),d5
		move.w	(SP_SPLAN,a0),d6

		*/ Init. le Blitter */
.wait		btst	#14,(DMACONR,a6)
		bne.s	.wait
		move.l	d0,(BLTAMOD,a6)		
		move.l	#$09F00000,(BLTCON0,a6)
		move.l	#-1,(BLTAFWM,a6)

		*/ Choisis un des 60 fonds */
		move.w	(ava-t,a5),d0		n° du fond à utiliser
		tst.w	(rout-t,a5)		Editeur ?
		bne.s	.ok			-> oui
		move.w	(hasard-t,a5),d0
		and.w	#63,d0
		cmp.w	#60,d0			d0 entre 0 & 59
		blt.s	.ok
		and.w	#31,d0
.ok		move.w	d0,d1
		lsr.w	#1,d1
		mulu	#MAP60.HAUT,d1
		mulu	(PG_SLINE,a4),d1
		btst	#0,d0
		beq.s	.nono
		add.l	#MAP60.LARG*5,d1
.nono		lea	(PG_DATA,a4),a4
		add.l	d1,a4

		*/ Boucle d'affichage */
		moveq	#MAP60.X-1,d0
.loopy		move.l	a2,a3
		moveq	#MAP60.X-1,d1
.loopx		move.l	a4,d4
		moveq	#0,d7
		move.w	(hasard-t,a5),d7
		add.w	d0,d7
		add.w	d1,d7
		add.w	(VHPOSR,a6),d7
		add.w	(VPOSR,a6),d7
		and.w	#$F,d7
		move.w	(.tab,d7*2,pc),d7
		add.l	d7,d4
.wait2		btst	#14,(DMACONR,a6)
		bne.s	.wait2
		move.l	d4,(BLTAPTH,a6)
		move.l	a3,(BLTDPTH,a6)
		move.w	d2,(BLTSIZE,a6)
		add.l	d5,d4
		add.l	d6,a3
.wait3		btst	#14,(DMACONR,a6)
		bne.s	.wait3
		move.l	d4,(BLTAPTH,a6)
		move.l	a3,(BLTDPTH,a6)
		move.w	d2,(BLTSIZE,a6)
		sub.l	d6,a3
		addq.l	#MAP60.LARG,a3
		dbf	d1,.loopx
		add.l	d3,a2
		dbf	d0,.loopy

		*/ Libère la page graphique */
		lea	(map60.pagegfx-t,a5),a0
		bsr	libere_mem
.exit		rts
.tab		dc.w	12,00,00,12,16,08,04,16,12,08
		dc.w	04,04,00,00,08,16

*-------------------------------------------------------------------------*
*------------------* Sauve le fond sous les briques *---------------------*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	adr. 't'.
*		=> CUSTOM
save_fond_bricks:
		*/ Réserve mémoire pour fond des briques */
		lea	(gateau-t,a5),a4
		bsr	reserve.mem
		bne	.exit

		*/ Sauve 2 premiers plans du fond des briques */
		move.l	(pt_screen-t,a5),a0
		lea	(gateau-t,a5),a4
		lea	(set_guide-t,a5),a1
		moveq	#0,d0
		move.w	(SP_SLINE,a0),d0
		subq.w	#2,d0
		swap	d0			d0=modulos A et D
		moveq	#-1,d1
		move.w	#NID-1,d2		d2=nombre de briques
		move.l	(RESA.ADR,a4),d3	d3=adresse sauvegarde
		moveq	#CONORS/2,d4
		move.w	#7*64+1,d5
		moveq	#0,d6
		move.w	(SP_SPLAN,a0),d6	d6=taille d'un plan écran
		move.l	(SPECRAN1,a0),d7
		WAITBLITTER
		move.l	d0,(BLTAMOD,a6)
		move.l	d1,(BLTAFWM,a6)
		move.l	#$09F00000,(BLTCON0,a6)
.set_loop:	move.l	(a1)+,d1		d1=adresse relative écran
		add.l	d7,d1			+ adresse écran 1
		WAITBLITTER
		move.l	d1,(BLTAPTH,a6)
		move.l	d3,(BLTDPTH,a6)
		move.w	d5,(BLTSIZE,a6)		sauve le plan 1
		add.l	d6,d1
		add.l	d4,d3
		WAITBLITTER
		move.l	d1,(BLTAPTH,a6)
		move.l	d3,(BLTDPTH,a6)
		move.w	d5,(BLTSIZE,a6)		sauve le plan 2
		add.l	d4,d3
		dbf	d2,.set_loop
		moveq	#0,d7
.exit		rts
