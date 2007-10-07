*-------------------------------------------------------------------------*
*
*	Affiche une image en "triple buffering" 		
*
*	Le 11 décembre 1993
*	
*-------------------------------------------------------------------------*
		SECTION	ETB,CODE
		OPT	P+		Pas de table de relogement
		OPT	C20+		Labels et symboles 20 caractères maxi.,  majuscules/minuscules respectées
		OPT	O-		Pas d'optimisation
		OPT	p=68020	
		INCDIR	dh0:Data/


;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONSTANTES <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
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
FLAG_BOB32	=0	Si=1 Alors assemble routines bobs (aff. écran 32 bits)
FLAG_FREQ	=0	Si=1 Alors assemble routine demande fréquence balayage
FLAG_CLOSEWB	=0	Si=1 Alors ferme l'écran WorkBench
FLAG_DMACON.MSK	=0

*>>>>>>>>>>>>>>>>>>>>>>>>>> Debut code MC68020 <<<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	
		INCLUDE	"Mentat/Include/mentat.s"
msg.pers	dc.b	"                                           "	¤-128
		EVEN

************* Routine appelée avant de quitter le multitâche **************
sys_extra:	
		lea	(name.ilbm,pc),a0
		moveq	#0,d0
		bsr	charge_fichier_dos
		bne.s	.exit
		move.l	d0,(adresse.ilbm-t,a5)
		moveq	#0,d7
.exit		rts
name.ilbm	dc.b	"DH0:DATA/ILBM/Planete1.ilbm",0
		EVEN

*>>>>>>>>>>>>>>>>>>>>>>>>>> début du programme <<<<<<<<<<<<<<<<<<<<<<<<<<<*
run:	
		bsr	first_init
		bsr	sprites_off
		move.l	(adresse.ilbm-t,a5),d0
		moveq	#3,d1			3 buffers
		moveq	#0,d2			pas de défilement possible
		bsr	affiche_ilbm
		move.l	(pt_screen-t,a5),a0
		moveq	#0,d0
		move.w	(SP_SLINE,a0),d0
		mulu	#32,d0
		addq.l	#8,d0
		add.l	d0,(SPECRAN1,a0)
		add.l	d0,(SPECRAN2,a0)
		add.l	d0,(SPECRAN3,a0)

*¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤ BOUCLE PRINCIPALE HORS INTERRUPTION ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤*
main_loop:
		bsr	flip_ecrans
		bsr	wait_vbl
		btst	#6,$bfe001
		bne.s	main_loop
		bra	sys_fin

******** Routine appelée par l'interruption du temps mort vertical ********
* Entrée	a5.l	=> Adr. 't'
extra_vbl:	rts
adresse.ilbm	dc.l	0

*>>>>>>>>>>>>>>>>>> Adresses utilisées par le programme <<<<<<<<<<<<<<<<<<*
		INCLUDE	"Mentat/Include/Systeme/t.s"
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
		dc.b	"$VER: Mentat0.3 (11.12.93)",0
		EVEN

*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><*
