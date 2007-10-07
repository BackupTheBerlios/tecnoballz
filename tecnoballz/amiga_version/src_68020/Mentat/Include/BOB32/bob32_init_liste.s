*« » « » « » « » « » « » « » « » « » « » « » « » « » « » « » « » « » « » «*
*<>.<>.<>.<>.<>.<>.<>.<>* Initialise la liste BOB *<>.<>.<>.<>.<>.<>.<>.<>*
*« » « » « » « » « » « » « » « » « » « » « » « » « » « » « » « » « » « » «*
* Entrée	=> a5.l Adr.'t'
* Sortie	<= d7.l=0 / Z=1 pas d'erreur
bob32_init_liste:
		tst.l	(bob32_liste-t,a5)
		beq	.nolist
		tst.l	(pt_screen-t,a5)
		beq	.noscreen
		tst.l	(bob32_pagegfx-t,a5)
		beq	.nopage
		move.l	(pt_screen-t,a5),a0
		move.l	(bob32_liste-t,a5),a4
		move.w	(SPPLAN,a0),d3		d3=nbre de plans
		move.l	(bob32_pagegfx-t,a5),a0
		move.w	(PG_SLINE,a0),d4
		lea	(PG_DATA,a0),a0
		move.l	a0,d5
		move.l	(bob32_pagemsk-t,a5),d6
		add.l	#PG_DATA,d6
		moveq	#0,d7
		move.l	a4,a3
		cmp.l	#'SBOB',(a4)+
		bne.s	.exit	
		move.l	(a4)+,d0		d0=Nbre de BOBs

.loop		move.l	(a4)+,d1
		beq	.nextbob
		move.l	d1,a1
		add.l	a3,a1
		move.l	a1,(-4,a4)

		* calcul fenêtre blitter *
		move.w	(TBOB_HEIGHT,a1),d1	d1=hauteur du bob en lignes
		mulu	d3,d1			*nombre de plan(s)
		lsl.w	#6,d1
		add.w	(TBOB_WIDTH,a1),d1	+largeur en mots
		addq.w	#1,d1			+1mot pour le décalage
		move.w	d1,(TBOB_WINDOW,a1)
		move.w	(TBOB_HEIGHT,a1),d1	d1=hauteur du bob en lignes
		lsl.w	#6,d1
		add.w	(TBOB_WIDTH,a1),d1	+largeur en mots
		addq.w	#1,d1			+1mot pour le décalage
		move.w	d1,(TBOB_WINDOW2,a1)	pour les ombres

		* calcul adr. du bob et du msk *
		move.w	(TBOB_NUMBER,a1),d1	d1=nbre d'anim. -1
		lea	(TBOB_TABLE,a1),a2
.loop_anim	move.w	(a2),d7			d7=pos. x en mots
		add.w	d7,d7			convertit en octets
		move.w	(2,a2),d2		d1=pos.y en lignes
		mulu	d4,d2			*taille d'une ligne
		add.l	d7,d2			+pos. x
		move.l	d2,(a2)			sauve adr. relative
		add.l	d5,(a2)+		+adr. page gfx
		add.l	d6,d2			+adr. page msk
		move.l	d2,(a2)+
		dbf	d1,.loop_anim
.nextbob	dbf	d0,.loop

		move.l	(bob32_liste-t,a5),a4
		move.l	#'IBOB',(a4)		liste est initialisé
.exit		move.l	(bob32_liste-t,a5),a4
		cmp.l	#'IBOB',(a4)
		bne.s	.nolist
		moveq	#0,d7
		rts
.nopage		moveq	#-30,d7			Pas de 'bob32_pagegfx'
		rts
.nolist		moveq	#-31,d7			Pas de 'bob32_liste'
		rts
.noscreen	moveq	#-32,d7			Pas de 'pt_screen'
		rts

