*-------------------------------------------------------------------------*

*-------------------------------------------------------------------------*
		SECTION	ETB,CODE
		OPT	P+		pas de table de relogement
		OPT	C20+		labels et symboles 20 caract�res maxi.,  majuscules/minuscules respect�es
		OPT	p=68020		assemble les instructions MC68020
		INCDIR	DH0:Data/
		INCDIR	DH1:Data/Armada/Include
		INCLUDE	"Data/Constantes.s"
		INCLUDE	"Data/Macros.s"

*>>>>>>>>>>>>>>>>>>>>>>>>>* Debut code MC68020 *<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	INCLUDE	"Mentat/Include/mentat.s"
msg.pers	dc.b	"                                           "	�-128
		EVEN

************* routine appel�e avant de quitter le multit�che **************
;sys_extra:	
;		moveq	#0,d7
;		rts
;sys_extra2:	rts

*>>>>>>>>>>>>>>>>>>>>>>>>>* d�but du programme *<<<<<<<<<<<<<<<<<<<<<<<<<<*
* Entr�e	a5.l	=> adr. 't'
*		a6.l	=> CUSTOM
run:		bsr	first_init
		bsr	sprites_off
		bsr	install_ecran
		bne	sys_erreur
		lea	(tbob-t,a5),a0
		move.l	(bob32_pagegfx-t,a5),d0
		bsr	bob32_main.init
		bne	sys_erreur


*/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*
*�����������������* BOUCLE PRINCIPALE HORS INTERRUPTION *�����������������*
*/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*
main_loop:	
		bsr	wait_vbl
		*bsr	flip_ecrans
		bsr	bob32_restore
		bsr	bob32_affiche
		tst.b	(mouse0_left-t,a5)	Bouton gauche souris ?
		beq.s	main_loop		-> Non
		bra	sys_fin

*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*
******** Routine appel�e par l'interruption du temps mort vertical ********
*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*
* Entr�e	a5.l	=> Adr. 't'
*		a6.l	=> CUSTOM
extra_vbl:
		rts

*/ INITIALISE LES BOBS /*
zbob:		

		moveq	#0,d0
		bsr	bob32.ajoute
		bne.s	.exit
		move.l	a1,(boba-t,a5)

		moveq	#0,d0
		bsr	bob32.ajoute
		bne.s	.exit
		move.l	a1,(bobb-t,a5)

		moveq	#0,d0
		bsr	bob32.ajoute
		bne.s	.exit
		move.l	a1,(bobc-t,a5)

		moveq	#0,d0
		bsr	bob32.ajoute
		bne.s	.exit
		move.l	a1,(bobd-t,a5)

		lea	(bob.liste-t,a5),a0
		moveq	#0,d5
		moveq	#NBOB-1,d4
		moveq	#0,d3
		lea	(tbob-t,a5),a1
		move.l	(4,a1),d2		d2=nbre de BOB-1
		move.l	(pt_pob1-t,a5),a1
		move.l	#(360/NBOB)*2,d1
.loop		move.l	d3,d0
		bsr	bob32.only
		bne.s	.exit
		move.w	d5,(BOB.POSX1,a1)	valeur de l'angle
		move.l	a1,(a0)+
		cmp.w	d3,d2			dernier BOB
		bne.s	.no			-> non
		moveq	#-1,d3
.no		addq.w	#1,d3
		add.w	d1,d5
		lea	(BOB.SIZE,a1),a1
		dbf	d4,.loop
		moveq	#0,d7
.exit		rts		


*-----------------------* DEPLACEMENT CIRCULAIRE *------------------------*
boubouhorror:

		*/ Place les pointeurs */
		lea	(boba-t,a5),a0
		lea	(xa-t,a5),a1
		moveq	#3,d2
		moveq	#PR.DEC,d1
.loop4s		move.l	(a0)+,a4
		move.l	(a1)+,d0
		asr.l	d1,d0
		move.w	d0,(BOB.POSX,a4)
		move.l	(a1)+,d0
		asr.l	d1,d0
		move.w	d0,(BOB.POSY,a4)
		dbf	d2,.loop4s

		move.w	#128,d3
		move.w	#80,d4
		lea	(bob.liste-t,a5),a0
		lea	(tabsin-t,a5),a1
		lea	(tabcos-t,a5),a2
		move.w	#360*2,d6
		moveq	#NBOB-1,d7
.loop		move.l	(a0)+,a4
		move.w	(BOB.POSX1,a4),d0	valeur de l'angle
		cmp.w	d6,d0
		blt.s	.dangerous4
		sub.w	d6,d0
.dangerous4	move.w	d0,(BOB.POSX1,a4)
		move.w	d3,d1
		move.w	d4,d2
		muls	(a1,d0),d1		sin(a)*rayon
		muls	(a2,d0),d2		cos(a)*rayon
		asr.l	#7,d1
		asr.l	#7,d2
		add.w	#160,d1
		add.w	#100,d2
		move.w	d1,(BOB.POSX,a4)
		move.w	d2,(BOB.POSY,a4)
		moveq	#0,d0
		move.w	(BOB.POSX1,a4),d0
		divu	#22,d0
		sub.w	#32,d0
		neg.w	d0
		and.w	#31,d0
		move.l	(BOB.TABLE,a4),a3	a0=table adr. gfx & msk
		move.l	(4.w,a3,d0.w*8),(BOB.MSK,a4)
		move.l	(a3,d0.w*8),(BOB.GFX,a4)
		addq.w	#4,(BOB.POSX1,a4)
		dbf	d7,.loop
		rts


*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\
****************************> I.N.C.L.U.D.E.S  <***************************
*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\*/\
		INCLUDE	"Gfx1/tbob.s"
		INCLUDE	"Init/bezier.s"
		INCLUDE	"Init/install_ecran.s"
		INCLUDE	"Init/sys_extra.s"
		INCLUDE	"Data/X.Struct.s"

*>>>>>>>>>>>>>>>>>* Adresses utilis�es par le programme *<<<<<<<<<<<<<<<<<*
		INCLUDE	"Mentat/Include/Systeme/t.s"
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
version:	dc.b	"$VER: ARMADA (06.01.96)",0,0
		CNOP	0,4
		CNOP	0,4
boba		dc.l	0
bobb		dc.l	0
bobc		dc.l	0
bobd		dc.l	0
xa		dc.l	300*PR.NUM
ya		dc.l	001*PR.NUM
xb		dc.l	200*PR.NUM
yb		dc.l	100*PR.NUM
xc		dc.l	100*PR.NUM
yc		dc.l	120*PR.NUM
xd		dc.l	050*PR.NUM
yd		dc.l	150*PR.NUM
point		dc.l	$80000000,$40000000,$20000000,$10000000
		dc.l	$08000000,$04000000,$02000000,$01000000
		dc.l	$00800000,$00400000,$00200000,$00100000
		dc.l	$00080000,$00040000,$00020000,$00010000
		dc.l	$00008000,$00004000,$00002000,$00001000
		dc.l	$00000800,$00000400,$00000200,$00000100
		dc.l	$00000080,$00000040,$00000020,$00000010
		dc.l	$00000008,$00000004,$00000002,$00000001

*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*

