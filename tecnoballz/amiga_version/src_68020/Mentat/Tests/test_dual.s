***************************************************************************
* TEST ECRAN HARD AGA DUAL-PLAYFIELD
* LE 01 OCTOBRE 1993
***************************************************************************
		OPT	p=68020
SLINE1		=48
SLINE2		=48
NPLAN1		=4
NPLAN2		=4
SPLAN1		=SLINE1*NPLAN1
SPLAN2		=SLINE2*NPLAN2
NLINE1		=512
NLINE2		=512
ECRAN1_SIZE	=SPLAN1*NLINE1
ECRAN2_SIZE	=SPLAN2*NLINE2
HSPRITE		=128
SPRITE_SIZE	=16*(HSPRITE+2)*2
BEAMCON0	=$1DC		($0200 = 50Hz / $0000 = 60Hz)
BPLCON3		=$106
BPLCON4		=$10C
FMODE		=$1FC
***************************************************************************
		SECTION AGACODE,CODE
debut:		bsr	save
		bsr.s	initialise
		bsr	loop
		bsr	restore
		moveq	#0,d0
		rts

***************************** INITIALISATION ******************************
initialise:	lea	$DFF000,a6
		lea	ecran.frau(pc),a0
		move.l	#fichier.frau,(a0)
		lea	ecran.girl(pc),a0
		move.l	#fichier.girl,(a0)
		bsr	spriteadr
		bsr	init_colors
		bsr	copytolutin16
		move	#$8360,$0096(a6)
		move	#$000f,FMODE(a6)
		move	#$0611,$0100(a6)	8 plans & dual-playfield
		move	#$0000,$0102(a6)
		move	#$0024,$0104(a6)
		move	#$0011,BPLCON4(a6)	couleur lutin palette 0, $1a0 
		move	#SPLAN1-SLINE1,$0108(a6)
		move	#SPLAN2-SLINE2,$010A(a6)
		move	#$2881,$008E(a6)
		move	#$28c1,$0090(a6)
		move	#$0038,$0092(a6)
		move	#$00d8,$0094(a6)
		move	#$0020,BEAMCON0(a6)	PAL 50Hz
		bsr	fillus
		bsr	fillos
		rts

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

		*/ Bouge les 8 lutins /*
		bsr	positionne_lutin

		*/ Raffraichit les pointeurs lutins /*
		lea	sprite0(pc),a0
		lea	$120(a6),a1
		moveq	#8-1,d0
.loop_sprite	move.l	(a0)+,(a1)+
		dbf	d0,.loop_sprite

		*/ Raffraichit les pointeurs écrans /*
		bsr	calcul_offset

		lea	$00E4(a6),a0
		moveq	#NPLAN2-1,d1
		move.l	#SLINE2,d2
		move.l	ecran.frau2(pc),d0
.loopbitplane	move.l	d0,(a0)
		addq	#8,a0
		add.l	d2,d0
		dbf	d1,.loopbitplane

		lea	$00E0(a6),a0
		move.l	#SLINE1,d2
		moveq	#NPLAN1-1,d1
		move.l	ecran.girl2(pc),d0
.loop_bitplane	move.l	d0,(a0)
		addq	#8,a0
		add.l	d2,d0
		dbf	d1,.loop_bitplane


		btst	#6,$bfe001
		bne	loop
		move	#$0000,$0100(a6)
xloop		btst	#6,$bfe001
		beq.s	xloop
		rts


*========================* Déplace les lutins *===========================*
positionne_lutin:
		move.l	a6,-(a7)
		move.l	#$81*4,d6
		move.l	#$28,d7
		lea	sprites.pt(pc),a2
		lea	sinus_x(pc),a3
		lea	sinus_y(pc),a4
		lea	sprite0(pc),a6
		moveq	#4-1,d5

		*/ lit coordonée x /*
.loop		move	(a2)+,d2		d2=vitesse incrément du pt. x
		add	d2,(a2)			incrémente le pt sur table x
		and	#$fe,(a2)		évite dépassement
		move	(a2)+,d2		d2=pointeur sur table x
		moveq	#0,d0
		move	(a3,d2),d0		d0=position x

		*/ lit coordonée y /*
		move	(a2)+,d2		d2=vitesse incrément du pt. y
		add	d2,(a2)			incrémente le pt sur table y
		and	#$3fe,(a2)		évite dépasement
		move	(a2)+,d2		d2=pointeur sur table y
		moveq	#0,d1
		move	(a4,d2),d1		d1=position y

		*/ caclul mots de contrôle /*
		move.l	(a6)+,a0		a0=adr. liste lutin paire
		move.l	(a6)+,a1		a1=adr. liste lutin impaire
		moveq	#HSPRITE,d2		d2=hauteur du lutin
		bsr	calcul_mot_ctrl
		move	d3,8(a0)	
		or	#$80,d3			Met le bit AT
		move	d3,8(a1)	
		swap	d3
		move	d3,(a0)	
		move	d3,(a1)	
		dbf	d5,.loop
		move.l	(a7)+,a6
		rts

*-------------------------------------------------------------------------*
init_colors
		bsr	raz_256colors

		lea	girlcolors(pc),a0
		move	vbplcon3(pc),BPLCON3(a6)
		lea	$180(a6),a1
		moveq	#8-1,d0
.loop		move.l	(a0)+,(a1)+
		dbf	d0,.loop

		lea	xcolors(pc),a0
		moveq	#8-1,d0
.bloop		move.l	(a0)+,(a1)+
		dbf	d0,.bloop

		move	vbplcon3(pc),d0
		or	#$2000,d0
		move	d0,BPLCON3(a6)
		lea	fraucolors(pc),a0
		lea	$180(a6),a1
		moveq	#8-1,d0
.loop1		move.l	(a0)+,(a1)+
		dbf	d0,.loop1
		rts

*-------------------------------------------------------------------------*
save:		move.l	4.w,a6
		jsr	-132(a6)
		lea	gfxname(pc),a1
		moveq	#0,d0
		jsr	-552(a6)
		lea	gfxbase(pc),a0
		move.l	d0,(a0)
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
		move.l	(a0),a1
		clr.l	(a0)
		jsr	-414(a6)
		jsr	-138(a6)
		moveq	#0,d0
		rts


*********** On calcule offset déplacement vertical des 2 écran ************
calcul_offset:

		move.l	#SPLAN1,d2
		move.l	ecran.girl(pc),d3
		lea	pt.girl(pc),a0
		bsr.s	.routine	
		move.l	#SPLAN1,d2
		move.l	ecran.girl(pc),d3
		lea	pt.girl(pc),a0
		bsr.s	.routine	

		move.l	#SPLAN2,d2
		move.l	ecran.frau(pc),d3
		lea	pt.frau(pc),a0


* entrée	d2=taille d'un plan
.routine	move	(a0)+,d1		d1=incrémentation
		move	(a0),d0			d0=offset
		beq.s	.zero			-> nul
		cmp	#256,d0			offset maximum ?
		blt.s	.ok			-> non
		moveq	#-1,d1
		move	d1,-2(a0)		
		bra.s	.ok
.zero		moveq	#1,d1
		move	d1,-2(a0)
.ok		add	d1,(a0)
		moveq	#0,d0
		move	(a0)+,d0		d0=offset
		mulu	d2,d0			*taille d'une ligne
		add.l	d3,d0			+adr.début  écran
		move.l	d0,(a0)
		rts		


* CALCUL ADRESSE DES LISTES LUTINS
spriteadr:	move.l	#sprite,d0
		lea	sprite0(pc),a0
		moveq	#8-1,d1
.loop		move.l	d0,(a0)+
		add.l	#SPRITE_SIZE,d0
		dbf	d1,.loop
		rts	

*/ RECOPIE DESSINS DANS LISTE LUTIN /*
copytolutin16:
		move.l	ecran.girl(pc),a2
		lea	SPLAN1*52+16(a2),a2
		move.l	sprite0(pc),a0
		move.l	sprite1(pc),a1
		bsr.s	.routine
		move.l	sprite2(pc),a0
		move.l	sprite3(pc),a1
		bsr.s	.routine
		move.l	sprite4(pc),a0
		move.l	sprite5(pc),a1
		bsr.s	.routine
		move.l	sprite6(pc),a0
		move.l	sprite7(pc),a1

.routine	move	#HSPRITE-1,d0
		move	#SPLAN1,d2
		moveq	#SLINE1,d1

		lea	16(a0),a0		Saute mot ctrl lutin paire
		lea	16(a1),a1		Saute mot ctrl lutin impaire
copylutin16:	movem.l	d0/a2/a4,-(a7)
.loop		move.l	a2,a4
		move.l	(a4),(a0)+		Plan 1 -> lutin paire
		move.l	4(a4),(a0)+		Plan 1 -> lutin paire
		add	d1,a4
		move.l	(a4),(a0)+		Plan 2 -> lutin paire
		move.l	4(a4),(a0)+		Plan 2 -> lutin paire
		add	d1,a4
		move.l	(a4),(a1)+		Plan 3 -> lutin impaire
		move.l	4(a4),(a1)+		Plan 3 -> lutin impaire
		add	d1,a4
		move.l	(a4),(a1)+		Plan 4 -> lutin impaire
		move.l	4(a4),(a1)+		Plan 4 -> lutin impaire
		add	d2,a2
		dbf	d0,.loop
		movem.l	(a7)+,d0/a2/a4
		rts


*************** Calcul les deux mots de contrôle d'un lutin ***************
* Entrée	d0.l	Pos. X du lutin
*		d1.l	Pos. Y du lutin
*		d2.w	Hauteur du lutin
*		d6.w	Pos. X fenêtre écran * 4 (superhires)
*		d7.w	Pos. Y fenêtre écran
* Sortie	d3.l	Les 2 mots de contrôles
* Utilise	d0,d1,d2,d3,d6,d7
* Bit n° 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00 (MOT CONTROLE 1)
*	 E7 E6 E5 E4 E3 E2 E1 E0 HA H9 H8 H7 H6 H5 H4 H3
* Bit n° 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00 (MOT CONTROLE 2)
*	 L7 L6 L5 L4 L3 L2 L1 L0 AT 00 00 H1 H0 E8 L8 H2
* bits HA-H8 Pos. horz. du lutin (HSTART) (11bits (AGA uniquement)
* bits E0-E8 Pos. vert. première ligne du lutin (VSTART)
* bits L0-L8 Pos. vert dernière ligne + 1 du lutin (VSTOP) 
* Attention la position x des lutins est gérés en superhires (1280 pixels)
calcul_mot_ctrl:
		add.w	d6,d0			Coordonnée X réelle
		add.w	d7,d1			Coordonnée Y réelle
		move.w	d1,d3			d3=Pos. Y 1ere ligne lutin
		add.w	d2,d3			d3=Pos. Y dernière ligne lutin
		lsl.w	#8,d3
		bcc.s	.not_e8			-> bit L8 à zéro
		or.w	#$0002,d3		Place le bit L8
.not_e8		lsr.w	#1,d0			Bit H0 mis ?
		bcc.s	.not_h0			-> non
		or.w	#$0008,d3		Place le bit H0
.not_h0		lsr.w	#1,d0			Bit H1 mis ?
		bcc.s	.not_h1			-> non
		or.w	#$0010,d3		Place le bit H1
.not_h1		lsr	#1,d0			Bit H2 mis ?
		bcc.s	.not_h2			-> non
		or.w	#$0001,d3		Place le bit H2
.not_h2		lsl.w	#8,d1
		bcc.s	.not_l8			-> bit E8 à zéro
		or.w	#$0004,d3		Place le bit E8
.not_l8		swap	d3
		move.w	d0,d3			d3=HSTART bit H1 à H8
		or.w	d1,d3			+ VSTART bit E0 à E7
		swap.w	d3
		rts

*===========* Entre une même valeur au 256 registres couleurs *===========*
raz_256colors:
		move	#$fff,d0
		move	#$fff,d1
		movem.l	d2-d7/a0-a2,-(a7)
		lea	$180(a6),a1
		lea	BPLCON3(a6),a2
		moveq	#8-1,d4			8 palettes de 32 registres
		move	vbplcon3(pc),d6
		move	vbplcon3(pc),d7
		or	#$0200,d7		sélectionne bits 0 à 3 RVB
		moveq	#0,d2			spécifie la palette

		* 4 bits de poids fort des 3 composantes *
.loop_color	moveq	#32-1,d3		32 regitres couleurs
		move.l	a1,a0			a0='COLOR00'
		move	d2,d5
		or	d6,d5			bits 4 à 7
		move	d5,(a2)			initialise 'BPLCON3'
.bits4a7	move	d0,(a0)+
		dbf	d3,.bits4a7

		* 4 bits de poids faible des 3 composantes *
.cont		moveq	#32-1,d3		32 regitres couleurs
		move.l	a1,a0			a0='COLOR00'
		move	d2,d5
		or	d7,d5			bits 0 à 3
		move	d5,(a2)			initialise 'BPLCON3'
.bits0a3	move	d1,(a0)+
		dbf	d3,.bits0a3

		add	#$2000,d2		prochaine palette
		dbf	d4,.loop_color
		movem.l	(a7)+,d2-d7/a0-a2
		rts




fillus:		move.l	ecran.frau(pc),a0
		lea	motif(pc),a1
		moveq	#16-1,d7
.loop0		move.l	(a1)+,d0
		move.l	(a1)+,d1
		move.l	(a1)+,d2
		move.l	(a1)+,d3
		moveq	#8-1,d4
.loop		move.l	d3,(a0)
		move.l	d2,SLINE1*1(a0)
		move.l	d1,SLINE1*2(a0)
		move.l	d0,SLINE1*3(a0)
		lea	SPLAN1(a0),a0
		dbf	d4,.loop
		dbf	d7,.loop0
		rts

fillos:		move.l	ecran.girl(pc),a0
		lea	36(a0),a0
		lea	motif(pc),a1
		moveq	#16-1,d7
.loop0		move.l	(a1)+,d0
		move.l	(a1)+,d1
		move.l	(a1)+,d2
		move.l	(a1)+,d3
		moveq	#8-1,d4
.loop		move.l	d3,(a0)
		move.l	d2,SLINE1*1(a0)
		move.l	d1,SLINE1*2(a0)
		move.l	d0,SLINE1*3(a0)
		lea	SPLAN1(a0),a0
		dbf	d4,.loop
		dbf	d7,.loop0
		rts



motif:		dc.l	00,00,00,00
		dc.l	00,00,00,-1
		dc.l	00,00,-1,00
		dc.l	00,00,-1,-1
		dc.l	00,-1,00,00
		dc.l	00,-1,00,-1
		dc.l	00,-1,-1,00
		dc.l	00,-1,-1,-1
		dc.l	-1,00,00,00
		dc.l	-1,00,00,-1
		dc.l	-1,00,-1,00
		dc.l	-1,00,-1,-1
		dc.l	-1,-1,00,00
		dc.l	-1,-1,00,-1
		dc.l	-1,-1,-1,00
		dc.l	-1,-1,-1,-1





		CNOP 0,8
copper:		dc.l	0
sprite0		dc.l	0
sprite1		dc.l	0
sprite2		dc.l	0
sprite3		dc.l	0
sprite4		dc.l	0
sprite5		dc.l	0
sprite6		dc.l	0
sprite7		dc.l	0
gfxbase		dc.l	0
ecran.girl	dc.l	0
ecran.frau	dc.l	0

		* Déplacement vertical des 2 écrans *
pt.girl		dc	1,0
ecran.girl2	dc.l	0
pt.frau		dc	1,0
ecran.frau2	dc.l	0

		* Déplacement des 4 lutins attachés *
sprites.pt	dc	2,00,6,0
		dc	2,32,6,128
		dc	2,64,2,256
		dc	2,96,6,512

*$0000,$0400,$0800,$0c00,$1000,$1400,$1800,$1c00

vbplcon3	dc	$1400
save_intena	dc	0
save_dmacon	dc	0

gfxname		dc.b	'graphics.library',0
		EVEN

sinus_x:	dc.w	1020,1019,1018,1014,1010,1004,0998,0990,0981,0970
		dc.w	0959,0946,0933,0918,0902,0886,0868,0850,0831,0811
		dc.w	0790,0769,0747,0724,0701,0677,0653,0629,0604,0579
		dc.w	0554,0529,0504,0478,0453,0428,0404,0379,0355,0331
		dc.w	0308,0285,0262,0241,0219,0199,0180,0161,0143,0126
		dc.w	0110,0094,0080,0067,0055,0044,0035,0026,0019,0013
		dc.w	0008,0004,0001,0000,0000,0001,0004,0008,0013,0019
		dc.w	0026,0035,0044,0055,0067,0080,0094,0110,0126,0143
		dc.w	0161,0179,0199,0219,0241,0262,0285,0308,0331,0355
		dc.w	0379,0404,0428,0453,0478,0504,0529,0554,0579,0604
		dc.w	0629,0653,0677,0701,0724,0747,0769,0790,0811,0831
		dc.w	0850,0868,0886,0902,0918,0933,0946,0959,0970,0981
		dc.w	0990,0998,1004,1010,1014,1018,1019,1020

sinus_y:	dc.w	113,113,113,113,113,113,113,113,113,113
		dc.w	113,112,112,112,112,112,112,112,112,111
		dc.w	111,111,111,111,111,110,110,110,110,109
		dc.w	109,109,109,108,108,108,108,107,107,107
		dc.w	106,106,106,105,105,105,104,104,103,103
		dc.w	103,102,102,101,101,101,100,100,099,099
		dc.w	098,098,097,097,096,096,095,095,094,094
		dc.w	093,093,092,092,091,091,090,090,089,088
		dc.w	088,087,087,086,085,085,084,084,083,082
		dc.w	082,081,081,080,079,079,078,077,077,076
		dc.w	075,075,074,073,073,072,071,071,070,069
		dc.w	069,068,067,067,066,065,065,064,063,063
		dc.w	062,061,060,060,059,058,058,057,056,056
		dc.w	055,054,054,053,052,051,051,050,049,049
		dc.w	048,047,047,046,045,045,044,043,043,042
		dc.w	041,041,040,039,039,038,037,037,036,035
		dc.w	035,034,033,033,032,032,031,030,030,029
		dc.w	028,028,027,027,026,025,025,024,024,023
		dc.w	023,022,022,021,020,020,019,019,018,018
		dc.w	017,017,016,016,015,015,014,014,014,013
		dc.w	013,012,012,011,011,011,010,010,009,009
		dc.w	009,008,008,008,007,007,007,006,006,006
		dc.w	005,005,005,004,004,004,004,003,003,003
		dc.w	003,003,002,002,002,002,002,001,001,001
		dc.w	001,001,001,001,001,000,000,000,000,000
		dc.w	000,000,000,000,000,000,000,000,000,000
		dc.w	000,000,000,000,000,000,000,001,001,001
		dc.w	001,001,001,001,001,002,002,002,002,002
		dc.w	003,003,003,003,003,004,004,004,004,005
		dc.w	005,005,006,006,006,007,007,007,008,008
		dc.w	008,009,009,009,010,010,011,011,011,012
		dc.w	012,013,013,014,014,014,015,015,016,016
		dc.w	017,017,018,018,019,019,020,020,021,022
		dc.w	022,023,023,024,024,025,025,026,027,027
		dc.w	028,028,029,030,030,031,032,032,033,033
		dc.w	034,035,035,036,037,037,038,039,039,040
		dc.w	041,041,042,043,043,044,045,045,046,047
		dc.w	047,048,049,049,050,051,051,052,053,054
		dc.w	054,055,056,056,057,058,058,059,060,060
		dc.w	061,062,063,063,064,065,065,066,067,067
		dc.w	068,069,069,070,071,071,072,073,073,074
		dc.w	075,075,076,077,077,078,079,079,080,081
		dc.w	081,082,082,083,084,084,085,085,086,087
		dc.w	087,088,088,089,090,090,091,091,092,092
		dc.w	093,093,094,094,095,095,096,096,097,097
		dc.w	098,098,099,099,100,100,101,101,101,102
		dc.w	102,103,103,103,104,104,105,105,105,106
		dc.w	106,106,107,107,107,108,108,108,108,109
		dc.w	109,109,109,110,110,110,110,111,111,111
		dc.w	111,111,111,112,112,112,112,112,112,112
		dc.w	112,113,113,113,113,113,113,113,113,113
		dc.w	113,113


girlcolors:	dc	$000,$ad5,$57a,$bf6
		dc	$000,$9c5,$8c5,$8b4
		dc	$7a4,$791,$693,$682
		dc	$582,$472,$361,$240

*girlcolors:	dc	$000,$ad5,$57a,$bf6
		dc	$000,$9c5,$8c5,$8b4
		dc	$fff,$791,$693,$682
		dc	$fff,$fff,$fff,$fff
		dc	$fff,$fff,$fff,$fff
		dc	$582,$472,$361,$240

fraucolors	dc	$000,$da7,$b85,$a74
	 	dc	$963,$741,$520,$300
	 	dc	$249,$24b,$35a,$46b
	 	dc	$57c,$68d,$79e,$24d

xcolors:	dc	$000,$d05,$70a,$f06
		dc	$000,$c05,$c05,$b04
		dc	$a04,$901,$903,$802
		dc	$802,$702,$601,$400


*=========================================================================*
**************************** Section graphique ****************************
*=========================================================================*
		SECTION		DATACHIP,DATA_C
		CNOP 0,8		* on aligne sur 64 bits
fichier.girl	incbin	"DH0:Data/ILBM/okgirl.RAW"
fichier.frau	incbin	"DH0:Data/ILBM/okfrau.RAW"
		CNOP 0,8		* on aligne sur 64 bits
sprite:		ds.l	SPRITE_SIZE*8/4


copper_list:	ds.l	4*256
* %000		couleur 00 à 15	 (palette 0)
* %001		couleur
* %010		couleur (?4*4 meme palette)
* %011		couleur (?2x8 meme palette)
* %100		couleur 16 à 31  (palette 0)
* %101 		couleur 32 à 47  (palette 1)
* %110		couleur	64 à 79  (palette 2)
* %111		couleur 96 à 111 (palette 3)

		END

                      BIT#   BPLCON0   BPLCON1   BPLCON2   BPLCON3    BPLCON4
                      ----   -------   -------   -------   -------    -------
                      15     HIRES     PF2H7=0   X         BANK2=0    BPLAM7=0 
                      14     BPU2      PF2H6=0   ZDBPSEL2  BANK1=0    BPLAM6=0
                      13     BPU1      PF2H1=0   ZDBPSEL1  BANK0=0    BPLAM5=0
                      12     BPU0      PF2H0=0   ZDBPSEL0  PF2OF2=0   BPLAM4=0
                      11     HAM       PF1H7=0   ZDBPEN    PF2OF1=1   BPLAM3=0
                      10     DPF       PF1H6=0   ZDCTEN    PF2OF0=1   BPLAM2=0
                      09     COLOR     PF1H1=0   KILLEHB   LOCT=0     BPLAM1=0
                      08     GAUD      PF1H0=0   RDRAM=0   X          BPLAM0=0
                      07     UHRES     PF2H5     SOGEN=0   SPRES1=0   ESPRM7=0
                      06     SHRES     PF2H4     PF2PRI    SPRES0=0   ESPRM6=0
                      05     BYPASS=0  PF2H3     PF2P2     BRDRBLNK=0 ESPRM5=0
                      04     BPU3=0    PF2H2     PF2P1     BRDNTRAN=0 ESPRM4=1
                      03     LPEN      PF1H5     PF2P0     X          OSPRM7=0
                      02     LACE      PF1H4     PF1P2     ZDCLKEN=0  OSPRM6=0
                      01     ERSY      PF1H3     PF1P1     BRDSPRT=0  OSPRM5=0
                      00     ECSENA=0  PF1H2     PF1P0     EXTBLKEN=0 OSPRM4=1


          ESPRMx = 4 Bit field provides the 4 high order color table address
          bits for even sprites: SPR0,SPR2,SPR4,SPR6. Default value is 0001
          binary. (x=7-4)
          
         OSPRMx = 4 Bit field provides the 4 high order color table address
         bits for odd sprites: SPR1,SPR3,SPR5,SPR7. Default value is 0001
         binary. (x=7-4)

