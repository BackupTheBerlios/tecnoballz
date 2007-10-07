*«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«*
*                             EQUALITY AMIGA				  *
*		       Programmation BRUNO ETHVIGNOT			  *
*    Version originale du 9 mars 1992 : Martin de Santero Jean-Michel	  *
*                 Début programmation en ECS => 01/04/92		  *
*                 Début programmation en AGA => 17/06/95		  *
*«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«».«*
		SECTION	NOUNOURS,CODE
		OPT	P+		Pas de table de relogement
		OPT	C20+		Labels et symboles 20 caractères maxi.,  majuscules/minuscules respectées
		OPT	O-		Pas d'optimisation
		OPT	p=68020		assemble les instructions MC68020
		INCDIR	DH0:Data/
		INCDIR	DH1:Data/

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONSTANTES <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

*------------------------ CONSTANTES DU MENTAT ---------------------------*
NReservation	=50	50 adresses mémoires réservées maximums
FLAG_AMIGADOS	=1	1=assemble routines chargement AmigaDOS
FLAG_BOB32	=0	1=assemble routines BOBs (aff. écran 32 bits)
FLAG_BOBSTRUCT	=0	1=Assemble la structure BOB
FLAG_DMACON.MSK	=0	1=ferme dma bitplane lors d'un accès disque-système
FLAG_EXIT	=0	0=sortie sous système possible avec la touche "ESC"
FLAG_FREQ	=0	1=assemble routine demande fréquence balayage
FLAG_ILBM	=1	1=assemble routines décompactage IFF-ILBM
FLAG_KEY	=1	1=assemble routines de test du clavier
FLAG_LUTIN	=1	1=assemble routines gestion des lutins
FLAG_MEMORY	=1	1=information sur la mémoire possible
FLAG_MOUSE0	=0	1=assemble la routine déplacement souris port 0
FLAG_PP		=1	1=assemble routines décompactage PowerPacker
FLAG_MUSIC	=1	SI=1 Alors assemble la playroutine
FLAG_SOUND	=1	Si=1 Alors assemble routine bruitages

;-------------------------- CONSTANTES EQUALITY ---------------------------
ETB		=$12143544 Code RAW des 4 touches E+T+B+RETURN (=TRAINER)
;		-> Routine "trainer" 'page_information.s' ; 'TestClavier.s'
;		'InitVarDebu.s'

		INCLUDE	"EQ_DATA/Constantes.s"
		INCLUDE	"EQ_DATA/declare_macro.s"




;>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Debut du code <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
debut_prg:
		INCLUDE	"Mentat/Include/mentat.s"
msg.pers	dc.b	"???????????????????????????????????????????"	¤-128
		EVEN


;============== Routine appelée avant de quitter le système ==============
sys_extra:
		lea	(pt_palette-t,a5),a4
		bsr.l	reserve.mem
		bne	.exit

		clr.b	(flag_modintro-t,a5)
		*/ Charge Bruitages */
		lea	(.nom_soundtable,pc),a0
		lea	(asound_table-t,a5),a4
		bsr	charge.pp
		bne.s	.exit
		*/ Charge page graphique */
		lea	(.nom_page_gfx,pc),a0
		move.l	(pt_palette-t,a5),a2
		lea	(page.gfx-t,a5),a4
		bsr	charge.pagepp
		bne.s	.exit
		move.l	(page.gfx-t,a5),a0
		lea	(PG_DATA,a0),a0
		move.l	a0,(adr_page-t,a5)
		*/ Charge page présentation */
		lea	(.nom_pres,pc),a0
		lea	(adr_pres-t,a5),a4
		bsr	charge.fichier
		bne.s	.exit
		*/ Charge les tableaux */
		lea	(.level_pp,pc),a0
		lea	(adr_levels-t,a5),a4
		bsr	charge.pp
		bne.s	.exit
		*/ Charge texte d'information (1ere page du jeu) */
		lea	(.info_pp,pc),a0
		lea	(adr_info-t,a5),a4
		bsr	charge.pp
		bne.s	.exit
		*/ Charge modules */
		lea	(.mod_intro,pc),a0
		lea	(mod_intro-t,a5),a4
		bsr	charge.pp
		bne.s	.exit
		lea	(.mod_over,pc),a0
		lea	(mod_over-t,a5),a4
		bsr	charge.pp
		bne.s	.exit
		lea	(.mod_jingles,pc),a0
		lea	(mod_jingles-t,a5),a4
		bsr	charge.pp
		bne.s	.exit
		moveq	#0,d7
.exit		rts
.level_pp	dc.b	"EQUALITY:Equality/Raw/Levels.pp",0
.info_pp	dc.b	"EQUALITY:Equality/ILBM/Info.pp",0
.nom_page_gfx	dc.b	"EQUALITY:Equality/ILBM/Page_Graphique.pp",0
.nom_soundtable	dc.b	'EQUALITY:Equality/RAW/SoundTable.pp',0
.nom_pres	dc.b	'EQUALITY:Equality/ILBM/Presentation.pp',0
.mod_intro	dc.b	'EQUALITY:Equality/Modules/Intro.pp',0
.mod_over	dc.b	'EQUALITY:Equality/Modules/Over.pp',0
.mod_jingles	dc.b	'EQUALITY:Equality/Modules/Jingles.pp',0
		EVEN
sys_extra2:	rts

;>>>>>>>>>>>>>>>>>>>>>>>>>> début du programme <<<<<<<<<<<<<<<<<<<<<<<<<<<<
run:	
		move.b	#1,(Quit-t,a5)		Interdit effets VBL jeu
		bsr	first_init
		bsr	conv_formes		|-> convertit codes formes
		bsr	page_information
		tst.l	d7
		bne	sys_erreur
go_presents	bsr	presentation
		tst.l	d7
		bne	sys_erreur
		bsr	initialisations


;>>>>>>>>>>>>>>> BOUCLE PRINCIPAL DU JEU HORS INTERRUPTION <<<<<<<<<<<<<<<<
main_loop:	Wait_Scan
		tst.b	(flag_game-t,a5)	Tableaux seulement ?
		bne.s	.continue		-> Non
		move.b	#1,(FinDuNiveau-t,a5)
		bsr	new_niveau		"Tableaux.s"
		bsr	FinD1Niveau2		"FinD1Niveau.s"
.continue	bsr	HasardPoids
		bsr	GereLePoids
		bsr	AffichPoids
		bsr	GereLeTemps		"ETB/affiche_counter.s"
		bsr	GereLeScore		"ETB/affiche_counter.s"
		bsr	test_si_mort
		bsr	FinD1Niveau
		bsr	GereLaPause		"WaitTouchCur.s"
		bsr	GereLeAbout		"WaitTouchCur.s"
		bsr	GereLeHelps		"WaitTouchCur.s"
		bsr	test_si_info
		tst.b	(Quit-t,a5)
		beq.s	main_loop
		move.w	#255,(flag_fade_off-t,a5)
.wait		tst	(flag_fade_off-t,a5)
		bne.s	.wait
		move.l	(pt_screen-t,a5),a0
		bsr	kill_screen
		bne	sys_erreur
		move.l	(adr_lutin2_star-t,a5),d0
		bsr	libere_memoire
		tst.l	d7
		bne	sys_erreur
		bra	go_presents



;>>>>>>>>>>>>>>>>>>>>>> Initialisation avant le jeu <<<<<<<<<<<<<<<<<<1<<<<<
initialisations:
		bsr	install_ecran
		tst.l	d7
		bne	sys_erreur
		bsr	init_star_lutins
		tst.l	d7
		bne	sys_erreur
		IFNE	FLAG_SOUND
		bsr	sound_init
		ENDC
		move.w	#-4,(ame_posx-t,a5)
		clr.b	(Quit-t,a5)		Autorise effets VBL jeu
		bsr	InitVarDebu		Initialisations des variables du Jeu.
		tst.b	(flag_game-t,a5)	Tableaux seulement ?
		beq.s	.continue
		bsr	HasardPoids2		"HasardPoids.s"
		bsr	HasardPoids2		"HasardPoids.s"
		bsr	AffichDecor		Affiche décor du jeu
		bsr	AfichPlatos2		"AfichPlatos.s"
.continue	bsr	TstEtAfich
		rts


;============= Test si demande d'informations sur la mémoire ==============
; Entrée	a5.l	Adr. 't'
test_si_info:
		IFNE	FLAG_MEMORY
		bsr.l	memory_list
		move.l	(atable_clavier-t,a5),a0
		lea	(TOUCHE_HELP,a0),a0
		tst.b	(a0)
		beq.s	.exit
.wait		tst.b	(a0)
		bne.s	.wait
		bsr	memory_info
.exit		
		ENDC
		rts

;********* Routine appelée tous les 50ieme de seconde par la VBL **********
; Entrée	a5.l	Adr. 't'
extra_vbl:
		IFNE	FLAG_MUSIC
		bsr	playroutine
		ENDC
		bsr	TestClavier
;		* Effet du jeu *
		tst.b	(Quit-t,a5)		Effet du jeu permis ?
		bne.s	.no_effet		-> Non
		bsr	lutin_star_move
		bsr	pointe_lutin_ame	"ETB/init_star_lutins.s"
		IFNE	FLAG_SOUND
		bsr	gere_bruitages
		ENDC
		IFNE	FLAG_MUSIC
		bsr	VBLtest_jingles
		ENDC
.no_effet	rts

*>>>>>>>>>>>>>>>>> QUELQUES INCLUDES DE PETITES ROUTINES <<<<<<<<<<<<<<<<<*
		INCLUDE	"EQ_DATA/HasardPoids.s"
		INCLUDE	"EQ_DATA/AffichPoids.s"
		INCLUDE	"EQ_DATA/AfichPlatos.s"
		INCLUDE	"EQ_DATA/AfichFormes.s"
		INCLUDE	"EQ_DATA/GereLePoids.s"
		INCLUDE	"EQ_DATA/TestClavier.s"
		INCLUDE	"EQ_DATA/RetombPoids.s"
		INCLUDE	"EQ_DATA/InitVarDebu.s"
		INCLUDE	"EQ_DATA/NouvoNiveau.s"
		INCLUDE	"EQ_DATA/aff_formes.s"
		INCLUDE	"EQ_DATA/FinD1Niveau.s"
		INCLUDE	"EQ_DATA/WaitTouchCur.s"
		INCLUDE	"EQ_DATA/AffichDecor.s"
		INCLUDE	"EQ_DATA/ClearScreen.s"
		INCLUDE	"EQ_DATA/tableaux.s"
		INCLUDE	"EQ_DATA/ETB/set_flag_plateau.s"
		INCLUDE	"EQ_DATA/ETB/test_si_mort.s"
		INCLUDE	"EQ_DATA/ETB/conv_formes.s"
		INCLUDE	"EQ_DATA/ETB/remplit_8x8.s"
		INCLUDE	"EQ_DATA/ETB/Affiche_8x8.s"
		INCLUDE	"EQ_DATA/ETB/Affiche_8xd0.s"
		INCLUDE	"EQ_DATA/ETB/Affiche_d0x8.s"
		INCLUDE	"EQ_DATA/ETB/Affiche_d0xd1.s"
		INCLUDE	"EQ_DATA/ETB/install_ecran.s"
		INCLUDE	"EQ_DATA/ETB/test_formes.s"
		INCLUDE	"EQ_DATA/ETB/init_star_lutins.s"
		INCLUDE	"EQ_DATA/ETB/lutin_star_move.s"
		INCLUDE	"EQ_DATA/ETB/affiche_counter.s"
		INCLUDE	"EQ_DATA/ETB/affiche_chaine.s"
		INCLUDE	"EQ_DATA/ETB/presentation.s"
		INCLUDE	"EQ_DATA/ETB/page_information.s"
		IFNE	FLAG_SOUND
		INCLUDE	"EQ_DATA/Sound/bruitages.s"
		ENDC
		IFNE	FLAG_MUSIC
		INCLUDE	"EQ_DATA/ETB/VBLtest_jingles.s"
		INCLUDE	"EQ_DATA/Musique/initialise_module.s"
		INCLUDE	"EQ_DATA/Musique/interruption_6.s"
		INCLUDE	"EQ_DATA/Musique/ProPlayer.s"
		INCLUDE	"EQ_DATA/Musique/stop_sound.s"
		ENDC


;>>>>>>>>>>>>>>>>>>>>>>>> Déclaration des données <<<<<<<<<<<<<<<<<<<<<<<<;
		CNOP	0,4
pt_palette	dc.l	0,1024,"COLO",0
tlk:		INCLUDE	"Mentat/Include/Systeme/t.s"
		INCLUDE "EQ_DATA/donnees.s"
sound		ds.b	256
sound_end
vue_metter	dc.w	0
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
version:	dc.b	"$VER  EQUALITY V0.3(26.07.95)",0
		EVEN
datasin		INCBIN	"EQ_DATA/ETB/sindata.table"

;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
fin_prg:
		END
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
