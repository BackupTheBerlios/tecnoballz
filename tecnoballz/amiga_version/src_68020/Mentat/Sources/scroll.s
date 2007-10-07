*-------------------------------------------------------------------------*
*
*	DEFILEMENT HORIZONTAL AU QUART DE PIXEL D'UNE PAGE
*	VITUELLE EN 320x256 8 plans
*
*-------------------------------------------------------------------------*
		SECTION	ETB,CODE
		OPT	P+		Pas de table de relogement
		OPT	C20+		Labels et symboles 20 caract�res maxi.,  majuscules/minuscules respect�es
		OPT	O-		Pas d'optimisation
		OPT	p=68020
		INCDIR	DH0:Data/

*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONSTANTES <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*
NReservation	=50	50 adresses m�moires r�serv�es maximums
FLAG_ARP	=0	1=ouvre la librairie ARP
FLAG_TRACKDISK	=0	1=assemble programme pour �tre charg� en "TrackDisk"
FLAG_MEMORY	=0	1=information sur la m�moire possible
FLAG_ILBM	=1	1=assemble routines d�compactage IFF-ILBM
FLAG_PP		=0	1=assemble routines d�compactage PowerPacker
FLAG_MOUSE0	=0	1=assemble la routine d�placement souris port 0
FLAG_AMIGADOS	=1	1=assemble routines chargement AmigaDOS
FLAG_EXIT	=0	0=sortie sous syst�me possible avec la touche "ESC"
FLAG_MUSIC	=0	1=assemble routines gestion module "ProTracker"
FLAG_KEY	=1	1=assemble routines de test du clavier
FLAG_SOUND	=0	1=assemble routines gestion des bruitages
FLAG_LUTIN	=0	1=assemble routines gestion des lutins
FLAG_TDISK	=0	1=assemble routines de lecture en "TrackDisk"
FLAG_TDISKW	=0	1=assemble routines d'�criture en "TrackDisk"
FLAG_SCROLL	=0	1=assemble routines gestion d�filement �cran
FLAG_BOB32	=0	1=Alors assemble routines bobs (aff. �cran 32 bits)
FLAG_FREQ	=0	1=Alors assemble routine demande fr�quence balayage
FLAG_CLOSEWB	=0	1=Alors ferme l'�cran WorkBench
FLAG_DMACON.MSK	=0

*>>>>>>>>>>>>>>>>>>>>>>>>>> Debut code MC68020 <<<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	
		INCLUDE	"Mentat/Include/mentat.s"
msg.pers	dc.b	"                                           "	�-128
		EVEN

************* Routine appel�e avant de quitter le multit�che **************
sys_extra:	
		lea	(name.ilbm,pc),a0
		moveq	#0,d0
		bsr	charge_fichier_dos
		bne.s	.exit
		move.l	d0,(adresse.ilbm-t,a5)
		moveq	#0,d7
.exit		rts
name.ilbm	dc.b	"DH0:DATA/ILBM/fondateurs",0
		EVEN

*>>>>>>>>>>>>>>>>>>>>>>>>>> d�but du programme <<<<<<<<<<<<<<<<<<<<<<<<<<<*
run:	
		bsr	first_init
		bsr	sprites_off
		move.l	(adresse.ilbm-t,a5),d0
		moveq	#1,d1			on d�sire un seul buffer
		moveq	#1,d2			on d�sire un hardscroll
		bsr	affiche_ilbm

*������������������ BOUCLE PRINCIPALE HORS INTERRUPTION ������������������*
main_loop:
		bsr	wait_vbl
		bsr	tennis
		btst	#6,$bfe001
		bne.s	main_loop
		clr.w	(flag_fade_on-t,a5)
		move.w	#256,(flag_fade_off-t,a5)
.ww		tst.w	(flag_fade_off-t,a5)
		bne.s	.ww
		bra	sys_fin

********************** Calcule la valeur de BPLCON1 ***********************
* BLTCON1
* % pF pE pD pC : iB iA i9 i8 : p7 p6 p5 p4 : i3 i2 i1 i0
* iB.iA.i3.i2.i1.i0.i9.i8
* 07;06;05;04;03;02;01;00
* Entr�e	=> d0.w	valeur du d�calage hard de 0 � 255
* Sortie	<= d3.w	valeur de BPLCON1
* Utilise	-- (d0,d1,d2,d3)
decalage_aga:
		move	d0,d1			d1=d�calage de 0 � 255
		move	d1,d2			d2=d�calage de 0 � 255
		lsr	#2,d2			�limine bits 0 & 1
		and	#$0f,d2			garde bits 2 � 5
		move	d2,d3			pour plans impaires
		lsl	#4,d2
		or	d2,d3			pour plans paires
		move	d1,d2			d2=d�calage de 0 � 255
		lsl	#4,d2
		and	#$0c00,d2		isole bits 6 & 7
		and	#$0003,d1		isole bits 0 & 1
		lsl	#8,d1
		or	d1,d2
		or	d2,d3			pour plans paires
		lsl	#4,d2
		or	d2,d3			pour plans impaires
		rts

*:*:*:*:*:*:*:*:*:*:*:*:*: Gestion du d�filement :*:*:*:*:*:*:*:*:*:*:*:*:*
* Entr�e	a5.l	table 't'
tennis:
		lea	(.tempo,pc),a0
		subq	#1,(a0)			Tempo pour ralentir
		;bne	.end
		move	#100,(a0)
		move.l	(pt_screen-t,a5),a0	a0=Adr. structure �cran
		lea	(.offset,pc),a1
		move	(a1)+,d0
		add	d0,(a1)
		bne.s	.ok
		move	#1,(-2,a1)
		bra.s	.go
.ok		cmp	#1279,(a1)
		bne.s	.go
		move	#-1,(-2,a1)
.go		moveq	#0,d3
		move	(a1),d3
		move.l	d3,d0
		lsr	#2,d0			1280 -> 320
		lsr	#3,d0			320 -> 40
		and	#$fff8,d0		tous les 8 octets
		add.l	(SPECRAN1,a0),d0
		bsr	pointe
		move.l	(SPDECALAGE,a0),a1
		move	d3,d0
		not	d0
		and	#$ff,d0
		bsr	decalage_aga
		move	d3,(2,a1)
.end		rts
.offset		dc	1,0
.tempo		dc	1

******** Routine appel�e par l'interruption du temps mort vertical ********
; Entr�e	a5.l	Adr. 't'
extra_vbl:	rts

adresse.ilbm	dc.l	0

*>>>>>>>>>>>>>>>>>> Adresses utilis�es par le programme <<<<<<<<<<<<<<<<<<*
		INCLUDE	"Mentat/Include/Systeme/t.s"
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
		dc.b	"$VER: SmallScroll V0.1 (25.07.93)",0

*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><*
