


		*	THE ULTIMATE GAMES FACTORY		*

		*	Version by DJI in Summer 94		*

		*	Started : 10 Aout 94			*

		*	System AGA by Nobru			*



;		Version 	Lundi 15 Aout 1994




		;OPT	P+		Pas de table de relogement, code relatif
		;OPT	p=68000

MAP_HIGH	=16

GFXB_AA_ALICE	=2
gb_ChipRevBits0 =$ec
ECRAN_WIDTH	=40
ECRAN_HIGH	=MAP_HIGH*16
ECRAN_DEEP	=8
ECRAN_LINE	=ECRAN_WIDTH*ECRAN_DEEP
ECRAN_SIZE	=ECRAN_WIDTH*ECRAN_HIGH*ECRAN_DEEP
NREGISTRE	=256
SCREEN_SIZE	=ECRAN_SIZE

MEMORY_SIZE=SCREEN_SIZE*2



		incdir	df0:
		*incdir	dh0:DJI/

		SECTION TLK,CODE

CIAA_PRA	=$bfe001	/FIR1 /FIR0 /RDY /TK0 /WPR0 /CHNG /LED OVL
CIAA_SDR	=$bfec01	CIAA serial data register (connected keyboard)
CIAA_ICR	=$bfed01	CIAA interrupt control register
CIAA_CRA	=$bfee01	CIAA control register A
CIAA_CRB	=$bfef01	CIAA control register B
CIAB_PRA	=$bfd000	/DTR /RTS /CD /CTS /DSR SEL POUT BUSY
CIAB_PRB	=$bfd100	/MTR /SEL3 /SEL2 /SEL1 /SEL0 /SIDE DIR /STEP
CIAB_TALO	=$bfd400	CIAB timer A low byte
CIAB_TAHI	=$bfd500	CIAB timer A high byte
CIAB_TBLO	=$bfd600	CIAB timer B low byte
CIAB_TBHI	=$bfd700	CIAB timer B high byte
CIAB_ICR	=$bfdd00	CIAB interrupt control register
CIAB_CRA	=$bfde00	CIAB control register A
CIAB_CRB	=$bfdf00	CIAB control register B
CUSTOM		=$dff000
COLOR00		=$180
INTREQR		=$01e
INTREQ		=$09c
DMACONR		=$002
INTENAR		=$01c
ADKCONR		=$010
DMACON		=$096
INTENA		=$09a
ADKCON		=$09e
BPLCON3		=$106	W	AGA	accès reg. couleurs + taille lutins
BPLCON4		=$10C	W	AGA	base palette lutins
BEAMCON0	=$1DC	W	ECS	$0020 = 50Hz / $0000 = 60Hz
FMODE		=$1FC	W	AGA	burst $0=1x / $2=2x / $3=4x
BPLHSTRT	=$1D4
BPLHSTOP	=$1D6




WB		MACRO
.tintin\@	btst	#14,$dff002
		bne.s	.tintin\@
		ENDM








;		-=- quitte proprement le systeme -=-
;			--------------------

d:		movem.l	d0-d7/a0-a6,-(a7)
		move.l	4.w,a6

		lea	gfxname(pc),a1
		moveq	#0,d0
		jsr	-552(a6)
		lea	gfxbase(pc),a0
		move.l	d0,(a0)
		beq	return		
		lea	dosname(pc),a1
		moveq	#0,d0
		jsr	-552(a6)
		lea	dosbase(pc),a0
		move.l	d0,(a0)
		beq	dos_error
		
		move.l	4.w,a6
		jsr	-132(a6)
		lea	vecteur(pc),a0
		move.l	$6c.w,(a0)
		lea	vecteurzz(pc),a0
		move.l	$68.w,(a0)
		lea	pil(pc),a0
		move.l	a7,(a0)
		lea	$dff000,a6


		lea	save_intena(pc),a0
		move	INTENAR(a6),(a0)
		or	#$c000,(a0)+
		move	DMACONR(a6),(a0)
		or	#$8000,(a0)+
		move	ADKCONR(a6),(a0)
		or	#$8000,(a0)
		move	#$7fff,$9a(a6)
		move	#$7fff,$9c(a6)
		move	#$07ff,$96(a6)
		lea	error(pc),a2
		lea	vektor(pc),a1
		lea	8.w,a0
		moveq	#9,d0
aubert		move.l	(a0),(a1)+
		move.l	a2,(a0)+
		dbf	d0,aubert
		move	#$0020,BEAMCON0(a6)




		*/ Réserve la mémoire /*
		move.l	#MEMORY_SIZE,d0
		moveq	#2,d1
		move.l	4.w,a6
		jsr	-198(a6)		|-> AllocMem
		lea	tlk(pc),a5
		move.l	d0,-04(a5)
		beq	memory_error


		bra	debut

;		-=- revient proprement au systeme -=-
;		     ---------------------------

fin:		move.l	pil(pc),a7
		lea	vektor(pc),a0
		lea	8.w,a1
		moveq	#9,d0
hubert		move.l	(a0)+,(a1)+
		dbf	d0,hubert
		lea	$dff000,a6
		move	#$7fff,$9a(a6)
		move	#$7fff,$9c(a6)
		move	#$07ff,$96(a6)
		move.l	vecteur(pc),$6c.w
		move.l	vecteurzz(pc),$68.w
		move	save_adkcon(pc),ADKCON(a6)
		move	save_dmacon(pc),DMACON(a6)
		move	save_intena(pc),INTENA(a6)
		bsr	remet_freq
		move.l	gfxbase(pc),a1
		move.l	38(a1),$80(a6)
		move	d0,$88(a6)

		*/ Libère la mémoire /*
		lea	tlk(pc),a5
		move.l	-04(a5),a1
		move.l	#MEMORY_SIZE,d0
		move.l	4.w,a6
		jsr	-210(a6)		|-> Freemem

		move.l	4.w,a6
		jsr	-138(a6)
memory_error	move.l	dosbase(pc),a1
		jsr	-414(a6)		
dos_error	move.l	gfxbase(pc),a1
		jsr	-414(a6)
return:		movem.l	(a7)+,d0-d7/a0-a6
		rts



install_vecteurs:

		lea	$dff000,a6
		lea	irq(pc),a0
		move.l	a0,$6c.w
		lea	interruption_clavier(pc),a0
		move.l	a0,$68.w
		move	#$87c0,$96(a6)			autorise 'DMA COPPER'
						;		 'DMA BITPLANE'
						;		 'DMA BLITER'
						;	+blitter prioritaire

		move	#$0020,$96(a6)			coupe 'DMA SPRITE'
		move	#$c028,$9a(a6)			autorise VBL+IT CLAVIER
		rts


*-----------------* Remet la fréquence en 60Hz ou 50Hz *------------------*
remet_freq:   
		movem.l	d0/a0/a6,-(a7)
		lea	CUSTOM,a6
		move.l	4.w,a0			a0=ExecBase
		moveq	#$0020,d0
		cmp.b	#50,530(a0)		50hz = pal ?
		beq.s	.pal			-> oui
		moveq	#0,d0
.pal		move	d0,BEAMCON0(a6)
		movem.l	(a7)+,d0/a0/a6
		rts

*---------------------* Traitement d'une exception *----------------------*
error:		lea	e_inf(pc),a0
		move	(a7)+,(a0)
		move.l	(a7)+,e_adr-e_inf(a0)
		move	(a7)+,e_inst-e_inf(a0)
		move	(a7)+,e_sr-e_inf(a0)
		move.l	(a7)+,e_pc-e_inf(a0)
		move	#0,sr
		bra	fin
e_inf		dc	0
e_adr		dc.l	0
e_inst		dc	0
e_sr		dc	0
e_pc		dc.l	0
vektor		ds.l	10
gfxname		dc.b	'graphics.library',0
dosname		dc.b	'dos.library',0
		EVEN
pil		dc.l	0
vecteur		dc.l	0
vecteurzz	dc.l	0
gfxbase		dc.l	0
dosbase		dc.l	0
save_intena	dc	0
save_dmacon	dc	0
save_adkcon	dc	0




*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*
*
*		DEBUT PROGRAMME
*
*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*

*=========================================================================*
*		Initialisation globale
*=========================================================================*
debut:		lea	CUSTOM,a6
		bsr	spriteoff


		lea	madness,a4		fichier ILBM
		lea	colormap,a2
		lea	fichier,a3
		bsr	decompacte_ilbm

		;bsr	convert_aga_color
		bsr	set_colors


		* Init. adresses *
		lea	tlk(pc),a5
		move.l	a5,d0
		add.l	d0,-12(a5)		adr table *200
		add.l	d0,-16(a5)		adr table *40


		* Calcul adresses "espace graphique" *
		move.l	-04(a5),d2		adr. ecran
		add.l	#ECRAN_SIZE,d2


		* Init. les adresses routines des IRQ + EDITEUR *
		lea	saut_irq(pc),a0
		move	#(saut_irqfin-saut_irq)/4-1,d1
.nenneh		add.l	d0,(a0)+
		dbf	d1,.nenneh



		* Initialise la table "mulu40" *
		move.l	-16(a5),a0
		moveq	#0,d0
		move	#ECRAN_HIGH-1,d1
.ror		move	d0,(a0)+
		add.l	#ECRAN_WIDTH,d0
		dbf	d1,.ror

		* Initialise la table "mulu200" *
		move.l	-12(a5),a0
		moveq	#0,d0
		move	#ECRAN_HIGH-1,d1
.rir		move.l	d0,(a0)+
		add.l	#ECRAN_LINE,d0
		dbf	d1,.rir


		* Efface les ecrans *

		move.l	-04(a5),a0
		moveq	#0,d1
		move	#ECRAN_SIZE/4-1,d0
.moi		move.l	d1,(a0)+
		dbf	d0,.moi





;		-=- init pré-boot -=-
		move	#0,go_irq-tlk(a5)


;		-=- mets la becane en place -=-
;			---------------


		move	#$8360,$0096(a6)

		move	#$000f,$01FC(a6)

		move	#$0211,$0100(a6)
		move	#$0000,$0102(a6)
		move	#$0024,$0104(a6)
		move	sbplcon3(pc),BPLCON3(a6)
		move	#(40*7),d0
		move	d0,$0108(a6)
		move	d0,$010A(a6)
		move	#$2881,$008E(a6)
		move	#$28c1,$0090(a6)
		move	#$0038,$0092(a6)
		move	#$00b8,$0094(a6)

		move	BPLHSTRT(a6),toto

		lea	copper,a0
		move.l	a0,$80(a6)
		move	d0,$88(a6)

		bsr	install_vecteurs


;		-=- initialise le module -=-
;			------------		
		lea	$dff000,a6
		bra	boucle_synchro




************ calcul adresse pointeurs vidéo de la liste copper ************
* entrée	=> a0	pt. liste copper
*		=> d0	adr. 1er plan
*		=> d1	nbre plan -1
*		=> d2	distance entre 2 plans
pointevideo:	swap	d0
		move	d0,(a0)
		addq.l	#4,a0
		swap	d0
		move	d0,(a0)	
		addq.l	#4,a0
		add.l	d2,d0
		dbf	d1,pointevideo
		rts

*************** Supprime les montants verticals des sprites ***************
* entrée	=> a6	CUSTOM
spriteoff:	lea	$120(a6),a0
		moveq	#0,d0
		move	#($180-$120)/2-1,d1
sp_off		move	d0,(a0)+
		dbf	d1,sp_off
		rts





*=========================================================================*
*		interruption VBL
*=========================================================================*
irq:		movem.l	d0-d7/a0-a6,-(a7)
		lea	CUSTOM,a6
		lea	vbl_flag(pc),a0
		addq	#1,(a0)
		move	#$0020,INTREQ(a6)
		movem.l	(a7)+,d0-d7/a0-a6
		rte


boucle_synchro:	lea	tlk(pc),a5
		lea	CUSTOM,a6
		lea	vbl_flag(pc),a0
		move	(a0),d0
.wait_vbl	cmp	(a0),d0
		beq.s	.wait_vbl

		lea	saut_irq(pc),a0
		move	go_irq(pc),d0
		add	d0,a0
		move.l	(a0),a0
		jsr	(a0)

		lea	$dff000,a6


		;tst	cycle-tlk(a5)
		;beq.s	.nu
		;move	#$444,$dff180

.nu		tst	sortie-tlk(a5)
		beq.s	boucle_synchro

		lea	$dff000,a6

		bra	fin










		*==========*		*==========*









init_presentation:

		lea	CUSTOM,a6

		move	#4,go_irq-tlk(a5)

		* Efface l'ecran *

;		move.l	-04(a5),a0
;		moveq	#0,d1
;		move	#ECRAN_SIZE/4-1,d0
;.moi		move.l	d1,(a0)+
;		dbf	d0,.moi

;		lea	textintro(pc),a2
;		bsr	aff_text
		rts



presentation:

		lea	CUSTOM,a6


		*/ Raffraichit les pointeurs écrans /*

;		lea	fichier,a0
;		move.l	a0,d0
;		lea	$00E0(a6),a0
;		moveq	#8-1,d1
;.loop_bitplane	move.l	d0,(a0)+
;		add.l	#40,d0
;		dbf	d1,.loop_bitplane


;.wait		move.l	$dff004,d2
;		and.l	#$0001ff00,d2
;		cmp.l	#$4000,d2
;		bne.s	.wait


;		lea	fichier,a0
;		move.l	a0,d0
;		lea	$00E0(a6),a0
;		moveq	#8-1,d1
;.loop_bitplane1	move.l	d0,(a0)+
;		add.l	#40,d0
;		dbf	d1,.loop_bitplane1


		lea	fichier,a0
		move.l	a0,d0
		lea	copper+2,a0
		moveq	#8-1,d1
		moveq	#40,d2
		bsr	pointevideo


		lea	fichier,a0
		move.l	a0,d0
		lea	copperbis+2,a0
		moveq	#8-1,d1
		moveq	#40,d2
		bsr	pointevideo



;		-=- debut des test du clavier -=-	

		lea	table_clavier(pc),a0

		tst.b	$45(a0)			Esc quitte ?
		beq.s	.aza1
		move	#1,sortie-tlk(a5)

.aza1




.fin		rts



		*==========*		*==========*






init_createmap:

		rts



createmap:
		rts








		*************************************************

		*						*

		*	G R A P H I C     R O U T I N E		*

		*						*

		*************************************************


eff_hlines	move.l	-04(a5),a0
		
		lea	mouse(pc),a4
		move	02(a4),d0
		and	#$fff0,d0
		mulu	#40*5,d0
		add.l	d0,a0

		move	#(40*5)/4-1,d1
		moveq	#0,d0
.real		move.l	d0,(a0)+
		dbf	d1,.real
		rts




aff_hlines	move.l	-04(a5),a0
		
		lea	mouse(pc),a4
		move	02(a4),d0
		and	#$fff0,d0
		mulu	#40*5,d0
		add.l	d0,a0

		move	#(40*5)/4-1,d1
		moveq	#-1,d0
.real		move.l	d0,(a0)+
		dbf	d1,.real
		rts




;		entrée	d2	chiffre à afficher


aff_score:	lea	pupitre,a0
		add.l	d0,a0

		moveq	#0,d3
.night		cmp	#100,d2
		blt.s	.time
		addq	#1,d3
		sub	#100,d2
		bra.s	.night
.time		lea	fonts2,a1
		add	#40*5+26,a1
		add	d3,a1
		bsr	.texas			affiche centaine

		moveq	#0,d3
.night1		cmp	#10,d2
		blt.s	.time1
		addq	#1,d3
		sub	#10,d2
		bra.s	.night1
.time1		lea	fonts2,a1
		add	#40*5+26,a1
		add	d3,a1
		bsr	.texas			affiche dizaine

		moveq	#0,d3
.night2		tst	d2
		beq.s	.time2
		addq	#1,d3
		sub	#1,d2
		bra.s	.night2
.time2		lea	fonts2,a1
		add	#40*5+26,a1
		add	d3,a1

.texas		moveq	#5*7-1,d0
		moveq	#0,d1
.loop		move.b	(a1,d1),(a0,d1)
		add.l	#40,d1
		dbf	d0,.loop
.loopend	addq.l	#1,a0
		rts



;		entrée	a2	structure text


aff_text:	move.l	(a2)+,d4
		move.l	-04(a5),a0
		add.l	d4,a0
		move.l	a0,d4
		moveq	#0,d0
		move.b	(a2)+,d0
		add.l	d0,a0


.again		moveq	#0,d0
		move.b	(a2)+,d0
		bne.s	.suit
		rts

.suit		cmp.b	#1,d0		-> next line ?
		bne.s	.verre		-> non
		add.l	#ECRAN_WIDTH*ECRAN_DEEP*12,d4
		move.l	d4,a0
		move.b	(a2)+,d0
		add.l	d0,a0
		bra.s	.again

.verre		cmp.b	#' ',d0
		beq.s	.loopend

		cmp.b	#'A',d0
		blt.s	.marria
		cmp.b	#'Z',d0
		bgt.s	.marria
		sub.b	#'A',d0

.marria		cmp.b	#'0',d0
		blt.s	.marria1
		cmp.b	#'9',d0
		bgt.s	.marria1
		sub.b	#'0',d0
		add	#26,d0
.marria1

		lea	fonts2,a1
		add	d0,a1

		move	#5*9-1,d0
		moveq	#0,d1
.loop		move.b	(a1),(a0,d1)
		add.l	#46,d1
		add.l	#40,a1
		dbf	d0,.loop
.loopend	addq.l	#1,a0
		bra.s	.again







decor_indic:	;lea	boule1(pc),a3		affiche le decor
		;move	#NBOULE-1,d4


		;move	(a5),d6
		;WB
		;move.l	#$ffffffff,$44(a6)
		;move.l	#$09f00000,$40(a6)
		;move	#ECRAN_WIDTH-4,$64(a6)
		;move	#ECRAN_WIDTH-4,$66(a6)

.loop		;move.l	(a3,d6),d2		des tirs
		;beq.s	.end
		;move.l	#0,(a3,d6)
		;move.l	d2,d1
		;add.l	#SCREEN_SIZE,d1
		;tst	d6
		;bne.s	.kuit
		;add.l	#SCREEN_SIZE,d1
.kuit		;move	#18*5*64+2,d3
		;WB
		;move.l	d1,$50(a6)
		;move.l	d2,$54(a6)
		;move	d3,$58(a6)
.end		;lea	BOUL_SIZE(a3),a3
		;dbf	d4,.loop
		rts





aff_indic:	;move	(a5),d6			Calcul coordonnées

		;move	#18*5*64+2,d3
		;move.l	#$ffff0000,$44(a6)
		;move	#80-4,$64(a6)		modulo A (masque)
		;move	#80-4,$62(a6)		modulo B (graphisme)
		;move	#ECRAN_WIDTH-4,$60(a6)		modulo C (écran)
		;move	#ECRAN_WIDTH-4,$66(a6)		modulo D (écran)

		;lea	boule1(pc),a3
		;move	#NBOULE-1,d4

.loop		;moveq	#0,d0
		;moveq	#0,d1
		;move	BOUL_X(a3),d0
		;subq	#2,d0
		;cmp	#16,d0
		;bne	.juit

		;move	#320+32,d0

.marria		;lea	txtdata,a0
		;add	txtdatapnt(pc),a0
		;add	#1,txtdatapnt-tlk(a5)
		;moveq	#0,d1
		;move.b	(a0),d1
		;bne.s	.carrey
		;move	#0,txtdatapnt-tlk(a5)
		;bra.s	.marria

.carrey


.carrey02	;cmp.b	#' ',d1
		;bne.s	.carrey1
		;move.l	#80-2,BOUL_PT(a3)
		;bra.s	.juit

.carrey1	;cmp	#'A',d1
		;blt.s	.carrey2
		;cmp	#'Z',d1
		;bgt.s	.carrey2
		;sub	#'A',d1
		;add	d1,d1
		;move.l	d1,BOUL_PT(a3)
		;bra.s	.juit

.carrey2	;cmp	#'0',d1
		;blt.s	.carrey3
		;cmp	#'9',d1
		;bgt.s	.carrey3
		;sub	#'0',d1
		;add	#26,d1
		;add	d1,d1
		;move.l	d1,BOUL_PT(a3)
.carrey3
.juit		;move	d0,BOUL_X(a3)

		;move.l	-04(a5),d2		adresse de l'écran.

		;tst	(a5)
		;beq.s	.titi
		;add.l	#SCREEN_SIZE,d2

.titi		;move	d0,d1
		;asr	#3,d1			convertit pixels en octets
		;and	#$fffe,d1		met sur une adresse paire
		;add.l	d1,d2			n° octet/la ligne
		;and	#$000f,d0		valeur du décalage
		;ror	#4,d0			déplace les bits 

.ever		;lea	sinus(pc),a0
		;addq	#2,BOUL_SIN(a3)
		;add	BOUL_SIN(a3),a0
		;move	(a0),d1
		;bge.s	.mode
		;move	#0,BOUL_SIN(a3)
		;bra.s	.ever

.mode		;add	BOUL_Y(a3),d1
		;lsl.l	#2,d1



		;move.l	-12(a5),a0		mult 200
		;move.l	(a0,d1),d1
		;add.l	d1,d2			adresse réelle d'affichage
		;move.l	d2,(a3,d6)
		;lea	fonts1,a0
		;move.l	a0,d5			le gfx
		;add.l	BOUL_PT(a3),d5
		;move.l	d5,d1			le msk
		;add.l	#80*18*5,d1
		;WB
		;move	d0,$42(a6)		décalage B (BLTCON1) masque
		;add	#$0fca,d0		additionne liaison logique
		;move	d0,$40(a6)		décalage A (BLTCON0) graphisme
		;move.l	d1,$50(a6)		source A (le masque)
		;move.l	d2,$48(a6)		source C (l'écran)
		;move.l	d2,$54(a6)		destination D (l'écran)
		;move.l	d5,$4c(a6)		source B (le graphisme)
		;move	d3,$58(a6)		fenêtre blitter 32*2
.end		;lea	BOUL_SIZE(a3),a3
		;dbf	d4,.loop
		rts





		*************************************************

		*						*

		*	H A R D   A M I G A    R O U T I N E	*

		*						*

		*************************************************


gere_mouse:	moveq	#0,d4
		moveq	#0,d5
		move.b	$b(a6),d4		abscisse (JOYDAT)
		move.b	ax(pc),d5
		move.b	d4,ax-tlk(a5)
		sub.b	d5,d4
		tst.b	d4
		bge.s	.witri
		moveq	#0,d5
		move.b	d4,d5
		not.b	d5
		sub.w	d5,00(a4)
		bra.s	.neil
.witri		add.w	d4,00(a4)

.neil		moveq	#0,d4
		moveq	#0,d5
		move.b	$a(a6),d4		ordonnée (JOYDAT)
		move.b	ay(pc),d5
		move.b	d4,ay-tlk(a5)
		sub.b	d5,d4
		tst.b	d4
		bge.s	.witri2
		moveq	#0,d5
		move.b	d4,d5
		not.b	d5
		sub	d5,02(a4)
		bra.s	.dpf
.witri2		add	d4,02(a4)

	

;		-=- test les boutons -=-

.dpf		move	#0,bt1_mouse-tlk(a5)

		btst	#6,$bfe001
		bne.s	.dxg

		move	#1,bt1_mouse-tlk(a5)


;		-=- test depassements coordonnées-=-

.dxg		tst	00(a4)
		bpl.s	.dxd
		move	#0,00(a4)

.dxd		cmp	#320,00(a4)
		blt.s	.dyh	
		move	#319,00(a4)

.dyh		tst	02(a4)
		bpl.s	.dyb
		move	#0,02(a4)

.dyb		cmp	#240,02(a4)
		ble.s	.end_mouse
		move	#240,02(a4)
.end_mouse	rts










*=========================================================================*
*		INTERRUPTION DE NIVEAU 2 : CIA-A, Test clavier
*=========================================================================*
interruption_clavier:
		btst	#3,CIAA_ICR		irq du port série ?
		beq	.exit2			-> non
		movem.l	d0/a0,-(a7)
		clr	d0
		move.b	CIAA_SDR,d0		d0=code raw-clavier

		* envoie signal Handhake au clavier (85 ms d'après Commodore) *
		* 1 ligne raster en pal = 63,5 microsecondes
		ori.b	#$40,CIAA_CRA
		swap	d0
		lea	$DFF006,a0
		move.b	(a0),d0
.wait1		cmp.b	(a0),d0
		beq.s	.wait1
		move.b	(a0),d0
.wait2		cmp.b	(a0),d0
		beq.s	.wait2
		move.b	(a0),d0
.wait3		cmp.b	(a0),d0
		beq.s	.wait3
		swap	d0
		andi.b	#$BF,CIAA_CRA

		* Test si touche enfoncée ou relachée *
		not.b	d0
		ror.b	#1,d0			d0=code raw clavier
		lea	table_clavier(pc),a0

		cmp	#$f9,d0			dernier code incorrect ?
		bne.s	.correct		-> non
		move	-4(a0),d0		d0=dernier code touche
		cmp	#128,d0
		blt.s	.corrige1
		sub	#128,d0
		bra.s	.touche_enfonce

.correct
		move	d0,-4(a0)		sauve code clavier
		cmp	#128,d0			touche enfoncée ?
		blt.s	.touche_enfonce		-> oui
		sub	#128,d0		
.corrige1	clr.b	(a0,d0)
		move	d0,code_cl
		tst	-2(a0)			nbre de touche enfoncée nul ?
		beq.s	.exit			-> oui
		subq	#1,-2(a0)		nbre de touche enfoncée - 1
		bra.s	.exit
.touche_enfonce	move.b	#1,(a0,d0)
		addq	#1,-2(a0)		nbre de touche enfoncée + 1
.exit		movem.l	(a7)+,d0/a0
.exit2		move	#$0008,CUSTOM+INTREQ
		rte
		dc	0			-4 dernier code touche clavier
		dc	0			-2 nbre de touche enfoncées
table_clavier:	dcb.b	128,0			 0 table des touches
***************************************************************************
*		FIN INTERRUPTION CLAVIER
***************************************************************************








;==============================================================================
;		-=- routines disques + reserve memoire -=-
;==============================================================================

load_sauve:	move.l	dosbase-tlk(a5),a6
		move.l	#50*2,d1
		jsr	-198(a6)		Delay
		move.l	4.w,a6
		jsr	-132(a6)
		lea	vecteur(pc),a0
		move.l	$6c.w,(a0)
		lea	vecteurzz(pc),a0
		move.l	$68.w,(a0)
		lea	$dff000,a6
		lea	save_intena(pc),a0
		move	INTENAR(a6),(a0)
		or	#$c000,(a0)+
		move	DMACONR(a6),(a0)
		or	#$8000,(a0)+
		move	ADKCONR(a6),(a0)
		or	#$8000,(a0)
		move	#$7fff,$9a(a6)
		move	#$7fff,$9c(a6)
		move	#$07ff,$96(a6)
		lea	error(pc),a2
		lea	vektor(pc),a1
		lea	8.w,a0
		moveq	#9,d0
.aubert		move.l	(a0),(a1)+
		move.l	a2,(a0)+
		dbf	d0,.aubert
		bsr	install_vecteurs
		rts
		

load_restore:
		lea	$dff000,a6
		move	#$7fff,$9a(a6)
		move	#$7fff,$9c(a6)
		move	#$07ff,$96(a6)
		lea	vektor(pc),a0
		lea	8.w,a1
		moveq	#9,d0
.hubert		move.l	(a0)+,(a1)+
		dbf	d0,.hubert
		move.l	vecteur(pc),$6c.w
		move.l	vecteurzz(pc),$68.w
		move	save_adkcon(pc),ADKCON(a6)
		move	save_dmacon(pc),DMACON(a6)
		move	save_intena(pc),INTENA(a6)
		move.l	gfxbase(pc),a1
		*move.l	38(a1),$80(a6)
		*move	d0,$88(a6)
		move.l	4.w,a6
		jsr	-138(a6)
		bsr	raz_table_clavier
		rts







********************************************
*	CHARGEMENT D'UN PROGRAMME AMIGADOS *
********************************************
* Entrée	=>	a0.l	nom du fichier		
*		=>	d0.l	adresse du buffer
*		=>	d1.l	nombre d'octets à charger
* Sortie	<=	d7.l	#-1 = erreur

load_file:	movem.l	d0-d6/a0-a6,-(a7)
		lea	.name(pc),a1
		move.l	a0,(a1)
		lea	.adresse(pc),a1
		move.l	d0,(a1)
		lea	.taille(pc),a1
		move.l	d1,(a1)
		
		lea	dosbase-tlk,a1
		add.l	a5,a1
		move.l	(a1),a6
		
		move.l	.name(pc),d1
		move.l	#1005,d2	mode_old	
		jsr	-30(a6)		ouvre le fichier
		lea	.handle(pc),a1
		move.l	d0,(a1)
		beq.s	.err

		move.l	.handle(pc),d1
		move.l	.adresse(pc),d2
		move.l	.taille(pc),d3
		jsr	-42(a6)		lit le fichier
		tst.l	d0
		bmi.s	.error
		
		move.l	.handle(pc),d1
		jsr	-36(a6)		ferme le fichier
		moveq	#0,d7
.exit		movem.l	(a7)+,d0-d6/a0-a6
		tst.l	d7
		rts
		
.error		move.l	.handle(pc),d1
		jsr	-36(a6)		ferme le fichier
.err		moveq	#-1,d7
		bra.s	.exit


.name		dc.l	0
.handle		dc.l	0
.adresse	dc.l	0
.taille		dc.l	0


*****************************************
*   SAUVEGARDE D'UN PROGRAMME AMIGADOS	*
*****************************************
* Entrée	=>	a0.l	nom du fichier		
*		=>	d0.l	adresse du buffer
*		=>	d1.l	nombre d'octets à sauver
* Sortie	<=	d7.l	#-1 = erreur

save_file:	movem.l	d0-d6/a0-a6,-(a7)
		lea	.name(pc),a1
		move.l	a0,(a1)
		lea	.adresse(pc),a1
		move.l	d0,(a1)
		lea	.taille(pc),a1
		move.l	d1,(a1)
		
		lea	dosbase-tlk,a1
		add.l	a5,a1
		move.l	(a1),a6
		
		move.l	.name(pc),d1
		move.l	#1006,d2	mode_new	
		jsr	-30(a6)		ouvre le fichier
		lea	.handle(pc),a1
		move.l	d0,(a1)
		beq.s	.err

		move.l	.handle(pc),d1
		move.l	.adresse(pc),d2
		move.l	.taille(pc),d3
		jsr	-48(a6)		sauve le fichier
		tst.l	d0
		bmi.s	.error

		move.l	.handle(pc),d1
		jsr	-36(a6)		ferme le fichier

		move.l	#50*5,d1	durée en 1/50 de seconde
		jsr	-198(a6)	|-> fonction Delay

		moveq	#0,d7
.exit		movem.l	(a7)+,d0-d6/a0-a6
		tst.l	d7
		rts
		
.error		move.l	.handle(pc),d1
		jsr	-36(a6)		ferme le fichier
.err		moveq	#-1,d7
		bra.s	.exit


.name		dc.l	0
.handle		dc.l	0
.adresse	dc.l	0
.taille		dc.l	0


************************* Efface la table clavier *************************
raz_table_clavier:
		lea	table_clavier(pc),a0
		moveq	#128/4-1,d0
		moveq	#0,d1
.loop		move.l	d1,(a0)+
		dbf	d0,.loop
		rts

;******************* Décompacte un fichier Powerpacké *********************
; Entrée	a0.l	Adr. fin fichier compacté 
;		a1.l	Adr. décompactage
;		a2.l	Adr. début fichier compacté 
; Sortie	d7.l	#-28 Fichier non powerpacké
decompacte_pp:	cmp.l	#'PP20',(a2)
		bne.s	.error
		movem.l	d0-d6/a0-a6,-(a7)
		bsr.s	.decompacte
		movem.l	(a7)+,d0-d6/a0-a6
		moveq	#0,d7
.exit		rts
.error		moveq	#-28,d7			¤Fichier non powerpacké
		bra.	.exit
.decompacte	;incbin	divers/powerpack.code






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


sex:
		movem.l	d2-d7/a0-a4,-(a7)
		lea	colormap,a4
		lea	2(a4),a3
		lea	COLOR00(a6),a1
		lea	BPLCON3(a6),a2
		moveq	#8-1,d4			8 palettes de 32 registres
		moveq	#0,d6
		move	sbplcon3(pc),d6		sélection bits 4 à 7 RVB
		move	d6,d7
		or	#$0200,d7		sélection bits 0 à 3 RVB

		* 4 bits de poids fort des 3 composantes *
.loop_color	moveq	#32-1,d3		32 regitres couleurs
		move.l	a1,a0			a0='COLOR00'
		move	d2,d5
		or	d6,d5			bits 4 à 7
		move	d5,(a2)			initialise 'BPLCON3'
.bits4a7	move	(a3),(a0)+
		addq.l	#4,a3
		dbf	d3,.bits4a7

		* 4 bits de poids faible des 3 composantes *
.cont		moveq	#32-1,d3		32 regitres couleurs
		move.l	a1,a0			a0='COLOR00'
		move	d2,d5
		or	d7,d5			bits 0 à 3
		move	d5,(a2)			initialise 'BPLCON3'
.bits0a3	move	(a4),(a0)+
		addq.l	#4,a4
		dbf	d3,.bits0a3

		add	#$2000,d2		prochaine palette
		dbf	d4,.loop_color
		movem.l	(a7)+,d2-d7/a0-a4
		rts


;==============================================================================
;		-=- Routines des FADE PALETTES -=-
;==============================================================================



;		-=- depuis le noir, put les bonnes teintes à la palette -=-
;		entree	a1	source
;			a0	destination

fad_on2:	moveq	#32-1,d1
fad_on2b	addq	#2,a0
.rot		move	(a0),d0
		and	#$f00,d0	garde le rouge
		move	(a1),d2
		and	#$f00,d2	garde le rouge
		cmp	d2,d0		meme rouge ?
		beq.s	.green		-> oui, occupe toi du vert
		add	#$100,(a0)

.green		move	(a0),d0
		and	#$0f0,d0	garde le vert
		move	(a1),d2
		and	#$0f0,d2	garde le vert
		cmp	d2,d0		meme vert ?
		beq.s	.red		-> oui, occupe toi du rouge
		add	#$010,(a0)

.red		move	(a0),d0
		and	#$00f,d0	garde le bleu
		move	(a1),d2
		and	#$00f,d2	garde le bleu
		cmp	d2,d0		meme bleu
		beq.s	.next		-> oui, passe au next registre
		addq	#$001,(a0)

.next		addq	#4,a0
		addq	#2,a1
		dbf	d1,.rot
		rts



;		-=- colorie une palette de 32 registres en noir -=-
;		entree a0

fad_noir:	addq	#2,a0

.rot		move	(a0),d0
		and	#$f00,d0	garde le rouge
		beq.s	.green		-> deja noir
		sub	#$100,(a0)

.green		move	(a0),d0
		and	#$0f0,d0	garde le vert
		beq.s	.red		-> oui, occupe toi du rouge
		sub	#$010,(a0)

.red		move	(a0),d0
		and	#$00f,d0	garde le rouge
		beq.s	.next		-> oui, passe au next registre
		subq	#$001,(a0)
.next		addq	#4,a0
		dbf	d1,.rot
		rts



;==============================================================================
;		-=- Fin des routines des FADE PALETTES -=-
;==============================================================================


* /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  *
*/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/*
********************>* Décompacte une image IFF ILBM *<********************
* /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  *
*/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/*
* Entrée	=> a2.l	adresse table couleur / 0 si pas de table couleur
*		=> a3.l	adresse décompacte    / 0 si pas de décompactage
*		=> a4.l	adresse fichier IFF
* Sortie	<= d0.w	largeur de l'image en pixels
*		<= d1.w	hauteur de l'image en lignes
*		<= d2.l	mot bas nombre de plan de l'image/mot haut flag HAM
*		<= d3.l	taille de l'image en octet
*		<= d4.w	format de compactage
*		<= d5.l	mot faible hauteur écran/mort fort largeur écran pixel
* 		<= d7.l	¤00	pas d'erreur
*			¤-2	fichier non IFF
*			¤-3	fichier non ILBM
*			¤-4	chunk non trouvé
*			¤-5	format compression inconnue
decompacte_ilbm:
		bsr	.test_FORM
		bne.s	.exit
		bsr.s	.save_sizeIFF
		bsr.s	.test_ILBM
		bne.s	.exit
		bsr	.decode_CMAP
		bne.s	.exit
		bsr	.decode_BMHD
		bne.s	.exit
		movem.l	d0-d5,-(a7)
		bsr	.decode_BODY
		movem.l	(a7)+,d0-d5
.exit		rts

*----------------* DEC IFF : test si le fichier est IFF *-----------------*
* entrée	=> a4.l	adr. fichier IFF
.test_FORM:	
		cmp.l	#'FORM',(a4)+
		bne.s	.no_IFF
		moveq	#0,d7
		rts
.no_IFF		moveq	#-2,d7			¤fichier non IFF
		rts

*----------------* DEC IFF : test si le fichier est ILBM *----------------*
* entrée	=> a4.l	adr. fichier IFF
.test_ILBM:	
		cmp.l	#'ILBM',(a4)+		fichier graphique ? 
		bne.s	.no_ILBM		-> non
		moveq	#0,d7
		rts
.no_ILBM	moveq	#-3,d7			¤fichier non ILBM
		rts

*--------------* DEC IFF : sauve la taille du fichier IFF *---------------*
* entrée	=> a4.l	adr. fichier IFF
.save_sizeIFF:	
		move.l	(a4)+,d6
		rts

*---------------------* DEC IFF : cherche un chunk *----------------------*
* entrée	=> d1.l	nom du chunk
*		=> d6.l	taille fichier IFF
*		=> a4.l	adr. fichier IFF
* sortie	<= a0.l	pt/nom du chunk
* utilise	d1/d2/d6/d7/a0/a1/a4
.cherche_chunk:
		move.l	a4,a0			début du fichier
		lea	(a4,d6.l),a1		fin du fichier
.chunk_loop	cmp.l	(a0),d1			identificateur ?
		beq.s	.chunk_found		oui, ok
		addq.l	#2,a0			prochain long mot
		cmp.l	a0,a1			fin du fichier ?
		bge.s	.chunk_loop		-> non
		moveq	#-4,d7			¤chunk non trouvé
		rts
.chunk_found	moveq	#0,d7
		rts

*-------------------* DEC IFF : décode le chunk CMAP *--------------------*
* entrée	d6.l	taille fichier IFF
*		a2.l	adr. table couleur
*		a4.l	adr. fichier IFF
* sortie	<= d0.l	taille du chunk 'CMAP'
.decode_CMAP:
		move.l	#'CMAP',d1
		bsr.s	.cherche_chunk		|-> cherche chunk CMAP
		bne	.CMAP_exit
		addq	#4,a0			saute le nom du chunk
		move.l	(a0)+,d0		taille chunk 'CMAP'
		move.l	a2,d1
		tst.l	d1			table couleur ?
		beq.s	.CMAP_exit		-> non
		move.l	d0,-(a7)		sauve taille chunk 'CMAP' (pour le test HAM8)
		moveq	#3,d5			couleur sur 24 bits (3 octets)
.CMAP_loop	clr.b	(a2)+			4 bits inutilisés
		move.b	(a0)+,(a2)+		valeur du rouge
		move.b	(a0)+,(a2)+	 	valeur du vert
		move.b	(a0)+,(a2)+ 		valeur du bleu
		movem.l	d0-d2,-(a7)
		move.l	-4(a2),d2
		bsr	aga_color
		move.w	d0,-4(a2)
		move.w	d1,-2(a2)
		movem.l	(a7)+,d0-d2
		sub.l	d5,d0
		bne.s	.CMAP_loop
		move.l	(a7)+,d0		restitue taille chunk 'CMAP' (pour le test HAM8)
		moveq	#0,d7
.CMAP_exit:	rts

*-------------------* DEC IFF : analyse le chunk BMHD *-------------------*
* entrée	=> d0.l	taille chunk 'CMAP'
*		=> d6.l	taille fichier IFF
*		=> a4.l	adr. fichier IFF
* sortie	<= d0.w	largeur de l'image en pixel
*		<= d1.w	hauteur de l'image en pixel
*		<= d2.l	mot bas nombre de plan de l'image/mot haut flag HAM
*		<= d3.l	taille de l'image en octet
*		<= d4.w	format de compactage
*		<= d5.l	mot faible hauteur écran/mort fort largeur écran pixel
*		<= d7.l	0 = pas d'erreur
.decode_BMHD:
		move.l	#'BMHD',d1
		bsr.s	.cherche_chunk		|-> cherche chunk BMHD
		bne	.BMHD_exit
		addq.l	#8,a0			saute nom & taille chunk
		moveq	#0,d1
		moveq	#0,d2
		moveq	#0,d3
		move.l	d0,d4			d4=taille chunk 'CMAP' (pour le test HAM8)
		moveq	#0,d0
		move.w	(a0)+,d0		d0=largeur image en pixels
		move.w	(a0)+,d1		d1=hauteur image en pixels
		addq.l	#4,a0			saute la position écran
		move.b	(a0)+,d2		d2=nombre de plans
		bsr	.test_si_HAM8
		addq.l	#1,a0			saute type de masque
		moveq	#0,d4
		move.b	(a0)+,d4		d4=type de compression
		addq.l	#5,a0			saute certaines informations
		move.w	(a0)+,d5		d5=largeur fenêtre écran en pixels
		swap	d5
		move.w	(a0)+,d5		d5=hauteur fenêtre écran en pixels
		move.w	d0,d3			d3=largeur image en pixels
		lsr.w	#3,d0			d0=largeur image en octets
		and.w	#$FFFE,d0		octet paire
		and.w	#$000F,d3		largeur multiple de 16 ?
		beq.s	.paire			-> oui		
		addq.w	#2,d0			non, rajoute un mot de 16 bits
.paire		move.w	d0,d3			d3=largeur image en octets
		lsl.w	#3,d0			d0=Largeur image en pixels
		mulu.w	d2,d3			d3=taille image decompactée
		mulu	d1,d3			d3=taille plan en raw normal
		cmp.w	#1,d4
		bhi.s	.type_compress
		moveq	#0,d7
.BMHD_exit:	rts
.type_compress	moveq	#-5,d7			¤format compression inconnue
		rts

*---------------------* DEC IFF : Test si mode HAM *----------------------*
* entrée	=> d2.w	Nombre de plan de 1 à 6
*		=> d4.l	Taille du chunk 'CMAP'
* sortie	<= d2.l	Mot bas nbre de plan/mot haut flag HAM
.test_si_HAM8
		cmp.w	#8,d2			8 plans ?
		bne.s	.pas_HAM8		-> Non
		cmp.l	#$300,d4		256 Registres couleurs ?
		beq.s	.pas_HAM8		-> Oui
		swap	d2
		move.w	#1,d2			d2=Flag mode HAM
		swap	d2
.pas_HAM8	rts

*-----------------* DEC IFF : décompacte le chunk BODY *------------------*
* entrée	=> d4.w	format de compactage
*		=> d3.l	taille image décompacté
*		=> d6.l	taille fichier IFF
*		=> a3.l	adr. décompactage / 0 si pas de décompactage
*		=> a4.l	adr. fichier IFF
.decode_BODY:
		move.l	#'BODY',d1
		bsr	.cherche_chunk		|-> cherche chunk BODY
		bne	.BODY_exit
		addq.l	#4,a0			saute nom du chunk
		move.l	a3,d0
		tst.l	d0			décompactage ?
		beq.s	.BODY_exit		-> non
		move.l	a3,a1			a1=adr. page graphique
		addq.l	#4,a0
		tst	d4			fichier compacté ByteRun1 ?
		beq	.nocrunch		-> non

		* routine testant la fin du chunk BODY *
* entrée	=> d3.l	taille page graphique
*		=> a0.l	adr. premier octet compacté
*		=> a1.l	adr. premier octet page graphique
.decomp_loop	moveq	#0,d1
.read		move.b	(a0)+,d1		octet de commande
		cmp.b	#$80,d1			code nul ?
		beq.s	.read			-> oui
		tst.b	d1
		bmi.s	.duplication
.copie_loop	move.b	(a0)+,(a1)+
		subq.l	#1,d3
		beq.s	.stop
		dbf	d1,.copie_loop
		bra.s	.decomp_loop
.duplication	neg.b	d1
		move.b	(a0)+,d6		d6=octet à dupliquer
.dupli_loop	move.b	d6,(a1)+
		subq.l	#1,d3
		beq.s	.stop
		dbf	d1,.dupli_loop
		bra.s	.decomp_loop
.stop		bra.s	.BODY_exit		-> fin décompactage

		* fichier non compacté *
.nocrunch	move.b	(a0)+,(a1)+
		subq.l	#1,d3
		bne.s	.nocrunch		-> non
.BODY_exit	rts

*==============================================================================
;		-=- début des variables, tables, buffer, etc -=-
;==============================================================================


col_boule	dc	$000,$fff,$ddc,$bba,$887,$665,$554,$443
		dc	$332,$221,$fff,$bcd,$9ab,$789,$678,$456
		dc	$234,$123,$fc6,$763,$f74,$732,$fb9,$f0f
		dc	$d0f,$b0f,$90f,$60f,$40f,$20f,$00f,$ccf



vbl_flag	dc	0		-22	compteur de VBL
		dc.l	0		-20	vide
		dc.l	mult40-tlk	-16	adr table *40
		dc.l	mult200-tlk	-12	adr table *200
		dc.l	0		-08	adr buffer txt
		dc.l	0		-04	adr ecran (réservation mém.)
tlk:		dc	0		00	dbuffer

sortie		dc	0
go_irq		dc	0		pointe sur la bonne irq
next_irq	dc	0
code_cl		dc	0		code clavier
sbplcon3	dc	$0004
toto		dc	0

ax		dc.b	0
ay		dc.b	0
mouse		dc	0,0
bt1_mouse	dc	0


color_noir	dc	0
color_okay	dc	0
cycle		dc	0
		even



textintro	dc.l	46*30*5+2
		dc.b	07,'THE ULTIMATE GAMES FACTORY',1,0,1
		dc.b	10,'BY DJI IN SUMMER 94',1,0,1,0,1,0,1
		dc.b	08,'F1   LECTURE DE DECOR GFX',1
		dc.b	08,'F2   LECTURE DE DECOR MAP',1
		dc.b	08,'F3   SAUVEGARDE DE DECOR MAP',1
		dc.b	08,'F4   CREATION DE DECOR MAP',1
		dc.b	08,'F5   LECTURE DE BOB GFX',1
		dc.b	08,'F6   ',1
		dc.b	08,'F7   ',1
		dc.b	08,'F8   ',1
		dc.b	08,'F9   ',1
		dc.b	08,'F10  '
		dc.b	0
		even


saut_irq:	dc.l	init_presentation-tlk	00
		dc.l	presentation-tlk       	04
		dc.l	init_createmap-tlk     	08
		dc.l	createmap-tlk     	12
saut_irqfin


mult40		ds	ECRAN_HIGH
mult200		ds.l	ECRAN_HIGH





version		dc.b	'$VER: Casimir VS Albator 03/08/94',0
		EVEN




*=========================================================================*
**************************** Section graphique ****************************
*=========================================================================*
		SECTION		DATACHIP,DATA_C
		CNOP 0,8		* on aligne sur 64 bits

		* Liste copper *

copper:		;dc.l	$01800000
		dc.l	$00e00000,$00e20000
		dc.l	$00e40000,$00e60000
		dc.l	$00e80000,$00ea0000
		dc.l	$00ec0000,$00ee0000
		dc.l	$00f00000,$00f20000
		dc.l	$00f40000,$00f60000
		dc.l	$00f80000,$00fa0000
		dc.l	$00fc0000,$00fe0000
		;dc.l	$01800000
		dc.l	$8001fffe
		dc.l	$01000201
copperbis	dc.l	$00e00000,$00e20000
		dc.l	$00e40000,$00e60000
		dc.l	$00e80000,$00ea0000
		dc.l	$00ec0000,$00ee0000
		dc.l	$00f00000,$00f20000
		dc.l	$00f40000,$00f60000
		dc.l	$00f80000,$00fa0000
		dc.l	$00fc0000,$00fe0000
		dc.l	$8101fffe,$01000211

		dc.l	$01FC000F
		dc.l	$fffffffe		fin de la liste copper


		CNOP 0,8		* on aligne sur 64 bits


*colormap:	incbin	"DF0:Pictures/Page.RAW"
*colormap:	incbin	"Dh0:data/ilbm/Page.RAW"
colormap:	dcb.b	82944,0
fichier		=colormap+256*4

madness		incbin	"dh0:data/ilbm/pic.ilbm"
*madness		incbin	"dh0:data/ilbm/madness.iff"

fonts2		;incbin	pictures/pacfonts2.rawb
pupitre		;incbin	pictures/pupitre.rawb
		END

