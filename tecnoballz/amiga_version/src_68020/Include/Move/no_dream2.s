*=========================================================================*
*            Test si "lost" est terminé alors remet la musique		  *
*=========================================================================*
* entrée	=> a5.l	adr. table 't'
no_dream2:
		tst.w	(musiclost-t,a5)	musique lost en cours ?
		beq	.no_dream1		-> non
		move.l	(pt_poz-t,a5),a0
		move.w	(8,a0),d0		pos. musique shop
		lea	(Variables+MT_SongPos-t,a5),a1
		cmp.b	(a1),d0			zik lost terminée ? 
		bne	.no_dream1		-> non
		move.w	(2,a0),d0
		move.w	(level-t,a5),d1		n° du tableau
		btst	#0,d1
		beq.s	.creve_kapone
		move.w	(a0),d0
.creve_kapone	bsr.l	np_change
		clr.w	(musiclost-t,a5)
		GETMESSAGE	14
		moveq	#0,d0
		move	#100,d1
		move.l	(pt_balle-t,a5),a3
		moveq	#NBALLE-1,d7
.loop		move.w	d0,(BALLE.GLUE,a3)	RAZ flag balle collée
		move.w	d1,(BALLE.START,a3)	RAZ compt temps
		lea	(BALLE.SIZE,a3),a3
		dbf	d7,.loop
		move.w	#1,(music4voix-t,a5)	flag musique 3 voix
.no_dream1	rts
