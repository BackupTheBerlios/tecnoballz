*/**********************************************/*
*/ TEST ECRAN HARD AGA					/*
*/ Affiche une image 640x256 en 32 couleurs 	/*
*/							/*
*/ Le 29 décembre 1993 					/*
*/							/*
*/**********************************************/*
		OPT	p=68020		assemble les instructions MC68020
		OPT	l-		auto link
SLINE		=80
NPLAN		=5
SPLAN		=SLINE*NPLAN
NLINE		=256
ECRAN_SIZE	=SPLAN*NLINE
HSPRITE		=52
SPRITE_SIZE	=16*(HSPRITE+2)*2
NREGISTRE	=32
CUSTOM		=$DFF000
BEAMCON0	=$1DC	W	ECS	$0200 = 50Hz / $0000 = 60Hz
COLOR00		=$180
BPLCON3		=$106

		SECTION ETB,code
*««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»«*
debut:		bsr	save
		bsr	convert_aga_color
		bsr	set_colors
		move	#$8360,$0096(a6)
		move	#$000F,$01FC(a6)
		move	#$D201,$0100(a6)
		move	#$0000,$0102(a6)
		move	#$0024,$0104(a6)
		move	sbplcon3(pc),BPLCON3(a6)
		move	#(SPLAN-SLINE),d0
		move	d0,$0108(a6)
		move	d0,$010A(a6)
		move	#$2881,$008E(a6)
		move	#$28c1,$0090(a6)
		move	#$0038,$0092(a6)
		move	#$00c0,$0094(a6)
		move	#$00c0,$0094(a6)

*-------------------------------------------------------------------------*
************* Boucle synchronisée tous les 50iemes de seconde *************
*-------------------------------------------------------------------------*
loop:
		*/ Attend le tempo mort vertical /*
		move	#$0020,d1
		move	d1,$009C(a6)
		lea	$001E(a6),a0
.waitvbl	move	(a0),d0
		and	d1,d0
		beq.s	.waitvbl
		move	d1,$009C(a6)

		*/ Raffraichit les pointeurs lutins /*
		lea	sprite,a0
		lea	$120(a6),a1
		moveq	#8-1,d0
.loop_sprite	move.l	a0,(a1)+
		dbf	d0,.loop_sprite

		*/ Raffraichit les pointeurs écrans /*
		moveq	#NPLAN-1,d1
		move.l	ecran(pc),d0
		lea	$00E0(a6),a0
.loop_bitplane	move.l	d0,(a0)+
		add.l	#SLINE,d0
		dbf	d1,.loop_bitplane

		*/ ATTEND UNE PRESSION DU BOUTON SOURIS /*
		btst	#6,$bfe001
		bne	loop
		move	#$0000,$0100(a6)
xloop		btst	#6,$bfe001
		beq.s	xloop
		move	#$0020,BEAMCON0(a6)
		bsr	restore
		moveq	#0,d0
		rts

*-------------------------------------------------------------------------*
save:		move.l	4.w,a6
		jsr	-132(a6)
		lea	gfxname(pc),a1
		moveq	#0,d0
		jsr	-552(a6)
		lea	gfxbase(pc),a0
		move.l	d0,(a0)
		lea	ecran(pc),a0
		move.l	#fichier,(a0)
		lea	$dff000,a6
		lea	save_intena(pc),a0
		move	$001C(a6),(a0)
		or	#$c000,(a0)+
		move	$0002(a6),(a0)
		or	#$8000,(a0)
		move	#$7fff,$009A(a6)
		move	#$7fff,$009C(a6)
		move	#$07ff,$0096(a6)
		rts

*-------------------------------------------------------------------------*
restore:
		move	#$7fff,$009A(a6)
		move	#$7fff,$009C(a6)
		move	#$07ff,$0096(a6)
		move.l	gfxbase(pc),a1
		move.l	38(a1),$0080(a6)
		move	d0,$0088(a6)
		move	save_dmacon(pc),$0096(a6)
		move	save_intena(pc),$009A(a6)
		move.l	4.w,a6
		lea	gfxbase(pc),a0
		lea	copper(pc),a1
		move.l	50(a1),(a1)
li:		move.l	(a0),a1
		clr.l	(a0)
		jsr	-414(a6)
		jsr	-138(a6)
		moveq	#0,d0
		rts

**** Convertit les couleurs 32bits de la structure écran en format AGA ****
* entrée	=> a0.l	adr. stucture écran
convert_aga_color:
		lea	colormap,a4
		move	#NREGISTRE-1,d3		d3=nbre de regitres - 1
.loop		move.l	(a4),d2			d2=couleur 32bits RVB
		bsr.s	aga_color		|-> convertit en AGA
		move	d0,(a4)+		bits 4 à 7 RVB
		move	d1,(a4)+		bits 0 à 3 RVB
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
		move	d1,d0
		lsr	#4,d0
		and	#$000f,d0	garde la composante bleue
		lsr	#8,d1
		or	d1,d0		ajoute composante verte
		swap	d1
		lsl	#4,d1
		or	d1,d0		ajoute composante rouge
		and.l	#$000f0f0f,d2
		move	d2,d1
		and	#$000f,d1	garde la composante bleue
		lsr	#4,d2
		or	d2,d1		ajoute composante verte
		swap	d2
		lsl	#8,d2
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
		move	#NREGISTRE-1,d1		d1=nombre de couleurs - 1
		lea	colormap,a1		a1=pointeur sur bits 4 à 7 RVB
		lea	colormap+2,a2		a2=pointeur sur bits 0 à 3 RVB
		lea	BPLCON3(a6),a3
		move.l	#CUSTOM+COLOR00,d0
		move	sbplcon3(pc),d6		sélection bits 4 à 7 RVB
		move	d6,d7
		or	#$0200,d7		sélection bits 0 à 3 RVB

		* boucle des bits de 4 à 7 *
.loop_color	moveq	#32-1,d3		d3=32 regitres couleurs-1
		move.l	d0,a0			a0=premier registre couleur
		move	d2,d5
		or	d6,d5			bits 4 à 7 RVB
		move	d5,(a3)			init. BPLCON3 (change de banc)
		move	d1,d4
.bits4a7	move	(a1),(a0)+		écrit dans le reg. couleur
		addq	#4,a1
		subq	#1,d4
		bmi.s	.cont
		dbf	d3,.bits4a7

		* boucle des bits de 0 à 3 *
.cont		moveq	#32-1,d3		32 regitres couleurs
		move.l	d0,a0			a0=premier registre couleur
		move	d2,d5
		or	d7,d5			bits 0 à 3 RVB
		move	d5,(a3)			init. BPLCON3 (change de banc)
.bits0a3	move	(a2),(a0)+		écrit dans le reg. couleur
		addq	#4,a2
		subq	#1,d1
		bmi.s	.end
		dbf	d3,.bits0a3
		add	#$2000,d2		prochain banc de couleurs
		bra.s	.loop_color
.end		rts

copper:		dc.l	0
sbplcon3	dc	$0000
save_intena	dc	0
save_dmacon	dc	0
gfxbase		dc.l	0
ecran		dc.l	0
gfxname		dc.b	'graphics.library',0
		EVEN


*=========================================================================*
**************************** Section graphique ****************************
*=========================================================================*
		SECTION		DATACHIP,DATA_C
		CNOP 0,8		* on aligne sur 64 bits
colormap:	incbin	"DH0:Data/ILBM/Desert.RAW"
fichier		=colormap+NREGISTRE*4
sprite		dc.l	0,0,0,0


