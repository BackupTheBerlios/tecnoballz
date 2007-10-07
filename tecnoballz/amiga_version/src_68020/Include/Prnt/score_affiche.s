*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
*************Affiche les Scores dans le GameOver des tableaux**************
*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
* Entrée	=> a5.l	adr. 't'
score_affiche:

		tst.w	(gobot.flag-t,a5)	encore lettres à afficher ?
		beq	.go_seal		-> non, sort

		*/ calcul les >3< adresses écrans  */
		move.l	(gobot.adr-t,a5),d0	1er position ecran
		add.w	(gobot.offset-t,a5),d0	+pointeur
		move.l	(ecran1-t,a5),a1
		add.l	d0,a1
		move.l	(ecran2-t,a5),a2
		add.l	d0,a2
		move.l	(ecran3-t,a5),a3
		add.l	d0,a3

		*/ lit caractère à afficher */
		move.l	(pt_score-t,a5),a0
		move.w	(hard-t,a5),d0
		mulu	#OFF.SCORE,d0
		lea	(IS+4,a0,d0),a0
		add.w	(gobot.pt-t,a5),a0	+pointeur sur caractère
		moveq	#0,d0
		move.b	(a0)+,d0

		*/ aff scores : lit nouvelle position */
		move.l	(pt_screen-t,a5),a4
		cmp.w	#$FF,d0			fin de la ligne ?
		bne.s	.go_steal		-> non
		move.w	(a0)+,d0		Y
		mulu	(SP_SLINE,a4),d0
		add.w	(a0),d0
		move.l	d0,(gobot.adr-t,a5)	Sauve nouvelle position
		clr.w	(gobot.offset-t,a5)	RAZ pt pos. écran
		addq.w	#5,(gobot.pt-t,a5)	+5 pt text
		bra	score_affiche

		*/  fin plus de text à afficher /*
.go_steal	cmp.w	#$FE,d0			fin des textes ?
		bne.s	.go_steal1		-> non
		clr.w	(gobot.flag-t,a5)	raz flag aff. text
		bra.s	.go_seal		-> sort routine
		cmp.w	#'[',d0			Espace ?
		bne.s	.go_steal1		-> Non
		addq.w	#1,(gobot.pt-t,a5)
		addq.w	#1,(gobot.offset-t,a5)
		bra	score_affiche
	
		*/ Calcul adr. fonte à afficher */
.go_steal1	move.l	(fonte2.pagegfx-t,a5),a0
		moveq	#0,d3
		move.w	(PG_SLINE,a0),d3
		lea	(PG_DATA,a0,d0),a0
		moveq	#0,d4
		move.w	(SP_SPLAN,a4),d4		
		move.w	(SPPLAN,a4),d5
		subq.w	#1,d5

		*/ Boucle d'affichage */
		moveq	#7,d2
.go_crazy	move.b	(a0),d0
		move.w	d5,d6
.lunatic	or.b	d0,(a1)
		add.l	d4,a1
		or.b	d0,(a2)
		add.l	d4,a2
		or.b	d0,(a3)
		add.l	d4,a3
		dbf	d6,.lunatic
		add.l	d3,a0
		dbf	d2,.go_crazy

		*/ Prépare affichage caractère suivant */
.go_fun		addq.w	#1,(gobot.pt-t,a5)
		addq.w	#1,(gobot.offset-t,a5)
.go_seal	rts
