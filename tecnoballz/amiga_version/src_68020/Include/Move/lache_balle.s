;=========================================================================;
*                      bouton droit, lache la balle			  *
;=========================================================================;
* Entrée	a5.l	adr. table 'tlk'
lache_balle:
		tst.w	(rak.team-t,a5)		mode team ?
		beq	lache_alon		-> non

		*/*	mode team port 1 : lache balle test bouton */*
		cmp.w	#BOUTON_LACHE,(mouse0-t,a5)
		bne	.every_team2		-> non
		move.l	(pt_balle-t,a5),a3
		moveq	#0,d0
		moveq	#0,d1
		moveq	#NBALLE-1,d2		nombre de balle à tester
.lache_start	move.w	(BALLE.GLUE,a3,d1),d0	test flag balle collée
		bne.s	.lache_loop		-> balle collée
.lache_suite	add.w	#BALLE.SIZE,d1
		dbf	d2,.lache_start
		bra	.every_team2
.lache_loop	and.w	#1,d0			ne garde que le bit 0
		beq.s	.lache_suite		-> balle du joueur 2
		tst.w	(musiclost-t,a5)	musique "lost" en cours ?
		beq.s	.po_rosanna		-> non
		clr.w	(musiclost-t,a5)	raz flag musique "lost" en cours
		move.l	(pt_poz-t,a5),a0	adr. table des pos. du module
		move.w	(2,a0),d0		n° pos. musique "jeu" n°2
		move.w	(level-t,a5),d3		n° du tableau 1 à 12
		btst	#0,d3			n° tableau paire ?
		beq.s	.preve_kapone		-> oui, pointe musique "jeu" n°2
		move.w	(a0),d0			n° pos. musique "jeu" n°1
.preve_kapone	bsr.l	np_change
		move.w	#1,(music4voix-t,a5)	flag musique 3 voix
		GETMESSAGE	02

		*/* mode team port 1 : enlève le pointeur de balle */*
.po_rosanna	moveq	#0,d0
		lea	(b1-t,a5),a4
		move.l	d0,(BUMP.BALLE,a4)
		move.l	d0,(2*BUMP.SIZE+BUMP.BALLE,a4)
		move	d0,(BALLE.GLUE,a3,d1)

		*/* mode team port 1 : met flag raquette colle sans balle */
		moveq	#1,d0
		tst.w	(BUMP.GLUE,a4)
		beq.s	.keft1
		move.w	d0,(BUMP.GLUE,a4)
.keft1		tst.w	(2*BUMP.SIZE+BUMP.GLUE,a4)
		beq.s	.keft2
		move.w	d0,(2*BUMP.SIZE+BUMP.GLUE,a4)
.keft2		bra	.lache_suite

		*/ mode team port 2 : lache balle test si appui du joueur */
.every_team2	cmp.w	#BOUTON_LACHE,(mouse1-t,a5)
		bne	creve_3
		move.l	(pt_balle-t,a5),a3
		moveq	#0,d0
		moveq	#0,d1
		moveq	#NBALLE-1,d2
.mache_start	move.w	(BALLE.GLUE,a3,d1),d0
		bne.s	.mache_loop		-> balle collée
.mache_suite	add.w	#BALLE.SIZE,d1
		dbf	d2,.mache_start
		bra	creve_3
.mache_loop	and.w	#1,d0			ne garde que le bit 0
		bne.s	.mache_suite		-> balle du joueur 1
		tst.w	(musiclost-t,a5)	musique "lost" en cours ?
		beq.s	.mo_rosanna		-> non
		clr.w	(musiclost-t,a5)	raz flag musique "lost" en cours
		move.l	(pt_poz-t,a5),a0	adr. table des pos. du module
		move.w	(2,a0),d0		n° pos. musique "jeu" n°2
		move.w	(level-t,a5),d3		n° du tableau 1 à 12
		btst	#0,d3			n° tableau paire ?
		beq.s	.mreve_kapone		-> oui, pointe musique "jeu" n°2
		move.w	(a0),d0			n° pos. musique "jeu" n°1
.mreve_kapone	bsr.l	np_change
		move.w	#1,(music4voix-t,a5)	flag musique 3 voix
		GETMESSAGE	02

		*/*	mode team port 2 : enlève le pointeur de balle */*
.mo_rosanna	moveq	#0,d0
		lea	(b2-t,a5),a4
		move.l	d0,(BUMP.BALLE,a4)
		move.l	d0,(2*BUMP.SIZE+BUMP.BALLE,a4)
		move	d0,(BALLE.GLUE,a3,d1)

		*/*	mode team port 2 : flag raquette colle sans balle */*
		moveq	#1,d0
		tst.w	(BUMP.GLUE,a4)
		beq.s	.meft1
		move.w	d0,(BUMP.GLUE,a4)
.meft1		tst.w	(2*BUMP.SIZE+BUMP.GLUE,a4)
		beq.s	.meft2
		move.w	d0,(2*BUMP.SIZE+BUMP.GLUE,a4)
.meft2		bra	.lache_suite

;=========================================================================;
;              Bouton droit, lache la balle en mode "alone" 		  ;
;=========================================================================;
lache_alon:
		cmp.w	#BOUTON_LACHE,(mouse0-t,a5)
		bne	creve_3

		*/ lache balle mode "alone" : remet zik "jeu", si musique "lost" */*
.every_one	tst	(musiclost-t,a5)	musique "lost" en cours ?
		beq.s	.no_rosanna		-> non
		clr.w	(musiclost-t,a5)	raz flag musique "lost" en cours
		move.l	(pt_poz-t,a5),a0	adr. table des pos. du module
		move.w	(2,a0),d0		n° pos. musique "jeu" n°2
		move.w	(level-t,a5),d1		n° du tableau 1 à 12
		btst	#0,d1			n° tableau paire ?
		beq.s	.kreve_kapone		-> oui, pointe musique "jeu" n°2
		move.w	(a0),d0			n° pos. musique "jeu" n°1
.kreve_kapone	bsr.l	np_change
		move.w	#1,(music4voix-t,a5)	flag musique 3 voix
		GETMESSAGE	02

		*/* lache balle mode "alone" : raz flag balles collées */
.no_rosanna:	moveq	#0,d0
		move.w	(balle_glue-t,a5),d1
		move.l	(pt_balle-t,a5),a3
V		SET	0
		REPT	NBALLE
		move	d0,(V*BALLE.SIZE+BALLE.GLUE,a3)
		move	d1,(V*BALLE.SIZE+BALLE.START,a3)
V		SET	V+1
		ENDR

		*/* lache balle mode "alone" : supprime pointeur de balle */*
		lea	(b1-t,a5),a3
		move.l	d0,(BUMP.BALLE,a3)
		move.l	d0,(1*BUMP.SIZE+BUMP.BALLE,a3)
		move.l	d0,(2*BUMP.SIZE+BUMP.BALLE,a3)
		move.l	d0,(3*BUMP.SIZE+BUMP.BALLE,a3)

		*/* lache balle mode "alone" : flag raquette colle sans balle */*
		moveq	#1,d0
		tst.w	(BUMP.GLUE,a3)		raquette bas colle ?
		beq.s	.left1			-> non
		move.w	d0,(BUMP.GLUE,a3)	flag raquette colle libre
.left1		tst.w	(1*BUMP.SIZE+BUMP.GLUE,a3)
		beq.s	.left2
		move.w	d0,(1*BUMP.SIZE+BUMP.GLUE,a3)
.left2		tst.w	(2*BUMP.SIZE+BUMP.GLUE,a3)
		beq.s	.left3
		move.w	d0,(2*BUMP.SIZE+BUMP.GLUE,a3)
.left3		tst.w	(3*BUMP.SIZE+BUMP.GLUE,a3)
		beq.s	creve_3
		move.w	d0,(3*BUMP.SIZE+BUMP.GLUE,a3)
creve_3		rts
