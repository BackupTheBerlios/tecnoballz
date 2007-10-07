;=========================================================================;
;		Routines qui incrémenentent le crédit
;=========================================================================;

		*-=- + 30 crédits -=-*
jupiter30	lea	(jupiter10,pc),a0
		jsr	(a0)
		jsr	(a0)
		jmp	(a0)

		*-=- + 20 crédits -=-*
jupiter20	lea	(jupiter10,pc),a0
		jsr	(a0)
		jmp	(a0)

		*-=- + 1 crédit -=-*
jupiter:	cmp.w	#9,(credit0-t,a5)
		bge.s	.zofy4
		addq.w	#1,(credit0-t,a5)
		rts
.zofy4		clr.w	(credit0-t,a5)

		*-=- + 10 crédits -=-*
jupiter10	cmp.w	#9,(credit1-t,a5)
		bge.s	.zofy5
		addq.w	#1,(credit1-t,a5)
		rts
.zofy5		clr.w	(credit1-t,a5)

		*-=- + 100 crédits -=-*
jupiter100	cmp.w	#9,(credit2-t,a5)
		bge.s	.zofy6
		addq.w	#1,(credit2-t,a5)
		rts
.zofy6		clr.w	(credit2-t,a5)

		*-=- + 1000 crédits -=-*
jupiter1000	cmp.w	#9,(credit3-t,a5)
		bge.s	.zofy7
		addq.w	#1,(credit3-t,a5)
		rts
.zofy7		clr.w	(credit3-t,a5)

		*-=- + 10000 crédits -=-*
jupiter10000	cmp.w	#9,(credit4-t,a5)
		bge.s	.zofy8
		addq.w	#1,(credit4-t,a5)
		rts
.zofy8		clr.w	(credit4-t,a5)

		*-=- + 100000 crédits -=-*
jupiter100000	addq.w	#1,(credit5-t,a5)
		rts
