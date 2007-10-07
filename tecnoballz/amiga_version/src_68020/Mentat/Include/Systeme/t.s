*-------------------------------------------------------------------------*
* \/\/\/\/\/\/\/\/\/\/\/\/ LA FAMEUSE TABLE 'T' /\/\/\/\/\/\/\/\/\/\/\/\/ *
*-------------------------------------------------------------------------*
*/  "¤" : adr. initialisé par 'first_init'.				 /*
*/  "^" : ne rien intercaler entre cette adresse et la suivante.	 /*
*/  "!" : ne rien intercaler entre cette adresse et la précédente.	 /*
*-------------------------------------------------------------------------*
		CNOP	0,4		* ALIGNE SUR UN LONG MOT *
*-------------------------------------------------------------------------*
t:
* longs mots de 32 bits *
atable_clavier	dc.l	0	¤	adr. de la table clavier + 4
adr_fonteraw	dc.l	0	¤	adr. police GFX ASCII (2048 pixels / 8 lignes / 1 plan)
adr_resmem	dc.l	0	¤	adr. routine réservation mémoire
asound_table	dc.l	0,0,"SOUN",2	¤	adr. de la table des sons
mem_liste.adr	dc.l	0		adr. de la table mémoire
mem_liste.size	dc.l	0		taille de la table mémoire
acopper		dc.l	0		adr. de la liste Copper courante
pt_screen:	dc.l	0,SIZE_ST_ECRAN,"PTSC",0		adr. structure écran en cours
pt_scroll	dc.l	0		adr. structure défilement en cours
mod_protracker	dc.l	0		adr. module en cours
base_vecteurs	dc.l	0		base des 64 vecteurs MC608x0
adrive_0	dc.l	0		adr. structure disque 0
adrive_1	dc.l	0		adr. structure disque 1
adrive_2	dc.l	0		adr. structure disque 2
adrive_3	dc.l	0		adr. structure disque 3
drive_current	dc.l	0		adr. structure disque en cours
atrackdisk_load	dc.l	0		adr. routine 'trackdisk_load'
adrive_motor	dc.l	0		adr. routine 'drive_motor'
await_timer	dc.l	0		adr. routine 'wait_timer'
adecompacte_pp	dc.l	0		adr. routine 'decompacte_pp'
ecran_travail	dc.l	0		adr. écran de travail
ecran_affiche	dc.l	0		adr. écran affiché
mentat_routine	dc.l	0		routine appellée par mentat info
mentat_screen	dc.l	0,FT.SLINE*5*8,"SCRN",2
infoblock.pt	dc.l	0,IB.SIZE,"LIST",0	pt/liste 'infoblock'
tableclavier.pt	dc.l	0,132,"TKEY",0	¤	adr. de la table clavier

pp.temp		dc.l	0,0,"LOAD",0		RESA chargement temporaire
pp.resa		dc.l	0
load.temp	dc.l	0,0,"LOAD",0		RESA chargement temporaire
load.resa	dc.l	0			pt/structure RESA
load.name	dc.l	0			pt/nom
load.colo	dc.l	0

mentat_nline	dc.w	0		nombre de ligne(s) text
flag_sys_gele	dc.w	0		flag 1=alors système Amiga est gelé
flag_sys_gele2	dc.w	0		flag 1=alors système Amiga est gelé
flag_fade_on	dc.w	0		flag/comp. fondu vers couleurs finales
flag_fade_off	dc.w	0		flag/comp. fondu vers la couleur noire
flag_play	dc.w	0		flag 1=interdit saut à la playroutine
flag_sound	dc.w	0		flag 1=interdit saut routine saut
vbl_counter	dc.w	0		compteur de VBL
joy0dat		dc.w	0		valeur du registre spécifique 'JOY0DAT'
mouse_y		dc.w	0	^	position x souris port 0
mouse_x		dc.w	0	^	position y souris port 0
mouse_minix	dc.w	0	^	position x minimum souris
mouse_maxix	dc.w	0	^	position x maximum souris
mouse_miniy	dc.w	0	^	position y minimum souris
mouse_maxiy	dc.w	0	!	position y maximum souris
genlock		dc.w	0

*---------------<* boutons souris & bouton feu joystick *>----------------*
mouse1_three	dc.b	0	^	flag $ff=3ieme bouton souris port1 enfoncé
mouse0_three	dc.b	0	^	flag $ff=3ieme bouton souris port0 enfoncé
mouse1_right	dc.b	0	^	flag $ff=bouton droit souris port1 enfoncé
mouse0_right	dc.b	0	^	flag $ff=bouton droit souris port0 enfoncé
mouse1_left	dc.b	0	^	flag $ff=bouton gauche souris port1 enfoncé
mouse0_left	dc.b	0	!	flag $ff=bouton gauche souris port0 enfoncé
xcurs:		dc.w	0		position curseur routine input
dmacon		dc.w	$8670		valeur actuelle de DMACON
frequence	dc.w	0	¤	fréquence de balayage 50hz ou 60hz
flag_flip	dc.w	0		flag "double-buffering" 0 ou 4
joy0_right	dc.b	0	^	flag $ff=joystick 0 à droite
joy0_left	dc.b	0	^	flag $ff=joystick 0 à gauche
joy0_down	dc.b	0	^	flag $ff=joystick 0 en bas 
joy0_up		dc.b	0	^	flag $ff=joystick 0 en haut
joy1_right	dc.b	0	^	flag $ff=joystick 1 à droite
joy1_left	dc.b	0	^	flag $ff=joystick 1 à gauche
joy1_down	dc.b	0	^	flag $ff=joystick 1 en bas 
joy1_up		dc.b	0	!	flag $ff=joystick 1 en haut
dmacon.msk	dc.w	0		masque de DMACON dans 'sys_restore'
vblfade.flag	dc.b	0		0=routine fade appellée pendant la VBL
		EVEN
*
*---------------------* Table adresses des 8 lutins *---------------------*
* table utilsée par la routine 'init_pts_lutins' pour initialiser les
* pointeurs lutins de la liste Copper
		IFNE	FLAG_LUTIN
		CNOP	0,4		* ALIGNE SUR UN LONG MOT *
table_lutins:	dcb.l	8,0
		ENDC
*
*-*-*-*-*-*-*-*- Adresses utilisées par les routines bobs -*-*-*-*-*-*-*-*-
		IFNE	FLAG_BOB32
		INCLUDE	"Mentat/Include/BOB32/bob32_data.s"
		ENDC
