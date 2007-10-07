***************************************************************************
******************  Déplacement & Collision des Gemmes  *******************
***************************************************************************
gestion_gemme:
		lea	(b1-t,a5),a0
		move.w	(BUMP.X,a0),d2
		move.w	(BUMP.Y,a0),d3
		move.w	d2,d4
		add.w	(rak.large-t,a5),d4
		move.w	d3,d5
		addq.w	#8,d5
		move.w	#YMAX,d6
		moveq	#NGEMME-1,d7
		move.l	(pt_gemme-t,a5),a4

.loop		tst.w	(a4)
		beq	.cdi01

		*/ Déplacement & test limites */
		addq.w	#1,(GEM.Y,a4)
		move.l	(GEM.X,a4),d0
		cmp.w	d6,d0
		bhi	.clear

		*/ Collision raquette & capsule */
		cmp.w	d5,d0			GEM.Y > BUMP.Y+8
		bhi	.cdi01
		addq.w	#8,d0
		sub.w	d5,d0			GEM.Y+8 < BUMP.Y
		blt	.cdi01
.testx		swap	d0
		addq.w	#3,d0
		cmp.w	d4,d0			GEM.X+3 > BUMP.X+32
		bhi	.cdi01
		addq.w	#1,d0
		sub.w	d2,d0			GEM.X+4 < BUMP.X
		blt	.cdi01
		lea	(venus2,pc),a3
		jsr	(a3)
		jsr	(a3)
		jsr	(a3)
		jsr	(a3)
		jsr	(a3)
		add.w	#500,(score.life-t,a5)


		move.b	#1,(sound+S_MONNAIE-t,a5)

		move.w	(GEM.NUM,a4),d0
		tst.b	(gemme_du_j1-t,a5,d0)	dèjà collectée ?
		bne	.clear
		not.b	(gemme_du_j1-t,a5,d0)
		addq.w	#1,(gemmex-t,a5)

		*/ Affichage de la Gemme dans l'écran 1 & 2 */ 
		move.l	(gprint-t,a5),a3
		lea	(gemme.adr-t,a5,d0*8),a0
		move.l	(a0)+,a1
		move.l	(a0),d0
		move.l	(ecran1-t,a5),a2
		add.l	d0,a2
		jsr	(a3)
		move.l	(ecran2-t,a5),a2
		add.l	d0,a2
		jsr	(a3)
		move.l	(ecran3-t,a5),a2
		add.l	d0,a2
		jsr	(a3)

		*/ Le joueur à toutes les gemmes */
		cmp.w	#6,(gemmex-t,a5)
		bne	.clear
		moveq	#0,d0
		move.w	d0,(gemmex-t,a5)
		lea	(gemme_du_j1-t,a5),a0
		move.l	d0,(a0)+
		move.w	d0,(a0)+
		moveq	#0,d0
		bsr	venus3			* Donne 2500 points *
		bsr	venus3
		lea	(venus2,pc),a3
		jsr	(a3)
		jsr	(a3)
		jsr	(a3)
		jsr	(a3)
		jsr	(a3)
		add.w	#2500,(score.life-t,a5)
		lea	jupiter100,a3		* Donne 500 crédits *
		jsr	(a3)
		jsr	(a3)
		jsr	(a3)
		jsr	(a3)
		jsr	(a3)
		lea	(lune1,pc),a3		* Donne 3 vies *
		jsr	(a3)
		jsr	(a3)
		jsr	(a3)
		moveq	#4,d0			* Augmente temps des raquettes *
		lea	(b2-t,a5),a3
		add.w	d0,(BUMP.ACTIF,a3)
		lea	(b3-t,a5),a3
		add.w	d0,(BUMP.ACTIF,a3)
		lea	(b4-t,a5),a3
		add.w	d0,(BUMP.ACTIF,a3)
		bsr	voleur			|-> Passe au niveau suivant
.clear		clr.w	(a4)
.cdi01		lea	(GEM.SIZE,a4),a4
		dbf	d7,.loop
		rts
