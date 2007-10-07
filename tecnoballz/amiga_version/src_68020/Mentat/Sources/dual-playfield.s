*-------------------------------------------------------------------------*

*	Test d'écran dual-playfield en 2 fois 16 couleurs

*	Samedi 23 juillet 1993

*-------------------------------------------------------------------------*
		SECTION	MASTER,CODE
		OPT	P+		pas de table de relogement
		OPT	C20+		labels et symboles 20 caractères maxi.,  majuscules/minuscules respectées
		OPT	O-		pas d'optimisation
		OPT	p=68020		assemble les instructions MC68020
		INCDIR	DH0:Data/

*>>>>>>>>>>>>>>>>>>>>>>>>>>* CONSTANTES MENTAT *<<<<<<<<<<<<<<<<<<<<<<<<<<*
NReservation	=50	50 adresses mémoires réservées maximums
FLAG_ARP	=0	1=ouvre la librairie ARP
FLAG_TRACKDISK	=0	1=assemble programme pour être chargé en "TrackDisk"
FLAG_MEMORY	=0	1=information sur la mémoire possible
FLAG_ILBM	=1	1=assemble routines décompactage IFF-ILBM
FLAG_PP		=0	1=assemble routines décompactage PowerPacker
FLAG_MOUSE0	=0	1=assemble la routine déplacement souris port 0
FLAG_AMIGADOS	=1	1=assemble routines chargement AmigaDOS
FLAG_EXIT	=0	0=sortie sous système possible avec la touche "ESC"
FLAG_MUSIC	=0	1=assemble routines gestion module "ProTracker"
FLAG_KEY	=1	1=assemble routines de test du clavier
FLAG_SOUND	=0	1=assemble routines gestion des bruitages
FLAG_LUTIN	=0	1=assemble routines gestion des lutins
FLAG_TDISK	=0	1=assemble routines de lecture en "TrackDisk"
FLAG_TDISKW	=0	1=assemble routines d'écriture en "TrackDisk"
FLAG_SCROLL	=0	1=assemble routines gestion défilement écran
FLAG_BOB32	=0	1=assemble routines bobs (aff. écran 32 bits)
FLAG_FREQ	=0	1=assemble routine demande fréquence balayage
FLAG_CLOSEWB	=0	1=ferme l'écran WorkBench
FLAG_DMACON.MSK	=0	1=ferme dma bitplane lors d'un accès disque-système

*>>>>>>>>>>>>>>>>>>>>>>>>>* Debut code MC680x0 *<<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	INCLUDE	"Mentat/Include/mentat.s"
msg.pers	dc.b	"                                           "	¤-128
		EVEN

************* routine appelée avant de quitter le multitâche **************
sys_extra:	
		lea	(okfrau_name,pc),a0
		moveq	#0,d0
		bsr	charge_fichier_dos
		bne.s	.exit
		move.l	d0,(okfrau_ilbm-t,a5)
		lea	(okgirl_name,pc),a0
		moveq	#0,d0
		bsr	charge_fichier_dos
		bne.s	.exit
		move.l	d0,(okgirl_ilbm-t,a5)
		moveq	#0,d7
.exit		rts
okfrau_name	dc.b	"DH0:DATA/ILBM/okfrau.iff",0
okgirl_name	dc.b	"DH0:DATA/ILBM/okgirl.iff",0
		EVEN

*>>>>>>>>>>>>>>>>>>>>>>>>>* début du programme *<<<<<<<<<<<<<<<<<<<<<<<<<<*
* Entrée	a5.l	=> adr. 't'
*		a6.l	=> CUSTOM
run:		bsr	first_init
		bsr	sprites_off
		bsr	install_ecran
		tst.l	d7
		bne	sys_erreur

*¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤* BOUCLE PRINCIPALE HORS INTERRUPTION *¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤*
main_loop:	
		bsr	wait_vbl
		btst	#6,$bfe001
		bne.s	main_loop
		bra	sys_fin

******** Routine appelée par l'interruption du temps mort vertical ********
* Entrée	a5.l	=> Adr. 't'
*		a6.l	=> CUSTOM
extra_vbl:
		rts

************************* Installe l'écran de jeu *************************
* Entrée	a5.l	=> adr. table 't'
install_ecran:
		bsr	screen_reserve
		tst.l	d7			dépassement mémoire ?
		bne	.exit			-> oui
		move.w	#320,(SNPIXEL,a0)
		move.w	#256,(SNLIGNE,a0)
		move.b	#8,SNPLAN(a0)
		move.w	#1,(SPBUFFER,a0)
		move.w	#384,(SPLARGE,a0)
		move.w	#512,(SPHAUTEUR,a0)
		move.w	#384,(SILARGE,a0)
		move.w	#512,(SIHAUTEUR,a0)
		move.b	#1,SDBPLF(a0)		Dual-Playfield
		move.w	#3,(SFMODE_PLANS,a0)	mode X4
		move.w	#3,(SFMODE_LUTINS,a0)	lutin de 64 pixels de large
		clr.w	(SLUTIN_RES,a0)		lutin affiché en basse résolution
		move.w	#3,(SFADEOFFSPEED,a0)
		move.w	#3,(SFADEONSPEED,a0)
		bsr	init_screen
		tst.l	d7
		bne.s	.exit

		* décompacte image 1 * 
		lea	(SCOLOR_TABLE,a0),a2	a2=adresse table couleur
		move.l	(SPECRAN1,a0),a3		a3=adr. de décompactage
		move.l	(okgirl_ilbm,pc),a4	a4=adr. du fichier ILBM
		bsr	decompacte_ilbm		|-> routine de décompactage
		tst.l	d7

		* décompacte image 2 * 
		move.l	(pt_screen-t,a5),a0
		lea	(SCOLOR_TABLE+32*4,a0),a2 a2=adresse table couleur
		move.l	(SIECRAN1,a0),a3	a3=adr. de décompactage
		move.l	(okfrau_ilbm,pc),a4	a4=adr. du fichier ILBM
		bsr	decompacte_ilbm		|-> routine de décompactage
		tst.l	d7
		bne.s	.exit
		move.l	(pt_screen-t,a5),a0
		move.w	#257,(flag_fade_on-t,a5)
.exit		rts

*>>>>>>>>>>>>>>>>>* Adresses utilisées par le programme *<<<<<<<<<<<<<<<<<*
		CNOP	0,4			aligne sur un long mot
okfrau_ilbm	dc.l	0
okgirl_ilbm	dc.l	0
		INCLUDE	"Mentat/Include/Systeme/t.s"
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
		dc.b	"$VER: Dual-Playfield v0.1 (24.07.93)",0

*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
