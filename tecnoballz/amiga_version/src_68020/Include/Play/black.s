*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*
*********************** ROUT=11 : gardien terminé *************************
*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*
* Entrée	=> a5.l Adr. 't'
black:
		*/ test si d'autres joueurs */
		bsr	nextplayer
		bne.s	.black0			-> pas de prochain joueur
		bsr	rupt4.off
		move.w	#4,(rout-t,a5)		>flag prepare monstre<
		bra	irq

		*/ test si tous les joueurs sont morts */
.black0		bsr	firstplayer
		bne	.totalgameover2		-> oui
		bsr	super_famicom

		*/ incrèmente pointeur sur la table gardien */
		addq.w	#4,(gd.pt-t,a5)
		cmp.w	#GARD_FF+4,(gd.pt-t,a5)
		blt.s	.benoid
		clr.w	(gd.pt-t,a5)
.benoid
		*/ incrèmente n° area + init n° tableau */
		cmp.w	#6,(level-t,a5)		gardien intermediaire
		bne.s	.black1			-> non
		bsr	level_plus
		add.w	#TB.SIZE,(niv00-t,a5)
		bra	.poche_end
.black1		clr.w	(niv00-t,a5)		raz pt/level

		*/ incrèmente Area & Tableaux */
		cmp.w	#5,(area-t,a5)		dernière area ?
		bne.s	.no_last_area		-> non
		cmp.w	#12,(level-t,a5)	avant dernier level ?
		blt.s	.noraa			-> non
		bsr	level_plus
		bsr	rupt4.off
		bsr	super_famicom
		move.w	#4,(rout-t,a5)		>flag prepare monstre<
		bra	irq
.no_last_area	addq.w	#1,(area-t,a5)		n° area suivante
		clr.w	(lev1-t,a5)		raz n° tableau dizaine
		moveq	#1,d0
		move.w	d0,(lev0-t,a5)		n° tableau unité à 1
		move.w	d0,(level-t,a5)		tableau n° 1
.noraa
		*/ Détermine les murs cassables */
		IFEQ	A_TAB
		moveq	#1,d1
		move.w	(area-t,a5),d0		n° de l'area (varie de 1 à 5)
		move.w	d1,(mur.kas_up-t,a5)	flag mur haut cassable
		cmp.w	#2,d0			area 2 ?
		beq.s	.poche_end		-> oui
		move.w	d1,(mur.kas_rg-t,a5)	flag mur droit cassable
		cmp.w	#3,d0			area 3 ?
		beq.s	.poche_end		-> oui
		move.w	d1,(mur.kas_lf-t,a5)	flag mur gauche cassable
		ENDC

.poche_end	bsr	rupt4.off
		IFEQ	A_TAB
		bsr	glockenspiel
		ENDC
		move.l	(pt_poz-t,a5),a0
		move.w	(8,a0),d0		Pos. magasin
		bsr.l	np_change		|-> change de position
		clr.w	(music.on-t,a5)		autorise saut à la playroutine
		move.w	#1,(music34-t,a5)	flag musique 4 cannaux
		bsr	super_famicom
		move.w	#2,(rout-t,a5)		>flag prépare magasin<
		bra	irq

		*/ tous les joueurs sont définitivement mort */
.totalgameover2	clr.w	(tecnowinner-t,a5)
		bsr.s	rupt4.off
		move.w	#6,(rout-t,a5)		>flag prépare intro<
		bra	irq

*«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«*
*---------------* LIBERE MEMOIRE OCCUPEE PAR LES GARDIENS *---------------*
*«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«»-«*
* Entrée	=> a5.l Adr. 't'
rupt4.off:
		bsr.s	.freemem
		bne.l	sys_fin
		rts
.freemem	bsr.l	np_end
		bne	.exit
		bsr.l	kill_screen
		bne	.exit	
		bsr	sbob.freemem2
		bne	.exit	
		bsr	ssha.freemem
		bne	.exit
		lea	libere_mem,a4
		lea	(pt_boule-t,a5),a0	Shape boules TecnoWinner
		jsr	(a4)
		bne	.exit
		lea	(go.beast-t,a5),a0	Liste Lutin HiScores-Table
		jsr	(a4)
		bne	.exit
		lea	(dalma.pagegfx-t,a5),a0
		jsr	(a4)
		bne	.exit
		lea	(mapeditor-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		lea	(missile-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		lea	(explomon-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		lea	(sc_mapadr-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		lea	(gd1-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		lea	(gd2-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		lea	(gd1.lutin-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		lea	(gd2.lutin-t,a5),a0
		jsr	(a4)
		bne.s	.exit
		lea	(pt_balle-t,a5),a0	Structures Balles
		jsr	(a4)
		bne.s	.exit
		lea	(pt_capsule-t,a5),a0	Structures Capsules
		jsr	(a4)
		bne.s	.exit
		lea	(ss.credit-t,a5),a0	SuperSprite Crédit
		jsr	(a4)
		bne.s	.exit
		lea	(ss.life-t,a5),a0	SuperSprite Vies
		jsr	(a4)
		bne.s	.exit
		lea	(gd.lutin45-t,a5),a0	Sprite NomJoueur
		jsr	(a4)
		bne.s	.exit
		lea	(blitz.lutin-t,a5),a0
		jsr	(a4)			7 Lutins GigaBlitz
		bne.s	.exit
		lea	(pt_gadget-t,a5),a0
		jsr	(a4)			Gadgets
		bne.s	.exit
		lea	(pt_bobtext-t,a5),a0	Lettres LEVEL xx COMPLETED
		jsr	(a4)
		bne.s	.exit
		moveq	#0,d7
.exit		rts
