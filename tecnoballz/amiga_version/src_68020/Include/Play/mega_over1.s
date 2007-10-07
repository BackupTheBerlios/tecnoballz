*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*
********************* ROUT=12 : Init GameOver Gardien *********************
*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*
* Entrée	=>	a5.l	Adr. 't'
*		=> 	a6.l	CUSTOM
mega_over1:
		bsr.l	np_end
		bsr	pointeplayer
		bsr	triscore
		tst.w	(tecnowinner-t,a5)
		beq.s	.bb
		lea	(mod.winne-t,a5),a3
		bra.s	.casimir_tf1
.bb		tst.l	d7
		beq.s	.non_classe		-> joueur non classé
		lea	(mod.hisco-t,a5),a3
		bra.s	.casimir_tf1
.non_classe	lea	(mod.endth-t,a5),a3
.casimir_tf1	bsr.l	np_init
		clr.w	(music.on-t,a5)		autorise saut à la playroutine

		*/ Décompacte le map-editor */
		tst.w	(tecnowinner-t,a5)
		beq.s	.bb2
		lea	(maped2.pp-t,a5),a4
		move.l	(RESA.ADR,a4),d0
		move.l	(RESA.SIZ,a4),d1
		lea	(mapeditor-t,a5),a4
		bsr.l	decrunch.pp
		bne.l	sys_erreur
.bb2		bsr.l	wait_vbl
		bsr	last_flip		|-> Echange listes Copper
		bsr	goscroll		£=> Affiche défilement
		bsr	ssha_restore
		bsr	sbob_restore
		bsr	ssha_affiche		|-> Affiche Shadows-SBOBs
		bsr	sbob_affiche		|-> Affiche Scroll-BOBs

		*/ Reserve les structures SBOBs */
		lea	(go.list-t,a5),a4
		moveq	#SBOB.GGGG,d6
.loopooo	move.l	d6,d0
		bsr	sbob.ajoute
		bne.l	sys_erreur
		bsr	ssha.only
		bne.l	sys_erreur
		move.l	a1,(a4)+
		addq.l	#1,d6
		cmp.w	#SBOB.RRRR+1,d6
		bne.s	.loopooo

		*/ Initialise les lettres 'GAME OVER' */
		lea	(go.list-t,a5),a3
		moveq	#0,d0
		lea	(zeus_over-t,a5),a0	table pos. initiale
		moveq	#NLETTRE-1,d5		6 lettres
.init_lettres:	move.l	(a3)+,a4
		move.l	#'BOB2',(SBOB.ACTIF,a4)
		move.w	(a0)+,(SBOB.POSX1,a4)
		move.w	(a0)+,(SBOB.POSY1,a4)
		move.w	#100,(SBOB.POSX,a4)
		move.w	#100,(SBOB.POSY,a4)
		dbf	d5,.init_lettres

		*/ Recopie la la palette */
		move.l	(pt_screen-t,a5),a0
		move.l	(pt_overpal-t,a5),a1
		tst.w	(tecnowinner-t,a5)
		beq.s	.bb4
		move.l	(pt_winnpal-t,a5),a1
.bb4		lea	(SCOLOR_TABLE,a0),a2
		move.w	#32-1,d0
.loop		move.l	(a1)+,(a2)+
		dbf	d0,.loop
		bsr	lutintin
		bne.l	sys_erreur

		*/ Détermine vitesse du défilement */
		move.w	(hasard-t,a5),d0
		and.w	#7,d0
		move.w	(.pt,pc,d0*2),(gd.sspeed-t,a5)
		tst.w	(tecnowinner-t,a5)
		beq.s	.bb3
		move.w	#-1,(gd.sspeed-t,a5)
		bsr	affiche_dalles
		bsr	copy_s1_to_s2s3
		clr.w	(sh.f-t,a5)
		moveq	#0,d0
		move.w	d0,(nu_video-t,a5)
		move.w	d0,(afline-t,a5)
		move.w	d0,(sc_y1-t,a5)
		move.w	d0,(ffecran-t,a5)
		move.w	d0,(flag_co-t,a5)
		move.w	#150,(gd.stemp-t,a5)	tempo défilement
		bsr	init.boule
		bne.l	sys_erreur
.bb3		move.w	#50*10,(go.tempo-t,a5)	compteur temps même dep. lettre
		moveq	#0,d0
		move.w	d0,(go.dep-t,a5)	pt 1er routine dépl. lettre
		move.w	d0,(gameover.out-t,a5)	raz flag sort magasin/gameover
		move.w	d0,(music4voix-t,a5)	musique 4 voix
		move.w	d0,(musicsound-t,a5)	son + musique
		move.w	d0,(gameover-t,a5)
		move.w	#13,(rout-t,a5)		>flag game over<
		tst.w	(tecnowinner-t,a5)
		bne.s	.xcxcxvsg5
		bsr.l	init_pts_lutins2
		bsr.l	colors_on
		bra	irq
.xcxcxvsg5	move.w	#255,(flag_fade_on-t,a5)
		bra	irq
.pt		dc.w	+1,-1,-2,-2,+3,-3,+2,+1


*[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][*
********************** Crée liste lutin des HiScores **********************
*[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][*
* Entrée	=> a5.l	Adr. 't'
lutintin:
		*/ Reserve les 3 listes Lutins /*
		move.l	a6,-(a7)
		lea	(go.beast-t,a5),a4
		move.l	#16*HSGO*3,(RESA.SIZ,a4)
		bsr.l	reserve.mem
		bne	.exit

		*/ Calcul mots de contrôles */
		move.l	(coordx-t,a5),d6
		move.l	(coordy-t,a5),d7
		lea	(table_lutins-t,a5),a3
		move.l	(go.beast-t,a5),a0
		moveq	#72,d0
		bsr	.caramel		|-> Lutin 0
		move.l	#72+64,d0
		lea	(16*HSGO,a0),a0
		bsr	.caramel		|-> Lutin 1
		move.l	#72+128,d0
		lea	(16*HSGO,a0),a0
		bsr	.caramel		|-> Lutin 2

		*/ Calcul adresse des 3 lutins */
 		move.l	(pt_score-t,a5),a1
		move.w	(hard-t,a5),d0
		mulu	#OFF.SCORE,d0
		lea	(IS+KS+NS+4,a1,d0),a1
		move.l	(go.beast-t,a5),d0
		add.l	#16,d0			d0=pt/Lutin 0
		move.l	d0,d1
		add.l	#16*HSGO,d1		d1=pt/Lutin 1
		move.l	d1,d2
		add.l	#16*HSGO,d2		d2=pt/Lutin 3
		move.l	(fonte2.pagegfx-t,a5),a0
		moveq	#0,d3
		move.w	(PG_SLINE,a0),d3
		moveq	#0,d4
		lea	(PG_DATA,a0),a0
		move.l	#10*16,d5
		moveq	#9,d7
		lea	(.print,pc),a6

		*/ Boucle d'affichage */
.loop		moveq	#7,d6
		move.l	d0,a4
		jsr	(a6)	|-> Lutin 0
		add.l	d5,d0
		moveq	#7,d6
		move.l	d1,a4
		jsr	(a6)	|-> Lutin 1
		add.l	d5,d1
		moveq	#6,d6
		move.l	d2,a4
		jsr	(a6)	|-> Lutin 2
		add.l	d5,d2
		addq.l	#5,a1
		dbf	d7,.loop

		moveq	#0,d7
.exit		move.l	(a7)+,a6
		tst.l	d7
		rts

*----------------> Affiche fonte(s) dans une liste lutin <----------------*
* Entrée	=> d6.w	Nombre de caractère(s) - 1	
*		=> a0.l	Adr. fontes
*		=> a1.l	Pt/text
*		=> a4.l	Pt/lutin 64 pixels
.print:		move.b	(a1)+,d4		d4=n° du caractère
		lea	(a0,d4),a2		a2=pt/fonte
		move.l	a4,a3
		move.b	(a2),(a3)		Ligne 1
		lea	(16,a3),a3
		add.l	d3,a2
		move.b	(a2),(a3)		Ligne 2
		lea	(16,a3),a3
		add.l	d3,a2
		move.b	(a2),(a3)		Ligne 3
		lea	(16,a3),a3
		add.l	d3,a2
		move.b	(a2),(a3)		Ligne 4
		lea	(16,a3),a3
		add.l	d3,a2
		move.b	(a2),(a3)		Ligne 5
		lea	(16,a3),a3
		add.l	d3,a2
		move.b	(a2),(a3)		Ligne 6
		lea	(16,a3),a3
		add.l	d3,a2
		move.b	(a2),(a3)		Ligne 7
		lea	(16,a3),a3
		add.l	d3,a2
		move.b	(a2),(a3)		Ligne 8
		addq.l	#1,a4
		dbf	d6,.print
		rts

*******************> Calcul mot de contrôle d'un lutin <*******************
* Entrée	=> d0.l	Positon X
*		=> a0.l Pt/lutin
.caramel	lsl.w	#2,d0
		moveq	#78,d1
		moveq	#HSGO-2,d2
		bsr.l	calcul_mot_ctrl_aga
		move.w	d3,(8,a0)
		swap	d3
		move.w	d3,(a0)	
		move.l	a0,(a3)+
		rts

*=========================================================================*
*----------------->Initialise les  boules du TecnoWinner<-----------------*
*=========================================================================*
* Entrée	=>	a5.l	Adr. 't'
init.boule:
		*/ Réserve les structures */
		lea	(pt_boule-t,a5),a4
		move.l	#SBOB.SIZE*NBR_BOULE,(RESA.SIZ,a4)
		bsr.l	reserve.mem
		bne.s	.exit

		*/ Init. les structures */
		move.l	#360,d5
		divu	#NBR_BOULE,d5
		add.w	d5,d5
		moveq	#0,d4
		move.l	(pt_boule-t,a5),a1
		moveq	#NBR_BOULE-1,d6
.loopinit	moveq	#SBOB.BOUL,d0
		bsr	sbob.only
		bne	.exit
		bsr	ssha.only
		bne	.exit
		move.w	d4,(SBOB.POSX1,a1)
		move.l	#'BOB2',(SBOB.ACTIF,a1)
		add.w	d5,d4
		lea	(SBOB.SIZE,a1),a1
		dbf	d6,.loopinit
		moveq	#0,d7
.exit		rts
