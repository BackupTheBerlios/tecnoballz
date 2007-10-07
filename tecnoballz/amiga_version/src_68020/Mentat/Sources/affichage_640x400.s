*-------------------------------------------------------------------------*

*	Affichage 640x400



*-------------------------------------------------------------------------*
		SECTION	ETB,CODE
		OPT	P+		pas de table de relogement
		OPT	C20+		labels et symboles 20 caract�res maxi.,  majuscules/minuscules respect�es
		OPT	O-		pas d'optimisation
		OPT	p=68020		assemble les instructions MC68020
		INCDIR	DH0:Data/

*>>>>>>>>>>>>>>>>>>>>>>>>>>* CONSTANTES MENTAT *<<<<<<<<<<<<<<<<<<<<<<<<<<*
NReservation	=50	50 adresses m�moires r�serv�es maximums
FLAG_AMIGADOS	=1	1=assemble routines chargement AmigaDOS
FLAG_ARP	=0	1=ouvre la librairie ARP
FLAG_BOB32	=0	1=assemble routines bobs (aff. �cran 32 bits)
FLAG_CLOSEWB	=0	1=ferme l'�cran WorkBench
FLAG_DMACON.MSK	=0	1=ferme dma bitplane lors d'un acc�s disque-syst�me
FLAG_EXIT	=0	0=sortie sous syst�me possible avec la touche "ESC"
FLAG_FREQ	=0	1=assemble routine demande fr�quence balayage
FLAG_ILBM	=1	1=assemble routines d�compactage IFF-ILBM
FLAG_KEY	=1	1=assemble routines de test du clavier
FLAG_LUTIN	=0	1=assemble routines gestion des lutins
FLAG_MEMORY	=0	1=information sur la m�moire possible
FLAG_MOUSE0	=0	1=assemble la routine d�placement souris port 0
FLAG_MUSIC	=0	1=assemble routines gestion module "ProTracker"
FLAG_PP		=0	1=assemble routines d�compactage PowerPacker
FLAG_SOUND	=0	1=assemble routines gestion des bruitages
FLAG_SCROLL	=0	1=assemble routines gestion d�filement �cran
FLAG_TDISK	=0	1=assemble routines de lecture en "TrackDisk"
FLAG_TDISKW	=0	1=assemble routines d'�criture en "TrackDisk"
FLAG_TRACKDISK	=0	1=assemble programme pour �tre charg� en "TrackDisk"

*>>>>>>>>>>>>>>>>>>>>>>>>>* Debut code MC680x0 *<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	INCLUDE	"Mentat/Include/mentat.s"
msg.pers	dc.b	"                                           "	�-128
		EVEN

************* routine appel�e avant de quitter le multit�che **************
sys_extra:	lea	(.name,pc),a0
		moveq	#0,d0
		bsr	charge_fichier_dos
		move.l	d0,(fichier-t,a5)
		rts
.name		dc.b	"DH1:GFXTOOLS/BRILLANCE/IMAGES/AMIGALAGOON2.PIC",0
*.name		dc.b	"DH1:GFXTOOLS/BRILLANCE/IMAGES/AMIGALAGOON.PIC",0
*.name		dc.b	"DH1:GFXTOOLS/BRILLANCE/IMAGES/MEGAPIC",0
*.name		dc.b	"DH0:Data/ILBM/GirlPhone.ilbm",0
*.name		dc.b	"DH0:Data/ILBM/GirlPhone2.ilbm",0
*.name		dc.b	"DH0:Data/ILBM/Desert.ilbm",0
		EVEN
fichier		dc.l	0


*>>>>>>>>>>>>>>>>>>>>>>>>>* d�but du programme *<<<<<<<<<<<<<<<<<<<<<<<<<<*
* Entr�e	a5.l	=> adr. 't'
*		a6.l	=> CUSTOM
run:	
		bsr	first_init
		bsr	sprites_off
		move.l	(fichier-t,a5),d0
		moveq	#1,d1
		moveq	#0,d2
		bsr	affiche_ilbm
		bne	sys_erreur
		move.l	(pt_screen-t,a5),d0

		move.w	(SPHAUTEUR,a0),d0
		subq.w	#1,d0
		moveq	#0,d1
		move.w	(SP_SLINE,a0),d1
		move.l	(SPECRAN1,a0),a1
		moveq	#-1,d2
.loopx		;move.l	d2,(a1)
		;add.l	d1,a1
		;dbf	d0,.loopx



*�����������������* BOUCLE PRINCIPALE HORS INTERRUPTION *�����������������*
main_loop:	
		bsr	wait_vbl
		btst	#6,$bfe001
		bne.s	main_loop
		bra	sys_fin

******** Routine appel�e par l'interruption du temps mort vertical ********
* Entr�e	a5.l	=> Adr. 't'
*		a6.l	=> CUSTOM
extra_vbl:
		rts

*>>>>>>>>>>>>>>>>>* Adresses utilis�es par le programme *<<<<<<<<<<<<<<<<<*
		INCLUDE	"Mentat/Include/Systeme/t.s"
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
		dc.b	"$VER: Affichage640x400 (29.12.93)",0

*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
