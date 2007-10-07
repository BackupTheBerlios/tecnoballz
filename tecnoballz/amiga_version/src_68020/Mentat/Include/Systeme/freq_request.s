***************************************************************************
***************** DEMANDE LA FREQUENCE DE BALAYAGE ECRAN ******************
***************************************************************************
freq_request:
		move.w	#5,(mentat_nline-t,a5)
		lea	(.text,pc),a0
		clr.l	(mentat_routine-t,a5)
		bsr	mentat_info
		tst.l	d7
		bne.s	.exit
		move.w	#50,(frequence-t,a5)
		moveq	#$0020,d1
		tst.w	d0
		beq.s	.pal
		move.w	#60,(frequence-t,a5)
		moveq	#0,d1
.pal		move.w	d1,(BEAMCON0,a6)
.exit		rts
.text:		dc.b "                              MENTAT REQUESTER                                  "
		dc.b "                                                                                "
		dc.b "  BOUTON GAUCHE SOURIS : Force fréquence balayage écran >50Hz< (standard  PAL)  "
		dc.b "                                                                                "
		dc.b "  BOUTON DROIT  SOURIS : Force fréquence balayage écran >60Hz< (standard NTSC)  "
		EVEN
