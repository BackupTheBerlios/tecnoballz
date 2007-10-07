*ררררררררררררררררררררררררררררררררררררררררררררררררררררררררררררררררררררררררר*
*-------------------------«Gagnי tableau terminי»-------------------------*
*ררררררררררררררררררררררררררררררררררררררררררררררררררררררררררררררררררררררררר*
tableau_end:
		tst.w	(tableau.end-t,a5)
		beq	.exit
		tst.w	(gameover-t,a5)
		bne	.exit

		*/ Supprime les balles */
		move.l	(pt_balle-t,a5),a4
		moveq	#0,d0
		moveq	#NBALLE-1,d1
.super_etb	move.w	d0,(a4)
		move.w	d0,(BALLE.GLUE,a4)
		lea	(BALLE.SIZE,a4),a4
		dbf	d1,.super_etb

		*/ Supprime les tirs */
		move.l	(pt_objet-t,a5),a4
		moveq	#NTIR*4-1,d1
.super_etb1	move.w	d0,(a4)
		lea	(FIRE.SIZE,a4),a4
		dbf	d1,.super_etb1
		move.w	#NLINE.BLITZ-1,(blitz.count-t,a5)
		move.l	(atable_clavier-t,a5),a0
		tst.b	(TOUCHE_ESPACE,a0)
		bne.s	.no_war
		bsr	move_bobtext

		*/ Attend que la muisque "win" soit terminי */
		move.l	(pt_poz-t,a5),a0	adr. table pos. du module
		move.w	(4,a0),d0		pos. musique WIN
		lea	(Variables+MT_SongPos-t,a5),a0
		cmp.b	(a0),d0			Musique "Win" terminיe ? 
		beq.s	.exit			-> non
.no_war		move.l	(pt_poz-t,a5),a0
		move.w	(8,a0),d0
		bsr.l	np_change		|-> position du magasin

	 	*/ test si il va a un gardien, si oui coupe la zik */
.no_dream3	moveq	#5,d0
		cmp.w	(level-t,a5),d0		level prochain = gardien inter. ?
		beq.s	.billy_apple		-> oui
		moveq	#11,d0
		cmp.w	(level-t,a5),d0		level prochain = gardien final ?
		bne.s	.billy_idol		-> non
.billy_apple	move.w	#1,(music.on-t,a5)	interdit saut א la playroutine
.billy_idol	bsr.l	go_fade_off
.test		bsr.l	wait_vbl
		bsr	gaga_fada		|-> Fade couleurs briques
		tst.w	(flag_fade_off-t,a5)
		bne.s	.test
		move.w	#8,(rout-t,a5)		>flag fin tableau<
.exit		rts
