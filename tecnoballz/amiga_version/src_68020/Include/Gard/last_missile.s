*=========================================================================*
*               Déplacement & collisions tirs des gardiens		  *
*=========================================================================*
* Entrée	=> a5.l	adr. table 't'
last_missile:
		move.l	(missile-t,a5),a4
		moveq	#NBR_MISSILE-1,d7
		lea	(dep_missile,pc),a0
		moveq	#2,d1
tu_es_belle	move.w	(MIS.ACTIF,a4),d2	Numero du tir 0=SBOB Inactif
		jmp	([a0,d2])
		CNOP	0,4
dep_missile	dc.l	betty_boo4	00
		dc.l	dep_missile01	04
		dc.l	dep_missile02	08
		dc.l	dep_missile03	12
		dc.l	dep_missile04	16
		dc.l	dep_missile05	20
		dc.l	dep_missile06	24
		dc.l	dep_missile07	28
		dc.l	dep_missile08	32
		dc.l	dep_missile09	40
		dc.l	dep_missile10	44
		dc.l	dep_missile11	48
		dc.l	dep_missile12	52
dep_missilex
*-------------------------------------------------------------------------*
*                         TIR >12< : triangle II			  *
*-------------------------------------------------------------------------*
dep_missile12:
		*- augmente le pt dans la table sinus/cosinus 0 à 718 *
		lea	(tabsin-t,a5),a1
		lea	(tabcos-t,a5),a2
		move.w	(MIS.FD3,a4),d2		lit valeur de l'angle
		addq.w	#8,d2			augmente la valeur de l'angle
		cmp.w	#360*2,d2
		blt.s	.dangerous4
		sub.w	#360*2,d2
.dangerous4	move.w	d2,(MIS.FD3,a4)

		*- lit le rayon -*
		moveq	#8,d3			rayon en x
		move.w	d3,d4

		*- calcul pos. (x,y) du tir -*
		muls	(a1,d2),d3		sin(a)*rayon
		muls	(a2,d2),d4		cos(a)*rayon
		asr.l	#7,d3
		asr.l	#7,d4
		add.w	(MIS.FD1,a4),d3		centre x
		add.w	(MIS.FD2,a4),d4		centre y
		move.w	d3,(MIS.X,a4)
		move.w	d4,(MIS.Y,a4)
		move.w	(MIS.FD4,a4),d4
		add.w	d4,(MIS.FD1,a4)
		move.w	(MIS.FD5,a4),d4
		add.w	d4,(MIS.FD2,a4)
		bra	betty_boo

*-------------------------------------------------------------------------*
*                         TIR  >11< : triangle I			  *
*-------------------------------------------------------------------------*
dep_missile11:
		lea	(tabsin-t,a5),a1	adr. table sinus
		lea	(tabcos-t,a5),a2	adr. table cosinus

		*- augmente le pt dans la table sinus/cosinus 0 à 718 -*
		move.w	(MIS.FD3,a4),d2		lit valeur de l'angle
		addq.w	#8,d2			augmente la valeur de l'angle
		cmp.w	#360*2,d2
		blt.s	.dangerous3
		sub.w	#360*2,d2
.dangerous3	move.w	d2,(MIS.FD3,a4)

		*- lit le rayon -*
		moveq	#8,d3			rayon en x
		move.w	d3,d4

		*- calcul pos. (x,y) du tir -*
		muls	(a1,d2),d3		sin(a)*rayon
		muls	(a2,d2),d4		cos(a)*rayon
		asr.l	#7,d3
		asr.l	#7,d4
		add.w	(MIS.FD1,a4),d3		centre x
		add.w	(MIS.FD2,a4),d4		centre y
		move.w	d3,(MIS.X,a4)		pos. x
		move.w	d4,(MIS.Y,a4)		pos. y
		addq.w	#3,(MIS.FD2,a4)
		bra	betty_boo

*-------------------------------------------------------------------------*
*                       TIR >10< : cercle eliptique			  *
*-------------------------------------------------------------------------*
dep_missile10:
		*- augmente le pt dans la table sinus/cosinus 0 à 718 -*
		lea	(tabsin-t,a5),a1
		lea	(tabcos-t,a5),a2
		move.w	(MIS.FD3,a4),d2		lit valeur de l'angle
		add.w	#12,d2			augmente la valeur de l'angle
		cmp.w	#360*2,d2
		blt.s	.dangerous2
		sub.w	#360*2,d2
.dangerous2	move.w	d2,(MIS.FD3,a4)

		* lit le rayon *
		move.w	(MIS.FD4,a4),d3		rayon en x
		move.w	d3,d4
		subq.w	#3,d4
		subq.w	#1,(MIS.FD5,a4)
		tst.w	(MIS.FD5,a4)
		bne.s	.lion_1
		move.w	#3,(MIS.FD5,a4)
		addq.w	#1,(MIS.FD4,a4)

		*- calcul pos. (x,y) du tir -*
.lion_1		muls	(a1,d2),d3		sin(a)*rayon
		muls	(a2,d2),d4		cos(a)*rayon
		asr.l	#7,d3
		asr.l	#7,d4
		add.w	(MIS.FD1,a4),d3		centre x
		add.w	(MIS.FD2,a4),d4		centre y
		move.w	d3,(MIS.X,a4)
		move.w	d4,(MIS.Y,a4)
		addq.w	#3,(MIS.FD2,a4)
		bra	betty_boo		-> test dépassement

*-------------------------------------------------------------------------*
*            TIR :  >09< : cercle rebondissant (tir ventouse)		  *
*-------------------------------------------------------------------------*
dep_missile09:
		*- augmente le pt dans la table sinus/cosinus 0 à 718 -*
		lea	(tabsin-t,a5),a1
		lea	(tabcos-t,a5),a2
		move.w	(MIS.FD3,a4),d2		lit valeur de l'angle
		addq.w	#8,d2			augmente la valeur de l'angle
		cmp.w	#360*2,d2
		blt.s	.dangerous1
		moveq	#0,d2
.dangerous1	move.w	d2,(MIS.FD3,a4)


		*- calcul pos. (x,y) du tir -*
		moveq	#16,d3			rayon en x
		move	d3,d4			rayon en y
		muls	(a1,d2),d3		sin(a)*rayon
		muls	(a2,d2),d4		cos(a)*rayon
		asr.l	#7,d3
		asr.l	#7,d4
		add.w	(MIS.FD1,a4),d3		centre x
		add.w	(MIS.FD2,a4),d4		centre y
		move.w	d3,(MIS.X,a4)
		move.w	d4,(MIS.Y,a4)
		addq.w	#1,(MIS.FD2,a4)
		tst.w	(MIS.FD4,a4)
		beq.s	.la_poste
		addq.w	#5,(MIS.FD1,a4)
		cmp.w	#220+64,(MIS.FD1,a4)
		ble	baby_boo
		clr.w	(MIS.FD4,a4)
		bra	baby_boo
.la_poste	subq.w	#5,(MIS.FD1,a4)
		cmp.w	#-80,(MIS.FD1,a4)
		bge	baby_boo
		move.w	#1,(MIS.FD4,a4)
		bra	baby_boo

*-------------------------------------------------------------------------*
*          TIR : tir >08< : cercle grandissant (cercle infernal)	  *
*-------------------------------------------------------------------------*
dep_missile08:
		* augmente le pt dans la table sinus/cosinus 0 à 718 *
		lea	(tabsin-t,a5),a1
		lea	(tabcos-t,a5),a2
		move.w	(MIS.FD3,a4),d2		lit valeur de l'angle
		addq.w	#4,d2			augmente la valeur de l'angle
		cmp.w	#360*2,d2
		blt.s	.dangerous
		moveq	#0,d2
.dangerous	move.w	d2,(MIS.FD3,a4)

		*- calcul pos. (x,y) du tir -*
		move.w	(MIS.FD4,a4),d3		rayon en x
		move.w	d3,d4			rayon en y
		muls	(a1,d2),d3		sin(a)*rayon
		muls	(a2,d2),d4		cos(a)*rayon
		asr.l	#7,d3
		asr.l	#7,d4
		move.l	(MIS.FD1,a4),a1		adr. du gardien tireur
		add.w	(GARD.X,a1),d3		+centre du cercle x
		add.w	(GARD.XFIRE,a1),d3
		add.w	(GARD.Y,a1),d4		+centre du cercle y
		add.w	(GARD.YFIRE,a1),d4
		move.w	d3,(MIS.X,a4)
		move.w	d4,(MIS.Y,a4)
		addq.w	#1,(MIS.FD4,a4)
		bra	betty_boo

*-------------------------------------------------------------------------*
*                       TIR : déplacement tir >07<			  *
*-------------------------------------------------------------------------*
dep_missile07:
		addq.w	#3,(MIS.Y,a4)
		cmp.w	#150,(MIS.Y,a4)
		bge	betty_boo
		lea	(b1-t,a5),a2
		move.w	(BUMP.X,a2),d2
		addq.w	#2,(MIS.X,a4)
		cmp.w	(MIS.X,a4),d2
		bge.w	betty_boo
		subq.w	#4,(MIS.X,a4)
		bra	betty_boo

*-------------------------------------------------------------------------*
*                       TIR : déplacement tir >06<			  *
*-------------------------------------------------------------------------*
dep_missile06:
		move.l	(MIS.FD4,a4),a1		table sinus x bob
		addq.w	#4,(MIS.FD3,a4)		pointeur table sinus
		move.w	(MIS.FD3,a4),d4
		cmp.w	#99,(a1,d4)		fin table sinus ?
		bne.s	.dep_missile06a		-> non
		moveq	#0,d4
		move.w	d4,(MIS.FD3,a4)		reset pointeur
.dep_missile06a	addq.w	#3,(MIS.FD2,a4)		nouvelle position y
		move.w	(MIS.FD2,a4),d2
		add.w	(2,a1,d4),d2		corrige y
		move.w	d2,(MIS.Y,a4)
		cmp.w	#150,(MIS.FD2,a4)
		bge.s	.dep_missile6b
		lea	(b1-t,a5),a2		adresse raquette
		move.w	(BUMP.X,a2),d2		position x raquette
		addq.w	#2,(MIS.FD1,a4)
		cmp.w	(MIS.FD1,a4),d2
		bge.s	.dep_missile6b
		subq.w	#4,(MIS.FD1,a4)
.dep_missile6b	move.w	(MIS.FD1,a4),d2		x initial
		add.w	(a1,d4),d2		corrige x
		move.w	d2,(MIS.X,a4)
		bra	betty_boo

*-------------------------------------------------------------------------*
*                       TIR : déplacement tir >05<			  *
*-------------------------------------------------------------------------*
dep_missile05:
		tst.w	(MIS.FD1,a4)
		beq.s	.dep_missile05a
		subq.w	#1,(MIS.FD1,a4)
		addq.w	#1,(MIS.Y,a4)		nouvelle position y
		bra	betty_boo
.dep_missile05a	move.w	(MIS.FD2,a4),d2
		add.w	d2,(MIS.X,a4)		nouvelle position x
		move.w	(MIS.FD3,a4),d2
		add.w	d2,(MIS.Y,a4)		nouvelle position x
		bra	betty_boo		-> test dépassement

*-------------------------------------------------------------------------*
*                       TIR : déplacement tir >04<			  *
*-------------------------------------------------------------------------*
dep_missile04:
		tst.w	(MIS.FD5,a4)
		beq.s	.dep_missile042
		subq.w	#1,(MIS.FD4,a4)
		bne	betty_boo
		move.w	(MIS.FD5,a4),(MIS.FD4,a4)
		subq.w	#1,(MIS.FD5,a4)
		lea	(sinus-t,a5),a1		table sinus x bob
		addq.w	#4,(MIS.FD3,a4)		pointeur table sinus
		move.w	(MIS.FD3,a4),d4
		cmp.w	#99,(a1,d4)		fin table sinus ?
		bne.s	.dep_missile04b		-> non
		moveq	#0,d4
		move.w	d4,(MIS.FD3,a4)		reset pointeur
.dep_missile04b	move.w	(MIS.FD2,a4),d2
		add.w	(2,a1,d4),d2		corrige y
		move.w	d2,(MIS.Y,a4)		y final
		move.w	(MIS.FD1,a4),d2		x initial
		add.w	(a1,d4),d2		corrige x
		move.w	d2,(MIS.X,a4)		x final
		bra	betty_boo
.dep_missile042	move.w	(MIS.FD6,a4),d2
		sub.w	d2,(MIS.X,a4)
		move.w	(MIS.FD7,a4),d2
		add.w	d2,(MIS.Y,a4)
		bra	betty_boo

*-------------------------------------------------------------------------*
*                       Tir : déplacement tir >03<			  *
*-------------------------------------------------------------------------*
dep_missile03	
		addq.w	#3,(MIS.Y,a4)		nouvelle position y
		bra	betty_boo		-> test dépassement

*-------------------------------------------------------------------------*
*                       TIR : déplacement tir >02<			  *
*-------------------------------------------------------------------------*
dep_missile02:
		move.l	(MIS.FD4,a4),a1		table sinus x bob
		addq.w	#4,(MIS.FD3,a4)		pointeur table sinus
		move.w	(MIS.FD3,a4),d4
		cmp.w	#99,(a1,d4)		fin table sinus ?
		bne.s	.dep_missile02a		-> non
		moveq	#0,d4
		move.w	d4,(MIS.FD3,a4)		reset pointeur
.dep_missile02a	addq.w	#3,(MIS.FD2,a4)		nouvelle position y
		move.w	(MIS.FD2,a4),d2
		add.w	(2,a1,d4),d2		corrige y
		move.w	d2,(MIS.Y,a4)
		move.w	(MIS.FD1,a4),d2		x initial
		add.w	(a1,d4),d2		corrige x
		move.w	d2,(MIS.X,a4)
		bra	betty_boo

*-------------------------------------------------------------------------*
*                       TIR : déplacement tir >01<			  *
*-------------------------------------------------------------------------*
dep_missile01:
		moveq	#4,d2			increment y
		add.w	d2,(MIS.Y,a4)		nouvelle position y
		lea	(tir01_pos-t,a5),a1	table sinus position x
		addq.w	#4,(MIS.FD1,a4)		pointeur table sinus
		move.w	(MIS.FD1,a4),d2
		move.w	(a1,d2),d2
		cmp.w	#99,d2			fin table sinus ?
		bne.s	.dep_missile01a		-> non
		clr.w	(MIS.FD1,a4)
		moveq	#32,d2			valeur sinus
.dep_missile01a	move.w	(MIS.FD2,a4),d3
		add.w	d2,d3
		move.w	d3,(MIS.X,a4)

*-------------------------------------------------------------------------*
*                     TIR : test dépassement des tirs			  *
*-------------------------------------------------------------------------*
betty_boo:
		* Dépassement à droite *
		moveq	#0,d0
		cmp.w	#TIR_MAXI_X,(MIS.X,a4)	dépasse à droite ?
		ble.s	.betty_boo0		-> non
		clr.w	(MIS.ACTIF,a4)
		bra	betty_boo3		-> fin boucle
		* Dépassement à gauche *
.betty_boo0	cmp.w	(MIS.X,a4),d1		dépasse à gauche ?
		ble.s	betty_boo1		-> non
		clr.w	(MIS.ACTIF,a4)		désactive le bob
		bra.s	betty_boo3		-> fin boucle
		* Dépassement en haut *
betty_boo1	cmp.w	(MIS.Y,a4),d1		Dépasse en haut ?
		ble.s	.betty_boo2		-> non
		clr.w	(MIS.ACTIF,a4)		Désactive le bob
		bra.s	betty_boo3		-> fin boucle
		* Dépassement en bas *
.betty_boo2	cmp.w	#TIR_MAXI_Y,(MIS.Y,a4)	dépasse en bas ?
		ble.s	betty_boo3		-> non
		clr.w	(MIS.ACTIF,a4)		désactive le bob
		bra.s	betty_boo3		-> fin boucle

		*-=- test rebond en x -=-*
baby_boo	moveq	#0,d0
		cmp.w	#214,(MIS.X,a4)		depasse a gauche ?
		ble.s	.baby_boo0		---> non
		move.w	#214,(MIS.X,a4)
		bra.s	betty_boo1		---> quitte la routine
.baby_boo0	cmp.w	(MIS.X,a4),d1		depasse a droite ?
		ble.s	.baby_boo1		---> non
		move.w	d1,(MIS.X,a4)

		*-=- test rebond en y -=-*
.baby_boo1	cmp.w	(MIS.Y,a4),d1		depasse en haut ?
		ble.s	.baby_boo2		---> non
		move.w	d1,(MIS.Y,a4)
		bra.s	betty_boo3		---> quitte la routine
.baby_boo2	cmp	#240,(MIS.Y,a4)		depasse en bas ?
		ble.s	betty_boo3		---> non
		clr.w	(MIS.ACTIF,a4)		desactive le bob

		*-=- test si collision avec la raquette port 1 -=-*
betty_boo3	cmp.w	#220,(MIS.Y,a4)
		ble	betty_boo4
		lea	(b1-t,a5),a1		adr. structure raquette
		move.w	(BUMP.X,a1),d4
		move.w	(BUMP.Y,a1),d5
		move.w	(MIS.X,a4),d2
		move.w	(MIS.Y,a4),d3
		addq.w	#5,d2
		addq.w	#5,d3
		cmp.w	d4,d2
		blt	.betty_boo31
		cmp.w	d5,d3
		blt	.betty_boo31
		add.w	(rak.large-t,a5),d4
		move.w	(MIS.X,a4),d2
		cmp.w	d4,d2
		bgt	.betty_boo31
		move.w	(BUMP.Y,a1),d5
		add.w	#9,d5
		move.w	(MIS.Y,a4),d3
		cmp.w	d5,d3
		bgt.s	.betty_boo31

		*-=- il y a eu colision -=-*
		clr.w	(MIS.ACTIF,a4)
		tst.w	(BUMP.TOUCH,a1)		raquette invincible ?
		bne.s	.betty_boo31		-> oui
		move.b	#1,(sound+S_RAK.EXPLO-t,a5)
		bsr	lune			supprime une vie
		lea	(b4-t,a5),a3
		move.w	#100,(BUMP.TOUCH,a3)
		move.w	#100,(BUMP.TOUCH,a1)
		move.l	(explomon-t,a5),a3
		moveq	#NBR_EXPLOMON-1,d0
.elegy_l1	tst.w	(SBOB.ACTIF,a3)		SBOB déjà actif ?
		bne.s	.elegy_1		-> oui, passe au suivant
		move.w	(BUMP.X,a1),d0
		addq.w	#8,d0
		move.w	d0,(SBOB.POSX,a3)
		move.w	(BUMP.Y,a1),d0
		subq.w	#8,d0
		move.w	d0,(SBOB.POSY,a3)	+ pos. y du gardien
		move.w	#1,(SBOB.ACTIF,a3)	active le bob
		clr.w	(SBOB.PTANIM,a3)	raz pointeur gfx animation
		move.w	(SBOB.INITTEMPO,a3),(SBOB.TEMPO,a3)
		move.l	(SBOB.TABLE,a3),a2
		move.l	(a2)+,(SBOB.GFX,a3)
		move.l	(a2),(SBOB.MSK,a3)
		bra.s	.betty_boo31
.elegy_1	lea	(SBOB.SIZE,a3),a3
		dbf	d0,.elegy_l1

		*-=- test si colision avec la raquette port 2 -=-*
.betty_boo31	tst.w	(rak.team-t,a5)		mode team ?
		beq	betty_boo4		-> non
		lea	(b4-t,a5),a1		adr. structure raquette
		move.w	(BUMP.X,a1),d4
		move.w	(BUMP.Y,a1),d5
		move.w	(MIS.X,a4),d2
		move.w	(MIS.Y,a4),d3
		addq.w	#5,d2
		addq.w	#5,d3
		cmp.w	d4,d2
		blt	betty_boo4
		cmp.w	d5,d3
		blt	betty_boo4
		add.w	(rak.large-t,a5),d4
		move.w	(MIS.X,a4),d2
		cmp.w	d4,d2
		bgt	betty_boo4
		move.w	(BUMP.Y,a1),d5
		add.w	#9,d5
		cmp.w	d5,d3
		bgt.s	betty_boo4

		*-=- il y a eu colision -=-*
		moveq	#0,d0
		clr.w	(MIS.ACTIF,a4)
		tst.w	(BUMP.TOUCH,a1)		raquette invincible ?
		bne.s	betty_boo4		-> oui
		move.b	#1,(sound+S_RAK.EXPLO-t,a5)
		bsr	lune			supprime une vie
		lea	(b1-t,a5),a3
		move.w	#100,(BUMP.TOUCH,a3)
		move.w	#100,(BUMP.TOUCH,a1)	raquette clignote
		move.l	(explomon-t,a5),a3
		moveq	#NBR_EXPLOMON-1,d0
.xxelegy_l1	tst.w	(SBOB.ACTIF,a3)		SBOB déjà actif ?
		bne.s	.xxelegy_1		-> oui, passe au suivant
		move.w	(BUMP.X,a1),d0
		addq.w	#8,d0
		move.w	d0,(SBOB.POSX,a3)
		move.w	(BUMP.Y,a1),d0
		subq.w	#8,d0
		move.w	d0,(SBOB.POSY,a3)	+ pos. y du gardien
		move.w	#1,(SBOB.ACTIF,a3)	active le bob
		clr.w	(SBOB.PTANIM,a3)	raz pointeur gfx animation
		move.w	(SBOB.INITTEMPO,a3),(SBOB.TEMPO,a3)
		move.l	(SBOB.TABLE,a3),a2
		move.l	(a2)+,(SBOB.GFX,a3)
		move.l	(a2),(SBOB.MSK,a3)
		bra.s	betty_boo4
.xxelegy_1	lea	(SBOB.SIZE,a3),a3
		dbf	d0,.xxelegy_l1


betty_boo4	lea	(MIS.SIZE,a4),a4	pointe le prochain tir
		dbf	d7,tu_es_belle
		rts

*=========================================================================*
****************** initialisation des tirs des gardiens *******************
*=========================================================================*
* Entrée	=> a1.l	Structure "GARD"
*		=> a5.l Adr. 't'
tir00_rout	dc.l	tir01_init-t
		dc.l	tir02_init-t
		dc.l	tir03_init-t
		dc.l	tir04_init-t
		dc.l	tir05_init-t
		dc.l	tir06_init-t
		dc.l	tir07_init-t
		dc.l	tir08_init-t
		dc.l	tir09_init-t
		dc.l	tir10_init-t
		dc.l	tir11_init-t

*-------------------------- INIT TIR : tir >01< --------------------------*
tir01_init:	
		moveq	#NBR_MISSILE-1,d0	nombre de bob utilisable
		move.l	(missile-t,a5),a4
		lea	(tir01_pos-t,a5),a0	table sinus x bob
		moveq	#0,d4			pointeur table sinus
		move.w	(GARD.X,a1),d2		position x du gardien
		add.w	(GARD.XFIRE,a1),d2
		sub.w	#16,d2
		move.w	(GARD.Y,a1),d3		position y du gardien
		moveq	#12,d1			nombre de bob utilisés
.tir01_i0	tst.w	(MIS.ACTIF,a4)		bob deja utilisé ?
		bne.s	.tir01_i1		-> oui
		move.w	(a0,d4),(MIS.X,a4)	position table sinus x
		add.w	d2,(MIS.X,a4)		par rapport au gardien
		move.w	d4,(MIS.FD1,a4)		pointeur table sinus x
		move.w	d2,(MIS.FD2,a4)		sauve position x
		move.w	d3,(MIS.Y,a4)		sauve position y
		move.w	#4,(MIS.ACTIF,a4)	active le bob tir 01
		addq.w	#5,d3			augmente position y
		add.w	#10,d4			incremente pointeur sinus
		cmp.w	#7,d1			la moitie des tirs ?
		bne.s	.tir01_ia		-> non
		move.w	(GARD.Y,a1),d3		position y du gardien
.tir01_ia	subq.w	#1,d1
		beq.s	.tir01_end
.tir01_i1	lea	(MIS.SIZE,a4),a4	pointe prochain bob
		subq.w	#1,d0			il reste des bobs utilisables ?
		bne.s	.tir01_i0
.tir01_end	rts

*-------------* tir 02, scie circulaire + support, 10 bobs *--------------*
tir02_init:	
		moveq	#NBR_MISSILE-1,d0
		move.l	(missile-t,a5),a4
		moveq	#0,d4
		move.w	(GARD.X,a1),d2
		move.w	(GARD.Y,a1),d3
		add.w	(GARD.XFIRE,a1),d2
		add.w	(GARD.YFIRE,a1),d3
		moveq	#7,d1
.tir02_i0	tst.w	(MIS.ACTIF,a4)
		bne.s	.tir02_i1
		move.w	d4,(MIS.FD3,a4)
		move.w	d2,(MIS.FD1,a4)
		move.w	d3,(MIS.FD2,a4)
		move.w	#8,(MIS.ACTIF,a4)
		move.l	#sinus,(MIS.FD4,a4)
		add.w	#4,d4
		subq.w	#1,d1
		beq	.tir02_ia
.tir02_i1	lea	(MIS.SIZE,a4),a4
		subq.w	#1,d0
		bne.s	.tir02_i0
		rts
.tir02_ia	moveq	#3,d1
		sub.w	#10,d2
.tir02_i2	tst.w	(MIS.ACTIF,a4)
		bne.s	.tir02_i3
		move.w	d2,(MIS.X,a4)
		move.w	d3,(MIS.Y,a4)
		move.w	#12,(MIS.ACTIF,a4)
		add.w	#10,d2
		subq.w	#1,d1
		beq.s	.tir02_end
.tir02_i3	lea	(MIS.SIZE,a4),a4
		subq.w	#1,d0
		bne.s	.tir02_i2
.tir02_end	rts

*----------------------*  TIR 03, xevious, 4 bobs  *----------------------*
tir03_init	
		moveq	#NBR_MISSILE-1,d0
		move.l	(missile-t,a5),a4
		moveq	#0,d4
		move.w	(GARD.X,a1),d2
		add.w	(GARD.XFIRE,a1),d2
		move.w	(GARD.Y,a1),d3
		add.w	(GARD.YFIRE,a1),d3
		moveq	#2,d1
.tir03_i0	tst.w	(MIS.ACTIF,a4)
		bne.s	.tir03_i1
		move.w	d4,(MIS.FD3,a4)
		move.w	d2,(MIS.FD1,a4)
		move.w	d3,(MIS.FD2,a4)
		move.w	#24,(MIS.ACTIF,a4)
		move.l	#sinus,(MIS.FD4,a4)
		moveq	#56,d4
		subq.w	#1,d1
		beq.s	.tir03_v2
.tir03_i1	lea	(MIS.SIZE,a4),a4
		subq.w	#1,d0
		bne.s	.tir03_i0
		rts
.tir03_v2	moveq	#2,d1
		moveq	#0,d4
.tir03_v0	tst.w	(MIS.ACTIF,a4)
		bne.s	.tir03_v1
		move.w	d4,(MIS.FD3,a4)
		move.w	d2,(MIS.FD1,a4)
		move.w	d3,(MIS.FD2,a4)
		move.w	#24,(MIS.ACTIF,a4)
		move.l	#tir02_pos,(MIS.FD4,a4)
		moveq	#56,d4
		subq.w	#1,d1
		beq.s	.tir03_i2
.tir03_v1	lea	(MIS.SIZE,a4),a4
		subq.w	#1,d0
		bne.s	.tir03_v0
		rts
.tir03_i2	tst.w	(MIS.ACTIF,a4)
		bne.s	.tir03_i3
		move.w	d2,(MIS.X,a4)
		move.w	d3,(MIS.Y,a4)
		move.w	#28,(MIS.ACTIF,a4)
		*bra	missile_col
		rts
.tir03_i3	lea	(MIS.SIZE,a4),a4
		subq.w	#1,d0
		bne.s	.tir03_i2
.tir03_end	rts

*----------------*TIR 04, scie circulaire II, 7 bobs*---------------------*
tir04_init:
		moveq	#NBR_MISSILE-1,d0
		move.l	(missile-t,a5),a4
		lea	(tir04_pos-t,a5),a0
		moveq	#0,d4
		move.w	(GARD.X,a1),d2
		move.w	(GARD.Y,a1),d3
		add.w	(GARD.XFIRE,a1),d2
		add.w	(GARD.YFIRE,a1),d3
		moveq	#7,d1
.tir04_i0	tst.w	(MIS.ACTIF,a4)
		bne.s	.tir04_i1
		move.w	#16,(MIS.ACTIF,a4)
		move.w	d2,(MIS.FD1,a4)
		move.w	d3,(MIS.FD2,a4)
		move.w	d4,(MIS.FD3,a4)
		move.w	#1,(MIS.FD4,a4)
		move.w	#10,(MIS.FD5,a4)
		move.w	(a0)+,(MIS.FD6,a4)
		move.w	(a0)+,(MIS.FD7,a4)
		add.w	#16,d4
		subq.w	#1,d1
		beq.s	.tir04_end
.tir04_i1	lea	(MIS.SIZE,a4),a4
		subq.w	#1,d0
		bne.s	.tir04_i0
.tir04_end	rts

**--------------------* TIR 05, automatique 9 coups *--------------------**
tir05_init:
		moveq	#NBR_MISSILE-1,d0
		move.l	(missile-t,a5),a4
		moveq	#10,d4
		moveq	#-4,d5
		move.w	(GARD.X,a1),d2
		move.w	(GARD.Y,a1),d3
		add.w	(GARD.XFIRE,a1),d2
		add.w	(GARD.YFIRE,a1),d3
		moveq	#9,d1
.tir05_i0	tst.w	(MIS.ACTIF,a4)
		bne.s	.tir05_i1
		move.w	d2,(MIS.X,a4)
		move.w	d3,(MIS.Y,a4)
		move.w	d4,(MIS.FD1,a4)
		move.w	d5,(MIS.FD2,a4)
		addq.w	#1,d5
		move.w	#5,(MIS.FD3,a4)
		move.w	#20,(MIS.ACTIF,a4)
		add.w	#15,d4
		subq.w	#1,d1
		beq.s	.tir05_end
.tir05_i1	lea	(MIS.SIZE,a4),a4
		subq.w	#1,d0
		bne.s	.tir05_i0
.tir05_end	rts

*---------------------* TIR 06, scie circulaire III *---------------------*
tir06_init:
		moveq	#30,d1
		moveq	#NBR_MISSILE-1,d0
		move.l	(missile-t,a5),a4
		moveq	#0,d4
.tir06_i0	tst.w	(MIS.ACTIF,a4)
		bne.s	.tir06_i1
		move.w	d4,(MIS.FD3,a4)
		move.w	#2,(MIS.FD4,a4)
		move.l	a1,(MIS.FD1,a4)
		move.w	#32,(MIS.ACTIF,a4)
		add.w	#24,d4
		subq.w	#1,d1
		beq.s	.tir06_end
.tir06_i1	lea	(MIS.SIZE,a4),a4
		subq.w	#1,d0
		bne.s	.tir06_i0
.tir06_end	rts

*---------------------* TIR 07, scie circulaire IV *----------------------*
tir07_init:
		moveq	#10,d1
		moveq	#NBR_MISSILE-1,d0
		move.l	(missile-t,a5),a4
		moveq	#0,d4
		move.w	(GARD.X,a1),d2
		add.w	(GARD.XFIRE,a1),d2
		move.w	(GARD.Y,a1),d3
		add.w	(GARD.YFIRE,a1),d3
.tir07_i0	tst.w	(MIS.ACTIF,a4)
		bne.s	.tir07_i1
		move.w	d2,(MIS.FD1,a4)
		move.w	d3,(MIS.FD2,a4)
		move.w	d4,(MIS.FD3,a4)
		move.w	#1,(MIS.FD4,a4)
		move.w	#36,(MIS.ACTIF,a4)
		add.w	#72,d4
		subq.w	#1,d1
		beq.s	.tir07_end
.tir07_i1	lea	(MIS.SIZE,a4),a4
		subq.w	#1,d0
		bne.s	.tir07_i0
.tir07_end	rts

*----------------------* TIR 08, scie circulaire V *----------------------*
tir08_init	moveq	#NBR_MISSILE-1,d0
		move.l	(missile-t,a5),a4
		moveq	#0,d4
		moveq	#0,d5
		move.w	(GARD.X,a1),d2
		add.w	(GARD.XFIRE,a1),d2
		move.w	(GARD.Y,a1),d3
		add.w	(GARD.YFIRE,a1),d3
		sub.w	#5,d3
		moveq	#5,d1
.tir08_i0	tst.w	(MIS.ACTIF,a4)
		bne.s	.tir08_i1
		move.w	d2,(MIS.FD1,a4)
		move.w	d3,(MIS.FD2,a4)
		move.w	d4,(MIS.FD3,a4)
		move.w	#40,(MIS.ACTIF,a4)
		move.w	d5,(MIS.FD4,a4)
		move.w	#20,(MIS.FD5,a4)
		add.w	#144,d4
		subq.w	#1,d1
		beq.s	.tir08_i2
.tir08_i1	lea	(MIS.SIZE,a4),a4
		subq.w	#1,d0
		bne.s	.tir08_i0
		rts
.tir08_i2	moveq	#5,d1
		add.w	#25,d3
.tir08_i3	tst	(MIS.ACTIF,a4)
		bne.s	.tir08_i4
		move.w	d2,(MIS.FD1,a4)
		move.w	d3,(MIS.FD2,a4)
		move.w	d4,(MIS.FD3,a4)
		move.w	#40,(MIS.ACTIF,a4)
		move.w	d5,(MIS.FD4,a4)
		move.w	#1,(MIS.FD5,a4)
		add.w	#144,d4
		subq.w	#1,d1
		beq.s	.tir08_end
.tir08_i4	lea	(MIS.SIZE,a4),a4
		subq	#1,d0
		bne.s	.tir08_i3
.tir08_end	rts

**************************** TIR 09, triangle *****************************
tir09_init:
		moveq	#3,d1
		moveq	#NBR_MISSILE-1,d0
		move.l	(missile-t,a5),a4
		moveq	#0,d4
		move.w	(GARD.X,a1),d2
		add.w	(GARD.XFIRE,a1),d2
		move.w	(GARD.Y,a1),d3
		add.w	(GARD.YFIRE,a1),d3
.tir09_i0	tst.w	(MIS.ACTIF,a4)
		bne.s	.tir09_i1
		move.w	d4,(MIS.FD3,a4)
		move.w	#44,(MIS.ACTIF,a4)
		move.w	d2,(MIS.FD1,a4)
		move.w	d3,(MIS.FD2,a4)
		add.w	#240,d4
		subq.w	#1,d1
		beq.s	.tir09_i2
.tir09_i1	lea	(MIS.SIZE,a4),a4
		subq	#1,d0
		bne.s	.tir09_i0
		rts
.tir09_i2	moveq	#0,d4
		moveq	#3,d1
		add.w	#16,d2
.tir09_i3	tst.w	(MIS.ACTIF,a4)
		bne.s	.tir09_i4
		move.w	d4,(MIS.FD3,a4)
		move.w	#44,(MIS.ACTIF,a4)
		move.w	d2,(MIS.FD1,a4)
		move	d3,(MIS.FD2,a4)
		add.w	#240,d4
		subq.w	#1,d1
		beq	.tir09_i5
.tir09_i4	lea	(MIS.SIZE,a4),a4
		subq.w	#1,d0
		bne.s	.tir09_i3
		rts
.tir09_i5	moveq	#0,d4
		moveq	#3,d1
		subq.w	#8,d2
		add.w	#16,d3
.tir09_i6	tst.w	(MIS.ACTIF,a4)
		bne.s	.tir09_i7
		move.w	d4,(MIS.FD3,a4)
		move.w	#44,(MIS.ACTIF,a4)
		move.w	d2,(MIS.FD1,a4)
		move.w	d3,(MIS.FD2,a4)
		add.w	#240,d4
		subq.w	#1,d1
		beq.s	.tir09_end
.tir09_i7	lea	(MIS.SIZE,a4),a4
		subq.w	#1,d0
		bne.s	.tir09_i6
.tir09_end	rts

**--------------------** tir 10, epee de DAMOCLES **---------------------**
tir10_init:
		moveq	#6,d1
		moveq	#NBR_MISSILE-1,d0
		move.l	(missile-t,a5),a4
		lea	(tir10_pos-t,a5),a2
		moveq	#0,d4
		move.w	(GARD.X,a1),d2
		add.w	(GARD.XFIRE,a1),d2
		sub.w	#17,d2
		move.w	(GARD.Y,a1),d3
		add.w	(GARD.YFIRE,a1),d3
		sub.w	#10,d3
.tir10_i0	tst.w	(MIS.ACTIF,a4)
		bne.s	.tir10_i1
		move.w	d2,d4
		add.w	(a2)+,d4
		move.w	d4,(MIS.X,a4)
		move.w	d3,d4
		add.w	(a2)+,d4
		move.w	d4,(MIS.Y,a4)
		move.w	#12,(MIS.ACTIF,a4)
		subq.w	#1,d1
		beq.s	.tir10_end
.tir10_i1	lea	(MIS.SIZE,a4),a4
		subq.w	#1,d0
		bne.s	.tir10_i0
.tir10_end	rts

*=----------------------=* TIR 11, triangle II *=------------------------=*
tir11_init:
		moveq	#5,d1
		moveq	#NBR_MISSILE-1,d0
		move.l	(missile-t,a5),a4
		moveq	#0,d4
		move.w	(GARD.X,a1),d2
		add.w	(GARD.XFIRE,a1),d2
		move.w	(GARD.Y,a1),d3
		add.w	(GARD.YFIRE,a1),d3
.tir11_i0	tst.w	(MIS.ACTIF,a4)
		bne.s	.tir11_i1
		move.w	d4,(MIS.FD3,a4)
		move.w	#48,(MIS.ACTIF,a4)
		move.w	d2,(MIS.FD1,a4)
		move.w	d3,(MIS.FD2,a4)
		clr.w	(MIS.FD4,a4)
		move.w	#5,(MIS.FD5,a4)
		add.w	#144,d4
		subq.w	#1,d1
		beq.s	.tir11_i2
.tir11_i1	lea	(MIS.SIZE,a4),a4
		subq.w	#1,d0
		bne.s	.tir11_i0
		rts
.tir11_i2	moveq	#0,d4
		moveq	#5,d1
.tir11_i3	tst.w	(MIS.ACTIF,a4)
		bne.s	.tir11_i4
		move.w	d4,(MIS.FD3,a4)
		move.w	#48,(MIS.ACTIF,a4)
		move.w	d2,(MIS.FD1,a4)
		move.w	d3,(MIS.FD2,a4)
		move.w	#-1,(MIS.FD4,a4)
		move.w	#4,(MIS.FD5,a4)
		add.w	#144,d4
		subq.w	#1,d1
		beq.s	.tir11_i5
.tir11_i4	lea	(MIS.SIZE,a4),a4
		subq.w	#1,d0
		bne.s	.tir11_i3
		rts
.tir11_i5	moveq	#0,d4
		moveq	#5,d1
.tir11_i6	tst.w	(MIS.ACTIF,a4)
		bne.s	.tir11_i7
		move.w	d4,(MIS.FD3,a4)
		move.w	#48,(MIS.ACTIF,a4)
		move.w	d2,(MIS.FD1,a4)
		move.w	d3,(MIS.FD2,a4)
		move.w	#1,(MIS.FD4,a4)
		move.w	#4,(MIS.FD5,a4)
		add.w	#144,d4
		subq.w	#1,d1
		beq.s	.tir11_end
.tir11_i7	lea	(MIS.SIZE,a4),a4
		subq.w	#1,d0
		bne.s	.tir11_i6
.tir11_end	rts
