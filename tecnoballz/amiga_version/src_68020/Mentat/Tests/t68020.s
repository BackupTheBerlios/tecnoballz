
	opt	p=68020

	moveq	#1,d0


	jmp	([table,pc,d0*4])
	rts




table	dc.l	ze
	dc.l	un
	dc.l	de
	dc.l	tr

ze:	moveq	#0,d0
	rts
un:	moveq	#1,d0
	rts
de:	moveq	#2,d0
	rts
tr:	moveq	#3,d1
	rts


