*-------------------------------------------------------------------------*
*	Essai de "Protracker Replay Routine"
*-------------------------------------------------------------------------*
		SECTION	SUCRE_D_ORGE,CODE
		*OPT	P+		pas de table de relogement
		OPT	C20+		labels et symboles 20 caractères maxi.,  majuscules/minuscules respectées
		OPT	O-		pas d'optimisation
		OPT	p=68020		assemble les instructions MC68020
		INCDIR	DH0:Data/

*>>>>>>>>>>>>>>>>>>>>>>>>>>* CONSTANTES MENTAT *<<<<<<<<<<<<<<<<<<<<<<<<<<*
NReservation	=50	50 adresses mémoires réservées maximums
FLAG_ARP	=0	1=ouvre la librairie ARP
FLAG_TRACKDISK	=0	1=assemble programme pour être chargé en "TrackDisk"
FLAG_MEMORY	=0	1=information sur la mémoire possible
FLAG_ILBM	=0	1=assemble routines décompactage IFF-ILBM
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

*>>>>>>>>>>>>>>>>>>>>>>>>>* Debut code MC680x0 *<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	INCLUDE	"Mentat/Include/mentat.s"

************* routine appelée avant de quitter le multitâche **************
sys_extra:	lea	(music.name,pc),a0
		moveq	#MEMF_CHIP,d0
		bsr	charge_fichier_dos
		bne.s	.exit		
		move.l	d0,(music.adr-t,a5)
		moveq	#0,d7
.exit		rts
music.name	dc.b	"DH2:tempo/modules/Arte/mod.Arte_Elekfunk !",0
		dc.b	"Ebonstar94:Ebonstar94/Sfx/mod.EqIntroFinal",0
		dc.b	"Ebonstar94:Ebonstar94/Sfx/mod.InGameMusic1",0
		dc.b	"Ebonstar94:Ebonstar94/Sfx/mod.Eq2",0
		EVEN
music.adr	dc.l	0


*>>>>>>>>>>>>>>>>>>>>>>>>>* début du programme *<<<<<<<<<<<<<<<<<<<<<<<<<<*
* Entrée	a5.l	=> adr. 't'
*		a6.l	=> CUSTOM
run:		bsr	first_init
		bsr	sprites_off
		bsr	install_ecran
		tst.l	d7
		bne	sys_erreur
		move.l	(music.adr,pc),a0
		bsr	MT_Init

*¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤* BOUCLE PRINCIPALE HORS INTERRUPTION *¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤*
main_loop:
		bsr	wait_vbl
VBlankLoop:	cmpi.b	#$a0,(VHPOSR,a6)	should use WaitTOF()
		bne.s	VBlankLoop		from graphics.library
		move.l	(pt_screen-t,a5),a0
		move.w	(SBPLCON3,a0),(BPLCON3,a6)
		move.w	#$00F,(COLOR00,a6)
		bsr	MT_Music
		move.l	(pt_screen-t,a5),a0
		move.w	(SBPLCON3,a0),(BPLCON3,a6)
		move.w	#$000,(COLOR00,a6)
		moveq	#0,d0
		move.b	(VHPOSR,a6),d0
		sub.w	#$A0,d0
		cmp.w	(rl-t,a5),d0
		blt.s	.okl
		move.w	d0,(rl-t,a5)

		moveq	#0,d2
		move.w	(toto-t,a5),d2
		bsr	convertit_decimal
		moveq	#5-1,d1
		lea	(titi,pc),a0
		bsr	conv_number_ascii

		moveq	#0,d2
		move.w	(rl-t,a5),d2
		bsr	convertit_decimal
		moveq	#3-1,d1
		lea	(tutu,pc),a0
		bsr	conv_number_ascii


		lea	(text,pc),a0
		moveq	#0,d0
		moveq	#0,d1
		bsr	printf




.okl		btst	#6,$bfe001
		bne	main_loop
		bsr	MT_End
		bra	sys_fin

text		dc.b	"Compteur de 'RetrigNotes' => "
titi		dc.b	"00000",0
		dc.b	"Valeur maximum de 'RasterLines' => "
tutu		dc.b	"000",0
		dc.b	0
		EVEN

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
		bne.s	.exit			-> oui
		move.w	#3,(SFMODE_PLANS,a0)
		move.w	#3,(SFMODE_LUTINS,a0)
		move.w	#640,(SNPIXEL,a0)
		move.w	#256,(SNLIGNE,a0)
		move.b	#1,(SNPLAN,a0)
		move.w	#1,(SPBUFFER,a0)
		move.w	#640,(SPLARGE,a0)
		move.w	#256,(SPHAUTEUR,a0)
		move.b	#1,(SHIRES,a0)
		move.l	#$000000,(SCOLOR_TABLE,a0)
		move.l	#$CC0201,(4+SCOLOR_TABLE,a0)
		bsr	init_screen
		bsr	convert_aga_color
		bsr	colors_on
		moveq	#0,d7
.exit		rts

		INCLUDE	"Mentat/Include/musique/protracker_replay.s"

*>>>>>>>>>>>>>>>>>* Adresses utilisées par le programme *<<<<<<<<<<<<<<<<<*
rl:		dc	0
		INCLUDE	"Mentat/Include/Systeme/t.s"
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
		dc.b	"$VER: REPLAY (02.03.94)",0

*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*
