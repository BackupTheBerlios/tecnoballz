*=========================================================================*
*»»»»»»»»»»»»»»»»»»»»» Scrolling écran des Gardiens ««««««««««««««««««««««*
*=========================================================================*
* Entrée	=> d7.w	déplacement positif / négatif
*		=> a5.l	Adr. table 't'
*		=> a6.l	CUSTOM
last_scroll:
		*/* Recopie dans l'écran d'affichage */*
		tst.w	(flag_co-t,a5)		modification précédent écran ?
		beq.s	.loupgarou		-> non
		lea	(BLTAPTH,a6),a2
		lea	(BLTDPTH,a6),a3
		lea	(BLTSIZE,a6),a4
		move.l	(ecran3-t,a5),d0
		move.l	(ecran_travail-t,a5),d1
		moveq	#0,d2
		move.w	(ffecran-t,a5),d2	adresse relative écran
		add.l	d2,d0
		add.l	d2,d1
		move.l	(mod_co-t,a5),d3	modulo A & D
		moveq	#-1,d4
		move.l	#$09F00000,d5		combinaison blitter
		moveq	#0,d6
		move.w	d6,(flag_co-t,a5)	RAZ flag modification
		WAITBLITTER
		move.l	d4,(BLTAFWM,a6)
		move.l	d5,(BLTCON0,a6)
		move.l	d3,(BLTAMOD,a6)
		move.w	(win_co-t,a5),d3	d3=fenêtre blitter
		move.l	d0,(a2)
		move.l	d1,(a3)
		move.w	d3,(a4)			Plan 1
		moveq	#0,d4
		move.w	(splan_co-t,a5),d4
		add.l	d4,d0
		add.l	d4,d1
		WAITBLITTER
		move.l	d0,(a2)
		move.l	d1,(a3)
		move.w	d3,(a4)			Plan 2
		add.l	d4,d0
		add.l	d4,d1
		WAITBLITTER
		move.l	d0,(a2)
		move.l	d1,(a3)
		move.w	d3,(a4)			Plan 3

		*/ Test s'il ya défilement de l'écran */
.loupgarou	tst.w	d7
		beq	.carbalas		-> pas de défilement

		*/ Calcul nouvelle ligne changement des pts vidéos */
		move.w	(s_nligne-t,a5),d3	d3=272
		move.w	(nu_video-t,a5),d1	0 « d1 « 271
		add.w	d7,d1			NU_VIDEO=NU_VIDEO+n
.etra_006	tst.w	d1
		bpl.s	.etra_007		IF NU_VIDEO<0 THEN
		add.w	d3,d1 			NU_VIDEO=NU_VIDEO+272
		bra.s	.etra_008
.etra_007	cmp.w	d3,d1			IF NU_VIDEO>272 THEN
		blt.s	.etra_008 		NU_VIDEO=NU_VIDEO-272
		sub.w	d3,d1
.etra_008	move.w	d1,(nu_video-t,a5)
		beq.s	.etra_nul		-> NU_VIDEO=0
		sub.w	(s_nligne-t,a5),d1
		neg.w	d1			AFLINE=(272-NU_VIDEO)*200
		mulu	(s_tligne-t,a5),d1
.etra_nul	move.w	d1,(afline-t,a5)	ADRESSE 1ERE LIGNE BALAYEE

		*/ test s'il ya réaff. de nouvelles dalles */
		move.l	(mapeditor-t,a5),d1	pt/1er ligne map-editor 
		move.l	(asc_end-t,a5),d2	pt/dernière ligne map-editor
		moveq	#M_LARGE*2,d3		taille ligne map-editor
		lea	(sc_map1-t,a5),a1	pt/1er ligne écran map-editor

		*/ met à jour la position Y sur la page écran (0 à 271) */
		move.w	(sc_y1-t,a5),d0		lit pos. y sur la page écran
		move.w	d0,d4
		sub.w	d7,d4			SC_Y1=SC_Y1-n
		bpl.s	.non_neg
		add.w	(s_nligne-t,a5),d4	IF Y1<0 THEN Y1=Y1+272
.non_neg	cmp.w	(s_nligne-t,a5),d4
		blt.s	.non_dep
		sub.w	(s_nligne-t,a5),d4	IF Y1»272 THEN Y1=Y1-272
.non_dep	move.w	d4,(sc_y1-t,a5)

		*/ test si apparition de nouvelle dalles */
		move.w	d4,d5			d5=SC_Y1
		lsr.w	#4,d0			divise par 16
		lsr.w	#4,d5			divise par 16
		cmp.w	d5,d0			reaff. de dalles ?
		beq	.carbalas
		tst.w	d7			n<1
		bmi	.carba			-> dalles apparaissent en bas

		*/* Apparition de nouvelle dalles en haut */*
		* décrèmente pointeur map-editor d'une seule ligne */
		cmp.l	(a1),d1			1er ligne ?
		bne.s	.no_fzi			-> non
		move.l	d2,(a1)			oui, pointe dernière ligne
.no_fzi		sub.l	d3,(a1)
		move.l	(a1),a0
		*- calcul adr. d'affichage -*
		lsl.w	#4,d5
		mulu	(s_tligne-t,a5),d5
		moveq	#0,d0
		move.w	d5,d0			adr. relative d'aff.
		bra.s	.sc_affiche

		*/* Apparition de nouvelle dalles en bas */*
		* incrèmente pointeur map-editor d'une seule ligne *
.carba		add.l	d3,(a1)
		cmp.l	(a1),d2			dernère ligne ?
		bhi.s	.no_fzy			-> non
		move.l	d1,(a1)			oui, pointe 1er ligne
.no_fzy		move.l	(a1),a0
		*/* les dalles apparaissent en bas additionne 16 lignes */*
		add.w	(m_haut-t,a5),a0
		cmp.l	d2,a0			dépassement ?
		blt.s	.no_dep3		-> non
		move.w	#1,(sh.f-t,a5)
		lea	(-M_LARGE*2*M_HAUTEUR,a0),a0
		* calcul adr. d'affichage *
.no_dep3	add.w	(s_nligne2-t,a5),d4	SC_Y1+256
		cmp.w	(s_nligne-t,a5),d4	dépassement ?  
		blt.s	.non_dep2		-> non
		sub.w	(s_nligne-t,a5),d4
.non_dep2	and.w	#$FFF0,d4		multiple de 16
		mulu	(s_tligne-t,a5),d4
		moveq	#0,d0
		move.w	d4,d0			adr. relative écran


*-------------------------------------------------------------------------*
*                 AFFICHAGE DES 20 DALLES DANS L'ECRAN 3		  *
*-------------------------------------------------------------------------*
		* initialise registres Blitter *
.sc_affiche	moveq	#-1,d1
		move.l	#$09F00000,d2		combinaison Blitter
		move.l	(mod_af-t,a5),d3	modulo A & D
		moveq	#0,d6
		move.w	d0,(ffecran-t,a5)
		add.l	(ecran3-t,a5),d0
		move.l	(sc_mapadr-t,a5),a1	pt/table pt/dalles
		WAITBLITTER
		move.l	d1,(BLTAFWM,a6)
		move.l	d2,(BLTCON0,a6)
		move.l	d3,(BLTAMOD,a6)
		*/- init. registre MC68000 -*/
		lea	(BLTAPTH,a6),a2
		lea	(BLTDPTH,a6),a3
		lea	(BLTSIZE,a6),a4
		move.l	a6,-(a7)
		lea	(DMACONR,a6),a6
		move.l	(adalles-t,a5),d1	adresse de la page dalle
		move.w	#16*64+1,d3		fenêtre blitter
		moveq	#0,d4
		move.w	(s_tplan2-t,a5),d4
		moveq	#0,d5
		move.w	(d_tplan-t,a5),d5	taille d'un plan (page dalle)
		moveq	#0,d6
		move.w	(splan_co-t,a5),d6	taille d'un plan (écran)
		moveq	#14,d7			Bit "BBUSY" de "DMACONR"

		REPT	M_LARGE			nbre de dalle en largeur
		move.w	(a0)+,d2		pt/adr. page dalle
		move.l	(a1,d2.w),d2		adr. de la dalle à afficher
		add.l	d1,d2			adr. de la page dalle
		WAIT_BLITTER
		move.l	d2,(a2)
		move.l	d0,(a3)
		move.w	d3,(a4)			PLAN 1
		add.l	d5,d2
		add.l	d6,d0
		WAIT_BLITTER
		move.l	d2,(a2)
		move.l	d0,(a3)
		move.w	d3,(a4)			PLAN 2
		add.l	d5,d2
		add.l	d6,d0
		WAIT_BLITTER
		move.l	d2,(a2)
		move.l	d0,(a3)
		move.w	d3,(a4)			PLAN 3
		sub.l	d4,d0
		ENDR
		move.l	(a7)+,a6

*-------------------------------------------------------------------------*
*                    Recopie dans l'écran d'affichage			  *
*-------------------------------------------------------------------------*
.ff1		move.l	(ecran3-t,a5),d0
		move.l	(ecran_travail-t,a5),d1
		moveq	#0,d2
		move.w	(ffecran-t,a5),d2	mot faible adr. desti./source
		add.l	d2,d0
		add.l	d2,d1
		move.w	#1,(flag_co-t,a5)	flag 1 = modification d'un écran
		move.w	(win_co-t,a5),d3	fenêtre blitter
		moveq	#0,d4
		move.w	(splan_co-t,a5),d4
		move.l	(mod_co-t,a5),d6
		WAITBLITTER
		move.l	d6,(BLTAMOD,a6)
		move.l	d0,(a2)
		move.l	d1,(a3)
		move.w	d3,(a4)			PLAN 1
		add.l	d4,d0
		add.l	d4,d1
		WAITBLITTER
		move.l	d0,(a2)
		move.l	d1,(a3)
		move.w	d3,(a4)			PLAN 2
		add.l	d4,d0
		add.l	d4,d1
		WAITBLITTER
		move.l	d0,(a2)
		move.l	d1,(a3)
		move.w	d3,(a4)			PLAN 3

*--------------*Calcul adresse des premiers pointeurs vidéo*--------------*
.carbalas	move.l	(pt_screen-t,a5),a0
		move.l	(sc_u1-t,a5),a1
		moveq	#0,d0
		move.w	(afline-t,a5),d0	adr. 1er ligne balayée
		add.l	(ecran_travail-t,a5),d0
		bsr.l	pointe.x

*---------------* Test si changement des pointeurs vidéos *---------------*
		move.l	(sc_w1-t,a5),a1
		move.l	#$FFFFFFFE,(a1)
		move.w	(nu_video-t,a5),d1	n° ligne chg. pts vidéos
		beq	.scroll_exit		-> pas de chg. pts vidéos
		cmp.w	(s_nligne2-t,a5),d1
		bge.s	.scroll_exit		-> pas de chg. pts vidéos
		*/ test si le n° "wait" est > 255 */
		move.l	#$01A00000,(a1)
		move.w	(2+coordy-t,a5),d0
		add.b	d0,d1
		bcc.s	.cancras
		move.l	#$FFE1FFFE,(a1)
.cancras	move.b	d1,(4,a1)
.scroll_exit	rts



* Pour un écran de 320 pixels, 256 lignes en 32 couleurs
* Page écran de 40 octets, 288 lignes sur 5 plans en Raw-Blitter
* Taille d'une ligne : 40*5=200 octets
*-------------------------------------------------------------------------*
*	0 « NU_VIDEO « 255 
*	si NU_VIDEO=0	=> pas de changement des pointeurs vidéo
*-------------------------------------------------------------------------*
*	si NU_VIDEO=0	=> AFLINE=0
*	sinon AFLINE=(272-NU_VIDEO)*200
*	Pour NU_VIDEO=001 => AFLINE=54200 (271*200)
*	Pour NU_VIDEO=010 => AFLINE=52400 (262*200)
*	Pour NU_VIDEO=128 => AFLINE=28800 (144*200)
*	Pour NU_VIDEO=255 => AFLINE=	 
