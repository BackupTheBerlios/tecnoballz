
*/**********************************************/*
*/ CYCLE TEST */
*/**********************************************/*
		OPT	p=68020		assemble les instructions MC68020
SLINE		=40
NPLAN		=8
SPLAN		=SLINE*NPLAN
NLINE		=256
ECRAN_SIZE	=SPLAN*NLINE
HSPRITE		=52
SPRITE_SIZE	=16*(HSPRITE+2)*2
NREGISTRE	=256
CUSTOM		=$DFF000
HTOTAL		=$1C0	W
HSSTOP		=$1C2	W
HBSTRT		=$1C4	W
HBSTOP		=$1C6	W
VTOTAL		=$1C8
VSSTOP		=$1CA	W
VBSTRT		=$1CC
VBSTOP		=$1CE
SPRHSTRT	=$1D0
SPRHSTOP	=$1D2
BPLHSTRT	=$1D4
BPLHSTOP	=$1D6
HHPOSW		=$1D8
HHPOSR		=$1DA
BEAMCON0	=$1DC	W	ECS	$0200 = 50Hz / $0000 = 60Hz
HSSTRT		=$1DE	W
VSSTRT		=$1E0	W
HCENTER		=$1E2	W
DIWHIGH		=$1E4	W		#$2101 dans liste Copper ??
COLOR00		=$180
BPLCON3		=$106
		SECTION ETB,code

debut:		bsr	save
		bsr	convert_aga_color
		bsr	set_colors
		*move.w	#$8360,($0096,a6)
		move.w	#$000F,($01FC,a6)
		move.w	#$0211,($0100,a6)
		move.w	#$0000,($0102,a6)
		move.w	#$0024,($0104,a6)
		move.w	(sbplcon3,pc),(BPLCON3,a6)
		move.w	#(40*7),d0
		move.w	d0,($0108,a6)
		move.w	d0,($010A,a6)
		move.w	#$2881,($008E,a6)
		move.w	#$28c1,($0090,a6)
		move.w	#$0038,($0092,a6)
		move.w	#$00b8,($0094,a6)
		* ECRAN 320 : (start=$38,stop=$9A à $B8, )

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

		*/ Raffraichit les pointeurs lutins /*
		lea	(sprite0,pc),a0
		lea	($120,a6),a1
		moveq	#8-1,d0
.loop_sprite	move.l	(a0)+,(a1)+
		dbf	d0,.loop_sprite

		*/ Raffraichit les pointeurs écrans /*
		moveq	#NPLAN-1,d1
		move.l	(ecran2,pc),d0
		lea	($00E0,a6),a0
.loop_bitplane	move.l	d0,(a0)+
		add.l	#SLINE,d0
		dbf	d1,.loop_bitplane

		lea	(6,a6),a0
		moveq	#$28,d0
.ss		cmp.b	(a0),d0
		bhi.s	.ss
		




		*----------------*
		not.w	titi
		beq.s	.fdd	
		clr.w	(106,a6)
		move.w	#$008,($180,a6)
		lea	zone,a0
		move.l	#f-zone,d0
		bsr	clear.memory
		bra.s	.s
		*----------------*
.fdd		clr.w	(106,a6)
		move.w	#$800,($180,a6)
		lea	zone,a0
		move.l	#f-zone,d0
		bsr	clear.mem
.s		clr.w	(106,a6)
		move.w	#$000,($180,a6)

		*----------------*
		btst	#6,$bfe001
		bne	loop
		move.w	#$0000,($0100,a6)
xloop		btst	#6,$bfe001
		beq.s	xloop

		move	#$0020,(BEAMCON0,a6)
		bsr	restore
		moveq	#0,d0
		rts

*-------------------------------------------------------------------------*
save:		move.l	4.w,a6
		jsr	(-132,a6)
		lea	(gfxname,pc),a1
		moveq	#0,d0
		jsr	(-552,a6)
		lea	(gfxbase,pc),a0
		move.l	d0,(a0)
		lea	(ecran,pc),a0
		move.l	#fichier,(a0)
		lea	(ecran2,pc),a0
		move.l	#fichier2,(a0)
		bsr	spriteadr
		lea	$dff000,a6
		lea	(save_intena,pc),a0
		move.w	($001C,a6),(a0)
		or.w	#$C000,(a0)+
		move.w	$0002(a6),(a0)
		or.w	#$8000,(a0)
		move.w	#$7FFF,($009A,a6)
		move.w	#$7FFF,($009C,a6)
		move.w	#$07FF,($0096,a6)
		rts

*-------------------------------------------------------------------------*
restore:
		move	#$7FFF,($009A,a6)
		move	#$7FFF,($009C,a6)
		move	#$07FF,($0096,a6)
		move.l	(gfxbase,pc),a1
		move.l	(38,a1),$0080(a6)
		move.w	d0,($0088,a6)
		move.w	(save_dmacon,pc),$0096(a6)
		move.w	(save_intena,pc),$009A(a6)
		move.l	4.w,a6
		lea	(gfxbase,pc),a0
		lea	(copper,pc),a1
		move.l	(50,a1),(a1)
li:		move.l	(a0),a1
		clr.l	(a0)
		jsr	-414(a6)
		jsr	-138(a6)
		moveq	#0,d0
		rts

* CALCUL ADRESSE DES LISTES LUTINS *
spriteadr:	move.l	(ecran,pc),d0
		add.l	#ECRAN_SIZE,d0	
		lea	(sprite0,pc),a0
		moveq	#8-1,d1
.loop		move.l	d0,d2
		add.l	#16,d2
		and.l	#$FFFFFFF0,d2
		move.l	d2,(a0)+
		add.l	#SPRITE_SIZE,d0
		dbf	d1,.loop
		rts	

*-------------------------------------------------------------------------*
**** Convertit les couleurs 32bits de la structure écran en format AGA ****
*-------------------------------------------------------------------------*
convert_aga_color:
		lea	colormap,a4
		move.w	#NREGISTRE-1,d3		d3=nbre de regitres - 1
.loop		move.l	(a4),d2			d2=couleur 32bits RVB
		bsr.s	aga_color		|-> convertit en AGA
		move.w	d0,(a4)+		bits 4 à 7 RVB
		move.w	d1,(a4)+		bits 0 à 3 RVB
		dbf	d3,.loop
		rts

****************** Convertit les couleurs au format AGA *******************
* entrée	=> d2.l	couleur 32 bits {X.X.R1.R2.V1.V2.B1.B2}
* 			X inutilisé / R1.R2 rouge / V1.V2 vert / B1.B2 bleu 
* sortie	<= d0.w	R1.V1.B1 (valeur des bits 4 à 7 comme l'original-chip)
*		<= d1.w	R2.V2.B2 (valeur des bits 0 à 3)
* utilise	-- d0,d1,d2
aga_color:
		move.l	d2,d1
		and.l	#$00f0f0f0,d1	garde les bits de 4 à 7
		move.w	d1,d0
		lsr.w	#4,d0
		and.w	#$F,d0		garde la composante bleue
		lsr.w	#8,d1
		or.w	d1,d0		ajoute composante verte
		swap	d1
		lsl.w	#4,d1
		or.w	d1,d0		ajoute composante rouge
		and.l	#$000F0F0F,d2
		move.w	d2,d1
		and.w	#$F,d1		garde la composante bleue
		lsr.w	#4,d2
		or.w	d2,d1		ajoute composante verte
		swap	d2
		lsl.w	#8,d2
		or	d2,d1		ajoute composante rouge
		rts

*-------------------------------------------------------------------------*
*===========* Entre une même valeur au 256 registres couleurs *===========*
*-------------------------------------------------------------------------*
* Entrée	=> a6.l CUSTOM

****************** Ecrit les couleurs dans les regitres *******************
* Entrée	=> a0.l adr. structure écran
*		=> a6.l	CUSTOM
* Utilise	-- (d0-d7 / a0-a3
set_colors:
		moveq	#0,d2			sélect. 1er banc des 8 bancs de couleurs
		move.w	#NREGISTRE-1,d1		d1=nombre de couleurs - 1
		lea	colormap,a1		a1=pointeur sur bits 4 à 7 RVB
		lea	colormap+2,a2		a2=pointeur sur bits 0 à 3 RVB
		lea	(BPLCON3,a6),a3
		move.l	#CUSTOM+COLOR00,d0
		move.w	(sbplcon3,pc),d6	sélection bits 4 à 7 RVB
		move	d6,d7
		or.w	#$0200,d7		sélection bits 0 à 3 RVB

		* boucle des bits de 4 à 7 *
.loop_color	moveq	#32-1,d3		d3=32 regitres couleurs-1
		move.l	d0,a0			a0=premier registre couleur
		move.w	d2,d5
		or.w	d6,d5			bits 4 à 7 RVB
		move.w	d5,(a3)			init. BPLCON3 (change de banc)
		move.w	d1,d4
.bits4a7	move.w	(a1),(a0)+		écrit dans le reg. couleur
		addq.l	#4,a1
		subq.w	#1,d4
		bmi.s	.cont
		dbf	d3,.bits4a7

		* boucle des bits de 0 à 3 *
.cont		moveq	#32-1,d3		32 regitres couleurs
		move.l	d0,a0			a0=premier registre couleur
		move.w	d2,d5
		or.w	d7,d5			bits 0 à 3 RVB
		move.w	d5,(a3)			init. BPLCON3 (change de banc)
.bits0a3	move.w	(a2),(a0)+		écrit dans le reg. couleur
		addq.l	#4,a2
		subq.w	#1,d1
		bmi.s	.end
		dbf	d3,.bits0a3
		add.w	#$2000,d2		prochain banc de couleurs
		bra.s	.loop_color
.end		rts

		CNOP	0,4
copper:		dc.l	0
sprite0		dc.l	0
sprite1		dc.l	0
sprite2		dc.l	0
sprite3		dc.l	0
sprite4		dc.l	0
sprite5		dc.l	0
sprite6		dc.l	0
sprite7		dc.l	0
sbplcon3	dc.w	$0004
save_intena	dc.w	0
save_dmacon	dc.w	0
titi		dc.w	0
gfxbase		dc.l	0
ecran		dc.l	0
ecran2		dc.l	0
gfxname		dc.b	'graphics.library',0
		EVEN

*--===---===---===---===---===---===---===---===---===---===---===---===--*
*-=-=-=-=-=-=-=-=-=-=-=-=-> Efface zone mémoire <-=-=-=-=-=-=-=-=-=-=-=-=-*
*--===---===---===---===---===---===---===---===---===---===---===---===--*
* Entrée	=> d0.l	taille de la zone mémoire en octets
*		=> a0.l	pointeur sur la zone mémoire
clear.memory:
		tst.l	d0
		beq	.exit
		movem.l	d0-d7/a0-a6,-(a7)
		add.l	d0,a0			a0=pt/fin de la zone mémoire
		lea	(.pile,pc),a1
		move.l	a7,(a1)
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
		divul.l	#56,d1:d0
		subq.w	#1,d1
		beq.s	.noloop1
.loop1		move.b	d2,-(a0)
		dbf	d1,.loop1
.noloop1	move.w	d0,d1
		swap	d0
		move.w	d1,d0	
		moveq	#0,d1
		and.w	#$1F,d0
		beq.s	.noloop2
		subq.w	#1,d0
.loop2		movem.l	d1-d7/a1-a7,-(a0)	Efface 56 octets
		dbf	d0,.loop2
.noloop2	swap	d0
		lsr.w	#5,d0
		subq.w	#1,d0
		bmi	.noloop3
.loop3
		REPT	32
		movem.l	d1-d7/a1-a7,-(a0)	Efface 56*16=896 octets
		ENDR
		dbf	d0,.loop3
.noloop3
		move.l	(.pile,pc),a7
		movem.l	(a7)+,d0-d7/a0-a6
.exit		rts
		CNOP	0,4
.pile		dc.l	0

clear.mem:	
		movem.l	d0/d1/d2/a0,-(a7)
		moveq	#0,d2
		move.l	d0,d1
		lsr.l	#4,d0
		subq.l	#1,d0
		bmi.s	.noloop1
.loop1		move.l	d2,(a0)+
		move.l	d2,(a0)+
		move.l	d2,(a0)+
		move.l	d2,(a0)+
		dbf	d0,.loop1
.noloop1	and.w	#$F,d1
		beq.s	.noloop2
		subq.w	#1,d1
.loop2		move.b	d2,(a0)+
		dbf	d1,.loop2
.noloop2	movem.l	(a7)+,d0/d1/d2/a0
		rts

		CNOP	0,4
mem		ds.l	1024


*=========================================================================*
**************************** Section graphique ****************************
*=========================================================================*
		SECTION		DATACHIP,DATA_C
		CNOP 0,8		* on aligne sur 64 bits
colormap:	incbin	"DH1:Data/ILBM/Page.RAW"
fichier2		=colormap+256*4
		ds.l	SPRITE_SIZE*8/4

		CNOP 0,8
fichier:	dcb.l	10*256*8,0


		CNOP	0,4
zone		dcb.b	2048,-1
f



