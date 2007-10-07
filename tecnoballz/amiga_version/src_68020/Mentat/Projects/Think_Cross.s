*=-----------------------------------------------------------------------=*
* » THINK-CROSS « (Version Amiga AGA d'après version ST)
* Essai de convertion par Bruno Ethvignot
* Start-Programming => 06-06-95
*=-----------------------------------------------------------------------=*
		SECTION	ETB,CODE
		OPT	P+		pas de table de relogement
		OPT	C20+		labels et symboles 20 caractères maxi.,  majuscules/minuscules respectées
		OPT	O-		pas d'optimisation
		OPT	p=68020		assemble les instructions MC68020
		INCDIR	DH0:Data/

*>>>>>>>>>>>>>>>>>>>>>>>>>>* CONSTANTES MENTAT *<<<<<<<<<<<<<<<<<<<<<<<<<<*
NReservation	=50	50 adresses mémoires réservées maximums
FLAG_AMIGADOS	=1	1=assemble routines chargement AmigaDOS
FLAG_BOB32	=0	1=assemble routines BOBs (aff. écran 32 bits)
FLAG_BOBSTRUCT	=0	1=Assemble la structure BOB
FLAG_DMACON.MSK	=0	1=ferme dma bitplane lors d'un accès disque-système
FLAG_EXIT	=0	0=sortie sous système possible avec la touche "ESC"
FLAG_FREQ	=0	1=assemble routine demande fréquence balayage
FLAG_ILBM	=1	1=assemble routines décompactage IFF-ILBM
FLAG_KEY	=1	1=assemble routines de test du clavier
FLAG_LUTIN	=0	1=assemble routines gestion des lutins
FLAG_MEMORY	=1	1=information sur la mémoire possible
FLAG_MOUSE0	=0	1=assemble la routine déplacement souris port 0
FLAG_PP		=0	1=assemble routines décompactage PowerPacker

*>>>>>>>>>>>>>>>>>>>>>>>>>* Debut code MC68020 *<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	INCLUDE	"Mentat/Include/mentat.s"
msg.pers	dc.b	"                                           "	¤-128
		EVEN

************* routine appelée avant de quitter le multitâche **************
sys_extra:
		lea	(.name2,pc),a0
		lea	(page-t,a5),a4
		bsr.l	charge.page		|-> Charge les blocks
		bne.s	.exit
		lea	(.name1,pc),a0
		lea	(ilbm-t,a5),a4
		bsr.l	charge.fichier		|-> Charge tableau
		bne.s	.exit
		moveq	#0,d7
.exit		rts
.name1		dc.b	"ThinkCross:ThinkCro.ilbm",0
.name2		dc.b	"ThinkCross:ThinkBlk.ilbm",0
		EVEN
sys_extra2:	rts


*>>>>>>>>>>>>>>>>>>>>>>>>>* début du programme *<<<<<<<<<<<<<<<<<<<<<<<<<<*
* Entrée	a5.l	=> adr. 't'
*		a6.l	=> CUSTOM
run:		bsr	first_init		|-> Init. VBL / CIA / Clavier
		bsr	sprites_off		|-> Vire les sprites
		bsr	cree_blocks		|-> cree table pt page blok
		bne	sys_erreur
		bsr	install_ecran		|-> Init. écran
		bne	sys_erreur
		bsr	genere_affiche		|-> Créé routine d'affichage
		bne	sys_erreur
		bra	pres			-> JEU ATARI-ST

*×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×*
******** Routine appelée par l'interruption du temps mort vertical ********
*×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×*
* Entrée	a5.l	=> Adr. 't'
*		a6.l	=> CUSTOM
extra_vbl:	bsr	vbl			|-> Routine ATARI-ST
		bsr	test_keys
		rts

*×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×*
*------------*  Créé une table pointant sur tous les blocks  *------------*
*×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×*
* Entrée	=> a5.l	adr. 't'
* Sortie	<= d7.l 0 / Z=1 pas d'erreur
cree_blocks:

		*/ Réserve la table en mémoire */
		move.l	(page-t,a5),a4		a4=page gfx des blocks
		move.w	(PG_SPLAN,a4),d0	d0=Largeur page en octets
		lsr.w	#1,d0			Nbre de blocks horzizontals
		move.w	(PG_HEIGHT,a4),d1	d1=Hauteur page en lignes
		lsr.w	#4,d1			Nbre de blocks verticals
		move.w	d0,d2
		mulu	d1,d2			d2=Nbre total de blocks
		mulu	#4,d2			Un long mot par blocks
		lea	(block-t,a5),a4		pt/table
		move.l	d2,(RESA.SIZ,a4)	Taille mémoire à réserver
		bsr	reserve.mem		|-> Reserve mémoire
		bne.s	.exit			-> erreur

		*/ Calcul la table */
		move.l	(page-t,a5),a4		a4=page gfx des blocks
		move.w	(PG_SLINE,a4),d2	d2=taille ligne (RAW-BLITTER)
		mulu	#16,d2
		lea	(PG_DATA,a4),a4		a4=Adr. données page
		move.l	(block-t,a5),a0		a0=pt/table
		subq.w	#1,d0			d0=Nbre de blocks horzizontals
		subq.w	#1,d1			d1=Nbre de blocks verticals
.loopy		move.l	a4,d3
		move.w	d0,d4			d4=Nbre de blocks horizontals
.loopx		move.l	d3,(a0)+
		addq.l	#2,d3
		dbf	d4,.loopx
		add.l	d2,a4
		dbf	d1,.loopy
		moveq	#0,d7
.exit		rts

*×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×*
************************* Installe l'écran de jeu *************************
*×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×*
* Entrée	a5.l	=> adr. table 't'
* Sortie	<= d7.l 0 / Z=1 pas d'erreur
install_ecran:
		lea	(pt_screen-t,a5),a4
		bsr	screen_res
		move.w	#3,(SFMODE_PLANS,a0)	mode Brust AGA
		move.w	#3,(SFMODE_LUTINS,a0)	mode Burst AGA
		move.w	#1,(SPBUFFER,a0)	nombre de buffer écran (3 maxi.)
		move.w	#3,(SFADEONSPEED,a0)
		move.w	#3,(SFADEOFFSPEED,a0)
		move.w	#2,(SFREQ,a0)		Force 60Hz
		move.l	(ilbm-t,a5),d0
		bsr	affiche_image_ilbm
		bne.s	.exit
		move.l	(pt_screen-t,a5),a0
		move.l	(SPECRAN1,a0),(ecran1-t,a5)
		move.w	(SP_SLINE,a0),d0
		mulu	#16,d0
		move.l	d0,(ecran.sline-t,a5)
		add.w	#12,d0
		add.l	(SPECRAN1,a0),d0
		move.l	d0,(ecran.offset-t,a5)
		moveq	#0,d7
.exit		rts

*«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«*
*----------------* Genère code 68020 routine d'affichage *----------------*
*«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«×»«*
* Créée routine d'affichage (code généré)
* Se créé en fonction des caracteristiques de l'écran & et de la page graphique
* On peut passer de 16 couleurs à 64, 128 ou 256 en modifiant uniquement
* les fichiers IFF-ILBM !
genere_affiche:
		move.l	(pt_screen-t,a5),a0
		move.l	(page-t,a5),a2

		*/ Routine affichage block dans le tableau */
		lea	(write1-t,a5),a4
		moveq	#6,d0
		bsr	generic
		bne.s	.exit
		move.l	(ecran.offset-t,a5),(a1)+
		bsr	genere_x
		bsr	genere_copie

		*/ Routine affichage écran 16x16 transparent */
		lea	(write2-t,a5),a4
		moveq	#8,d0
		bsr	genere_3
		bne.s	.exit
		bsr	genere_transparent

		*/ Routine affichage écran 16x16 du masque */
		lea	(write3-t,a5),a4
		moveq	#12,d0
		bsr	genere_3
		bne.s	.exit
		bsr	genere_msk

		*/ routine affichage écran 16x16 du block */
		lea	(write4-t,a5),a4
		moveq	#6,d0
		bsr	genere_3
		bne.s	.exit
		bsr	genere_copie
		moveq	#0,d7
.exit		rts

*-------------------* Reserve mémoire pour la routine *-------------------*
* Entrée	=> d0.l	Taille d'une instruction
*		=> a0.l	Structure écran
*		=> a2.l pt/page gfx
*		=> a4.l	Structure RESA
* Sortie	<= d0.w	Taille plan source
*		<= d1.w Taille plan destination
*		<= d2.w	Taille plan source
*		<= d3.w Taille plan destination
*		<= d4.w Nbre de plans * nbre de lignes - 2
*		<= d7.l	#0 = pas d'erreur
generic:
		mulu.w	#16,d0
		mulu.w	(SPPLAN,a0),d0
		add.w	#28,d0
		move.l	d0,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne.s	.exit	
		move.l	(a4),a1
		move.w	(SP_SPLAN,a0),d1
		move.w	(PG_SPLAN,a2),d0
		move.w	d0,d2
		move.w	d1,d3
		move.w	(SPPLAN,a0),d4
		mulu	#16,d4
		subq.w	#2,d4
		move.w	#$41F9,(a1)+			lea ??,a0
		moveq	#0,d7
.exit		rts

		*/ Code qui calcul adresses d'affichage */
genere_x:
		move.l	#$D0C0D0C0,(a1)+		2X "adda.w d0,a0"
		move.w	#$3801,(a1)+			"move.w	 d1,d4"
		move.w	#$C8FC,(a1)+			"mulu.w #??,d4"
		move.w	(2+ecran.sline-t,a5),(a1)+
		move.w	#$D1C4,(a1)+			"add.l d4,a0"
		move.w	#$43F9,(a1)+			"lea	??,a1"
		move.l	(block-t,a5),(a1)+
		move.l	#$22712400,(a1)+		"movea.l (a1,d2*4),a1"
		rts
genere_3:
		bsr	generic
		bne.s	.exit
		move.l	(ecran1-t,a5),(a1)+
		bsr	genere_x
		moveq	#0,d7
.exit		rts


*-----------------* Genère code copie simple (a1),(a0) *------------------*
* Entrée	=> d0.w	Taille plan source
*		=> d1.w Taille plan destination
*		=> d2.w	Taille plan source
*		=> d3.w Taille plan destination
*		=> d4.w Nbre de plans * nbre de lignes - 2
*		=> a1.l Adresse mémoire routine
genere_copie:
		move.w	#$3091,(a1)+	+2	"move.w	(a1),(a0)"
.loop		move.w	#$3169,(a1)+	+6*np-1	"move.w	(??,a1),(??,a0)"
		move.w	d2,(a1)+
		move.w	d3,(a1)+
		add.w	d0,d2
		add.w	d1,d3
		dbf	d4,.loop
		move.w	#$4E75,(a1)		"rts"
		rts

genere_transparent:
		move.w	#$3811,(a1)+		"move.w (a1),d4"
		move.w	#$8950,(a1)+		"or.w d4,(a0)"
.loop		move.w	#$3829,(a1)+		"move.w (??,a1),d4"
		move.w	d2,(a1)+
		move.w	#$8968,(a1)+		"or.w d4,(??,a0)"
		move.w	d3,(a1)+
		add.w	d0,d2
		add.w	d1,d3
		dbf	d4,.loop
		move.w	#$4E75,(a1)		"rts"
		rts
genere_msk:
		move.w	#$3811,(a1)+		"move.w (a1),d4"
		move.w	#$8950,(a1)+		"or.w d4,(a0)"
		move.w	#$B950,(a1)+		"eor.w d4,(a0)"
		moveq	#16-2,d4
		move.w	(SPPLAN,a0),d5
		subq.w	#1,d5
		move.w	d5,d6
		beq.s	.next
		subq.w	#1,d6
		bra.s	.loop2
.loop1		move.w	#$3829,(a1)+		"move.w (??,a1),d4"
		move.w	d2,(a1)+
		move.w	d5,d6
.loop2		move.w	#$8968,(a1)+		"or.w d4,(??,a0)
		move.w	d3,(a1)+
		move.w	#$B968,(a1)+		"eor.w d4,(??,a0)	
		move.w	d3,(a1)+
		add.w	d0,d2
		add.w	d1,d3
		dbf	d6,.loop2
.next		dbf	d4,.loop1
		move.w	#$4E75,(a1)		"rts"
		rts


*×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×*
*»»»»»»»»»»* Test les touches claviers avec "Repeat" & "Delay" *««««««««««*
*×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×x×*
* Entrée	=> a5.l	Adr. 't'
test_keys:
		move.l	(atable_clavier-t,a5),a0
		lea	(test_clavier,pc),a2
		lea	(TOUCHE_GAUCHE,a0),a1
		jsr	(a2)
		move.b	d0,(gauche.key-t,a5)
		lea	(TOUCHE_DROITE,a0),a1
		jsr	(a2)
		move.b	d0,(droite.key-t,a5)
		lea	(TOUCHE_BAS,a0),a1
		jsr	(a2)
		move.b	d0,(bas.key-t,a5)
		lea	(TOUCHE_HAUT,a0),a1
		jsr	(a2)
		move.b	d0,(haut.key-t,a5)
		lea	(TOUCHE_ESPACE,a0),a1
		jsr	(a2)
		move.b	d0,(espace.key-t,a5)
		lea	(TOUCHE_RETURN,a0),a1
		jsr	(a2)
		move.b	d0,(return.key-t,a5)
		lea	(TOUCHE_F1,a0),a1
		jsr	(a2)
		move.b	d0,(f1.key-t,a5)
		lea	(TOUCHE_F2,a0),a1
		jsr	(a2)
		move.b	d0,(f2.key-t,a5)
		lea	(TOUCHE_F3,a0),a1
		jsr	(a2)
		move.b	d0,(f3.key-t,a5)
		lea	(TOUCHE_F10,a0),a1
		jsr	(a2)
		move.b	d0,(f10.key-t,a5)
		rts

*«<*>»*«<*>»*«<*>»*«<*>»*«<*>»*«<*>»*«<*>»*«<*>»*«<*>»*«<*>»*«<*>»*«<*>»*«*
*>>>>>>>>>>>>>>>>>>>>>>>>>> DEBUT DU PROGRAMME <<<<<<<<<<<<<<<<<<<<<<<<<<<*
*«<*>»*«<*>»*«<*>»*«<*>»*«<*>»*«<*>»*«<*>»*«<*>»*«<*>»*«<*>»*«<*>»*«<*>»*«*
main_loop:
*----------------------------* Presentation *-----------------------------*
pres:
		moveq	#0,d0
		move.w	d0,(lev-t,a5)
		move.b	d0,(lev1-t,a5)   ?
		move.b	d0,(lev2-t,a5)   ?
		move.b	d0,(lev1-t,a5)   ? 
		move.b	d0,(partie-t,a5) ?
		move.b	#3,(bmb-t,a5)
		move.b	#3,(reco-t,a5)
		move.b	d0,(recom-t,a5)
		move.b	d0,(tim1-t,a5)
		move.b	d0,(tim2-t,a5)
		move.b	d0,(tim3-t,a5)
		move.b	d0,(pts1-t,a5)
		move.b	d0,(pts2-t,a5)
		move.b	d0,(pts3-t,a5)
		move.b	d0,(pts4-t,a5)
		move.b	d0,(pts5-t,a5)
		move.b	d0,(pts6-t,a5)
		move.w	d0,(fix-t,a5)
		move.w	d0,(bomb-t,a5)
		move.b	d0,(jeu-t,a5)
		lea	(present-t,a5),a0
		move.l	a0,(niveau-t,a5)
		bsr	toutab
		bsr	wait
.waitloop	tst.b	(espace.key-t,a5)	Espace = quitte présentation
		bne.s	readi
		tst.b	(f1.key-t,a5)		F1 = entre le code 
		bne.s	entcode
		tst.b	(f10.key-t,a5)		F10 = quitte jeu
		bne	quit
		bra.s	.waitloop

*---------------------------*  Entre le code  *---------------------------*
entcode		lea	(incode-t,a5),a0
		move.l	a0,(niveau-t,a5)
		bsr	toutab

*----------------------------* Ready to Play ? *--------------------------*
readi		lea	(ready-t,a5),a0
		move.l	a0,(niveau-t,a5)
		bsr	copitab
		bsr	toutab
		bsr	affval
		bsr	wait

*--------------------------* Affiche le niveau *--------------------------*
		lea	(screen-t,a5),a0
		move.l	a0,(niveau-t,a5)
		bsr	toutab
		clr.w	(xpteur-t,a5)
		clr.w	(ypteur-t,a5)
		bsr	affcur
		move.b	#1,(jeu-t,a5)
		move.b	#1,(partie-t,a5)

*----------------------------* Boucle du jeu *----------------------------*
sais1:		bsr	wait_vbl
		bsr	memory_list
		bsr	memory_help
		cmp.b	#1,(plusblok-t,a5)
		bne	.curs
		clr.b	(plusblok-t,a5)
		addq.w	#1,(lev-t,a5)
		bra	readi
.curs		tst.b	(return.key-t,a5)		curseur saisie enclenche (return)
		beq	sais2
		cmp.w	#-1,(bomb-t,a5)
		bne	pasbomb
		tst.b	(bmb-t,a5)
		beq	desbomb
		move.w	(xpteur-t,a5),d0
		move.w	(ypteur-t,a5),d1
		bsr	rechblk
		tst.w	d2
		beq	desbomb	
		move.w	d0,(modx-t,a5)
		move.w	d1,(mody-t,a5)
		clr.w	d2
		bsr	modifi
		subq.b	#1,(bmb-t,a5)
		bsr	boncompt
desbomb		not.w	(bomb-t,a5)
		bra	finret
pasbomb		bsr	affcur
		not.w	(fix-t,a5)
finret		nop

sais2		tst.b	(haut.key-t,a5)		curseur vers le haut
		beq.s	sais3
		bsr	mocurh
sais3		tst.b	(bas.key-t,a5)		curseur vers le bas
		beq	sais4
		bsr	mocurb
sais4		tst.b	(droite.key-t,a5)	curseur vers la droite
		beq	sais5
		bsr	mocurd
sais5		tst.b	(gauche.key-t,a5)	curseur vers la gauche
		beq	sais6
		bsr	mocurg
sais6		tst.b	(f2.key-t,a5)		F2 (bombe)
		beq	sais7
		not.w 	(bomb-t,a5)
		bsr	affcur
sais7		tst.b	(f3.key-t,a5)		F3 (recommence)
		beq	sais8
		tst.b	(reco-t,a5)
		beq	finf3
		move.b	#1,(recom-t,a5)
		bsr	copitab
		bsr	toutab
		subq.b	#1,(reco-t,a5)
		bsr	boncompt
		bsr	affcur
finf3		nop
sais8		tst.b	(f10.key-t,a5)		f10  game over
		beq.s	sais9
		bra	gamov
sais9		tst.b	(jeu-t,a5)
		bne	sais1

gamov		lea	(gover-t,a5),a0
		move.l	a0,(niveau-t,a5)
		bsr	toutab	
		clr.b	(jeu-t,a5)
		bsr	wait
		bra	pres
quit		bra	sys_fin

*+«-»+«-»+«-»+«-»+«-»+«-»+«-»+«-»+«-»+«-»+«-»+«-»+«-»+«-»+«-»+«-»+«-»+«-»+*
wait		
.wait		tst.b	(espace.key-t,a5)
		beq.s	.wait
		rts

*=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=*
*------------------* Routine appellées toutes les VBL *-------------------*
*=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=><=*
vbl:		
*------------*/ compte des blocks action present a l'ecran /*-------------*
.compar		lea	(blk1-t,a5),a0
		moveq	#1,d0
.debucompar	move.l	(niveau-t,a5),a1
		move.w	#129,d1
		clr.b	(a0)
.autrecompar	cmp.b	(a1)+,d0
		bne.s	.pascompar
		addq.b	#1,(a0)
.pascompar	dbra	d1,.autrecompar
		addq.b	#1,d0
		addq.l	#1,a0
		cmp.b	#9,d0
		beq	.sorticompar
		bra	.debucompar
.sorticompar	
		tst.b	(partie-t,a5)
		beq	finvbl
		moveq	#0,d7
		move.b	(blk1-t,a5),d7
		add.b	(blk2-t,a5),d7
		add.b	(blk3-t,a5),d7
		add.b	(blk4-t,a5),d7
		add.b	(blk5-t,a5),d7
		add.b	(blk6-t,a5),d7
		add.b	(blk7-t,a5),d7
		add.b	(blk8-t,a5),d7
		tst.b	d7
		bne.s	.encblk
		move.b	#1,(plusblok-t,a5)
		clr.b	(partie-t,a5)
.encblk	
*-----------------------* fliping couleur curseur *-----------------------*
flipcol		tst.w	(fix-t,a5)
		beq	.col1
		lea	(ccolor2-t,a5),a0
		bra	.col2
.col1		lea	(ccolor1-t,a5),a0
.col2		move.w	(ptccol-t,a5),d0
		cmp.w	#24,d0
		bne.s	.flip
		moveq	#0,d0
.flip		move.w	(a0,d0),d1		d1=code couleur ST 
		add.w	d1,d1			*2 = code Amiga (bits 4 à7)
		move.l	(pt_screen-t,a5),a0
		move.w	(SBPLCON3,a0),(BPLCON3,a6) (selectionne bits 4 à 7)
		move.w	d1,(COLOR15,a6)		  
		addq.w	#2,d0
		move.w	d0,(ptccol-t,a5)
	
		move.w	(timer-t,a5),d0
		cmp.w	#50,d0
		bne.s	.ftimer
		clr.w	(timer-t,a5)
		cmp.b	#1,(jeu-t,a5)
		bne.s	.ftimer
		bsr	caltim
		bsr	blkcompt
.ftimer		addq.w	#1,(timer-t,a5)

		move.w	(action-t,a5),d0
		cmp.w	#25,(action-t,a5)
		bne.s	.faction
		clr.w	(action-t,a5)
		cmp.b	#1,(jeu-t,a5)
		bne.s	.faction
		bsr	blkact
		addq.w	#1,(auanit-t,a5)
		cmp.w	#2,(auanit-t,a5)
		bne.s	.faction
		clr.w	(auanit-t,a5)

.faction	addq.w	#1,(action-t,a5)
finvbl		rts


****** Comparaison blocks actions & destruction ceux qui se touchent ******

blkact:		lea	(screen-t,a5),a3
		clr.w	(bactx-t,a5)
		clr.w	(bacty-t,a5)
	
.debcot		cmp.b	#10,(a3)
		bgt	.ani
		bra	.noani

.ani		cmp.b	#37,(a3)
		bne	.autrani
		clr.w	(anicteur-t,a5)
		bne.s	.ani1
		move.w	#37,(anicompt-t,a5)
.ani1		cmp.w	#1,(anicteur-t,a5)
		bne.s	.ani2
		move.w	#38,(anicompt-t,a5)
.ani2		cmp.w	#2,(anicteur-t,a5)
		bne.s	.ani3
		move.w	#39,(anicompt-t,a5)
.ani3		cmp.w	#3,(anicteur-t,a5)
		bne.s	.ani4
		clr.w	(anicompt-t,a5)
	
.ani4		move.w	(bactx-t,a5),d0
		move.w	(bacty-t,a5),d1
		clr.l	d2
		move.w	(anicompt-t,a5),d2
		jsr	([write1-t,a5])	
		bra	.problok	

.autrani	cmp.b	#33,(a3)
		bne	.problok
		clr.w	(anicteur-t,a5)
		bne	.ani5
		move.w	#33,(auani-t,a5)
.ani5		cmp.w	#1,(auanit-t,a5)
		bne	.ani6
		move.w	#34,(auani-t,a5)
.ani6		cmp.w	#2,(auanit-t,a5)
		bne	.ani7
		move.w	#33,(auani-t,a5)
.ani7		move.w	(bactx-t,a5),d0
		move.w	(bacty-t,a5),d1
		clr.l	d2
		move.w	(auani-t,a5),d2
		jsr	([write1-t,a5])	
		bra	.problok
.noani		tst.b	(a3)
		beq	.problok 

.coted		cmp.w	#12,(bactx-t,a5)
		beq	.coteb
		move.l	a3,a4
		clr.l	d7
		move.b	(1,a4),d7
		cmp.b	(a3),d7
		bne	.coteb
		move.b	#1,(cotd-t,a5)
		move.b	(2,a4),d7
		cmp.b	(a3),d7
		bne	.pasext
		move.b	#1,(cotd1-t,a5)
.pasext		move.b	(13,a4),d7
		cmp.b	(a3),d7
		bne	.pasdes
		move.b	#1,(cotd2-t,a5)
.pasdes 	move.b	(14,a4),d7
		cmp.b	(a3),d7
		bne	.coteb
		move.b	#1,(cotd3-t,a5)
	
.coteb		cmp.w	#10,(bacty-t,a5)
		beq	.effcod
		move.l	a3,a4
		clr.l	d7
		move.b	(13,a4),d7
		cmp.b	(a3),d7
		bne.s	.effcod
		move.b	#1,(cotb-t,a5)

.effcod		tst.b	(cotd-t,a5)
		beq	.effcob
		move.l	a3,a4
		clr.b	(1,a4)
		clr.b	(a3)
		move.w	(bactx-t,a5),d0
		move.w	(bacty-t,a5),d1
		moveq	#0,d2
		jsr	([write1-t,a5])
		addq.w	#1,d0
		move.w	(bacty-t,a5),d1
		moveq	#0,d2
		jsr	([write1-t,a5])

		cmp.b	#1,(cotd1-t,a5)
		bne	.neffcd1
		move.l	a3,a4
		clr.b	(2,a4)
		move.w	(bactx-t,a5),d0
		addq.w	#2,d0
		move.w	(bacty-t,a5),d1
		moveq	#0,d2
		jsr	([write1-t,a5])

.neffcd1	cmp.b	#1,(cotd2-t,a5)
		bne	.neffcd2
		move.l	a3,a4
		clr.b	(13,a4)
		move.w	(bactx-t,a5),d0
		move.w	(bacty-t,a5),d1
		addq.w	#1,d1
		moveq	#0,d2
		jsr	([write1-t,a5])

.neffcd2	cmp.b	#1,(cotd3-t,a5)
		bne	.neffcd3
		move.l	a3,a4
		clr.b	(14,a4)
		move.w	(bactx-t,a5),d0
		move.w	(bacty-t,a5),d1
		addq.w	#1,d0
		addq.w	#1,d1
		moveq	#0,d2
		jsr	([write1-t,a5])

.neffcd3 	clr.b	(cotd-t,a5)
		clr.b	(cotd1-t,a5)
		clr.b	(cotd2-t,a5)
		clr.b	(cotd3-t,a5)
	
.effcob		cmp.b	#1,(cotb-t,a5)
		bne	.rienblk
		clr.b	(13,a3)
		clr.b	(a3)
		move.w	(bactx-t,a5),d0
		move.w	(bacty-t,a5),d1
		moveq	#0,d2
		jsr	([write1-t,a5])
		move.w	(bactx-t,a5),d0
		addq.w	#1,d1
		moveq	#0,d2
		jsr	([write1-t,a5])
		clr.b	(cotb-t,a5)

.rienblk	
		cmp.w	#9,(bacty-t,a5)
		beq	.problok
		cmp.b	#32,(26,a3)
		bne	.pasdoor
		tst.b	(13,a3)
		beq	.pasdoor
		tst.b	(26,a3)
		move.w	(bactx-t,a5),d0
		move.w	(bacty-t,a5),d1
		addq.w	#2,d1
		clr.l	d2
		jsr	([write1-t,a5])
	
.pasdoor	cmp.b	#37,(13,a3)
		bne	.paspont
		tst.b	(26,a3)
		bne	.problok
		tst.w	(anicompt-t,a5)
		bne	.problok
		move.w	(bactx-t,a5),d0
		move.w	(bacty-t,a5),d1
		clr.l	d2
		bsr	rechblk
		clr.b	(a3)
		move.b	d2,(26,a3)
		addq.w	#2,d1
		jsr	([write1-t,a5])
		subq.w	#2,d1
		clr.l	d2
		jsr	([write1-t,a5])
		move.w	#15,(action-t,a5)
		rts
		
.paspont	tst.b	(13,a3)
		bne	.problok
		move.w	(bactx-t,a5),d0
		move.w	(bacty-t,a5),d1
		clr.l	d2
		bsr	rechblk
		clr.b	(a3)
		move.b	d2,(13,a3)
		addq.w	#1,d1
		jsr	([write1-t,a5])
		sub.w	#1,d1
		clr.l	d2
		jsr	([write1-t,a5])
		move.w	#15,(action-t,a5)
		rts

.problok	cmp.w	#10,(bacty-t,a5)
		beq	.fintot
		addq.w	#1,(bactx-t,a5)
		addq.l	#1,a3
		cmp.w	#13,(bactx-t,a5)
		bne	.fincot
		clr.w	(bactx-t,a5)
		addq.w	#1,(bacty-t,a5)
.fincot		bra	.debcot
.fintot		bsr	affcur
		cmp.w	#3,(anicteur-t,a5)
		bne	.finani
		clr.w	(anicteur-t,a5)

.finani		rts


*---------------------* move curseur vers la droite *---------------------*
mocurd	
		lea	(screen-t,a5),a0
		move.w	(xpteur-t,a5),d0
		move.w	(ypteur-t,a5),d1
		clr.l	d2
		cmp.w	#12,d0
		beq	finmcud
		addq.w	#1,d0
		bsr	rechblk
		*cmp.b	#10,d2
		*bgt	finmcud
		cmp.b	#-1,(fix-t,a5)
		bne	concurd	
		tst.b	d2
		bne	finmcud
		move.w	(xpteur-t,a5),d0
		move.w	(ypteur-t,a5),d1
		bsr	rechblk
		tst.b	d2
		beq	nofix
		addq.w	#1,d0
		move.w	d0,(modx-t,a5)
		move.w	d1,(mody-t,a5)
		bsr	modifi
		move.w	(xpteur-t,a5),(modx-t,a5)
		move.w	(ypteur-t,a5),(mody-t,a5)
		clr.l	d2
		bsr	modifi
		addq.w	#1,(xpteur-t,a5)
		bsr	blkact
		bra	finmcud
concurd		move.w	(xpteur-t,a5),d0
		move.w	(ypteur-t,a5),d1
		bsr	rechblk
		jsr	([write1-t,a5])
		addq.w	#1,d0
		addq.w	#1,(xpteur-t,a5)
		bsr	rechblk
		jsr	([write1-t,a5])
		bra	finmcud
nofix		not	(fix-t,a5)
finmcud		bsr	affcur
		rts

*---------------------* move curseur vers la gauche *---------------------*
mocurg:
		lea	(screen-t,a5),a0
		move.w	(xpteur-t,a5),d0
		move.w	(ypteur-t,a5),d1
		clr.l	d2
		tst.w	d0
		beq	finmcug
		subq.w	#1,d0
		bsr	rechblk
		*cmp.b	#10,d2
		*bgt	finmcug
		cmp.b	#-1,(fix-t,a5)
		bne	concurg	
		tst.b	d2
		bne	finmcug
		move.w	(xpteur-t,a5),d0
		move.w	(ypteur-t,a5),d1
		bsr	rechblk
		tst.b	d2
		beq	nofix
		subq.w	#1,d0
		move.w	d0,(modx-t,a5)
		move.w	d1,(mody-t,a5)
		bsr	modifi
		move.w	(xpteur-t,a5),(modx-t,a5)
		move.w	(ypteur-t,a5),(mody-t,a5)
		clr.l	d2
		bsr	modifi
		subq.w	#1,(xpteur-t,a5)
		bsr	blkact
		bra	finmcud
concurg		move.w	(xpteur-t,a5),d0
		move.w	(ypteur-t,a5),d1
		bsr	rechblk
		jsr	([write1-t,a5])
		subq.w	#1,d0
		subq.w	#1,(xpteur-t,a5)
		bsr	rechblk
		jsr	([write1-t,a5])
finmcug		bsr	affcur
		rts

*----------------------* move curseur vers le haut *----------------------*
mocurh:
		lea	(screen-t,a5),a0
		move.w	(xpteur-t,a5),d0
		move.w	(ypteur-t,a5),d1
		clr.l	d2
		tst.w	d1
		beq	finmcuh
		subq.w	#1,d1
		bsr	rechblk
		*cmp.b	#10,d2
		*bgt	finmcuh
		cmp.b	#-1,(fix-t,a5)
		bne	concurh	
		not.w	(fix-t,a5)
concurh		move.w	(xpteur-t,a5),d0
		move.w	(ypteur-t,a5),d1
		bsr	rechblk
		jsr	([write1-t,a5])
		subq.w	#1,d1
		subq.w	#1,(ypteur-t,a5)
		bsr	rechblk
		jsr	([write1-t,a5])
finmcuh		bsr	affcur
		rts

*-----------------------* move curseur vers le bas *----------------------*
mocurb:
		lea	(screen-t,a5),a0
		move.w	(xpteur-t,a5),d0
		move.w	(ypteur-t,a5),d1
		clr.l	d2
		cmp.w	#9,d1
		beq	finmcub
		addq.w	#1,d1
		bsr	rechblk
		*cmp.b	#10,d2
		*bgt	finmcub
		cmp.b	#-1,(fix-t,a5)
		bne	concurb	
		not.w	(fix-t,a5)
concurb 	move.w	(xpteur-t,a5),d0
		move.w	(ypteur-t,a5),d1
		bsr	rechblk
		jsr	([write1-t,a5])
		addq.w	#1,d1
		addq.w	#1,(ypteur-t,a5)
		bsr	rechblk
		jsr	([write1-t,a5])
finmcub		bsr	affcur
		rts

*------------------------* affichage du curseur *-------------------------*
affcur:		
		move.w	(xpteur-t,a5),d0
		move.w	(ypteur-t,a5),d1
		addq.w	#6,d0
		addq.w	#1,d1
		cmp.w	#-1,(bomb-t,a5)
		bne.s	.nobomb1
		moveq	#43,d2
		bra	.yebomb1
.nobomb1	moveq	#42,d2
.yebomb1	jsr	([write3-t,a5])
		cmp.w	#-1,(bomb-t,a5)
		bne.s	.nobomb2
		moveq	#43,d2
		bra	.yebomb2
.nobomb2	moveq	#42,d2
.yebomb2 	jsr	([write2-t,a5])
		rts

*------------* modifie le tableau en fonction de la demande *-------------*
* bombe,deplacement appariage *
modifi		lea	(screen-t,a5),a0
		move.w 	(mody-t,a5),d5
		mulu.w	#13,d5
		add.w	(modx-t,a5),d5
		add.l	d5,a0
		move.b	d2,(a0)
		move.w	(modx-t,a5),d0
		move.w	(mody-t,a5),d1
		jmp	([write1-t,a5])

*-----------* recherche du numero de blok sur la pos actuelle *-----------*
rechblk:
		lea	(screen-t,a5),a0
		move.w 	d1,d5
		mulu.w	#13,d5
		add.w	d0,d5
		add.l	d5,a0
		clr.l	d2
		move.b	(a0),d2
		rts

*---------------------------* calcul du timer *---------------------------*
caltim:
		clr.l	d0
		clr.l	d1
		clr.l	d2
		move.b	(tim3-t,a5),d0
		move.b	(tim2-t,a5),d1
		move.b	(tim1-t,a5),d2
		tst.b	d0
		bne.s	debtim
		tst.b	d1
		bne.s	debtim
		tst.b	d2
		beq.s	zero
debtim		subq.b	#1,d0
		cmpi.b	#-1,d0
		bne.s	tsttim
		move.b	#9,d0
		subq.b	#1,d1
tsttim		cmpi.b	#-1,d1
		bne	tsttim1
		move.b	#5,d1
		subq.b	#1,d2
tsttim1		cmpi.b	#-1,d2
		bne	fintim
zero		clr.b	(jeu-t,a5)
		clr.b	(tim1-t,a5)
		clr.b	(tim2-t,a5)
		clr.b	(tim3-t,a5)
fintim		move.b	d0,(tim3-t,a5)
		move.b	d1,(tim2-t,a5)
		move.b	d2,(tim1-t,a5)
		bsr	timcompt
		rts

*-------* affiche les valeurs (time, level, code du niveau actuel *-------*
* sur l'ecran get ready *
affval:
		moveq	#0,d7
aflev		moveq	#8,d0
		add.w	d7,d0
		moveq	#1,d1
		lea	(lev1-t,a5),a0
		clr.l	d2
		move.b	(a0,d7),d2
		add.b	#44,d2
		jsr	([write1-t,a5])
		addq.w	#1,d7
		cmp.w	#3,d7
		bne	aflev	
		moveq	#0,d7
afcod		moveq	#6,d0
		add.w	d7,d0
		moveq	#3,d1
		lea	(cod1-t,a5),a0
		clr.l	d2
		move.b	(a0,d7),d2
		add.b	#44,d2
		jsr	([write1-t,a5])
		addq.w	#1,d7
		cmp.w	#6,d7
		bne	afcod	
		moveq	#8,d0
		moveq	#5,d1
		clr.l	d2
		move.b	(tim1-t,a5),d2
		add.b	#44,d2
		jsr	([write1-t,a5])
		moveq	#10,d0
		moveq	#5,d1
		clr.l	d2
		move.b	(tim2-t,a5),d2
		add.b	#44,d2
		jsr	([write1-t,a5])
		moveq	#11,d0
		moveq	#5,d1
		clr.l	d2 
		move.b	(tim3-t,a5),d2
		add.b	#44,d2
		jsr	([write1-t,a5])
		rts

*---------* copie le niveau en memoire ou il peut etre modifie *----------*
copitab		
		move.w	(lev-t,a5),d0
		mulu	#142,d0
		lea	(game-t,a5),a0
		add.l	d0,a0
		lea	(screen-t,a5),a1
		cmp.b	#1,(recom-t,a5)
		bne	decopit
		lea	(12,a0),a0
		clr.b	(recom-t,a5)
		bra	ficopit
decopit		move.b	(a0)+,(lev1-t,a5)
		move.b	(a0)+,(lev2-t,a5)
		move.b	(a0)+,(lev3-t,a5)
		move.b	(a0)+,(cod1-t,a5)
		move.b	(a0)+,(cod2-t,a5)
		move.b	(a0)+,(cod3-t,a5)
		move.b	(a0)+,(cod4-t,a5)
		move.b	(a0)+,(cod5-t,a5)
		move.b	(a0)+,(cod6-t,a5)
		move.b	(a0)+,(tim1-t,a5)
		move.b	(a0)+,(tim2-t,a5)
		move.b	(a0)+,(tim3-t,a5)
ficopit		move.w	#130,d0
copiniv		move.b	(a0)+,(a1)+
		dbra	d0,copiniv
		bsr	timcompt
		bsr	levcompt
		bsr	boncompt
		rts

*-------------------* affichage du compte des points *--------------------*
ptscompt:
		moveq	#0,d7
affpts		move.l	d7,d0
		addq.w	#1,d0
		moveq	#0,d1
		move.w	#10,d2
		jsr	([write4-t,a5])
		lea	(pts1-t,a5),a0
		move.w	d7,d0
		addq.w	#1,d0
		moveq	#0,d1
		clr.l	d2
		move.b	(a0,d7),d2
		move.b	d2,d6
		add.w	#90,d2
		add.w	#44,d6
		jsr	([write3-t,a5])
		move.w	d7,d0
		addq.w	#1,d0
		moveq	#0,d1
		clr.l	d2
		move.w	d6,d2
		jsr	([write2-t,a5])
		addq.w	#1,d7
		cmp.w	#6,d7
		bne	affpts
		rts

*--------------------* Affichage  du compte du temps *--------------------*
timcompt:	moveq	#0,d7
		move.l	d7,d0
		addq.w	#8,d0
		moveq	#0,d1
		move.w	#10,d2
		jsr	([write4-t,a5])
		lea	(tim1-t,a5),a0
		move.w	d7,d0
		addq.w	#8,d0
		moveq	#0,d1
		clr.l	d2
		move.b	(a0,d7),d2
		move.b	d2,d6
		add.w	#90,d2
		add.w	#44,d6
		jsr	([write3-t,a5])
		move.w	d7,d0
		addq.w	#8,d0
		moveq	#0,d1
		clr.l	d2
		move.w	d6,d2
		jsr	([write2-t,a5])
		addq.w	#1,d7
afftim		move.l	d7,d0
		add.w	#9,d0
		moveq	#0,d1
		move.w	#10,d2
		jsr	([write4-t,a5])
		lea	(tim1-t,a5),a0
		move.w	d7,d0
		add.w	#9,d0
		moveq	#0,d1
		clr.l	d2
		move.b	(a0,d7),d2
		move.b	d2,d6
		add.w	#90,d2
		add.w	#44,d6
		jsr	([write3-t,a5])
		move.w	d7,d0
		add.w	#9,d0
		moveq	#0,d1
		clr.l	d2
		move.w	d6,d2
		jsr	([write2-t,a5])
		addq.w	#1,d7
		cmp.w	#3,d7
		bne	afftim
		rts

*--------------------* affichage  du compte du level *--------------------*
levcompt:
		moveq	#0,d7
afflev		move.l	d7,d0
		add.w	#13,d0
		moveq	#0,d1
		move.w	#10,d2
		jsr	([write4-t,a5])
		lea	(lev1-t,a5),a0
		move.w	d7,d0
		add.w	#13,d0
		moveq	#0,d1
		clr.l	d2
		move.b	(a0,d7),d2
		move.b	d2,d6
		add.w	#90,d2
		add.w	#44,d6
		jsr	([write3-t,a5])
		move.w	d7,d0
		add.w	#13,d0
		moveq	#0,d1
		clr.l	d2
		move.w	d6,d2
		jsr	([write2-t,a5])
		addq.w	#1,d7
		cmp.w	#3,d7
		bne	afflev
		rts

*---------* affichage  du compte du bonus (bombes et recommence) *--------*
boncompt:	
		moveq	#0,d7
		move.l	d7,d0
		add.w	#17,d0
		moveq	#0,d1
		move.w	#10,d2
		jsr	([write4-t,a5])
		lea	(bmb-t,a5),a0
		move.w	d7,d0
		add.w	#17,d0
		moveq	#0,d1
		clr.l	d2
		move.b	(a0,d7),d2
		move.b	d2,d6
		add.w	#90,d2
		add.w	#44,d6
		jsr	([write3-t,a5])
		move.w	d7,d0
		add.w	#17,d0
		moveq	#0,d1
		clr.l	d2
		move.w	d6,d2
		jsr	([write2-t,a5])
		move.l	d7,d0
		add.w	#19,d0
		moveq	#0,d1
		move.w	#10,d2
		jsr	([write4-t,a5])
		lea	(reco-t,a5),a0
		move.w	d7,d0
		add.w	#19,d0
		moveq	#0,d1
		clr.l	d2
		move.b	(a0,d7),d2
		move.b	d2,d6
		add.w	#90,d2
		add.w	#44,d6
		jsr	([write3-t,a5])
		move.w	d7,d0
		add.w	#19,d0
		moveq	#0,d1
		clr.l	d2
		move.w	d6,d2
		jsr	([write2-t,a5])
		rts

*----------* affichage transparent du compte des blocks actifs *----------*
blkcompt	
		moveq	#25,d7
		moveq	#0,d6
.aftrans	moveq	#3,d0
		move.w	d7,d1
		move.w	#118,d2
		jsr	([write3-t,a5])		|-> masblok
		lea	(blk1-t,a5),a0
		moveq	#3,d0
		move.w	d7,d1
		moveq	#0,d2
		move.b	(a0,d6),d2
		add.w	#108,d2
		jsr	([write2-t,a5])		|-> tr1blok
		add.w	#18,d7
		addq.w	#1,d6
		cmp.w	#8,d6
		bne.s	.aftrans
		rts

*-----------------* affichage complet du tableau 13X10 *------------------*
toutab:
		move.l	(write1-t,a5),a2
		lea	(28,a2),a2
		move.l	(block-t,a5),a3		table pointeurs blocks
		move.l	(niveau-t,a5),a4	tableau a afficher
		moveq	#9,d0			10 lignes de blocks
		move.l	(ecran.offset-t,a5),a0	adresse écran 1er block
		move.l	(ecran.sline-t,a5),d3
		sub.l	#13*2,d3		offset ligne suivante
		moveq	#0,d4
		moveq	#14,d6
.loopy		moveq	#12,d1			13 blocks par ligne
.loopx		move.b	(a4)+,d4		d4=n° du block
		move.l	(a3,d4*4),a1		a1=adresse du block
		jsr	(a2)
		addq.l	#2,a0
		dbf	d1,.loopx		
		add.l	d3,a0
		dbf	d0,.loopy
		rts

*>>>>>>>>>>>>>>>>>* Adresses utilisées par le programme *<<<<<<<<<<<<<<<<<*
		INCLUDE	"Mentat/Include/Systeme/t.s"

*-----------* Pointeurs adresses mémoire allouée au système *-------------*
* 1er   long mot = adresse mémoire
* 2ieme long mot = taille mémoire en octets
* 3ieme long mot = identificateur 4 caractères (non indispensable)
* 4ième long mot = type 0=Public (FAST ou CHIP) | 2=CHIP | 4=FAST
		CNOP	0,4	(<= aligne sur un long mot)
write1		dc.l	0,0,"CODE",0	=> af1blok
write2		dc.l	0,0,"CODE",0	=> tr1blok
write3		dc.l	0,0,"CODE",0	=> masblok
write4		dc.l	0,0,"CODE",0	=> toublok
block		dc.l	0,0,">PTS",0	=> table pointeurs sur les blocks
ilbm		dc.l	0,0,"ILBM",0	=> écran de jeu compacté IFF-ILBM
page		dc.l	0,0,"PAGE",0	=> page blocks en RAW-BLITTER
*/ Flag touches */
f1.key		dc.b	0
f2.key		dc.b	0
f3.key		dc.b	0
f10.key		dc.b	0
return.key	dc.b	0
haut.key	dc.b	0
bas.key		dc.b	0
droite.key	dc.b	0
gauche.key	dc.b	0
espace.key	dc.b	0
		CNOP	0,4
ecran.sline	dc.l	0		taille ligne écran * 16
ecran.offset	dc.l	0		adresse écran affichage 1ier Block

niveau		ds.l	1
ecran1		ds.l	1		adresse écran
lev		ds.w	1
timer		ds.w	1
action		ds.w	1
xpteur		ds.w	1
ypteur		ds.w	1
modx		ds.w	1
mody		ds.w	1
bactx		ds.w	1
bacty		ds.w	1
anicompt	ds.w	1
anicteur	ds.w	1
auani		ds.w	1
auanit		ds.w	1
partie		ds.b	1
blk1		ds.b	1
blk2		ds.b	1
blk3		ds.b	1
blk4		ds.b	1
blk5		ds.b	1
blk6		ds.b	1
blk7		ds.b	1
blk8		ds.b	1
recom		ds.b	1
pts1		ds.b	1
pts2		ds.b	1
pts3		ds.b	1
pts4		ds.b	1
pts5		ds.b	1
pts6		ds.b	1
tim1		ds.b	1
tim2		ds.b	1
tim3		ds.b	1
lev1		ds.b	1
lev2		ds.b	1
lev3		ds.b	1
bmb		ds.b	1
reco		ds.b	1
cod1		ds.b	1
cod2		ds.b	1
cod3		ds.b	1
cod4		ds.b	1
cod5		ds.b	1
cod6		ds.b	1
fix		ds.w	1
jeu		ds.b	1
cotg		ds.b	1
cotd		ds.b	1
cotd1		ds.b	1
cotd2		ds.b	1
cotd3		ds.b	1
cotb		ds.b	1
bomb		ds.w	1
plusblok	ds.b	1
ccolor1		dc.w	$111,$222,$333,$444,$555,$666,$777,$666,$555,$444,$333,$222
ccolor2		dc.w	$100,$200,$300,$400,$500,$600,$700,$600,$500,$400,$300,$200
ptccol		dc.w	0

*************** tableau de presentation, niveau et game over *****************
present		dc.b	100,101,0,0,0,0,0,0,0,0,0,101,102
		dc.b	103,73,61,62,67,64,0,56,71,68,72,72,104
		dc.b	0,72,73,0,0,75,58,71,72,62,68,67,0
		dc.b	0,56,68,57,58,89,89,89,89,57,68,66,0
		dc.b	0,54,57,57,89,89,89,54,65,54,62,67,0
		dc.b	0,72,67,57,89,89,73,58,65,65,58,71,0
		dc.b	0,60,71,54,59,77,89,89,89,57,68,66,0
		dc.b	0,0,0,0,0,0,0,0,0,0,0,0,0
		dc.b	103,69,71,58,72,72,0,72,69,54,56,58,104
		dc.b	105,106,0,0,0,0,0,0,0,0,0,106,107	
	
ready		dc.b	100,101,0,0,0,0,0,0,0,0,0,101,102
		dc.b	103,0,65,58,75,58,65,0,44,44,44,0,104
		dc.b	0,0,0,0,0,0,0,0,0,0,0,0,0
		dc.b	0,56,68,57,58,0,44,44,44,44,44,44,0
		dc.b	0,0,0,0,0,0,0,0,0,0,0,0,0
		dc.b	0,73,62,66,58,0,0,0,44,81,44,44,0
		dc.b	0,0,0,0,0,0,0,0,0,0,0,0,0
		dc.b	0,69,71,58,72,72,0,72,69,54,56,58,0
		dc.b	103,0,0,73,68,0,69,65,54,78,0,0,104
		dc.b	105,106,0,0,0,0,0,0,0,0,0,106,107

gover		dc.b	19,19,19,19,23,19,19,19,0,24,22,22,21
		dc.b	24,22,22,22,21,22,22,0,0,23,19,19,23
		dc.b	23,19,0,19,23,19,19,0,0,0,0,20,27
		dc.b	20,0,0,0,0,0,0,0,0,0,0,0,19
		dc.b	0,0,60,54,66,58,0,68,75,58,71,0,21
		dc.b	4,0,0,0,0,0,0,0,0,0,0,0,0
		dc.b	7,0,0,0,0,0,0,0,0,0,0,0,0
		dc.b	21,0,0,0,0,0,0,0,0,0,0,0,6
		dc.b	23,19,1,0,0,0,0,0,0,4,0,0,19
		dc.b	26,22,22,21,3,6,0,0,0,19,19,0,19

incode		dc.b	24,22,22,20,0,0,0,0,20,22,22,21,25
		dc.b	23,19,0,0,0,0,0,0,0,0,19,23,23
		dc.b	23,62,67,72,58,71,73,0,56,68,57,58,23
		dc.b	23,0,0,0,0,0,0,0,0,0,0,0,20
		dc.b	20,0,0,0,0,0,0,0,0,0,0,0,0
		dc.b	2,0,0,88,88,88,88,88,88,0,0,0,0
		dc.b	8,2,6,0,0,0,0,0,0,0,0,7,8
		dc.b	24,25,19,20,0,0,1,3,0,0,19,24,25
		dc.b	26,21,25,0,0,5,20,2,3,1,24,21,27
		dc.b	19,26,27,0,4,20,21,20,4,19,26,27,19
	
game		dc.b	0,0,1,1,5,2,3,2,4,7,0,0
		dc.b	1,0,0,0,0,5,7,8,0,0,3,4,0
		dc.b	20,0,24,22,22,22,20,22,20,0,24,22,25
		dc.b	23,0,23,11,24,22,27,0,0,0,23,0,20
		dc.b	23,0,26,22,27,0,0,0,0,2,23,0,0
		dc.b	23,0,0,0,0,0,0,0,20,22,20,25,33
		dc.b	20,22,22,22,20,0,0,0,0,0,0,23,20
		dc.b	23,11,14,24,27,0,0,0,0,0,0,26,27
		dc.b	23,16,18,23,0,0,0,0,0,0,0,0,0
		dc.b	23,15,17,23,0,0,1,3,7,0,0,0,0
		dc.b	26,22,22,27,1,3,2,4,8,5,0,0,0
	
		dc.b	0,0,2,1,4,3,8,9,5,8,0,0
		dc.b	3,5,0,0,0,0,0,0,0,0,0,0,0
		dc.b	20,20,29,37,37,37,37,28,2,1,0,0,0
		dc.b	0,0,0,0,0,0,0,0,20,20,0,0,0
		dc.b	9,0,0,0,0,0,0,0,0,0,5,0,0
		dc.b	20,0,0,0,0,0,0,0,1,7,2,0,0
		dc.b	21,20,0,0,0,6,0,0,24,22,25,0,0
		dc.b	20,33,33,33,33,20,0,6,21,0,21,0,0
		dc.b	26,25,24,22,22,27,0,3,4,0,0,0,0
		dc.b	24,21,21,25,0,0,6,4,8,0,0,0,20
		dc.b	20,26,27,26,20,0,3,8,7,0,0,20,21
	
		dc.b	0,0,3,1,5,7,9,2,7,8,0,0
		dc.b	0,0,0,0,0,0,0,0,0,0,0,0,0
		dc.b	0,0,0,0,7,0,2,4,0,0,0,0,7
		dc.b	8,0,0,0,20,22,22,20,6,0,0,8,3
		dc.b	6,0,0,6,0,0,0,0,3,0,0,1,7
		dc.b	5,0,0,32,32,0,0,32,32,0,3,2,20
		dc.b	20,4,5,0,0,0,0,0,0,6,5,24,25
		dc.b	23,19,3,2,0,0,0,0,0,8,24,27,23
		dc.b	26,22,25,6,0,0,0,0,0,24,27,21,23
		dc.b	24,25,23,19,1,0,0,0,24,27,21,21,23
		dc.b	26,27,26,22,20,3,8,20,26,22,22,22,27

		dc.b	0,0,4,5,4,9,2,4,6,8,0,0
		dc.b	1,0,0,0,8,0,0,0,0,3,0,0,0
		dc.b	24,29,37,37,28,2,0,0,21,21,0,0,0
		dc.b	20,0,0,0,0,20,0,0,0,0,2,0,4
		dc.b	3,0,8,0,0,7,1,0,0,20,22,22,21
		dc.b	21,1,2,0,4,21,37,20,8,0,0,0,6
		dc.b	26,22,20,32,20,0,0,0,21,0,0,1,19
		dc.b	5,0,0,0,23,7,0,6,0,0,4,19,0
		dc.b	21,5,0,0,26,20,32,20,32,0,19,0,8
		dc.b	26,21,0,0,0,0,0,0,0,0,0,0,7
		dc.b	20,5,0,0,0,0,8,0,0,0,20,22,22

		dc.b	0,0,5,4,7,6,2,8,1,8,0,0
		dc.b	1,8,0,4,3,0,2,4,0,0,7,0,3
		dc.b	20,20,0,20,20,0,20,20,0,20,20,0,20
		dc.b	2,0,0,9,23,0,0,23,0,2,23,0,8
		dc.b	21,0,0,7,20,0,6,20,5,1,23,0,4
		dc.b	20,32,32,20,27,32,32,20,32,21,27,32,20
		dc.b	0,0,8,0,0,0,5,0,0,0,0,0,6
		dc.b	8,0,3,0,0,0,2,0,0,0,0,0,1
		dc.b	4,0,29,37,37,37,21,37,37,28,22,25,19
		dc.b	20,32,20,0,0,0,0,0,0,0,0,26,25
		dc.b	31,35,20,33,33,33,33,33,33,33,33,33,21	

		dc.b	-1,-1,-1

		*/ Tableau en cours */
screen		ds.b	130


fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
version:	dc.b	"$VER: ThinkCroos 0.1 (11.06.95)",0

*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
