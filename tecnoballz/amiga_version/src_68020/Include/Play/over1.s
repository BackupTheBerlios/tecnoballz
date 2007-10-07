*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*
******************** ROUT=9 : Init. GameOver Tableaux *********************
*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*
over1:
		bsr.l	np_end
		*/ Retour à l'éditeur */
		tst.w	(editor-t,a5)		Editeur en cours ?
		beq.s	.noed			-> non
		bsr.l	np_end
		bsr	rupt1.off
		move.w	#14,(rout-t,a5)
		bra	irq
.noed
		bsr	pointeplayer
		bsr	triscore
		tst.l	d7
		beq.s	.non_classe		-> joueur non classé
		lea	(mod.hisco-t,a5),a3
		bra.s	.casimir_tf1
.non_classe	lea	(mod.endth-t,a5),a3
.casimir_tf1	bsr.l	np_init
		clr.w	(music.on-t,a5)		autorise saut à la playroutine
		move.w	#1,(music34-t,a5)	flag musique 4 cannaux

		*/ Reserve les structures BOBs */
		lea	(go.list-t,a5),a4
		moveq	#BOB.G,d6
.loopooo	move.l	d6,d0
		bsr	bob32.ajoute
		bne.l	sys_erreur
		bsr	shado.ajoute
		bne.l	sys_erreur
		move.l	a1,(a4)+
		addq.l	#1,d6
		cmp.l	#BOB.R+1,d6
		bne.s	.loopooo

		*/ initialise les lettres 'GAME OVER' */
		lea	(go.list-t,a5),a3
		moveq	#0,d0
		lea	(zeus_over-t,a5),a0	table pos. initiale
		moveq	#NLETTRE-1,d5		6 lettres
.init_lettres:	move.l	(a3)+,a4
		move.l	#'BOB1',(BOB.ACTIF,a4)
		move.w	(a0)+,(BOB.POSX1,a4)
		move.w	(a0)+,(BOB.POSY1,a4)
		move.w	#100,(BOB.POSX,a4)
		move.w	#100,(BOB.POSY,a4)
		dbf	d5,.init_lettres

		*/ Initialise structure affichage score */
		moveq	#0,d0
		move.w	#1,(gobot.flag-t,a5)	flag: 1= affiche les text
		move.w	d0,(gobot.offset-t,a5)
		move	d0,(gobot.pt-t,a5)	pointeur caracteres
		move.l	(pt_screen-t,a5),a0
		move.l	(pt_score-t,a5),a1
		move.w	(hard-t,a5),d0
		mulu	#OFF.SCORE,d0
		lea	(IS,a1,d0),a1
		move.w	(a1)+,d0
		mulu	(SP_SLINE,a0),d0
		add.w	(a1),d0
		move.l	d0,(gobot.adr-t,a5)

		*/ init game over : raz qqc adr. */
		move.w	#50*10,(go.tempo-t,a5)	compteur temps même dep. lettre
		moveq	#0,d0
		move.w	d0,(go.aff-t,a5)	raz pt/lettre à afficher
		move.w	d0,(go.dep-t,a5)	pt 1er routine dépl. lettre
		move.w	d0,(gameover.out-t,a5)	raz flag sort magasin/gameover
		move.w	d0,(gameover-t,a5)
		move.w	d0,(music4voix-t,a5)	musique 4 voix
		move.w	d0,(musicsound-t,a5)	son + musique
		move.w	#10,(rout-t,a5)		>flag game over<
		bra	irq
