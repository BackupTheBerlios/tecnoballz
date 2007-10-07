***************************************************************************
                        */ Convertit les modules */
***************************************************************************
		OPT	P=68020
		SECTION	ETB,CODE
***************************************************************************
start:		lea	(t,pc),a5
		lea	module,a6
		bsr	init1
		bsr	xx.sample		|-> En tête samples
		bsr	xx.pos			|-> Table des positions
		bsr	xx.sample2		|-> Samples
		bsr	xx.pattern
		rts
***************************************************************************

*----------------------->  Recopie les patterns <-------------------------*
xx.pattern:
		lea	(1084,a6),a1
		move.l	(np_tpatt-t,a5),a2
		move.l	(np_fpatt-t,a5),a4
		move.w	(hpos-t,a5),d7		d7=nombre de Patterns
		subq.w	#1,d7

.loop1		moveq	#3,d6			4 cannaux
.loopy		move.l	a4,a0
		add.w	(a2)+,a0
		moveq	#63,d5			64 lignes par Pattern
.loop2		move.b	(a0)+,d1		d1 = 1er octet de la note
		bpl.s	.nopause		-> positif pas de pause
		ext	d1
.pause		clr.l	(a1)
		lea	(16,a1),a1
		addq.w	#1,d1
		bpl	.next
		dbf	d5,.pause
		bra	.next2
.nopause:	move.b	(a0)+,d3		d3 = numéro commande
		move.b	(a0)+,d4		d4 = paramètre commande
		*/ SAMPLE */
		move.b	d1,d2			d2 = 1er octet de la note
		lsl.w	#8,d2			d2  = bit n° 4 du n° sample
		or.b	d3,d2			+d3 = + bit 0 à 3 du n° du sample
		and.w	#$01F0,d2		élimine bits indésirables
		or.b	d2,(2,a1)
		and.w	#$0F00,d2
		lsr.w	#4,d2
		or.b	d2,(a1)
		*/ PERIODE */
		and.w	#$FE,d1			 pt/table période
		move.w	(np_per1,pc,d1),d1
		or.w	d1,(a1)

		*/ COMMANDE */
		and.w	#$f,d3
		cmp.w	#8,d3			APERGIO ?
		bne.s	.noa
		moveq	#0,d3
		bra	.cmd

.noa		cmp.w	#$B,d3			JMP
		bne.s	.noj
		lsr.b	#1,d4
		addq.b	#1,d4
		bra	.cmd

.noj		;cmp.w	#$E,d3			E
		;bne.s	.noe
		;lsr.b	#1,d4
		;bra	.cmd

.noe		cmp.w	#$A,d3			Volume Slide 
		beq.s	.vs
.nov		cmp.w	#6,d3			Vibra
		beq.s	.vs
		cmp.w	#5,d3			Tone	
		bne.s	.not
.vs		tst.b	d4
		bmi.s	.neg
		lsl.b	#4,d4
		bra	.cmd
.neg		neg.b	d4
.not
.cmd		or.b	d3,(2,a1)
		or.b	d4,(3,a1)
		lea	(16,a1),a1
.next		dbf	d5,.loop2
.next2		lea	(-1020,a1),a1
		dbf	d6,.loopy
		lea	(1008,a1),a1
		dbf	d7,.loop1

		rts


*------------------------> Recopie les samples <--------------------------*
xx.sample2
		lea	(np_sample,pc),a0
		move.w	(hpos-t,a5),d0
		lsl.w	#8,d0
		lsl.w	#2,d0
		add.l	#1084,d0
		add.l	a6,d0
		move.l	d0,a1
		move.l	a1,(mod_sample-t,a5)
		moveq	#30,d1
.loop		move.l	(a0)+,d0
		beq.s	.exit
		move.l	d0,a2
		move.w	(a0),d0
		subq.w	#1,d0
.loop2		move.w	(a2)+,(a1)+
		dbf	d0,.loop2
		addq.l	#8,a0
		dbf	d1,.loop
.exit		move.l	a1,(np_end-t,a5)
		rts

*-------------------> Complète la table des positions <-------------------*
xx.pos:
		move.l	(np_tapos-t,a5),a0
		lea	position,a1
		move.w	(-2,a0),d2
		lsr.w	#1,d2
		move.b	d2,(-1,a1)
		move.w	(-4,a0),d2
		lsr.w	#1,d2
		move.b	d2,(-2,a1)
		subq.w	#1,d2
		moveq	#0,d1
.loop		move.w	(a0)+,d0
		lsr.w	#3,d0
		move.b	d0,(a1)+
		cmp.w	d0,d1
		bhi.s	.ok
		move.w	d0,d1
.ok		dbf	d2,.loop
		addq.w	#1,d1
		move.w	d1,(hpos-t,a5)
		rts

*--------------------->> Complète la liste sample <<----------------------*
xx.sample:
		lea	(np_sample,pc),a0
		lea	sample+22,a1
		moveq	#30,d0
.loop		addq.l	#4,a0
		move.l	(a0)+,(a1)+		Taille en mots & Volume
		move.l	(a0)+,(a1)+		Start Repeat & End Repeat
		lea	(22,a1),a1
		dbf	d0,.loop
		rts

*--------------> Sauve quelques adresses du module source <---------------*
init1:		
		move.l	a5,a4			a4=module propacked
		move.l	a4,a3
		add.w	(a4)+,a3
		move.l	a3,(np_tapos-t,a5)	Table positions
		add.w	(a4)+,a3
		move.l	a3,(np_tpatt-t,a5)	Table Patterns
		add.w	(a4)+,a3
		move.l	a3,(np_fpatt-t,a5)	Premier Pattern
		*/ Calcul adresses des 31 samples */
		add.w	(a4)+,a3		adr du 1er sample
		move.l	a4,a0
		add.w	(-8,a4),a0
		lea	(-12,a0),a0
		lea	(np_sample,pc),a1
.np_init	move.l	a3,(a1)+		adresse du sample
		move.w	(6,a4),d0
		move.w	d0,(a1)+		taille du sample en mots
		move.w	(a4),(a1)+		Volume
		move.w	(14,a4),(a1)+		Offset début Repeat en mots
		move.w	(12,a4),(a1)+		Longeur du Repeat en mots
		add.w	d0,d0			convertit en octet
		add.w	d0,a3			adr. du sample suivant
		lea	(16,a4),a4		adr. structure sample suivante
		cmp.l	a4,a0			reste t-il des structures samples ?
		bhi.s	.np_init		-> oui
		rts	

		CNOP	0,4
np_tapos	dc.l	0
np_tpatt	dc.l	0
np_fpatt	dc.l	0
np_sample	ds.b	31*12
mod_sample	dc.l	0
np_end		dc.l	0			Fin du module
hpos		dc.w	0			N° du Pattern le + élevé


;------------------- PLAYROUTINE : table des périodes ---------------------
np_per1:	dc.w	0
		dc	$0358			02 C1	 261,7 hz °
		dc	$0328			04 C#1	 277,2 hz
		dc	$02fa			06 D1	 293,7 hz
		dc	$02d0			08 D#1	 311,2 hz
		dc	$02a6			10 E1	 329,7 hz
		dc	$0280			12 F1	 349,3 hz
		dc	$025c			14 F#1	 370,0 hz
		dc	$023a			16 G1	 392,0 hz
		dc	$021a			18 G#1	 415,3 hz
		dc	$01fc			20 A1	 440,0 hz
		dc	$01e0			22 A#1	 466,2 hz
		dc	$01c5			24 B1	 493,9 hz
		dc	$01ac			26 C2	 523,4 hz °
		dc	$0194			28 C#2	 554,4 hz
		dc	$017d			30 D2	 587,4 hz
		dc	$0168			32 D#2	 622,4 hz
		dc	$0153			34 E2	 659,4 hz
		dc	$0140			36 F2	 698,6 hz
		dc	$012e			38 F#2	 740,0 hz
		dc	$011d			40 G2	 784,0 hz
		dc	$010d			42 G#2	 830,6 hz
		dc	$00fe			44 A2	 880,0 hz
		dc	$00f0			46 A#2	 932,4 hz
		dc	$00e2			48 B2	 987,8 hz
		dc	$00d6			50 C3	1046,8 hz ° 
		dc	$00ca			52 C#3	1108,8 hz
		dc	$00be			54 D3	1174,8 hz
		dc	$00b4			56 D#3	1244,8 hz
		dc	$00aa			58 E3	1318,8 hz
		dc	$00a0			60 F3	1397,2 hz
		dc	$0097			62 F#3	1480,0 hz
		dc	$008f			64 G3	1568.0 hz
		dc	$0087			66 G#3	1661,2 hz
		dc	$007f			68 A3	1760,0 hz
		dc	$0078			70 A#3	1864,8 hz
		dc	$0071			72 B3	1975,6 HZ

t:		INCBIN	DH1:Tempo/Data/TCC_Data/Disque/d
*t:		INCBIN	DH1:Tempo/Data/TCC_Data/Disque/x	45270
		* b/c/d/e/f/h/i
module		
title		dc.b	"UNE CHANSON DOUCE 1 "
sample		dc.b	"MUSIC BY REG & ZBB 01 ........"
		dc.b	"MUSIC BY REG & ZBB 02 ........"
		dc.b	"MUSIC BY REG & ZBB 03 ........"
		dc.b	"MUSIC BY REG & ZBB 04 ........"
		dc.b	"MUSIC BY REG & ZBB 05 ........"
		dc.b	"MUSIC BY REG & ZBB 06 ........"
		dc.b	"MUSIC BY REG & ZBB 07 ........"
		dc.b	"MUSIC BY REG & ZBB 08 ........"
		dc.b	"MUSIC BY REG & ZBB 09 ........"
		dc.b	"MUSIC BY REG & ZBB 10 ........"
		dc.b	"MUSIC BY REG & ZBB 11 ........"
		dc.b	"MUSIC BY REG & ZBB 12 ........"
		dc.b	"MUSIC BY REG & ZBB 13 ........"
		dc.b	"MUSIC BY REG & ZBB 14 ........"
		dc.b	"MUSIC BY REG & ZBB 15 ........"
		dc.b	"MUSIC BY REG & ZBB 16 ........"
		dc.b	"MUSIC BY REG & ZBB 17 ........"
		dc.b	"MUSIC BY REG & ZBB 18 ........"
		dc.b	"MUSIC BY REG & ZBB 19 ........"
		dc.b	"MUSIC BY REG & ZBB 20 ........"
		dc.b	"MUSIC BY REG & ZBB 21 ........"
		dc.b	"MUSIC BY REG & ZBB 22 ........"
		dc.b	"MUSIC BY REG & ZBB 23 ........"
		dc.b	"MUSIC BY REG & ZBB 24 ........"
		dc.b	"MUSIC BY REG & ZBB 25 ........"
		dc.b	"MUSIC BY REG & ZBB 26 ........"
		dc.b	"MUSIC BY REG & ZBB 27 ........"
		dc.b	"MUSIC BY REG & ZBB 28 ........"
		dc.b	"MUSIC BY REG & ZBB 29 ........"
		dc.b	"MUSIC BY REG & ZBB 30 ........"
		dc.b	"MUSIC BY REG & ZBB 31 ........"
		dc.b	$00
		dc.b	$7F
position	ds.b	128
		dc.l	"M.K."
pattern		ds.b	200000
m2
		END

*/ Modules Propacked */--------------------------------------------------*
* M+0000	p1 =Pointe sur M+p1  table position
* M+0002	p2 =Pointe sur la table des patterns M+p1+p2
* M+0004	p3 =Pointe sur les patterns M+p1+p2+p3
* M+0006	p4 =Pointe sur les échantillions M+p1+p2+p3+p4
* M+0008	Table des samples (16 octets par sample)
*		N+00	Volume
*		N+02	Adresse du sample
*		N+06	Taille du sample en mots
*		N+08	Adresse Sample du Repeat
*		N+12	Longeur du Repeat en mots
*		N+14	Offset début du Repeat en mots	
*/ Modules Protracker */--------------------------------------------------*
* M+0000	Nom du module
* M+0020	Nom de l'échantillion 1
* M+0042	Taille de l'échantillion 1 en mots
* M+0044	Volume de l'échantillion 1
* M+0046	Début de la répétition de l'échantillion 1
* M+0048	Fin de la répétition de l'échantillion 1
* M+0050	Nom de l'échantillion 2
etc...
* M+0920	Nom de l'échantillion 30
* M+0942	Taille de l'échantillion 30 en mots
* M+0944	Volume de l'échantillion 30
* M+0946	Début de la répétition de l'échantillion 30
* M+0948	Fin de la répétition de l'échantillion 30
* M+0950	Nombre de positions ?
* M+0951	Nombre maximum de positions ?
* M+0952	Orde des Patterns

* M+1080	"M.K."
* M+1084	Patterns (1024 octets pour un pattern)

*			0   1  2  3  
*		dc.l	$AB-CD-EF-GH
*		AE  = N° de l'échantillion	1 à 31
*		BCD = Période
*		F   = Commande
*		GH  = Paramètre de la commande

* M+1084+(Numéro_du_pattern_le_plus_élévé+1)*1024
		Echantillion 1

		AB-CD-EF
		AB = Période & $FFFE
		BC = N° du sample
		D  = N° de la commande
		EF = Paramètre

*		8=0

