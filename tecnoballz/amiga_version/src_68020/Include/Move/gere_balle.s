
* NON OPERATIONNEL => UTILISE GERE_BALLE.OLD

*=========================================================================*
*-=-=-=-=-=  Gestion des balles, colle, rebond raquette, coins  =-=-=-=-=-*
*=========================================================================*
*	Entrée	=> a5.l	adr. table 't'
gere_balle:
		move.l	a6,-(a7)
		bsr	collision_robot
		bsr	speedball
		bsr	vitus
		bsr	dancer
		move.l	(a7)+,a6
		rts

*------------------* TEST SI LA BALLE SORT DE L'ECRAN *-------------------*
vitus:		lea	(b1-t,a5),a1
		lea	(b2-t,a5),a2
		lea	(b3-t,a5),a3
		lea	(b4-t,a5),a4
		move.l	(pt_balle-t,a5),a6
		move.w	(balle_nbre-t,a5),d2
		moveq	#XMIN,d3
		move.w	#XMAX,d4
		moveq	#YMIN,d5
		move.w	#YMAX,d6
		moveq	#NBALLE-1,d7
.loop:		tst.w	(a6)			Balle active ?
		beq.s	.next			-> Non
		move.l	(BALLE.X,a6),d0
		cmp.w	d5,d0			Y < YMIN ?
		ble.s	.haut			-> oui sortie en haut !
		sub.w	d6,d0			Y > YMAX ?
		bge.s	.bas			-> oui sortie en bas !
		swap	d0
		cmp.w	d3,d0			X < XMIN ?
		ble.s	.gauche			-> oui sortie gauche !
		sub.w	d4,d0
		blt.s	.next
		moveq	#2,d0
		move.l	a2,a0
		bra.s	.clear
.gauche		moveq	#4,d0
		move.l	a4,a0
		bra.s	.clear
.haut		move.l	a3,a0
		moveq	#3,d0
		bra.s	.clear
.bas		move.l	a1,a0
		moveq	#1,d0
.clear		clr.w	(a6)
		subq.w	#1,d2
		bmi	lastball
.next		lea	(BALLE.SIZE,a6),a6
		dbf	d7,.loop
		move.w	d2,(balle_nbre-t,a5)

*-------------*  Gestion collisions avec la raquette <1> *----------------*
vitus2:		lea	(sound+S_TOUCHE.RAK-t,a5),a4
		move.l	(pt_balle-t,a5),a6
		lea	(b1-t,a5),a0
		move.w	(BUMP.X,a0),d0
		move.w	d0,d2
		add.w	(rak.large-t,a5),d2
		move.w	(BUMP.Y,a0),d1
		move.w	d1,d3
		addq.w	#8,d3
		moveq	#NBALLE-1,d7

.loop:		tst.w	(a6)
		beq.s	.next
		move.l	(BALLE.X,a6),d4
		move.w	(BOB.WIDTH,a6),d5
		cmp.w	d3,d4
		bgt.s	.next
		add.w	d5,d4
		cmp.w	d1,d4
		blt.s	.next
		swap	d4
		cmp.w	d2,d4
		bgt.s	.next
		add.w	d5,d4
		sub.w	d0,d4
		blt.s	.next
		move.w	(BUMP.Y,a0),d4
		sub.w	d5,d4
		move.w	d4,(BALLE.Y,a6)
		move.w	#NLINE.BLITZ-1,(blitz.count-t,a5)
		move.b	#1,(a4)
		move.l	a0,(BALLE.RAK,a6)
		cmp.w	#1,(BUMP.GLUE,a0)
		bne.s	.noglue
		move.w	#2,(BUMP.GLUE,a0)
		move.l	a6,(BUMP.BALLE,a0)
		move.l	a0,(BALLE.RAKGLUE,a6)
		move.w	d4,(BALLE.GLUE,a6)
.noglue		clr.w	(BALLE.TILT,a6)
		move.l	(BUMP.REBOND,a0),a0
		add.l	(BALLE.DIRECT,a6),a0
		move.l	(a0),(BALLE.DIRECT,a6)
.next		lea	(BALLE.SIZE,a6),a6
		dbf	d7,.loop

*-------------*  Gestion collisions avec la raquette <2> *----------------*
vitus3:		lea	(b2-t,a5),a0
		tst.w	(BUMP.ACTIF,a0)
		beq	vitus4
		move.l	(pt_balle-t,a5),a6
		move.w	(BUMP.X,a0),d0
		move.w	d0,d2
		add.w	(rak.large-t,a5),d2
		move.w	(BUMP.Y,a0),d1
		move.w	d1,d3
		addq.w	#8,d3
		moveq	#NBALLE-1,d7
.loop:		tst.w	(a6)
		beq	.next
		move.l	(BALLE.X,a6),d4
		move.w	(BOB.WIDTH,a6),d5


		move.w	(BUMP.X,a1),d0
		move.w	(BUMP.Y,a1),d1
		move.w	(BALLE.Y,a3),d3
		move.w	(BOB.WIDTH,a3),d2
		lsr.w	#1,d2
		add.w	d2,d3
		move.w	(BALLE.X,a3),d2
		add.w	(BOB.WIDTH,a3),d2
		cmp.w	d0,d2
		blt	.re_bump3

		cmp.w	d1,d3
		blt	.re_bump3

		add.w	(rak.haut-t,a5),d0
		cmp.w	d0,d2
		bgt	.re_bump3

		add.w	(rak.large-t,a5),d1
		cmp.w	d1,d3
		bgt	.re_bump3

		move.w	(BUMP.X,a1),d0
		sub.w	(BOB.WIDTH,a3),d0
		move.w	d0,(BALLE.X,a3)


		move.w	(BUMP.X,a2),d4
		sub.w	(BOB.WIDTH,a6),d4
		move.w	d4,(BALLE.X,a6)
		moveq	#2,d4
		move.l	a2,a0
		bra.s	.ok
.ok		move.b	#1,(sound+S_TOUCHE.RAK-t,a5)
		move.l	a0,(BALLE.RAK,a6)
		cmp.w	#1,(BUMP.GLUE,a0)
		bne.s	.noglue
		move.l	a0,(BALLE.RAKGLUE,a6)
		move.w	#2,(BUMP.GLUE,a0)
		move.l	a6,(BUMP.BALLE,a0)
		move.w	d4,(BALLE.GLUE,a6)
.noglue		clr.w	(BALLE.TILT,a6)
		move.l	(BUMP.REBOND,a0),a0
		add.l	(BALLE.DIRECT,a6),a0
		move.l	(a0),(BALLE.DIRECT,a6)
.next		lea	(BALLE.SIZE,a6),a6
		dbf	d7,.loop

*-------------*  Gestion collisions avec la raquette <3> *----------------*
vitus4:		lea	(b3-t,a5),a0
		tst.w	(BUMP.ACTIF,a0)
		beq	vitus5
		move.w	(BUMP.X,a0),d0
		move.w	d0,d2
		add.w	(rak.large-t,a5),d2
		move.w	(BUMP.Y,a0),d1
		move.w	d1,d3
		addq.w	#8,d3
		move.l	(pt_balle-t,a5),a6
		moveq	#NBALLE-1,d7
.loop:		tst.w	(a6)
		beq	.next
.bump3		tst.w	(BUMP.ACTIF,a3)
		beq.s	.next
		move.l	(BALLE.X,a6),d4
		move.w	(BOB.WIDTH,a6),d5


		move.w	d0,d2
		add.w	d5,d2
		move.w	(BOB.WIDTH,a6),d5
		move.w	d1,d3
		add.w	d5,d3
		move.l	(BUMP.X,a3),d4
		cmp.w	d3,d4
		bhi.s	.next
		addq.w	#8,d4
		sub.w	d1,d4
		blt.s	.next
		swap	d4
		cmp.w	d2,d4
		bhi.s	.next
		add.w	d6,d4
		cmp.w	d0,d4
		blt.s	.next
		move.w	(BUMP.Y,a3),d4
		addq.w	#8,d4
		move.w	d4,(BALLE.Y,a6)
		moveq	#3,d4
		move.l	a3,a0

		move.b	#1,(a4)
		move.l	a0,(BALLE.RAK,a6)
		cmp.w	#1,(BUMP.GLUE,a0)
		bne.s	.noglue
		move.l	a0,(BALLE.RAKGLUE,a6)
		move.w	#2,(BUMP.GLUE,a0)
		move.l	a6,(BUMP.BALLE,a0)
		move.w	d4,(BALLE.GLUE,a6)
.noglue		clr.w	(BALLE.TILT,a6)
		move.l	(BUMP.REBOND,a0),a0
		add.l	(BALLE.DIRECT,a6),a0
		move.l	(a0),(BALLE.DIRECT,a6)
.next		lea	(BALLE.SIZE,a6),a6
		dbf	d7,.loop

*-------------*  Gestion collisions avec la raquette <4> *----------------*
vitus5:		lea	(b4-t,a5),a0
		tst.w	(BUMP.ACTIF,a0)
		beq	vitus6
		move.w	(BUMP.X,a0),d0
		move.w	d0,d2
		add.w	(rak.large-t,a5),d2
		move.w	(BUMP.Y,a0),d1
		move.w	d1,d3
		addq.w	#8,d3
		move.l	(pt_balle-t,a5),a6
		moveq	#NBALLE-1,d7
.loop:		tst.w	(a6)
		beq	.next
.bump4		tst.w	(BUMP.ACTIF,a4)
		beq	.next
		move.l	(BALLE.X,a6),d4
		move.w	(BOB.WIDTH,a6),d5

		move.b	#1,(a4)
		move.l	a0,(BALLE.RAK,a6)
		cmp.w	#1,(BUMP.GLUE,a0)
		bne.s	.noglue
		move.l	a0,(BALLE.RAKGLUE,a6)
		move.w	#2,(BUMP.GLUE,a0)
		move.l	a6,(BUMP.BALLE,a0)
		move.w	d4,(BALLE.GLUE,a6)
.noglue		clr.w	(BALLE.TILT,a6)
		move.l	(BUMP.REBOND,a0),a0
		add.l	(BALLE.DIRECT,a6),a0
		move.l	(a0),(BALLE.DIRECT,a6)
.next		lea	(BALLE.SIZE,a6),a6
		dbf	d7,.loop
		bra	vitus7

*--------------------* Gestion de la raquette colle *---------------------*
vitus6		lea	(cool_list-t,a5),a4	Table position pointeur
		move.l	(pt_balle-t,a5),a6
		move.w	(rak.large-t,a5),d6
		lsr.w	#1,d6
		moveq	#NBALLE-1,d7
.loop:		tst.w	(a6)
		beq	.next
		lea	(BALLE.GLUE,a6),a2
		move.w	(a2)+,d0
		jmp	([.list,pc,d0*4])
.rak1		lea	(b1-t,a5),a1
		move.w	(BOB.WIDTH,a6),d2
		move.w	d6,d0
		lsr.w	#1,d2
		addq.w	#1,d2
		sub.w	d2,d0
		swap	d0
		move.w	#-1,d0
		sub.w	(BOB.WIDTH,a6),d0
		lea	(2+pets1-t,a5),a0
		bra.s	.go
.rak2		lea	(b2-t,a5),a1
		move.w	(BOB.WIDTH,a6),d2
		move.w	#-1,d0
		sub.w	d2,d0
		swap	d0
		move.w	d6,d0
		lsr.w	#1,d2
		addq.w	#1,d2
		sub.w	d2,d0
		lea	(2+pets2-t,a5),a0
		bra.s	.go
.rak3		lea	(b3-t,a5),a1
		move.w	(BOB.WIDTH,a6),d2
		move.w	d6,d0
		lsr.w	#1,d2
		addq.w	#1,d2
		sub.w	d2,d0
		swap	d0
		move.w	#10,d0
		lea	(2+pets3-t,a5),a0
		bra.s	.go
.rak4		lea	(b4-t,a5),a1
		move.w	(BOB.WIDTH,a6),d2
		move.w	#10,d0
		swap	d0
		move.w	d6,d0
		lsr.w	#1,d2
		addq.w	#1,d2
		sub.w	d2,d0
		lea	(2+pets4-t,a5),a0
.go		add.l	(BUMP.X,a1),d0
		move.l	d0,(BALLE.X,a6)
		subq.w	#1,(a2)			BALLE.PBTEMP
		bne.s	.dec
		move.w	#5,(a2)+		BALLE.PBTEMP
		move.l	(a2),d2			BALLE.PBROTA
		addq.w	#4,d2
		cmp.w	#56,d2
		blt.s	.nomax
		moveq	#0,d2
.nomax		move.l	d2,(a2)+		BALLE.PBROTA
		add.l	d2,a0
		move.l	d0,(a2)			BALLE.PBX
		move.w	(a0),d0
		move.w	d0,(2+BALLE.DIRECT,a6)
		move.w	(2,a4,d0*2),d1
		add.w	d1,(a2)+
		move.w	(6,a4,d0*2),d1
		add.w	d1,(a2)
.dec		subq.w	#1,(BALLE.START,a6)
		bne.s	.next
		clr.w	(BALLE.GLUE,a6)
		move.w	(balle_glue-t,a6),(BALLE.START,a6)
		tst.w	(BUMP.GLUE,a0)
		beq.s	.run
		move.w	#1,(BUMP.GLUE,a0)
.run:		move.l	(BALLE.SPEED,a6),a0
		move.w	(2+BALLE.DIRECT,a6),d0
		add.w	d0,a0
		move.l	(a0),d0
		add.l	d0,(BALLE.X,a6)
.next		lea	(BALLE.SIZE,a6),a6
		dbf	d7,.loop
		bra.s	vitus7

		CNOP	0,4	
.list		dc.l	.run
		dc.l	.rak1
		dc.l	.rak2
		dc.l	.rak3
		dc.l	.rak4

*------------------------* Gestion des éjecteurs *------------------------*
vitus7:		move.l	(pt_balle-t,a5),a6
		moveq	#NBALLE-1,d7
.loop:		tst.w	(a6)
		beq	.next

.go_coin1:	move.w	(BALLE.X,a6),d1
		move.w	(BALLE.Y,a6),d2
		lea	(eject1-t,a5),a0
		lea	(BALLE.EJECT1,a6),a1
		tst.w	(a1)
		bgt	.ok_coin
		cmp.w	#EJ1Y-05,d2
		blt.s	.out1
		cmp.w	#EJ1Y+00,d2
		bgt.s	.out1
		cmp.w	#EJ1X+19,d1
		bgt.s	.out1
		cmp.w	#EJ1X-05,d1
		bgt.s	.no_out1
.out1		cmp.w	#EJ1X-05,d1
		blt.s	.go_coin2
		cmp.w	#EJ1X+00,d1
		bgt.s	.go_coin2
		cmp.w	#EJ1Y-05,d2
		blt.s	.go_coin2
		cmp.w	#EJ1Y+19,d2
		bgt.s	.go_coin2
.no_out1	move.w	(BOB.WIDTH,a6),d1
		lsr.w	#1,d1
		move.w	#EJ1X+10,d0
		sub.w	d1,d0
		move.w	d0,(BALLE.X,a6)
		move.w	#EJ1Y+10,d0
		sub.w	d1,d0
		move.w	d0,(BALLE.Y,a6)
		bra	.vive_loriciel

*»-----------*» Test si balle va dans le coin >bas gauche< «*------------«*
.go_coin2	lea	(eject2-t,a5),a0
		lea	(BALLE.EJECT2,a6),a1
		tst.w	(a1)
		bgt	.ok_coin
		cmp	#EJ3Y-06,d2
		blt.s	.out2
		cmp	#EJ3Y+30,d2
		bgt.s	.out2
		cmp	#EJ3X+00,d1
		bgt.s	.out2
		cmp	#EJ3X-06,d1
		bgt.s	.no_out2
.out2		cmp	#EJ3Y+16,d2
		blt.s	.go_coin3
		cmp	#EJ3Y+24,d2
		bgt.s	.go_coin3
		cmp	#EJ3X+30,d1
		bgt.s	.go_coin3
		cmp	#EJ3X-06,d1
		blt.s	.go_coin3
.no_out2	move.w	(BOB.WIDTH,a6),d1
		lsr.w	#1,d1
		move.w	#EJ3X+10,d0
		sub.w	d1,d0
		move.w	d0,(BALLE.X,a6)
		move.w	#EJ3Y+5,d0
		sub.w	d1,d0
		move.w	d0,(BALLE.Y,a6)
		bra	.vive_loriciel

*»------------*» Test si balle va dans le coin >bas-droite «*------------«*
.go_coin3	lea	(eject3-t,a5),a0
		lea	(BALLE.EJECT3,a6),a1
		tst	(a1)
		bgt	.ok_coin
		cmp.w	#EJ4Y-06,d2
		blt.s	.out3
		cmp.w	#EJ4Y+20,d2
		bgt.s	.out3
		cmp.w	#EJ4X+16,d1
		blt.s	.out3
		cmp.w	#EJ4X+24,d1
		blt.s	.no_out3
.out3		cmp.w	#EJ4X-06,d1
		blt.s	.go_coin4
		cmp.w	#EJ4X+24,d1
		bgt.s	.go_coin4
		cmp.w	#EJ4Y+24,d2
		bgt.s	.go_coin4
		cmp.w	#EJ4Y+16,d2
		blt.s	.go_coin4
.no_out3	move.w	(BOB.WIDTH,a6),d1
		lsr.w	#1,d1
		move.w	#EJ4X+5,d0
		sub.w	d1,d0
		move.w	d0,(BALLE.X,a6)
		move.w	#EJ4Y+5,d0
		sub.w	d1,d0
		move.w	d0,(BALLE.Y,a6)
.vive_loriciel:	move.w	#1,(a1)
		move.l	#64,(BALLE.DIRECT,a6)
		bsr	venus1
		add.w	#10,(score.life-t,a5)
		move.b	#1,(sound+S_COIN.ASPIRE-t,a5)
		bra	.next

*»-----------*» Test si balle va dans le coin >haut-droite< «*-----------«*
.go_coin4	lea	(eject4-t,a5),a0
		lea	(BALLE.EJECT4,a6),a1
		tst.w	(a1)			balle dèjà dans coin >haut-droite< ?
		bgt	.ok_coin		-> oui
		cmp.w	#EJ2Y-06,d2
		blt.s	.out4
		cmp.w	#EJ2Y+00,d2
		bgt.s	.out4
		cmp.w	#EJ2X+24,d1
		bgt.s	.out4
		cmp.w	#EJ2X-06,d1
		bgt.s	.no_out4
.out4		cmp.w	#EJ2X+16,d1
		blt.s	.hout4
		cmp.w	#EJ2X+24,d1
		bgt.s	.hout4
		cmp.w	#EJ2Y-06,d2
		blt.s	.hout4
		cmp.w	#EJ2Y+24,d2
		blt.s	.no_out4
.hout4		bra	.next
.no_out4	move.w	(BOB.WIDTH,a6),d1
		lsr.w	#1,d1
		move.w	#EJ2X+5,d0
		sub.w	d1,d0
		move.w	d0,(BALLE.X,a6)		229-2=227
		move.w	#EJ2Y+10,d0
		sub.w	d1,d0
		move.w	d0,(BALLE.Y,a6)		026-2=024
		bra	.vive_loriciel

		** gère balle : balle dèjà dans un coin **
		* a0.l		pt/table direction ejection 'eject1' à 'eject4'
		* a1.l	BALLE.EJECT1	balle coin >haut-gauche<
		* 	BALLE.EJECT2	balle coin >bas-gauche<
		* 	BALLE.EJECT3	balle coin >bas-droite<
		* 	BALLE.EJECT4	balle coin >haut-droite<
		* a6.l		adr. structure balle
		* a5.l		adr. table 't'
.ok_coin:	moveq	#0,d0
		addq.w	#1,(a1)			inc. compteur/flag
		cmp.w	#50,(a1)		balle restée une seconde ?
		bne.s	.next			-> non, sort de la routine
		move.w	d0,(a1)			raz flag balle dans coin
		moveq	#0,d0
		move.w	(hasard-t,a5),d0	lit compteur "hasard"
		and.w	#$F,d0
		add.w	d0,d0
		add.w	d0,d0
		add.w	d0,a0
		move.l	(a0),(BALLE.DIRECT,a6)	nouveau pt direction balle
		move.b	#1,(sound+S_COIN.EJECT-t,a5)

.next		lea	(BALLE.SIZE,a6),a6
		dbf	d7,.loop
		rts

*-------------* Perte de la dernère balle / perte d'une vie *-------------*
* Entrée	=> d0.w 1/2/3/4
*		=> a0.l	Structure raquette
*		=> a1.l	Structure raquette 1
*		=> a6.l Structure balle
lastball:
		tst.w	(BUMP.ACTIF,a0)
		bne.s	.ok
		moveq	#1,d0
		move.l	a1,a0
.ok		clr.w	(balle_nbre-t,a5)
		move.w	#1,(a6)
		move.w	d0,(BALLE.GLUE,a6)
		move.l	a0,(BALLE.RAK,a6)
		move.w	#2,(BALLE.PT,a6)
		move.w	#5,(BOB.WIDTH,a6)
		move.w	(balle_start-t,a5),(BALLE.START,a6)
		move.l	(balle_speed-t,a5),(BALLE.SPEED,a6)
		move.w	(balle_scount-t,a5),(BALLE.SCOUNT,a6)
		move.l	(pt_atom-t,a5),a0
		move.w	(atom.reapp-t,a5),d0
		move.w	(atom.resist-t,a5),d1
		moveq	#NATOM-1,d2
.loop		tst.w	(ATOM.ACTIF,a0)
		bne.s	.next
		clr.w	(BOB.PTANIM,a0)
		move.w	d0,(ATOM.ACTIF,a0)
		move.w	d1,(ATOM.POWER,a0)
		move.w	#1,(ATOM.EXPLO,a0)
		move.w	(ATOM.OEXP,a0),(ATOM.OFF,a0)
.next		lea	(ATOM.SIZE,a0),a0
		dbf	d2,.loop
		tst.w	(tableau.end-t,a5)	Tableau terminé ?
		bne.s	.exit			-> Oui
		clr.w	(music4voix-t,a5)	0=musique 4 voix
		move.l	(pt_poz-t,a5),a0	Adresse table des positions
		move.w	(6,a0),d0		Position musique "lost"
		bsr.l	np_change
		move.w	#1,(musiclost-t,a5)	Flag musique "lost" en cours
		GETMESSAGE	10
		clr.w	(rak.inver-t,a5)	RAZ flag inverseur
		bsr	lune			Décrèmente compteur vie(s)
.exit		rts

*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*
*                    Augmente les vitesses des balles			  *
*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*
* Entrée	=> a5.l	adr. 't' 
speedball:
		lea	(speed1-t,a5),a1
		lea	(speed2-t,a5),a2
		lea	(speed3-t,a5),a3
		move.l	(pt_balle-t,a5),a4	Première balle
		move.w	(balle_scount-t,a5),d0	Temps que dure cette vitesse
		moveq	#NBALLE-1,d7
.loop		cmp.l	(BALLE.SPEED,a4),a3	Dernière vitesse ?
		beq.s	.bobine			-> Oui
		subq.w	#1,(BALLE.SCOUNT,a4)	Dec. compt. temps
		bne.s	.bobine			-> pas encore à 0
		move.w	d0,(BALLE.SCOUNT,a4)	Réinit. compt. temps
		cmp.l	(BALLE.SPEED,a4),a2	2ieme vitesse ?
		beq.s	.turbine		-> oui
		move.l	a2,(BALLE.SPEED,a4)	Pointe table 2ieme vitesse
		bra.s	.bobine
.turbine	move.l	a3,(BALLE.SPEED,a4)	Pointe table 3ieme vitesse
.bobine		lea	(BALLE.SIZE,a4),a4
		dbf	d7,.loop
		rts

*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*
*             Gère les compteurs tilt (baillement + sonnerie)		  *
*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*>*<*
*	Entrée	=> a5.l	adr. table 't'
dancer:		
		move.w	(balle_tilt-t,a5),d0	Temps avant tilt
		moveq	#1,d1
		moveq	#TETE_BAILLE1,d2
		lea	(head.req-t,a5),a0
		lea	(sound+S_TILT.ALARM-t,a5),a1
		move.l	(pt_balle-t,a5),a4
		moveq	#NBALLE-1,d7
.loop		tst.w	(BALLE.ACTIF,a4)	balle active ?
		beq.s	.no_tilt		-> non
		tst.w	(BALLE.GLUE,a4)		balle collée ?
		bne.s	.no_tilt2		-> oui
		cmp.w	(BALLE.TILT,a4),d0	autorise tilt ?
		blt.s	.no_tilt2		-> tilt dèjà autorisé
		bne.s	.no_tilt		-> non
		move.b	d1,(a1)			demande son alarm
		move.w	d2,(a0)			animation tête : baillement
.no_tilt	addq.w	#1,(BALLE.TILT,a4)
.no_tilt2	lea	(BALLE.SIZE,a4),a4
		dbf	d7,.loop
		rts

*---------------------> Collision Balles & le Robot <---------------------*
collision_robot:
		tst.w	(robot-t,a5)
		beq	.exit
		lea	(sound+S_TOUCHE.RAK-t,a5),a2
		move.l	(pt_balle-t,a5),a3
		lea	(br-t,a5),a4
		move.w	(BUMP.X,a4),d0		pos. x raqu. haut
		move.w	d0,d2
		add.w	#64,d2
		move.w	(BUMP.Y,a4),d1		pos. y raqu. gauche
		move.w	d1,d3
		addq.w	#8,d3
		move.l	(BUMP.REBOND,a4),d6
		moveq	#NBALLE-1,d7
.loop		tst.w	(a3)
		beq.s	.next
		move.l	(BALLE.X,a3),d4
		move.w	(BOB.WIDTH,a3),d5
		cmp.w	d3,d4			BY > RY+8
		bgt.s	.next
		add.w	d5,d4
		cmp.w	d1,d4			BY+W < RY
		blt.s	.next
		swap	d4
		cmp.w	d2,d4			BX > RX+64
		bgt.s	.next
		add.w	d5,d4
		sub.w	d0,d4			BX+W < RX
		blt.s	.next
		swap	d4
		sub.w	d5,d4
		sub.w	d5,d4
		move.w	d4,(BALLE.Y,a3)
		move.b	#1,(a2)
		move.l	d6,a0
		lea	(2+BALLE.DIRECT,a3),a1
		move.w	(a1),d4
		add.w	d4,a0
		move.w	(a0),(a1)
.next		lea	(BALLE.SIZE,a3),a3
		dbf	d7,.loop
.exit		rts
