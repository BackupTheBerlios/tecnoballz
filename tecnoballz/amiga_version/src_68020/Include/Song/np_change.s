*=========================================================================*
*              Routine qui change de position dans un module		  *
*=========================================================================*
* Entrée	=> d0.w	n° position *2
*		=> a5.l	table 't'
* utilise	d0,a5
np_change:
		move.w	#1,(music.on-t,a5)	interdit saut à la playroutine
		lea	(Variables-t,a5),a0
		move.b	d0,(MT_SongPos,a0)
		move.b	#6,(MT_Speed,a0)
		moveq	#0,d0
		move.b	d0,(MT_Counter,a0)
		move.w	d0,(MT_PatternPos,a0)
		lea	(MT_Chan1Temp-t,a5),a0
		move.l	d0,(N_LoopStart,a0)
		move.w	d0,(N_Replen,a0)
		move.w	#$0001,(N_DMABit,a0)
		lea	(MT_Chan2Temp-t,a5),a0
		move.l	d0,(N_LoopStart,a0)
		move.w	d0,(N_Replen,a0)
		move.w	#$0002,(N_DMABit,a0)
		lea	(MT_Chan3Temp-t,a5),a0
		move.l	d0,(N_LoopStart,a0)
		move.w	d0,(N_Replen,a0)
		move.w	#$0004,(N_DMABit,a0)
		lea	(MT_Chan4Temp-t,a5),a0
		move.l	d0,(N_LoopStart,a0)
		move.w	d0,(N_Replen,a0)
		move.w	#$0008,(N_DMABit,a0)
		lea	CUSTOM,a0
		move.w	d0,(AUD0VOL,a0)
		move.w	d0,(AUD1VOL,a0)
		move.w	d0,(AUD2VOL,a0)
		move.w	d0,(AUD3VOL,a0)
		move.w	#$F,(DMACON,a0)
		lea	(np0-t,a5),a0
		move.l	#NP_SIZE*4,d0
		bsr.l	clear.memory
		move.w	#$8200,(np_dmacon-t,a5)
		lea	(sound-t,a5),a0
		move.l	#KOFFER,d0
		bsr.l	clear.memory
		lea	(razure_nous,pc),a0
		move.l	a0,$78.w
		clr.w	(music.on-t,a5)		autorise saut à la playroutine
		rts
