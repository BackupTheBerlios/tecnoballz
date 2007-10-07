***************************************************************************
********************** QUAND JE BOIS JE VOIS DOUBLE ***********************
***************************************************************************
* Entrée	=> sans frapper
pertedememoire:
		tst.b	(flag.tab-t,a5)
		beq.s	.jxjfhsf
		bsr.l	memory_list
.jxjfhsf	tst.b	(flag.help-t,a5)
		beq.s	.jxjfhsf2
		bsr.l	memory_help
.jxjfhsf2	rts
