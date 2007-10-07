*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*
************************ ROUT=13 : GameOver Gardien ***********************
*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*
* Entrée	=>	a5.l	Adr. 't'
*		=> 	a6.l	CUSTOM
mega_over2:
		*/ Vire les lettres */
		move.l	(pt_bobtext-t,a5),a4
		move.w	(tb.lx-t,a5),d6
.loopxxxx	clr.w	(a4)
		lea	(SBT.SIZE,a4),a4
		dbf	d6,.loopxxxx

		bsr	hasard_ok
		tst.w	(sh.f-t,a5)		Défilement a fait un tour ?
		beq.s	.ok			-> non
		bsr.l	init_pts_lutins2	|-> Affiche les scores-lutins
.ok		bsr.l	wait_vbl
		RASTER	#$008
		bsr	last_flip		|-> Echange listes Copper
		bsr	goscroll		|-> Affiche défilement
		bsr	ssha_restore
		bsr	sbob_restore
		bsr.l	oo_aventure		|-> Déplace Lettres GameOver
		bsr	boubouhorror		£=> Déplace Boules
		bsr	affiche_les_bobs
		bsr	quit			|-> Retour Main-Menu ?
		bsr	pertedememoire		|-> Info-Mémoire ?
		bsr	megaover.exit		£=> Quitte MegaGameOver ?
		RASTER	#$000
		bra	irq

*/////////////////////////////////////////////////////////////////////////*
*------------------------*DEPLACEMENT DES BOULES*-------------------------*
*/////////////////////////////////////////////////////////////////////////*
* Spécial si le jeu est terminé (fin de TecnoballZ)
* Entrée	=> a5.l Adr. 't'
boubouhorror:
		tst.w	(tecnowinner-t,a5)	TecnoballZ terminé ?
		beq	.exit			-> non
		lea	(tabsin-t,a5),a1
		lea	(tabcos-t,a5),a2
		move.w	#360*2,d6
		moveq	#80,d3
		moveq	#80,d4

		*/ VARIE VITESSE BOULES */
		lea	(gow4-t,a5),a0
		move.w	(a0),d0
		move.w	(hasard-t,a5),d1
		and.w	#$6,d1
		add.w	d1,d0
		cmp.w	d6,d0
		blt.s	.dangers
		sub.w	d6,d0
.dangers	move.w	d0,(a0)
		moveq	#2,d5
		moveq	#2,d1
		muls	(a1,d0),d5
		muls	(a2,d0),d1
		asr.l	#7,d1
		asr.l	#7,d5
		addq.w	#2,d1
		addq.w	#2,d5
		add.w	d1,d5
		add.w	d5,d5
		bne.s	.s
		moveq	#2,d5
.s
		*/ VARIE LE POINTEUR */
		lea	(gow3-t,a5),a0
		move.w	(a0),d0
		move.w	(hasard-t,a5),d1
		and.w	#$E,d1
		add.w	d1,d0
		cmp.w	d6,d0
		blt.s	.danger
		sub.w	d6,d0
.danger		move.w	d0,(a0)
		moveq	#3,d7
		moveq	#3,d1
		muls	(a1,d0),d7
		muls	(a2,d0),d1
		asr.l	#7,d1
		asr.l	#7,d7
		addq.w	#3,d1
		addq.w	#3,d7
		add.w	d1,d7
		add.w	d7,d7

		*/ VARIE RAYON X */
		lea	(gow-t,a5),a0
		move.w	(a0),d0
		add.w	d7,d0
		cmp.w	d6,d0
		blt.s	.dangerous
		sub.w	d6,d0
.dangerous	move.w	d0,(a0)
		moveq	#30,d1
		moveq	#30,d2
		muls	(a1,d0),d1
		muls	(a2,d0),d2
		asr.l	#7,d1
		asr.l	#7,d2
		add.w	d1,d3
		add.w	d2,d3

		*/ VARIE RAYON Y */
		lea	(gow2-t,a5),a0
		move.w	(a0),d0
		add.w	d7,d0
		add.w	d7,d0
		cmp.w	d6,d0
		blt.s	.dangeroux
		sub.w	d6,d0
.dangeroux	move.w	d0,(a0)
		moveq	#15,d1
		moveq	#15,d2
		muls	(a2,d0),d1
		muls	(a2,d0),d2
		asr.l	#7,d1
		asr.l	#7,d2
		add.w	d1,d4
		add.w	d2,d4
		move.l	(pt_boule-t,a5),a4
		moveq	#NBR_BOULE-1,d7
.loop		move.w	(SBOB.POSX1,a4),d0	valeur de l'angle
		add.w	d5,d0
		cmp.w	d6,d0
		blt.s	.dangerous4
		sub.w	d6,d0
.dangerous4	move.w	d0,(SBOB.POSX1,a4)
		move.w	d3,d1
		move.w	d4,d2
		muls	(a1,d0),d1		sin(a)*rayon
		muls	(a2,d0),d2		cos(a)*rayon
		asr.l	#7,d1
		asr.l	#7,d2
		add.w	#160,d1
		add.w	#120,d2
		move.w	d1,(SBOB.POSX,a4)
		move.w	d2,(SBOB.POSY,a4)
		lea	(SBOB.SIZE,a4),a4
		dbf	d7,.loop
.exit		rts

*/////////////////////////////////////////////////////////////////////////*
*-------------------* Affiche les SBOBs du GamesOver *--------------------*
*/////////////////////////////////////////////////////////////////////////*
* Entrée	=>	a5.l	Adr. 't'
*		=> 	a6.l	CUSTOM
affiche_les_bobs:
		lea	(go.list-t,a5),a3
		moveq	#NLETTRE-1,d7
		moveq	#32,d0
.z_tsee0	move.l	(a3)+,a4
		add.w	d0,(SBOB.POSX,a4)
		dbf	d7,.z_tsee0
		bsr	ssha_affiche		|-> Affiche Shadows-SBOBs
		bsr	sbob_affiche		|-> Affiche Scroll-BOBs
		lea	(go.list-t,a5),a3
		moveq	#NLETTRE-1,d7
		moveq	#32,d0
.z_tsee2567	move.l	(a3)+,a4
		sub.w	d0,(SBOB.POSX,a4)
		dbf	d7,.z_tsee2567
		rts

*/////////////////////////////////////////////////////////////////////////*
*************************** ? QUITTE MEGAOVER ? ***************************
*/////////////////////////////////////////////////////////////////////////*
* Entrée	=>	a5.l	Adr. 't'
megaover.exit:
		tst.w	(gameover.out-t,a5)	Déjà quitté ?
		bne.s	.rajkkk			-> oui
		bsr	mouse_buttons		|-> Test Boutons souris
		tst.w	(mouse0-t,a5)		bouton(s) souris préssé(s) ?
		beq.s	.oo_exit		-> non
		move.w	#1,(gameover.out-t,a5)	flag sortie
.rajkkk		bsr.l	go_fade_off
.test		tst.w	(flag_fade_off-t,a5)
		bne.s	.test
		move.w	#11,(rout-t,a5)		>flag fin gardien<
.oo_exit	rts
