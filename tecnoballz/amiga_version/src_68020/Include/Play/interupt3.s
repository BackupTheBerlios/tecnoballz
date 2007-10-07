*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*
**************************** ROUT=3 : MAGASIN *****************************
*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l CUSTOM
interupt3:
		bsr	testendmod		£=> Test si fin module
		bsr	init_pts_lutins
		bsr	wait_ze_scan
		bsr	flip_screens
		bsr	yourris			|-> Déplacement souris
		bsr	yneil2			|-> Dépassement souris
		bsr	bob32_restore
		bsr	shopcredit		|-> Affiche le fric
		bsr	affprix			|-> Affiche prix bonus
		bsr	shoptext		|-> Pointe les textes
		bsr	shapesunderwater	|-> Animaton des 21 gadgets
		bsr	le_guidon		|-> Achète option
		bsr	auchan			|-> Quitte le magasin
		bsr	kurzor			|-> Efface/Affiche LED
		bsr	rexattitude		|-> Affiche les textes
		bsr	bob32_affiche
		bsr	sh.ballade		|-> Ballade les gadgets
		bsr	sh.mercryl		|-> Animation sprite pt souris
		bsr.l	pertedememoire		|-> Glou-Glou
		bsr	trainer_test
		bsr.l	quit
		bra	irq

*-------------------------------------------------------------------------*
***************************** Test la souris ******************************
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
yourris:
		moveq	#0,d4
		moveq	#0,d5
		move.b	(1+JOY0DAT,a6),d4
		move.b	(sh.ax-t,a5),d5
		move.b	d4,(sh.ax-t,a5)
		tst.w	(sh.optinfo-t,a5)
		bne.s	.yneil
		sub.b	d5,d4
		tst.b	d4
		bge.s	.ywitri
		moveq	#0,d6
		move.b	d4,d6
		not.b	d6
		sub.w	d6,(sh.mousex-t,a5)
		bra.s	.yneil
.ywitri		add	d4,(sh.mousex-t,a5)
.yneil		moveq	#0,d4
		moveq	#0,d5
		move.b	(JOY0DAT,a6),d4
		move.b	(sh.ay-t,a5),d5
		move.b	d4,(sh.ay-t,a5)
		tst.w	(sh.optinfo-t,a5)
		bne.s	.yneil2
		sub.b	d5,d4
		tst.b	d4
		bge.s	.ywitri2
		moveq	#0,d6
		move.b	d4,d6
		not.b	d6
		sub.w	d6,(sh.mousey-t,a5)
		bra.s	.yneil2
.ywitri2	add.w	d4,(sh.mousey-t,a5)
.yneil2		rts

*--------------------------------------------------------------------------*
*                    Test dépassement pointeur souris			   *
*--------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
yneil2:		
		tst.w	(sh.mousex-t,a5)
		bpl.s	.ndxg_bis
		clr.w	(sh.mousex-t,a5)
.ndxg_bis	tst.w	(sh.mousey-t,a5)
		bmi.s	.factor2
		cmp.w	#1,(sh.mousey-t,a5)
		bhi.s	.ndyb_bis
.factor2	move.w	#1,(sh.mousey-t,a5)
.ndyb_bis	cmp.w	#303,(sh.mousex-t,a5)
		blt.s	.ndyh_bis
		move.w	#303,(sh.mousex-t,a5)
.ndyh_bis	cmp.w	#211,(sh.mousey-t,a5)
		blt.s	.ncoor
		move.w	#211,(sh.mousey-t,a5)
.ncoor		rts

*--------------------------------------------------------------------------*
****************** Affiche la somme d'argent disponible *******************
*--------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
shopcredit:
		moveq	#38,d0
		move.l	#200,d1
		moveq	#5,d2
		lea	(credit0-t,a5),a0
		bsr	print_tropic
		rts

*-------------------------------------------------------------------------*
************************Affiche le prix de l'option************************
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
affprix:
		*/ Recopie le prix */
		move.w	(sh.ptget-t,a5),d0		Gadget volant
		bpl.s	.mister
		move.w	(sh.pt-t,a5),d0			Gadget selectionné
		bmi.s	.nul
.mister		lea	(curs_pri2-t,a5),a1
		tst.w	(sh.prix-t,a5)
		beq.s	.nolaon
		lea	(tcurs_pri2-t,a5),a1
.nolaon		move.l	(a1,d0),a2
		add.l	a5,a2
		lea	(prix0-t,a5),a0
		moveq	#5,d0
.pri_loop	move.w	(a2)+,(a0)+
		dbf	d0,.pri_loop

		*/ Affiche le prix d'un bonus */
		lea	(prix0-t,a5),a0
		bra.s	.ok
.nul		lea	(pri00-t,a5),a0
.ok		moveq	#38,d0
		move.w	#244,d1
		moveq	#5,d2
		bsr	print_tropic
.exit		rts

*-------------------------------------------------------------------------*
**************************** RECOPY LES TEXTES ****************************
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
shoptext:
		bsr	etat_raquette
		*/ Nombre de vies */
		lea	(text70+5-t,a5),a0
		move.b	(1+life1-t,a5),(a0)
		add.b	#27+65,(a0)+
		move.b	(1+life0-t,a5),(a0)
		add.b	#27+65,(a0)
		*/ Recopie le PASSWORD */
		move.w	(area-t,a5),d0
		subq.w	#1,d0
		mulu	#10*4,d0
		move.w	(hard-t,a5),d1		Un password different pour les 4 niveaux
		mulu	#10,d1
		add.w	d1,d0
		lea	(code.area1-t,a5,d0),a1
		lea	(text74+10-t,a5),a0
		moveq	#10-1,d0
.m_m_loop	move.b	(a1)+,(a0)		recopie password
		add.b	#65,(a0)+
		dbf	d0,.m_m_loop
		*/ Difficulté */
		move.w	(hard-t,a5),d0
		and.w	#3,d0
		move.l	(.lev,pc,d0*4),d0
		move.l	d0,(16+text75-t,a5)
.exit		rts
		CNOP	0,4
.lev		dc.l	"EASY","HARD","MADf","DEAD"

*--------------------> DONNE L'ETAT DES 3 RAQUETTES <---------------------*
etat_raquette:
		lea	(text.info-t,a5),a1
		lea	(text22-t,a5),a2
		lea	(b4-t,a5),a4
		move.w	(area-t,a5),d7
		moveq	#2,d6			3 Bumpers
.loop		move.l	a2,a0			"ACTIVE"
		cmp.w	#1,(BUMP.ACTIF,a4)	Bumper actif ?
		bgt.s	.ok			-> oui
		lea	(SH.TEXT,a0),a0		"NO ACTIVE"
		move.w	(.area,pc,d6*2),d5
		cmp.w	d5,d7			Bumper indispensable ?
		blt.s	.ok			-> non
		lea	(SH.TEXT,a0),a0		"BUY"
.ok		moveq	#SH.TEXT-1,d0
.copy		move.b	(a0)+,(a1)+
		dbf	d0,.copy
		lea	(SH.TEXT*3,a2),a2
		lea	(-BUMP.SIZE,a4),a4
		dbf	d6,.loop
		rts
.area		dc.w	4,2,3			LEFT/UP/RIGHT


*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*
*====================- Animation  bonus sélectionné -=====================*
*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*
* Entrée	=> a5.l	Adr. 't'
shapesunderwater:
		*/ Lit pointeur sur le gadget sélectionné */
		move.l	(pt_gadget-t,a5),a4
		clr.l	(a4)
		move.w	(sh.pt-t,a5),d0
		bmi.s	.le_guidon
		lea	(curs_boby-t,a5),a0
		add.w	d0,a0
		move.l	(a0),d3			pt/dessin du gadget
		cmp.w	#P.VIDE00,d3		pas de gadget ?
		beq.s	.le_guidon		-> non !
		move.l	#'BOB1',(a4)
		move.w	d3,(GAD.PTBOB,a4)	Pt/Animation du BOB

		*/ Pointe les 20 gadgets */
.le_guidon	lea	(GAD.SIZE,a4),a4
		tst.w	(sh.get-t,a5)
		bne.s	.xanim
		lea	(course-t,a5),a1
		lea	(pascal-t,a5),a2
		moveq	#NGAD-3,d4
.loop9		clr.l	(a4)
		move.w	(a1)+,d0
		beq.s	.next
		move.w	(a2,d0),d0
		move.l	#'BOB1',(a4)
		cmp.w	(GAD.PTBOB,a4),d0
		beq.s	.next
		move.w	d0,(GAD.PTBOB,a4)
		move.w	(GAD.PTANIM,a4),d0	d0=pt/animation
		move.l	(BOB.TABLE,a4),a0	a0=table adr. gfx & msk
		add.w	d0,a0
		add.w	(GAD.PTBOB,a4),a0	pt/le bonus
		move.l	(a0)+,(BOB.GFX,a4)
		move.l	(a0),(BOB.MSK,a4)
.next		lea	(GAD.SIZE,a4),a4
		dbf	d4,.loop9

		*/ Animation des 22 gagdgets */
.xanim		move.l	(pt_gadget-t,a5),a4
		moveq	#NGAD-1,d4
.loop3		subq.w	#1,(GAD.TEMPO,a4)	tempo=tempo-1
		bne.s	.noamim			-> tempo > 0
		move.w	#4,(GAD.TEMPO,a4)
		move.w	(GAD.PTANIM,a4),d0	d0=pt/animation
		cmp.w	#7*8,d0			pt < au pt maxi ?
		bne.s	.no_maxi		-> oui
		moveq	#0,d0
		move.w	d0,(GAD.PTANIM,a4)
.no_maxi	addq.w	#8,(GAD.PTANIM,a4)	
		move.l	(BOB.TABLE,a4),a0	a0=table adr. gfx & msk
		add.w	d0,a0
		add.w	(GAD.PTBOB,a4),a0	pt/le bonus
		move.l	(a0)+,(BOB.GFX,a4)
		move.l	(a0),(BOB.MSK,a4)
.noamim		lea	(GAD.SIZE,a4),a4
		dbf	d4,.loop3
.exit		rts

*-------------------------------------------------------------------------*
*                      Test bouton gauche / achète 			  *
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
le_guidon:	
		move.w	(sh.pt-t,a5),d6
		bmi.s	.exit
		lea	(sh.left-t,a5),a0
		tst.b	(mouse0_left-t,a5)
		bne.s	.button_ok
		clr.w	(a0)
		bra.s	.exit
.button_ok	tst.w	(a0)			Bouton gauche enfoncé avant ?
		bne.s	.exit			-> oui, sort
		move.w	#1,(a0)
		move.l	(curs_routine-t,a5,d6),a0
		jmp	(a0)
.exit		rts

*---------------------*/ Augmente taille raquette */----------------------*
s.size_p:	
		bsr	sh_gadget
		rts
*--------------------------*/ Tir puissance 1 /*--------------------------*
s.fire01	
		bsr	sh_gadget
		rts
*--------------------------*/ Tir puissance 2 /*--------------------------*
s.fire02	
		bsr	sh_gadget
		rts
*---------------*/ Rajoute 3 balles dans les éjecteurs  /*----------------*
s.balle2	
		bsr	sh_gadget
		rts
*---------------*/ Rajoute 3 balles à partir d'une balle /*---------------*
s.balle3	
		bsr	sh_gadget
		rts
*-------------------------*/ Balle puissance 1 /*-------------------------*
s.power1	
		bsr	sh_gadget
		rts
*-------------------------*/ Balle puissance 2 /*-------------------------*
s.power2	
		bsr	sh_gadget
		rts
*--------------------------*/ Rajoute une vie /*--------------------------*
s.life_p	
		bsr	sh_gadget
		rts
*-----------------------*/ Affiche un mur en bas /*-----------------------*
s.wall01	
		bsr	sh_gadget
		rts
*--------------------------*/ Balle taille 2 /*---------------------------*
s.size01	
		bsr	sh_gadget
		rts
*--------------------------*/ Balle taille 3 /*---------------------------*
s.size02	
		bsr	sh_gadget
		rts
*----------------------*/ Active un Robot en bas /*-----------------------*
s.robot1	
		bsr	sh_gadget
		rts
*--------------------*/ Active contrôle des balles /*---------------------*
s.contro	
		bsr	sh_gadget
		rts
*---------------------*/ Active la raquette colle /*----------------------*
s.glue00	
		bsr	sh_gadget
		rts

*-------------------------*/ Enlève 10 Briques /*-------------------------*
s.lessbk:
		bsr	sh_assez_riche
		bne.s	.exit
		lea	(lessbrick-t,a5),a0
		moveq	#10,d0
		cmp.w	(a0),d0
		beq.s	.ok
		move.w	d0,(a0)
		bsr	sh_achete1
.ok		bsr	sh_messageok
.exit		rts
*---------------------------*/ Informations /*----------------------------*
s.inform:
		lea	(sh.optinfo-t,a5),a2
		tst.w	(a2)
		bne.s	.next
		tst.w	(sh.infovu-t,a5)	Infos déjà vues ?
		bne.s	.ok			-> oui, ne paie pas
		move.w	#1,(sh.infovu-t,a5)
		bsr	sh_assez_riche
		bne.s	.exit
		bsr	sh_achete1
.ok		move.w	#1,(sh.optinfo-t,a5)
		bsr	sh_messageok
		bra.s	.exit
.next		move.w	(a2),d0
		addq.w	#1,(a2)
		and.w	#3,(a2)			sh.optinfo=0/1/2/3
		subq.w	#1,d0
		mulu	#SH.TEXT*3,d0
		lea	(text23-t,a5,d0),a0
		bsr	put_ze_text
		tst.w	(a2)			dernier message ?
		bne.s	.exit			-> non
		tst.w	(sh.code-t,a5)		code entrée ? "040670"
		beq	.exit			-> non
		bsr	put_ze_text		"TLK GAMES - TECNOBALL"
		cmp.l	#ETB,(triche-t,a5)	E+T+B+RETURN ?
		bne.s	.exit			-> Non
		bsr	put_ze_text
.exit		rts
*-------------------------*/ Reconstuit le mur /*-------------------------*
s.rebuil:
		cmp	#5,(area-t,a5)		Area 5 ?
		bne.s	.no			-> non
		bsr	sh_assez_riche
		bne.s	.exit
		lea	(brico.rebuild-t,a5),a0
		tst.w	(a0)
		bne.s	.ok
		move.w	#1,(a0)
		bsr	sh_achete1
.ok		bsr	sh_messageok
		bra.s	.exit
.no		lea	(text56-t,a5),a0
		bsr	put_ze_text
.exit		rts

*---------------------*/ Active la raquette gauche /*---------------------*
s.bump04:
		lea	(b4-t,a5),a3
		bsr	sh_bumper
		rts
*---------------------*/ Active la raquette haute /*----------------------*
s.bump03:
		lea	(b3-t,a5),a3
		bsr	sh_bumper
		rts
*---------------------*/ Active la raquette droite /*---------------------*
s.bump02:
		lea	(b2-t,a5),a3
		bsr	sh_bumper
		rts
*-------------------------*/ Sort du magasin /*---------------------------*
s.__exit:
		tst.w	(shopout-t,a5)		sortie en cours ?
		bne.s	.exit			-> oui
		move.w	#1,(shopout-t,a5)	flag sort du magasin
		bsr	sh_messageok
.exit		rts

***************************************************************************
*
*--------------------------* Achète un bumper *---------------------------*
* Entrée	=> d6.w	sh.pt
*		=> a3.l	Adr. bumper
sh_bumper:
		bsr	sh_assez_riche
		bne.s	.exit
		lea	(BUMP.ACTIF,a3),a0
		moveq	#4,d0
		cmp.w	(a0),d0
		beq.s	.ok
		move.w	d0,(a0)
		bsr	sh_achete1
.ok		bsr	sh_messageok
.exit		rts

*--------------------------* Achete un gadget *---------------------------*
* Entrée	=> d6.w	sh.pt
sh_gadget:	
		bsr	sh_assez_riche
		bne.s	.exit
		bsr	sh_20gadgets
		bne.s	.exit
		bsr	sh.ajoute_gad
		bsr	sh_achete1		|-> soustrait crédit
		bsr.s	sh_messageok
.exit		rts

*-------------------* Message si l'option est achétée *-------------------*
sh_messageok:
		move.l	(police-t,a5,d6),d0
		mulu	#SH.TEXT*3,d0
		lea	(text12-t,a5,d0),a0
		bsr	put_ze_text
		rts

********************** test si le joueur peut payer ***********************
* Entrée	=> d6.w	sh.pt
* Sortie 	<= d7=0 assez de crédits
sh_assez_riche:	
		moveq	#0,d7
		lea	(curs_pri2-t,a5),a1
		tst.w	(sh.prix-t,a5)
		beq.s	.nolaon
		lea	(tcurs_pri2-t,a5),a1
.nolaon		add.w	d6,a1
		move.l	(a1),a2
		add.l	a5,a2
		lea	(credit0-t,a5),a0
		move.w	(12,a2),d0
		move.w	(12,a0),d1
		cmp.w	d0,d1
		bge.s	.achete
		* le joueur n'a pas assez de crédit *
		lea	(text41-t,a5),a0
		bsr	put_ze_text
		moveq	#1,d7
.achete		tst.l	d7
		rts

************* Test si le joueur a achété moins de 20 gadgets **************
* Sortie 	<= d7=0 moins de 20 gadgets
sh_20gadgets:
		moveq	#0,d7
		cmp.w	#NOPTIONS,(bonus.achete-t,a5)	plus de 20 gagdets ?
		blt.s	.exit			-> non
		lea	(text63-t,a5),a0
		bsr	put_ze_text
		moveq	#1,d7
.exit		tst.l	d7
		rts

*---------------------* Ajoute Gagdet dans la liste *---------------------*
* Entrée	=> d6.w	sh.pt
sh.ajoute_gad:
		move.w	(bonus.achete-t,a5),d0
		addq.w	#1,(bonus.achete-t,a5)
		lea	(course-t,a5,d0*2),a0	liste gagdet
		lea	(curs_dst-t,a5,d6),a1
		move.w	(2,a1),(a0)
		lea	(sh.tablept-t,a5,d0*2),a0
		move.w	d6,(a0)
		rts

**************** soustrait le prix ascii au crédit ascii ******************
* Entrée	=> d6.w	sh.pt
sh_achete1:
		lea	(curs_pri2-t,a5),a1
		tst.w	(sh.prix-t,a5)
		beq.s	.nolaon
		lea	(tcurs_pri2-t,a5),a1
.nolaon		add.w	d6,a1
		move.l	(a1),a2
		add.l	a5,a2
		lea	(credit0-t,a5),a0
		move.w	(12,a2),d0
		sub.w	d0,(12,a0)
		moveq	#9,d4
		lea	(prix0-t,a5),a0		pt/prix unité
		lea	(credit0-t,a5),a1	pt/crédit unité
		moveq	#5,d3
.ac_loop	moveq	#0,d2
		move.w	(a0),d2
		tst.w	d2
		beq	.ac_sarah
		subq	#1,d2
.ac_unite	tst.w	(a1)
		beq.s	.ac_subdiz
		subq.w	#1,(a1)
		bra	.ac_dizaine
.ac_subdiz	tst.w	(2,a1)
		beq.s	.ac_subcent
		move.w	d4,(a1)
		subq.w	#1,(2,a1)
		bra.s	.ac_dizaine
.ac_subcent	tst.w	(4,a1)
		beq.s	.ac_submille
		move.w	d4,(a1)
		move.w	d4,(2,a1)
		subq.w	#1,(4,a1)
		bra.s	.ac_dizaine
.ac_submille	tst.w	(6,a1)
		beq.s	.ac_subdixmille
		move.w	d4,(a1)
		move.w	d4,(2,a1)
		move.w	d4,(4,a1)
		subq.w	#1,(6,a1)
		bra.s	.ac_dizaine
.ac_subdixmille	tst.w	(8,a1)
		beq.s	.ac_subcentmill
		move.w	d4,(a1)
		move.w	d4,(2,a1)
		move.w	d4,(4,a1)
		move.w	d4,(6,a1)
		subq.w	#1,(8,a1)
		bra.s	.ac_dizaine
.ac_subcentmill	move.w	d4,(a1)
		move.w	d4,(2,a1)
		move.w	d4,(4,a1)
		move.w	d4,(6,a1)
		move.w	d4,(8,a1)
		subq	#1,(10,a1)
.ac_dizaine	dbf	d2,.ac_unite
.ac_sarah	addq.l	#2,a0
		addq.l	#2,a1
		dbf	d3,.ac_loop
		rts
*-------------------------------------------------------------------------*
**************************** Sort du magasin ******************************
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
auchan:
		tst.w	(sh.optinfo-t,a5)	info ?
		bne.s	.lilijgf		-> oui
		tst.w	(shopout-t,a5)		flag sort magasin ?
		beq.s	.lilijgf		-> non
		clr.w	(sh.prix-t,a5)
		bsr	nextplayer
		tst.l	d7			encore un joueur ?
		bne.s	.liberte		-> non
		bsr	kill_screen
		move.w	#2,(rout-t,a5)		>flag prepare magasin<
		bra.s	.dirqfin

.liberte	bsr	firstplayer
		bsr.l	go_fade_off
.test786	tst.w	(flag_fade_off-t,a5)
		bne.s	.test786
		bsr.s	rupt3.off
		clr.w	(rout-t,a5)		>flag prépare level<
.dirqfin	move.l	#irq,(a7)
.lilijgf	rts

*/---------------------------/Libère mémoire/----------------------------/*
rupt3.off	bsr.s	.freemem	
		bne.l	sys_fin
		rts
.freemem	bsr	bob32.freemem2
		bne.s	.exit	
		bsr	kill_screen
		bne.s	.exit	
		lea	libere_mem,a4
		lea	(shop.sprite-t,a5),a0	ANIM. SPRITE CURSEUR SOURIS
		jsr	(a4)
		bne.s	.exit	
		lea	(hedgehog-t,a5),a0	ROUTINE PRINT
		jsr	(a4)
		bne.s	.exit	
		lea	(pt_gadget-t,a5),a0	STRUCTURE BOB GADGETS
		jsr	(a4)
		bne.s	.exit	
		moveq	#0,d7
.exit		rts

***************************************************************************
****************************** AFFICHAGE LED ******************************
***************************************************************************
* Entrée	=> a5.l	Adr. 't'
kurzor:
		move.l	(bob.led-t,a5),a1
		clr.l	(a1)
		move.w	#-1,(sh.pt-t,a5)
		tst.w	(sh.get-t,a5)
		bne.s	.exit
		*/ calcul pos. led par rapport au pt souris */
		moveq	#0,d1
		move.w	(sh.mousex-t,a5),d1	d1=coordonnée x souris
		cmp.w	#287,d1
		bhi	.exit
		divu	#48,d1			une case tous les 48 pixels
		lsl.w	#2,d1			*4 un long mot par case
		moveq	#0,d0
		move.w	(sh.mousey-t,a5),d0	pos y souris
		divu	#48,d0			/48 : une case ts les 48 lignes
		mulu	#4*6,d0			6 long mot par ligne de case
		and.w	#$FFFC,d0
		add.w	d0,d1
		move	d1,(sh.pt-t,a5)
		*/ Donne les bonnes coordonnées du LED-BOB */
		move.w	(sh.pt-t,a5),d0
		lea	(curs_pos-t,a5,d0),a0
		move.l	(a0),(BOB.POSX,a1)
		move.l	#'BOB1',(a1)
.exit		rts

*=========================================================================*
*==========================AFFICHE LIGNES TEXTES==========================*
*=========================================================================*
* Entrée	=> a5.l	Adr. 't'
rexattitude:
		move.l	(sh.line1-t,a5),a0
		moveq	#8,d0
		move.w	#196,d1
		moveq	#21,d2
		bsr	print_xchar
		move.l	(sh.line2-t,a5),a0
		moveq	#8,d0
		move.w	#206,d1
		moveq	#21,d2
		bsr	print_xchar
		move.l	(sh.line3-t,a5),a0
		moveq	#8,d0
		move.w	#216,d1
		moveq	#21,d2
		bsr	print_xchar
		rts

*=========================================================================*
*********************ANIMATION SPRITE POINTEUR SOURIS**********************
*=========================================================================*
* Entrée	=> a5.l	Adr. 't'
sh.mercryl:
		*/ Animation */
		move.w	(sh.ptanim2-t,a5),d0	d0=pt/animation
		subq.w	#1,(sh.tempo2-t,a5)	tempo=tempo-1
		bne.s	.noanim			-> tempo > 0
		move.w	#5,(sh.tempo2-t,a5)
		addq.w	#8,(sh.ptanim2-t,a5)
.noanim		cmp.w	#7*8,d0			pt < au pt maxi ?
		bne.s	.no_maxi		-> oui
		moveq	#0,d0
		move.w	d0,(sh.ptanim2-t,a5)
.no_maxi	lea	(shop.lutin-t,a5),a2
		lea	(table_lutins-t,a5),a3
		add.w	d0,a2
		move.l	(a2)+,a0
		move.l	(a2),a1
		move.l	a0,(a3)+ 
		move.l	a1,(a3)+
		tst.w	(editor-t,a5)		Editeur en cours ?
		bne.s	.noed			-> oui
		move.l	a0,(a3)+ 
		move.l	a1,(a3)+
		move.l	a0,(a3)+ 
		move.l	a1,(a3)+
		move.l	a0,(a3)+ 
		move.l	a1,(a3)
.noed		move.l	(pt_screen-t,a5),a2
		move.w	(SCOORDX,a2),d6
		move.w	(SCOORDY,a2),d7
		moveq	#16,d2
		moveq	#0,d0
		not.w	(sh.clito-t,a5)
		beq.s	.ok
		tst.w	(sh.optinfo-t,a5)
		beq.s	.ok
		move.w	#-65,d0
		bra.s	.yo
.ok		move.w	(sh.mousex-t,a5),d0
.yo		moveq	#0,d1
		move.w	(sh.mousey-t,a5),d1
		bsr	calcul_mot_ctrl
		move.l	d3,(a0)
		or.w	#$80,d3
		move.l	d3,(a1)
		rts

****************** Test si on entre le code du Trainer ********************
trainer_test:
		tst.w	(sh.code-t,a5)		code entrée ?
		beq.s	.exit
		tst.w	(sh.mousex-t,a5)
		bne.s	.exit
		move.l	(atable_clavier-t,a5),a0
		cmp.w	#1,(-2,a0)			Une touche enfoncée ?
		bne.s	.exit				-> non, sort
		move.w	(-4,a0),d0			d0=Code RAW clavier
		lea	(.last,pc),a1
		cmp.w	(a1),d0				Même code clavier ?
		beq.s	.exit				-> oui, sort
		move.w	d0,(a1)				Sauve code clavier
		move.l	(triche-t,a5),d1
		lsl.l	#8,d1
		move.b	d0,d1
		move.l	d1,(triche-t,a5)
.exit		rts
.last		dc.w	0

***************************************************************************
*--------------------*Pointe les 3 lignes du texte*-----------------------*
***************************************************************************
put_ze_text:
		lea	(sh.line1-t,a5),a1
		move.l	a0,(a1)+
		lea	(SH.TEXT,a0),a0
		move.l	a0,(a1)+
		lea	(SH.TEXT,a0),a0
		move.l	a0,(a1)
		lea	(SH.TEXT,a0),a0
		rts

***************************************************************************
*-----------------------> ON BALLADE LES GADGETS <------------------------*
***************************************************************************
sh.ballade:
		move.l	(bob.get-t,a5),a4
		clr.l	(a4)
		tst.w	(sh.get-t,a5)
		beq	.veux

		*/ Le gadget se balade avec la souris */
		move.w	#-1,(sh.pt-t,a5)
		tst.b	(mouse0_left-t,a5)
		beq.s	.lache
		move.l	(bob.get-t,a5),a4
		move.w	#1,(a4)
		move.l	(sh.mousex-t,a5),(BOB.POSX,a4)
		move.l	(bob.cli-t,a5),a4
		not.w	(a4)			Gadget clignote
		bra	.exit

		*/ Vire le Gadget */
.lache		move.l	(bob.get-t,a5),a4
		clr.l	(a4)
		clr.w	(sh.get-t,a5)
		bsr	sh.collision
		beq.s	.insert
		clr.w	(sh.pt-t,a5)
		move.l	(sh.course-t,a5),a0
		move.l	a0,a1
		clr.w	(a0)+
.l1		move.w	(a0),d0
		bmi.s	.ex
		clr.w	(a0)+
		move.w	d0,(a1)+
		bra.s	.l1
.ex		bsr	sh_vend
		move.w	#-1,(sh.ptget-t,a5)
		bra	.exit

		*/ Insert un Gadget */
.insert		move.w	#-1,(sh.ptget-t,a5)
		lea	(course-t,a5),a0
		lea	(sh.tempo-t,a5),a1
		move.l	(sh.course-t,a5),a2
		move.w	(a2),d1			d1=code source
		move.w	#NOPTIONS-1,d0
		cmp.l	a2,a3			destination=source ?
		beq	.exit
.lozz1		cmp.l	a0,a2			emplacement source ?
		bne.s	.no1
		addq.w	#2,a0
		bra.s	.nx
.no1		cmp.l	a0,a3			emplacement destination ?
		bne.s	.no2
		cmp.l	a3,a2
		bhi.s	.ova	
		move.w	(a0)+,(a1)+
		subq.w	#1,d0
		bmi.s	.xxx
.ova		move.w	d1,(a1)+
		subq.w	#1,d0
		bmi.s	.xxx
.no2		move.w	(a0)+,(a1)+
.nx		dbf	d0,.lozz1
.xxx
		lea	(sh.tempo-t,a5),a0
		lea	(course-t,a5),a1
		move.w	#NOPTIONS-1,d0
.lozz		move.w	(a0)+,(a1)+
		dbf	d0,.lozz
		bra.s	.exit

		*/ Prend un gadget */
.veux		move.w	#-1,(sh.ptget-t,a5)
		tst.b	(mouse0_left-t,a5)
		beq	.exit
		bsr	sh.collision
		bne	.exit
		move.w	(a2),(sh.ptget-t,a5)
		move.l	a3,(sh.course-t,a5)
		move.l	a4,(bob.cli-t,a5)
		clr.l	(a4)
		move.l	(bob.get-t,a5),a3
		move.w	(GAD.PTBOB,a4),(GAD.PTBOB,a3)
		move.w	#1,(sh.get-t,a5)
		move.w	#-1,(sh.pt-t,a5)
.exit		rts

*-----> Collision entre le pointeur souris et la liste des gadgets <------*
* Sortie	<= d7.w	=0 Pointeur souris sur un gadget de la liste
*		<= a2.l pt.table 'sh.tablept'
*		<= a3.l	pt/table 'course'
*		<= a4.l	pt/BOB
sh.collision:
		lea	(sh.tablept-t,a5),a2
		lea	(course-t,a5),a3
		move.l	(pt_gadget-t,a5),a4
		lea	(GAD.SIZE,a4),a4
		move.w	(sh.mousex-t,a5),d0
		move.w	d0,d2
		sub.w	#15,d2
		move.w	(sh.mousey-t,a5),d1
		move.w	d1,d3
		subq.w	#7,d3
		moveq	#NGAD-3,d7
.loop9		tst.w	(a4)
		beq.s	.next
		move.l	(BOB.POSX,a4),d4
		cmp.w	d3,d4			POSY+7 < MOUSEY ?
		blt.s	.next
		sub.w	d1,d4			POSY > MOUSEY ?
		bhi.s	.next
		swap	d4
		cmp.w	d2,d4			POSX+15 < MOUSEX ?
		blt.s	.next
		sub.w	d0,d4			POSX < MOUSEX ?
		bhi.s	.next
		moveq	#0,d7
		bra.s	.exit
.next		lea	(GAD.SIZE,a4),a4
		addq.l	#2,a2
		addq.l	#2,a3
		dbf	d7,.loop9
.exit		tst.l	d7
		rts

**************** soustrait le prix ascii au crédit ascii ******************
sh_vend:
		subq.w	#1,(bonus.achete-t,a5)
		lea	(curs_pri2-t,a5),a1
		tst.w	(sh.prix-t,a5)
		beq.s	.nolaon
		lea	(tcurs_pri2-t,a5),a1
.nolaon		add.w	(sh.ptget-t,a5),a1
		move.l	(a1),a2
		add.l	a5,a2
		lea	(credit0-t,a5),a0
		move.w	(12,a2),d0
		add.w	d0,(12,a0)
		moveq	#0,d4
		lea	(prix0-t,a5),a0		pt/prix unité
		lea	(credit0-t,a5),a1	pt/crédit unité
		moveq	#5,d3
.ac_loop	moveq	#0,d2
		move.w	(a0),d2
		tst.w	d2
		beq	.ac_sarah
		subq.w	#1,d2
.ac_unite	cmp.w	#9,(a1)
		beq.s	.ac_subdiz
		addq.w	#1,(a1)
		bra	.ac_dizaine
.ac_subdiz	cmp.w	#9,(2,a1)
		beq.s	.ac_subcent
		move.w	d4,(a1)
		addq.w	#1,(2,a1)
		bra.s	.ac_dizaine
.ac_subcent	cmp.w	#9,(4,a1)
		beq.s	.ac_submille
		move.w	d4,(a1)
		move.w	d4,(2,a1)
		addq.w	#1,(4,a1)
		bra.s	.ac_dizaine
.ac_submille	cmp.w	#9,(6,a1)
		beq.s	.ac_subdixmille
		move.w	d4,(a1)
		move.w	d4,(2,a1)
		move.w	d4,(4,a1)
		addq.w	#1,(6,a1)
		bra.s	.ac_dizaine
.ac_subdixmille	cmp.w	#9,(8,a1)
		beq.s	.ac_subcentmill
		move.w	d4,(a1)
		move.w	d4,(2,a1)
		move.w	d4,(4,a1)
		move.w	d4,(6,a1)
		addq.w	#1,(8,a1)
		bra.s	.ac_dizaine
.ac_subcentmill	move.w	d4,(a1)
		move.w	d4,(2,a1)
		move.w	d4,(4,a1)
		move.w	d4,(6,a1)
		move.w	d4,(8,a1)
		addq.w	#1,(10,a1)
.ac_dizaine	dbf	d2,.ac_unite
.ac_sarah	addq.l	#2,a0
		addq.l	#2,a1
		dbf	d3,.ac_loop
		rts

*--------> Si la musique magasin est terminé alors on recommence <--------*
testendmod:	
		lea	(Variables+MT_SongPos-t,a5),a0
		tst.b	(a0)			Position 0=musique JEU !
		bne.s	.exit
		move.l	(pt_poz-t,a5),a0	adr. table pos. du module
		move.w	(8,a0),d0		pos. musique SHOP
		bsr.l	np_change		|-> position du magasin
.exit		rts
