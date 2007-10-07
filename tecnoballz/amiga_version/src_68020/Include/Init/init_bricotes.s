***************************************************************************
*------------------------*INITIALISE LES BRICOTES*------------------------*
***************************************************************************
* Entrée	=> a5.l	adr. 't'
init_bricotes:
		bsr	init_cnp	
		bsr	init_sadr		|-> réserve buffer mémoire
		bne.s	.exit
		bsr	init_mpos		|-> calcul adr. écran bricotes
		bsr	j_le_rigolo		|-> reconstruit map-editor mur
		bsr	mamie_nova		|-> sauve fond 4 couleurs
		bsr	init_obricotes
		bsr	affiche.bricotes	|-> affiche les bricotes
		moveq	#0,d7
.exit		rts

*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*
*-------------------* Efface table reaffichage décor *--------------------*
*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*
* Entrée	=> a5.l	adr. 't'
init_cnp:
		clr.w	(brico.save-t,a5)
		clr.w	(brico.clr-t,a5)
		move.l	#COTE.SIZE*CNP,d0
		lea	(cote_pnt-t,a5),a0
		move.l	a0,(pt_cote-t,a5)
		bsr	clear.memory
		rts

*=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=*
*/-------------------* Initialise les tables "sadr" *--------------------/*
*=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=-.=*
* Entrée	=> a5.l	adr. 't'
init_sadr:
		*/ Réserve le Buffer de sauvegarde */
		lea	(tarte-t,a5),a4
		bsr	reserve.mem
		bne.s	.exit

		*/ Mur Haut */
		move.l	(tarte-t,a5),d0
		lea	(sadr_haut-t,a5),a0
		move.l	d0,(a0)+
		move.l	d0,(a0)+
		moveq	#12-1,d1
.loop1		move.l	d0,(a0)+
		add.l	#2*4*2,d0
		dbf	d1,.loop1

		*/ Mur Gauche */
		lea	(sadr_gauche-t,a5),a0
		move.l	d0,(a0)+
		move.l	d0,(a0)+
		moveq	#12-1,d1
.loop2		move.l	d0,(a0)+
		add.l	#2*16*2,d0
		dbf	d1,.loop2

		*/ Mur Droit */
		lea	(sadr_droit-t,a5),a0
		move.l	d0,(a0)+
		move.l	d0,(a0)+
		moveq	#12-1,d1
.loop3		move.l	d0,(a0)+
		add.l	#2*16*2,d0
		dbf	d1,.loop3
		moveq	#0,d7
.exit		rts

*-------------------------------------------------------------------------*
*--------------------* Initialise les tables "mpos" *---------------------*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	adr. 't'
init_mpos:
		*/ Mur de gauche */
		move.l	(pt_screen-t,a5),a0
		lea	(mpos_gauch2-t,a5),a1
		move.w	#32,d1
		mulu	(SP_SLINE,a0),d1
		moveq	#16,d2
		mulu	(SP_SLINE,a0),d2
		moveq	#12-1,d0
.loop1		move.w	d1,(a1)+
		add.w	d2,d1
		dbf	d0,.loop1	

		*/ Mur de droite */
		lea	(mpos_droi2-t,a5),a1
		move.w	#32,d1
		mulu	(SP_SLINE,a0),d1
		add.w	#30,d1
		moveq	#12-1,d0
.loop2		move.w	d1,(a1)+
		add.w	d2,d1
		dbf	d0,.loop2

		*/ Mur du haut */
		lea	(mpos_hau2-t,a5),a1
		move.w	#12,d1
		mulu	(SP_SLINE,a0),d1
		addq.w	#4,d1
		moveq	#12-1,d0
.loop3		move.w	d1,(a1)+
		addq.w	#2,d1
		dbf	d0,.loop3
		rts

*-------------------------------------------------------------------------*
*                      Test si reconstuit les 3 murs			  *
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	adr. 't'
j_le_rigolo:

		*/ Détermine murs cassable */
		clr.w	(mur.kas_lf-t,a5)
		clr.w	(mur.kas_rg-t,a5)
		clr.w	(mur.kas_up-t,a5)
		moveq	#1,d1
		move.w	(area-t,a5),d0		n° de l'area (varie de 1 à 5)
		cmp.w	#1,d0			area 1 ?
		beq.s	.torture723		-> 
		move.w	d1,(mur.kas_up-t,a5)	flag mur haut cassable
		cmp.w	#2,d0			area 2 ?
		beq.s	.poche_end		-> oui
		move.w	d1,(mur.kas_rg-t,a5)	flag mur droit cassable
		cmp.w	#3,d0			area 3 ?
		beq.s	.poche_end		-> oui
		move.w	d1,(mur.kas_lf-t,a5)	 flag mur gauche cassable
.poche_end	cmp.w	#5,d0			Area 5 ?
		bne.s	.torture723		-> non
		tst.w	(brico.rebuild-t,a5)	Reconstruit mur ?
		beq	.fromage		-> non
.torture723	clr.w	(brico.rebuild-t,a5)	RAZ Flag reconstruit mur

		*/ Reconstuit le mur de gauche */
		moveq	#1,d1
		tst.w	(rak.team-t,a5)		mode team ?
		beq.s	.go_akai		-> oui
		move.w	d1,(mur.kas_lf-t,a5)	Flag  mur gauche cassable
		moveq	#0,d1
		lea	(b4-t,a5),a4
		move.w	#2,(BUMP.ACTIF,a4)
.go_akai	lea	(bri_h1+2-t,a5),a0	map-editor mur de gauche
		moveq	#12-1,d0		12 briques de cotés mur gauche
.loop_skyrock	move.b	d1,(a0)+
		dbf	d0,.loop_skyrock

		*/ Reconstuit mur de droite */
		moveq	#1,d1
		lea	(bri_h2+2-t,a5),a0	map-editor mur de droite
		moveq	#12-1,d0		12 briques de cotés mur droite
.loop_skypock	move.b	d1,(a0)+
		dbf	d0,.loop_skypock

		*/ Reconstuit le mur de haut */
		lea	(bri_h3+2-t,a5),a0	map-editor mur du haut
		moveq	#12-1,d0		12 briques de cotés mur haut
.loop_skylock	move.b	d1,(a0)+
		dbf	d0,.loop_skylock

		*/ Recopie map-editor mur pour l'effacement */
.fromage	lea	(bri_h1-t,a5),a0
		lea	(bry_x1-t,a5),a1
		move.w	#bry_x1-bri_h1-1,d0
.babibel	move.b	(a0)+,(a1)+
		dbf	d0,.babibel
		rts

*-------------------------------------------------------------------------*
*------------------->Sauve le décor sous les bricotes<--------------------*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	adr. 't'
*		=> a6.l CUSTOM
mamie_nova:
		* INIT PAPY BLITTER *
		moveq	#-1,d0
		move.l	#$09F00000,d1
		move.l	(pt_screen-t,a5),a0
		move.l	(SPECRAN1,a0),d7
		move.w	(SP_SLINE,a0),d2	
		subq.w	#2,d2
		moveq	#0,d6
		move.w	(SP_SPLAN,a0),d6
		WAITBLITTER
		move.l	d0,(BLTAFWM,a6)
		move.l	d1,(BLTCON0,a6)
		move.w	d2,(BLTAMOD,a6)
		move.w	#0,(BLTDMOD,a6)
		* MUR GAUCHE *
		lea	(mpos_gauch2-t,a5),a1
		lea	(sadr_gauch2-t,a5),a0
		move.w	#16*64+1,d3
		moveq	#2*16,d5
		bsr.s	.glaire
		* MUR DROIT *
		lea	(mpos_droi2-t,a5),a1
		lea	(sadr_droi2-t,a5),a0
		bsr.s	.glaire
		* MUR HAUT *
		move.w	#4*64+1,d3
		moveq	#2*04,d5
		lea	(mpos_hau2-t,a5),a1
		lea	(sadr_hau2-t,a5),a0
		* Routine Blitterisé *
.glaire:	moveq	#12-1,d4
.xglaire	moveq	#0,d2
		move.w	(a1)+,d2		lit adrese écran relative
		add.l	d7,d2
		move.l	(a0)+,d0		lit adr. buffer
		WAITBLITTER
		move.l	d2,(BLTAPTH,a6)
		move.l	d0,(BLTDPTH,a6)
		move.w	d3,(BLTSIZE,a6)
		add.l	d6,d2
		add.l	d5,d0
		WAITBLITTER
		move.l	d2,(BLTAPTH,a6)
		move.l	d0,(BLTDPTH,a6)
		move.w	d3,(BLTSIZE,a6)
		dbf	d4,.xglaire
		rts

*-------------------------------------------------------------------------*
*------------------------* Affiche les bricotes *-------------------------*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	adr. 't'
affiche.bricotes:
		*/ mur de gauche */
		moveq	#BCG.Y,d4
		moveq	#BRICOT.NUM-1,d5
		lea	(2+bri_h1-t,a5),a2
.glaire		tst.b	(a2)+
		beq.s	.n1
		moveq	#BCG.X,d1
		move.w	d4,d2
		moveq	#BOB.VBRICOTE,d0
		moveq	#0,d3
		bsr.l	bob32.gfx
.n1		add.w	#16,d4
		dbf	d5,.glaire

		*/ mur de droite */
		moveq	#BCD.Y,d4
		moveq	#BRICOT.NUM-1,d5
		lea	(2+bri_h2-t,a5),a2
.glaire2	tst.b	(a2)+
		beq.s	.n2
		move.w	#BCD.X,d1
		move.w	d4,d2
		moveq	#BOB.VBRICOTE,d0
		moveq	#0,d3
		bsr.l	bob32.gfx
.n2		add.w	#16,d4
		dbf	d5,.glaire2

		*/ mur du haut */
		moveq	#BCH.X,d4
		moveq	#BRICOT.NUM-1,d5
		lea	(2+bri_h3-t,a5),a2
.glaire3	tst.b	(a2)+
		beq.s	.n3
		move.w	d4,d1
		moveq	#BCH.Y,d2
		moveq	#BOB.HBRICOTE,d0
		moveq	#0,d3
		bsr.l	bob32.gfx
.n3		add.w	#16,d4
		dbf	d5,.glaire3
		rts

*-------------------------------------------------------------------------*
*------------------* Affiche les ombres des bricotes *--------------------*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	adr. 't'
init_obricotes:
		*/ mur de gauche */
		moveq	#BCG.Y+ODECA,d4
		moveq	#BRICOT.NUM-1,d3
		lea	(2+bri_h1-t,a5),a2
.glaire		tst.b	(a2)+
		beq.s	.n1
		moveq	#BCG.X-ODECA,d1
		move.w	d4,d2
		moveq	#BOB.VBRICOTE,d0
		bsr.l	shado.gfx
.n1		add.w	#16,d4
		dbf	d3,.glaire

		*/ mur de droite */
		moveq	#BCD.Y+ODECA,d4
		moveq	#BRICOT.NUM-1,d3
		lea	(2+bri_h2-t,a5),a2
.glaire2	tst.b	(a2)+
		beq.s	.n2
		move.w	#BCD.X-ODECA,d1
		move.w	d4,d2
		moveq	#BOB.VBRICOTE,d0
		bsr.l	shado.gfx
.n2		add.w	#16,d4
		dbf	d3,.glaire2

		*/ mur du haut */
		moveq	#BCH.X-ODECA,d4
		moveq	#BRICOT.NUM-1,d3
		lea	(2+bri_h3-t,a5),a2
.glaire3	tst.b	(a2)+
		beq.s	.n3
		move.w	d4,d1
		moveq	#BCH.Y+ODECA,d2
		moveq	#BOB.HBRICOTE,d0
		bsr.l	shado.gfx
.n3		add.w	#16,d4
		dbf	d3,.glaire3
		rts
