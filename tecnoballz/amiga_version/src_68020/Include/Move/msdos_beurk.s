*=========================================================================*
*                    Déplacement des capsules monnaie			  *
*=========================================================================*
* Entrée	=> a5.l	adr. table 't'
msdos_beurk:
		moveq	#0,d6
		move.l	(pt_capsule-t,a5),a4
		moveq	#NMONNAIE-1,d7
.capsloop	tst.w	(CAP.ACTIF,a4)		Capsule Monnaie active ?
		beq.s	.cdtv			-> Non
		move.l	(CAP.DEP,a4),a0
		jsr	(a0)			|-> routine déplacement
.cdtv		lea	(CAP.SIZE,a4),a4
		dbf	d7,.capsloop
		bra	cap_fin			-> non

		*-=- déplacement en bas + test dépass. -=-*
cap_bas:	move.w	(CAP.SPEED,a4),d0
		add.w	d0,(CAP.Y,a4)
		cmp.w	#NLIGNE-10,(CAP.Y,a4)
		bge	cap_god
		lea	(b1-t,a5),a1
		move.w	(rak.large-t,a5),d4
		moveq	#8,d5
		bra.s	cap_idem

		*-=- déplacement en haut + test dépass. -=-*
cap_haut:	move.w	(CAP.SPEED,a4),d0
		sub.w	d0,(CAP.Y,a4)
		cmp.w	#10,(CAP.Y,a4)
		bmi	cap_god
		lea	(b3-t,a5),a1
		move.w	(rak.large-t,a5),d4
		moveq	#8,d5
		bra.s	cap_idem

		*-=- déplacement à droite + test dépass. -=-*
cap_est:	move.w	(CAP.SPEED,a4),d0
		add.w	d0,(CAP.X,a4)
		cmp	#256-16,(CAP.X,a4)
		bge	cap_god
		lea	(b2-t,a5),a1
		moveq	#9,d4
		move.w	(rak.large-t,a5),d5
		bra.s	cap_idem

		*-=- déplacement à gauche + test dépass. -=-*
cap_ouest:	move.w	(CAP.SPEED,a4),d0
		sub.w	d0,(CAP.X,a4)
		cmp.w	#3,(CAP.X,a4)
		bls	cap_god
		lea	(b4-t,a5),a1
		moveq	#9,d4
		move.w	(rak.large-t,a5),d5

		*-=- test collisions capsule/raquette -=-*
cap_idem:	move.w	(BUMP.X,a1),d0		x raquette
		move.w	(BUMP.Y,a1),d1		y raquette
		move.w	(CAP.X,a4),d2		x capsule
		move.w	(CAP.Y,a4),d3		y capsule
		add.w	#16,d2
		cmp.w	d0,d2
		blt	cap_fin
		addq.w	#7,d3
		cmp.w	d1,d3
		blt.s	cap_fin
		add.w	d4,d0
		move.w	(CAP.X,a4),d2
		cmp.w	d0,d2
		bgt.s	cap_fin
		add.w	d5,d1
		move.w	(CAP.Y,a4),d3
		cmp.w	d1,d3
		bgt.s	cap_fin
		move.w	#YINVERS-5,(invy-t,a5)

		*-=- capsule touche raquette, augmente score -=-*
		move.b	#1,(sound+S_MONNAIE-t,a5)
		bsr	venus1
		bsr	venus1
		add.w	#20,(score.life-t,a5)

		*-=- détermine la valeur de la bourse -=-*
		lea	(jupiter10,pc),a1
		moveq	#10,d1
		cmp.w	#20,(CAP.VAL,a4)	bourse de 20 crédits ?
		beq.s	.cap_bas1		-> oui
		cmp.w	#30,(CAP.VAL,a4)	bourse de 30 crédits ?
		bne.s	.cap_cap		-> non
		lea	(jupiter30,pc),a1
		moveq	#30,d1
		bra.s	.cap_cap	
.cap_bas1	lea	(jupiter20,pc),a1
		moveq	#20,d1

		*-=- augmente le crédit du joueur -=-*
.cap_cap	add.w	d1,(credit-t,a5)
		jsr	(a1)
		cmp.w	#3,(area-t,a5)		area 3 ?
		blt.s	cap_god			-> non
		add.w	d1,(credit-t,a5)
		jsr	(a1)
		cmp.w	#5,(area-t,a5)		area 5 ?
		bne.s	cap_god			-> non
		jsr	(a1)
		add.w	d1,(credit-t,a5)

		* capsule sort de l'écran ou touche raquette  *
cap_god:	move.w	d6,(CAP.ACTIF,a4)	RAZ flag capsule actif
cap_fin		rts
