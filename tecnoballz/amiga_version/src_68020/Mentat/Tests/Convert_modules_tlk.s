***************************************************************************
                        */ Convertit les modules */
***************************************************************************
		OPT	P=68020
		SECTION	ETB,CODE
***************************************************************************
start:		lea	t,a5
		lea	module,a6
		move.l	a5,a0
		lea	(42,a6),a1
		moveq	#30,d0
.loop		move.w	(124,a0),(a1)+	longeur
		move.w	(186,a0),(a1)+	volume
		move.w	(248,a0),(a1)+	replen start
		move.w	(310,a0),(a1)+	replen end
		addq.l	#2,a0
		lea	(22,a1),a1
		dbf	d0,.loop
		lea	(372,a5),a0
		lea	(950,a6),a1
		move.w	#129,d0
.loop2		move.b	(a0)+,(a1)+
		dbf	d0,.loop2
	
		lea	(952,a6),a1
		moveq	#127,d0
		moveq	#0,d1
.mtloop:	move.l	d1,d2
		subq.w	#1,d0
.mtloop2:	move.b	(a1)+,d1
		cmp.b	d2,d1
		bgt.s	.mtloop
		dbf	d0,.mtloop2
		move.w	d2,d0
		addq.b	#1,d2
		asl.l	#8,d2
		asl.l	#2,d2
		add.l	#1084,d2
		add.l	a6,d2
		move.l	d2,a2

		lea	(502,a5),a0
		lea	(1084,a6),a1
.boucle		moveq	#63,d2
.cont		bsr	kopy
		bsr	kopy
		bsr	kopy
		bsr	kopy
		dbf	d2,.cont
		dbf	d0,.boucle

		move.l	a5,a3
		lea	(124,a5),a4
		moveq	#30,d1
.ya		move.l	(a3)+,a0		adresse
		add.l	a5,a0
		move.w	(a4)+,d0
		beq.s	.snext
		subq.w	#1,d0
.sample		move.w	(a0)+,(a2)+
		dbf	d0,.sample
.snext		dbf	d1,.ya
		rts

kopy		move.b	(2,a0),d1
		and.b	#$0F,d1
		sub.b	#$0E,d1
		beq.s	.ok
		move.l	(a0)+,(a1)+
		rts
.ok		addq.l	#4,a0
		clr.l	(a1)+
		rts

t:		INCBIN	DH1:Tempo/modtlk.intro
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

