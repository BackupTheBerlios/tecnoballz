
		OPT	C24+		labels et symboles 24 caractères maxi.,  majuscules/minuscules respectées
		OPT	O-		pas d'optimisation
		OPT P=68020

*	60 fichiers = 77280 (dont 480 octets couleurs & 76800 de gfx)
*	60 fonts de 160*32*2 = 1280 octets
*	2 par ligne 320 pixels
*	32*30=960 lignes	= 76800 octets

start:		lea	(iff.body.data,pc),a6
		lea	f00,a0		
		lea	(f01-f00,a0),a1
		lea	(color,pc),a5
		moveq	#0,d2
		moveq	#30-1,d1		2560*30=76800
loopy		bsr	kolor
		lea	(20*32,a0),a2		pt/plan 2
		lea	(20*32,a1),a3		pt.plan2
		moveq	#32-1,d0		40*32*2=2560
loopx		move.l	(a0)+,(a6)+		
		move.l	(a0)+,(a6)+
		move.l	(a0)+,(a6)+
		move.l	(a0)+,(a6)+
		move.l	(a0)+,(a6)+
		move.l	(a1)+,(a6)+
		move.l	(a1)+,(a6)+
		move.l	(a1)+,(a6)+
		move.l	(a1)+,(a6)+
		move.l	(a1)+,(a6)+
		move.l	(a2)+,(a6)+		
		move.l	(a2)+,(a6)+
		move.l	(a2)+,(a6)+
		move.l	(a2)+,(a6)+
		move.l	(a2)+,(a6)+
		move.l	(a3)+,(a6)+
		move.l	(a3)+,(a6)+
		move.l	(a3)+,(a6)+
		move.l	(a3)+,(a6)+
		move.l	(a3)+,(a6)+
		dbf	d0,loopx
		lea	(f01-f00,a2),a0
		lea	(f01-f00,a3),a1
		dbf	d1,loopy
li:		rts

kolor		move.w	(a0)+,d2
		bsr.s	.coco
		move.w	(a0)+,d2
		bsr.s	.coco
		move.w	(a0)+,d2
		bsr.s	.coco
		move.w	(a0)+,d2
		bsr.s	.coco
		move.w	(a1)+,d2
		bsr.s	.coco
		move.w	(a1)+,d2
		bsr.s	.coco
		move.w	(a1)+,d2
		bsr.s	.coco
		move.w	(a1)+,d2
		*$00000XYZ= $00X0Y0Z0
.coco		move.l	d2,d3
		and.w	#$F00,d3
		lsl.l	#8,d3
		lsl.l	#4,d3
		move.w	d2,d3
		and.w	#$0F0,d3
		lsl.w	#8,d3
		and.w	#$F,d2
		lsl.w	#4,d2
		or.w	d2,d3
		move.l	d3,(a5)+
		rts	
*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
color		ds.l	60*4
color.end
*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
iff:		dc.l	"FORM"
		dc.l	iff.end-iff-8	taille du fichier - 8 octets
		dc.l	"ILBM"
		dc.l	"BMHD"
		dc.l	$14		taile du chunk BMHD
		dc.w	320	ø	largeur de la page
		dc.w	960	ø	hauteur	de la page
		dc.l	0
		dc.b	2	ø	nombre de plan(s) (1 à 8)
		dc.b	0		type de masque
		dc.b	0		pas de compression (0=aucune)
		dc.b	0
		dc.l	$00002C2C
		dc.w	320	ø	Largeur fenêtre écran
		dc.w	256	ø	Hauteur fenêtre écran
		* CHUNK CMAP * 
		dc.l	"CMAP"
		dc.l	12	ø
		dc.b	$40,$20,$40
		dc.b	$60,$40,$60
		dc.b	$80,$60,$80
		dc.b	$A0,$80,$A0
		* CHUNK BODY *
		dc.l	"BODY"
		dc.l	40*960*2
iff.body.data:	ds.b	40*960*2
iff.end:

*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
		* AREA 1 *
f00:		incbin	dh1:Tempo/Fonts/FA01
f01:		incbin	dh1:Tempo/Fonts/FA02
		incbin	dh1:Tempo/Fonts/FA03
		incbin	dh1:Tempo/Fonts/FA04
		incbin	dh1:Tempo/Fonts/FA05
		incbin	dh1:Tempo/Fonts/FA06
		incbin	dh1:Tempo/Fonts/FA07
		incbin	dh1:Tempo/Fonts/FA08
		incbin	dh1:Tempo/Fonts/FA09
		incbin	dh1:Tempo/Fonts/FA10
		incbin	dh1:Tempo/Fonts/FA11
		incbin	dh1:Tempo/Fonts/FA12

		* AREA 2 *
		incbin	dh1:Tempo/Fonts/FB01
		incbin	dh1:Tempo/Fonts/FB02
		incbin	dh1:Tempo/Fonts/FB03
		incbin	dh1:Tempo/Fonts/FB04
		incbin	dh1:Tempo/Fonts/FB05
		incbin	dh1:Tempo/Fonts/FB06
		incbin	dh1:Tempo/Fonts/FB07
		incbin	dh1:Tempo/Fonts/FB08
		incbin	dh1:Tempo/Fonts/FB09
		incbin	dh1:Tempo/Fonts/FB10
		incbin	dh1:Tempo/Fonts/FB11
		incbin	dh1:Tempo/Fonts/FB12

		* AREA 3 *
		incbin	dh1:Tempo/Fonts/FC01
		incbin	dh1:Tempo/Fonts/FC02
		incbin	dh1:Tempo/Fonts/FC03
		incbin	dh1:Tempo/Fonts/FC04
		incbin	dh1:Tempo/Fonts/FC05
		incbin	dh1:Tempo/Fonts/FC06
		incbin	dh1:Tempo/Fonts/FC07
		incbin	dh1:Tempo/Fonts/FC08
		incbin	dh1:Tempo/Fonts/FC09
		incbin	dh1:Tempo/Fonts/FC10
		incbin	dh1:Tempo/Fonts/FC11
		incbin	dh1:Tempo/Fonts/FC12

		* AREA 4 *
		incbin	dh1:Tempo/Fonts/FD01
		incbin	dh1:Tempo/Fonts/FD02
		incbin	dh1:Tempo/Fonts/FD03
		incbin	dh1:Tempo/Fonts/FD04
		incbin	dh1:Tempo/Fonts/FD05
		incbin	dh1:Tempo/Fonts/FD06
		incbin	dh1:Tempo/Fonts/FD07
		incbin	dh1:Tempo/Fonts/FD08
		incbin	dh1:Tempo/Fonts/FD09
		incbin	dh1:Tempo/Fonts/FD10
		incbin	dh1:Tempo/Fonts/FD11
		incbin	dh1:Tempo/Fonts/FD12

		* AREA 5 *
		incbin	dh1:Tempo/Fonts/FE01
		incbin	dh1:Tempo/Fonts/FE02
		incbin	dh1:Tempo/Fonts/FE03
		incbin	dh1:Tempo/Fonts/FE04
		incbin	dh1:Tempo/Fonts/FE05
		incbin	dh1:Tempo/Fonts/FE06
		incbin	dh1:Tempo/Fonts/FE07
		incbin	dh1:Tempo/Fonts/FE08
		incbin	dh1:Tempo/Fonts/FE09
		incbin	dh1:Tempo/Fonts/FE10
		incbin	dh1:Tempo/Fonts/FE11
zozo		incbin	dh1:Tempo/Fonts/FE12
f99:
*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
