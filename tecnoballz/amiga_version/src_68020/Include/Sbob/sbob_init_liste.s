***************************************************************************
************************* INITIALISE LA LISTE SBOB ************************
***************************************************************************
* Entrée	=> a5.l Adr.'t'
* Sortie	<= d7.l=0 / Z=1 pas d'erreur
sbob_init_liste:
		tst.l	(sbob_liste-t,a5)
		beq	.nolist
		tst.l	(pt_screen-t,a5)
		beq	.noscreen
		tst.l	(sbob_pagegfx-t,a5)
		beq	.nopage
		move.l	(pt_screen-t,a5),a0
		move.l	(sbob_liste-t,a5),a4
		move.w	(SPPLAN,a0),d3		d3=nbre de plans
		move.l	(sbob_pagegfx-t,a5),a0
		move.w	(PG_SLINE,a0),d4
		lea	(PG_DATA,a0),a0
		move.l	a0,d5
		move.l	(sbob_pagemsk-t,a5),d6
		add.l	#PG_DATA,d6
		moveq	#0,d7
		move.l	a4,a3
		cmp.l	#'SBOB',(a4)+
		bne.s	.exit	
		move.l	(a4)+,d0		d0=nbre de bob(s)
.loop		move.l	(a4),a1
		add.l	a3,a1
		move.l	a1,(a4)+

		* calcul fenêtre blitter *
		move.w	(TSBOB_HEIGHT,a1),d1	d1=hauteur du bob en lignes
		mulu	d3,d1			*nombre de plan(s)
		lsl.w	#6,d1
		add.w	(TSBOB_WIDTH,a1),d1	+largeur en mots
		addq.w	#1,d1			+1mot pour le décalage
		move.w	d1,(TSBOB_WINDOW,a1)

		* Calcul adr. du Bob et du msk *
		move.w	(TSBOB_NUMBER,a1),d1	d1=nbre d'anim. -1
		lea	(TSBOB_TABLE,a1),a2
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
		dbf	d0,.loop
		bsr	ilata
		bne.s	.zigi			-> out memory
		move.l	(sbob_liste-t,a5),a4
		move.l	#'IBOB',(a4)		liste est initialisé
.exit		move.l	(sbob_liste-t,a5),a4
		cmp.l	#'IBOB',(a4)
		bne.s	.nolist
		moveq	#0,d7
.zigi		rts
.nopage		moveq	#-30,d7			Pas de 'sbob_pagegfx'
		rts
.nolist		moveq	#-31,d7			Pas de 'sbob_liste'
		rts
.noscreen	moveq	#-32,d7			Pas de 'pt_screen'
		rts


*-*-*-*-*-*-*-*-*-*-*-*				   *-*-*-*-*-*-*-*-*-*-*-*-
*»»»»»»»»»»»»»»»»»»»»»» INITIALISE LES "LAST_TABLE" ««««««««««««««««««««««*
*-*-*-*-*-*-*-*-*-*-*-*				   *-*-*-*-*-*-*-*-*-*-*-*-
ilata:
		move.l	(sbob_liste-t,a5),a4
		addq.l	#4,a4
		move.l	(a4),d0			d0=nbre de SBOB(s)
		lsl.l	#4,d0			TAILLE,PT/LASTTABLE/PT/LASTSHADO
		addq.l	#4,d0			+Nbre de LASTTABLE
		moveq	#0,d1
		bsr	reserve_memoire		-> table de travail
		lea	(.wk,pc),a0
		move.l	d0,(a0)
		beq	.outmem
		move.l	d0,a0
		move.l	#-1,(a0)

		move.l	(a4)+,d6		d6=nbre de SBOB(s)
.alasoupe	move.l	(a4)+,a1
		move.l	(TSBOB_WIDTH,a1),d1
		move.l	a0,a2			a2={.wk}
		move.l	(a2)+,d2
		bmi.s	.new
.suchen		cmp.l	(a2)+,d1		même largeur & hauteur ? 
		beq.s	.old			-> oui
		addq.l	#8,a2
		dbf	d2,.suchen
.new		move.l	d1,(a2)+		sauve largeur+hauteur
		move.w	d1,d5			hauteur du SBOB
		swap	d1
		move.w	d1,d4			largeur du SBOB
		addq.w	#1,d4			+ décalage
		bsr	calul_last_table
		bne.s	.outmem
		addq.l	#1,(a0)			une "last_table" de +
		move.l	d0,(a2)			sauve adr. de la last_table
		move.l	d1,(4,a2)
.old		move.l	(a2)+,(TSBOB_LASTTABLE,a1)
		move.l	(a2),(TSBOB_LASTSHADO,a1)
		dbf	d6,.alasoupe

		*/ libère mémoire de travail */
		move.l	(.wk,pc),d0
		bsr	libere_memoire
		moveq	#0,d7
		rts
.outmem		moveq	#-12,d7
		rts
		CNOP	0,4
.wk		dc.l	0

*-----------------					--------------*
*»»»»»»»»»»»»»»»» CALCUL TABLE D'AFFICHAGE POUR SBOB «««««««««««««««««*
*-----------------					--------------*
* Entrée	=> d4 	Largeur du SBOB en mots (décalage compris)
*		=> d5  	Hauteur du SBOB en lignes
* Sortie	<= d0.l	Adr. de la "last_table" des dessins (tous les plans)
*		<= d1.l Adr. de la "last_table" des ombres (un plan)
calul_last_table:
		movem.l	d2-d6/a0-a4,-(a7)
		move.l	(pt_screen-t,a5),a4
		moveq	#0,d0
		move.w	(SPHAUTEUR,a4),d0	d0=nbre lignes page écran
		lsl.l	#4,d0			4 octets pour une ligne (mais on double le nombre de ligne)
		moveq	#0,d1
		bsr	reserve_memoire
		tst.l	d0
		beq	.outmem			-> dépassement mémoire
		move.l	d0,-(a7)
		move.l	d0,a0
		move.w	(SPHAUTEUR,a4),d0	d0=nbre lignes page écran
		add.w	d0,d0			nbre de lignes à calculer
		subq.w	#1,d0
		move.w	(SPHAUTEUR,a4),d1	pos. y maximum
		sub.w	(SPDOWN,a4),d1
		moveq	#0,d2			commence  y=0
		move.w	d5,d6			hauteur du SBOB
		mulu	(SPPLAN,a4),d6		*nombre de plans
		lsl.w	#6,d6
		add.w	d4,d6			d6=fenêtre Blitter normale
.loop		cmp.w	d2,d1			dépasse n° ligne maxi ?
		bne.s	.no_maxy		-> non
		moveq	#0,d2			oui, y = 0
.no_maxy	move.w	d2,d3			d3=y
		add.w	d5,d3			+ hauteur du SBOB
		cmp.w	d1,d3			dernière ligne bob > dernière ligne écran ?
		bhi.s	.two_win		-> oui, 2 fenêtre blitter
		move.w	d6,(a0)+		non, 1 fenêtre blitter
		clr.w	(a0)+			raz deuxième fenêtre Blitter
		bra.s	.next			-> boucle
.two_win	sub.w	d1,d3			dern. ligne bob - maxy = haut. 2ieme fenêtre blitter
		move.l	d3,-(a7)
		mulu	(SPPLAN,a4),d3		*nombre de plans
		lsl.w	#6,d3
		add.w	d4,d3			+ largeur en mots du SBOB
		move.w	d3,(2,a0)		sauve 2ieme fenêtre
		move.l	(a7)+,d3
		sub.w	d5,d3			- hauteur du SBOB
		neg.w	d3			hauteur 1ere fenêtre blitter
		mulu	(SPPLAN,a4),d3		*nombre de plans
		lsl.w	#6,d3
		add.w	d4,d3			+ largeur en mots du SBOB
		move.w	d3,(a0)+		sauve 1ere fenêtre blitter
		addq.l	#2,a0
.next		addq.w	#1,d2			y=y+1
		dbf	d0,.loop
		move.l	a0,-(a7)		sauve adr. "last-table" shadow

		*/ Calcul LAST-TABLE des shadows */
		move.w	(SPHAUTEUR,a4),d0	d0=nbre lignes page écran
		add.w	d0,d0			nbre de lignes à calculer
		subq.w	#1,d0
		move.w	d5,d6			hauteur du SBOB
		lsl.w	#6,d6			fenêtre Blitter sur un plan
		add.w	d4,d6			d6=fenêtre Blitter normale
		moveq	#ODECAFEINE,d2		commence  y=3
.shloop		cmp.w	d2,d1			dépasse n° ligne maxi ?
		bne.s	.shno_maxy		-> non
		moveq	#0,d2			oui, y = 0
.shno_maxy	move.w	d2,d3			d3=y
		add.w	d5,d3			+ hauteur du SBOB
		cmp.w	d1,d3			dernière ligne bob > dernière ligne écran ?
		bhi.s	.shtwo_win		-> oui, 2 fenêtre blitter
		move.w	d6,(a0)+		non, 1 fenêtre blitter
		clr.w	(a0)+			raz deuxième fenêtre Blitter
		bra.s	.shnext			-> boucle
.shtwo_win	sub.w	d1,d3			dern. ligne bob - maxy = haut. 2ieme fenêtre blitter
		move.l	d3,-(a7)
		lsl.w	#6,d3
		add.w	d4,d3			+ largeur en mots du SBOB
		move.w	d3,(2,a0)		sauve 2ieme fenêtre
		move.l	(a7)+,d3
		sub.w	d5,d3			- hauteur du SBOB
		neg.w	d3			hauteur 1ere fenêtre blitter
		lsl.w	#6,d3
		add.w	d4,d3			+ largeur en mots du SBOB
		move.w	d3,(a0)+		sauve 1ere fenêtre blitter
		addq.l	#2,a0
.shnext		addq.w	#1,d2			y=y+1
		dbf	d0,.shloop

		move.l	(a7)+,d1
		move.l	(a7)+,d0
		movem.l	(a7)+,d2-d6/a0-a4
		moveq	#0,d7
		rts
.outmem		movem.l	(a7)+,d2-d6/a0-a4
		moveq	#-12,d7		¤	dépassement mémoire
		rts
