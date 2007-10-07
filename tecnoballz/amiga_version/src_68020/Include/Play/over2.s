*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*
*********************** ROUT=10 : GameOver Tableaux ***********************
*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*
over2:
		*/ Vire les lettres */
		move.l	(pt_bobtext-t,a5),a4
		move.w	(tb.lx-t,a5),d6
.loopxxxx	clr.w	(a4)
		lea	(BT.SIZE,a4),a4
		dbf	d6,.loopxxxx

		lea	(go.parasite-t,a5),a0
		addq.w	#1,(a0)
		and.w	#511,(a0)
		bne.s	.pacman
		move.w	#TETE_PARASITE1,(head.req-t,a5)
.pacman		bsr.l	wait_vbl
		bsr	flip_screens
		bsr	hasard_ok
		bsr	bob32_restore
		bsr	shado_restore
		bsr	score_affiche
		bsr	no_dream1		|-> affiche messages
		bsr.l	oo_aventure
		bsr	atom_dep		|-> Déplace les 4 atoms
		bsr	shado_affiche
		bsr	bob32_affiche
		bsr	print_bricks
		bsr	print_score
		bsr	print_lifes
		bsr	total_recal		|-> affiche tête
		bsr	pertedememoire
		bsr	quit
		bsr.l	pub1			|-> affiche inverseur
		bsr.l	pub3			|-> affiche le crédit

		* test si bouton souris dèjà préssé *
		tst.w	(gameover.out-t,a5)	déjà quitté ?
		bne.s	.rajkkk			-> oui
		bsr	mouse_buttons
		tst.w	(mouse0-t,a5)		bouton(s) souris préssé(s) ?
		beq	.oo_exit		-> non
		move.w	#1,(gameover.out-t,a5)	flag sortie
.rajkkk		bsr.l	go_fade_off
.test		tst.w	(flag_fade_off-t,a5)
		bne.s	.test
		bsr.l	np_end
		move.w	#8,(rout-t,a5)		>flag fin tableau<
.oo_exit	bra	irq
