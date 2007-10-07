* CORRIGE CODE BRIQUES *
		OPT	P=68020
tplanb		=14
nplanb		=05
tligneb		=tplanb*nplanb
nligneb		=63
ll		=tligneb*7
start:		lea	(noza,pc),a1
		lea	(yorkais,pc),a2
		move.l	a2,d1
		move.w	#(nozk-noza)/2-1,d0
.loop		move.l	d1,a2
		move.w	(a1),d2
		moveq	#63-1,d3		63 codes briques possibles
.york2		cmp.w	(a2)+,d2		code brik existe ?
		beq.s	.york1			-> oui
		dbf	d3,.york2
		clr.w	(a1)+
		dbf	d0,.loop
		bra.s	.s
.york1		move.w	(zorkais-yorkais-2,a2),(a1)+
		dbf	d0,.loop
.s		rts

;--------------------------------------------------------------------------
;		jeu : table des différentes valeurs briks du 1er Tecnoball
;--------------------------------------------------------------------------
yorkais:	dc	0000,0002,0004,0006,0008,0010,0012
		dc	0588,0590,0592,0594,0596,0598,0600
		dc	1176,1178,1180,1182,1184,1186,1188
		dc	1764,1766,1768,1770,1772,1774,1776
		dc	2352,2354,2356,2358,2360,2362,2364
		dc	2940,2942,2944,2946,2948,2950,2952
		dc	3528,3530,3532,3534,3536,3538,3540
		dc	4116,4118,4120,4122,4124,4126,4128
		dc	4704,4706,4708,4710,4712,4714,4716

zorkais:	dc.b	0,00,0,02,0,04,0,06,0,08,0,10,0,12
		dc.b	0,00,1,02,1,04,1,06,1,08,1,10,1,12
		dc.b	0,00,2,02,2,04,2,06,2,08,2,10,2,12
		dc.b	0,00,3,02,3,04,3,06,3,08,3,10,3,12
		dc.b	0,00,4,02,4,04,4,06,4,08,4,10,4,12
		dc.b	0,00,5,02,5,04,5,06,5,08,5,10,5,12
		dc.b	0,00,6,02,6,04,6,06,6,08,6,10,6,12
		dc.b	0,00,7,02,7,04,7,06,7,08,7,10,7,12
		dc.b	8,00,8,02,8,04,8,06,8,08,8,10,8,12

;--------------------------------------------------------------------------
;		jeu : table des différentes valeurs briks de TecnoballZ
;--------------------------------------------------------------------------
*zorkais:	dc	ll*0+00,ll*0+02,ll*0+04,ll*0+06,ll*0+08,ll*0+10,ll*0+12
		dc	ll*0+00,ll*1+02,ll*1+04,ll*1+06,ll*1+08,ll*1+10,ll*1+12
		dc	ll*0+00,ll*2+02,ll*2+04,ll*2+06,ll*2+08,ll*2+10,ll*2+12
		dc	ll*0+00,ll*3+02,ll*3+04,ll*3+06,ll*3+08,ll*3+10,ll*3+12
		dc	ll*0+00,ll*4+02,ll*4+04,ll*4+06,ll*4+08,ll*4+10,ll*4+12
		dc	ll*0+00,ll*5+02,ll*5+04,ll*5+06,ll*5+08,ll*5+10,ll*5+12
		dc	ll*0+00,ll*6+02,ll*6+04,ll*6+06,ll*6+08,ll*6+10,ll*6+12
		dc	ll*0+00,ll*7+02,ll*7+04,ll*7+06,ll*7+08,ll*7+10,ll*7+12
		dc	ll*8+00,ll*8+02,ll*8+04,ll*8+06,ll*8+08,ll*8+10,ll*8+12

noza	
		incbin	dh1:tempo/level/noza
		incbin	dh1:tempo/level/nozb
		incbin	dh1:tempo/level/nozc
		incbin	dh1:tempo/level/nozd
		incbin	dh1:tempo/level/noze
		incbin	dh1:tempo/level/nozf
		incbin	dh1:tempo/level/nozg
		incbin	dh1:tempo/level/nozh
		incbin	dh1:tempo/level/nozi
		incbin	dh1:tempo/level/nozj
nozk

