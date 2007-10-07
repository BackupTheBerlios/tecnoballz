*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*
*                          Débute jeu Tecnoball				  *
*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*
* Entrée	=> a5.l	adr. table 't'
star:	
		bsr.l	megadrive		"SCORE.s" |-> recopie meilleur score
		clr.w	(niv00-t,a5)		raz pt/level
		clr.l	(triche-t,a5)
		IFEQ	A_TRICHE
		move.l	#ETB,(triche-t,a5)
		move.w	#1,(sh.code-t,a5)
		ENDC
		bsr.l	np_end

		*/ Convertit CODEAREA en caractères "J" */
		IFEQ	A_INTRO
		lea	(10+tv2-t,a5),a0		adr. code entré
		lea	(codearea-t,a5),a1
		moveq	#9,d0
		bsr	jconvert
		ENDC

		*/ Init. joueurs */
		bsr	init_players
		bsr	recopie_noms
		clr.b	(flag.quit-t,a5)

		IFEQ	A_TAB
		*/ Test si débute à l' >area 2< */
		move.w	#GARD_F1,(gd.pt-t,a5)	pt/table gardien final area 1
		moveq	#0,d0
		moveq	#2,d2			n° area de départ
		lea	(code.area2-t,a5),a0	adr. code >area 2<
		bsr	gregorien		|-> test si code >area 2<
		bne.s	.ephore			-> oui, commence area 2

		*/ Test si débute à l' >area 3< */
		move.w	#GARD_F2,(gd.pt-t,a5)	pt/table gardien final area 2
		moveq	#3,d2			n° area de départ
		lea	(code.area3-t,a5),a0	adr. code >area 3<
		bsr	gregorien		|-> test si code >area 3<
		bne.s	.ephore			-> oui, commence area 3

		*/ Test si débute à l' >area 4< */
		move.w	#GARD_F3,(gd.pt-t,a5)	pt/table gardien final area 3
		moveq	#4,d2			n° area de départ
		lea	(code.area4-t,a5),a0	adr. code >area 4<
		bsr	gregorien		|-> test si code >area 4<
		bne.s	.ephore			-> oui, commence area 4

		*/ Test si débute à l' >area 5< */
		move.w	#GARD_F4,(gd.pt-t,a5)	pt/table gardien final area 4
		moveq	#5,d2			n° area de départ
		lea	(code.area5-t,a5),a0	adr. code >area 5<
		bsr	gregorien		|-> test si code >area 5<
		bne.s	.ephore			-> oui, commence area 5

		*/ Test si débute uu final l' >area 5< */
		tst.w	(sh.code-t,a5)		CODE DATE DE NAISSANCE ?
		beq	.nosh			-> non
		move.w	#GARD_F5,(gd.pt-t,a5)	pt/table gardien final area 5
		moveq	#6,d2			n° area de départ
		lea	(code.area6-t,a5),a0	adr. code >area 5<
		bsr	gregorien		|-> test si code >area 5<
		bne.s	.ephore			-> oui, commence area 5

		*/ Test si débute au final-final */
		move.w	#GARD_FF,(gd.pt-t,a5)	pt/table gardien final area 5
		moveq	#6,d2			n° area de départ
		lea	(code.area7-t,a5),a0	adr. code >area 5<
		bsr	gregorien		|-> test si code >area 5<
		bne.s	.ephore			-> oui, commence area 5
		ENDC

		*/ Sauve n° de l'area */
.nosh		moveq	#1,d2			commence "area 1"
.ephore:	move.w	#1,(level-t,a5)
		move.w	#1,(lev0-t,a5)
		clr.w	(lev1-t,a5)
.px		move.w	d2,(area-t,a5)		sauve n° area
		IFEQ	A_TAB
		clr.w	(mur.kas_lf-t,a5)
		clr.w	(mur.kas_up-t,a5)
		clr.w	(mur.kas_rg-t,a5)
		clr.w	(score.life-t,a5)
		clr.w	(lessbrick-t,a5)
		clr.w	(gameover-t,a5)
		clr.w	(musiclost-t,a5)
		lea	(gemme_du_j1-t,a5),a0
		clr.l	(a0)+
		clr.w	(a0)
		clr.w	(gemmex-t,a5)
		move.w	#10,(lessbrick.count-t,a5)
		ENDC
		bsr	raton_laveur		|-> init. 6 structures joueurs
		bsr	playerlive		|-> init. le nbre de vie 
		bsr	firstfirst		|-> pointe le 1er joueur

		*/  débute area 1 */
		IFEQ	A_TAB
		cmp.w	#1,(area-t,a5)		débute "area 1" ?
		bne.s	.ephore2		-> non
		clr.w	(gd.pt-t,a5)		raz pt/sur les listes gardiens
		move.w	#1,(level-t,a5)		n° tableau : 1
		move.w	#1,(lev0-t,a5)		unité n° tableau : 1
		clr.w	(lev1-t,a5)		diza. n° tableau : 0
		bsr	super_famicom		|-> copie n° level & area dans liste joueur
		clr.w	(rout-t,a5)		>flag prepare level<
		rts

		* Débute area 2,3,4 & 5 *
.ephore2:	subq.w	#1,(area-t,a5)		commence area précédente
		ENDC
		move.w	#12,(level-t,a5)	n° tableau : 12
		move.w	#2,(lev0-t,a5)		unité n° tableau : 2
		move.w	#1,(lev1-t,a5)		diza. n° tableau : 1
		bsr	super_famicom		|-> copie n° level & area dans liste joueur
		move.w	#4,(rout-t,a5)		>flag init. gardien<
.exit		rts


*=========================================================================*
*                            Compare code entrée			  *
*=========================================================================*
* Entrée	=> d2.w	n° de l'area
*		=> a0.l	adr. du code de l'area
*		=> a5.l	adr. table 't'
* sort		d2.w	n° de l'aera si code bon / 0 si code mauvais
* utilise	d0,d1,d2,3 / a0,a1,a2,a5
gregorien:
		moveq	#0,d3			Difficulté 0
.lz		move.l	a0,a2
		lea	(codearea-t,a5),a1
		moveq	#9,d1			10 caractères par code
.malachite	move.b	(a1)+,d0		lit caractère code entré
		cmp.b	(a2)+,d0		compare au caractère code
		bne.s	.perplexe		-> différent, mauvais code
		dbf	d1,.malachite		-> boucle
		move.w	d3,(hard-t,a5)
		tst.w	d2
		rts				<-| retour, code bon
.perplexe	lea	(10,a0),a0
		addq.w	#1,d3			Difficulté+1
		cmp.w	#4,d3
		blt.s	.lz
		moveq	#0,d2			raz d2, code mauvais
		rts				<-| retour


