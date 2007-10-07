*-------------------------------------------------------------------------*





*-------------------------------------------------------------------------*
		SECTION	ETB,CODE
		OPT	P+		pas de table de relogement
		OPT	C20+		labels et symboles 20 caract�res maxi.,  majuscules/minuscules respect�es
		OPT	O-		pas d'optimisation
		OPT	p=68020		assemble les instructions MC68020
		INCDIR	DH0:Data/

*>>>>>>>>>>>>>>>>>>>>>>>>>>* CONSTANTES MENTAT *<<<<<<<<<<<<<<<<<<<<<<<<<<*
RethinkDisplay	=-390
RemakeDisplay	=-384
NReservation	=50	50 adresses m�moires r�serv�es maximums
FLAG_AMIGADOS	=0	1=assemble routines chargement AmigaDOS
FLAG_ARP	=0	1=ouvre la librairie ARP
FLAG_BOB32	=0	1=assemble routines BOBs (aff. �cran 32 bits)
FLAG_BOBSTRUCT	=0	1=Assemble la structure BOB
FLAG_CLOSEWB	=0	1=ferme l'�cran WorkBench
FLAG_DMACON.MSK	=0	1=ferme dma bitplane lors d'un acc�s disque-syst�me
FLAG_EXIT	=0	0=sortie sous syst�me possible avec la touche "ESC"
FLAG_FREQ	=0	1=assemble routine demande fr�quence balayage
FLAG_ILBM	=0	1=assemble routines d�compactage IFF-ILBM
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

*>>>>>>>>>>>>>>>>>>>>>>>>>* Debut code MC68020 *<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	INCLUDE	"Mentat/Include/mentat.s"

************* routine appel�e avant de quitter le multit�che **************
sys_extra:	
		moveq	#0,d7
		rts

*>>>>>>>>>>>>>>>>>>>>>>>>>* d�but du programme *<<<<<<<<<<<<<<<<<<<<<<<<<<*
* Entr�e	a5.l	=> adr. 't'
*		a6.l	=> CUSTOM
run:		bsr	first_init
		bsr	sprites_off
		bsr	install_ecran
		tst.l	d7
		bne	sys_erreur

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

************************* Installe l'�cran de jeu *************************
* Entr�e	a5.l	=> adr. table 't'
install_ecran:
		bsr	screen_reserve
		tst.l	d7			d�passement m�moire ?
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

*>>>>>>>>>>>>>>>>>* Adresses utilis�es par le programme *<<<<<<<<<<<<<<<<<*
		INCLUDE	"Mentat/Include/Systeme/t.s"
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
		dc.b	"$VER: Mentat0.31 (24.01.94)",0

*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
