*-------------------------------------------------------------------------*
* "init_screen:" 	
* 	Entrée	=> a0.l : adresse struture écran
*		=> a5.l 't' ; => a6.l CUSTOM
* 	Sortie	<= a0.l	adresse de la structure écran
*		<= a1.l	pt/dernière instr. liste copper 1
*		<= a2.l	pt/dernière instr. liste copper 2
* Initialise un nouvel écran.
*-------------------------------------------------------------------------*
* "SCREEN:" 
* Structure écran.
*-------------------------------------------------------------------------*
* "screen_reserve:" 
*	Entrée	=> a5.l 't'
*	Sortie	<= a0.l adresse structure écran
* Réserve une structure écran (non initialisée).
*-------------------------------------------------------------------------*
* "screen_res:"
*	Entrée	=> a4.l structure 'RESA'
*		=> a5.l 't'
*	Sortie	<= a0.l adresse structure écran
* Réserve une structure écran (non initialisée).
*-------------------------------------------------------------------------*
* "convert_aga_color:"  
*	Entrée	=> a0.l adresse structure écran
* Convertit la palette "SCOLOR_TABLE" 32 bits au format AGA.
*-------------------------------------------------------------------------*
* "aga_color"
*	Entrée	=> d2.l	couleur 32 bits {X.X.R1.R2.V1.V2.B1.B2}
* 		X inutilisé / R1.R2 rouge / V1.V2 vert / B1.B2 bleu 
* 	Sortie	<= d0.w	R1.V1.B1 (valeur des bits 4 à 7 comme l'original-chip)
*		<= d1.w	R2.V2.B2 (valeur des bits 0 à 3)
* Convertit une couleur 32 bits au format AGA.
*-------------------------------------------------------------------------*
* "colors_on"
*	Entrée	=> a5.l 't'
* Met toutes les couleurs dans les registres hard (sans fondu).
*-------------------------------------------------------------------------*
* "set_colors:"
* 	Entrée	=> a0.l adr. structure écran
*		=> a6.l	CUSTOM
* Initialise les couleurs de la table 'SCOLOR_TABLE2" Copper ou registres.
*-------------------------------------------------------------------------*
* "aga_fade_on:"
* 	Entrée	=> a0.l adr. structure écran
* Initialise les couleurs avec fondu (appellée toutes les VBL).
*-------------------------------------------------------------------------*
* "aga_fade_off:"
* 	Entrée	=> a0.l adr. structure écran
* Met à zéro les couleurs avec fondu (appellée toutes les VBL).
*-------------------------------------------------------------------------*
* "raz_256colors:"
*	Entrée	=> a5.l 't' ; => a6.l CUSTOM
* Met à zéro les 256 regitres couleurs.
*-------------------------------------------------------------------------*
* "set_256colors:"
* 	Entrée	=> d0.w	couleur bit 4 à 7
*		=> d1.w couleur bit 0 à 3
*		=> a5.l 't' ; => a6.l CUSTOM
* Met la même valeur dans les 256 registres couleurs.
*-------------------------------------------------------------------------*
* "kill_screen:"
* 	Entrée	=> a0.l	Adr. de la structure écran
*		=> a5.l	Adr. table 't'
*		=> a6.l	'CUSTOM'
* 	Sortie	<= d7.l	¤ 00  pas d'erreur
*			¤-15 écran déjà détruit
* Détruit un écran.
*-------------------------------------------------------------------------*
* "vbl_fade_on:"
*	Entrée	=> a5.l	't'
* Appelle la routine d'initialisation des couleurs avec fondu.
*-------------------------------------------------------------------------*
* "vbl_fade_off:"
*	Entrée	=> a5.l	't'
* Appelle la routine de mise à zéro des couleurs avec fondu.
*-------------------------------------------------------------------------*
* "vbl_pointe_video"
*	Entrée	=> a5.l 't' ; => a6.l CUSTOM
* Modifie pointeurs vidéos de la liste Copper
*-------------------------------------------------------------------------*
* 'copy_s1_to_s2:'	> copie de l'écran 1 dans l'écran 2
* 'copy_s1_to_s2:'	> copie de l'écran 1 dans l'écran 2
* 'copy_s1_to_s2s3:'	> copie de l'écran 1 dans l'écran 2 et l'écran 3
* 'copy_s2_to_s1:'	> copie de l'écran 2 dans l'écran 1
* 'copy_s2_to_s3:'	> copie de l'écran 2 dans l'écran 3
* 'copy_s2_to_s1s3:'	> copie de l'écran 2 dans l'écran 1 et l'écran 3
* 'copy_s3_to_s1:'	> copie de l'écran 3 dans l'écran 1
* 'copy_s3_to_s2:'	> copie de l'écran 3 dans l'écran 2
* 'copy_s3_to_s1s2:'	> copie de l'écran 3 dans l'écran 1 et l'écran3
*	Entrée	=> a5.l	Adr. 't'
* Recopie d'un écran dans un ou deux autres.
*-------------------------------------------------------------------------*
* "clear_ecran1:"	> efface l'écran 1
* "clear_ecran2:"	> efface l'écran 2
* "clear_ecran3:"	> efface l'écran 3
*	Entrée	=> a5.l	Adr. 't'
* Efface un des 3 écrans.
*-------------------------------------------------------------------------*
* "pointe:"
* 	Entrée	=> d0.l	adr. écran
*		=> a0.l	adr. structure écran
* Ecrit les adresses écran pour les pointeurs videos de la liste Copper.
*-------------------------------------------------------------------------*
* "pointe_ecran1:"
* "pointe_ecran2:"
* "pointe_ecran3:"
*	Entrée	=> a5.l	Adr. 't'
* Pointe un des 3 écrans.
*-------------------------------------------------------------------------*
* "flip_ecrans:"
*	Entrée	=> a5.l	Adr. 't'
* Echange les 2 écrans pour faire du "double buffering".
*-------------------------------------------------------------------------*
* go_fade_on:
* go_fade_off:
*	Entrée	=> a5.l	Adr. 't'
* Lance le fade off/fade on
*-------------------------------------------------------------------------*
* decalage.aga:
* 	Entrée	=> d0.w	valeur du décalage hard de 0 à 255
* 	Sortie	<= d3.w	valeur de BPLCON1
* 	Utilise	-- (d0,d1,d2,d3)
* Calcule la valeur de BPLCON1
*-------------------------------------------------------------------------*
*
*-------------------------------------------------------------------------*
*********************** INITIALISE UN NOUVEL ECRAN ************************
*-------------------------------------------------------------------------*
* Entrée	=> a0.l	adresse de la structure écran
*		=> a5.l	adresse de la table 't'
*		=> a6.l	'CUSTOM'
* Sortie	<= d7.l ¤00  pas d'erreur
*			¤-1 dépassement mémoire
* 		<= a0.l	adresse de la structure écran
*		<= a1.l	pt/dernière instr. liste copper 1
*		<= a2.l	pt/dernière instr. liste copper 2
*		<= a5.l	adresse de la table 't'
*		??move	#DIWHIGH,(a4)+
*		??move	#$2101,(a4)+
init_screen:
		tst.w	(SFLAG,a0)		structure déjà intialisée ?
		bne	.deja_init		-> oui, erreur
		bsr	raz_256colors
		move.l	a0,(pt_screen-t,a5)
		bsr	change_freq
		bsr	raz_scolor_table2
		moveq	#0,d7
		bsr	limite_mouse
		bsr	color_number		|-> combien de couleurs
		bsr	test_dual
		bsr	test_aga_mode
		bsr	verify_width
		bsr	calcul_valeur
		bsr	reserve_ecran
		bne	.exit
		bsr	reserve_copper
		bne	.exit
		bsr	calcul_color
		bsr	calcul_sprxpt
		bsr	calcul_bplxpth
		bsr	calcul_bplcon0
		bsr	calcul_bplcon1
		bsr	calcul_bplcon2
		bsr	calcul_bplcon4
		bsr	calcul_fmode
		bsr	calcul_bplmod
		bsr	calcul_display_window
		bsr	calcul_display_fetch
		bsr	special_vscroll
		moveq	#0,d7
		move.l	#$FFFFFFFE,(a1)		Fin de la liste Copper
		tst.w	(NBCOPPER,a0)		Une seule liste Copper ?
		beq.s	.one_liste		-> Oui
		move.l	#$FFFFFFFE,(a2)		Fin de la liste Copper
.one_liste	move.l	(SACOPPER,a0),d0
		move.l	d0,(COP1LCH,a6)
		move.l	d0,(acopper-t,a5)
		moveq	#0,d0
		move.w	d0,(COPJMP1,a6)
		move.w	(SBPLCON3,a0),(BPLCON3,a6)
		move.w	#$8380,(DMACON,a6)	active DMA copper + plans
		or.w	#$0180,(dmacon-t,a5)
		move.l	(SPECRAN1,a0),(ecran_travail-t,a5)
		move.l	(SPECRAN1,a0),(ecran_affiche-t,a5)
		move.w	#1,(SFLAG,a0)		flag struct. init.
		moveq	#0,d7
.exit:		rts
.deja_init	moveq	#-6,d7			¤ structure écran dèjà init.
		rts

*------------------* init screen : Change la fréquence *------------------*
* entrée	=> a0.l	adr. structure écran
*		=> a5.l adr. 't'
*		=> a6.l CUSTOM
change_freq:	move.w	(SFREQ,a0),d0
		subq.w	#1,d0
		beq.s	.force_pal
		subq.w	#1,d0
		beq.s	.force_ntsc
.exit		rts
.force_pal	move.w	#50,(frequence-t,a5)
		move.w	#$0020,(BEAMCON0,a6)
		bra.s	.exit
.force_ntsc	move.w	#60,(frequence-t,a5)
		move.w	#$0000,(BEAMCON0,a6)
		bra.s	.exit

*------------------* init screen : RAZ 'SCOLOR_TABLE2' *------------------*
* entrée	=> a0.l	adr. structure écran
raz_scolor_table2:
		lea	(SCOLOR_TABLE2,a0),a4
		moveq	#0,d1
		move.w	#256-1,d0
.loop		move.l	d1,(a4)+
		dbf	d0,.loop
.exit		rts

*----------* init screen : sauve coord. maxi. pointeur souris *-----------*
* entrée	=> a0.l	adr. structure écran
*		=> a5.l	adr. 't'
limite_mouse:
		move.w	(SNPIXEL,a0),(mouse_maxix-t,a5)
		move.w	(SNLIGNE,a0),(mouse_maxiy-t,a5)
		clr.w	(mouse_minix-t,a5)
		clr.w	(mouse_miniy-t,a5)
		rts

*---------* init screen : test le mode d'affichage plans/lutins *---------*
test_aga_mode:
		and.w	#3,(SFMODE_PLANS,a0)
		and.w	#3,(SFMODE_LUTINS,a0)
		and.w	#3,(SLUTIN_RES,a0)
		cmp.w	#1,(SFMODE_PLANS,a0)
		bne.s	.ok_x2
		move	#2,(SFMODE_PLANS,a0)
.ok_x2		cmp.w	#1,(SFMODE_LUTINS,a0)
		bne.s	.ok32
		move.w	#2,(SFMODE_LUTINS,a0)
.ok32		cmp.w	#1,(SLUTIN_RES,a0)
		bne.s	.ok_low
		clr.w	(SLUTIN_RES,a0)
.ok_low		move.w	(SLUTIN_RES,a0),d0
		lsl.w	#6,d0
		or.w	d0,(SBPLCON3,a0)
		rts

*------------* init screen : vérifie la largeur d'une ligne *-------------*
verify_width:
		lea	(.burstx1,pc),a4
		tst.w	(SFMODE_PLANS,a0)	mode burstX1 ?
		beq.s	.routine		-> oui
		lea	(.burstx4,pc),a4
		cmp.w	#3,(SFMODE_PLANS,a0)	mode burstX4 ?
		beq.s	.routine		-> oui
		lea	(.burstx2,pc),a4
.routine	lea	(SPLARGE,a0),a3
		jsr	(a4)			|-> largeur plans paires
		lea	(SILARGE,a0),a3
		jsr	(a4)			|-> largeur plans impaires
		rts
		* Mode burstX1 (Original Chips) largeur aligné sur un mot *
.burstx1	move.w	(a3),d0			d0=largeur en pixels
		move.w	d0,d1
		and.w	#$F,d1
		beq.s	.ok1
		and.w	#$FFF0,d0
		add.w	#16,d0
		move.w	d0,(a3)
.ok1		rts
		* Mode burstX2 (AGA Chips) largeur aligné sur un long mot *
.burstx2	move.w	(a3),d0			d0=largeur en pixels
		move.w	d0,d1
		and.w	#$1F,d1
		beq.s	.ok2
		and.w	#$FFE0,d0
		add.w	#32,d0
		move.w	d0,(a3)
.ok2		rts
		* Mode burstX4 (AGA Chips) largeur aligné sur double long mot *
.burstx4	move.w	(a3),d0			d0=largeur en pixels
		move.w	d0,d1
		and.w	#$3F,d1			multiple de 64 ?
		beq.s	.ok4			-> oui
		and.w	#$FFC0,d0		multiple inférieur
		add.w	#64,d0			multiple supérieur
		move.w	d0,(a3)
.ok4		rts

*----------* init screen : calcul nombre de registres couleurs *----------*
* entrée	=> a0.l	adr. structure écran
color_number:	moveq	#1,d1			nbre de couleur
		moveq	#0,d0
		move.b	(SNPLAN,a0),d0		nbre de plan
		subq.w	#1,d0
.loop		add.w	d1,d1
		dbf	d0,.loop
		subq	#1,d1
		move.w	d1,(NREGISTRE,a0)	nombre reg. - 1
		rts

*------------------* init_screen : test Dual-Playfield *------------------*
* entrée	=> a0.l	adr. structure écran
test_dual:
		clr.w	(SIPLAN,a0)		raz nombre plan impaire
		moveq	#0,d0
		move.b	(SNPLAN,a0),d0		d0=nbre de plan total
		move.w	d0,(SPPLAN,a0)		nombre de plan paire
		tst.b	(SDBPLF,a0)		dual playfield ?
		beq.s	.exit			-> non
		cmp.w	#1,d0			un seul plan ?
		beq.s	.exit			-> oui pas de dual playfield
		move.w	d0,d1
		lsr.w	#1,d1
		move.w	d1,(SIPLAN,a0)		nombre plan impaire
		btst	#0,d0			nombre de plans totals paire ?
		beq.s	.paire			-> oui
		addq.w	#1,d1
.paire		move.w	d1,(SPPLAN,a0)		nombre de plans paires
.exit		rts

*-----------------* init screen : calcul valeurs écran *------------------*
* entrée	=> a0.l	adr. structure écran
calcul_valeur:
		* plans paires si dual *
		move.w	(SPPLAN,a0),d1		d1=nbre de plan
		move.w	(SPLARGE,a0),d0		d0=largeur page en pixel
		lsr.w	#3,d0			d0=largeur page en octet
		move.w	d0,(SP_SPLAN,a0)	sauve taille & largeur d'un plan
		mulu	d1,d0
		move.w	d0,(SP_SLINE,a0)	sauve taille d'une ligne
		mulu	(SPHAUTEUR,a0),d0	d0=taille page écran en octet
		moveq	#0,d1
		move.w	(SP_HSCROLL,a0),d1	d1=Nbre pixel de défilement horz.
		beq.s	.no_hscroll		-> Pas de défilement horz.
		sub.w	(SPLARGE,a0),d1
		add.w	(SPLEFT,a0),d1
		add.w	(SPRIGHT,a0),d1
		lsr.w	#3,d1			d1=Nombre d'octets
		and.w	#$FFF8,d1		multiple de 8 ?
		beq.s	.good			-> oui
		addq.w	#8,d1
.good		addq.w	#8,d1
		add.l	d1,d0
		add.l	d1,d0
.no_hscroll	move.l	d0,(SP_SIZE,a0)		sauve taille octet d'un écran
		move.w	(SPUP,a0),d0		d0=nbre lignes cachées en haut
		mulu	(SP_SLINE,a0),d0
		moveq	#0,d1
		move.w	(SPLEFT,a0),d1		d1=Nbre pixels cachés à gauche
		lsr.w	#3,d1			Convertit en octet
		and.w	#$FFFE,d1		Aligne sur un mot
		tst.w	(SFMODE_PLANS,a0)
		beq.s	.xx
		and.w	#$FFFC,d1		Aligne sur un long mot
		cmp.w	#3,(SFMODE_PLANS,a0)
		bne.s	.xx
		and.w	#$FFF8,d1		Aligne sur un double long mot
.xx		add.l	d1,d0
		move.l	d0,(SPOFFSET,a0)

		* plans impaires si dual *
		move.w	(SIPLAN,a0),d1		d1=nbre de plan
		beq.s	.exit
		move.w	(SILARGE,a0),d0		d0=largeur page en pixel
		lsr.w	#3,d0			d0=largeur page en octet
		move.w	d0,(SI_SPLAN,a0)	sauve taille & largeur d'un plan
		mulu	d1,d0
		move.w	d0,(SI_SLINE,a0)	sauve taille d'une ligne
		mulu	(SPHAUTEUR,a0),d0	d0=taille page écran en octet
		move.l	d0,(SI_SIZE,a0)
		move.w	(SIUP,a0),d0		d0=nbre lignes cachées en haut
		mulu	(SI_SLINE,a0),d0
		moveq	#0,d1
		move.w	(SILEFT,a0),d1		d1=Nbre pixels cachés à gauche
		lsr.w	#3,d1			Convertit en octet
		and.w	#$FFFE,d1		Octet paire
		tst.w	(SFMODE_PLANS,a0)
		beq.s	.ww
		and.w	#$FFFC,d1		Aligne sur un long mot
		cmp.w	#3,(SFMODE_PLANS,a0)
		bne.s	.ww
		and.w	#$FFF8,d1		Aligne sur un double long mot
.ww		add.l	d1,d0
		move.l	d0,(SIOFFSET,a0)
.exit		rts

*----------------* init. screen : réserve mémoire écran *-----------------*
* entrée	=> a0.l	adr. structure écran
* sortie	<= d7.l = 0 / Z=1 pas d'erreur
reserve_ecran:
		* PLAN PAIRES *
		and.w	#3,(SPBUFFER,a0)	3 buffers écrans maximums
		bne.s	.non_nul
		move.w	#1,(SPBUFFER,a0)
.non_nul	move.l	(SP_SIZE,a0),d3		nbre d'octet à allouer
		move.w	(SPBUFFER,a0),d4	d4 nbre de buffer
		lea	(SPECRAN1,a0),a4
		bsr.s	.reserve_mem
		bne.s	.exit
		lea	(SPECRAN2,a0),a4
		bsr.s	.reserve_mem
		bne.s	.exit
		lea	(SPECRAN3,a0),a4
		bsr.s	.reserve_mem
		bne.s	.exit

		* PLANS IMPAIRES (SI DUEL-PLAYFIELD) *
		tst.b	(SDBPLF,a0)		dual playfield ?
		beq.s	.exit			-> non
		and.w	#3,(SIBUFFER,a0)	3 buffers écrans maximums
		bne.s	.non_nul2
		move.w	#1,(SIBUFFER,a0)
.non_nul2	move.l	(SI_SIZE,a0),d3		nbre d'octet à reserver
		beq.s	.exit
		move.w	(SIBUFFER,a0),d4	d4 nbre de buffer
		lea	(SIECRAN1,a0),a3
		bsr.s	.reserve_mem
		bne.s	.exit
		lea	(SIECRAN2,a0),a3
		bsr.s	.reserve_mem
		bne.s	.exit
		lea	(SIECRAN3,a0),a3
		bsr.s	.reserve_mem
.exit		rts

		* Réserve la mémoire écran *
* entrée	=> d3.l	Taille à reserver
*		=> d4.w	Nbre de buffer écran de 1 à 3
*		=> a3.l	Pt/pt adresse écran
.reserve_mem	subq.w	#1,d4
		bmi.s	.zz
		move.l	d3,(4.w,a4)
		move.l	#'SCRN',(8.w,a4)
		move.l	#MEMF_CHIP,(12.w,a4)
		bsr	reserve.mem
		bne.s	.end
.zz		moveq	#0,d7
.end		rts

*----------* init. screen : reserve mémoire de la liste Copper *----------*
* entrée	=> a0.l	adr. structure écran
reserve_copper:
		* caclul place pour les BPLPT *
		moveq	#0,d1
		move.b	(SNPLAN,a0),d1		nbre de plan
		lsl.w	#3,d1			*8 (8 octets par plan)
		move.l	d1,d0

		* calcul place pour les "MOVES" couleurs *
		tst.w	(SCOLOR.F,a0)		couleur dans la liste Copper ?
		beq.s	.next			-> non
		moveq	#0,d1
		move.b	(SNPLAN,a0),d1		d1=nbre de plan
		add.w	(.colors-4,pc,d1*2),d0	+ "Moves" couleurs
		bra	.next
.colors		dc.w	4*4			1 PLAN  (  4 MOVES)
		dc.w	10*4			2 PLANS ( 10 MOVES)
		dc.w	18*4			3 PLANS ( 18 MOVES)
		dc.w	34*4			4 PLANS ( 34 MOVES)
		dc.w	66*4			5 PLANS ( 66 MOVES)
		dc.w	132*4			6 PLANS (132 MOVES)
		dc.w	264*4			7 PLANS (264 MOVES)
		dc.w	528*4			8 PLANS (528 MOVES)

		* autres "MOVES" *
.next		add.w	#4*5,d0			+ 5 "Moves" BPLCON0 à BPLCON4
		add.w	#4*2,d0			+ 2 "Moves" BPLMOD0 & BPLMOD1
		add.w	#4*2,d0			+ 2 "Moves" DIWSTRT & DIWSTOP
		add.w	#4*2,d0			+ 2 "Moves" DDFSTRT & DDFSTOP
		add.w	#4*1,d0			+ 1 "Move"  FMODE
		add.w	#4*01,d0		+ 01 "Wait" ($ff,$fe)
		tst.w	(SLUTIN,a0)		Pts lutins ?
		beq.s	.no_pts_lutins		-> Non
		add.w	#16*4,d0		16 "Moves" pour 8 lutins
.no_pts_lutins	tst.w	(SP_VSCROLL,a0)		Défilement vertical ?
		beq.s	.no_vscroll		-> Non
		add.w	#4*02,d0		+ 02 "Waits"
		add.w	d1,d0			+ "Moves" pointeurs vidéos
.no_vscroll	moveq	#0,d1
		move.w	(KKCOPPER,a0),d1
		add.l	d1,d0			+octets en + demandés
		*/ Reserve Liste Copper 1 */
		lea	(SACOPPER,a0),a4
		move.l	d0,(STCOPPER,a0)	sauve taille liste Copper
		move.l	#'COPP',(8.w,a4)
		move.l	#MEMF_CHIP,(12.w,a4)
		bsr	reserve.mem
		bne.s	.sexy
		move.l	(a4),a1
		*/ Reserve Liste Copper 2 */
		tst.w	(NBCOPPER,a0)		Une seule liste Copper ?
		beq.s	.one_liste		-> Oui
		move.l	(STCOPPER,a0),d0
		lea	(SACOPPER2,a0),a4
		move.l	d0,(4.w,a4)
		move.l	#'COPP',(8.w,a4)
		move.l	#MEMF_CHIP,(12.w,a4)
		bsr	reserve.mem
		bne.s	.sexy
		move.l	(a4),a2
.one_liste	moveq	#0,d7
.sexy		rts

*----------* init. screen : installe les "Moves" couleurs AGA *-----------*
* entrée	=> a0.l	adr. structure écran
*		=> a1.l	adr. liste copper 1
*		=> a2.l	adr. liste copper 2
calcul_color:
		tst.w	(SCOLOR.F,a0)		"colors-moves-copper" ?
		beq.s	.exit			-> non
		move.l	a1,(SACOLOR1,a0)
		move.l	a1,a4
		bsr.s	.routine
		move.l	a4,a1
		tst.w	(NBCOPPER,a0)		Une seule liste Copper ?
		beq.s	.exit			-> Oui
		move.l	a2,(SACOLOR2,a0)
		move.l	a2,a4
		bsr.s	.routine
		move.l	a4,a2
.exit		rts

		* installe les "moves" chargeant les couleurs *
.routine	moveq	#0,d2			sélect. 1er banc des 8 bancs de couleurs
		move.w	(NREGISTRE,a0),d1	nombre de couleurs - 1
		beq.s	.end
		move.w	(SBPLCON3,a0),d5
.loop_color	moveq	#32-1,d3		32 regitres couleurs
		move.w	#COLOR00,d0
		move.w	#BPLCON3,(a4)+
		move.w	d5,(a4)			sélection bits 4 à 7 RVB
		or.w	d2,(a4)+		+ banc registre
		move.w	d1,d4
.bits4a7	move.w	d0,(a4)+
		clr.w	(a4)+
		addq.w	#2,d0
		subq.w	#1,d4
		bmi.s	.cont
		dbf	d3,.bits4a7
.cont		moveq	#32-1,d3		32 regitres couleurs
		move.w	#COLOR00,d0
		move.w	#BPLCON3,(a4)+
		move.w	d5,(a4)
		or.w	#$0200,(a4)		sélectionne bits 0 à 3 RVB
		or.w	d2,(a4)+		+ banc registre
.bits0a3	move.w	d0,(a4)+
		clr.w	(a4)+
		addq.w	#2,d0			n° registre suivant
		subq.w	#1,d1
		bmi.s	.end
		dbf	d3,.bits0a3
		add.w	#$2000,d2		prochain banc de couleurs
		bra.s	.loop_color
.end		rts

*------* init. screen : rajoute BPLxPT pour le défilement vertical *------*
* entrée	=> a0.l	adr. structure écran
*		=> a1.l	adr. liste copper 1
*		=> a2.l	adr. liste copper 2
special_vscroll:
		tst.w	(SP_VSCROLL,a0)		Défilement vertical ?
		beq.s	.exit			-> Non
		move.l	a1,a4
		move.l	a4,(SPWAIT1,a0)
		bsr.s	.routine
		move.l	a4,(SPSCROLLPT,a0)
		move.l	a4,a1
		move.l	(SPECRAN1,a0),d0	d0=adresse écran
		bsr	plans_paires
		move.l	a4,a1
		tst.w	(NBCOPPER,a0)		Une seule liste Copper ?
		beq.s	.exit			-> Oui
		move.l	a2,a4
		move.l	a4,(SPWAIT2,a0)
		bsr.s	.routine
		move.l	a4,a2
		move.l	a4,(SPSCROLLPT2,a0)
		move.l	(SPECRAN2,a0),d0	d0=adresse écran
		bsr	plans_paires
		move.l	a4,a2
.exit		rts
.routine	move.l	#$FFFFFFFE,(a4)+
		move.l	#$0001FFFE,(a4)+
		rts

*-------------* init. screen : calcul les pointeurs lutins *--------------*
* entrée	=> a0.l	adr. structure écran
*		=> a1.l	adr. liste copper 1
*		=> a2.l	adr. liste copper 2
calcul_sprxpt:	tst.w	(SLUTIN,a0)		pointeurs lutins ?
		beq.s	.exit			-> non
		move.l	a1,(SALUTIN1,a0)
		move.l	a1,a4
		bsr.s	.routine
		move.l	a4,a1
		tst.w	(NBCOPPER,a0)		Une seule liste Copper ?
		beq.s	.exit			-> oui
		move.l	a2,(SALUTIN2,a0)
		move.l	a2,a4
		bsr.s	.routine
		move.l	a4,a2
.exit		rts
.routine	moveq	#(8*2)-1,d0		16 pts lutins pour 8 lutins
		move.w	#SPR0PTH,d1
		moveq	#0,d2
.loop		move	d1,(a4)+
		move	d2,(a4)+
		addq	#2,d1
		dbf	d0,.loop
		rts

*--------------------* init. screen : calcul BPLxPT *---------------------*
* entrée	=> a0.l	adr. structure écran
* 		=> a0.l	adr. structure écran
*		=> a1.l	adr. liste Copper 1
*		=> a2.l	adr. liste Copper 2
calcul_bplxpth:
		move.l	a1,a4
		move.l	a4,(SPPOINTEUR,a0)
		move.l	(SPECRAN1,a0),d0	d0=adresse écran
		bsr	plans_paires
		tst.b	(SDBPLF,a0)		dual playfield ?
		beq.s	.no_dual		-> non
		move.l	a4,(SIPOINTEUR,a0)
		move.l	(SIECRAN1,a0),d0	d0=adresse écran
		bsr	plans_impaires
.no_dual	move.l	a4,a1
		tst.w	(NBCOPPER,a0)		Une seule liste Copper ?
		beq.s	.exit			-> Oui
		move.l	a2,a4
		move.l	a4,(SPPOINTEUR2,a0)
		move.l	(SPECRAN2,a0),d0	d0=adresse écran
		bsr	plans_paires
		tst.b	(SDBPLF,a0)		dual playfield ?
		beq.s	.no_dual2		-> non
		move.l	a4,(SIPOINTEUR2,a0)
		move.l	(SIECRAN2,a0),d0	d0=adresse écran
		bsr	plans_impaires
.no_dual2	move.l	a4,a2
.exit		rts

*------------* init screen : calcul BPLxPT des plans paires *-------------*
* entrée	=> d0.l	adresse écran
* 		=> a0.l	adr. structure écran
*		=> a1.l	adr. liste Copper 1
*		=> a2.l	adr. liste Copper 2
plans_paires:	
		moveq	#2,d7
		tst.b	(SDBPLF,a0)		dual playfield ?
		beq.s	.no_dual		-> non
		moveq	#2+4,d7
.no_dual	move.w	(SPPLAN,a0),d2		d2=Nbre de plan
		subq.w	#1,d2
		add.l	(SPOFFSET,a0),d0
		moveq	#0,d1
		move.w	(SP_SPLAN,a0),d1	taille d'un plan raw-blitter
		move.w	#BPL1PTH,d3
.loop		move.w	d3,(a4)+		pointeur haut
		addq.w	#2,d3
		swap	d0
		move.w	d0,(a4)+		adresse haute
		move.w	d3,(a4)+		pointeur bas
		add.w	d7,d3
		swap	d0
		move.w	d0,(a4)+		adresse basse
		add.l	d1,d0			d0 = adresse prochain plan
		dbf	d2,.loop
		rts

*-----------* init screen : calcul BPLxPT des plans impaires *------------*
* entrée	=> d0.l	adr. écran
* 		=> a0.l	adr. structure écran
*		=> a1.l	adr. liste Copper 1
*		=> a2.l	adr. liste Copper 2
plans_impaires:	
		tst.b	(SDBPLF,a0)		dual playfield ?
		beq.s	.no_dual		-> non
		move.w	(SIPLAN,a0),d2		d2=nbre de plan
		subq.w	#1,d2
		add.l	(SIOFFSET,a0),d0
		moveq	#0,d1
		move.w	(SI_SPLAN,a0),d1	taille d'un plan raw-blitter
		move.w	#BPL2PTH,d3
.loop		move.w	d3,(a4)+		pointeur haut
		addq.w	#2,d3
		swap	d0
		move.w	d0,(a4)+		adresse haute
		move.w	d3,(a4)+		pointeur bas
		addq.w	#6,d3
		swap	d0
		move.w	d0,(a4)+		adresse basse
		add.l	d1,d0			d0 = adresse prochain plan
		dbf	d2,.loop
.no_dual	rts

*-------------------* init. screen : calcul 'BPLCON0' *-------------------*
* entrée	=> a0.l	adr. structure écran
*		=> a1.l	adr. liste Copper 1
*		=> a2.l	adr. liste Copper 2
calcul_bplcon0:
		move.l	a1,a4
		bsr.s	.routine		|-> Liste Copper 1
		move.l	a4,a1
		tst.w	(NBCOPPER,a0)		Une seule liste Copper ?
		beq.s	.exit			-> Oui
		move.l	a2,a4
		bsr.s	.routine		|-> Liste Copper 2
		move.l	a4,a2
.exit		rts
.routine	moveq	#0,d0
		move.b	(SNPLAN,a0),d0		nbre de plan 0 à 8
.okplans	and.w	#$0007,d0		garde bit BPU0 à BPU2
		ror.w	#4,d0
		or.w	#$0200,d0		+ bit COLOR
		moveq	#0,d1
		move.b	(SHIRES,a0),d1		bit HIRES (haute résolution)
		ror.w	#1,d1
		or.w	d1,d0
		moveq	#0,d1
		move.b	(SHOMOD,a0),d1		bit HOMOD (mode HAM)
		ror.w	#5,d1
		or.w	d1,d0
		moveq	#0,d1
		move.b	(SDBPLF,a0),d1		bit DBPLF (mode dual-playfield)
		ror.w	#6,d1
		or.w	d1,d0
		moveq	#0,d1
		move.b	(SERSY,a0),d1		bit ERSY (synchro. externe)
		add.w	d1,d1
		or.w	d1,d0
		move.b	(SLACE,a0),d1		bit LACE (mode interlace)
		lsl.w	#2,d1
		or.w	d1,d0
		move.b	(SNPLAN,a0),d1		nbre de plan 0 à 8
		and.w	#$0008,d1		garde bit BPU 3
		add.w	d1,d1
		or.w	d1,d0			ecrit bit BPU 3
		tst.b	(SUPERHIRES,a0)		super haute résolution ?
		beq.s	.nosuperhires		-> non
		or.w	#$0040,d0		met bit6 = superhires
.nosuperhires	or.w	#$0001,d0		met le bit ESCENA
		tst.w	(genlock-t,a5)
		beq.s	.nogenlock
		or.w	#$0002,d0		Bit ERSY à 1
.nogenlock	move.w	#BPLCON0,(a4)+
		move.w	d0,(a4)+
		rts

*-------------------* init. screen : calcul 'BPLCON1' *-------------------*
* entrée	=> a0.l	adr. structure écran
*		=> a1.l	adr. liste Copper 1
*		=> a2.l	adr. liste Copper 2
calcul_bplcon1:
		move.l	a1,(SPDECALAGE,a0)
		move.l	a1,a4
		bsr.s	.routine		|-> Liste Copper 1
		move.l	a4,a1
		tst.w	(NBCOPPER,a0)		Une seule liste Copper ?
		beq.s	.exit			-> Oui
		move.l	a2,(SPDECALAGE2,a0)
		move.l	a2,a4
		bsr.s	.routine		|-> Liste Copper 2
		move.l	a4,a2
.exit		rts
.routine	move.w	#BPLCON1,(a4)+
		clr.w	(a4)+
		rts

*-------------------* init. screen : calcul 'BPLCON2' *-------------------*
* entrée	=> a0.l	adr. structure écran
*		=> a1.l	adr. liste Copper 1
*		=> a2.l	adr. liste Copper 2
calcul_bplcon2:
		move.l	a1,a4
		bsr.s	.routine		|-> Liste Copper 1
		move.l	a4,a1
		tst.w	(NBCOPPER,a0)		Une seule liste Copper ?
		beq.s	.exit			-> Oui
		move.l	a2,a4
		bsr.s	.routine		|-> Liste Copper 2
		move.l	a4,a2
.exit		rts
.routine	move.w	#BPLCON2,(a4)+
		move.w	(SBPLCON2,a0),(a4)
		or.w	#$0200,(a4)+		contre le half-bright 6 plans
		rts

*-------------------* init. screen : calcul 'BPLCON4' *-------------------*
* entrée	=> a0.l	adr. structure écran
*		=> a1.l	adr. liste Copper 1
*		=> a2.l	adr. liste Copper 2
calcul_bplcon4:
		move.l	a1,a4
		bsr.s	.routine		|-> Liste Copper 1
		move.l	a4,a1
		tst.w	(NBCOPPER,a0)		Une seule liste Copper ?
		beq.s	.exit			-> Oui
		move.l	a2,a4
		bsr.s	.routine		|-> Liste Copper 2
		move.l	a4,a2
.exit		rts
.routine	move.w	#BPLCON4,(a4)+
		move.w	(SBPLCON4,a0),(a4)+	base couleur lutins
		rts

*--------------------* init. screen : calcul 'FMODE' *--------------------*
* entrée	=> a0.l	adr. structure écran
*		=> a1.l	adr. liste Copper 1
*		=> a2.l	adr. liste Copper 2
calcul_fmode:
		move.l	a1,a4
		bsr.s	.routine		|-> Liste Copper 1
		move.l	a4,a1
		tst.w	(NBCOPPER,a0)		Une seule liste Copper ?
		beq.s	.exit			-> Oui
		move.l	a2,a4
		bsr.s	.routine		|-> Liste Copper 2
		move.l	a4,a2
.exit		rts
.routine	move.w	(SFMODE_PLANS,a0),d0
		cmp.w	#2,d0
		bne.s	.no2
		moveq	#1,d0
.no2		move.w	(SFMODE_LUTINS,a0),d1
		lsl.w	#2,d1
		or.w	d1,d0
		move.w	#FMODE,(a4)+
		move.w	d0,(a4)+
		rts

*---------------* init. screen : calcul BPL1MOD & BPL2MOD *---------------*
* entrée	=> a0.l	adr. structure écran
*		=> a1.l	adr. liste Copper 1
*		=> a2.l	adr. liste Copper 2
calcul_bplmod:	
		move.l	a1,a4
		bsr.s	.routine		|-> Liste Copper 1
		move.l	a4,a1
		tst.w	(NBCOPPER,a0)		Une seule liste Copper ?
		beq.s	.exit			-> Oui
		move.l	a2,a4
		bsr.s	.routine		|-> Liste Copper 2
		move.l	a4,a2
.exit		rts
.routine	move.w	(SNPIXEL,a0),d0		d0 = larg. écran en pixel
		lsr.w	#3,d0			d0 = larg. écran en octet
		move.w	(SPLARGE,a0),d7		d7 = larg. page1 en pixel
		lsr.w	#3,d7			d7 = larg. page1 en octet
		move.w	d7,d6			d6 = larg. page1 en octet
		moveq	#0,d2
		move.b	(SNPLAN,a0),d2		d2 = nbre de plan
		move.w	d2,d3			d3 = nbre de plan
		move.w	(SP_SLINE,a0),d4	d4 = taille d'une ligne
		move.w	d4,d5			d5 = taille d'une ligne

		* spécial écran dual playfield *
		tst.b	(SDBPLF,a0)		dual playfield ?
		beq.s	.no_dual		-> non
		lsr.w	#1,d3			d3 = nbre plan impaires
		sub.w	d3,d2			d2 = nbre plan paires
		move.w	(SILARGE,a0),d7		d7 = larg. page2 en pixel
		lsr.w	#3,d7			d7 = larg. page2 en octet
		move	(SI_SLINE,a0),d5	d5 = taille d'une ligne
.no_dual	mulu	d2,d7			d7 = larg. * nbre plan paire
		sub.w	d0,d7			d7 = modulo plans paires
		mulu	d3,d6			d6 = larg. * nbre plan impaire
		sub.w	d0,d6			d6 = modulo plans impaires

		* spécial écran interlace *
		tst.b	(SLACE,a0)		Ecran lignes interlacées ?
		beq.s	.no_interlace		-> Non
		add.w	d4,d6			Pour un écran interlacé...
		add.w	d5,d7			... on ajoute taille 1 ligne
.no_interlace	
		* sauve valeurs modulos *
		tst.w	(SHARDSCROLL.F,a0)	défilement hard ?
		beq.s	.no_hardscroll		-> non
		subq.w	#2,d6
		subq.w	#2,d7
		tst.w	(SFMODE_PLANS,a0)	mode X1
		beq.s	.no_hardscroll		-> oui
		subq.w	#2,d6
		subq.w	#2,d7
		cmp.w	#2,(SFMODE_PLANS,a0)	mode X2
		beq.s	.no_hardscroll		-> oui
		subq.w	#4,d6
		subq.w	#4,d7
.no_hardscroll	move.w	#BPL1MOD,(a4)+
		move.w	d6,(a4)+
		move.w	#BPL2MOD,(a4)+
		move.w	d7,(a4)+
		rts

*---------------* init. screen : calcul DIWSTRT / DIWSTOP *---------------*
* entrée	=> a0.l	adr. structure écran
*		=> a1.l	adr. liste Copper 1
*		=> a2.l	adr. liste Copper 2
calcul_display_window:
		move.l	a1,(SADIW1,a0)
		move.l	a1,a4
		bsr.s	.routine		|-> liste Copper 1
		move.l	a4,a1
		tst.w	(NBCOPPER,a0)		Une seule liste Copper ?
		beq.s	.exit			-> Oui
		move.l	a2,(SADIW2,a0)
		move.l	a2,a4
		bsr.s	.routine		|-> liste Copper 1
		move.l	a4,a2
.exit		rts
.routine	move.w	(SCOORDX,a0),d0
		beq.s	.calcul_startx
.cont1		move.w	(SCOORDY,a0),d1
		beq	.calcul_starty	
.cont2		addq.w	#1,d0
		move.w	d1,d2
		lsl.w	#8,d2
		add.b	d0,d2
		move.w	#DIWSTRT,(a4)+
		move.w	d2,(a4)+
		tst.w	(SDFFSTART,a0)
		beq.s	.no_hscroll		-> Non
		*tst.w	(SP_HSCROLL,a0)		Défilement horizontal ?
		*bne.s	.no_hscroll		-> Non
.noword		add.w	#16,(-2,a4)
		tst.w	(SFMODE_PLANS,a0)
		beq.s	.no_hscroll
		add.w	#16,(-2,a4)
		cmp.w	#2,(SFMODE_PLANS,a0)
		bne.s	.no_hscroll
		add.w	#16*2,(-2,a4)
.no_hscroll	move.w	(SNPIXEL,a0),d2		d2=largeur de l'écran
		tst.b	(SHIRES,a0)		écran haute-résolution ?
		beq.s	.no_hire		-> non
		lsr.w	#1,d2			d2=largeur écran/2
.no_hire	tst.b	(SUPERHIRES,a0)		écran super-haute-résolution ?
		beq.s	.no_superhire		-> non
		lsr.w	#2,d2			d2=largeur écran/4
.no_superhire	add.w	d2,d0			+ largeur de l'écran
		move.w	(SNLIGNE,a0),d3		d3=hauteur de l'écran
		tst.b	(SLACE,a0)		écran interlace ?
		beq.s	.no_interlace		-> non
		lsr.w	#1,d3
.no_interlace	add.w	d3,d1			+ hauteur de l'écran
		move.w	d1,d2
		lsl.w	#8,d2
		add.b	d0,d2
		move.w	#DIWSTOP,(a4)+
		move.w	d2,(a4)+
		rts

		* Calcul DIWSTRT *
.calcul_startx	move.w	(SNPIXEL,a0),d0		Largeur écran demandé
		tst.b	(SHIRES,a0)		Haute resolution ?
		beq.s	.no_hires		-> Non
		lsr.w	#1,d0
.no_hires	tst.b	(SUPERHIRES,a0)		écran super-haute-résolution ?
		beq.s	.no_superhires		-> non
		lsr.w	#2,d0
.no_superhires	sub.w	#320,d0			- Largeur écran stantard
		asr.w	#1,d0
		sub.w	#HSTART_STANDARD,d0	- Pos. x pour écran standard
		neg.w	d0
		move.w	d0,(SCOORDX,a0)
		bra	.cont1
.calcul_starty	move.w	(SNLIGNE,a0),d1		Hauteur écran demandé
		tst.b	(SLACE,a0)		Ecran entrelacé ?
		beq.s	.no_lace		-> Non
		lsr.w	#1,d1
.no_lace	sub.w	#200,d1			- Hauteur écran standard
		cmp.w	#50,(frequence-t,a5)
		bne.s	.no_pal
		sub.w	#56,d1
.no_pal		asr.w	#1,d1
		sub.w	#VSTART_STANDARD,d1	- Pos. Y standard
		neg.w	d1
		move.w	d1,(SCOORDY,a0)
		bra	.cont2
		
*---------------* init. screen : calcul DDFSTRT / DDFSTOP *----------------*
* entrée	=> a0.l	adr. structure écran
*		=> a1.l	adr. liste Copper 1
*		=> a2.l	adr. liste Copper 2
calcul_display_fetch:
		move.l	a1,a4
		bsr.s	.routine		|-> liste Copper 1
		move.l	a4,a1
		tst.w	(NBCOPPER,a0)		Une seule liste Copper ?
		beq.s	.exit			-> Oui
		move.l	a2,a4
		bsr.s	.routine		|-> liste Copper 2
		move.l	a4,a2
.exit		rts
.routine	move.w	(SCOORDX,a0),d0		d0=début horz. fenêtre écran
		addq.w	#1,d0
		move.w	(SNPIXEL,a0),d1		d1=largeur de l'écran

		* change valeurs si défilement hard horizontal *
		tst.w	(SHARDSCROLL.F,a0)	défilement hard ?
		beq.s	.no_hardscroll		-> non
		sub.w	#16,d0
		add.w	#16,d1
		tst.w	(SFMODE_PLANS,a0)	mode X1
		beq.s	.no_hardscroll		-> oui
		sub.w	#16,d0
		add.w	#16,d1
		cmp.w	#2,(SFMODE_PLANS,a0)	mode X2
		beq.s	.no_hardscroll		-> oui
		sub.w	#32,d0
		add.w	#32,d1
.no_hardscroll	
		* écrit "Display Data Fetch Start"" *
		bsr	fetch
.ddf_ok		move.w	#DDFSTRT,(a4)+
		cmp.w	#$18,d0			supérieur valeur mini. ?
		bge.s	.set_ddfstrt		-> oui
		move.w	#$18,d0
.set_ddfstrt	move.w	d0,(a4)+

		* écrit "Display Data Fetch Stop" *
		move.w	#DDFSTOP,(a4)+
		cmp.w	#$D8,d1			dépasse valeur maxi. ?
		bls.s	.set_ddfstop		-> non
		sub.w	#$D8,d1
		sub.w	d1,-4(a4)
		move.w	#$D8,d1
.set_ddfstop	move.w	d1,(a4)+
		rts

*---------------------------> CALCUL DDFSTRT <----------------------------*
* entrée	=> d0.w DIWSTRT horizontal
*		=> d1.w	largeur écran en pixels
* sortie	<= d0.w valeur de DDFSTRT
*		<= d1.w valeur de DDFSTOP
fetch:
		move.w	(SFMODE_PLANS,a0),d2
		tst.b	(SHIRES,a0)		mode haute résolution ?
		bne.s	.hires			-> oui
		tst.b	(SUPERHIRES,a0)		mode super haute résolution ?
		bne.s	.superhires		-> oui

		* spécial mode "low resolution" (320 pixels par ligne) *
		*sub.w	(.low,pc,d2*2),d0
		sub.w	#17,d0
		lsr.w	#1,d0
		and.w	#$FFFC,d0
		lsr.w	#1,d1			d1 = largeur de l'écran / 2
		bra.s	.ddf_ok

		* spécial mode "high resolution" (640 pixels par ligne) *
.hires		*sub.w	(.hire,pc,d2*2),d0
		sub.w	#17,d0
		lsr.w	#1,d0			
		and.w	#$FFFC,d0
		lsr.w	#2,d1			d1 = largeur de l'écran / 4
		add.w	(.hire2,pc,d2*2),d1
		bra.s	.ddf_ok

		* spécial mode "super high resolution" (1280 pixels par ligne) *
.superhires	*sub.w	(.super,pc,d2*2),d0
		sub.w	#5,d0
		lsr.w	#1,d0			
		and.w	#$FFFC,d0
		lsr.w	#3,d1			d1 = largeur de l'écran / 8
		add.w	(.super2,pc,d2*2),d1

.ddf_ok		
		sub.w	(.love,pc,d2*2),d1
		add	d0,d1
		rts

;.low		dc.w	17,33,33,65	(8.5*2 , 16.5*2 , 32.5*2)
;.hire		dc.w	09,17,17,33	(4.5*2 ,  8.5*2	, 16.5*2)
;.super		dc.w	05,09,09,17	(2.5*2 ,  4.5*2 ,  8.5*2)
.hire2		dc.w	$00,$08,$08,26
.super2		dc.w	$08,$10,$10,$20

.love		dc.w	08,30,30,56


*-------------------------------------------------------------------------*
*>>>>>>>>>>>>>>>>>>>>>>>>>> STRUCTURE AGA ECRAN <<<<<<<<<<<<<<<<<<<<<<<<<<*
*-------------------------------------------------------------------------*
* le signe "ø" signifie que la valeur est initialisé par 'init_screen'
		RSRESET
SCREEN:		rs	0
SMEM		rs.w	1		Flag, si 1 mém. struct. réservée
SFLAG		rs.w	1	ø	Flag, si 1 struct. initialisé
SCOORDX		rs.w	1		Coord. x gauche fenêtre écran
SCOORDY		rs.w	1		Coord. y haute fenêtre écran
SNPIXEL		rs.w	1		Largeur de la fenêtre écran
SNLIGNE		rs.w	1		Hauteur de la fenêtre écran
SNPLAN		rs.b	1		Nombre de plan total
SHIRES		rs.b	1		HIRES de BPLCON0 0/1 (haute résol.)
SHOMOD		rs.b	1		HOMOD de BPLCON0 0/1 (mode HAM)
SDBPLF		rs.b	1		DBPLF de BPLCON0 0/1 (Dual-Playfield)
SERSY		rs.b	1		ERSY  de BPLCON0 0/1 (synchro externe)
SLACE		rs.b	1		LACE  de BPLCON0 0/1 (lignes entrelacées)
SUPERHIRES	rs.b	1		SHIRES de BPLCON0 0/1 (superhaute résol.)
		rs.b	1
SLUTIN		rs.w	1		Flag, si 1 pts lutins dans liste Copper
SBPLCON2	rs.w	1		valeur de BPLCON2
SBPLCON4	rs.w	1		valeur de BPLCON4
SDFFSTART	rs.w	1		si=1 DMA plan commence un mot avant
SHARDSCROLL.F	rs.w	1		si=1 DMA plan commence 8 octets avant
SCOLOR.F	rs.w	1		si=1 couleurs dans la liste copper
SFADEONSPEED	rs.w	1		vitesse du fade-on
SFADEOFFSPEED	rs.w	1		vitesse du fade-off
SFMODE_PLANS	rs.w	1		FMode Plans (0=X1/1=X2/2=X2/3=X3)
SFMODE_LUTINS	rs.w	1		FMode Lutins (0=16/1=32/2=32/3=64)
SLUTIN_RES	rs.w	1		Résolution lutins (0=Low/1=low/2=Hires/3=Super)
SBPLCON3	rs.w	1	ø	Valeur de BPLCON3
SFREQ		rs.w	1		Flag 1=Force 50Hz /2=Force 60Hz
NBCOPPER	rs.w	1		0= une liste Copper, sinon 2 listes
KKCOPPER	rs.w	1		nbre octets en + liste Copper
		rs.w	1
SACOPPER	rs.l	1	ø RESA	adr. de la liste copper
STCOPPER	rs.l	3	ø	taille octets liste copper + ID&TYPE
SACOPPER2	rs.l	4	ø RESA	adr. de la deuxieme liste copper
SACOLOR1	rs.l	1	ø	adr. moves couleurs liste Copper 1
SACOLOR2	rs.l	1	ø	adr. moves couleurs liste Copper 1
SALUTIN1	rs.l	1	ø	adr. moves pointeurs lutins liste 1
SALUTIN2	rs.l	1	ø	adr. moves pointeurs lutins liste 2
SADIW1		rs.l	1	ø	adr. de DIWSTRT liste copper 1
SADIW2		rs.l	1	ø	adr. de DIWSTRT liste copper 2
SPDECALAGE	rs.l	1	ø	adr. déc. hard dans la liste Copper
SPDECALAGE2	rs.l	1	ø	adr. déc. hard dans la liste Copper

* PLANS PAIRS SI DUAL-PLAYFIELD OU ECRAN NORMAL *
SPBUFFER	rs.w	1		nbre de buffer (3 maxi)
SPLARGE		rs.w	1		largeur page écran
SPHAUTEUR	rs.w	1		hauteur page écran
SPLEFT		rs.w	1		nbre pixels cachés à gauche
SPRIGHT		rs.w	1		nbre pixels cachés à droite
SPUP		rs.w	1		nbre lignes cachées en haut
SPDOWN		rs.w	1		nbre lignes cachées en bas
SP_HSCROLL	rs.w	1		largeur du défilement horizontal
SP_VSCROLL	rs.w	1		largeur du défilement vertical
SPPLAN		rs.w	1	ø	nombre de plan
SP_SPLAN	rs.w	1	ø	taille & largeur d'un plan
SP_SLINE	rs.w	1	ø	taille d'une ligne
SPOFFSET	rs.l	1	ø	offset (SPUP*SP_SLINE)+(SPLEFT<<8)
SPPOINTEUR	rs.l	1	ø	adr. pointeur vidéo dans liste Copper
SPPOINTEUR2	rs.l	1	ø	adr. pointeur vidéo dans liste Copper
SPSCROLLPT	rs.l	1	ø	adr. pointeur vidéo dans liste Copper
SPSCROLLPT2	rs.l	1	ø	adr. pointeur vidéo dans liste Copper
SPWAIT1		rs.l	1	ø	adr. wait vscroll
SPWAIT2		rs.l	1	ø	adr. wait vscroll
SPECRAN1	rs.l	1	ø RESA	adr. buffer1
SP_SIZE		rs.l	3	ø	taille de l'écran
SPECRAN2	rs.l	4	ø RESA	adr. buffer2
SPECRAN3	rs.l	4	ø RESA	adr. buffer3

* PLANS IMPAIRS SI DUAL-PLAYFIELD *
SIBUFFER	rs.w	1		nbre de buffer (3 maxi) (dual-playfield)
SILARGE		rs.w	1		largeur page écran (dual-playfield)
SIHAUTEUR	rs.w	1		hauteur page écran (dual-playfield)
SILEFT		rs.w	1		nbre pixels cachés à gauche
SIRIGHT		rs.w	1		nbre pixels cachés à droite
SIUP		rs.w	1		nbre lignes cachées en haut
SIDOWN		rs.w	1		nbre lignes cachées en bas
SIPLAN		rs.w	1	ø	nombre de plan(s) (dual-playfield)
SI_SPLAN	rs.w	1	ø	taille & largeur d'un plan (dual-playfield)
SI_SLINE	rs.w	1	ø	taille d'une ligne (dual-playfield)
SIOFFSET	rs.l	1	ø	offset (SIUP*SI_SLINE)+(SILEFT<<8)
SIPOINTEUR	rs.l	1	ø	adr. pointeur vidéo dans liste Copper
SIPOINTEUR2	rs.l	1	ø	adr. pointeur vidéo dans liste Copper
SIECRAN1	rs.l	1	ø RESA	adr. buffer1 (dual-playfield)
SI_SIZE		rs.l	3	ø	taille de l'écran  (dual-playfield)
SIECRAN2	rs.l	4	ø RESA	adr. buffer2 (dual-playfield)
SIECRAN3	rs.l	4	ø RESA	adr. buffer3 (dual-playfield)

SFADE		rs.w	1		valeur maximum componsante dans la palette
NREGISTRE	rs.w	1	ø	nbre de regitsre couleur - 1
SCOLOR_TABLE	rs.l	256		256 registres couleurs maximums (1 mot bit 4 à 7, 2ieme mot bits 0 à 3)
SCOLOR_TABLE2	rs.l	256		256 registres couleurs (fadeon/fadeoff)
SCREEN_END	rs	0
SIZE_ST_ECRAN	=SCREEN_END-SCREEN	taille d'une structure écran
SP_MOT		=SHARDSCROLL.F

*
*-------------------------------------------------------------------------*
***************** Réserve mémoire pour la structure écran *****************
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. 't'
* Sortie	<= d7.l #0 / Z=1 Pas d'erreur
*		<= a0.l	Adr. structure écran
screen_reserve:
		move.l	#SIZE_ST_ECRAN,d0
		moveq	#0,d1	
		bsr	reserve_memoire
		tst.l	d0
		beq.s	.out_mem
		move.l	d0,a0
		move.w	#1,(SMEM,a0)		flag struct. réservée
		moveq	#0,d7
		rts
.out_mem	moveq	#-12,d7			¤ Dépassement mémoire
		rts

*-------------------------------------------------------------------------*
***************** Réserve mémoire pour la structure écran *****************
*-------------------------------------------------------------------------*
* Entrée	=> a4.l	pt/structure RESA
* Sortie	<= d7.l #0 / Z=1 Pas d'erreur
*		<= a0.l	Adr. structure écran
screen_res:
		move.l	#SIZE_ST_ECRAN,(4.w,a4)
		bsr	reserve.mem
		bne.s	.exit
		move.l	(a4),a0
		move.w	#1,(SMEM,a0)		flag struct. réservée
		moveq	#0,d7
.exit		rts

*
*-------------------------------------------------------------------------*
**** Convertit les couleurs 32bits de la structure écran en format AGA ****
*-------------------------------------------------------------------------*
* entrée	=> a0.l	adr. stucture écran
convert_aga_color:
		move.w	(NREGISTRE,a0),d3	d3=nbre de regitres - 1
		lea	(SCOLOR_TABLE,a0),a4
.loop		move.l	(a4),d2			d2=couleur 32bits RVB
		bsr.s	aga_color		|-> convertit en AGA
		move.w	d0,(a4)+		bits 4 à 7 RVB
		move.w	d1,(a4)+		bits 0 à 3 RVB
		dbf	d3,.loop
.end		rts
*
*-------------------------------------------------------------------------*
****************** Convertit les couleurs au format AGA *******************
*-------------------------------------------------------------------------*
* entrée	=> d2.l	couleur 32 bits {X.X.R1.R2.V1.V2.B1.B2}
* 			X inutilisé / R1.R2 rouge / V1.V2 vert / B1.B2 bleu 
* sortie	<= d0.w	R1.V1.B1 (valeur des bits 4 à 7 comme l'original-chip)
*		<= d1.w	R2.V2.B2 (valeur des bits 0 à 3)
* utilise	-- d0,d1,d2
aga_color:
		move.l	d2,d1
		and.l	#$F0F0F0,d1	garde les bits de 4 à 7
		move.w	d1,d0
		lsr.w	#4,d0
		and.w	#$00F,d0	garde la composante bleue
		lsr.w	#8,d1
		or.w	d1,d0		ajoute composante verte
		swap	d1
		lsl.w	#4,d1
		or.w	d1,d0		ajoute composante rouge
		and.l	#$0F0F0F,d2
		move.w	d2,d1
		and.w	#$00F,d1 	garde la composante bleue
		lsr.w	#4,d2
		or.w	d2,d1		ajoute composante verte
		swap	d2
		lsl.w	#8,d2
		or.w	d2,d1		ajoute composante rouge
		rts
*
*-------------------------------------------------------------------------*
*************** Met toutes les couleurs dans les registres ****************
*-------------------------------------------------------------------------*
* Entrée	a5.l	adresse 't'
colors_on:	
		tst.l	(pt_screen-t,a5)
		beq.s	.exit
		move.l	(pt_screen-t,a5),a0
		lea	(SCOLOR_TABLE,a0),a1
		lea	(SCOLOR_TABLE2,a0),a2
		move.w	#256-1,d0
.loop		move.l	(a1)+,(a2)+
		dbf	d0,.loop
		bsr.s	set_colors
.exit		rts
*
*-------------------------------------------------------------------------*
************* Allume les couleurs valeurs de 'SCOLOR_TABLE2' **************
*-------------------------------------------------------------------------*
****************** Met les couleurs dans la liste Copper ******************
* Entrée	=> a0.l adr. structure écran
*		=> a6.l	CUSTOM
* Utilise	-- (d1, d3, d4 / a0, a1, a2, a4)
set_colors:
		tst.w	(SCOLOR.F,a0)		couleurs dans liste Copper ?
		beq	set_colors2		-> non
		move.w	(NREGISTRE,a0),d1	d1=nombre de couleurs-1
		move.l	(SACOLOR1,a0),a4	a4=adr. liste Copper
		lea	(SCOLOR_TABLE2,a0),a1	pointe sur bits 4 à 7 RVB
		lea	(2,a1),a2		pointe sur bits 0 à 3 RVB

		* boucle des bits de 4 à 7 *
.loop_color	moveq	#32-1,d3		32 regitres couleurs
		addq.l	#4,a4			saute le "MOVE #$xxxx,BPLCON3
		move.w	d1,d4			d1=nbre reg. pour 2ieme boucle
.bits4a7	addq.l	#2,a4			saute dest. "MOVE" (COLORxx)
		move.w	(a1),(a4)+		ecrit la source du "MOVE"
		addq.l	#4,a1			pt/prochain "MOVE"
		subq.w	#1,d4			décr. nbre de regitres
		bmi.s	.cont			-> moins de 32 registres
		dbf	d3,.bits4a7		32 regitres couleurs maximum

		* boucle des bits de 0 à 3 *
.cont		moveq	#32-1,d3		32 regitres couleurs
		addq.l	#4,a4			saute le "MOVE #$xxxx,BPLCON3
.bits0a3	addq.l	#2,a4			saute dest. "MOVE" (COLORxx)
		move.w	(a2),(a4)+		ecrit la source du "MOVE"
		addq.l	#4,a2			pt/prochain "MOVE"
		subq.w	#1,d1			décr. nbre de regitres
		bmi.s	.end			-> moins de 32 registres
		dbf	d3,.bits0a3		32 regitres couleurs maximum
		bra.s	.loop_color
.end		rts
****************** Ecrit les couleurs dans les regitres *******************
* Entrée	=> a0.l adr. structure écran
*		=> a6.l	CUSTOM
* Utilise	-- (d0-d7 / a0-a3
set_colors2:
		moveq	#0,d2			sélect. 1er banc des 8 bancs de couleurs
		move.w	(NREGISTRE,a0),d1	d1=nombre de couleurs - 1
		lea	(SCOLOR_TABLE2,a0),a1	a1=pointeur sur bits 4 à 7 RVB
		lea	(2,a1),a2		a2=pointeur sur bits 0 à 3 RVB
		lea	(BPLCON3,a6),a3
		move.l	#CUSTOM+COLOR00,d0
		move.w	(SBPLCON3,a0),d6	sélection bits 4 à 7 RVB
		move.w	d6,d7
		or.w	#$0200,d7		sélection bits 0 à 3 RVB

		* boucle des bits de 4 à 7 *
.loop_color	moveq	#32-1,d3		d3=32 regitres couleurs-1
		move.l	d0,a0			a0=premier registre couleur
		move.w	d2,d5
		or.w	d6,d5			bits 4 à 7 RVB
		move.w	d5,(a3)			init. BPLCON3 (change de banc)
		move.w	d1,d4
.bits4a7	move.w	(a1),(a0)+		écrit dans le reg. couleur
		addq.l	#4,a1
		subq.w	#1,d4
		bmi.s	.cont
		dbf	d3,.bits4a7

		* boucle des bits de 0 à 3 *
.cont		moveq	#32-1,d3		32 regitres couleurs
		move.l	d0,a0			a0=premier registre couleur
		move.w	d2,d5
		or.w	d7,d5			bits 0 à 3 RVB
		move.w	d5,(a3)			init. BPLCON3 (change de banc)
.bits0a3	move.w	(a2),(a0)+		écrit dans le reg. couleur
		addq.l	#4,a2
		subq.w	#1,d1
		bmi.s	.end
		dbf	d3,.bits0a3
		add.w	#$2000,d2		prochain banc de couleurs
		bra.s	.loop_color
.end		rts
*
*-------------------------------------------------------------------------*
************** fondu noir -> couleurs finales (AGA 24 bits) ***************
*-------------------------------------------------------------------------*
* Entrée	=> a0.l adr. structure écran
* Utilise	-- (d0-d7 /a0-a5)
aga_fade_on:
		moveq	#$00F,d2
		move	#$0F0,d3
		move	#$F00,d4
		move	#$100,d5
		move.w	(SFADEONSPEED,a0),d6		d6=nbre de passage-1
.loop		lea	(SCOLOR_TABLE2,a0),a2		bits 4 à 7
		lea	(2,a2),a1			bits 0 à 3
		lea	(SCOLOR_TABLE,a0),a4		bits 4 à 7
		lea	(2,a4),a3			bits 0 à 3
		move.w	(NREGISTRE,a0),d7		d7=nombre de couleurs-1
.loop_master	
		* Composante Bleu *
		move.w	(a2),d0			d0=couleur RVB actuelle bits 4 à 7
		move.w	(a4),d1			d1=couleur RVB à atteindre bits 4 à 7 
		and.w	d2,d0			&$00F
		and.w	d2,d1			&$00F
		cmp	d0,d1
		bne.s	.blue_modify
		move.w	(a1),d0			d0=couleur RVB actuelle bits 0 à 3  
		move.w	(a3),d1			d1=couleur RVB à atteindre bits 0 à 3
		and.w	d2,d0			&$00F
		and.w	d2,d1			&$00F
		cmp.w	d0,d1
		beq.s	.blue_end
.blue_modify	move.w	(a1),d0			d0=couleur RVB actuelle bits 0 à 3
		addq.w	#1,d0   		bleu=bleu+1
		and.w	d2,d0	 		dépassement ?
		bne.s	.blueok 		-> non
		and.w	#$FF0,(a1)		raz bits 0 à 3 du bleu
		addq.w	#1,(a2) 		augmente composante bleue des bits 4 à 7
		bra.s	.blue_end
.blueok		addq.w	#1,(a1) 		augmente composante bleue des bits 0 à 3
.blue_end
		 * composante verte *
		move.w	(a2),d0			d0=couleur RVB actuelle bits 4 à 7
		move.w	(a4),d1			d1=couleur RVB à atteindre bits 4 à 7 
		and.w	d3,d0			&$0F0
		and.w	d3,d1			&$0F0
		cmp.w	d0,d1
		bne.s	.green_modify
		move.w	(a1),d0			d0=couleur RVB actuelle bits 0 à 3  
		move.w	(a3),d1			d1=couleur RVB à atteindre bits 0 à 3
		and.w	d3,d0			&$0F0
		and.w	d3,d1			&$0F0
		cmp.w	d0,d1
		beq.s	.green_end
.green_modify	move.w	(a1),d0			d0=couleur RVB actuelle bits 0 à 3
		add.w	#$010,d0   		vert=vert+1
		and.w	d3,d0	 		dépassement ?
		bne.s	.greenok 		-> non
		and.w	#$F0F,(a1)		raz bits 0 à 3 du vert
		add.w	#$010,(a2) 		augmente composante des bits 4 à 7
		bra.s	.green_end
.greenok	add.w	#$010,(a1) 		augmente composante des bits 0 à 3
.green_end
		 * composante rouge *
		move.w	(a2),d0			d0=couleur RVB actuelle bits 4 à 7
		move.w	(a4),d1			d1=couleur RVB à atteindre bits 4 à 7 
		and.w	d4,d0			&$F00
		and.w	d4,d1			&$F00
		cmp.w	d0,d1
		bne.s	.red_modify
		move.w	(a1),d0			d0=couleur RVB actuelle bits 0 à 3  
		move.w	(a3),d1			d1=couleur RVB à atteindre bits 0 à 3
		and.w	d4,d0			&$F00
		and.w	d4,d1			&$F00
		cmp.w	d0,d1
		beq.s	.red_end
.red_modify	move.w	(a1),d0			d0=couleur RVB à actuelle bits 0 à 3
		add.w	d5,d0   		rouge=rouge+1
		and.w	d4,d0	 		dépassement ?
		bne.s	.redok	 		-> non
		and.w	#$0FF,(a1)		raz bits 0 à 3 du rouge
		add.w	d5,(a2) 		augmente composante des bits 4 à 7
		bra.s	.red_end
.redok		add.w	d5,(a1) 		augmente composante des bits 0 à 3
.red_end	addq.l	#4,a1			prochaine couleur à atteindre bits 0 à 3
		addq.l	#4,a2			prochaine couleur à atteindre bits 4 à 7
		addq.l	#4,a3			prochaine couleur actuelle bits 0 à 7
		addq.l	#4,a4			prochaine couleur actuelle bits 4 à 7
		dbf	d7,.loop_master
		subq.w	#1,(flag_fade_on-t,a5)
		beq.s	.exit
		dbf	d6,.loop
.exit		rts
*
*-------------------------------------------------------------------------*
****************** fondu couleurs -> noir (AGA 24 bits) *******************
*-------------------------------------------------------------------------*
* Entrée	=> a0.l adr. structure écran
* Utilise	-- (d0,d1,d2,d3,d4,d5,d6,d7 / a0,a1)
aga_fade_off:
		moveq	#$00F,d2
		move.w	#$0F0,d3
		move.w	#$F00,d4
		moveq	#$010,d5
		move.w	(SFADEOFFSPEED,a0),d6
.loop		lea	(SCOLOR_TABLE2,a0),a2		bits 4 à 7
		lea	(2,a2),a1			bits 0 à 3
		move.w	(NREGISTRE,a0),d7	d7=nombre de couleurs-1
.loop_master
		 * composante bleu *
		move.w	(a2),d0			d0=couleur RVB bits 4 à 7
		and.w	d2,d0
		bne.s	.blue_modify
		move.w	(a1),d0			d0=couleur RVB bits 0 à 3  
		and.w	d2,d0
		beq.s	.blue_end
.blue_modify	move.w	(a1),d0			d0=couleur RVB bits 0 à 3
		and.w	d2,d0	 		à zéro ?
		bne.s	.blueok 		-> non
		or.w	d2,(a1)	
		subq.w	#1,(a2) 		diminue bleu bits 4 à 7
		bra.s	.blue_end
.blueok		subq.w	#1,(a1) 		diminue bleu bits 0 à 3
.blue_end
		 * composante verte *
		move.w	(a2),d0			d0=couleur RVB actuelle bits 4 à 7
		and.w	d3,d0
		bne.s	.green_modify
		move.w	(a1),d0			d0=couleur RVB actuelle bits 0 à 3  
		and.w	d3,d0
		beq.s	.green_end
.green_modify	move.w	(a1),d0			d0=couleur RVB actuelle bits 0 à 3
		and.w	d3,d0	 		zéro ?
		bne.s	.greenok 		-> non
		or.w	d3,(a1)
		sub.w	d5,(a2) 		augmente composante des bits 4 à 7
		bra.s	.green_end
.greenok	sub.w	d5,(a1) 		augmente composante des bits 0 à 3
.green_end
		 * composante rouge *
		move.w	(a2),d0			d0=couleur RVB actuelle bits 4 à 7
		and.w	d4,d0
		bne.s	.red_modify
		move.w	(a1),d0			d0=couleur RVB actuelle bits 0 à 3  
		and.w	d4,d0
		beq.s	.red_end
.red_modify	move.w	(a1),d0			d0=couleur RVB à actuelle bits 0 à 3
		and.w	d4,d0	 		dépassement ?
		bne.s	.redok	 		-> non
		or.w	d4,(a1)
		sub.w	#$100,(a2) 		augmente composante des bits 4 à 7
		bra.s	.red_end
.redok		sub.w	#$100,(a1) 		augmente composante des bits 0 à 3
.red_end
		addq.l	#4,a1
		addq.l	#4,a2
		dbf	d7,.loop_master
		subq.w	#1,(flag_fade_off-t,a5)
		beq.s	.exit
		dbf	d6,.loop
.exit		rts
*
*-------------------------------------------------------------------------*
**************** Met à zéro les 256 regitres couleurs 32 bits *************
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	't'
*		=> a6.l CUSTOM
raz_256colors:
		moveq	#0,d0
		moveq	#0,d1
*
*-------------------------------------------------------------------------*
************* Entre une même valeur au 256 registres couleurs *************
*-------------------------------------------------------------------------*
* Entrée	=> d0.w	couleur bit 4 à 7
*		=> d1.w couleur bit 0 à 3
*		=> a5.l	't'
*		=> a6.l CUSTOM
set_256colors:
		movem.l	d2-d7/a0-a2,-(a7)
		lea	(COLOR00,a6),a1
		lea	(BPLCON3,a6),a2
		moveq	#8-1,d4			8 palettes de 32 registres
		moveq	#0,d6
		tst.l	(pt_screen-t,a5)	structure écran ?
		beq.s	.no_ecran		-> non
		move.l	(pt_screen-t,a5),a0
		move.w	(SBPLCON3,a0),d6	sélection bits 4 à 7 RVB
.no_ecran	move.w	d6,d7
		or.w	#$0200,d7		sélection bits 0 à 3 RVB

		* 4 bits de poids fort des 3 composantes *
.loop_color	moveq	#32-1,d3		32 regitres couleurs
		move.l	a1,a0			a0='COLOR00'
		move.w	d2,d5
		or.w	d6,d5			bits 4 à 7
		move.w	d5,(a2)			initialise 'BPLCON3'
.bits4a7	move.w	d0,(a0)+
		dbf	d3,.bits4a7

		* 4 bits de poids faible des 3 composantes *
.cont		moveq	#32-1,d3		32 regitres couleurs
		move.l	a1,a0			a0='COLOR00'
		move.w	d2,d5
		or.w	d7,d5			bits 0 à 3
		move.w	d5,(a2)			initialise 'BPLCON3'
.bits0a3	move.w	d1,(a0)+
		dbf	d3,.bits0a3
		add.w	#$2000,d2		prochaine palette
		dbf	d4,.loop_color
		movem.l	(a7)+,d2-d7/a0-a2
		rts
*
*-------------------------------------------------------------------------*
**************************** Détruit un écran *****************************
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	Adr. table 't'
*		=> a6.l	'CUSTOM'
* Sortie	<= d7.l ¤ 00 / Z=1 pas d'erreur
*			¤-15 écran déjà détruit
kill_screen:
		tst.l	(pt_screen-t,a5)
		beq	.exit
		move.l	(pt_screen-t,a5),a0
		tst.w	(SFLAG,a0)		structure déjà détruite ?
		beq	.exit			-> oui
		moveq	#0,d1
		move.w	d1,(SFLAG,a0)
		move.l	d1,(acopper-t,a5)
		move.l	d1,(pt_screen-t,a5)
		move.w	#$0180,(DMACON,a6)	supprime DMA Copper & Plans
		and.w	#$FE7F,(dmacon-t,a5)
		lea	(libere_memoire,pc),a2
		lea	(.free,pc),a3
		lea	(SACOPPER,a0),a1
		jsr	(a3)			|-> vire liste Copper 1
		bne	.exit
		lea	(SACOPPER2,a0),a1
		jsr	(a3)			|-> vire liste Copper 2
		bne	.exit
		lea	(SPECRAN1,a0),a1
		jsr	(a3)			|-> vire écran 1
		bne	.exit
		lea	(SPECRAN2,a0),a1
		jsr	(a3)			|-> vire écran 2
		bne	.exit
		lea	(SPECRAN3,a0),a1
		jsr	(a3)			|-> vire écran 3
		bne	.exit
		lea	(SIECRAN1,a0),a1
		jsr	(a3)			|-> vire écran 1 plan impaires
		bne.s	.exit
		lea	(SIECRAN2,a0),a1
		jsr	(a3)			|-> vire écran 2 plan impaires
		bne.s	.exit
		lea	(SIECRAN3,a0),a1
		jsr	(a3)			|-> vire écran 3 plan impaires
		bne.s	.exit
		moveq	#0,d7
		tst.w	(SMEM,a0)		Mém. struct. réservée ?
		beq.s	.exit			-> Non
		move.w	d1,(SMEM,a0)
		move.l	a0,d0
		jsr	(a2)			|-> Libère mémoire structure
		bne.s	.exit
		moveq	#0,d7
.exit:		rts
* Libère mémoire * 
* Entrée	d1.l	#0
*		a1.l	pt/adresse à libérer
*		a2.l	'libere_memoire'
.free		move.l	(a1),d0			d0=Adr. mémoire
		beq.s	.no_mem			-> Nul, pas de mméoire
		jsr	(a2)
		move.l	d1,(a1)
.no_mem		rts
*
*-------------------------------------------------------------------------*
*======================* VBL : Routine de Fade On *=======================*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	adr. table 't'
vbl_fade_on:
		tst.w	(flag_fade_on-t,a5)
		beq.s	.exit
		move.l	(pt_screen-t,a5),a0
		tst.w	(SFLAG,a0)		structure initialisée ?
		beq.s	.exit			-> non
		bsr	aga_fade_on
		move.l	(pt_screen-t,a5),a0
		bsr	set_colors
.exit		rts
*
*-------------------------------------------------------------------------*
*======================* VBL : Routine de Fade On *=======================*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	adr. table 't'
vbl_fade_off:
		tst.w	(flag_fade_off-t,a5)
		beq.s	.exit
		move.l	(pt_screen-t,a5),a0
		tst.w	(SFLAG,a0)		structure initialisée ?
		beq.s	.exit			-> non
		bsr	aga_fade_off
		move.l	(pt_screen-t,a5),a0
		bsr	set_colors
.exit		rts
*
*-------------------------------------------------------------------------*
*=========================== VBL : Pointe l'écran ========================*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l	adr. table 't'
*		=> a6.l	CUSTOM
vbl_pointe_video:
		tst.l	(pt_screen-t,a5)	structure écran existe ?
		beq.s	.exit			-> non, sort
		move.l	(pt_screen-t,a5),a0	a0=adr. structure écra,
		tst.w	(SFLAG,a0)		structure initialisée ?
		beq.s	.exit			-> non, sort
		tst.w	(SP_VSCROLL,a0)		Scrolling vertical ?
		bne.s	.exit			-> oui
		move.l	(ecran_affiche-t,a5),d0	d0=adr. écran 1ere ligne
		moveq	#0,d1
		tst.b	(SLACE,a0)		écran interlacé ?
		beq.s	.pointe			-> non
		tst.b	(SHIRES,a0)		écran haute-résolution
		bne.s	.hires			-> oui
		move.w	(VPOSR,a6),d1		j'ai pas compris pourquoi en
		bmi.s	.pointe			"lo-res" c'est l'inverse.
		bra.s	.impaire
.hires		move.w	(VPOSR,a6),d1		Lit le bit LOF (bit 15)
		bpl.s	.pointe			LOF=0 balaie lignes impaires...
*						...prochain balayage lignes paires
*						-> calcul lignes paires
*		- lignes impaires -
* LOF=1 : en ce moment les lignes paires sont balayées ; la routine va calculer
* les adresses des pointeurs vidéos des lignes impaires dans la liste Copper,
* afin de préparer le balayage des lignes impaires de la prochaine trame.
.impaire	move.w	(SP_SLINE,a0),d1	d1=taille d'une ligne
		add.l	d1,d0			d0=adr. écran 2ieme ligne

		* modifie pointeurs vidéos dans la liste Copper *
.pointe		add.l	(SPOFFSET,a0),d0
		move.w	(SP_SPLAN,a0),d1	d1=taille d'un plan
		move.w	(SPPLAN,a0),d2		d2=nbre de plan
		subq.w	#1,d2
		move.l	(SPPOINTEUR,a0),a1	a1=adr. pts vidéos liste Copper
		addq.l	#2,a1
.loop		swap	d0
		move.w	d0,(a1)
		addq.l	#4,a1
		swap	d0
		move.w	d0,(a1)
		addq.l	#4,a1
		add.l	d1,d0
		dbf	d2,.loop
.exit		rts
*
*-------------------------------------------------------------------------*
**************** Recopie d'un écran dans un ou deux autres ****************
*-------------------------------------------------------------------------*
*	Entrée	a5.l	Adr. 't'
* 'copy_s1_to_s2'	> copie de l'écran 1 dans l'écran 2
* 'copy_s1_to_s2'	> copie de l'écran 1 dans l'écran 2
* 'copy_s1_to_s2s3'	> copie de l'écran 1 dans l'écran 2 et l'écran 3
* 'copy_s2_to_s1'	> copie de l'écran 2 dans l'écran 1
* 'copy_s2_to_s3'	> copie de l'écran 2 dans l'écran 3
* 'copy_s2_to_s1s3'	> copie de l'écran 2 dans l'écran 1 et l'écran 3
* 'copy_s3_to_s1'	> copie de l'écran 3 dans l'écran 1
* 'copy_s3_to_s2'	> copie de l'écran 3 dans l'écran 2
* 'copy_s3_to_s1s2'	> copie de l'écran 3 dans l'écran 1 et l'écran3
copy_s1_to_s2:	bsr	test_si_ecran
		bne.s	.exit
.loop		move.w	(a1)+,(a2)+
		dbf	d0,.loop
		dbf	d1,.loop
.exit		rts
copy_s1_to_s3:	bsr	test_si_ecran
		bne.s	.exit
		cmp.l	#0,a3
		beq.s	.exit
.loop		move.w	(a1)+,(a3)+
		dbf	d0,.loop
		dbf	d1,.loop
.exit		rts
copy_s1_to_s2s3	bsr	test_si_ecran
		bne.s	.exit
		cmp.l	#0,a3
		beq.s	.exit
.loop		move.w	(a1),(a2)+
		move.w	(a1)+,(a3)+
		dbf	d0,.loop
		dbf	d1,.loop
.exit		rts
copy_s2_to_s1:	bsr	test_si_ecran
		bne.s	.exit
.loop		move.w	(a2)+,(a1)+
		dbf	d0,.loop
		dbf	d1,.loop
.exit		rts
copy_s2_to_s3:	bsr	test_si_ecran
		bne.s	.exit
		cmp.l	#0,a3
		beq.s	.exit
.loop		move.w	(a2)+,(a3)+
		dbf	d0,.loop
		dbf	d1,.loop
.exit		rts
copy_s2_to_s1s3	bsr	test_si_ecran
		bne.s	.exit
		cmp.l	#0,a3
		beq.s	.exit
.loop		move.w	(a2),(a1)+
		move.w	(a2)+,(a3)+
		dbf	d0,.loop
		dbf	d1,.loop
.exit		rts
copy_s3_to_s1:	bsr	test_si_ecran
		bne.s	.exit
		cmp.l	#0,a3
		beq.s	.exit
.loop		move.w	(a3)+,(a1)+
		dbf	d0,.loop
		dbf	d1,.loop
.exit		rts
copy_s3_to_s2:	bsr	test_si_ecran
		bne.s	.exit
		cmp.l	#0,a3
		beq.s	.exit
.loop		move.w	(a3)+,(a2)+
		dbf	d0,.loop
		dbf	d1,.loop
.exit		rts
copy_s3_to_s1s2	bsr	test_si_ecran
		bne.s	.exit
		cmp.l	#0,a3
		beq.s	.exit
.loop		move.w	(a3),(a1)+
		move.w	(a3)+,(a2)+
		dbf	d0,.loop
		dbf	d1,.loop
.exit		rts
*	Entrée	a5.l	Adr. 't'
*	Sortie	d0.w	mot faible de la boucle
*		d1.w	mot fort de la boucle
*		a0.l	Structure écran
*		a1.l	Adr. écran 1
*		a2.l	Adr. écran 2
*		a3.l	Adr. écran 3
*		FLAG Z 	0 erreur
test_si_ecran:
		tst.l	(pt_screen-t,a5)	Structure écran ?
		beq.s	.no_screen		-> Non, sort
		move.l	(pt_screen-t,a5),a0
		tst.w	(SFLAG,a0)		Structure écran initialisée ?
		beq.s	.no_screen		-> Non, sort
		tst.l	(SPECRAN1,a0)
		beq.s	.no_screen
		tst.l	(SPECRAN2,a0)
		beq.s	.no_screen
		move.l	(SP_SIZE,a0),d0		d0=taille écran en octets
		lsr.l	#1,d0			convertit en mots
		subq.l	#1,d0			-1 pour la boucle
		move.l	d0,d1
		swap	d1
		move.l	(SPECRAN1,a0),a1
		move.l	(SPECRAN2,a0),a2
		move.l	(SPECRAN3,a0),a3
		moveq	#0,d7
		rts
.no_screen	moveq	#-25,d7
		rts
*
*-------------------------------------------------------------------------*
************************* Efface un des 3 écrans **************************
*-------------------------------------------------------------------------*
*	Entrée	a5.l	Adr. 't'
* "clear_ecran1:"	> efface l'écran 1
* "clear_ecran2:"	> efface l'écran 2
* "clear_ecran3:"	> efface l'écran 3
clear_ecran1:	bsr	test_si_ecran
		bne.s	.exit
		moveq	#0,d3
.loop		move.w	d3,(a1)+
		dbf	d0,.loop
		dbf	d1,.loop
.exit		rts
clear_ecran2:	bsr	test_si_ecran
		bne.s	.exit
		cmp.l	#0,a2
		beq.s	.exit
		moveq	#0,d3
.loop		move.w	d3,(a2)+
		dbf	d0,.loop
		dbf	d1,.loop
.exit		rts
clear_ecran3:	bsr	test_si_ecran
		bne.s	.exit
		cmp.l	#0,a3
		beq.s	.exit
		moveq	#0,d3
.loop		move.w	d3,(a3)+
		dbf	d0,.loop
		dbf	d1,.loop
.exit		rts
*
*-------------------------------------------------------------------------*
** Ecrit les adresses écran pour les pointeurs videos de la liste Copper **
*-------------------------------------------------------------------------*
* Entrée	d0.l	adr. écran
*		a0.l	adr. structure écran
* Utilise	d0,d1,d2,a0,a1
pointe:
		add.l	(SPOFFSET,a0),d0
		move.l	(SPPOINTEUR,a0),a1	a1=adr. liste Copper
		addq.l	#2,a1			saute dest. du "Move"
pointe.x:	move.w	(SPPLAN,a0),d1		d1=nbre de plan
		subq.w	#1,d1
		moveq	#0,d2
		move.w	(SP_SPLAN,a0),d2	d2=taille d'un plan en octets
.loop		swap	d0
		move.w	d0,(a1)			Mot faible de l'adresse
		addq.l	#4,a1
		swap	d0
		move.w	d0,(a1)			Mot fort de l'adresse
		addq.l	#4,a1
		add.l	d2,d0
		dbf	d1,.loop
		rts
*
*-------------------------------------------------------------------------*
************************* Pointe un des 3 écrans **************************
*-------------------------------------------------------------------------*
*	Entrée	=> a5.l 't'
pointe_ecran1:	move.l	(pt_screen-t,a5),a0	a0=Adr. structure écran
		move.w	#4,(flag_flip-t,a5)	Flag=4=écran 1 est affiché
		move.l	(SPECRAN1,a0),d0	d0=Adr. écran 1
		bra	pointe
pointe_ecran2:	move.l	(pt_screen-t,a5),a0	a0=Adr. structure écran
		clr.w	(flag_flip-t,a5)	Flag=0=écran 2 est affiché
		move.l	(SPECRAN2,a0),d0	d0=Adr. écran 2
		bra	pointe			-> Modifie liste Copper
pointe_ecran3:	move.l	(pt_screen-t,a5),a0	a0=Adr. structure écran
		move.l	(SPECRAN3,a0),d0	d0=Adr. écran 1
		bra	pointe

*-------------------------------------------------------------------------*
*===========* ECRAN : ECHANGE DEUX ECRANS (DOUBLE-BUFFERING) *============*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l adr. 't'
flip_ecrans:
		move.l	(pt_screen-t,a5),a0	a0=Adr. structure écran
		tst.w	(flag_flip-t,a5)	Affiche écran 1 ?
		beq.s	.flip			-> Oui
		clr.w	(flag_flip-t,a5)	Flag=0=écran 2 est affiché
		move.l	(SPECRAN2,a0),(ecran_affiche-t,a5)
		move.l	(SPECRAN1,a0),(ecran_travail-t,a5)
		rts
.flip		move.w	#4,(flag_flip-t,a5)	Flag=4=écran 1 est affiché
		move.l	(SPECRAN1,a0),(ecran_affiche-t,a5)
		move.l	(SPECRAN2,a0),(ecran_travail-t,a5)
		rts

*»»»»»»»»»»»»»»»»»»»»»»»»»»» LANCE LE FADE ON ««««««««««««««««««««««««««««*
* Entrée	=> a5.l adr. 't'
go_fade_on:
		move.l	(pt_screen-t,a5),a0	a0=Adr. structure écran
		tst.w	(SFADE,a0)
		bne.s	.ok
		bsr	fade_valeurs
.ok		tst.w	(flag_fade_off-t,a5)
		bne.s	.ok
		move.w	(SFADE,a0),(flag_fade_on-t,a5)
		rts

*»»»»»»»»»»»»»»»»»»»»»»»»»»» LANCE LE FADE OFF «««««««««««««««««««««««««««*
* Entrée	=> a5.l adr. 't'
go_fade_off:
		move.l	(pt_screen-t,a5),a0	a0=Adr. structure écran
		tst.w	(SFADE,a0)
		bne.s	.ok
		bsr	fade_valeurs
.ok		tst.w	(flag_fade_on-t,a5)
		bne.s	.ok
		move.w	(SFADE,a0),(flag_fade_off-t,a5)
		rts

*-----------------> Calcul la composante la plus élevée <-----------------*
* Entrée	=> a0.l	Adr. structure écran
fade_valeurs:
		lea	(SCOLOR_TABLE,a0),a2	a2=pointeur sur bits 4 à 7 RVB
		lea	(2,a2),a1		a1=pointeur sur bits 0 à 3 RVB
		moveq	#0,d5
		move.w	(NREGISTRE,a0),d0
		moveq	#$00F,d6
		move.w	#$0F0,d7

		*/ VALEUR DU BLEU /*
.loop		move.w	(a1),d1
		move.w	(a2),d2
		move.w	d1,d3			bits 0 à 3
		move.w	d2,d4			bits 4 à 7
		and.w	d6,d3
		and.w	d6,d4
		lsl.w	#4,d4
		or.w	d4,d3
		cmp.w	d3,d5
		bhi.s	.no1
		move.w	d3,d5
.no1
		*/ VALEUR DU VERT /*
		move.w	d1,d3			bits 0 à 3
		move.w	d2,d4			bits 4 à 7
		and.w	d7,d3
		and.w	d7,d4
		lsr.w	#4,d3
		or.w	d4,d3
		cmp.w	d3,d5
		bhi.s	.no2
		move.w	d3,d5
.no2
		*/ VALEUR DU ROUGE /*
		and.w	#$F00,d1
		and.w	#$F00,d2
		lsr.w	#8,d1
		lsr.w	#4,d2
		or.w	d2,d1
		cmp.w	d1,d5
		bhi.s	.no3
		move.w	d1,d5
.no3		addq.l	#4,a1
		addq.l	#4,a2
		dbf	d0,.loop
		move.w	d5,(SFADE,a0)
		rts

***************************************************************************
********************** CALCULE LA VALEUR DE BPLCON1 ***********************
***************************************************************************
* BLTCON1
* % pF pE pD pC : iB iA i9 i8 : p7 p6 p5 p4 : i3 i2 i1 i0
* iB.iA.i3.i2.i1.i0.i9.i8
* 07;06;05;04;03;02;01;00
* Entrée	=> d0.w	valeur du décalage hard de 0 à 255
* Sortie	<= d3.w	valeur de BPLCON1
* Utilise	-- (d0,d1,d2,d3)
decalage.aga:
		move.w	d0,d1			d1=décalage de 0 à 255
		move.w	d1,d2			d2=décalage de 0 à 255
		lsr.w	#2,d2			élimine bits 0 & 1
		and.w	#$F,d2			garde bits 2 à 5
		move.w	d2,d3			pour plans impaires
		lsl.w	#4,d2
		or.w	d2,d3			pour plans paires
		move.w	d1,d2			d2=décalage de 0 à 255
		lsl.w	#4,d2
		and.w	#$C00,d2		isole bits 6 & 7
		and.w	#$003,d1		isole bits 0 & 1
		lsl.w	#8,d1
		or.w	d1,d2
		or.w	d2,d3			pour plans paires
		lsl.w	#4,d2
		or.w	d2,d3			pour plans impaires
		rts
