*>»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->*
* TRONIC Amiga Version (original by JMMDS from PC  			  *
* START PROGRAMMING 21.09.1995	   					  *
* BY BRUNO ETHVIGNOT		   					  *
*>»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->*
VERZION	MACRO
	dc.b	"    $VER  TRONIC V0.36(19.12.95)    ",0
	ENDM
PASS	MACRO
	*dc.b	"COSMICBABY.."	15
	dc.b	"TECNOBALLZ.."	0
	ENDM
* TRICHE SI NOM ="040670"
*	ET SI PASSWORD="BESANCON25"

*xoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxox*
A_RASTER	=0		0=RASTERS POSSIBLES
A_SOUND		=0
ETB		=$12143544 Code RAW des 4 touches E+T+B+RETURN (=TRAINER)
*xoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxox*
		SECTION	ETB,CODE
		OPT	C20+		labels et symboles 20 caractères maxi.,  majuscules/minuscules respectées
		OPT	O-		pas d'optimisation
		OPT	p=68020		assemble les instructions MC68020
		INCDIR	DH0:Data/
*>»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->*
		INCDIR	DH1:Data/Tronic/Include
		INCLUDE	Data/Constantes.s
		INCLUDE	Dive/Macro.s
*>>>>>>>>>>>>>>>>>>>>>>>>>* Debut code MC68020 *<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	INCLUDE	"Mentat/Include/mentat.s"
msg.pers	dc.b	"                                           "	¤-128
		EVEN
sys_extra2	bra.l	sauve_score
sys_extra	bra	load_extra
*>>>>>>>>>>>>>>>>>>>>>>>>>* début du programme *<<<<<<<<<<<<<<<<<<<<<<<<<<*
* Entrée	a5.l	=> adr. 't'
*		a6.l	=> CUSTOM
run:		bsr	first_init
		bsr	sprites_off
		bne.l	sys_erreur
		bsr.l	np_firstinit
		bsr	first
		bne.l	sys_erreur
gomenu:		bsr	zemenu
		bne.l	sys_erreur
		bsr	init_main
		bra	gameplay
*-------------------------------------------------------------------------*
******** Routine appelée par l'interruption du temps mort vertical ********
*-------------------------------------------------------------------------*
* Entrée	a5.l	=> Adr. 't'
*		a6.l	=> CUSTOM
extra_vbl:
		move.w	(vbl_counter-t,a5),d0
		addq.w	#1,d0
		add.w	d0,(hasard-t,a5)
		move.l	a5,d0
		add.w	d0,(hasard-t,a5)
		move.w	(2+scorej1-t,a5),d0
		add.w	d0,(hasard3-t,a5)
		bsr	vbl_clavier
		tst.w	(zik-t,a5)
		beq.s	.no
		bra.l	mt_music
.no		bra.l	noisemaker
vbl.exit	rts

*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><
*>*>*>*>*>*>*>*>*>*  FICHIERS SOURCES INCLUS AUX JEUX  *<*<*<*<*<*<*<*<*<*<
*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><*><
		*** Routines d'initialisation ***
		INCLUDE	Init/init_main.s
		INCLUDE	Init/init_fond.s
		INCLUDE	Init/init_score.s
		INCLUDE	Init/init_bricks.s
		INCLUDE	Init/init_atom.s
		INCLUDE	Init/init_rak.s
		INCLUDE	Init/init_balle.s
		INCLUDE	Init/install_ecran.s
		INCLUDE	Init/shado_initbob.s
		INCLUDE	Init/shado.ajoute.s
		INCLUDE	Init/init_degrade.s
		INCLUDE	Init/init_gadget.s
		INCLUDE	Init/init_objet.s
		INCLUDE	Init/init_blitz.s
		INCLUDE	Init/init_print.s
		INCLUDE	Init/init_etoile.s
		INCLUDE	Init/init_explo.s
		INCLUDE	Init/init_gemme.s
		INCLUDE	Init/init_mur.s
		INCLUDE	Init/init_freez.s
		*** Routines du jeu ***
		INCLUDE	Play/gameplay.s
		INCLUDE	Play/gaga_fada.s
		INCLUDE	Play/bump_bump.s
		INCLUDE	Play/bp_depl1.s
		INCLUDE	Play/gestion_ball.s
		INCLUDE	Play/lache_balle.s
		INCLUDE	Play/next.level.s
		INCLUDE	Play/gestion_buibui.s
		INCLUDE	Play/add_etoil.s
		INCLUDE	Play/add_explo.s
		INCLUDE	Play/anim_etoil.s
		INCLUDE	Play/anim_explo.s
		INCLUDE	Play/add_bonus.s
		INCLUDE	Play/add_gemme.s
		INCLUDE	Play/attribut_bonus.s
		INCLUDE	Play/gestion_bonus.s
		INCLUDE	Play/gestion_gemme.s
		INCLUDE	Play/gestion_tir.s
		INCLUDE	Play/add_tirs.s
		INCLUDE	Play/gere_robot.s
		INCLUDE	Play/extralife.s
		INCLUDE	Play/gestion_mur.s
		INCLUDE	Play/add_and_del_buibui.s
		INCLUDE	Play/klara_triche.s
		INCLUDE	Play/gameover.s
		INCLUDE	Play/active_controlator.s
		INCLUDE	Play/decrement_brik.s
		INCLUDE	Play/chunky.s		Gigablitz
		**** Routines graphismes ****
		INCLUDE	Gfx1/score_affiche.s
		INCLUDE	Gfx1/bob32.gfx.s
		INCLUDE	Gfx1/shado.gfx.s
		INCLUDE	Gfx1/shado_affiche.s
		INCLUDE	Gfx1/shado_restore.s
		INCLUDE	Gfx1/megaprint.s
		INCLUDE	Gfx1/printchar.s
		INCLUDE	Gfx1/choucroutte.s		*/ Efface briques */
		**** Routines diverses ****
		INCLUDE	Dive/load_extra.s	Charge tous les fichiers
		INCLUDE	Dive/pertedememoire.s
		INCLUDE	Dive/hasard_ok.s
		INCLUDE	Dive/zemenu.s
		INCLUDE	Dive/vbl_clavier.s
		INCLUDE	Dive/quit.s
		INCLUDE	Dive/SCORE.s		Charge & Sauve le score
		INCLUDE	Dive/triscore.s
		INCLUDE	Dive/first.s
		*** Musique/Bruitages ***
		INCLUDE Song/np_firstinit.s
		INCLUDE Song/noisemaker.s
		INCLUDE Song/np_sfx.s
		INCLUDE Song/noise_init.s
		INCLUDE Song/protracker_replay.s
*§><§><§><§><§><§><§><§><§><§>< * DONNEES *<§><§><§><§><§><§><§><§><§><§><*
kilt1:		INCLUDE Song/np_table.s
		INCLUDE	Song/replay_data.s
		INCLUDE	Data/bump_data.s
		INCLUDE	Gfx1/tbob.s
		INCLUDE	Data/shadow_data.s
		INCLUDE	Data/brick.data.s
		INCLUDE	"Mentat/Include/Systeme/t.s"
		INCLUDE	Data/blitz.data.s
		INCLUDE	Data/print_data.s
		INCLUDE	Data/fire_data.s
		INCLUDE	Data/atom_data.s
		INCLUDE	Data/balle_data.s
		INCLUDE	Data/color_data.s
		INCLUDE	Data/etoile_data.s
		INCLUDE	Data/explo_data.s
		INCLUDE	Data/gemme_data.s
		INCLUDE	Data/gadget.data.s
		INCLUDE	Data/mur_data.s
		INCLUDE	Data/gameover.data.s
		INCLUDE	Data/freez_data.s
		INCLUDE	Data/menu_data.s
kilt2:
*/ Fichier powerpackés */
		CNOP	0,4
triche		dc.l	0		Code clavier RAW : E+T+B+RETURN
tablo.pp	dc.l	0,0,"TAB©",0	50 tableaux
brick.pp	dc.l	0,0,"BRK©",0	560*127*6 Jeux de briques (ILBM)
score.pp	dc.l	0,0,"SCR©",0	064*200*6 Bande des scores (ILBM)
map60.pp	dc.l	0,0,"MAP©",0	320*960*2 60 fonds tableaux (ILBM)
tron2.pp	dc.l	0,0,"GFX©",0	320*200*8 Image presentation (ILBM)
endpic.pp	dc.l	0,0,"GFX©",0	320*200*8 Image de fin (ILBM)
menupic.pp	dc.l	0,0,"GFX©",0	 (ILBM)
config.pp	dc.l	0,0,"GFX©",0	320*200*8 (ILBM)
hiscores.pp	dc.l	0,0,"GFX©",0	320*200*8 (ILBM)
bord.pp		dc.l	0,0,"GFX©",0	320*200*8 (ILBM)
doc.pp		dc.l	0,0,"GFX©",0	320*200*8 (ILBM)
first.pp	dc.l	0,0,"GFX©",0	320*200*7 (ILBM)
*/ Page graphiques RAW-BLITTER */
map60.pagegfx	dc.l	0,0,"MAP6",MEMF_CHIP	60 fonds 4 couleurs
score.pagegfx	dc.l	0,0,"SCOR",MEMF_CHIP	Bandes des scores 64 couleurs
brick.pagegfx	dc.l	0,0,"BRIK",MEMF_CHIP	10 jeux de briques
bk.pagegfx	dc.l	0,0,"BRIK",MEMF_CHIP	Jeu de briques en cours
fonte80.pagegfx	dc.l	0,0,"FONT",0		640*008*3 => 80 fontes de 8x8
fonte4.pagegfx	dc.l	0,0,"FON4",0		352*8*2 fontes
fonte2.pagegfx	dc.l	0,0,"FON2",0		352*8*2 fontes
fontex.pagegfx	dc.l	0,0,"FONX",0		080*8*4 fontes de 0 à 9
menupic.pagegfx	dc.l	0,0,"GFX!",MEMF_CHIP	 (ILBM)
*/ Divers */
pt_cos		dc.l	0,0,"tCOS",0
pt_palette:	dc.l	0,64*4,"PALL",0		palette 64 couleurs du jeu
map60.color	dc.l	0,0,"COLO",0		60 palettes 4 couleurs 60 fonds
tableaux	dc.l	0,0,"TABL",0		50 tableaux
adr_info	dc.l	0,0,"INFO",0		640*400*4 page info
pt_score	dc.l	0,SIZE_FSCORE,"BEST",0	40 Scores sauvegardés
module		dc.l	0,0,"MODU",MEMF_CHIP
*/ ECRAN */
ecran		dc.l	0
ecran1		dc.l	0	Adresse écran 1
ecran2		dc.l	0	Adresse écran 2
ecran3		dc.l	0	Adresse écran 3
coordx		dc.l	0	position X écran en hire-res
coordy		dc.l	0	position Y écran
gadget.triche	dc.w	-1	Pt/le gadget activé par une touche(-1 = aucun)
*/ Adresses du jeu */
vitesse_ball	dc.w	0
mode_de_jeu	dc.w	0
hasard		dc.w	0		Compteur utilisé pour le hasard
hasard2		dc.w	0		Compteur utilisé pour le hasard
hasard3		dc.w	0		Compteur utilisé pour le hasard
tableau.end	dc.w	0		Flag=1 tableau terminé ++ / -1 => --
xvbl		dc.w	0		n° de la VBL en début de VBL
xgo		dc.w	0		1=limite affichage
flag_pause	dc.b	0		Flag=1 -> Pause
flag.raster	dc.b	0		Flag=1 -> Raster-Lines
flag.quit	dc.b	0		Flag=1 -> Exit Menu 
flag.help	dc.b	0		Flag=1 -> Memory-Infos
flag.tab	dc.b	0		Flag=1 -> Memory-List
flag.game	dc.b	0		Flag=1 -> Jeu en cours
*>>>>>>>>>>>>>>>>>* Adresses utilisées par le programme *<<<<<<<<<<<<<<<<<*
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
version:	VERZION
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
fin_prg:	
		END
* A faire :
- corriger la routine de collision raquette/briques
- finir les options du bonus surprise
- afficher les passwords dans les tableaux (si on ramasse le Surprise)

* V 0.36 *----------------------------------------------------------------*
- (=> FINAL-DP 159 rue Voltaire / BP202 / 59334 CEDEX TOURCOING le 30-11-95)
* V 0.37 *----------------------------------------------------------------*



PAGE BOB	80*343*6	=164640
PAGE BRIQUE	80*126*6	=060480
GIGABLITZ	08*564*4	=018048
FONDS		40*960*2	=076800
SCORE		08*205*6	=009840
5 PAGES 	40*200*8	=320000
PAGE		80*400*8	=256000
PAGE		40*400*7	=224000
MENU		16*105*8	=013440
				-------
			TOTAL	1143248 = 1116 Ko RAW-GFX

