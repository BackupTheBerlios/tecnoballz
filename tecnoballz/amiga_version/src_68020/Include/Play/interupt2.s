*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*
*==================== ROUT=2 : INITIALISE LE MAGASIN =====================*
*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*
interupt2:
		bsr.s	rupt2
		bne.l	sys_erreur
		move.w	#3,(rout-t,a5)		>Flag magasin<
		bra	irq
*-------------------------------------------------------------------------*
rupt2:		bsr	shop.decrunch
		bne	.exit
		bsr	init_print
		bne	.exit
		lea	(tbob-t,a5),a0
		move.l	(bob32_pagegfx-t,a5),d0
		bsr	bob32_main.init
		bne	.exit
		bsr	shop.lulu
		bne	.exit
		bsr	shop.name
		bsr	init_shopgadget
		bne	.exit
		*/ Reserve BOB curseur-LED */
		moveq	#BOB.LED1,d0
		bsr.l	bob32.ajoute
		bne	.exit
		move.l	a1,(bob.led-t,a5)

		*/ Pointe le text de bienvenu */
		lea	(text00-t,a5),a0
		bsr	put_ze_text		"interupt3.s"
		tst.w	(sh.prix-t,a5)		PRICE BONUS ?
		beq.s	.nola			-> Non
		move.l	a0,(a1)
.nola
		*/ RAZ quelques adresses */
		moveq	#0,d0
		move.w	d0,(bonus.achete-t,a5)	RAZ nbre de bonus
		move.w	d0,(bonus.step-t,a5)	RAZ fréquence chute bonus
		move.w	d0,(lessbrick-t,a5)	RAZ flag less briques
		move.w	d0,(shopout-t,a5)	RAZ flag quitte magasin
		move.w	d0,(sh.infovu-t,a5)	RAZ info deja achete
		move.b	d0,(flag_pause-t,a5)	RAZ flag pause
		move.w	d0,(music4voix-t,a5)	Musique 4 cannaux
		move.w	d0,(musicsound-t,a5)	Son + musique
		move.w	d0,(sh.optinfo-t,a5)
		move.w	d0,(sh.get-t,a5)
		moveq	#0,d7
.exit		rts

*-------------------------------------------------------------------------*
*************************  Décompacte le magasin  *************************
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
shop.decrunch
		*/ Décompacte le magasin */
		lea	(pt_screen-t,a5),a4
		bsr	screen_res
		bne.s	.exit
		move.w	#1,(SFREQ,a0)		force 50Hz
		clr.w	(SFMODE_LUTINS,a0)	lutin de 16 pixels de large
		move.w	#3,(SFMODE_PLANS,a0)
		move.w	#1,(SLUTIN,a0)		pt lutins dans la liste
		move.w	#$24,(SBPLCON2,a0)
		move.w	#$11,(SBPLCON4,a0)	Couleur lutin
		move.w	#1,(NBCOPPER,a0)	deux liste Copper
		move.w	#3,(SPBUFFER,a0)
		lea	(shop1.pp-t,a5),a4
		move.l	(RESA.ADR,a4),d0
		move.l	(RESA.SIZ,a4),d1
		bsr	affiche_pp
.exit		rts

*-------------------------------------------------------------------------*
*======================= Recopie le nom du joueur ========================*
*-------------------------------------------------------------------------*
* entrée	=> a5.l	adr. table 't'
shop.name:
		lea	(8+text00-t,a5),a0
		lea	(name.x-t,a5),a1
		moveq	#0,d0
		moveq	#5,d1
.loop		move.b	(a1)+,d0
		add.b	#65,d0
		move.b	d0,(a0)+
		dbf	d1,.loop
		rts

***************************************************************************
*******************INITIALISE LE SPRITE POINTEUR SOURIS********************
***************************************************************************
* entrée	=> a5.l	adr. table 't'
shop.lulu:	
		*/ Reserve les lutins /*
		lea	(shop.sprite-t,a5),a4
		bsr.l	reserve.mem
		bne.s	.exit
		lea	(shop.sprite-t,a5),a4
		move.l	(RESA.ADR,a4),a4
		lea	(shop.lutin-t,a5),a0
		moveq	#15,d0
.looooop	move.l	a4,(a0)+
		lea	(4*18,a4),a4
		dbf	d0,.looooop
	
		*/ CONVERTIT BOB -> SPRITE */
		moveq	#BOB.PT1,d0
		move.w	(hasard-t,a5),d1
		and.w	#$0001,d1
		beq.s	.sqs
		moveq	#BOB.PT2,d0
.sqs		bsr	bob32_get.bob
		lea	(TBOB_TABLE,a0),a4
		move.l	(bob32_pagegfx-t,a5),a0
		move.w	(PG_SPLAN,a0),d1
		move.w	(PG_SLINE,a0),d2
		lea	(shop.lutin-t,a5),a3
		moveq	#7,d4
.loopapa	move.l	(a3)+,a0
		move.l	(a3)+,a1
		move.l	(a4),a2
		moveq	#15,d0
		bsr	copytolutin16.0
		addq.l	#8,a4
		dbf	d4,.loopapa
		bsr	raz_table_lutins
		moveq	#0,d7
.exit		rts

*-------------------------------------------------------------------------*
*----------------* Initialise les gadgets bonus/malus *-------------------*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
* Sortie	<= d7.l	0=pas d'erreur
init_shopgadget:
		*/ Réserve les structures */
		lea	(pt_gadget-t,a5),a4
		move.l	#GAD.SIZE*NGAD,(RESA.SIZ,a4)
		bsr.l	reserve.mem
		bne	.exit

		*/ Init. les structures */
		move.l	(pt_gadget-t,a5),a1
		moveq	#NGAD-1,d6
		move.w	#-5,d5
.loopinit	moveq	#BOB.GADGET,d0
		bsr	bob32.only
		bne	.exit
		move.w	#1,(GAD.TEMPO,a1)
		clr.l	(BOB.ACTIF,a1)
		move.w	#295,(GAD.X,a1)
		move.w	d5,(GAD.Y,a1)
		add.w	#9,d5	
		lea	(b1-t,a5),a0
		move.l	a0,(GAD.RAK,a1)
		lea	(GAD.SIZE,a1),a1
		dbf	d6,.loopinit
		*/ GADGET SE DEPLACANT AVEC LA SOURIS */
		lea	(-GAD.SIZE,a1),a1
		move.l	a1,(bob.get-t,a5)
		*/ GADGET SELECTIONNEE */
		move.l	(pt_gadget-t,a5),a1
		move.w	#280,(GAD.X,a1)
		move.w	#221,(GAD.Y,a1)
		*/ RAZ table options */
		lea	(course-t,a5),a0
		moveq	#NOPTIONS-1,d0
.lx		clr.w	(a0)+
		dbf	d0,.lx
		moveq	#0,d7
.exit		rts
