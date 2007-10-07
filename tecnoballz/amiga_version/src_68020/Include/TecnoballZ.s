*>»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->*
* TECNOBALL Z  SPECIAL VERSION AGA *
* START PROGRAMMING 25.03.1995	   *
* BY BRUNO ETHVIGNOT		   *
*>»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->*


*=> les chemins d'accès aux fichiers includes :
		INCDIR	DH0:Data/
		INCDIR	DH0:Data/TecnoballZ/Include

*=> Le programme cherche ses fichiers graphiques, modules, données diverses
    sur le disque TECNOBALLZ: 
	(nom des fichiers TecnoballZ/Include/Dive/sys_extra.s)
    - Insert la disquette du jeu TECNOBALLZ: en DF0:
    1> Makedir DH0:TecnoballZ
    1> Copy DF0:TecnoballZ DH0:TecnoballZ all 
    1> Assign TECNOBALLZ: DH0:TecnoballZ
    - retire la disquette TECNOBALLZ:


VERZION	MACRO
	dc.b	"$VER  TECNOBALLZ.AGA V0.89(12.05.96)",0
	ENDM
*-------------------------------------------------------------------------*
* TRAINER 
* 1) Les noms des 6 joueurs sont initialisés à ma date de naissance
* 2) Dans le Magasin déplacer le curseur souris tout à gauche
* 3) Puis frapper les 3 lettres ETB suivie de la touche RETURN
* TOUCHE AMIGA-DROITE + SHIFT-DROIT
*.........................................................................*
* TRAINER TABLEAUX 
* [1] Glue	[2] NextLevel	[3] Fire1	[4] Fire2	[5] Size-
* [6] Size+	[7] Life-	[8] Life+	[9] 2Balls	[0] 3Balls
* [°] Power1	[_] Power2	[|] Inverseur	[0-NUM] Speed3	[A] Bumper1
* [B] Bumper2	[C] Bumper3	[D] Bumper4 	[T] Size2	[Y] Size3
* [U] Random	[I] Mega	[O] Price1	[P] Wall	[ê] Robot
* [$] Control
* TRAINER GARDIENS => [2] Explosion
*---------------------------------------------------------------------------
*/"LARRYHEARD"/"SAUNDERSON"/"JUANATKINS"/"STEPHENSON" Area 2
*/"DANCEFLOOR"/"REVOLUTION"/"LOOKTOSEXY"/"REACHINGUP" Area 3
*/"ZULUNATION"/"HOUSEPIMPS"/"ANDRONICUS"/"DEFINITIVE" Area 4
*/"DANCEMANIA"/"PEPPERMINT"/"SOLARTRIBE"/"PROJECTXYZ" Area 5
*/"RINGOFFIRE"/"POINTBLANK"/"TEMPTATION"/"BLUEMONDAY" Area5 Fin	Si sh.code = 1
*/"SHELLSHOCK"/"HOUSEMUSIC"/"DAVECLARKE"/"CYBERACTIF" Final	Si sh.code = 1
* AMIGA-LEFT & AMIGA-RIGHT + L = RASTER-LINES
* AMIGA-LEFT & AMIGA-RIGHT + P = PAUSE
* AMIGA-LEFT + AMIGA-RIGHT + HELP = INFORMATIONS MEMOIRE
* AMIGA-LEFT + AMIGA-RIGHT + TAB  = LISTE MEMOIRE
*/ SI NOM=ETB ALORS + 5 VIES
*/ SI NOM=DJI ALORS + 4 VIES
*/ SI NOM=JMM ALORS + 3 VIES
*/ SI NOM=ZBB ALORS + 2 VIES
*/ SI NOM=REG ALORS + 1 VIE
*xoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxox*
A_RASTER	=0		0=RASTERS POSSIBLES
A_INTRO		=0		0=ASSEMBLE INTRO
A_SHOP		=0		0=ASSEMBLE MAGASIN
A_TAB		=0		0=ASSEMBLE TABLEAUX
A_GARD		=0		0=ASSEMBLE GARDIENS
A_SOUND		=0
A_TRICHE	=1		0=Triche
ETB		=$12143544 Code RAW des 4 touches E+T+B+RETURN (=TRAINER)
*xoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxox*
		SECTION	ETB,CODE
		OPT	C20+		labels et symboles 20 caractères maxi.,  majuscules/minuscules respectées
		OPT	O-		pas d'optimisation
		OPT	p=68020		assemble les instructions MC68020
*>»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->»=«<->*
		INCLUDE	Data/Constantes.s
		INCLUDE	Dive/Macro.s
*>>>>>>>>>>>>>>>>>>>>>>>>>* Debut code MC68020 *<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	INCLUDE	"Mentat/Include/mentat.s"
msg.pers	dc.b	"                                           "	¤-128
		EVEN
sys_extra2	bra.l	sauve_score

*>>>>>>>>>>>>>>>>>>>>>>>>>* début du programme *<<<<<<<<<<<<<<<<<<<<<<<<<<*
* Entrée	a5.l	=> adr. 't'
*		a6.l	=> CUSTOM
run:		bsr	first_init
		bsr	sprites_off
		bsr.l	np_firstinit
		bsr.l	page_information
		bne.l	sys_erreur
		move.w	#6,(rout-t,a5)

*=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=*
*====================== SAUT A LA ROUTINE DEMANDEE =======================*
*=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=*
main_loop:
irq:		move.w	(rout-t,a5),d0
		jmp	([.list,pc,d0*4])
		CNOP	0,4
.list:		dc.l	interupt0		00 init. jeu
		dc.l	interupt1		01 jeu
		dc.l	interupt2		02 init. magasin
		dc.l	interupt3		03 magasin
		dc.l	interupt4		04 init. gardien
		dc.l	interupt5		05 gardien
		dc.l	menu.init		06 init. introduction
		dc.l	menu.run		07 introduction
		dc.l	propaganda		08 tableau terminé
		dc.l	over1			09 init gameover
		dc.l	over2			10 gameover
		dc.l	black			11 gardien terminé
		dc.l	mega_over1		12 init gameover gardien
		dc.l	mega_over2		13 gameover gardien
		dc.l	interupt14		14 init. éditeur de tableaux
		dc.l	interupt15		15 éditeur de tableaux

*=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=-*-=*
		IFNE	A_SHOP
interupt2
interupt3:	clr.w	(rout-t,a5)		>flag prépare level<
		bra	irq
		ENDC
		IFNE	A_TAB
over1:
over2
interupt0:
interupt1:	move.w	#8,(rout-t,a5)
		bra	irq
		ENDC
		IFNE	A_GARD
interupt4:
interupt5:
mega_over1:
mega_over2:
		move.w	#11,(rout-t,a5)
		bra	irq
		ENDC

		IFNE	A_INTRO
menu.init:
menu.run:	bsr.l	star
		bra	irq
		ENDC

*-------------------------------------------------------------------------*
******** Routine appelée par l'interruption du temps mort vertical ********
*-------------------------------------------------------------------------*
* Entrée	a5.l	=> Adr. 't'
*		a6.l	=> CUSTOM
extra_vbl:
		move.l	a5,-(a7)
		bsr.l	mt_music
		move.l	(a7)+,a5
		move.w	(rout-t,a5),d1
		move.w	(vbl_counter-t,a5),d0
		add.l	a5,d0
		add.w	d1,d0
		add.w	d0,(hasard-t,a5)
		sub.w	d1,(hasard3-t,a5)
		jmp	([.list,pc,d1*4])
		CNOP	0,4
.list:		dc.l	vbl.exit		00 init. jeu
		dc.l	vbl_clavier		01 jeu
		dc.l	vbl.exit		02 init. magasin
		dc.l	vbl_clavier3		03 magasin
		dc.l	vbl.exit		04 init. gardien
		dc.l	vbl_clavier2		05 gardien
		dc.l	vbl.exit		06 init. introduction
		dc.l	vbl_clavier4		07 introduction
		dc.l	vbl.exit		08 tableau terminé
		dc.l	vbl.exit		09 init gameover
		dc.l	vbl_clavier3		10 gameover
		dc.l	vbl.exit		11 gardien terminé
		dc.l	vbl.exit		12 init gameover gardien
		dc.l	vbl_clavier3		13 gameover gardien
		dc.l	vbl.exit		14 init. levels-editor
		dc.l	vbl.editor		15 levels-editor
vbl.exit	rts

		**** INITIALISATIONS-TABLEAUX ****
		IFEQ	A_TAB
		INCLUDE	Init/init_fond.s
		INCLUDE	Init/init_score.s
		INCLUDE	Init/init_bricks.s
		INCLUDE	Init/init_atom.s
		INCLUDE	Init/init_eject.s
		INCLUDE	Init/init_rak.s
		INCLUDE	Init/init_balle.s
		INCLUDE	Init/init_bricotes.s
		INCLUDE	Init/install_ecran.s
		INCLUDE	Init/shado_initbob.s
		INCLUDE	Init/shado.ajoute.s
		INCLUDE	Init/init_degrade.s
		INCLUDE	Init/init_level.s
		INCLUDE	Init/init_capsule.s
		INCLUDE	Init/init_gadget.s
		INCLUDE	Init/init_objet.s
		INCLUDE	Init/init_head.s
		INCLUDE	Init/init_blitz.s
		INCLUDE	Init/init_mur.s
		INCLUDE	Init/init_gemme.s
		INCLUDE	Init/init_oeil.s
		INCLUDE	Init/init_bobtext.s
		ENDC
		**** SCROLL-BOBS ****
		INCLUDE	Sbob/sbob_affiche.s
		INCLUDE	Sbob/ssha_affiche.s
		INCLUDE	Sbob/sbob_restore.s
		INCLUDE	Sbob/ssha_restore.s
		INCLUDE	Sbob/sbob_main.init.s
		INCLUDE	Sbob/sbob_calcul_msk.s
		INCLUDE	Sbob/sbob_init_liste.s
		INCLUDE	Sbob/sbob_initbob.s
		INCLUDE	Sbob/ssha_initsha.s
		INCLUDE	Sbob/sbob.ajoute.s
		INCLUDE	Sbob/ssha.ajoute.s
		*** ROUTINES PRINT ***
		INCLUDE	Prnt/jconvert.s
		IFEQ	A_TAB
		INCLUDE	Prnt/init_print.s
		INCLUDE	Prnt/print_bricks.s
		INCLUDE	Prnt/print_score.s
		INCLUDE	Prnt/print_lifes.s
		INCLUDE	Prnt/print_char.s
		INCLUDE	Prnt/print_xchar.s
		INCLUDE	Prnt/print_looping.s
		INCLUDE	Prnt/print_box.s
		INCLUDE	Prnt/print_tropic.s
		INCLUDE	Prnt/score_affiche.s
		INCLUDE	Prnt/init_message.s
		INCLUDE	Prnt/no_dream1.s
		INCLUDE	Prnt/pub1.s
		INCLUDE	Prnt/pub2.s
		INCLUDE	Prnt/pub3.s
		ENDC
*---------------------> Routine Gestion des joueurs <---------------------*
		INCLUDE	Joue/init_players.s	Réserve mémoire listes joueurs
		INCLUDE	Joue/raton_laveur.s	Initialise les listes joueurs
		INCLUDE	Joue/xcopy.s		Copie Jeu => Joueur
		INCLUDE	Joue/marauder.s		Copie Joueur => Jeu
		INCLUDE	Joue/super_famicom.s	Copie Area & Level => Joueur
		INCLUDE	Joue/playerlive.s	Initialise vies Joueurs
		INCLUDE	Joue/nextplayer.s	Passe au Joueur suivant
		INCLUDE	Joue/firstplayer.s	Pointe premier joueur
		INCLUDE	Joue/pointeplayer.s	Pointe un Joueur
		INCLUDE	Joue/recopie_noms.s	Noms -> Joueur

**--------------------------> Gestion du Jeu <----------------------------*
		INCLUDE	PLAY/level_plus.s
		INCLUDE	PLAY/star.s
		IFEQ	A_SHOP
		INCLUDE	PLAY/interupt2.s
		INCLUDE	PLAY/interupt3.s
		ENDC
		IFEQ	A_GARD
		INCLUDE	PLAY/interupt4.s
		INCLUDE	PLAY/interupt5.s
		INCLUDE	PLAY/mega_over1.s
		INCLUDE	PLAY/mega_over2.s
		ENDC
		IFEQ	A_TAB
		INCLUDE	PLAY/over1.s
		INCLUDE	PLAY/over2.s
		ENDC
		INCLUDE	PLAY/black.s
		INCLUDE	PLAY/propaganda.s
		IFEQ	A_TAB
		INCLUDE	PLAY/interupt0.s
		INCLUDE	PLAY/interupt1.s
		ENDC
		IFEQ	A_INTRO
		INCLUDE	PLAY/menu.init.s
		INCLUDE	PLAY/menu.run.s
		ENDC
		IFEQ	A_TAB
		INCLUDE	PLAY/interupt14.s
		INCLUDE	PLAY/interupt15.s
		ENDC

		*** ROUTINES GARDIEN ****
		IFEQ	A_GARD
		INCLUDE	Gard/gardien.init.s
		INCLUDE	Gard/init.sc_mapadr.s
		INCLUDE	Gard/affiche_dalles.s
		INCLUDE	Gard/last_flip.s
		INCLUDE	Gard/last_scroll.s
		INCLUDE	Gard/parametres2.s
		INCLUDE	Gard/gere_scroll.s
		INCLUDE	Gard/last_move.s
		INCLUDE	Gard/last_missile.s
		INCLUDE	Gard/last_test_souris.s
		INCLUDE	Gard/last_vitus.s
		INCLUDE	Gard/elegy.s
		ENDC
		**** GRAPHISMES ****
		IFEQ	A_TAB
		INCLUDE	Gfx1/bob32.gfx.s
		INCLUDE	Gfx1/shado.gfx.s
		INCLUDE	Gfx1/shado_affiche.s
		INCLUDE	Gfx1/shado_restore.s
		INCLUDE	Gfx1/choucroutte.s
		INCLUDE	Gfx1/msx.s
		INCLUDE	Gfx1/total_recal.s
		INCLUDE	Gfx1/choupette.s
		INCLUDE	Gfx1/gestion_mur.s
		ENDC

*-------------------------> TABLEAUX - GARDIENS <-------------------------*
		INCLUDE	Taga/oo_aventure.s	Déplacement lettres GAMEOVER
		INCLUDE	Taga/venus.s		Incrémente le score
		INCLUDE	Taga/jupiter.s		Incrémente le crédit
		INCLUDE	Taga/lune.s		Décrémente les vies
		INCLUDE	Taga/lune1.s		Incrémente les vies
		INCLUDE	Taga/extralife.s	Test si vie (tous les 25000 points)
		INCLUDE	Taga/move_bobtext.s	Déplace les lettres BOBs

*------------------------------> TABLEAUX <-------------------------------*
		IFEQ	A_TAB
		INCLUDE	Tabl/etb_end.s		GameOver Tableaux
		INCLUDE Tabl/klara.triche.s
		INCLUDE	Tabl/soleil.s		Décrémente les briques
		INCLUDE	Tabl/tableau_end.s
		INCLUDE	Tabl/byte_rtel.s	LESS-BRICKS
		INCLUDE	Tabl/gaga_fada.s	FADE ON/OFF BRICKS COLORS
		ENDC

		**** DIVERS ****
		INCLUDE	Dive/sys_extra.s	Charge tous les fichiers
		INCLUDE	Dive/SCORE.s		Charge & Sauve le score
		INCLUDE	Dive/mouse_buttons.s
		INCLUDE	Dive/pertedememoire.s
		INCLUDE	Dive/hasard_ok.s
		INCLUDE	Dive/vbl_clavier.s	Toutes les routines clavier
		INCLUDE	Dive/triscore.s
		INCLUDE	Dive/quit.s
		INCLUDE	Dive/page_information.s	1ere page du jeu
		**** DEPLACEMENTS ****
		IFEQ	A_TAB
		INCLUDE	Move/msdos_beurk.s	Déplacement monnaies
		INCLUDE	Move/bob_anim.s
		INCLUDE	Move/bump_bump.s
		INCLUDE	Move/chunky.s
		INCLUDE	Move/atom_dep.s
		INCLUDE	Move/bp_depl1.s
		INCLUDE	Move/gere_balle.old
		*INCLUDE	Move/gere_balle.s
		INCLUDE	Move/gad_re.s
		INCLUDE	Move/lache_balle.s
		INCLUDE	Move/no_what.s		Test si envoie monnaies
		INCLUDE	Move/yes_what.s		Test si envoie gadgets
		INCLUDE	Move/gadget_dep.s
		INCLUDE	Move/gadgets.s
		INCLUDE	Move/tir_deplacement.s
		INCLUDE	Move/laser.s		Test si une raquette tir
		INCLUDE	Move/mouse_fire.s	Test si une raquette tir
		INCLUDE	Move/fire.color.s
		INCLUDE	Move/fire_bricks.s
		INCLUDE	Move/atom_collision1.s
		INCLUDE	Move/atom_collision2.s
		INCLUDE	Move/tilt.s
		INCLUDE	Move/mouse_blitz.s
		INCLUDE	Move/clear_allbricks.s
		INCLUDE	Move/no_dream2.s
		INCLUDE	Move/gere_robot.s
		INCLUDE	Move/gestion_gemme.s
		INCLUDE	Move/add_gemme.s
		INCLUDE	Move/gestion_oeil.s
		ENDC

		*** MUSIQUE/BRUITAGES ***
		INCLUDE	Song/Noisemaker.s
		INCLUDE Song/np_firstinit.s
		INCLUDE Song/np_sfx.s
		INCLUDE Song/np_change.s
		INCLUDE Song/ajust_music.s
		INCLUDE Song/protracker_replay.s

*§><§><§><§><§><§><§><§><§><§>< * DONNEES *<§><§><§><§><§><§><§><§><§><§><*
kilt1:		INCLUDE	Data/Sprites.data.s
		INCLUDE Song/np_table.s
		INCLUDE Song/replay_data.s
		INCLUDE	Data/bump_data.s
		IFEQ	A_TAB
		INCLUDE	Gfx1/tbob.s
		INCLUDE	Data/shadow_data.s
		INCLUDE	Data/brick.data.s
		ENDC
		INCLUDE	"Mentat/Include/Systeme/t.s"
		IFEQ	A_TAB
		INCLUDE	Data/Head_data.s
		INCLUDE	Data/bricote.data.s
		ENDC
		INCLUDE	Data/blitz.data.s
		INCLUDE	Data/joueur.data.s
		INCLUDE	Data/print_data.s
		IFEQ	A_SHOP
		INCLUDE	Shop/shop.data.s
		ENDC
		IFEQ	A_TAB
		INCLUDE	Prnt/mess.data.s
		ENDC
		INCLUDE	Sbob/SBob_Data.s
		INCLUDE	Sbob/tsbob.s
		INCLUDE	Gard/gardien.data.s
		INCLUDE	Data/fire_data.s
		INCLUDE	Data/atom_data.s
		INCLUDE	Data/level_data.s
		INCLUDE	Data/balle_data.s
		INCLUDE	Data/color_data.s
		INCLUDE	Data/capsule.data.s
		INCLUDE	Data/gadget.data.s
		INCLUDE	Data/gameover.data.s
		IFEQ	A_INTRO
		INCLUDE	Data/menu.data.s
		ENDC
		INCLUDE	Data/editor_data.s
		INCLUDE	Data/mur_data.s
		INCLUDE	Data/gemme_data.s
		INCLUDE	Data/oeil_data.s
		INCLUDE	Data/bobtext_data.s
kilt2:
*/ Fichier powerpackés */
		CNOP	0,4
triche		dc.l	0		Code clavier RAW : E+T+B+RETURN
bump.triche	dc.l	0		pt/structure raquette
maped.pp	dc.l	0,0,'EDM©',0	Scroll Gardiens
maped2.pp	dc.l	0,0,'EDM©',0	Scroll TecnoWinner
maped3.pp	dc.l	0,0,'EDM©',0	Scroll Menu
tablo.pp	dc.l	0,0,"TAB©",0	100 tableaux (2 x 50 JMM & ZBB)
heads.pp	dc.l	0,0,"TET©",0	448*029*5 Tête du Gugus (ILBM)
brick.pp	dc.l	0,0,"BRK©",0	560*127*5 Jeux de briques (ILBM)
score.pp	dc.l	0,0,"SCR©",0	064*256*5 Bande des scores (ILBM)
map60.pp	dc.l	0,0,"MAP©",0	320*960*2 60 fonds tableaux (ILBM)
rak01.pp	dc.l	0,0,"RAK©",0	448*096*4 Jeu raquette 1 (ILBM)
rak02.pp	dc.l	0,0,"RAK©",0	448*096*4 Jeu raquette 2 (ILBM)
shop1.pp	dc.l	0,0,"SH1©",0	320*226*5 Magasin 1 (ILBM)
shop2.pp	dc.l	0,0,"SH2©",0	320*226*5 Magasin 2 (ILBM)
dalma.pp	dc.l	0,0,'DAL©',0	320*544*3 Dalles gardiens (ILBM)

*/ Page graphiques RAW-BLITTER */
heads.pagegfx	dc.l	0,0,"HEAD",MEMF_CHIP
map60.pagegfx	dc.l	0,0,"MAP6",MEMF_CHIP	60 fonds 4 couleurs
score.pagegfx	dc.l	0,0,"SCOR",MEMF_CHIP	Bandes des scores 32 couleurs
brick.pagegfx	dc.l	0,0,"BRIK",MEMF_CHIP	10 jeux de briques
bk.pagegfx	dc.l	0,0,"BRIK",MEMF_CHIP	Jeu de briques en cours
fonte80.pagegfx	dc.l	0,0,"FONT",0		640*008*3 => 80 fontes de 8x8
fonte4.pagegfx	dc.l	0,0,"FON4",0		352*8*2 fontes
fonte2.pagegfx	dc.l	0,0,"FON2",0		352*8*2 fontes
mini.pagegfx	dc.l	0,0,"MINI",0		480*6*1 fontes 
fontex.pagegfx	dc.l	0,0,"FONX",0		080*8*4 fontes de 0 à 9

*/ Divers */
pt_cos		dc.l	0,0,"tCOS",0
pt_logopal:	dc.l	0,32*4,"PALL",0		palette 16 couleurs LOGO
pt_menupal:	dc.l	0,32*4,"PALL",0		palette 32 couleurs MENU
pt_winnpal:	dc.l	0,32*4,"PALL",0		palette 32 couleurs TECNOWINNER
pt_overpal:	dc.l	0,32*4,"PALL",0		palette 32 couleurs GameOver gardiens
pt_palette:	dc.l	0,32*4,"PALL",0		palette 32 couleurs du jeu
map60.color	dc.l	0,0,"COLO",0		60 palettes 4 couleurs 60 fonds
tableaux	dc.l	0,0,"TABL",0		100 tableaux
adr_info	dc.l	0,0,"INFO",0		640*400*4 page info
pt_tableaux	dc.l	0,0,"TAB2",0		50 Tableaux de l'editeur
pt_score	dc.l	0,SIZE_FSCORE,"BEST",0	40 Scores sauvegardés

*/ ECRAN Tableaux/Gardien/Menu */
ecran		dc.l	0
ecran1		dc.l	0	Adresse écran 1
ecran2		dc.l	0	Adresse écran 2
ecran3		dc.l	0	Adresse écran 3
coordx		dc.l	0	position X écran en hire-res
coordy		dc.l	0	position Y écran
ecran_shadow	dc.l	0	
gadget.triche	dc.w	-1	Pt/le gadget activé par une touche(-1 = aucun)

*/ Adresses du jeu */
newtab:		dc.w	0		0=utilse les tableaux originaux
rout:		dc.w	0		N° routine 0 à 15
area:		dc.w	0,0		N° area de 1 à 5
level:		dc.w	0,0		N° level 1 à 12
love1		dc.w	0		N° vies initiales dizaine
love		dc.w	8		N° vies initiales 1 à 9
hard		dc.w	0		Difficulté 0/1/2/3
niv00:		dc.w	0		Pt/les tableaux
hasard		dc.w	0		Compteur utilisé pour le hasard
hasard2		dc.w	0		Compteur utilisé pour le hasard
hasard3		dc.w	0		Compteur utilisé pour le hasard
score.life	dc.w	0		Compteur points 25000=vie gratuite
mouse1		dc.w	0		Flag bouton souris port 1 préssée
mouse0		dc.w	0		Flag bouton souris port 0 préssée
tableau.end	dc.w	0		Flag=1 tableau terminé
gameover	dc.w	0		Flag=1 gameover
tecnowinner	dc.w	0		Flag=1 jeu terminé
rastx		dc.w	0		Offset déplacement Y écran (Tilt)
sh.code		dc.w	0		Flag trainer actif si noms=040670
copper.diwstrt	dc.l	0,0		Pt/la liste Copper DIWSTRT
flag_pause	dc.b	0		Flag=1 -> Pause
flag.raster	dc.b	0		Flag=1 -> Raster-Lines
flag.quit	dc.b	0		Flag=1 -> Exit Menu 
flag.help	dc.b	0		Flag=1 -> Memory-Infos
flag.tab	dc.b	0		Flag=1 -> Memory-List
controlator	dc.b	0		
		EVEN

*>>>>>>>>>>>>>>>>>* Adresses utilisées par le programme *<<<<<<<<<<<<<<<<<*
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
version:	VERZION
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
* V 0.82 *----------------------------------------------------------------*
¶ (=> Diffusé à 2 personnes à la Garden Party du 28-05-95)
* V 0.83 *----------------------------------------------------------------*
- 2 listes Coppers dans les tableaux  (Pour corriger le bug pointage écran
				 	avec mémoire FAST)
- 3 tailles de balles dans les tableaux
- Gadgets balle taille 2 & balle taille 3
- Table des tables malus de 64 Gadgets au lieu de 32
- Modification animation tirs des Gardiens
- Gadgets RANDOM & Mega (Mega=toutes les options activées)
- Modification des structures Atoms (8 Atoms à l'écran)
- Optimisation des routines "fire_bricks" & "gere_balle"
- 20 balles à l'écran
¶ (=> Jean-Michel le 6 juin 1995)
¶ (=> SBMC ALMA DIFFUSION / FDS / IFA / ASCII le 8 juin 1995)
¶ (=> le 11 juin à Dominique & ZBB)
* V 0.84 *----------------------------------------------------------------*
- Page d'information sur le ShareWare
- Vire le module musique des options (plus de place sur le disque)
- Force le 50Hz/PAL
- Pages fontes se logent en Fast-Memory (Affichage au 68020)
- Scores Menu : Corrige les postions de 1 à 10 (au lieu de 1 à toutes les lignes)
- Genlock ON/OFF dans les options
¶ (=> Jean-Michel le 25 juin 1995)
¶ (=> ORION le 5 juillet 1995)
* V 0.85 *----------------------------------------------------------------*
- Change la page d'information sur le ShareWare
- Corrige le bug du menu (plantage si on presse le bouton souris entre 2 menus)
- Vire le module musique des hiscores (plus de place sur le disque)
- Supprime Genlock ON/OFF dans les options (qui ne marche pas)
- Editeur de tableaux (création de 50 tableaux en plus)
- 4 tableaux des scores pour les 4 niveaux de difficulté
- Sauve un seul score pour un même nom (le meilleur)
- Corrige le bug du bonus vie à 25000 points
- Même routine tilt que les gardiens dans les tableaux
¶ (=> JOIN Laurent 43, rue d'Orleans - 93600 Aulnay-sous-Bois le 29-08-95)
¶ (=> ORION début septembre 1995)
*-V 0.86 -----------------------------------------------------------------*
- Modifie la page d'information sur le ShareWare
- Corrige bug des scores (2 scores portant le même nom)
- Bonus "2Balls"=+3 balles éjecteur / "3balls"=+3 balles même balle
- Modifie les messages des tableaux
- (=> Jean-Pierre DEROURE début octobre)
- (=> FINAL-DP 159 rue Voltaire / BP202 / 59334 CEDEX TOURCOING le 05-10-95)
*-V 0.87 -----------------------------------------------------------------*
- Corrige le bug des bruitages
- Bonus Affichage d'un mur en bas
- Bonus Raquette Robot en bas
- Optimise les routines "gad_re" & "fire_brick" 
		. (Elargissement de la table de collision à tout l'écran)
		. (Rajout d'une table collision)
- Les balles BIGSIZE & MEGASIZE décrèmentent les briques + rapidement
- Le tir 2 décrèmente les briques + rapidement
- Le bonus "3balls" donnent les caractèristiques (taille, puissance & vitesse)
		de la balle source
- Les Atoms-BouïBouïs donnent bonus, malus et monnaies
- Modifie appel des messages
- Ajout de 6 gemmes (que donnent les Atoms) si le joueur collecte
	les 6 gemmes diffèrentes il obtient 2500 points, 3 vies, 500 crédits,
	3 niveaux supplémentaires pour les raquettes, et il passe au niveau 
	suivant
- (=> donnée à ZBB)
*-V 0.88 -----------------------------------------------------------------*
- Change la replayroutine Noisepacker par une replay Protracker
- Malus de l'oeil qui repousse les balles
- Modification magasin : Ajout bonus : Size2/Size3/Wall/Robot/ Controlator
-			: Création d'une liste des 20 Gagdets
- Un code par AREA & par niveau de difficulté
- (=> donnée à JOE)
*-V 0.89 -----------------------------------------------------------------*
* rien de nouveaux 
				  *
****************************< TAILLE DES GFX >*****************************
01) 56*029*5	=008120 octets	(Tête du Gugus)
02) 70*127*5 	=044450 octets	(Jeux de briques)
03) 08*256*5 	=010240 octets	(Bande des scores)
04) 40*960*2 	=076800 octets	(60 fonds tableaux)
05) 56*096*4 	=021504 octets	(Jeu raquette 1)
06) 56*096*4 	=021504 octets	(Jeu raquette 2)
07) 40*256*5 	=051200 octets	(Magasin)
09) 40*544*3 	=065280 octets	(Dalles gardiens)
10) 40*806*5	=161200 octets	(Page des Bobs)
11) 08*564*4	=018048 octets	(Page GigaBlitz)
12) 44*008*1	=000352 octets	(fontes 2 couleurs) score-gameover
13) 44*009*2	=000796 octets	(fontes 4 couleurs tableaux & magasin)
14) 56*008*4	=001792 octets	(fontes 16 couleurs du menu)
15) 60*006*1	=000360 octets	(mini-fontes messages tableaux)
16) 10*008*4	=000320 octets	(fontes de 0 à 9 Crédits & inverseur)
17) 80*400*4	=128000 octets	(page information début du jeu)
		 ------

* NOMBRE D'OBJETS A L'ECRAN *
*---------------------------------------------------*
* BALLES	= 20		BALLES		=10
* TIRS		= 28		TIRS		=40
* RAQUETTES	= 04		RAQUETTES	=02
* GIGABLITZ	= 01		GIGABLITZ	=01
* ATOMS		= 08		GARDIENS	=02		
* MONNAIES	= 04		MONNAIES	=04
* GADGETS	= 04		GADGETS		=04
*				EXPLOSIONS	=14
*---------------------------------------------------*
*		= 69				=78
