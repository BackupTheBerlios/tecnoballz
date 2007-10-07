*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<*
* Création des bonus Directo & Electo d 'Ebonstar94
*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<>*<*
		OPT	p=68020		assemble les instructions MC68020
		SECTION ETB,code
*--<>--*--<>--*--<>--*--<>--*--<>--*--<>--*--<>--*--<>--*--<>--*--<>--*--<*
DMACONR		=$0002
SPLAN		=40
NPLAN		=1
SLINE		=SPLAN*NPLAN
NLINE		=256
ECRAN_SIZE	=SPLAN*NLINE
MEMSIZE		=ECRAN_SIZE*2*2
CUSTOM		=$DFF000
COLOR00		=$180
BPLCON3		=$106
BLTCON0		=$040
BLTCON1		=$042
BLTAFWM		=$044
BLTALWM		=$046
BLTCPTH		=$048
BLTCPTL		=$04A
BLTBPTH		=$04C
BLTBPTL		=$04E
BLTAPTH		=$050
BLTAPTL		=$052
BLTDPTH		=$054
BLTDPTL		=$056
BLTSIZE		=$058
BLTCON0L	=$05B
BLTSIZV		=$05C
BLTSIZH		=$05E
BLTCMOD		=$060
BLTBMOD		=$062
BLTAMOD		=$064
BLTDMOD		=$066
BLTCDAT		=$070
BLTBDAT		=$072
BLTADAT		=$074
OCTANT8		=24
OCTANT7		=4
OCTANT6		=12
OCTANT5		=28
OCTANT4		=20
OCTANT3		=8
OCTANT2		=0
OCTANT1		=16
LINEMODE	=1
*--<>--*--<>--*--<>--*--<>--*--<>--*--<>--*--<>--*--<>--*--<>--*--<>--*--<*
debut:		
		movem.l	d0-d7/a0-a6,-(a7)
		bsr	save
		bsr	init
		bsr	loop
		bsr	restore
		movem.l	(a7)+,d0-d7/a0-a6
		rts

*-------------------------------------------------------------------------*
************* Boucle synchronisée tous les 50iemes de seconde *************
*-------------------------------------------------------------------------*
loop:
		*/ Attend le tempo mort vertical /*
		move.w	#$0020,d1
		move.w	d1,($009C,a6)
		lea	($001E,a6),a0
.waitvbl	move.w	(a0),d0
		and.w	d1,d0
		beq.s	.waitvbl
		move.w	d1,($009C,a6)

		*/ Raffraichit les pointeurs écrans /*
		move.l	(ecran1,pc),d0
		move.l	(ecran2,pc),d1
		lea	(flip,pc),a0
		not.w	(a0)
		beq.s	.ecran1		
		exg	d0,d1
.ecran1		lea	(ecran,pc),a0
		move.l	d1,(a0)
		moveq	#NPLAN-1,d1
		lea	($00E0,a6),a0
.loop_bitplane	move.l	d0,(a0)+
		add.l	#SLINE,d0
		dbf	d1,.loop_bitplane

		*/ Trace */
		bsr	clear_screen
		bsr	xx.move
		bsr	init_blitter_draw
		bsr	xx.trace

		*/ Test pression bouton gauche de la souris */
		btst	#6,$bfe001
		bne	loop
		rts

*<<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><*
*------------------------> Gestion du mouvement <-------------------------*
*<<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><*
xx.num		=12

POTGOR		=$016
xx.move:
		move.w	(POTGOR,a6),d0
		btst	#10,d0			Test bit DATLY
		beq	.exit

		lea	(p1,pc),a4
		moveq	#xx.num-1,d4
.loop		addq.w	#4,(a4)
		and.w	#1023,(a4)+
		dbf	d4,.loop
.exit		rts

*<<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><*
*--------------------------- Trace les droites ---------------------------*
*<<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><*
xx		=50
xx.trace:
		lea	(p1,pc),a4
		moveq	#xx.num-1,d4
.loop		move.w	(a4)+,d5
		and.w	#1023,d5
		move.l	(sinus,pc,d5),d0
		muls	#xx,d0
		asr.l	#8,d0
		asr.l	#2,d0
		add.l	#160,d0
		move.l	(cosinus,pc,d5),d1
		muls	#xx,d1
		asr.l	#8,d1
		asr.l	#2,d1
		add.l	#128,d1
		add.w	#512,d5
		and.w	#1023,d5
		move.l	(sinus,pc,d5),d2
		muls	#xx,d2
		asr.l	#8,d2
		asr.l	#2,d2
		add.l	#160,d2
		move.l	(cosinus,pc,d5),d3
		muls	#xx,d3
		asr.l	#8,d3
		asr.l	#2,d3
		add.l	#128,d3
		bsr	blitter.line
		dbf	d4,.loop
		rts

*-------------------------------------------------------------------------*
*««««««««««««««««««««««««««« Quitte le système »»»»»»»»»»»»»»»»»»»»»»»»»»»*
*-------------------------------------------------------------------------*
save:		
		move.l	4.w,a6
		jsr	(-132,a6)
		lea	(gfxname,pc),a1
		moveq	#0,d0
		jsr	(-552,a6)
		lea	(gfxbase,pc),a0
		move.l	d0,(a0)
		move.l	#MEMSIZE,d0
		move.l	#$10002,d1
		jsr	(-198,a6)
		lea	(ecran,pc),a0
		move.l	d0,(a0)
		lea	$dff000,a6
		lea	(save_intena,pc),a0
		move.w	($001C,a6),(a0)
		or.w	#$C000,(a0)+
		move.w	($0002,a6),(a0)
		or.w	#$8000,(a0)
		move.w	#$7FFF,($009A,a6)
		move.w	#$7FFF,($009C,a6)
		move.w	#$07FF,($0096,a6)
		rts

*-------------------------------------------------------------------------*
*«««««««««««««««««««««««««« Retourne au système »»»»»»»»»»»»»»»»»»»»»»»»»»*
*-------------------------------------------------------------------------*
restore:
		move.w	#$7FFF,($009A,a6)
		move.w	#$7FFF,($009C,a6)
		move.w	#$07FF,($0096,a6)
		move.l	(gfxbase,pc),a1
		move.l	(38,a1),($0080,a6)
		move.w	d0,($0088,a6)
		move.w	(save_dmacon,pc),($0096,a6)
		move.w	(save_intena,pc),($009A,a6)
		move.l	4.w,a6
li:		jsr	(-414,a6)
		jsr	(-138,a6)
		move.l	(ecran1,pc),a1
		move.l	#MEMSIZE,d0
		jsr	(-210,a6)
		moveq	#0,d0
		rts

*-------------------------------------------------------------------------*
*«««««««««««««««««««««««««« Initialise l'écran »»»»»»»»»»»»»»»»»»»»»»»»»»»*
*-------------------------------------------------------------------------*
init:
		move.w	#$8340,($0096,a6)
		move.w	#$000F,($01FC,a6)
		move.w	#$1201,($0100,a6)
		move.w	#$0000,($0102,a6)
		move.w	#$0024,($0104,a6)
		move.w	(sbplcon3,pc),(BPLCON3,a6)
		move.w	#SLINE-SPLAN,d0
		move.w	d0,($0108,a6)
		move.w	d0,($010A,a6)
		move.w	#$2881,($008E,a6)
		move.w	#$28C1,($0090,a6)
		move.w	#$0038,($0092,a6)
		move.w	#$00B8,($0094,a6)
		move.w	#0,(COLOR00,a6)
		move.w	#$A,($0182,a6)
		move.l	(ecran,pc),d0
		lea	(ecran1,pc),a0
		move.l	d0,(a0)
		lea	(ecran2,pc),a0
		add.l	#ECRAN_SIZE,d0
		move.l	d0,(a0)
		rts

*-------------------------------------------------------------------------*
*««««««««««««««««««««««««««   TRACE UNE LIGNE  »»»»»»»»»»»»»»»»»»»»»»»»»»»*
*-------------------------------------------------------------------------*
* Entrée	=> d0.w	X1
*		=> d1.w	Y1
*		=> d2.w	X2
*		=> d3.w	Y2
blitter.line:
line16:		cmp.w	d3,d1
		bhi.s	.sup
		exg	d2,d0
		exg	d3,d1
.sup		moveq	#$F,d6
		and.w	d0,d6
		sub.w	d0,d2
		bmi.s	.xneg
		sub.w	d1,d3
		bmi.s	.yneg
		cmp	d3,d2
		bmi.s	.ygtx
		moveq	#OCTANT1+LINEMODE,d5
		bra.s	.lineagain
.ygtx		exg	d2,d3
		moveq	#OCTANT2+LINEMODE,d5
		bra.s	.lineagain
.yneg		neg.w	d3
		cmp.w	d3,d2
		bmi.s	.ynygtx
		moveq	#OCTANT8+LINEMODE,d5
		bra.s	.lineagain
.ynygtx		exg	d2,d3
		moveq	#OCTANT7+LINEMODE,d5
		bra.s	.lineagain
.xneg		neg.w	d2
		sub.w	d1,d3
		bmi.s	.xyneg
		cmp.w	d3,d2
		bmi.s	.xnygtx
		moveq	#OCTANT4+LINEMODE,d5
		bra.s	.lineagain
.xnygtx		exg	d2,d3
		moveq	#OCTANT3+LINEMODE,d5
		bra.s	.lineagain
.xyneg		neg.w	d3
		cmp.w	d3,d2
		bmi.s	.xynygtx
		moveq	#OCTANT5+LINEMODE,d5
		bra.s	.lineagain
.xynygtx	exg	d3,d2
		moveq	#OCTANT6+LINEMODE,d5
.lineagain	move.w	d1,d7
		mulu	#SLINE,d7
		moveq	#0,d1
		move.w	d0,d1
		lsr.w	#3,d1
		add.l	d1,d7
		add.l	(ecran,pc),d7
		add.w	d3,d3
		add.w	d3,d3
		move.w	d3,d1
		sub.w	d2,d1
		sub.w	d2,d1
		bpl.s	.no_signflag
		or.w	#$40,d5
.no_signflag	swap	d3
		move.w	d1,d3
		sub.w	d2,d3
		sub.w	d2,d3
		moveq	#0,d0
		move.w	d6,d0
		swap	d0
		asr.l	#4,d0
		or.w	#$0B4A,d0
		swap	d0
		move.w	d5,d0
		asl.w	#6,d2
		add.w	#$42,d2
.waitzeblit	btst	#14,(DMACONR,a6)
		bne.s	.waitzeblit
		move.l	d3,(BLTBMOD,a6)
		move.w	d1,(BLTAPTL,a6)
		move.l	d7,(BLTCPTH,a6)
		move.l	d7,(BLTDPTH,a6)
		move.l	d0,(BLTCON0,a6)
		move.w	d2,(BLTSIZE,a6)
		rts

**************** INITIALISE LE BLITTER EN TRACE DE DROITE *****************
* entrée	=> a6.l	CUSTOM
init_blitter_draw:
.wait_blitter	btst	#14,(DMACONR,a6)
		bne.s	.wait_blitter
		move.w	#$8000,(BLTADAT,a6)
		move.w	#$FFFF,(BLTBDAT,a6)
		move.l	#$FFFFFFFF,(BLTAFWM,a6)
		move.w	#SLINE,(BLTCMOD,a6)
		move.w	#SLINE,(BLTDMOD,a6)
		rts

*»»»»»»»»»»»»»»»»»»»»» Efface un écran de 320*256*5 ««««««««««««««««««««««*
CLS:		movem.l	d0-d7/a0-a4,-(a7)
		bsr.s	clear_screen
		movem.l	(a7)+,d0-d7/a0-a4
		rts
clear_screen:	movem.l	a5-a6,-(a7)
		lea	(.pile,pc),a0
		move.l	a7,(a0)	
		moveq	#0,d1
		moveq	#0,d2
		moveq	#0,d3
		moveq	#0,d4
		moveq	#0,d5
		moveq	#0,d6
		moveq	#0,d7
		sub.l	a1,a1
		sub.l	a2,a2
		sub.l	a3,a3
		sub.l	a4,a4
		sub.l	a5,a5
		sub.l	a6,a6
		sub.l	a7,a7
		move.l	(ecran,pc),a0
		add.l	#ECRAN_SIZE,a0		10240 octets

		* ON EFFACE  7056 OCTETS *
		moveq	#2-1,d0			2*63*56=7056 octets
.loop_clear
		REPT	63			63 instrutions = 252 octets 
		movem.l	d1-d7/a1-a7,-(a0)
		ENDR
		dbf	d0,.loop_clear		+ 4 octets = taille du cache

		* ON EFFACE 3136 OCTETS *
		REPT	56
		movem.l	d1-d7/a1-a7,-(a0)	32*56=1792 octets
		ENDR		

		* ON EFFACE 48 OCTETS *
		movem.l	d1-d7/a1-a5,-(a0)
		move.l	(.pile,pc),a7
		movem.l	(a7)+,a5-a6
		rts
		CNOP	0,4
.pile		dc.l	0

*-------------------------------------------------------------------------*
*«««««««««««««««««««««««««« Données & pointeurs »»»»»»»»»»»»»»»»»»»»»»»»»»*
*-------------------------------------------------------------------------*
p1		dc.w	0,4,8,12,16,20
		dc.w	0,0,0,0,0,42

p2		dc.w	42
p3		dc.w	42*2
p4		dc.w	42*3
p5		dc.w	42*4
p6		dc.w	42*5
p7		dc.w	42*6
p8		dc.w	42*7
p9		dc.w	42*8
p10		dc.w	42*9
p11		dc.w	42*10
p12		dc.w	42*11

flip		dc.w	0
sbplcon3	dc.w	$0004
toto		dc.w	0
save_intena	dc.w	0
save_dmacon	dc.w	0
		CNOP	0,4
gfxbase		dc.l	0
ecran		dc.l	0
ecran1		dc.l	0
ecran2		dc.l	0
gfxname		dc.b	'graphics.library',0
		EVEN

*»»»»»»»»»»»»»»»»»»» TABLE DE SINUS & COSINUS VALEUR «««««««««««««««««««««*
* INT(SIN(a)*1024)
		CNOP	0,4
sinus:		dc.l	00000,+0025,+0050,+0075,+0100,+0125,+0150,+0175
		dc.l	+0199,+0224,+0248,+0273,+0297,+0321,+0344,+0368
		dc.l	+0391,+0414,+0437,+0460,+0482,+0504,+0526,+0547
		dc.l	+0568,+0589,+0609,+0629,+0649,+0668,+0687,+0706
		dc.l	+0724,+0741,+0758,+0775,+0791,+0807,+0822,+0837
		dc.l	+0851,+0865,+0878,+0890,+0903,+0914,+0925,+0936
		dc.l	+0946,+0955,+0964,+0972,+0979,+0986,+0993,+0999
		dc.l	+1004,+1008,+1012,+1016,+1019,+1021,+1022,+1023
cosinus:	dc.l	+1024,+1023,+1022,+1021,+1019,+1016,+1012,+1008
		dc.l	+1004,+0999,+0993,+0986,+0979,+0972,+0964,+0955
		dc.l	+0946,+0936,+0925,+0914,+0903,+0890,+0878,+0865
		dc.l	+0851,+0837,+0822,+0807,+0791,+0775,+0758,+0741
		dc.l	+0724,+0706,+0687,+0668,+0649,+0629,+0609,+0589
		dc.l	+0568,+0547,+0526,+0504,+0482,+0460,+0437,+0414
		dc.l	+0391,+0368,+0344,+0321,+0297,+0273,+0248,+0224
		dc.l	+0199,+0175,+0150,+0125,+0100,+0075,+0050,+0025
		dc.l	00000,-0026,-0051,-0076,-0101,-0126,-0151,-0176
		dc.l	-0200,-0225,-0249,-0274,-0298,-0322,-0345,-0369
		dc.l	-0392,-0415,-0438,-0461,-0483,-0505,-0527,-0548
		dc.l	-0569,-0590,-0610,-0630,-0650,-0669,-0688,-0707
		dc.l	-0725,-0742,-0759,-0776,-0792,-0808,-0823,-0838
		dc.l	-0852,-0866,-0879,-0891,-0904,-0915,-0926,-0937
		dc.l	-0947,-0956,-0965,-0973,-0980,-0987,-0994,-1000
		dc.l	-1005,-1009,-1013,-1017,-1020,-1022,-1023,-1024
		dc.l	-1025,-1024,-1023,-1022,-1020,-1017,-1013,-1009
		dc.l	-1005,-1000,-0994,-0987,-0980,-0973,-0965,-0956
		dc.l	-0947,-0937,-0926,-0915,-0904,-0891,-0879,-0866
		dc.l	-0852,-0838,-0823,-0808,-0792,-0776,-0759,-0742
		dc.l	-0725,-0707,-0688,-0669,-0650,-0630,-0610,-0590
		dc.l	-0569,-0548,-0527,-0505,-0483,-0461,-0438,-0415
		dc.l	-0392,-0369,-0345,-0322,-0298,-0274,-0249,-0225
		dc.l	-0200,-0176,-0151,-0126,-0101,-0076,-0051,-0026 
sinus.end	dc.l	00000,+0025,+0050,+0075,+0100,+0125,+0150,+0175
		dc.l	+0199,+0224,+0248,+0273,+0297,+0321,+0344,+0368
		dc.l	+0391,+0414,+0437,+0460,+0482,+0504,+0526,+0547
		dc.l	+0568,+0589,+0609,+0629,+0649,+0668,+0687,+0706
		dc.l	+0724,+0741,+0758,+0775,+0791,+0807,+0822,+0837
		dc.l	+0851,+0865,+0878,+0890,+0903,+0914,+0925,+0936
		dc.l	+0946,+0955,+0964,+0972,+0979,+0986,+0993,+0999
		dc.l	+1004,+1008,+1012,+1016,+1019,+1021,+1022,+1023
