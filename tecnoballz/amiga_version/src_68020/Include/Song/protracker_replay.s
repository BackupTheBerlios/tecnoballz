*-------------------------------------------------------------------------*
*************************>> Routine de musique <<**************************
*-------------------------------------------------------------------------*
mt_music:
		tst.w	(music.on-t,a5)		Playroutine autorisée ?
		bne	MT_Return2		-> non
		tst.w	(music4voix-t,a5)	Musique 4 cannaux ? 
		beq.s	.np_noise		-> oui
		cmp.w	#1,(musicsound-t,a5)	Bruitages 4 cannaux ?
		beq	noisemaker		-> oui
		bsr	one_noise		"noisemaker.s" Bruitage 1 Cannal
.np_noise
		lea	(Variables-t,a5),a5
		addq.b	#1,(MT_Counter,a5)
		move.b	(MT_Counter,a5),d0
		cmp.b	(MT_Speed,a5),d0	Lit nouvelle ligne ?
		blo.s	.nonewnote		-> non
		clr.b	(MT_Counter,a5)
		tst.b	(MT_PattDelTime2,a5)	"Pattern Delay "?
		beq.s	.getnewnote		-> Non, lecture nouvelle ligne
		bsr.s	.nonewallchan
		bra	MT_Dskip
.nonewnote:	bsr.s	.nonewallchan
		bra	MT_NoNewPosYet

*----------------------* Espace entre deux lignes *-----------------------*
.nonewallchan:	move.w	#AUD0LCH,d5
		lea	(NT_CANNAL1,a5),a4
		tst.w	(music4voix-Variables,a5)	Musique 4 cannaux ? 
		bne.s	.no4can1
		bsr	MT_CheckEfx
.no4can1	move.w	#AUD1LCH,d5
		lea	(N_SIZE,a4),a4
		bsr	MT_CheckEfx
		move.w	#AUD2LCH,d5
		lea	(N_SIZE,a4),a4
		bsr	MT_CheckEfx
		move.w	#AUD3LCH,d5
		lea	(N_SIZE,a4),a4
		bra	MT_CheckEfx
		rts

*-------------------* Lecture d'une ligne du Pattern *--------------------*
.getnewnote:	move.l	(MT_SongDataPtr,a5),a0
		lea	(12,a0),a3
		lea	(952,a0),a2		pattpo
		lea	(1084,a0),a0		patterndata
		moveq	#0,d0
		moveq	#0,d1
		move.b	(MT_SongPos,a5),d0
		move.b	(a2,d0.w),d1
		asl.l	#8,d1
		asl.l	#2,d1
		add.w	(MT_PatternPos,a5),d1
		clr.w	(MT_DMACONTemp,a5)
		move.w	#AUD0LCH,d5
		lea	(NT_CANNAL1,a5),a4
		tst.w	(music4voix-Variables,a5) Musique 4 cannaux ? 
		bne.s	.no4can2
		bsr.s	.playvoice
.no4can2	addq.l	#4,d1
		move.w	#AUD1LCH,d5
		lea	(N_SIZE,a4),a4
		bsr.s	.playvoice
		addq.l	#4,d1
		move.w	#AUD2LCH,d5
		lea	(N_SIZE,a4),a4
		bsr.s	.playvoice
		addq.l	#4,d1
		move.w	#AUD3LCH,d5
		lea	(N_SIZE,a4),a4
		bsr.s	.playvoice
		bra	MT_SetDMA

********************* Routine traitant un seul cannal *********************
.playvoice:	tst.l	(a4)
		bne.s	.plvskip
		bsr	MT_PerNop
.plvskip:	move.l	(a0,d1.l),(a4)
		moveq	#0,d2
		move.b	(N_Cmd,a4),d2
		andi.b	#$F0,d2
		lsr.b	#4,d2
		move.b	(a4),d0
		andi.b	#$F0,d0
		or.b	d0,d2
		beq	MT_SetRegs
		moveq	#0,d3
		move.l	a5,a1
		suba.w	#142,a1
		move	d2,d4
		subq.l	#1,d2
		asl.l	#2,d2
		mulu	#30,d4
		move.l	(a1,d2.l),(N_Start,a4)
		move.w	(a3,d4.l),(N_Length,a4)
		move.w	(a3,d4.l),(N_RealLength,a4)
		move.b	(2,a3,d4.l),(N_FineTune,a4)
		move.b	(3,a3,d4.l),(N_Volume,a4)
		move.w	(4,a3,d4.l),d3 		Get repeat
		beq.s	MT_NoLoop
		move.l	(N_Start,a4),d2		Get start
		add.w	d3,d3
		add.l	d3,d2			Add repeat
		move.l	d2,(N_LoopStart,a4)
		move.l	d2,(N_WaveStart,a4)
		move.w	(4,a3,d4.l),d0		Get repeat
		add.w	(6,a3,d4.l),d0		Add replen
		move.w	d0,(N_Length,a4)
		move.w	(6,a3,d4.l),(N_Replen,a4) Save replen
		moveq	#0,d0
		move.b	(N_Volume,a4),d0
		move.w	d0,(AC_VOL,a6,d5.w)	Set volume
		bra.s	MT_SetRegs
MT_NoLoop:	move.l	(N_Start,a4),d2
		add.l	d3,d2
		move.l	d2,(N_LoopStart,a4)
		move.l	d2,(N_WaveStart,a4)
		move.w	(6,a3,d4.l),(N_Replen,a4) Save replen
		moveq	#0,d0
		move.b	(N_Volume,a4),d0
		move.w	d0,(AC_VOL,a6,d5.w)	Set volume
MT_SetRegs:	move.w	(a4),d0
		andi.w	#$FFF,d0
		beq	MT_CheckMoreEfx		If no note
		move.w	(2,a4),d0
		andi.w	#$FF0,d0
		cmpi.w	#$E50,d0
		beq.s	MT_DoSetFineTune
		move.b	(2,a4),d0
		andi.b	#$F,d0
		cmpi.b	#3,d0			TonePortamento
		beq.s	MT_ChkTonePorta
		cmpi.b	#5,d0
		beq.s	MT_ChkTonePorta
		cmpi.b	#9,d0			Sample Offset
		bne.s	MT_SetPeriod
		bsr	MT_CheckMoreEfx
		bra.s	MT_SetPeriod
MT_DoSetFineTune:
		bsr	MT_SetFineTune
		bra.s	MT_SetPeriod
MT_ChkTonePorta:
		bsr	MT_SetTonePorta
		bra	MT_CheckMoreEfx
MT_SetPeriod:	movem.l	d0-d1/a0-a1,-(a7)
		move.w	(a4),d1
		andi.w	#$FFF,d1
		lea	(MT_PeriodTable,pc),a1
		moveq	#0,d0
		moveq	#36,d7
MT_FtuLoop:	cmp.w	(a1,d0.w),d1
		bhs.s	MT_FtuFound
		addq.l	#2,d0
		dbf	d7,MT_FtuLoop
MT_FtuFound:	moveq	#0,d1
		move.b	(N_FineTune,a4),d1
		mulu	#72,d1
		add.l	d1,a1
		move.w	(a1,d0.w),(N_Period,a4)
		movem.l	(a7)+,d0-d1/a0-a1
		move.w	(2,a4),d0
		andi.w	#$FF0,d0
		cmpi.w	#$ED0,d0		Notedelay
		beq	MT_CheckMoreEfx
		move.w	(N_DMABit,a4),(DMACON,a6)
		btst	#2,(N_WaveControl,a4)
		bne.s	MT_Vibnoc
		clr.b	(N_VibratoPos,a4)
MT_Vibnoc:	btst	#6,(N_WaveControl,a4)
		bne.s	MT_Trenoc
		clr.b	(N_TremoloPos,a4)
MT_Trenoc:	move.l	(N_Start,a4),(AC_PTR,a6,d5.w)
		move.w	(N_Length,a4),(AC_LEN,a6,d5.w)
		move.w	(N_Period,a4),d0
		move.w	d0,(AC_PER,a6,d5.w)
		move.w	(N_DMABit,a4),d0
		or.w	d0,(MT_DMACONTemp,a5)
		bra	MT_CheckMoreEfx

*=================> Lance les sons d'une nouvelle ligne <=================*
MT_SetDMA:	bsr	MT_DMAWaitLoop
		move.w	(MT_DMACONTemp,a5),d0
		ori.w	#$8000,d0
		move.w	d0,(DMACON,a6)
		bsr	MT_DMAWaitLoop
		lea	(NT_CANNAL4,a5),a4
		move.l	(N_LoopStart,a4),(AUD3LCH,a6)
		move.w	(N_Replen,a4),(AUD3LEN,a6)
		lea	(-N_SIZE,a4),a4
		move.l	(N_LoopStart,a4),(AUD2LCH,a6)
		move.w	(N_Replen,a4),(AUD2LEN,a6)
		lea	(-N_SIZE,a4),a4
		move.l	(N_LoopStart,a4),(AUD1LCH,a6)
		move.w	(N_Replen,a4),(AUD1LEN,a6)
		lea	(-N_SIZE,a4),a4
		tst.w	(music4voix-Variables,a5) Musique 4 cannaux ? 
		bne.s	MT_Dskip
		move.l	(N_LoopStart,a4),(AUD0LCH,a6)
		move.w	(N_Replen,a4),(AUD0LEN,a6)
MT_Dskip:	addi.w	#16,(MT_PatternPos,a5)
		move.b	(MT_PattDelTime,a5),d0
		beq.s	MT_Dskc
		move.b	d0,(MT_PattDelTime2,a5)
		clr.b	(MT_PattDelTime,a5)
MT_Dskc:	tst.b	(MT_PattDelTime2,a5)
		beq.s	MT_Dska
		subq.b	#1,(MT_PattDelTime2,a5)
		beq.s	MT_Dska
		sub.w	#16,(MT_PatternPos,a5)
MT_Dska:	tst.b	(MT_PBreakFlag,a5)
		beq.s	MT_Nnpysk
		clr.b	(MT_PBreakFlag,a5)
		moveq	#0,d0
		move.b	(MT_PBreakPos,a5),d0
		clr.b	(MT_PBreakPos,a5)
		lsl.w	#4,d0
		move.w	d0,(MT_PatternPos,a5)
MT_Nnpysk:	cmpi.w	#1024,(MT_PatternPos,a5)
		blo.s	MT_NoNewPosYet
MT_NextPosition moveq	#0,d0
		move.b	(MT_PBreakPos,a5),d0
		lsl.w	#4,d0
		move.w	d0,(MT_PatternPos,a5)
		clr.b	(MT_PBreakPos,a5)
		clr.b	(MT_PosJumpFlag,a5)
		addq.b	#1,(MT_SongPos,a5)
		andi.b	#$7F,(MT_SongPos,a5)
		move.b	(MT_SongPos,a5),d1
		move.l	(MT_SongDataPtr,a5),a0
		cmp.b	(950,a0),d1
		blo.s	MT_NoNewPosYet
		clr.b	(MT_SongPos,a5)
MT_NoNewPosYet:	tst.b	(MT_PosJumpFlag,a5)
		bne.s	MT_NextPosition
		rts

*************** Routine traintant un cannal entre deux lignes *************
MT_CheckEfx:	bsr	MT_UpdateFunk
		move.w	(N_Cmd,a4),d0
		andi.w	#$FFF,d0
		beq.s	MT_PerNop
		move.b	(N_Cmd,a4),d0
		andi.b	#$F,d0
		beq.s	MT_Arpeggio
		cmpi.b	#1,d0
		beq	MT_PortaUp
		cmpi.b	#2,d0
		beq	MT_PortaDown
		cmpi.b	#3,d0
		beq	MT_TonePortamento
		cmpi.b	#4,d0
		beq	MT_Vibrato
		cmpi.b	#5,d0
		beq	MT_TonePlusVolSlide
		cmpi.b	#6,d0
		beq	MT_VibratoPlusVolSlide
		cmpi.b	#$E,d0
		beq	MT_E_Commands
SetBack:	move.w	(N_Period,a4),(AC_PER,a6,d5.w)
		cmpi.b	#7,d0
		beq	MT_Tremolo
		cmpi.b	#$a,d0
		beq	MT_VolumeSlide
MT_Return2:	rts
MT_PerNop:	move.w	(N_Period,a4),(AC_PER,a6,d5.w)
		rts
MT_Arpeggio:	moveq	#0,d0
		move.b	(MT_Counter,a5),d0
		divs	#3,d0
		swap	d0
		tst.w	D0
		beq.s	MT_Arpeggio2
		cmpi.w	#2,d0
		beq.s	MT_Arpeggio1
		moveq	#0,d0
		move.b	(N_Cmdlo,a4),d0
		lsr.b	#4,d0
		bra.s	MT_Arpeggio3
MT_Arpeggio1:	moveq	#0,d0
		move.b	(N_Cmdlo,a4),d0
		andi.b	#15,d0
		bra.s	MT_Arpeggio3
MT_Arpeggio2:	move.w	(N_Period,a4),d2
		bra.s	MT_Arpeggio4
MT_Arpeggio3:	add.w	d0,d0
		moveq	#0,d1
		move.b	(N_FineTune,a4),d1
		mulu	#72,d1
		lea	(MT_PeriodTable,pc),a0
		add.w	d1,a0
		moveq	#0,d1
		move.w	(N_Period,a4),d1
		moveq	#36,d7
MT_ArpLoop:	move.w	(a0,d0.w),d2
		cmp.w	(a0),d1
		bhs.s	MT_Arpeggio4
		addq.w	#2,a0
		dbf	d7,MT_ArpLoop
		rts
MT_Arpeggio4:	move.w	d2,(AC_PER,a6,d5.w)
		rts
MT_FinePortaUp:	tst.b	(MT_Counter,a5)
		bne.s	MT_Return2
		move.b	#$F,(MT_LowMask,a5)
MT_PortaUp:	moveq	#0,d0
		move.b	(N_Cmdlo,a4),d0
		and.b	(MT_LowMask,a5),d0
		st	(MT_LowMask,a5)
		sub.w	d0,(N_Period,a4)
		move.w	(N_Period,a4),d0
		andi.w	#$FFF,d0
		cmpi.w	#113,d0
		bpl.s	MT_PortaUskip
		andi.w	#$F000,(N_Period,a4)
		ori.w	#113,(N_Period,a4)
MT_PortaUskip:	move.w	(N_Period,a4),d0
		andi.w	#$FFF,d0
		move.w	d0,(AC_PER,a6,d5.w)
		rts
 MT_FinePortaDown:
		tst.b	(MT_Counter,a5)
		bne	MT_Return2
		move.b	#$0f,(MT_LowMask,a5)
MT_PortaDown:	clr.w	d0
		move.b	(N_Cmdlo,a4),d0
		and.b	(MT_LowMask,a5),d0
		st	(MT_LowMask,a5)
		add.w	d0,(N_Period,a4)
		move.w	(N_Period,a4),d0
		andi.w	#$0fff,d0
		cmpi.w	#856,d0
		bmi.s	MT_PortaDskip
		andi.w	#$f000,(N_Period,a4)
		ori.w	#856,(N_Period,a4)
MT_PortaDskip:	move.w	(N_Period,a4),d0
		andi.w	#$0fff,d0
		move.w	d0,(AC_PER,a6,d5.w)
		rts
MT_SetTonePorta	move.l	a0,-(a7)
		move.w	(a4),d2
		andi.w	#$0fff,d2
		moveq	#0,d0
		move.b	(N_FineTune,a4),d0
		mulu	#74,d0
		lea	(MT_PeriodTable,pc),a0
		add.w	d0,a0
		moveq	#0,d0
MT_StpLoop:	cmp.w	(a0,d0.w),d2
		bhs.s	MT_StpFound
		addq.w	#2,d0
		cmpi.w	#74,d0
		blo.s	MT_StpLoop
		moveq	#70,d0
MT_StpFound:	move.b	(N_FineTune,a4),d2
		andi.b	#8,d2
		beq.s	MT_StpGoss
		tst.w	d0
		beq.s	MT_StpGoss
		subq.w	#2,d0
MT_StpGoss:	move.w	(a0,d0.w),d2
		move.l	(a7)+,a0
		move.w	d2,(N_WantedPeriod,a4)
		move.w	(N_Period,a4),d0
		clr.b	(N_TonePortDirec,a4)
		cmp.w	d0,d2
		beq.s	MT_ClearTonePorta
		bge	MT_Return2
		move.b	#1,(N_TonePortDirec,a4)
		rts
MT_ClearTonePorta:
		clr.w	(N_WantedPeriod,a4)
		rts
MT_TonePortamento:
		move.b	(N_Cmdlo,a4),d0
		beq.s	MT_TonePortNoChange
		move.b	d0,(N_TonePortSpeed,a4)
		clr.b	(N_Cmdlo,a4)
MT_TonePortNoChange:
		tst.w	(N_WantedPeriod,a4)
		beq	MT_Return2
		moveq	#0,d0
		move.b	(N_TonePortSpeed,a4),d0
		tst.b	(N_TonePortDirec,a4)
		bne.s	MT_TonePortaUp
MT_TonePortaDown:
		add.w	d0,(N_Period,a4)
		move.w	(N_WantedPeriod,a4),d0
		cmp.w	(N_Period,a4),d0
		bgt.s	MT_TonePortaSetPer
		move.w	(N_WantedPeriod,a4),(N_Period,a4)
		clr.w	(N_WantedPeriod,a4)
		bra.s	MT_TonePortaSetPer
MT_TonePortaUp:	sub.w	d0,(N_Period,a4)
		move.w	(N_WantedPeriod,a4),d0
		cmp.w	(N_Period,a4),d0
		blt.s	MT_TonePortaSetPer
		move.w	(N_WantedPeriod,a4),(N_Period,a4)
		clr.w	(N_WantedPeriod,a4)
MT_TonePortaSetPer:
		move.w	(N_Period,a4),d2
		move.b	(N_GlissFunk,a4),d0
		andi.b	#$0f,d0
		beq.s	MT_GlissSkip
		moveq	#0,d0
		move.b	(N_FineTune,a4),d0
		mulu	#72,d0
		lea	(MT_PeriodTable,pc),a0
		add.w	d0,a0
		moveq	#0,d0
MT_GlissLoop:	cmp.w	(a0,d0.w),d2
		bhs.s	MT_GlissFound
		addq.w	#2,d0
		cmpi.w	#72,d0
		blo.s	MT_GlissLoop
		moveq	#70,d0
MT_GlissFound:	move.w	(a0,d0.w),d2
MT_GlissSkip:	move.w	d2,(AC_PER,a6,d5.w)	Set period
		rts
MT_Vibrato:	move.b	(N_Cmdlo,a4),d0
		beq.s	MT_Vibrato2
		move.b	(N_VibratoCmd,a4),d2
		andi.b	#$0f,d0
		beq.s	MT_VibSkip
		andi.b	#$f0,d2
		or.b	d0,d2
MT_VibSkip:	move.b	(N_Cmdlo,a4),d0
		andi.b	#$f0,d0
		beq.s	MT_VibSkip2
		andi.b	#$0f,d2
		or.b	d0,d2
MT_VibSkip2:	move.b	d2,(N_VibratoCmd,a4)
MT_Vibrato2:	move.b	(N_VibratoPos,a4),d0
		lea	(MT_VibratoTable,pc),a0
		lsr.w	#2,d0
		andi.w	#$1F,d0
		moveq	#0,d2
		move.b	(N_WaveControl,a4),d2
		andi.b	#$3,d2
		beq.s	MT_Vib_Sine
		lsl.b	#3,d0
		cmpi.b	#1,d2
		beq.s	MT_Vib_RampDown
		st	d2
		bra.s	MT_Vib_Set
MT_Vib_RampDown	tst.b	(N_VibratoPos,a4)
		bpl.s	MT_Vib_RampDown2
		st	d2
		sub.b	d0,d2
		bra.s	MT_Vib_Set
MT_Vib_RampDown2:
		move.b	d0,d2
		bra.s	MT_Vib_Set
MT_Vib_Sine:	move.b	(a0,d0.w),d2
MT_Vib_Set:	move.b	(N_VibratoCmd,a4),d0
		andi.w	#15,d0
		mulu	d0,d2
		lsr.w	#7,d2
		move.w	(N_Period,a4),d0
		tst.b	(N_VibratoPos,a4)
		bmi.s	MT_VibratoNeg
		add.w	d2,d0
		bra.s	MT_Vibrato3
MT_VibratoNeg:	sub.w	d2,d0
MT_Vibrato3:	move.w	d0,(AC_PER,a6,d5.w)
		move.b	(N_VibratoCmd,a4),d0
		lsr.w	#2,d0
		andi.w	#$3C,d0
		add.b	d0,(N_VibratoPos,a4)
		rts
MT_TonePlusVolSlide:
		bsr	MT_TonePortNoChange
		bra	MT_VolumeSlide
MT_VibratoPlusVolSlide:
		bsr.s	MT_Vibrato2
		bra	MT_VolumeSlide
MT_Tremolo:	move.b	(N_Cmdlo,a4),d0
		beq.s	MT_Tremolo2
		move.b	(N_TremoloCmd,a4),d2
		andi.b	#$0f,d0
		beq.s	MT_TreSkip
		andi.b	#$f0,d2
		or.b	d0,d2
MT_TreSkip:	move.b	(N_Cmdlo,a4),d0
		and.b	#$f0,d0
		beq.s	MT_TreSkip2
		andi.b	#$0f,d2
		or.b	d0,d2
MT_TreSkip2:	move.b	d2,(N_TremoloCmd,a4)
MT_Tremolo2:	move.b	(N_TremoloPos,a4),d0
		lea	(MT_VibratoTable,pc),a0
		lsr.w	#2,d0
		andi.w	#$1f,d0
		moveq	#0,d2
		move.b	(N_WaveControl,a4),d2
		lsr.b	#4,d2
		andi.b	#3,d2
		beq.s	MT_Tre_Sine
		lsl.b	#3,d0
		cmpi.b	#1,d2
		beq.s	MT_Tre_RampDown
		st	d2
		bra.s	MT_Tre_Set
MT_Tre_RampDown:
		tst.b	(N_VibratoPos,a4)
		bpl.s	MT_Tre_RampDown2
		st	d2
		sub.b	d0,d2
		bra.s	MT_Tre_Set
MT_Tre_RampDown2:
		move.b	d0,d2
		bra.s	MT_Tre_Set
MT_Tre_Sine:	move.b	(a0,d0.w),d2
MT_Tre_Set:	move.b	(N_TremoloCmd,a4),d0
		andi.w	#15,d0
		mulu	d0,d2
		lsr.w	#6,d2
		moveq	#0,d0
		move.b	(N_Volume,a4),d0
		tst.b	(N_TremoloPos,a4)
		bmi.s	MT_TremoloNeg
		add.w	d2,d0
		bra.s	MT_Tremolo3
MT_TremoloNeg:	sub.w	d2,d0
MT_Tremolo3:	bpl.s	MT_TremoloSkip
		clr.w	d0
MT_TremoloSkip:	cmpi.w	#$40,d0
		bls.s	MT_TremoloOk
		move.w	#$40,d0
MT_TremoloOk:	move.w	d0,(AC_VOL,a6,d5.w)
		move.b	(N_TremoloCmd,a4),d0
		lsr.w	#2,d0
		andi.w	#$3C,d0
		add.b	d0,(N_TremoloPos,a4)
		rts
MT_SampleOffset	moveq	#0,d0
		move.b	(N_Cmdlo,a4),d0
		beq.s	MT_SoNoNew
		move.b	d0,(N_SampleOffset,a4)
MT_SoNoNew:	move.b	(N_SampleOffset,a4),d0
		lsl.w	#7,d0
		cmp.w	(N_Length,a4),d0
		bge.s	MT_SofSkip
		sub.w	d0,(N_Length,a4)
		add.w	d0,d0
		add.l	d0,(N_Start,a4)
		rts
MT_SofSkip:	move.w	#1,(N_Length,a4)
		rts
MT_VolumeSlide:	moveq	#0,d0
		move.b	(N_Cmdlo,a4),d0
		lsr.b	#4,d0
		tst.b	d0
		beq.s	MT_VolSlideDown
MT_VolSlideUp:	add.b	d0,(N_Volume,a4)
		cmpi.b	#$40,(N_Volume,a4)
		bmi.s	MT_VsuSkip
		move.b	#$40,(N_Volume,a4)
MT_VsuSkip:	move.b	(N_Volume,a4),d0
		move.w	d0,(AC_VOL,a6,d5.w)
		rts
MT_VolSlideDown	moveq	#0,d0
		move.b	(N_Cmdlo,a4),d0
		andi.b	#$0f,d0
MT_VolSlideDown2:
		sub.b	d0,(N_Volume,a4)
		bpl.s	MT_VsdSkip
		clr.b	(N_Volume,a4)
MT_VsdSkip:	move.b	(N_Volume,a4),d0
		move.w	d0,(AC_VOL,a6,d5.w)
		rts
MT_PositionJump	move.b	(N_Cmdlo,a4),(MT_SongPos,a5)
		subq.b	#1,(MT_SongPos,a5)
MT_PJ2:		clr.b	(MT_PBreakPos,a5)
		st 	(MT_PosJumpFlag,a5)
		rts
MT_VolumeChange	moveq	#0,d0
		move.b	(N_Cmdlo,a4),d0
		cmpi.b	#$40,d0
		bls.s	MT_VolumeOk
		moveq	#$40,d0
MT_VolumeOk:	move.b	d0,(N_Volume,a4)
		move.w	d0,(AC_VOL,a6,d5.w)
		rts
MT_PatternBreak	moveq	#0,d0
		move.b	(N_Cmdlo,a4),d0
		move.l	d0,d2
		lsr.b	#4,d0
		mulu	#10,d0
		andi.b	#$F,d2
		add.b	d2,d0
		cmpi.b	#63,d0
		bhi.s	MT_PJ2
		move.b	d0,(MT_PBreakPos,a5)
		st	(MT_PosJumpFlag,a5)
		rts
MT_SetSpeed:	move.b	(3,a4),d0
		beq	MT_Return2
		clr.b	(MT_Counter,a5)
		move.b	d0,(MT_Speed,a5)
		rts
MT_CheckMoreEfx	bsr	MT_UpdateFunk
		move.b	(2,a4),d0
		andi.b	#$F,d0
		cmpi.b	#$9,d0
		beq	MT_SampleOffset
		cmpi.b	#$b,d0
		beq	MT_PositionJump
		cmpi.b	#$d,d0
		beq.s	MT_PatternBreak
		cmpi.b	#$e,d0
		beq.s	MT_E_Commands
		cmpi.b	#$f,d0
		beq.s	MT_SetSpeed
		cmpi.b	#$c,d0
		beq	MT_VolumeChange
		bra	MT_PerNop
MT_E_Commands:	move.b	(N_Cmdlo,a4),d0
		andi.b	#$f0,d0
		lsr.b	#4,d0
		beq.s	MT_FilterOnOff
		cmpi.b	#1,d0
		beq	MT_FinePortaUp
		cmpi.b	#2,d0
		beq	MT_FinePortaDown
		cmpi.b	#3,d0
		beq.s	MT_SetGlissControl
		cmpi.b	#4,d0
		beq	MT_SetVibratoControl
		cmpi.b	#5,d0
		beq	MT_SetFineTune
		cmpi.b	#6,d0
		beq	MT_JumpLoop
		cmpi.b	#7,d0
		beq	MT_SetTremoloControl
		cmpi.b	#9,d0
		beq	MT_RetrigNote
		cmpi.b	#$a,d0
		beq	MT_VolumeFineUp
		cmpi.b	#$b,d0
		beq	MT_VolumeFineDown
		cmpi.b	#$c,d0
		beq	MT_NoteCut
		cmpi.b	#$d,d0
		beq	MT_NoteDelay
		cmpi.b	#$e,d0
		beq	MT_PatternDelay
		cmpi.b	#$f,d0
		beq	MT_FunkIt
		rts
MT_FilterOnOff:	move.b	(N_Cmdlo,a4),d0
		andi.b	#1,d0
		add.b	d0,d0
		andi.b	#$fd,$bfe001
		or.b	d0,$bfe001
		rts
MT_SetGlissControl:
		move.b	(N_Cmdlo,a4),d0
		andi.b	#$0f,d0
		andi.b	#$f0,(N_GlissFunk,a4)
		or.b	d0,(N_GlissFunk,a4)
		rts
MT_SetVibratoControl:
		move.b	(N_Cmdlo,a4),d0
		andi.b	#$0f,d0
		andi.b	#$f0,(N_WaveControl,a4)
		or.b	d0,(N_WaveControl,a4)
		rts
MT_SetFineTune:	move.b	(N_Cmdlo,a4),d0
		andi.b	#$0f,d0
		move.b	d0,(N_FineTune,a4)
		rts
MT_JumpLoop:	tst.b	(MT_Counter,a5)
		bne	MT_Return2
		move.b	(N_Cmdlo,a4),d0
		andi.b	#$0f,d0
		beq.s	MT_SetLoop
		tst.b	(N_LoopCount,a4)
		beq.s	MT_JumpCnt
		subq.b	#1,(N_LoopCount,a4)
		beq	MT_Return2
MT_JmpLoop:	move.b	(N_PattPos,a4),(MT_PBreakPos,a5)
		st	(MT_PBreakFlag,a5)
		rts
MT_JumpCnt:	move.b	d0,(N_LoopCount,a4)
		bra.s	MT_JmpLoop
MT_SetLoop:	move.w	(MT_PatternPos,a5),d0
		lsr.w	#4,d0
		move.b	d0,(N_PattPos,a4)
		rts
MT_SetTremoloControl:
		move.b	(N_Cmdlo,a4),d0
		lsl.b	#4,d0
		andi.b	#$0f,(N_WaveControl,a4)
		or.b	d0,(N_WaveControl,a4)
		rts
MT_RetrigNote:	move.l	d1,-(a7)
		moveq	#0,d0
		move.b	(N_Cmdlo,a4),d0
		andi.b	#$0f,d0
		beq.s	MT_RtnEnd
		moveq	#0,d1
		move.b	(MT_Counter,a5),d1
		bne.s	MT_RtnSkp
		move.w	(a4),d1
		andi.w	#$FFF,d1
		bne.s	MT_RtnEnd
		moveq	#0,d1
		move.b	(MT_Counter,a5),d1
MT_RtnSkp:	divu	d0,d1
		swap	d1
		tst.w	d1
		bne.s	MT_RtnEnd
MT_DoRetrig:	move.w	(N_DMABit,a4),(DMACON,a6) Channel DMA off
		move.l	(N_Start,a4),(AC_PTR,a6,d5.w)
		move.w	(N_Length,a4),(AC_LEN,a6,d5.w)
		bsr	MT_DMAWaitLoop
		move.w	(N_DMABit,a4),d0
		ori.w	#$8000,d0
		move.w	d0,(DMACON,a6)
		bsr	MT_DMAWaitLoop
		move.l	(N_LoopStart,a4),(AC_PTR,a6,d5.w)
		move.l	(N_Replen,a4),(AC_LEN,a6,d5.w)
MT_RtnEnd:	move.l	(a7)+,d1
		rts
MT_VolumeFineUp:tst.b	(MT_Counter,a5)
		bne	MT_Return2
		moveq	#0,d0
		move.b	(N_Cmdlo,a4),d0
		andi.b	#$d,d0
		bra	MT_VolSlideUp
MT_VolumeFineDown:
		tst.b	(MT_Counter,a5)
		bne	MT_Return2
		moveq	#0,d0
		move.b	(N_Cmdlo,a4),d0
		andi.b	#$0f,d0
		bra	MT_VolSlideDown2
MT_NoteCut:	moveq	#0,d0
		move.b	(N_Cmdlo,a4),d0
		andi.b	#$0f,d0
		cmp.b	(MT_Counter,a5),d0
		bne	MT_Return2
		clr.b	(N_Volume,a4)
		clr.w	(AC_VOL,a6,d5.w)
		rts
MT_NoteDelay:	moveq	#0,d0
		move.b	(N_Cmdlo,a4),d0
		andi.b	#$0f,d0
		cmp.b	(MT_Counter,a5),d0
		bne	MT_Return2
		move.w	(a4),d0
		beq	MT_Return2
		move.l	d1,-(a7)
		bra	MT_DoRetrig
MT_PatternDelay	tst.b	(MT_Counter,a5)
		bne	MT_Return2
		moveq	#0,d0
		move.b	(N_Cmdlo,a4),d0
		andi.b	#$F,d0
		tst.b	(MT_PattDelTime2,a5)
		bne	MT_Return2
		addq.b	#1,d0
		move.b	d0,(MT_PattDelTime,a5)
		rts
MT_FunkIt:	tst.b	(MT_Counter,a5)
		bne	MT_Return2
		move.b	(N_Cmdlo,a4),d0
		lsl.b	#4,d0
		andi.b	#$0f,(N_GlissFunk,a4)
		or.b	d0,(N_GlissFunk,a4)
		tst.b	d0
		beq	MT_Return2
MT_UpdateFunk:	movem.l	a0/d1,-(a7)
		moveq	#0,d0
		move.b	(N_GlissFunk,a4),d0
		lsr.b	#4,d0
		beq.s	MT_FunkEnd
		lea	(MT_FunkTable,pc),a0
		move.b	(a0,d0.w),d0
		add.b	d0,(N_FunkOffset,a4)
		btst	#7,(N_FunkOffset,a4)
		beq.s	MT_FunkEnd
		clr.b	(N_FunkOffset,a4)
		move.l	(N_LoopStart,a4),d0
		moveq	#0,d1
		move.w	(N_Replen,a4),d1
		add.l	d1,d0
		add.l	d1,d0
		move.l	(N_WaveStart,a4),a0
		addq.w	#1,a0
		cmp.l	d0,a0
		blo.s	MT_FunkOk
		move.l	(N_LoopStart,a4),a0
MT_FunkOk:	move.l	a0,(N_WaveStart,a4)
		moveq	#-1,d0
		sub.b	(a0),d0
		move.b	d0,(a0)
MT_FunkEnd:	movem.l	(a7)+,a0/d1
		rts

*--------------------> Temporisation en lignes écran <--------------------*
MT_DMAWaitLoop:	move.w	d1,-(a7)
		moveq	#5,d0		Attend 5+1 lignes
.loop		move.b	(VHPOSR,a6),d1	Lit position Y du raster
.wait		cmp.b	(VHPOSR,a6),d1
		beq.s	.wait		wait until it changes
		dbf	d0,.loop	do it again
		move.w	(a7)+,d1
		rts

*-------------------------------------------------------------------------*
***********************>>  Initialise le Module  <<************************
*-------------------------------------------------------------------------*
*	Entrée	=> a3.l  Adresse structure "RESA" du module powerpacké
np_init:
		*/ Réserve la mémoire pour le module */
		move.l	(RESA.ADR,a3),d0
		move.l	(RESA.SIZ,a3),d1
		lea	(module-t,a5),a4
		bsr.l	decrunch.pp
		bne	.exit

		*/ Efface les variables */
		move.l	d0,d7
		lea	(MT_Chan1Temp,pc),a0
		move.l	#-NT_SIZE,d0
		bsr.l	clear.memory
		move.l	d7,a0
	
		*/ Cherche le numéro du Pattern le plus élevé *
		lea	(Variables-t,a5),a4
		move.l	a0,(MT_SongDataPtr,a4)
		lea	(952,a0),a1
		moveq	#127,d0
		moveq	#0,d1
.mtloop:	move.l	d1,d2
		subq.w	#1,d0
.mtloop2:	move.b	(a1)+,d1
		cmp.b	d2,d1
		bgt.s	.mtloop
		dbf	d0,.mtloop2

		*/ Calcul les adresses des samples */
		addq.b	#1,d2
		lea	(NT_SAMPLES,a4),a1
		asl.l	#8,d2
		asl.l	#2,d2
		addi.l	#1084,d2
		add.l	a0,d2
		move.l	d2,a2
		moveq	#30,d0
.mtloop3:	move.l	a2,(a1)+
		moveq	#0,d1
		move.w	(42,a0),d1
		beq.s	.nosample
		clr.l	(a2)
		add.l	d1,d1
		add.l	d1,a2
.nosample	lea	(30,a0),a0
		dbf	d0,.mtloop3

		*/ Inititalise quelques adresses */
		lea	(NT_CANNAL1,a4),a0
		move.w	#$0001,(N_DMABit,a0)
		move.w	#$0002,(N_SIZE+N_DMABit,a0)
		move.w	#$0004,(2*N_SIZE+N_DMABit,a0)
		move.w	#$0008,(3*N_SIZE+N_DMABit,a0)
		ori.b	#2,$bfe001
		move.b	#6,(MT_Speed,a4)
		moveq	#0,d0
		move.b	d0,(MT_Counter,a4)
		move.b	d0,(MT_SongPos,a4)
		move.w	d0,(MT_PatternPos,a4)
		move.w	d0,(AUD0VOL,a6)
		move.w	d0,(AUD1VOL,a6)
		move.w	d0,(AUD2VOL,a6)
		move.w	d0,(AUD3VOL,a6)
		move.w	#$F,(DMACON,a6)
		move.w	#$2000,($9A,a6)
		*/ Change vecteur d'int. de niveau 6 */
		lea	(np_oldirq-t,a5),a1
		lea	(razure_nous,pc),a0
		move.l	a0,(a1)
		move.l	a0,$78.w
		move.l	(np_oldirq,pc),$78.w
		*/ RAZ structures bruitages */
		lea	(np0-t,a5),a0
		move.l	#NP_SIZE*4,d0
		bsr.l	clear.memory
		move.w	#$8200,(np_dmacon-t,a5)
		lea	(sound-t,a5),a0
		move.l	#KOFFER,d0
		bsr.l	clear.memory
		moveq	#0,d7	
.exit		rts

**************************>>> Coupe le module <<<**************************
np_end:
		move.w	#1,(music.on-t,a5)	flag interdit saut à la playroutine
		moveq	#0,d0
		move.w	d0,(AUD0VOL,a6)
		move.w	d0,(AUD1VOL,a6)
		move.w	d0,(AUD2VOL,a6)
		move.w	d0,(AUD3VOL,a6)
		move.w	#$F,(DMACON,a6)
		move.w	#$2000,($9A,a6)
		*/ Vire le module */
		lea	(module-t,a5),a0
		bsr.l	libere_mem
		bne.l	sys_erreur
		moveq	#0,d7
.exit		rts

* ProTracker2.2a replay routine by Crayon/Noxious. Improved and modified
* by Teeme of Fist! Unlimited in 1992. Share and enjoy! :)
* Rewritten for Devpac (slightly..) by CJ. Devpac does not like bsr.L
* cmpi is compare immediate, it requires immediate data! And some
* labels had upper/lower case wrong...
* Now improved to make it work better if CIA timed - thanks Marco!
