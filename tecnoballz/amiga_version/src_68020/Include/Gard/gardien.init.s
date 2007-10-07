*<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<*
*=-*-=><=-*-=><=-*-=><= INITIALISATION DES GARDIENS =><=-*-=><=-*-=><=-*-=*
*<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<>«»<*
* Entrée	a5.l	=> adr. table 't'
*		=> 	a6.l	CUSTOM
* Sortie	d7.l	<=0 pas d'erreur
gardien.init:
		bsr	jesusadit		£=> Décompacte map & dalles
		bne	.exit
		bsr	screengenese		£=> Initialise écran
		bne	.exit
		bsr	morfalou		£=> Remplit le plan 4
		bsr	init.sc_mapadr		|-> Calcul pt/dalle
		bne	.exit
		bsr	bruno			£=> Calcul valeur Blitter
		bsr	parametres2		|-> Sauve valeurs jeu
		bsr	affiche_dalles		|-> Affiche dalles fonds
		move.l	(bob32_pagegfx-t,a5),d0
		lea	(tsbob-t,a5),a0
		bsr	sbob_main.init		|-> Initialise ScrollBOBs
		bne	.exit
		bsr	ssha_initsha		|-> Init. SahdowScrollBOBs
		bne	.exit
		bsr	init_struct		£=> Initialise Gardiens
		bne.s	.exit
		bsr	accordeon		£=> Init. Raquettes
		bne.s	.exit
		bsr	mariodebellis		£=> Init. Missiles
		bne.s	.exit
		bsr	paragon			£=> Init. Explosions		
		bne	.exit
		bsr	nautilus		£=> Init. Balles
		bne	.exit
		bsr	capvert			£=> Init. Capsules monnaies
		bne.s	.exit
		bsr	last_astrapy		£=> Calcul mot ctrl crédit
		bsr	fanta			£=> Vies & Monnaies -> Lutins
		bne.s	.exit
		bsr	sprite.name		£=> Nom du joueur
		bne.s	.exit
		bsr	torpeur			£=> Barre de résistance
		bne.s	.exit
		bsr	cree_lutinblitz2	£=> Calcul lutin Blitz
		bne.s	.exit
		bsr	zinit_gadget		£=> Calcul Shape Gadgets
		bne.s	.exit
		move.w	#8,(rak.haut-t,a5)	Raquette 8 de haut
		moveq	#0,d0
		move.w	d0,(rak.inver-t,a5)	RAZ flag inverseur
		move.w	#5,(gd.explospace-t,a5) Explosion espacée
		move.w	d0,(gameover-t,a5)	RAZ flag gameover
		move.w	d0,(tableau.end-t,a5)	RAZ flag tableau terminé
		move.b	d0,(flag_pause-t,a5)	RAZ flag pause
		move.w	d0,(music.on-t,a5)	Flag autorise saut à la playroutine
		move.w	d0,(tecnowinner-t,a5)
		bsr.l	init_pts_lutins2
		moveq	#0,d7
.exit		rts

*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
*----------------* Décompacte les dalles & le map-editor *----------------*
*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
* Entrée	a5.l	=> adr. table 't'
* Sortie	d7.l	<=0 pas d'erreur
jesusadit:
		*/ Décompacte les dalles */
		lea	(gd.color-t,a5),a2
		lea	(dalma.pp-t,a5),a3
		lea	(dalma.pagegfx-t,a5),a4
		bsr.l	decode.pagepp
		bne	.exit

		*/ Décompacte le map-editor */
		lea	(maped.pp-t,a5),a4
		move.l	(RESA.ADR,a4),d0
		move.l	(RESA.SIZ,a4),d1
		lea	(mapeditor-t,a5),a4
		bsr.l	decrunch.pp
		bne.s	.exit
		moveq	#0,d7
.exit		rts

*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
*------------------------ Installe l'écran de jeu ------------------------*
*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
* Entrée	a5.l	=> adr. table 't'
* Sortie	d7.l	<=0 pas d'erreur
screengenese:
		lea	(pt_screen-t,a5),a4
		bsr.l	screen_res
		bne	.exit
		move.w	#1,(SFREQ,a0)		force 50Hz
		move.w	#3,(SFMODE_PLANS,a0)
		move.w	#3,(SFMODE_LUTINS,a0)
		move.w	#320,(SNPIXEL,a0)
		move.w	#256,(SNLIGNE,a0)
		move.b	#5,(SNPLAN,a0)
		move.w	#3,(SPBUFFER,a0)
		move.w	#320,(SPLARGE,a0)
		move.w	#256+32,(SPHAUTEUR,a0)
		move.w	#16,(SPDOWN,a0)
		move.w	#1,(SLUTIN,a0)		pt lutins dans la liste
		move.w	#$24,(SBPLCON2,a0)
		move.w	#$11,(SBPLCON4,a0)	Couleur lutin
		move.w	#1,(SP_VSCROLL,a0)	Scrolling vertical
		move.w	#512,(KKCOPPER,a0)	Intructions en +
		move.w	#1,(NBCOPPER,a0)	deux liste Copper
		move.w	#5,(SFADEOFFSPEED,a0)
		move.w	#3,(SFADEONSPEED,a0)
		bsr.l	init_screen
		bne	.exit

		*/ Recopie la palette de couleurs des dalles */
		lea	(gd.color-t,a5),a1
		lea	(SCOLOR_TABLE,a0),a2
		lea	(8*4,a2),a2
		moveq	#7,d0
.ggloop		move.l	(a1)+,(a2)+
		dbf	d0,.ggloop

		*/ Recopie les 16 dernières couleurs */
		move.l	(pt_palette-t,a5),a1
		lea	(16*4,a1),a1
		moveq	#15,d0
.loop		move.l	(a1)+,(a2)+
		dbf	d0,.loop

		*/ Calcul les huit valeurs couleurs des ombres */
		lea	(SCOLOR_TABLE,a0),a2
		lea	(8*4,a2),a1
		moveq	#7,d3
.zodkdiso	move.w	(a1)+,d0		Bits 4 à 7
		move.w	(a1)+,d1		Bits 0 à 3
		lsr.w	#1,d1
		and.w	#$777,d1
		btst	#0,d0
		bne.s	.nob
		or.w	#$004,d1
.nob		btst	#4,d0
		bne.s	.nog
		or.w	#$040,d1
		btst	#8,d0
.nog		bne.s	.nor
		or.w	#$400,d1
.nor		lsr.w	#1,d0
		and.w	#$777,d0
		move.w	d0,(a2)+
		move.w	d1,(a2)+
		dbf	d3,.zodkdiso

		*/ Sauve quelques valeurs */
		move.l	(SPECRAN1,a0),(ecran1-t,a5)
		move.l	(SPECRAN2,a0),(ecran2-t,a5)
		move.l	(SPECRAN3,a0),(ecran3-t,a5)
		moveq	#0,d0
		moveq	#0,d1
		move.w	(SCOORDX,a0),d0
		move.w	(SCOORDY,a0),d1
		lsl.w	#2,d0
		move.l	d0,(coordx-t,a5)
		move.l	d1,(coordy-t,a5)
		move.w	(SP_SPLAN,a0),d0
		mulu	#NOPLAN,d0
		move.l	d0,(ecran_shadow-t,a5)
		moveq	#0,d7
.exit		rts

*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
*------------------------* Initialise les missiles *----------------------*
*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
* Entrée	a5.l	=> adr. table 't'
* Sortie	d7.l	<=0 pas d'erreur
mariodebellis:
		*/ Réserve structures Shapes */
		lea	(missile-t,a5),a4
		bsr.l	reserve.mem
		bne.s	.exit

		*/ Initialise structure tirs des gardiens */
		move.l	(missile-t,a5),a1
		moveq	#NBR_MISSILE-1,d1
.sandra		moveq	#SBOB.FIRE,d0
		moveq	#SBOB.TIR2,d0
		bsr	sbob.only
		bne.s	.exit
		move.w	#6,(SBOB.INITTEMPO,a1)
		clr.l	(SBOB.ACTIF,a1)
		lea	(MIS.SIZE,a1),a1
		dbf	d1,.sandra
		moveq	#0,d7
.exit		rts

*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
*-----------------------* Initialise les explosions *---------------------*
*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
* Entrée	a5.l	=> adr. table 't'
* Sortie	d7.l	<=0 pas d'erreur
paragon:
		*/ Réserve structures Shapes */
		lea	(explomon-t,a5),a4
		bsr.l	reserve.mem
		bne.s	.exit

		*/ Initialise structure tirs des gardiens */
		move.l	(explomon-t,a5),a1
		moveq	#NBR_EXPLOMON-1,d2
		moveq	#SBOB.EXP2,d1
		moveq	#SBOB.EXP1,d0
.sandra		bsr	sbob.only
		bne.s	.exit
		exg	d0,d1
		move.w	#5,(SBOB.INITTEMPO,a1)
		clr.l	(SBOB.ACTIF,a1)
		lea	(SBOB.SIZE,a1),a1
		dbf	d2,.sandra
		moveq	#0,d7
.exit		rts

*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
************************* Calcul valeurs Blitter **************************
*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
* Entrée	a5.l	=> adr. table 't'
bruno:
		move.l	(pt_screen-t,a5),a0
		move.l	(dalma.pagegfx-t,a5),a4
		move.l	a4,(adalles-t,a5)
		add.l	#PG_DATA,(adalles-t,a5)
		move.w	(SPHAUTEUR,a0),d0
		sub.w	#16,d0
		move.w	d0,(s_nligne-t,a5)
		sub.w	#16,d0
		move.w	d0,(s_nligne2-t,a5)
		move.w	(PG_SPLAN,a4),(d_tplan-t,a5)
		move.w	(SP_SPLAN,a0),d0
		add.w	d0,d0
		subq.w	#2,d0
		move.w	d0,(s_tplan2-t,a5)
	
		*/ Bande 20 dalles 16 lignes ECRAN -> ECRAN */
		move.w	(SP_SPLAN,a0),d0
		move.w	d0,(splan_co-t,a5)
		lsr.w	#1,d0
		add.w	#16*64,d0
		move.w	d0,(win_co-t,a5)
		move.w	(SP_SLINE,a0),d0
		move.w	d0,(s_tligne-t,a5)
		sub.w	(SP_SPLAN,a0),d0
		move.w	d0,(mod_co-t,a5)
		move.w	d0,(2+mod_co-t,a5)

		*/ OFFSET MAP-EDITOR BAS ECRAN */
		move.w	(SPHAUTEUR,a0),d0
		sub.w	#16+16,d0
		lsr.w	#4,d0
		mulu	#M_LARGE*2,d0
		move.w	d0,(m_haut-t,a5)

		*/ MODULO COPIE PAGE DALLE -> ECRAN 16x16 */
		move.w	(PG_SLINE,a4),d0
		subq.w	#2,d0
		swap	d0
		move.w	(SP_SLINE,a0),d0
		subq.w	#2,d0
		move.l	d0,(mod_af-t,a5)
		rts

*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
***************************/ Remplit le plan 4 /***************************
*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
* Entrée	a5.l	=> adr. table 't'
morfalou:
		move.l	(pt_screen-t,a5),a0
		move.w	(SP_SPLAN,a0),d0
		move.w	d0,d4
		mulu	#NOPLAN,d4
		add.l	(SPECRAN1,a0),d4
		move.w	(SP_SLINE,a0),d5
		sub.w	d0,d5
		move.w	(SPHAUTEUR,a0),d2
		lsl.w	#6,d2
		lsr.w	#1,d0
		add.w	d0,d2
		moveq	#-1,d1
		WAITBLITTER
		move.w	d1,(BLTADAT,a6)
		move.l	d1,(BLTAFWM,a6)
		move.l	#$01F00000,(BLTCON0,a6)
		move.l	d4,(BLTDPTH,a6)
		move.w	d5,(BLTDMOD,a6)
		move.w	d2,(BLTSIZE,a6)
		rts

*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
*=|-|-|-|-|-|-|-|-| Initialise les structures Gardiens -|-|-|-|-|-|-|-|-|=*
*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
* Entrée	=> a5.l	Adr. 't'
init_struct:
		*/ Détermine le nombre de Gardiens */
		sub.l	a1,a1
		move.w	#1,(gd.numb-t,a5)
		move.w	(gd.pt-t,a5),d0
		lea	(sugar-t,a5),a0
		add.w	d0,a0
		move.l	(a0)+,a0
		add.l	a5,a0
		move.l	(a0)+,d0
		beq.s	.onegard
		move.w	#2,(gd.numb-t,a5)
		lea	(-4,a0,d0),a1
.onegard	move.l	a0,(alain1-t,a5)
		move.l	a1,(alain2-t,a5)

		*/ Réserve structures Gardiens */
		clr.l	(gd2-t,a5)
		lea	(gd1-t,a5),a4
		bsr.l	reserve.mem
		bne	.exit
		cmp.w	#2,(gd.numb-t,a5)
		bne.s	.no2
		lea	(gd2-t,a5),a4
		bsr.l	reserve.mem
		bne	.exit
.no2
		*/ RAZ quelques adresses */
		moveq	#0,d0
		move.w	d0,(gd.sspeed-t,a5)
		move.w	d0,(gd.lasty-t,a5)

		*/ Sauve paramètres du Gardien 1 */
		bsr.l	raz_table_lutins
		move.l	(alain1-t,a5),a0
		move.l	(gd1-t,a5),a4
		bsr	tout_est_bleu
		bne	.exit
		move.l	a4,a3
		lea	(gd1.lutin-t,a5),a4
		bsr	psychotechnique
		bne.s	.exit
		lea	(table_lutins-t,a5),a0
		move.l	(GARD.SPR0,a3),(a0)+
		move.l	(GARD.SPR1,a3),(a0)

		*/ Sauve paramètres du Gardien 2 */
		cmp.w	#2,(gd.numb-t,a5)
		bne.s	.noxdfjs2
		move.l	(alain2-t,a5),a0
		move.l	(gd2-t,a5),a4
		bsr	tout_est_bleu
		bne.s	.exit
		move.l	a4,a3
		lea	(gd2.lutin-t,a5),a4
		bsr	psychotechnique
		bne.s	.exit
		lea	(8+table_lutins-t,a5),a0
		move.l	(GARD.SPR0,a3),(a0)+
		move.l	(GARD.SPR1,a3),(a0)
.noxdfjs2	bsr	last_move

		moveq	#0,d7
.exit		rts

*------------------>ROUTINE INITIALISATION D'UN GARDIEN<------------------*
* Entrée	=>	a0.l	pt/table "sugar"
*		=>	a4.l	Structure "GARD"
tout_est_bleu:
		*/ Sauve Taille & Adresse GFX */
		move.w	(S.NSBOB,a0),d0
		move.l	(sbob_liste-t,a5),a1
		move.l	(8,a1,d0*4),a1
		move.w	(TSBOB_LARGE,a1),(GARD.WIDTH,a4)
		move.w	(TSBOB_HEIGHT,a1),(GARD.HEIGHT,a4)
		move.l	(TSBOB_TABLE,a1),(GARD.GFX,a4)
		move.w	(S.NSBOB,a0),d0
		bsr	ssha.ajoute
		bne	.exit
		move.l	a1,(GARD.BOB,a4)

		*/ Sauve limites de déplacement */
		move.l	(pt_screen-t,a5),a1
		move.w	(SNPIXEL,a1),d0
		sub.w	(GARD.WIDTH,a4),d0
		move.w	d0,(GARD.MAXX,a4)
		move.w	#YRAKB-1-16,d0
		sub.w	(GARD.HEIGHT,a4),d0
		move.w	d0,(GARD.MAXY,a4)
		move.w	#6,(GARD.MINX,a4)
		move.w	#2,(GARD.MINY,a4)

		*/ Sauve adresse table déplacement */
		move.w	(S.LISSA,a0),d0		d0=n° table de 0 à 11
		move.l	(lissa-t,a5),a1		
		move.l	(a1,d0*4),d0
		add.l	a1,d0
		move.l	d0,(GARD.LISSA,a4)
		move.l	d0,(GARD.LISSA2,a4)

		*/ Sauve zones de collisions */
		move.w	(S.COLX1,a0),(GARD.X1,a4)
		move.w	(S.COLY1,a0),(GARD.Y1,a4)
		move.w	(S.COLX2,a0),(GARD.X2,a4)
		move.w	(S.COLY2,a0),(GARD.Y2,a4)

		*/ Sauve coodonnées départ des tirs */
		move.w	(S.XCENT,a0),(GARD.XFIRE,a4)
		move.w	(S.YCENT,a0),(GARD.YFIRE,a4)

		*/ Divers */
		move.w	(hard-t,a5),d7
		addq.w	#1,d7
		move.w	(S.POWER,a0),d0
		mulu	d7,d0
		move.w	d0,(GARD.POWER,a4)
		moveq	#0,d0
		move.w	(S.WAIT2,a0),d0
		divu	d7,d0
		move.w	d0,(GARD.WAIT2,a4)
		move.w	d0,(GARD.WAIT,a4)
		moveq	#0,d0
		move.w	(S.WAITF,a0),d0
		divu	d7,d0
		move.w	d0,(GARD.WAITF2,a4)
		move.w	d0,(GARD.WAITF,a4)

		move.w	(S.SPEED,a0),d0
		cmp.w	#3,d7
		blt.s	.ka
		add.w	d0,d0
.ka		move.w	d0,(GARD.SPEED,a4)
		lea	(S.TFIRE,a0),a1
		move.l	a1,(GARD.FIRE,a4)
		moveq	#0,d7
.exit		rts

*--------------------------* Calcul les lutins *--------------------------*
* Entrée	=> a3.l	Structure "GARD"
*		=> a4.l	Pt/Structure RESA
psychotechnique:
		*/ Réserve les 2 listes lutins */
		move.w	(GARD.HEIGHT,a3),d0
		addq.w	#2,d0
		mulu	#16*2,d0
		add.l	#16*2*20,d0		+ Raquette & Crédit
		move.l	d0,(RESA.SIZ,a4)
		bsr.l	reserve.mem
		bne.s	.exit

		*/ Sauve Adresse des deux listes lutins */
		move.l	(RESA.ADR,a4),d1
		move.l	d1,(GARD.SPR0,a3)
		move.l	(RESA.SIZ,a4),d0
		lsr.l	#1,d0
		add.l	d0,d1
		move.l	d1,(GARD.SPR1,a3)

		*/ Recopie le BOB dans la liste lutin */
		move.l	(sbob_pagegfx-t,a5),a4
		move.w	(GARD.HEIGHT,a3),d0
		subq.w	#1,d0
		move.w	(PG_SPLAN,a4),d1
		move.w	(PG_SLINE,a4),d2
		move.l	(GARD.SPR0,a3),a0
		move.l	(GARD.SPR1,a3),a1
		move.l	(GARD.GFX,a3),a2
		cmp.w	#32,(GARD.WIDTH,a3)
		beq.s	.s
		bsr.l	copytolutin16.3
		bra.s	.sq
.s		bsr	kklutin64
.sq		moveq	#0,d7
.exit		rts

****************** Recopie Gfx de 32 dans un lutin de 64 ******************
* Entrée	=> d0.w	nombre de ligne du lutin - 1
*		=> d1.w	taille d'un plan raw-blitter
*		=> d2.w	taille d'une ligne
*		=> a0.l	Adr. du lutin paire
*		=> a1.l	Adr. du lutin impaire
*		=> a2.l	Adr. du GFX
* utilise (d0/d1/d2/a0/a1/a2/a4)
* modifie (a0/a1)
kklutin64:
		lea	(16,a0),a0		Saute mot ctrl lutin paire
		lea	(16,a1),a1		Saute mot ctrl lutin impaire
kklutin:	movem.l	d0/a2/a4,-(a7)
.loop		move.l	a2,a4
		move.l	(a4),(a0)		Plan 1 -> lutin paire
		addq.l	#8,a0
		add.w	d1,a4
		move.l	(a4),(a0)		Plan 2 -> lutin paire
		addq.l	#8,a0
		add.w	d1,a4
		move.l	(a4),(a1)		Plan 3 -> lutin impaire
		addq.l	#8,a1
		add.w	d1,a4
		move.l	(a4),(a1)		Plan 4 -> lutin impaire
		addq.l	#8,a1
		add.w	d2,a2
		dbf	d0,.loop
		movem.l	(a7)+,d0/a2/a4
		rts

*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
*/*/*/*/*/*/*/*/*/*/*/*/*Initialise les Raquettes*/*/*/*/*/*/*/*/*/*/*/*/*/
*-|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|=|-*
* Entrée	a5.l	=> adr. table 't'
* Sortie	d7.l	<=0 pas d'erreur
accordeon:
		*/ Sauve largeur raquette */
		move.w	(rak.large-t,a5),(rak.slarge-t,a5)
		move.w	#32,(rak.large-t,a5)

		*/ Efface structure BUMP */
		lea	(b1-t,a5),a0
		move.w	(1*BUMP.SIZE+BUMP.ACTIF,a0),d2
		move.w	(2*BUMP.SIZE+BUMP.ACTIF,a0),d3
		move.w	(3*BUMP.SIZE+BUMP.ACTIF,a0),d4
		move.l	#4*BUMP.SIZE,d0
		bsr.l	clear.memory
		lea	(b1-t,a5),a0
		move.w	d2,(1*BUMP.SIZE+BUMP.ACTIF,a0)
		move.w	d3,(2*BUMP.SIZE+BUMP.ACTIF,a0)
		move.w	d4,(3*BUMP.SIZE+BUMP.ACTIF,a0)

*------------------------*/ Raquette basse <1> /*-------------------------*
		*/ Recopie le Lutin 0 & 1 */ 
		move.l	(gd1-t,a5),a3
		bsr	.brainchild		|-> calcul adresse lutins
		move.l	d1,(bump1.lutin0-t,a5)
		move.l	d2,(bump1.lutin1-t,a5)
		bsr	.nostrum		|-> Copie Bumper -> Lutin
		move.l	a0,(cred.lutin0-t,a5)
		move.l	a1,(cred.lutin1-t,a5)
		*/ Initialise structure BUMP */
		lea	(b1-t,a5),a4
		move.w	#1,(BUMP.NUMBER,a4)
		move.l	(pt_screen-t,a5),a0
		moveq	#0,d6
		move.w	(SNPIXEL,a0),d6		d6=320
		sub.w	(rak.large-t,a5),d6	d6=288
		lsr.w	#1,d6			d6=144
		tst.w	(rak.team-t,a5)
		beq.s	.jaisoif
		move.w	(SNPIXEL,a0),d6		d6=320
		sub.w	(rak.large-t,a5),d6
		sub.w	(rak.large-t,a5),d6	d6=256
		divu	#3,d6			d6=85
		add.w	d6,d6			d6=170
		add.w	(rak.large-t,a5),d6	d6=202
.jaisoif	bsr	.positive
		bne	.exit

*------------------------*/ Raquette basse <4> /*-------------------------*
		tst.w	(rak.team-t,a5)		Mode Team ?
		beq	.jaidesmars		-> Non
		cmp.w	#2,(gd.numb-t,a5)	Deux Gardiens ?
		beq.s	.noxdfjs2		-> Oui
		lea	(gd2.lutin-t,a5),a4
		move.l	#16*2*20,(RESA.SIZ,a4)
		bsr.l	reserve.mem
		bne	.exit
		move.l	(gd2.lutin-t,a5),d1
		move.l	d1,d2
		add.l	#16*20,d2
		lea	(8+table_lutins-t,a5),a0
		move.l	d1,(a0)+
		move.l	d2,(a0)+
		bra.s	.recall
.noxdfjs2	move.l	(gd2-t,a5),a3
		bsr	.brainchild		|-> calcul adresse lutins
.recall		move.l	d1,(bump2.lutin2-t,a5)
		move.l	d2,(bump2.lutin3-t,a5)
		bsr	.nostrum
		move.l	a0,(life.lutin2-t,a5)
		move.l	a1,(life.lutin3-t,a5)
		*/ Initialise structure BUMP */
		move.l	(pt_screen-t,a5),a0
		moveq	#0,d6
		move.w	(SNPIXEL,a0),d6		d6=320
		sub.w	(rak.large-t,a5),d6
		sub.w	(rak.large-t,a5),d6	d6=256
		divu	#3,d6			d6=85
		lea	(b4-t,a5),a4
		move.w	#4,(BUMP.NUMBER,a4)
		bsr	.positive
		bne.s	.exit
		bra.s	.vestige
		
		*/ Pas de 2ieme BUMPER */
.jaidesmars	cmp.w	#2,(gd.numb-t,a5)
		beq.s	.noxdfjs277
		lea	(gd2.lutin-t,a5),a4
		move.l	#16*2*10,(RESA.SIZ,a4)
		bsr.l	reserve.mem
		bne.s	.exit
		move.l	(gd2.lutin-t,a5),d1
		move.l	d1,d2
		add.l	#16*10,d2
		lea	(8+table_lutins-t,a5),a0
		move.l	d1,(a0)+
		move.l	d2,(a0)+
		bra.s	.recall2
.noxdfjs277	move.l	(gd2-t,a5),a3
		bsr	.brainchild		|-> calcul adresse lutins
.recall2	move.l	d1,(life.lutin2-t,a5)
		move.l	d2,(life.lutin3-t,a5)
.vestige	moveq	#0,d7
.exit		rts

		*/ Donne adresse liste lutins des Bumpers */
		* => a3.l Structure Gardien *
.brainchild:	move.l	(GARD.SPR0,a3),d1
		move.l	(GARD.SPR1,a3),d2
		move.w	(GARD.HEIGHT,a3),d0
		addq.w	#1,d0
		mulu	#16,d0
		add.l	d0,d1
		add.l	d0,d2
		rts

		*/ Recopie le lutin Bumper */
		* => d1.l Lutin Paire / d2.l Lutin umpaire
.nostrum	moveq	#SBOB.BUMP,d0
		bsr	sbob.get
		bne	.exit
		move.w	(TSBOB_HEIGHT,a0),d0
		subq.w	#1,d0
		move.l	(TSBOB_TABLE,a0),a2
		move.l	d1,a0
		move.l	d2,a1
		move.l	(sbob_pagegfx-t,a5),a4
		move.w	(PG_SPLAN,a4),d1
		move.w	(PG_SLINE,a4),d2
		bsr	kklutin64		|-> GFX 32 -> Lutin 64
		rts
	
		*/ Initialise Structure BUMP */
		* => d6.w Pos.X / a4.l Structure BUMP
.positive	moveq	#SBOB.BUMP,d0
		bsr	ssha.ajoute
		bne.s	.exit2
		move.l	a1,(BUMP.BOB,a4)
		move.w	d6,(SBOB.POSX,a1)
		move.w	#YRAKB,(SBOB.POSY,a1)
		move.w	d6,(BUMP.X,a4)
		move.w	#YRAKB,(BUMP.Y,a4)
		lea	(midi01_left-t,a5),a0
		move.l	a0,(BUMP.RBLEFT,a4)
		lea	(midi01_right-t,a5),a0
		move.l	a0,(BUMP.RBRIGHT,a4)
		move.w	#2,(BUMP.NORMAL,a4)
		clr.w	(BUMP.FIRE,a4)
		clr.w	(BUMP.GLUE,a4)
		move.w	#1,(BUMP.ACTIF,a4)
		moveq	#0,d7
.exit2		rts

*/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/*
**************************-*INITIALISE LA BALLE*-**************************
*/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/*
* Entrée	a5.l	=> adr. table 't'
nautilus:
		*/ Init. les structures */
		lea	(pt_balle-t,a5),a4
		move.l	#SBALLE.SIZE*NSBALL,(RESA.SIZ,a4)
		bsr.l	reserve.mem
		bne	.exit
		move.l	(pt_balle-t,a5),a1
		moveq	#NSBALL-1,d6
.loop		moveq	#SBOB.BALL,d0
		bsr	sbob.only
		bne	.exit
		bsr	ssha.only
		bne	.exit
		clr.l	(SBOB.ACTIF,a1)
		lea	(b1-t,a5),a2
		move.w	#200,(SBALLE.X,a1)
		move.w	(BUMP.Y,a2),(SBALLE.Y,a1)
		move.l	a2,(SBALLE.RAK,a1)
		move.w	#5,(SBALLE.PBTEMP,a1)
		move.w	(balle_start-t,a5),(SBALLE.START,a1)
		move.w	#1,(SBALLE.GLUE,a1)
		move.l	(balle_speed-t,a5),(SBALLE.SPEED,a1)
		move.w	(balle_scount-t,a5),(SBALLE.SCOUNT,a1)
		move.w	#5,(SBOB.WIDTH,a1)
		move.w	#1,(SBALLE.POWER,a1)
		lea	(SBALLE.SIZE,a1),a1
		dbf	d6,.loop
		move.l	(pt_balle-t,a5),a1
		move.w	#1,(SBALLE.ACTIF,a1)
		clr.w	(balle_nbre-t,a5)
		moveq	#0,d7
.exit		rts

*/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/*
*---------------------*INITIALISE STRUCTURE MONNAIE*----------------------*
*/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/*
* Entrée	a5.l	=> adr. table 't'
capvert:
		*/ Réserve les structures */
		lea	(pt_capsule-t,a5),a4
		move.l	#CAP.SIZE*NMONNAIE,(RESA.SIZ,a4)
		bsr.l	reserve.mem
		bne.s	.exit

		*/ Init. les structures */
		move.l	(pt_capsule-t,a5),a1
		moveq	#NMONNAIE-1,d6
.loopinit	moveq	#SBOB.CAPS,d0
		bsr	sbob.only
		bne	.exit
		bsr	ssha.only
		bne	.exit
		move.w	#3,(SBOB.INITTEMPO,a1)
		clr.l	(SBOB.ACTIF,a1)
		move.w	#128,(SCAP.X,a1)
		lea	(b1-t,a5),a0
		move.l	a0,(SCAP.RAK,a1)
		lea	(SCAP.SIZE,a1),a1
		dbf	d6,.loopinit
		moveq	#0,d7
.exit		rts

*/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/*
***********************/ CACLUL POSITION CREDIT */*************************
*/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/*
* Entrée	=> a5.l	Adr. 't'
last_astrapy:
		move.l	(coordx-t,a5),d6
		move.l	(coordy-t,a5),d7
		move.l	(cred.lutin0-t,a5),a0
		move.l	(cred.lutin1-t,a5),a1
		moveq	#26*4,d0
		bsr.s	.totomania
		move.l	(life.lutin2-t,a5),a0
		move.l	(life.lutin3-t,a5),a1
		move.l	#264*4,d0
.totomania	moveq	#0,d1
		move.w	#247,d1
		moveq	#8,d2
		bsr.l	calcul_mot_ctrl_aga
		move.w	d3,(8,a0)
		or.w	#$80,d3
		move.w	d3,(8,a1)
		swap	d3
		move.w	d3,(a0)	
		move.w	d3,(a1)
		rts

*/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/*
*-----------------* CONVERTIT BOBS LIFES EN SPRITES *---------------------*
*/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/////\/*
* Entrée	=> a5.l	Adr. 't'
fanta:		
		moveq	#SBOB.LIFE,d0
		lea	(ss.life-t,a5),a3
		move.l	(life.lutin2-t,a5),(SS.LUTIN0,a3)
		move.l	(life.lutin3-t,a5),(SS.LUTIN1,a3)
		bsr.s	.supersprite
		bne	.exit
		moveq	#SBOB.CAPS,d0
		lea	(ss.credit-t,a5),a3
		move.l	(cred.lutin0-t,a5),(SS.LUTIN0,a3)
		move.l	(cred.lutin1-t,a5),(SS.LUTIN1,a3)
.supersprite	add.l	#16,(SS.LUTIN0,a3)
		add.l	#16,(SS.LUTIN1,a3)
		move.w	#1,(SS.TEMPO,a3)
		move.w	#5,(SS.INIT,a3)
		bsr	sbob.get
		moveq	#0,d0
		move.w	(TSBOB_WIDTH,a0),d0	d0=Largeur en mots
		add.w	d0,d0			d0=Largeur en octets
		mulu	(TSBOB_HEIGHT,a0),d0	d0=Taille d'un plan
		add.w	d0,d0			d0=Taille d'un lutin
		move.l	d0,(SS.TAILLE,a3)
		add.w	d0,d0
		move.l	d0,(SS.TAILLE2,a3)
		mulu	(TSBOB_NUMBER,a0),d0
		lea	(SS.ADR,a3),a4
		move.l	d0,(RESA.SIZ,a4)
		move.l	#'SPRI',(RESA.IDE,a4)
		move.l	#MEMF_CHIP,(RESA.TYP,a4)
		bsr.l	reserve.mem
		bne.s	.exit
		move.w	(TSBOB_HEIGHT,a0),d0
		subq.w	#1,d0
		move.w	d0,(SS.HEIGHT,a3)
		move.w	(TSBOB_NUMBER,a0),d6
		subq.w	#1,d6
		move.l	(sbob_pagegfx-t,a5),a4
		move.w	(PG_SPLAN,a4),d1
		move.w	(PG_SLINE,a4),d2
		lea	(TSBOB_TABLE,a0),a4
		move.l	(SS.ADR,a3),a0
		move.l	(SS.TAILLE,a3),d3
		lea	(a0,d3),a1
.loop		move.l	(a4),a2
		bsr.l	copylutin16.0
		add.l	d3,a0
		add.l	d3,a1
		addq.l	#8,a4
		dbf	d6,.loop
		moveq	#0,d7
.exit		rts
* -=O=- -=O=- -=O=- -=O=- -=O=- -=O=- -=O=- -=O=- -=O=- -=O=- -=O=- -=O=- *
*Oo-----------=-=-==-==> Nom du joueur en lutins <==-==-=-=-------------oO*
* -=O=- -=O=- -=O=- -=O=- -=O=- -=O=- -=O=- -=O=- -=O=- -=O=- -=O=- -=O=- *
* Entrée	=> a5.l	Adr. 't'
sprite.name:
		*/ Réserve la liste Lutin */
		lea	(gd.lutin45-t,a5),a4
		move.l	#16*DRAX*2,(RESA.SIZ,a4)
		bsr.l	reserve.mem
		bne	.exit

		*/ Calcul mots de contrôles */
		move.l	(RESA.ADR,a4),a0
		lea	(16*DRAX,a0),a1
		move.l	a0,(name.lutin4-t,a5)
		move.l	a1,(name.lutin5-t,a5)
		move.l	(coordx-t,a5),d6
		move.l	(coordy-t,a5),d7
		lea	(24+table_lutins-t,a5),a2
		move.l	#136*4,d0		Position X du lutin
		moveq	#1,d1			Position Y du lutin
		moveq	#DRAX1,d2
		bsr.l	calcul_mot_ctrl_aga
		move.w	d3,(8,a0)
		or.w	#$80,d3
		move.w	d3,(8,a1)
		swap	d3
		move.w	d3,(a0)	
		move.w	d3,(a1)	
		move.l	a0,(a2)+
		move.l	a1,(a2)+
		move.l	(name.lutin4-t,a5),a4
		bsr.s	.kata
		move.l	(name.lutin5-t,a5),a4
.kata		lea	(16,a4),a4			Saute mot de contrôle
		move.l	(fonte2.pagegfx-t,a5),a0
		moveq	#0,d3
		move.w	(PG_SLINE,a0),d3
		moveq	#0,d4
		lea	(PG_DATA,a0),a0
		lea	(name.x-t,a5),a1
		moveq	#5,d6

*----------------> Affiche fonte(s) dans une liste lutin <----------------*
* Entrée	=> d6.w	Nombre de caractère(s) - 1	
*		=> a0.l	Adr. fontes
*		=> a1.l	Pt/text
*		=> a4.l	Pt/lutin 64 pixels
.print		move.b	(a1)+,d4		d4=n° du caractère
		lea	(a0,d4),a2		a2=pt/fonte
		move.l	a4,a3
		move.b	(a2),(a3)		Ligne 1
		move.b	(a2),(8,a3)		Ligne 1
		lea	(16,a3),a3
		add.l	d3,a2
		move.b	(a2),(a3)		Ligne 2
		move.b	(a2),(8,a3)		Ligne 2
		lea	(16,a3),a3
		add.l	d3,a2
		move.b	(a2),(a3)		Ligne 3
		move.b	(a2),(8,a3)		Ligne 3
		lea	(16,a3),a3
		add.l	d3,a2
		move.b	(a2),(a3)		Ligne 4
		move.b	(a2),(8,a3)		Ligne 4
		lea	(16,a3),a3
		add.l	d3,a2
		move.b	(a2),(a3)		Ligne 5
		move.b	(a2),(8,a3)		Ligne 5
		lea	(16,a3),a3
		add.l	d3,a2
		move.b	(a2),(a3)		Ligne 6
		move.b	(a2),(8,a3)		Ligne 6
		lea	(16,a3),a3
		add.l	d3,a2
		move.b	(a2),(a3)		Ligne 7
		move.b	(a2),(8,a3)		Ligne 7
		lea	(16,a3),a3
		add.l	d3,a2
		move.b	(a2),(a3)		Ligne 8
		move.b	(a2),(8,a3)		Ligne 8
		addq.l	#1,a4
		dbf	d6,.print
		moveq	#0,d7
.exit		rts

*==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:=*
*/\/\/\/\/\/\/\ Calcul la barre de resistance des gardiens \/\/\/\/\/\/\/\*
*==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:==:=*
* Entrée	a5.l	=> adr. table 't'
torpeur:
		move.l	(name.lutin4-t,a5),a0
		move.l	(name.lutin5-t,a5),a1
		lea	(16*(DRAX1+1),a0),a0
		lea	(16*(DRAX1+1),a1),a1
		move.l	(coordx-t,a5),d6
		move.l	(coordy-t,a5),d7
		moveq	#2,d0			Position X du lutin
		moveq	#104,d1			Position Y du lutin
		move.w	#DRAX2,d2
		bsr.l	calcul_mot_ctrl_aga
		move.w	d3,(8,a0)
		or.w	#$80,d3
		move.w	d3,(8,a1)
		swap	d3
		move.w	d3,(a0)	
		move.w	d3,(a1)	
		lea	(16*(1+DRAX2),a0),a2
		lea	(16*(1+DRAX2),a1),a3
		move.l	a2,a0
		move.l	a3,a1
		move.l	(gd1-t,a5),a4
		bsr.s	.rapide
		move.l	a0,(power1.lutin4-t,a5)
		move.l	a1,(power1.lutin5-t,a5)
		cmp.w	#2,(gd.numb-t,a5)
		bne	.stop
		lea	(2,a2),a0
		lea	(2,a3),a1
		move.l	(gd2-t,a5),a4
		bsr.s	.rapide
		move.l	a0,(power2.lutin4-t,a5)
		move.l	a1,(power2.lutin5-t,a5)
		bra	.stop
.rapide		move.w	(GARD.POWER,a4),d0
		lsr.w	#2,d0			4 unités pour une ligne
		move.w	d0,(GARD.POW,a4)
		btst	#0,d0
		beq.s	.paire			-> BIT 0 à ZERO
		move.b	#%01001010,(a0)		Plan 1
		move.b	#%01100110,(8,a0)	Plan 2
		move.b	#%10000001,(a1)		Plan 3
		move.b	#%11111111,(8,a1)	Plan 4
		lea	(-16,a0),a0
		lea	(-16,a1),a1
.paire		lsr.w	#1,d0
		subq.w	#1,d0
		bmi.s	.stop
.loop		move.b	#%10110101,(a0)		Plan 1
		move.b	#%00101100,(8,a0)	Plan 2
		move.b	#%11000011,(a1)		Plan 3
		move.b	#%11111111,(8,a1)	Plan 4
		lea	(-16,a0),a0
		lea	(-16,a1),a1
		move.b	#%01001010,(a0)		Plan 1
		move.b	#%01100110,(8,a0)	Plan 2
		move.b	#%10000001,(a1)		Plan 3
		move.b	#%11111111,(8,a1)	Plan 4
		lea	(-16,a0),a0
		lea	(-16,a1),a1
		dbf	d0,.loop
		lea	(16,a0),a0
		lea	(16,a1),a1
.stop		moveq	#0,d7
.exit		rts

*/--------------------/ Crée les 7 lutins GigaBlitz /--------------------/*
* Entrée	=> a5.l	Adr. 't'
cree_lutinblitz2:
		clr.w	(blitz.haut-t,a5)	GigaBlitz en action ?

		*/ Décompacte la page graphique des 7 GigaBlitz */
		sub.l	a2,a2
		lea	(blitz.pp-t,a5),a3
		lea	(blitz.pagegfx-t,a5),a4
		bsr.l	decode.pagepp
		bne	.exit

		*/ Réserve la mémoire des 7 Lutins */
		lea	(blitz.lutin-t,a5),a4
		bsr.l	reserve.mem
		bne	.exit

		*/ Recopie les 7 Lutins au format Sprite */
		move.l	(blitz.pagegfx-t,a5),a2
		move.w	(PG_SPLAN,a2),d1
		move.w	(PG_SLINE,a2),d2
		lea	(PG_DATA,a2),a2
		lea	(blitz.type-t,a5),a4
		move.l	(blitz.lutin-t,a5),a0
		lea	(blitz.list-t,a5),a3
		moveq	#6,d4			7 GigaBlitz
.looplaloop	move.l	a0,a1			
		move.l	(a4)+,d3		d3=taille liste lutin
		add.l	d3,a1
		move.l	(a4),d0			d0=hauteur du lutin
		subq.w	#1,d0
		move.l	a0,(a3)+		sauve adresse lutin paire
		move.l	a1,(a3)+		sauve adresse lutin impaire
		bsr.l	copytolutin16.3
		lea	(16,a0),a0
		lea	(16,a1),a1
		add.l	d3,a0
		move.l	(a4)+,d0		d0=hauteur du lutin
		mulu	d2,d0			*taille ligne page graphique
		add.l	d0,a2			a2=adresse prochain GigaBlitz
		dbf	d4,.looplaloop

		*/ Retourne les lutins */
		lea	(blitz.list-t,a5),a3
		lea	(blitz.height-t,a5),a4
		moveq	#6,d7			7 GigaBlitz
.loopalanoix	move.w	(a4),d5			d5=hauteur lutin
		subq.w	#1,d5
		lsl.w	#4,d5
		move.l	(a3)+,a0		lutin paire
		bsr.s	.rotate
		move.l	(a3)+,a0		lutin paire
		bsr.s	.rotate
		addq.l	#2,a4
		dbf	d7,.loopalanoix
		bra	.z7
.rotate		move.w	(a4),d6			hauteur lutin
		lsr.w	#1,d6
		subq.w	#1,d6
		lea	(16,a0),a0		saute mot de ctrl
		lea	(a0,d5),a1
.lax		move.l	(a0),d1
		move.l	(4,a0),d2
		move.l	(8,a0),d3
		move.l	(12,a0),d4
		move.l	(a1),(a0)
		move.l	(4,a1),(4,a0)
		move.l	(8,a1),(8,a0)
		move.l	(12,a1),(12,a0)
		move.l	d1,(a1)
		move.l	d2,(4,a1)
		move.l	d3,(8,a1)
		move.l	d4,(12,a1)
		lea	(16,a0),a0
		lea	(-16,a1),a1
		dbf	d6,.lax
		rts
.z7
		*/ Calcul coordonnées */
		lea	(blitz.height-t,a5),a2
		lea	(blitz.list-t,a5),a3
		move.l	(coordx-t,a5),d6
		move.l	(coordy-t,a5),d7
		moveq	#6,d4			7 GigaBlitz
.hope		moveq	#-64,d0
		moveq	#0,d1
		move.w	(a2)+,d2
		bsr.l	calcul_mot_ctrl_aga
		move.l	(a3)+,a0
		move.l	(a3)+,a1
		move.w	d3,(8,a0)
		or.w	#$80,d3
		move.w	d3,(8,a1)
		swap	d3
		move.w	d3,(a0)	
		move.w	d3,(a1)	
		dbf	d4,.hope
		lea	(blitz.list-t,a5),a3
		lea	(16+table_lutins-t,a5),a0	a0=Adr. table des 8 adresses
		move.l	(a3)+,(a0)+
		move.l	(a3)+,(a0)+

		*/ Libère la page graphique */
		lea	(blitz.pagegfx-t,a5),a0
		bsr.l	libere_mem
.exit		rts

*-------------------------------------------------------------------------*
*-------------------* Initialise les gadgets monnaie *-------------------*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
* Sortie	<= d7.l	0=pas d'erreur
zinit_gadget:
		*/ Réserve les structures */
		lea	(pt_gadget-t,a5),a4
		move.l	#SGAD.SIZE*NGADGET,(RESA.SIZ,a4)
		bsr.l	reserve.mem

		*/ Init. les structures */
		move.l	(pt_gadget-t,a5),a1
		moveq	#NGADGET-1,d6
.loopinit	moveq	#SBOB.GADG,d0
		bsr	sbob.only
		bne	.exit
		bsr	ssha.only
		bne	.exit
		move.w	#1,(SGAD.TEMPO,a1)
		clr.l	(SBOB.ACTIF,a1)
		lea	(SGAD.SIZE,a1),a1
		dbf	d6,.loopinit
		moveq	#0,d7
.exit		rts

*=========================================================================*
***-----------------------Initialise LE BOB TEXT -----------------------***
*=========================================================================*
init_bobtext2:
		lea	(ze_bobtext2-t,a5),a0
		move.b	(1+lev0-t,a5),(7,a0)
		move.b	(1+lev1-t,a5),(6,a0)
		add.b	#'\',(6,a0)
		add.b	#'\',(7,a0)
		move.w	#320,(tb.la-t,a5)
		*/ Compte le nombre de lettres /* 
		move.l	a0,(pt_tb-t,a5)
		move.l	(pt_tb-t,a5),a0
		moveq	#0,d0
.n0		tst.b	(a0)+
		beq.s	.n2
		addq.w	#1,d0
		bra.s	.n0
.n2		move.w	d0,(tb.l1-t,a5)
		subq.w	#1,(tb.l1-t,a5)
		moveq	#0,d1
.n3		tst.b	(a0)+
		beq.s	.n4
		addq.w	#1,d1
		bra.s	.n3
.n4		move.w	d1,(tb.l2-t,a5)
		subq.w	#1,(tb.l2-t,a5)
		add.w	d1,d0
		move.w	d0,(tb.lx-t,a5)
		subq.w	#1,(tb.lx-t,a5)

		*/ Réserve les structures  */
		mulu	#SBT.SIZE,d0
		lea	(pt_bobtext-t,a5),a4
		move.l	d0,(RESA.SIZ,a4)
		bsr.l	reserve.mem
		bne	.exit

		*/ Init Les structures */
		move.l	(pt_tb-t,a5),a2
		move.w	(tb.lx-t,a5),d6
		move.l	(pt_bobtext-t,a5),a1
.loopinit	moveq	#SBOB.TEXT,d0
		bsr	sbob.only
		bne	.exit
		bsr	ssha.only
		bne	.exit
		clr.l	(SBOB.ACTIF,a1)
.nnn		move.b	(a2)+,d0
		beq.s	.nnn
		sub.b	#'A',d0
		lsl.w	#3,d0
		move.l	(SBOB.TABLE,a1),a0	a0=table adr. gfx & msk
		move.w	d0,(SBOB.PTANIM,a1)
		move.l	(0,a0,d0),(SBOB.GFX,a1)
		move.l	(4,a0,d0),(SBOB.MSK,a1)
		lea	(SBT.SIZE,a1),a1
		dbf	d6,.loopinit

		*/ Init structures */
		moveq	#0,d7			d7=pt/table sinus
		move.l	(pt_bobtext-t,a5),a1
		move.w	(tb.l1-t,a5),d6
		move.w	#200,d2
		bsr.s	.sex
		move.w	(tb.l2-t,a5),d6
		move.w	#220,d2
.sex		moveq	#0,d5
		move.w	(tb.la-t,a5),d5
		sub.w	#16,d5
		divu	d6,d5
		moveq	#0,d4
		move.w	d6,d3
		addq.w	#1,d3
		lsl.w	#4,d3
		sub.w	(tb.la-t,a5),d3
		neg.w	d3
		lsr.w	#1,d3			d3=Pos. X
.lo1		move.w	d4,(SBOB.POSX,a1)	X départ
		move.w	d2,(SBOB.POSY1,a1)
		move.w	d3,(SBOB.POSX1,a1)	X arrivé
		move.w	d7,(SBOB.POSY2,a1)
		addq.w	#8,d7
		and.w	#1022,d7
		clr.w	(SBOB.POSX2,a1)
		cmp.w	d4,d3			X arrivé > X départ ?
		beq.s	.oks
		bhi.s	.sup
		move.w	#-1,(SBOB.POSX2,a1)
		bra.s	.oks
.sup		move.w	#1,(SBOB.POSX2,a1)
.oks		add.w	d5,d4
		add.w	#16,d3
		lea	(SBT.SIZE,a1),a1
		dbf	d6,.lo1
		moveq	#0,d7
.exit		rts	
