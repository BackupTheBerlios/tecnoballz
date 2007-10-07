*=========================================================================*
*              Gère l'option 'less bricks' acheté au magasin		  *
*=========================================================================*
* Entrée	=> a5.l	adr. table 't'
byte_rtel:
		tst.w	(lessbrick-t,a5)	Options 'less bricks' ?
		beq.s	.pub1			-> non
		subq.w	#1,(lessbrick.count-t,a5)
		tst.w	(lessbrick.count-t,a5)
		bne.s	.pub1
		move.w	#10,(lessbrick.count-t,a5)
		subq.w	#1,(lessbrick-t,a5)
		move.b	#1,(sound+S_TOUCHE.BRIK-t,a5)
		bsr	soleil
		subq.w	#1,(brick.count-t,a5)
.pub1		rts
