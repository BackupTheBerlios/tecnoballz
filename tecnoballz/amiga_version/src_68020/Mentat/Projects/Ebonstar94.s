*«*»>X<«*»>X<«*»>X<«*»>X<«*»>X<«*»>X<«*»>X<«*»>X<«*»>X<«*»>X<«*»>X<«*»>X<«*
*>»    /\    /\    /\	 /			 \    /\    /\	  /\    «<*
*<    /  \  /  \  /  \  /  E B O N S T A R   9 4  \  /	\  /  \	 /  \ 	 >*
*>»  /    \/    \/    \/			   \/	 \/    \/    \	«<*
*<  /        Commencé le dimanche 26 décembre 1993 à 15:35:43	      \	 >*
*>»/          Légèrement inspiré d'EBONSTAR de MicroIllusion	       \«<*
*< \                  Programmation : Bruno ETHVIGNOT & son chat       / >*
*>» \                       Version du 24.03.95			      /	«<*
*<   \  							     / 	 >*
*«*»>X<«*»>X<«*»>X<«*»>X<«*»>X<«*»>X<«*»>X<«*»>X<«*»>X<«*»>X<«*»>X<«*»>X<«*
* BUGS
* 1) - Bug du double-buffering (passage en 2 trames)
* 2) - Parfois une palette de couleur buggée
* 3) - Affichage des Bobs parfois buggés
* A FAIRE :
* 1) - Gestion correcte de l'entrelacé
* 2) - Gestion des bonus Directo & Electro
* 3) - ElectroFire
* 4) - Collision correcte
* 5) - Attraction du trou

		SECTION	CASSOULET,CODE
		OPT	C24+		labels et symboles 24 caractères maxi.,  majuscules/minuscules respectées
		OPT	O-		pas d'optimisation
		OPT	p=68020		assemble les instructions MC68020
		INCDIR	DH0:Data/
		INCLUDE	"Ebonstar94/Includes/Tables/Constantes.s"
VBL.MOVE=0	(si=1 alors déplacements des objets pendant la VBL)

*>>>>>>>>>>>>>>>>>>>>>>>>>* Début code MC68020 *<<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	INCLUDE	"Mentat/Include/mentat.s"
msg.pers
		dc.b	"score_init : DIWSTOP pas trouvé !          "	¤-128
		dc.b	"hole.init : Un trou par niveau minimum...  "	¤-129
		dc.b	"                                           "	¤
		EVEN

************* routine appelée avant de quitter le multitâche **************
sys_extra:	bsr	chargement
		rts
sys_extra2	rts

*>>>>>>>>>>>>>>>>>>>>>>>>>* DEBUT DU PROGRAMME *<<<<<<<<<<<<<<<<<<<<<<<<<<*
* Entrée	a5.l	=> adr. 't'
*		a6.l	=> CUSTOM
run:		
		lea	(level.adr-t,a5),a4
		bsr	reserve.mem		|-> réserve mémoire niveaux
		bne	sys_erreur
		move.l	(a4),(pt_level-t,a5)
		lea	(vbl.exit-t,a5),a0
		move.l	a0,(pt_vbl-t,a5)
		bsr	first_init
		bne	sys_erreur
		bsr	sprites_off
		bra	gomenu

		*/ INITIALISE LE NIVEAU */
ze_jeu:		clr.b	(flag.quit-t,a5)
		clr.b	(flag_pause-t,a5)
		clr.b	(flag.raster-t,a5)
		clr.b	(flag.help-t,a5)
		clr.b	(flag.del-t,a5)
		clr.b	(flag.hole-t,a5)
		bsr	install_ecran
		bne	sys_erreur
		bsr	avantjeu
		bne	sys_erreur
		lea	(vbl_clavier-t,a5),a0
		move.l	a0,(pt_vbl-t,a5)

*»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»*
*¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤* BOUCLE PRINCIPALE HORS INTERRUPTION *¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤*
*»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»*
main_loop:	
		*bsr	vbl_pointe_video
		bsr	wait_vbl
		bsr	flip_ecrans

		*»»» RESTAURE DECOR «««*
		RASTER	#$0A0
		bsr	bob32_restore
		RASTER	#$0AA
		bsr	hump.restore		'Gfx/Hump.s'
		RASTER	#$888
		bsr	hole.restore		'Gfx/Hole.s'
		RASTER	#$00A
		*bsr	restore_trace

		*»»» DEPLACE LES OBJETS «««*
		IFEQ	VBL.MOVE
		bsr	shape_move
		ENDC

		*»»» AFFICHAGE DES BOBS «««*
		tst.b	(flag.hole-t,a5)
		bne.s	.nohole
		RASTER	#$0FF
		bsr	trace_rayon		'Gfx/Hole.s'
		bsr	hole.affiche		'Gfx/Hole.s'
.nohole		RASTER	#$0A0
		bsr	hump.affiche		'Gfx/Hump.s'
		RASTER	#$0AA
		bsr	bob32_affiche
		RASTER	#$00A
		*bsr	trace

		*»»» AFFICHE LES SCORES «««*
		bsr	print_score
		bsr	cycle.score

		*»»» AFFICHE INFORMATIONS MEMOIRE «««*
		bsr	memory_list
		tst.b	(flag.help-t,a5)
		beq.s	.nohelp
		bsr	memory_help
.nohelp
		*»»» SAUVE LA PAGE ECRAN SUR DISQUE «««*
		bsr	save_x

		*»»» GESTION DU NOMBRE ALEATOIRE «««*
		bsr	hasard_ok

		*»»» AFFICHE UN NOMBRE A L'ECRAN «««*
		*bsr	affx

		*»»»  FIN DE LA BOULCE «««*
		moveq	#0,d0
		bsr	raster_line
		tst.b	(flag.quit-t,a5)
		beq	main_loop
		clr.b	(flag.quit-t,a5)
		bsr	kill_screen
		bsr	libere_mem.game
		rts

*¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤*

*-------------------------------------------------------------------------*
*»»»»»»»»»»»»»»»»»»»»»»»» DEPLACEMENT DES OBJETS «««««««««««««««««««««««««*
*-------------------------------------------------------------------------*
shape_move:	tst.b	(flag_pause-t,a5)
		bne.s	.pause
		RASTER	#$A00
		bsr	hole.current
		bsr	joueur.dep
		bsr	hump.move
		bsr	gros_move
		bsr	hole.move
		bsr	bonusprout.move
		bsr	bonusdirecto.move
		bsr	bonuselectro.move
		bsr	electro.move
		bsr	boule.move
		bsr	explo.run
		RASTER	#$000
.pause		rts

*///////////////////////////////////////////////////////////////////////////
*/*/*/*/ ROUTINE APPELEE PAR L'INTERRUPTION DU TEMPS MORT VERTICAL /*/*/*/*
*///////////////////////////////////////////////////////////////////////////
* Entrée	a5.l	=> Adr. 't'
*		a6.l	=> CUSTOM
extra_vbl:
		move.w	(vbl_counter-t,a5),d0
		addq.w	#1,d0
		add.w	d0,(hasard-t,a5)
		move.l	a5,d0
		add.w	d0,(hasard-t,a5)
		jmp	([pt_vbl-t,a5])
vbl.exit	rts

;øøøøøøøøøøøøøøøøøøøøøøøøøøø		       øøøøøøøøøøøøøøøøøøøøøøøøøøø;
;»»»»»»»»»»»»»»»»»»»»»»»»»»»  I N C L U D E S  «««««««««««««««««««««««««««;
;øøøøøøøøøøøøøøøøøøøøøøøøøøø		       øøøøøøøøøøøøøøøøøøøøøøøøøøø;

* DIVERS *
	INCLUDE	"Ebonstar94/Includes/Divers/save_x.s"
	INCLUDE	"Ebonstar94/Includes/Divers/save_screen.s"
	INCLUDE	"Ebonstar94/Includes/Divers/vbl_clavier.s"
	INCLUDE	"Ebonstar94/Includes/Divers/hasard_ok.s"
	INCLUDE	"Ebonstar94/Includes/Divers/look_bord.s"
	INCLUDE	"Ebonstar94/Includes/Divers/raster_line.s"
	INCLUDE	"Ebonstar94/Includes/Divers/affx.s"
	INCLUDE	"Ebonstar94/Includes/Divers/absolut_joymove.s"

* INIT *
	INCLUDE	"Ebonstar94/Includes/Init/chargement.s"
	INCLUDE	"Ebonstar94/Includes/Init/Install_Ecran.s"
	INCLUDE	"Ebonstar94/Includes/Init/affiche_fond.s"
	INCLUDE	"Ebonstar94/Includes/Init/hole.init.s"
	INCLUDE	"Ebonstar94/Includes/Init/hump.init.s"
	INCLUDE	"Ebonstar94/Includes/Init/joueur_firstinit.s"
	INCLUDE	"Ebonstar94/Includes/Init/avantjeu.s"
	INCLUDE	"Ebonstar94/Includes/Init/bob.cool_init.s"
	INCLUDE	"Ebonstar94/Includes/Init/bob.supinit.s"
	INCLUDE	"Ebonstar94/Includes/Init/init_tan.s"
	INCLUDE	"Ebonstar94/Includes/Init/goediteur.s"
	INCLUDE	"Ebonstar94/Includes/Init/gomenu.s"
	INCLUDE	"Ebonstar94/Includes/Init/keyboard.def.s"
	INCLUDE	"Ebonstar94/Includes/Init/libere_mem.game.s"
	INCLUDE	"Ebonstar94/Includes/Init/score_init.s"
	INCLUDE	"Ebonstar94/Includes/Init/init.shapes.s"

* GFX *
	INCLUDE	"Ebonstar94/Includes/Gfx/Hole.s"
	INCLUDE	"Ebonstar94/Includes/Gfx/Hump.s"
	INCLUDE	"Ebonstar94/Includes/Gfx/Table_Bobs.s"
	***INCLUDE	"Ebonstar94/Includes/Gfx/Draw.s"
	INCLUDE	"Ebonstar94/Includes/Gfx/print_score.s"
	INCLUDE	"Ebonstar94/Includes/Gfx/cycle.score.s"

* DEPLACEMENT *
	INCLUDE	"Ebonstar94/Includes/Deplacement/Deplacement.s"
	INCLUDE	"Ebonstar94/Includes/Deplacement/hole.move.s"
	INCLUDE	"Ebonstar94/Includes/Deplacement/joueur.dep.s"
	INCLUDE	"Ebonstar94/Includes/Deplacement/gros_move.s"
	INCLUDE	"Ebonstar94/Includes/Deplacement/hump.move.s"
	INCLUDE	"Ebonstar94/Includes/Deplacement/hole.current.s"
	INCLUDE	"Ebonstar94/Includes/Deplacement/explo.s"
	INCLUDE	"Ebonstar94/Includes/Deplacement/bonusprout.move.s"
	INCLUDE	"Ebonstar94/Includes/Deplacement/bonusdirecto.move.s"
	INCLUDE	"Ebonstar94/Includes/Deplacement/bonuselectro.move.s"
	INCLUDE	"Ebonstar94/Includes/Deplacement/electro.move.s"
	INCLUDE	"Ebonstar94/Includes/Deplacement/boule.move.s"

* TABLES *
	INCLUDE	"Mentat/Include/Systeme/t.s"
	INCLUDE	"Ebonstar94/Includes/Tables/Sinus.Table.s"
	INCLUDE	"Ebonstar94/Includes/Tables/BOB32.Structure.s"
	INCLUDE	"Ebonstar94/Includes/Tables/Hole.Data.s"
	INCLUDE	"Ebonstar94/Includes/Tables/Joueur.Data.s"
	INCLUDE	"Ebonstar94/Includes/Tables/Hump.Data.s"
	INCLUDE	"Ebonstar94/Includes/Tables/Gros.Data.s"
	INCLUDE	"Ebonstar94/Includes/Tables/Shape.Data.s"
	INCLUDE	"Ebonstar94/Includes/Tables/Level_Structure.s"
	INCLUDE	"Ebonstar94/Includes/Tables/Level_Parametres.s"
	INCLUDE	"Ebonstar94/Includes/Tables/Fontes80.Table.s"
	INCLUDE	"Ebonstar94/Includes/Tables/key.table.s"
	INCLUDE	"Ebonstar94/Includes/Tables/Editeur.Data.s"

*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*>>>>>>>>>>>>>>>>>* Adresses utilisées par le programme *<<<<<<<<<<<<<<<<<*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*
*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
*»»»»»»»»»»»»»»»»»»»»»»»»»»» ADRESSES DIVERSES «««««««««««««««««««««««««««*
*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*

*-------------------------*/ Pages graphiques /*--------------------------*
		CNOP	0,4
score.pagegfx	dc.l	0,0,"SCOR",2	080*024*3 => bande des scores
holecol.pagegfx	dc.l	0,0,"HMSK",2	112*105*3 => masque couleur du trou
hole24.pagegfx	dc.l	0,0,"HOL2",2	560*525*2 => 24 trous + masques
hole16.pagegfx	dc.l	0,0,"HOL1",2	448*477*2 => 16 ½ trous
hump.pagegfx	dc.l	0,0,"HUMP",2	320*175*2 => animation des bosses
nfonte.pagegfx	dc.l	0,0,"FONT",2	256*010*3 => 16 fontes de 16x10 (0 à F)
fonte80.pagegfx	dc.l	0,0,"FONT",0	640*008*3 => 80 fontes de 8x8
keymsk.pagegfx	dc.l	0,0,"KMSK",2	320*097*3 => masque clavier

*-------------------------*/ pointeurs divers /*--------------------------*
		CNOP	0,4
level.adr	dc.l	0,LL.SIZE,"NIVO",0	pt/le fichier niveau
joueur.pt	dc.l	0			pt/la premiere structure...
		dc.l	PLAYER.NUM*J.TAILLE	...joueur
		dc.l	"LIST",0
pt_level:	dc.l	0			pt/la structure level
pt_palette	dc.l	0,256*4,"PALE",0	pt/palette de 256 couleurs format AGA
pt_tan:		dc.l	0,0,"TAN!",0		pt/table tangante (routine bob_direction)
pt_explo	dcb.l	EXPLO.NUM,0 		pt/bob explosions
pt_vbl:		dc.l	0			pt/routine vbl en cours

*--------------------* Fichier compactés powerpacker *--------------------*
pt_edscreen_pp	dc.l	0		pt/fichier IFF compacté PP de l'écran éditeur
edscreen_ppsize	dc.l	0,"EDI©",0	taille fichier screen-editeur ILBM compacté PP
pt_keymsk_pp	dc.l	0		pt/fichier IFF compacté PP du masque clavier
keymsk_ppsize	dc.l	0,"KYM©",0	taille fichier masque-keyboard ILBM compacté PP
pt_key_pp	dc.l	0		pt/fichier IFF compacté PP du clavier
key_ppsize	dc.l	0,"KEY©",0	taille fichier screen-keyboard ILBM compacté PP
pt_menu_pp	dc.l	0		pt/fichier IFF compacté PP de l'écran menu
menu_ppsize	dc.l	0,"MEN©",0	taille fichier screen-menu ILBM compacté PP

*---------------------------*/ valeurs écran /*---------------------------*
		CNOP	0,4
ecran_splan	dc.l	0	taille plan écran / largeur ligne écran
ecran_sline	dc.l	0	taille d'une ligne écran en octets
prx.mini	dc.l	0	position x minimum en haute-précision (*1024)
pry.mini	dc.l	0	position y minimum en haute-précision (*1024)
prx.max		dc.l	0	position x maximum en haute-précision (*1024)
pry.max		dc.l	0	position y maximum en haute-précision (*1024)
x.mini		dc.w	0 ¤	position x minimum en pixels
y.mini		dc.w	0 ¤	position y minimum en lignes
x.max		dc.w	0 ¤	position x maximum en pixels
y.max		dc.w	0 !	position y maximum en lignes
ecran_nplan	dc.w	0	Nombre de plans écran

*-----------------------------*/ Drapeaux /*------------------------------*
flag.hole	dc.b	0	1=supprime affichage du trou
flag_pause	dc.b	0	1=pause
flag.raster	dc.b	0	1=affiche raster-line
flag.help	dc.b	0	1=demande informations memoire
flag.del	dc.b	0	1=demande sauveagarde écran
flag.quit	dc.b	0	1=demande le retour au menu 
		EVEN
*------------------------------*/ Divers /*-------------------------------*
		CNOP	0,4

joueur:		dcb.l	PLAYER.NUM,0
hasard:		dc.w	0	nombre "aléatoire"
file.num	dc.w	0	Numéro du fichier
level.num	dc.w	0	Numéro du level binaire
level.num_ascii	dc.b	'00'	Numéro du level ASCII
life.numb	dc.w	5	^ Nbre de vies initiales
fire.numb	dc.w	1	^ Nbre de tirs que donne un ProutBonus
dire.numb	dc.w	1	^ Nbre de directos que donne un DirectoBonus
elec.numb	dc.w	1	! Nbre d'electros que donne un ElectroBonus
dire.time	dc.w	TIME.DIR Temps d'un directo
frek		dc.w	0	0->PAL / 56=NTSC
rez		dc.w	0	0->LOW1 / 1=HIRE / LACE / LOW2
largue		dc.w	0	Temporisation laragage

*-------------------------*/ Bande des scores /*--------------------------*
		CNOP	0,4
;score.printf	dc.l	0,0,"CODE",0 ¤	routine généré PRINT du mentat
level.num_adr	dc.l	0	adresse affichage du level
cycle.score03	dc.l	0	pt/liste Copper couleurs scores bits 0 à 3
cycle.score47	dc.l	0	pt/liste Copper couleurs scores bits 4 à 7
cycle.score1	dc.b	0,1	Durée du cycling , 1++ / 0-- 	
		dc.b	0,1
		dc.b	0,1
		dc.b	0,1
		EVEN

*<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«

fonteraw:	INCBIN	"Mentat/Include/Raw/fontes.raw"
version:	dc.b	"$VER: EBONSTAR94  V0.33 (24.03.95)",0,0
		CNOP	0,4	
	
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
fin_prg:	
		END

*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
*
* Les 32 directions que peut prendre un objet :
*
*		 24 
*	       23  25
*	      22 /\ 26
*	     21	 ||  27
*	    20*	 ||  *28
*	   19	 ||    29
*	  18	 ||	30
*	 17 	 ||	 31
*	16 «=====OO=====» 00
*	 15	 ||      01
*	  14	 ||     02
*	   13	 ||    03
*	    12*	 ||  *04
*	     11	 ||  05
*	      10 \/ 06
*	       09  07
*		 08
*
*<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«
*
* PAGES GRAPHIQUES :
* PAGE BOSSE 		: 40 octets / 175 lignes
* PAGE 24 TROUS 	: 70 octets / 525 lignes
* PAGE 16 ½ TROUS 	: 56 octets / 477 lignes
* PAGE MSK COLOR TROU	: 14 octets / 105 lignes
* PAGE BANDE DES SCORES : 80 octets /  24 lignes
*
*<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«
*
* MEMOIRE GRAPHIQUES :
* ECRANS TRIPLE BUFFER	: 56*312*6*3	=> 314.496 octets ~ 307 K
* PAGE BOSSE 		: 40*175*2	=>  14.000 octets ~  14 K
* PAGE 24 TROUS 	: 70*525*2	=>  73.500 octets ~  72 K
* PAGE 16 ½ TROUS 	: 56*477*2	=>  53.424 octets ~  52 K
* PAGE MSK COLOR TROU	: 14*105*3	=>   4.410 octets ~   4 K
* PAGE BOB + MASQUES	: 40*512*6*2	=> 245.160 octets ~ 240 K
* BANDE DES SCORES	: 80*24*3	=>   5.760 octets ~   6 K
*					   -------
*					   710.750 octets ~ 694 K
*
*<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«=-=»>**<«
