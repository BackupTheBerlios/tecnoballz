***************************************************************************
************ DANS UN JEU IL YA AUSSI LES BRUITAGES NOM DE DIEU ************
***************************************************************************
noisemaker:
		lea	CUSTOM,a6
		moveq	#0,d6			d6=RAZ
		moveq	#0,d5			d5=DMACON
		moveq	#64,d2			d2=Volume
		lea	(xt-t,a5),a0		a0=table des sons
		lea	(np3,pc),a3
		lea	(AUD3LCH,a6),a2
		moveq	#8,d0
		bsr.s	.soundy
		lea	(np2,pc),a3
		lea	(AUD2LCH,a6),a2
		moveq	#4,d0
		bsr.s	.soundy
		lea	(np1,pc),a3
		lea	(AUD1LCH,a6),a2
		moveq	#2,d0
		bsr.s	.soundy
		lea	(np0,pc),a3
		lea	(AUD0LCH,a6),a2
		moveq	#1,d0
		bsr.s	.soundy
		lea	(DMACON,a6),a0
		lea	(np_dmacon,pc),a1
		move.w	d5,(a0)			désactive bit audxlen de dmacon
		or.w	d5,(a1)			sauve ce bit
		lea	(np_irq1,pc),a0
		move.l	a0,$78.w
		move.b	#$19,$BFDE00
.np.exit:	rts

************************> Routine gérant une voix <************************
.soundy:	tst.w	(NP_WAIT,a3)		NP_WAIT
		beq.s	.free
		subq.w	#1,(NP_WAIT,a3)		NP_WAIT
.free		move.w	(a3),d1			NP_NEWS = son demadé ?
		beq	.soundx			-> non
		move.w	d6,(a3)+		NP_NEWS=0
		subq.w	#1,d1
		or.w	d0,d5
		lsl.w	#4,d1
		lea	(a0,d1),a1
		move.l	(a1)+,d3		d3=adresse du sample
		move.w	(a1)+,d1		d1=taille du sample en mots
		move.w	(a1)+,d7		d7=valeur de la période
		move.w	(a1)+,(a3)+	NP_WAIT	Temps pendant lequel le son se joue
		move.l	d3,(a3)+	NP_STAR	AUDxlc pour repeat
		move.w	(a1),(a3)	NP_PRIO	Priorité du sample
		move.l	d3,(a2)+	AUDxLC	(adresse sample)
		move.w	d1,(a2)+	AUDxLEN	(taille sample)
		move.w	d7,(a2)+	AUDxPER	(période)
		move.w	d2,(a2)+	AUDxVOL	(volume)
.soundx		rts

************** Interruptions de niveau 6 : timer A du CIA-B ***************
np_irq1:	
		tst.b	$BFDD00		*	Efface registre ICR
		move.b	#$19,$BFDE00	*	CRA : bit START à 1 démarre minuterie
					*	CRA : bit RUNMODE à 1 = mode one-shot (arrête minuterie après décompte)
					*	CRA : bit LOAD à 1
		move.l	#np_irq2,$78.w
		move.w	(np_dmacon,pc),$DFF096
		move.w	#$2000,$DFF09C
		rte
*-------------------------------------------------------------------------*
razure_nous:	tst.b	$BFDD00			efface registre ICR
		move	#$2000,$DFF09C
		rte
*-------------------------------------------------------------------------*
np_irq2:	tst.b	$BFDD00			efface registre ICR
		move.l	(np3+NP_STAR,pc),CUSTOM+AUD3LCH
		move.w	#1,CUSTOM+AUD3LEN
		move.l	(np2+NP_STAR,pc),CUSTOM+AUD2LCH
		move.w	#1,CUSTOM+AUD2LEN
		move.l	(np1+NP_STAR,pc),CUSTOM+AUD1LCH
		move.w	#1,CUSTOM+AUD1LEN
		move.l	(np0+NP_STAR,pc),CUSTOM+AUD0LCH
		move.w	#1,CUSTOM+AUD0LEN
		move.l	(np_oldirq,pc),$78.w
		move.w	#$2000,$DFF09C
		rte

*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*
*-----------------------*/EMISSION UNE SEULE VOIX*/-----------------------*
*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*
one_noise:	
		moveq	#0,d0
		lea	(xt-t,a5),a0		a0=table des sons
		lea	(MT_Chan1Temp,pc),a4
		lea	(np0,pc),a3
		lea	(AUD0LCH,a6),a2
		tst.w	(NP_WAIT,a3)		NP_WAIT
		beq.s	.free
		subq.w	#1,(NP_WAIT,a3)		NP_WAIT
.free		move.w	(a3),d1			NP_NEWS = son demadé ?
		beq	.soundx			-> non
		clr.w	(a3)+			NP_NEWS=0
		subq.w	#1,d1
		lsl.w	#4,d1
		lea	(a0,d1),a1
		move.l	(a1)+,d3		d3=adresse du sample
		move.w	(a1)+,d1		d1=taille du sample en mots
		move.w	(a1)+,d7		d7=valeur de la période
		move.w	(a1)+,(a3)+	NP_WAIT	Temps pendant lequel le son se joue
		move.l	d3,(a3)+	NP_STAR	AUDxlc pour repeat
		move.w	(a1),(a3)	NP_PRIO	Priorité du sample
		move.l	d3,(a2)+	AUDxLC	(adresse sample)
		move.w	d1,(a2)+	AUDxLEN	(taille sample)
		move.w	d7,(a2)+	AUDxPER	(période)
		move.w	#64,(a2)+	AUDxVOL	(volume)
		move.w	(N_DMABit,a4),d0
.soundx		lea	(DMACON,a6),a0
		lea	(np_dmacon,pc),a1
		move.w	d0,(a0)			désactive bit AUDxLEN de DMACON
		or.w	d0,(a1)			sauve ce bit
		lea	(mp_irq1,pc),a0
		move.l	a0,$78.w
		move.b	#$19,$BFDE00
		rts

************** Interruptions de niveau 6 : timer A du CIA-B ***************
mp_irq1:	
		tst.b	$BFDD00
		move.b	#$19,$BFDE00
		move.l	#mp_irq2,$78.w
		move.w	(np_dmacon,pc),$DFF096
		move.w	#$2000,$DFF09C
		rte
*-------------------------------------------------------------------------*
mp_irq2:	tst.b	$BFDD00			efface registre ICR
		move.l	(np0+NP_STAR,pc),CUSTOM+AUD0LCH
		move.w	#1,CUSTOM+AUD0LEN
		move.l	(np_oldirq,pc),$78.w
		move.w	#$2000,$DFF09C
		rte
