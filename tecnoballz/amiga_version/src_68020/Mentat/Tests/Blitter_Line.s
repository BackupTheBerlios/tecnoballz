*-------------------------------------------------------------------------*

*		TRACE DE DROITES AU BLITTER

*		LE 28 JANVIER 1994

*-------------------------------------------------------------------------*
		SECTION	ETB,CODE
		OPT	P+		pas de table de relogement
		OPT	C20+		labels et symboles 20 caractères maxi.,  majuscules/minuscules respectées
		OPT	O-		pas d'optimisation
		OPT	p=68020		assemble les instructions MC68020
		INCDIR	DH0:Data/

*>>>>>>>>>>>>>>>>>>>>>>>>>>* CONSTANTES MENTAT *<<<<<<<<<<<<<<<<<<<<<<<<<<*
NReservation	=50	50 adresses mémoires réservées maximums
FLAG_AMIGADOS	=1	1=assemble routines chargement AmigaDOS
FLAG_ARP	=0	1=ouvre la librairie ARP
FLAG_BOB32	=0	1=assemble routines BOBs (aff. écran 32 bits)
FLAG_BOBSTRUCT	=0	1=Assemble la structure BOB
FLAG_CLOSEWB	=0	1=ferme l'écran WorkBench
FLAG_DMACON.MSK	=0	1=ferme dma bitplane lors d'un accès disque-système
FLAG_EXIT	=0	0=sortie sous système possible avec la touche "ESC"
FLAG_FREQ	=0	1=assemble routine demande fréquence balayage
FLAG_ILBM	=1	1=assemble routines décompactage IFF-ILBM
FLAG_KEY	=1	1=assemble routines de test du clavier
FLAG_LUTIN	=0	1=assemble routines gestion des lutins
FLAG_MEMORY	=0	1=information sur la mémoire possible
FLAG_MOUSE0	=0	1=assemble la routine déplacement souris port 0
FLAG_MUSIC	=0	1=assemble routines gestion module "ProTracker"
FLAG_PP		=0	1=assemble routines décompactage PowerPacker
FLAG_SOUND	=0	1=assemble routines gestion des bruitages
FLAG_SCROLL	=0	1=assemble routines gestion défilement écran
FLAG_TDISK	=0	1=assemble routines de lecture en "TrackDisk"
FLAG_TDISKW	=0	1=assemble routines d'écriture en "TrackDisk"
FLAG_TRACKDISK	=0	1=assemble programme pour être chargé en "TrackDisk"

*>>>>>>>>>>>>>>>>>>>>>>>>>* Debut code MC68020 *<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	INCLUDE	"Mentat/Include/mentat.s"

************* routine appelée avant de quitter le multitâche **************
sys_extra:	
		lea	(name,pc),a0
		moveq	#0,d0
		bsr	charge_fichier_dos
		bne.s	.error
		move.l	d0,(pageiff-t,a5)
		moveq	#0,d4
.error		rts
name		dc.b	'DH0:DATA/ILBM/PIC.ILBM',0
		EVEN

*>>>>>>>>>>>>>>>>>>>>>>>>>* début du programme *<<<<<<<<<<<<<<<<<<<<<<<<<<*
* Entrée	a5.l	=> adr. 't'
*		a6.l	=> CUSTOM
run:		bsr	first_init
		bsr	sprites_off
		bsr	install_ecran
		bne	sys_erreur
		bsr	init_ymulu
		bne	sys_erreur
		bsr	init_xbyte
		bne	sys_erreur
		bsr	init_xplot
		bne	sys_erreur

		*/ RECOPIE ECRAN 1 DANS ECRAN 2 \*
		bsr	test_si_ecran
		add.l	(SP_SIZE,a0),a2		a2=adr. fin écran 2
		move.w	(SP_SLINE,a0),d7	d7=taille ligne écran
		move.w	(SPHAUTEUR,a0),d5	d5=nbre de lignes
		subq.w	#1,d5
		move.w	d7,d0
		lsr.w	#1,d0
		subq.w	#1,d0
		sub.w	d7,a2
		add.w	d7,d7
.loopc0		move.w	d0,d1
.loopc1		move.w	(a1)+,(a2)+
		dbf	d1,.loopc1
		sub.w	d7,a2
		dbf	d5,.loopc0


*¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤* BOUCLE PRINCIPALE HORS INTERRUPTION *¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤*
main_loop:	

;		move.w	#$20,(INTREQ,a6)
;.vblwait	move.w	(INTREQR,a6),d0
;		and.w	#$20,d0
;		beq.s	.vblwait
;		move.w	#$20,(INTREQ,a6)

		bsr	wait_vbl
		move.l	(pt_screen-t,a5),a0
		move.w	(SBPLCON3,a0),(BPLCON3,a6)
		move.w	#$00F,(COLOR00,a6)
		bsr	draw
		move.l	(pt_screen-t,a5),a0
		move.w	(SBPLCON3,a0),(BPLCON3,a6)
		move.w	#$000,(COLOR00,a6)

		;bsr	clav
		btst	#6,$bfe001
		bne.s	main_loop
.wait		btst	#6,$bfe001
		beq.s	.wait
		bra	sys_fin

*»»»»»»»»»»»»»» TEST CLAVIER AFFICHE UNE DES 3 PAGES ECRAN «««««««««««««««*
clav:		move.l	(atable_clavier-t,a5),a0
		tst.b	(TOUCHE_F1,a0)
		bne.s	.touchef1
		tst.b	(TOUCHE_F2,a0)
		bne.s	.touchef2
		tst.b	(TOUCHE_F3,a0)
		bne.s	.touchef3
.touchep	tst.b	(TOUCHE_P,a0)
		bne.s	.touchep
		rts
.touchef1	bra	pointe_ecran1
.touchef2	bra	pointe_ecran2
.touchef3	bra	pointe_ecran3

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
		move.w	#320,(SNPIXEL,a0)
		move.w	#256,(SNLIGNE,a0)
		move.b	#8,(SNPLAN,a0)
		move.w	#3,(SPBUFFER,a0)
		move.w	#320,(SPLARGE,a0)
		move.w	#256,(SPHAUTEUR,a0)
		move.w	#4,(SFADEONSPEED,a0)
		move.b	#0,(SHIRES,a0)
		bsr	init_screen
		bne.s	.exit
		lea	(SCOLOR_TABLE,a0),a2
		move.l	(pageiff-t,a5),a4
		move.l	(SPECRAN1,a0),a3
		bsr	decompacte_ilbm
		bne.s	.exit
		move.w	#257,(flag_fade_on-t,a5)
.nul		tst.w	(flag_fade_on-t,a5)
		bne.s	.nul
.exit		rts



************************ TRACE UNE LIGNE AU 68020 *************************
*/ Entrée	=> d0.w	x1
*/		=> d1.w	y1
*/		=> d2.w	x2
*/		=> d3.w	y2
line68020:
		move.l	(ymulu-t,a5),a4
		move.l	(xbyte-t,a5),a3
		move.l	(xplot-t,a5),a2

		moveq	#1,d4			s1=1
		sub.w	d0,d2			d2=x2-x1=dx
		bpl.s	.dxpos
		neg.w	d2
		moveq	#-1,d4			s1=-1
.dxpos		moveq	#1,d5			s2=1
		sub.w	d1,d3			d3=y2-y1=dy
		bpl.s	.dypos
		neg.w	d3
		moveq	#-1,d5			s2=-1
.dypos		cmp.w	d3,d2			dx > dy ?
		bhi.s	.inter0
*-------------------------------------------------------------------------*
		exg.l	d3,d2
		move.w	d2,d6			d6=dx
		subq.w	#1,d6
		move.w	d3,d7
		add.w	d3,d7
		sub.w	d2,d7			e=dy+dy-dx
.loop1		bsr	plotxy
		tst.w	d7			e<0 ?
		bmi.s	.wend1			-> oui
.while1		add.w	d4,d0			x=x+s1
		sub.w	d2,d7
		sub.w	d2,d7			e=e-2*dx
		bpl.s	.while1
.wend1		add.w	d5,d1			y=y+s2
		add.w	d3,d7
		add.w	d3,d7			e=e+2*dy
		dbf	d6,.loop1
		rts
*-------------------------------------------------------------------------*
.inter0		move.w	d2,d6			d6=dx
		subq.w	#1,d6
		move.w	d3,d7
		add.w	d3,d7
		sub.w	d2,d7			e=dy+dy-dx
.loop0		bsr	plotxy
		tst.w	d7			e<0 ?
		bmi.s	.wend0			-> oui
.while0		add.w	d5,d1			y=y+s2
		sub.w	d2,d7
		sub.w	d2,d7			e=e-2*dx
		bpl.s	.while0
.wend0		add.w	d4,d0			x=x+s1
		add.w	d3,d7
		add.w	d3,d7			e=e+2*dy
		dbf	d6,.loop0
		rts
			





* Entrée	d0.w	x
*		d1.w	y
plotxy:
		move.l	d6,-(a7)
		move.l	(a4,d1*4),a1
		add.l	(a3,d0*4),a1
		add.l	a6,a1
		move.l	(a2,d0*4),d6
		or.l	d6,(a1)
		or.l	d6,(40,a1)
		or.l	d6,(80,a1)
		or.l	d6,(120,a1)
		or.l	d6,(160,a1)
		or.l	d6,(200,a1)
		or.l	d6,(240,a1)
		or.l	d6,(280,a1)
		move.l	(a7)+,d6
		rts

		CNOP	0,4

***************************************************************************
***************************************************************************
***************************************************************************
draw:		move.l	a6,-(a7)
		move.l	(pt_screen-t,a5),a0
		move.l	(SPECRAN1,a0),a6
		move.w	(SPHAUTEUR,a0),d4
		sub.w	#128,d4
		move.l	#160,d0
		move.l	#128,d1
		moveq	#0,d2
		moveq	#0,d3
.loop		movem.l	d0-d4,-(a7)
		*bsr	trace
		bsr	line68020
		movem.l	(a7)+,d0-d4
		add	#16,d3			Y2=Y2+16 
		cmp	d3,d4
		bhi.s	.loop
		move.l	(a7)+,a6
		rts
***************************************************************************
draw_blitter:	move.l	(pt_screen-t,a5),a0
		move.w	(SPHAUTEUR,a0),d4
		sub.w	#128,d4
		move.l	#160,d0
		move.l	#128,d1
		moveq	#0,d2
		moveq	#0,d3
.loop		movem.l	d0-d4,-(a7)
		bsr	trace
		movem.l	(a7)+,d0-d4
		add	#16,d3			Y2=Y2+16 
		cmp	d3,d4
		bhi.s	.loop
		rts
***************************************************************************
***************************************************************************
***************************************************************************


trace:


*/ Entrée	d0.w	x1	/*
*/		d1.w	y1	/*
*/		d2.w	x2	/*
*/		d3.w	y2	/*

*/ initialise le Blitter pour le tracé de droites /*
.wait_blitter	btst	#14,(DMACONR,a6)
		bne.s	.wait_blitter
		move.w	#$8000,(BLTADAT,a6)
		move.w	#$FFFF,(BLTBDAT,a6)
		move.l	#$FFFFFFFF,(BLTAFWM,a6)
		move.w	(SP_SLINE,a0),d6
		move.w	d6,(BLTCMOD,a6)
		move.w	d6,(BLTDMOD,a6)
		bsr	.cherche_octant

		lea	(table,pc),a4
*/ BLTAMOD = (4*(dy-dx)) ; /*
		move.w	d3,d7			d7=dy
		sub.w	d2,d7			d7=(dy-dx)
		add.w	d7,d7
		add.w	d7,d7			d7=4*(dy-dx)
		move.w	d7,(BLTAMOD,a6)
		move.w	d7,(a4)+

*/ BLTBMOD = (4*dy) ; /*
		move	d3,d7			d7=dy
		add	d7,d7
		add	d7,d7			d7=4*dy
		move	d7,(BLTBMOD,a6)
		move	d7,(a4)+

*/ BLTAPTL = (4*dy-2*dx) ; if (4*dy-2*dx<0) { SIGN=1 ; } /* 
		move	d3,d7			d7=dy
		add	d7,d7
		add	d7,d7			d7=4*dy
		sub	d2,d7
		sub	d2,d7			d7=4*dy-2*dx
		bpl.s	.no_signflag
		or	#$40,d5			SIGN=1 (4*dy-2*dx<0)
.no_signflag	move	d7,(BLTAPTL,a6)
		move	d7,(a4)+


*/ BLTCPTH = BLTDPTH = adr. écran */
		move	d1,d7			d7=y1
		mulu	(SP_SLINE,a0),d7
		moveq	#0,d6
		move	d0,d6			d6=x1
		lsr	#3,d6
		and	#$fffe,d6
		add.l	d6,d7
		move.l	d7,d4
		add.l	(SPECRAN1,a0),d7
		add.l	(SPECRAN2,a0),d4
		*move.l	d4,(BLTBPTH,a6)
		move.l	d7,(BLTCPTH,a6)
		move.l	d7,(BLTDPTH,a6)

		and	#$f,d0			d0=x1 modulo 15
		ror	#4,d0
		or	d0,d5
		or	#$0faa,d0
		or.w	#$0BFA,d0


		*or	(mimi-t,a5),d0

		move	d0,(BLTCON0,a6)
		move	d5,(BLTCON1,a6)
		move	d0,(a4)+
		move	d5,(a4)+


		addq	#1,d2			dx=dx+1
		lsl	#6,d2
		addq	#2,d2
		move	d2,(BLTSIZE,a6)
	
		*rts


		move.w	(SPPLAN,a0),d6
		subq.w	#2,d6
		moveq	#3,d6

.xloop		*add.w	(SP_SPLAN,a0),d7
		add.l	#40,d7
		add.l	#40,d4
.waitb		btst	#14,(DMACONR,a6)
		bne.s	.waitb
		lea	(table,pc),a4
		move	(a4)+,d0	(BLTAMOD,a6)
		move	(a4)+,d0	(BLTBMOD,a6)
		move	(a4)+,(BLTAPTL,a6)
		move	(a4)+,(BLTCON0,a6)
		move	(a4)+,(BLTCON1,a6)
		*move.l	d4,(BLTBPTH,a6)
		move.l	d7,(BLTCPTH,a6)
		move.l	d7,(BLTDPTH,a6)
		move	d2,(BLTSIZE,a6)
		dbf	d6,.xloop


		rts


************************* RECHERCHE DE L'OCTANT ***************************
* Entrée	d0.w	X1
*		d1.w	Y1
*		d2.w	X2
*		d3.w	Y2
;;OCTANT8		=24
;;OCTANT7		=4
;;OCTANT6		=12
;;OCTANT5		=28
;;OCTANT4		=20
;;OCTANT3		=8
;;OCTANT2		=0
;;OCTANT1		=16
;;LINEMODE	=1
.cherche_octant
		sub.w	d0,d2			d2=X2-X1=DX
		bmi.s	.xneg			-> X1>X2
		sub.w	d1,d3			d3=Y2-Y1=DY
		bmi.s	.yneg			-> Y1>Y2
		cmp	d3,d2			d3 > d2 / DY > DX ?
		bmi.s	.ygtx			-> oui
		moveq	#OCTANT1+LINEMODE,d5
		bra.s	.lineagain
.ygtx		exg	d2,d3
		moveq	#OCTANT2+LINEMODE,d5
		bra.s	.lineagain

		* SI Y1>Y2 DONC DY<0 *
.yneg		neg.w	d3			d3=abs(DY)
		cmp.w	d3,d2			DY > DX ?
		bmi.s	.ynygtx			-> oui
		moveq	#OCTANT8+LINEMODE,d5
		bra.s	.lineagain
.ynygtx		exg	d2,d3
		moveq	#OCTANT7+LINEMODE,d5
		bra.s	.lineagain

		* SI X1>X2 DONC DX<0 *
.xneg		neg.w	d2			d2=abs(DX)
		sub.w	d1,d3			d3=Y2-Y1=DY
		bmi.s	.xyneg			-> Y1>Y2
		cmp.w	d3,d2
		bmi.s	.xnygtx
		moveq	#OCTANT4+LINEMODE,d5
		bra.s	.lineagain
.xnygtx		exg	d2,d3
		moveq	#OCTANT3+LINEMODE,d5
		bra.s	.lineagain

.xyneg		neg	d3
		cmp	d3,d2
		bmi.s	.xynygtx
		moveq	#OCTANT5+LINEMODE,d5
		bra.s	.lineagain
.xynygtx	exg	d3,d2
		moveq	#OCTANT6+LINEMODE,d5
.lineagain	rts


************************** CREE UNE TABLE YMULU ***************************
init_ymulu:
		* réserve la table 'ymulu' *
		tst.l	(ymulu-t,a5)		déjà réservée ?
		bne.s	.no_ymulu		-> oui
		moveq	#0,d0
		move.l	(pt_screen-t,a5),a0
		move.w	(SPHAUTEUR,a0),d0	d0=largeur écran en pixels
		lsl.w	#2,d0			4 octets par ligne
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(ymulu-t,a5)
		beq.s	.out_memory
.no_ymulu	
		*/ CREE LA TABLE /*
		move.l	(pt_screen-t,a5),a0
		move.l	(ymulu-t,a5),a1
		move.w	(SPHAUTEUR,a0),d0	d0=nbre de lignes de la page
		subq.w	#1,d0
		moveq	#0,d1
		move.w	(SP_SLINE,a0),d1	d1=taille ligne en octets
		moveq	#0,d2			d0=pt/première ligne
.loop		move.l	d2,(a1)+		sauve pt
		add.l	d1,d2			adr. prochaine ligne
		dbf	d0,.loop
		moveq	#0,d7
		rts
.out_memory	moveq	#-12,d7			¤Dépassement mémoire
		rts

********************* CREE UNE TABLE POSITION X OCTET *********************
init_xbyte:
		tst.l	(xbyte-t,a5)	déjà réservée ?
		bne.s	.no			-> oui
		moveq	#0,d0
		move.l	(pt_screen-t,a5),a0
		move.w	(SPLARGE,a0),d0		d0=largeur écran en pixels
		lsl.w	#2,d0			4 octets par pixel
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(xbyte-t,a5)
		beq.s	.out_memory
.no
		*/ CALCUL LA TABLE /*
		move.l	(pt_screen-t,a5),a0
		move.l	(xbyte-t,a5),a1
		move.w	(SPLARGE,a0),d0
		subq.w	#1,d0
		moveq	#0,d1			d1=pos. x bob en pixels
		move.w	#$FFFC,d4
		moveq	#0,d2
.loop		move.w	d1,d2
		lsr.w	#3,d2			convertit pixels -> octets
		and.w	d4,d2			tjrs un multiple de 4 (&$FFFC)
		move.l	d2,(a1)+		sauve pos. x en octet
		addq.w	#1,d1			x=x+1
		dbf	d0,.loop
		moveq	#0,d7
		rts
.out_memory	moveq	#-12,d7			¤Dépassement mémoire
		rts


********************* CREE UNE TABLE POINT *********************
init_xplot:
		tst.l	(xplot-t,a5)	déjà réservée ?
		bne.s	.no			-> oui
		moveq	#0,d0
		move.l	(pt_screen-t,a5),a0
		move.w	(SPLARGE,a0),d0		d0=largeur écran en pixels
		lsl.w	#2,d0			4 octets par pixel
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(xplot-t,a5)
		beq.s	.out_memory
.no
		*/ CALCUL LA TABLE /*
		move.l	(pt_screen-t,a5),a0
		move.l	(xplot-t,a5),a1
		move.w	(SPLARGE,a0),d0
		subq.w	#1,d0
		move.l	#$80000000,d1
.loop		move.l	d1,(a1)+
		ror.l	#1,d1
		dbf	d0,.loop
		moveq	#0,d7
		rts
.out_memory	moveq	#-12,d7			¤Dépassement mémoire
		rts
*plot32:	dc.l	$80000000,$40000000,$20000000,$10000000
		dc.l	$08000000,$04000000,$02000000,$01000000
		dc.l	$00800000,$00400000,$00200000,$00100000
		dc.l	$00080000,$00040000,$00020000,$00010000
		dc.l	$00008000,$00004000,$00002000,$00001000
		dc.l	$00000800,$00000400,$00000200,$00000100
		dc.l	$00000080,$00000040,$00000020,$00000010
		dc.l	$00000008,$00000004,$00000002,$00000001



*>>>>>>>>>>>>>>>>>* Adresses utilisées par le programme *<<<<<<<<<<<<<<<<<*
		CNOP	0,4
ymulu		dc.l	0
xbyte		dc.l	0
xplot		dc.l	0

pageiff		dc.l	0
table		dcb	20,0
mimi		dc	0
		INCLUDE	"Mentat/Include/Systeme/t.s"
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
		dc.b	"$VER:CRAZY MAN (28.01.94)",0

*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
