*=========================================================================*
*       Routines appellées lorqu'un bonus/malus touche une raquette	  *
*=========================================================================*
* NE PAS UTILISEE a6 & d7 (utilisee par la routine "gadget_dep.s"
* Routines appellées par "gadget_dep.s"
* Entrée	=> d6.l	#0
*		=> a1.l	structure raquette
*		=> a5.l	adr. table 't'
*
*-----------------------------------------------------------------------------*
*»»»»»»»»»»»»»»»»»»»»»»»»» Ramasse bonus 'colle' «««««««««««««««««««««««««««««*
*-----------------------------------------------------------------------------*
gad_colle:	tst.w	(BUMP.GLUE,a1)
		bne.s	gad_go
		moveq	#1,d0
		move.w	d0,(BUMP.COPY,a1)
		move.w	d0,(BUMP.GLUE,a1)
		move.b	d0,(sound+S_TRANSFORME-t,a5)
		GETMESSAGE	16
gad_go		rts

*-------------------------------------------------------------------------*
*»»»»»»»»»»»»»»»»»»»»»»»» Passe au niveau suivant ««««««««««««««««««««««««*
*-------------------------------------------------------------------------*
* Ne pas utiliser a2 !
voleur:		tst.w	(gameover-t,a5)
		bne.s	.oleoa
		move.w	#1,(tableau.end-t,a5)
		moveq	#0,d0
		move.w	d0,(brick0-t,a5)
		move.w	d0,(brick1-t,a5)
		move.w	d0,(brick2-t,a5)
		move.w	d0,(music4voix-t,a5)
		move.l	(pt_poz-t,a5),a0
		move	(4,a0),d0		pos. musique win
		bsr.l	np_change
		clr.w	(musiclost-t,a5)
		GETMESSAGE	17
.oleoa		rts

*-------------------------------------------------------------------------*
*««««««««««««««««««« Ramasse malus 'rétrécit raquette' »»»»»»»»»»»»»»»»»»»*
*-------------------------------------------------------------------------*
gad_taim:	cmp.w	#16,(rak.large-t,a5)
		beq	gad_go
		subq.w	#8,(rak.large-t,a5)
		moveq	#1,d0
		move.w	d0,(rak.allcopy-t,a5)
		move.b	d0,(sound+S_TRANSFORME-t,a5)

		*-=- désactive les tirs -=-*
		move.l	(pt_objet-t,a5),a1
		lea	(FIRE.ACTIF,a1),a1
		moveq	#0,d0
		moveq	#NTIR*4-1,d1
.nice_blonde	move.w	d0,(a1)
		lea	(FIRE.SIZE,a1),a1
		dbf	d1,.nice_blonde
		GETMESSAGE	18
		rts

*-------------------------------------------------------------------------*
*»»»»»»»»»»»»»»»»»»» Ramasse bonus 'allonge raquette' ««««««««««««««««««««*
*-------------------------------------------------------------------------*
gad_taip:	cmp.w	#64,(rak.large-t,a5)
		beq	gad_go
		GETMESSAGE	19
		addq.w	#8,(rak.large-t,a5)
		moveq	#1,d0
		move.w	d0,(rak.allcopy-t,a5)
		move.b	d0,(sound+S_TRANSFORME-t,a5)

		*-=- Désactive les tirs -=-*
		move.l	(pt_objet-t,a5),a1
		lea	(FIRE.ACTIF,a1),a1
		moveq	#0,d0
		moveq	#NTIR*4-1,d1
.nice_brune	move.w	d0,(a1)
		lea	(FIRE.SIZE,a1),a1
		dbf	d1,.nice_brune
		tst.w	(rak.team-t,a5)		Mode team ?
		beq.s	la_garce		-> non

		*-=- mode team test si la raqu. hori. ne dépasse pas  -=-*
gad_ici:	lea	(b1-t,a5),a1
		move.w	#224,d0
		sub.w	(rak.large-t,a5),d0
		cmp.w	(BUMP.X,a1),d0
		ble.s	.cadis_gnou
		bra.s	.cadis_jnou
.cadis_gnou	move.w	d0,(BUMP.X,a1)
		move.w	d0,(2*BUMP.SIZE+BUMP.X,a1)
		*-=- mode team test si la raqu. verti. ne dépasse pas  -=-*
.cadis_jnou	lea	(b2-t,a5),a1
		cmp.w	(BUMP.Y,a1),d0
		ble.s	.cadis_goun
		rts
.cadis_goun	move	d0,(BUMP.Y,a1)
		move	d0,(2*BUMP.SIZE+BUMP.Y,a1)
		rts

		*-=- mode alone test si la raqu. ne dépasse pas  -=-*
la_garce	lea	(b1-t,a5),a1
		move.w	#224,d0
		sub.w	(rak.large-t,a5),d0
		cmp.w	(BUMP.X,a1),d0
		ble.s	.cadis_gland
		rts
.cadis_gland	move.w	d0,(BUMP.X,a1)
		move.w	d0,(BUMP.SIZE+BUMP.Y,a1)
		move.w	d0,(2*BUMP.SIZE+BUMP.X,a1)
		move.w	d0,(3*BUMP.SIZE+BUMP.Y,a1)
		rts

*-------------------------------------------------------------------------*
*«««««««««««««««««««« Ramasse malus 'enlève une vie' »»»»»»»»»»»»»»»»»»»»»*
*-------------------------------------------------------------------------*
gad_viem:	bsr	lune
		GETMESSAGE	10
		rts

*-------------------------------------------------------------------------*
*-------------------- Ramasse bonus 'ajoute une vie' ---------------------*
*-------------------------------------------------------------------------*
gad_viep:	bsr	lune1
		GETMESSAGE	11
		rts

*-------------------------------------------------------------------------*
*                     Ramasse bonus 'tir puissance 2'			  *
*-------------------------------------------------------------------------*
gad_fire2:	move.w	#1,(fire.flag-t,a5)	flag tir puissance 2
		move.w	#4,(fire.val-t,a5)
		moveq	#PFIRE2,d1		adr. gfx
		GETMESSAGE	05
		bra.s	gad_fire3

*-------------------------------------------------------------------------*
*                     Ramasse bonus 'tir puissance 1'			  *
*-------------------------------------------------------------------------*
gad_fire1:	clr.w	(fire.flag-t,a5)	flag tire puissance 1
		move.w	#2,(fire.val-t,a5)
		moveq	#PFIRE1,d1		adr. gfx
		GETMESSAGE	04

		*-=- init la raquette tireuse et les tirs -=-*
gad_fire3	move.w	#1,(BUMP.FIRE,a1)
		move.w	#1,(BUMP.COPY,a1)
		clr.w	(BUMP.NORMAL,a1)
		move.l	(BUMP.OBJET,a1),a0
		moveq	#0,d0
		moveq	#NTIR-1,d2
.loop		move.w	d1,(FIRE.PTBOB,a0)
		move.w	d0,(FIRE.ACTIF,a0)
		lea	(FIRE.SIZE,a0),a0
		dbf	d2,.loop
		move.b	#1,(sound+S_TRANSFORME-t,a5)
		rts

*-------------------------------------------------------------------------*
*                      Ramasse bonus 'plus 3 balles éjecteurs'			  *
*-------------------------------------------------------------------------*
gad_2ball:
		GETMESSAGE	20
		move.b	#1,(sound+S_GADGET-t,a5)
		lea	(furax-2-t,a5),a0
		move.w	(a0),d0
		cmp.w	#3*12,d0
		bne.s	.nf
		moveq	#-12,d0
.nf		add.w	#12,d0
		move.w	d0,(a0)+
		add.w	d0,a0
		moveq	#2,d5
		lea	(speed1-t,a5),a2
		lea	(bri_poi-t,a5),a3
		move.l	(pt_balle-t,a5),a4	1er struct. balle
		moveq	#64,d2
		moveq	#0,d3
		moveq	#NBALLE-1,d0		nbre maxi de balle
.gorby_loop	tst.w	(a4)			balle active ?
		bne.s	.gorby_zoop		-> oui
		move.w	#1,(a4)
		move.l	(a0)+,(BALLE.EJECT1,a4)
		move.l	(a0)+,(BALLE.EJECT3,a4)
		move.l	(a0)+,(BALLE.X,a4)
		move.w	(balle_scount-t,a5),(BALLE.SCOUNT,a4)
		move.w	#2,(BALLE.PT,a4)	balle normale
		move.l	a1,(BALLE.RAK,a4)
		move.l	a2,(BALLE.SPEED,a4)
		move.l	a3,(BALLE.POI,a4)
		move.l	d2,(BALLE.DIRECT,a4)
		move.w	#5,(BOB.WIDTH,a4)
		move.w	#2,(BALLE.VAL,a4)
		clr.w	(BALLE.ROT,a4)
		addq.w	#1,(balle_nbre-t,a5)		+ une balle
		dbf	d5,.gorby_zoop
		rts
.gorby_zoop	lea	(BALLE.SIZE,a4),a4	
		dbf	d0,.gorby_loop
.rien_va_plus	rts

*-------------------------------------------------------------------------*
*                      Ramasse bonus 'plus 3 balles'			  *
*-------------------------------------------------------------------------*
gad_3ball:	
		GETMESSAGE	21
		move.b	#1,(sound+S_GADGET-t,a5)
		moveq	#NBALLE-1,d2
		move.l	(pt_balle-t,a5),a4
.f1		tst.w	(a4)			Cherche une balle active
		bne.s	.go
		lea	(BALLE.SIZE,a4),a4
		dbf	d2,.f1
		rts

.go		move.w	(2+BALLE.DIRECT,a4),d0
		moveq	#2,d1
		moveq	#NBALLE-1,d2
		move.w	(BOB.WIDTH,a4),d3
		move.w	(BALLE.PT,a4),d4
		move.l	(pt_balle-t,a5),a0
		move.l	(BALLE.SPEED,a4),a2
		move.l	(BALLE.POI,a4),a3

.loop		tst.w	(a0)
		bne.s	.next
		move.w	#1,(a0)
		move.l	(BALLE.X,a4),(BALLE.X,a0)
		addq.w	#8,d0
		and.w	#60,d0
		move.w	d0,(2+SBALLE.DIRECT,a0)
		clr.w	(BALLE.GLUE,a0)
		clr.w	(BALLE.TILT,a0)
		move.w	d4,(BALLE.PT,a0)
		move.w	d3,(BOB.WIDTH,a0)
		move.l	a1,(BALLE.RAK,a0)
		move.l	a2,(BALLE.SPEED,a0)
		move.l	a3,(BALLE.POI,a0)
		move.w	(BALLE.VAL,a4),(BALLE.VAL,a0)
		move.w	(balle_scount-t,a5),(BALLE.SCOUNT,a0)
		clr.w	(BALLE.ROT,a0)
		addq.w	#1,(balle_nbre-t,a5)
		dbf	d1,.next
		rts

.next		lea	(BALLE.SIZE,a0),a0
		dbf	d2,.loop
		rts

*-------------------------------------------------------------------------*
*                    Ramasse bonus 'balle puissance 1'			  *
*-------------------------------------------------------------------------*
gad_destr1:	moveq	#4,d0
		GETMESSAGE	22
		bra.s	gad_dset

*-------------------------------------------------------------------------*
*                    Ramasse bonus 'balle puissance 2'			  *
*-------------------------------------------------------------------------*
gad_destr2:	moveq	#6,d0
		GETMESSAGE	23
gad_dset	move.b	#1,(sound+S_GADGET-t,a5)
		move.l	(pt_balle-t,a5),a4
		lea	(BALLE.PT,a4),a4
		moveq	#NBALLE-1,d1
.loop		move.w	d0,(a4)
		lea	(BALLE.SIZE,a4),a4
		dbf	d1,.loop
		rts

*-------------------------------------------------------------------------*
*                 Ramasse malus 'inverseur de commandes'		  *
*-------------------------------------------------------------------------*
gad_inverse:	move.w	#50*4,(rak.inver-t,a5)	4 secondes
		moveq	#1,d0
		move.w	d0,(inv50-t,a5)		compteur 50ieme de sec.
		move.w	#9,(inv0-t,a5)		unité à 9
		move.w	#9,(inv1-t,a5)		dizaine à 9
		GETMESSAGE	24
		rts


*-------------------------------------------------------------------------*
*                    */ Augmente vitesse des balles */			  *
*-------------------------------------------------------------------------*
gad_speed:
		moveq	#NBALLE-1,d0
		move.l	(pt_balle-t,a5),a3
		lea	(speed3-t,a5),a1
.koloop		move.l	a1,(BALLE.SPEED,a3)
		lea	(BALLE.SIZE,a3),a3
		dbf	d0,.koloop
		GETMESSAGE	25
		rts

*-------------------------------------------------------------------------*
*----------------------*  Bonus "active Raquette"  *----------------------*
*-------------------------------------------------------------------------*
gad_bump1	lea	(b1-t,a5),a1
		bra.s	sonic
gad_bump2	lea	(b2-t,a5),a1
		GETMESSAGE	26
		bra.s	sonic
gad_bump3	lea	(b3-t,a5),a1
		GETMESSAGE	27
		bra.s	sonic
gad_bump4	lea	(b4-t,a5),a1
		GETMESSAGE	28
sonic		addq.w	#1,(BUMP.ACTIF,a1)
		move.l	a1,(bump.triche-t,a5)
		move.l	(BUMP.BOB,a1),a1
		move.l	#'BOB1',(BOB.ACTIF,a1)
		move.b	#1,(sound+S_TRANSFORME-t,a5)
		rts

*-------------------------------------------------------------------------*
*-------------------* Bonus augmente taille de balle *--------------------*
*-------------------------------------------------------------------------*
gad_size1	moveq	#7,d0
		moveq	#4,d2
		lea	(bri_poi2-t,a5),a3
		GETMESSAGE	29
		bra.s	gad_setsize
gad_size2	moveq	#9,d0
		moveq	#6,d2
		lea	(bri_poi3-t,a5),a3
		GETMESSAGE	30
gad_setsize	move.l	(pt_balle-t,a5),a4
		moveq	#NBALLE-1,d1
.loop		move.w	d0,(BOB.WIDTH,a4)
		move.w	d2,(BALLE.VAL,a4)
		move.l	a3,(BALLE.POI,a4)
		lea	(BALLE.SIZE,a4),a4
		dbf	d1,.loop
		move.b	#1,(sound+S_GADGET-t,a5)
		rts

*-------------------------------------------------------------------------*
*                              Gadget RANDOM				  *
*-------------------------------------------------------------------------*
gad_random:
		move.w	(hasard2-t,a5),d0
		add.w	(hasard3-t,a5),d0
		and.w	#127,d0
		move.w	(random.list-t,a5,d0*2),d0
		jmp	([gad_jump1,pc,d0*2])

*-------------------------------------------------------------------------*
*----------------* Gadget MEGA-BONUS (toutes les options) *---------------*
*-------------------------------------------------------------------------*
gad_mega:
		move.b	#1,(sound+S_GADGET-t,a5)
		GETMESSAGE	31

		*/ Active toutes les balles */
		moveq	#NBALLE-1,d4		nbre maxi de balle
		lea	(furax2-t,a5),a0
		lea	(speed1-t,a5),a2
		lea	(bri_poi3-t,a5),a3
		move.l	(pt_balle-t,a5),a4	1er struct. balle
		moveq	#64,d2
		moveq	#6,d0
		moveq	#9,d1
.gorby_loop	tst.w	(BALLE.ACTIF,a4)	balle active ?
		bne	.gorby_zoop		-> oui
		move.w	#1,(BALLE.ACTIF,a4)
		move.l	(a0)+,(BALLE.EJECT1,a4)
		move.l	(a0)+,(BALLE.EJECT3,a4)
		move.l	(a0)+,(BALLE.X,a4)
		move.w	(balle_scount-t,a5),(BALLE.SCOUNT,a4)
		move.l	a2,(BALLE.SPEED,a4)
		move.l	a1,(BALLE.RAK,a4)
		move.l	d2,(BALLE.DIRECT,a4)
		addq.w	#1,(balle_nbre-t,a5)
.gorby_zoop	move.w	d0,(BALLE.PT,a4)
		move.w	d1,(BOB.WIDTH,a4)
		move.l	a3,(BALLE.POI,a4)
		lea	(BALLE.SIZE,a4),a4
		dbf	d4,.gorby_loop

		*/ Active Raquette <1> */
		lea	(b1-t,a5),a1
		moveq	#1,d0
		move.w	d0,(BUMP.GLUE,a1)
		move.w	d0,(BUMP.FIRE,a1)
		clr.w	(BUMP.NORMAL,a1)

		*/ Active Raquette <2> */
		lea	(b2-t,a5),a1
		move.w	d0,(BUMP.GLUE,a1)
		move.w	d0,(BUMP.FIRE,a1)
		clr.w	(BUMP.NORMAL,a1)
		addq.w	#1,(BUMP.ACTIF,a1)
		move.l	(BUMP.BOB,a1),a1
		move.w	d0,(BOB.ACTIF,a1)

		*/ Active Raquette <3> */
		lea	(b3-t,a5),a1
		move.w	d0,(BUMP.GLUE,a1)
		move.w	d0,(BUMP.FIRE,a1)
		clr.w	(BUMP.NORMAL,a1)
		addq.w	#1,(BUMP.ACTIF,a1)
		move.l	(BUMP.BOB,a1),a1
		move.w	d0,(BOB.ACTIF,a1)

		*/ Active Raquette <4> */
		lea	(b4-t,a5),a1
		move.w	d0,(BUMP.GLUE,a1)
		move.w	d0,(BUMP.FIRE,a1)
		clr.w	(BUMP.NORMAL,a1)
		addq.w	#1,(BUMP.ACTIF,a1)
		move.l	(BUMP.BOB,a1),a1
		move.w	d0,(BOB.ACTIF,a1)

		move.w	#64,(rak.large-t,a5)
		move.w	#1,(rak.allcopy-t,a5)
		move.w	#1,(fire.flag-t,a5)	flag tir puissance 2

		*-=- Désactive les tirs & init pt. gfx -=-*
		move.l	(pt_objet-t,a5),a1
		moveq	#0,d0
		moveq	#PFIRE2,d1
		moveq	#NTIR*4-1,d2
.nice_brune	move.w	d0,(FIRE.ACTIF,a1)
		move.w	d1,(FIRE.PTBOB,a1)
		lea	(FIRE.SIZE,a1),a1
		dbf	d2,.nice_brune
		bra	gad_ici

*-------------------------------------------------------------------------*
*                           PRIX DU MAGASIN A 1				  *
*-------------------------------------------------------------------------*
gad_prixone
		move.b	#1,(sound+S_GADGET-t,a5)
		GETMESSAGE	15
		move.w	#1,(sh.prix-t,a5)
		rts

*-------------------------------------------------------------------------*
*****************************> Active le mur <*****************************
*-------------------------------------------------------------------------*
gad_mur:
		GETMESSAGE	32
		clr.w	(robot-t,a5)
		tst.w	(mur_du_bas-t,a5)
		beq.s	.first
		tst.w	(mur_off-t,a5)
		bne.s	.first
		add.w	#500,(mur_du_bas-t,a5)
		rts
.first		add.w	#500,(mur_du_bas-t,a5)
		move.w	#3,(mur_on-t,a5)
		clr.w	(mur_off-t,a5)
		rts

*-------------------------------------------------------------------------*
****************************> Active le Robot <****************************
*-------------------------------------------------------------------------*
gad_robot:
		tst.w	(robot-t,a5)
		bne.s	.yola
		move.b	#1,(sound+S_GADGET-t,a5)
		GETMESSAGE	33
		move.b	#1,(sound+S_BIG.EXPLO-t,a5)
		move.w	#1,(robot-t,a5)
		lea	(br-t,a5),a0
		move.w	#(BP_MAX-BP_MIN-64)/2+BP_MIN,d0
		move.w	d0,(BUMP.X,a0)
.yola		tst.w	(mur_du_bas-t,a5)
		beq.s	.exit
		clr.w	(mur_du_bas-t,a5)
		clr.w	(mur_on-t,a5)
		move.w	#3,(mur_off-t,a5)
.exit		rts

*-------------------------------------------------------------------------*
****************************> Active le Controlator <**********************
*-------------------------------------------------------------------------*
gad_controlator	
		tst.b	(controlator-t,a5)
		bne.s	.exit
		move.b	#1,(sound+S_GADGET-t,a5)
		GETMESSAGE	34
		move.b	#1,(controlator-t,a5)
.exit		rts


gad_oeil:
		move.l	(pt_oeil-t,a5),a4
		move.w	#1,(a4)			Oeil actif
		rts
