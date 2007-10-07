		OPT	P=68020

		SECTION	ETB,CODE

;--------------------- Offset d'appel librairie exec ---------------------;
Forbid		=-132
Permit		=-138
SuperState	=-150
UserState	=-156
AllocMem	=-198
CUSTOM		=$DFF000
INTENAR		=$01C
DMACONR		=$002
ADKCONR		=$010
ADKCON		=$09E	W
DMACON		=$096	W
DSKLEN		=$024	W
INTENA		=$09A	W
INTREQ		=$01E	R


***************************************************************************
debut:
		* gèle le système *
		move.l	4.w,a6
		jsr	(Forbid,a6)
		lea	CUSTOM,a6
		lea	(save_intena,pc),a0
		move.w	(INTENAR,a6),(a0)
		or.w	#$c000,(a0)+
		move	(DMACONR,a6),(a0)
		or.w	#$8000,(a0)+
		move.w	(ADKCONR,a6),(a0)
		or.w	#$8000,(a0)
		move.w	#$7FFF,(INTENA,a6)
		move.w	#$7FFF,(INTREQ,a6)
		move.w	#$07FF,(DMACON,a6)
		move.l	4.w,a6
		jsr	(SuperState,a6)		sortie : d0=SysStack
		movec	vbr,d1			d1=base des vecteurs
		lea	base_vecteurs,a0
		move.l	d1,(a0)
		move.l	4.w,a6
		jsr	(UserState,a6)		entrée : d0=SysStack

***************************************************************************
		lea	$6c.w,a0
		move.l	(base_vecteurs,pc),d1
		add.l	d1,a0
		move.l	(a0),d0
		lea	(saut+2,pc),a1
		move.l	d0,(a1)

***************************************************************************
		moveq	#(irq2-irq),d0
		moveq	#0,d1
		move.l	4.w,a6
		jsr	(AllocMem,a6)
		tst.l	d0
		beq	error

***************************************************************************
		lea	$6c.w,a0
		move.l	(base_vecteurs,pc),d1
		add.l	d1,a0
		lea	(irq,pc),a1
		move.l	d0,a2
		move	#(irq2-irq)-1,d1
.loop		move.b	(a1)+,(a2)+
		dbf	d1,.loop


***************************************************************************
		move.l	d0,(a0)

***************************************************************************
		lea	CUSTOM,a6
		move.w	(save_adkcon,pc),(ADKCON,a6)
		move.w	(save_dmacon,pc),(DMACON,a6)
		move.w	(save_intena,pc),(INTENA,a6)
		move.l	4.w,a6
		jsr	(Permit,a6)
		moveq	#0,d0
		rts

***************************************************************************
error		move.w	(save_adkcon,pc),(ADKCON,a6)
		move.w	(save_dmacon,pc),(DMACON,a6)
		move.w	(save_intena,pc),(INTENA,a6)
		move.l	4.w,a6
		jsr	(Permit,a6)
		moveq	#-1,d0
		rts

***************************************************************************
irq:		move	#$000,$DFF1FC
saut:		jmp	0
irq2

***************************************************************************
		CNOP	0,4
base_vecteurs	dc.l	0
save_intena	dc.w	0
save_dmacon	dc.w	0
save_adkcon	dc.w	0
