*-------------------------------------------------------------------------*

*-------------------------------------------------------------------------*
		SECTION	ETB,CODE
		OPT	P+		pas de table de relogement
		OPT	C20+		labels et symboles 20 caract�res maxi.,  majuscules/minuscules respect�es
		OPT	p=68020		assemble les instructions MC68020
		INCDIR	DH0:Data/

*>>>>>>>>>>>>>>>>>>>>>>>>>>* CONSTANTES MENTAT *<<<<<<<<<<<<<<<<<<<<<<<<<<*
NReservation	=50	50 adresses m�moires r�serv�es maximums
FLAG_AMIGADOS	=0	1=assemble routines chargement AmigaDOS
FLAG_BOB32	=0	1=assemble routines BOBs (aff. �cran 32 bits)
FLAG_BOBSTRUCT	=0	1=Assemble la structure BOB
FLAG_DMACON.MSK	=0	1=ferme dma bitplane lors d'un acc�s disque-syst�me
FLAG_EXIT	=0	0=sortie sous syst�me possible avec la touche "ESC"
FLAG_FREQ	=0	1=assemble routine demande fr�quence balayage
FLAG_ILBM	=0	1=assemble routines d�compactage IFF-ILBM
FLAG_KEY	=1	1=assemble routines de test du clavier
FLAG_LUTIN	=0	1=assemble routines gestion des lutins
FLAG_MEMORY	=0	1=information sur la m�moire possible
FLAG_MOUSE0	=0	1=assemble la routine d�placement souris port 0
FLAG_PP		=0	1=assemble routines d�compactage PowerPacker

*>>>>>>>>>>>>>>>>>>>>>>>>>* Debut code MC68020 *<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	INCLUDE	"Mentat/Include/mentat.s"
msg.pers	dc.b	"                                           "	�-128
		EVEN

************* routine appel�e avant de quitter le multit�che **************
sys_extra:	
		moveq	#0,d7
		rts
sys_extra2:	rts

*>>>>>>>>>>>>>>>>>>>>>>>>>* d�but du programme *<<<<<<<<<<<<<<<<<<<<<<<<<<*
* Entr�e	a5.l	=> adr. 't'
*		a6.l	=> CUSTOM
run:		bsr	first_init
		bsr	sprites_off
		bsr	install_ecran
		bne	sys_erreur
		moveq	#0,d0			Pos. x octets
		moveq	#0,d1			Pos. y octets
		lea	(version,pc),a0
		bsr	printf

*/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*
*�����������������* BOUCLE PRINCIPALE HORS INTERRUPTION *�����������������*
*/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*
main_loop:	
		bsr	wait_vbl
		tst.b	(mouse0_left-t,a5)	Bouton gauche souris ?
		beq.s	main_loop		-> Non
		bra	sys_fin

*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*
******** Routine appel�e par l'interruption du temps mort vertical ********
*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*
* Entr�e	a5.l	=> Adr. 't'
*		a6.l	=> CUSTOM
extra_vbl:
		rts

*�*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*
*����������������������| Initialisation d'un �cran |����������������������*
*�*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*��*
* Entr�e	=> a5.l Adr. table 't'
* Sortie	<= d7.l	#0 Pas d'erreur
install_ecran:
		bsr	screen_reserve		|-> R�serve structure m�moire
		bne.s	.exit			-> erreur
		move.w	#3,(SFMODE_PLANS,a0)	Mode AGA Plans BurstX4
		move.w	#3,(SFMODE_LUTINS,a0)	Mode AGA Lutins BurstX4
		move.w	#640,(SNPIXEL,a0)	R�solution horizontale �cran
		move.w	#256,(SNLIGNE,a0)	R�solution verticale �cran
		move.b	#1,(SNPLAN,a0)		Nombre de plans binaires (max. 8)
		move.w	#1,(SPBUFFER,a0)	Nombre d'�cran (maximum 3)
		move.w	#640,(SPLARGE,a0)	Largeur de la page �cran
		move.w	#256,(SPHAUTEUR,a0)	Hauteur de la page �cran
		move.b	#1,(SHIRES,a0)		Haute r�solution
		lea	(SCOLOR_TABLE,a0),a1
		clr.l	(a1)+			Valeur couleur 0
		move.l	#$CC0201,(a1)+		Valeur couleur 1
		bsr	init_screen		|-> Initialise un �cran
		bne.s	.exit			-> erreur
		bsr	convert_aga_color	Format couleur 32 bits -> AGA 
		bsr	colors_on		Init. registres couleurs
		moveq	#0,d7			� 0=pas d'erreur
.exit		rts

*>>>>>>>>>>>>>>>>>* Adresses utilis�es par le programme *<<<<<<<<<<<<<<<<<*
		INCLUDE	"Mentat/Include/Systeme/t.s"
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
version:	dc.b	"$VER: Mentat0.38 (05.06.95)",0,0
		CNOP	0,4
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*

