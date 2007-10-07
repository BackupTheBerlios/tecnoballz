;--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-;
;************************** SUPPRIME LES LUTINS **************************;
;--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*--*-;
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l	CUSTOM
sprites_off:	and.w	#$FFDF,(dmacon-t,a5)
sprites_off2	move.w	#$0020,(DMACON,a6)	Bit SPREN = 0
		lea	(SPR0PTH,a6),a0
		moveq	#0,d0
		move.w	#(COLOR00-SPR0PTH)/2-1,d1
.loop		move.w	d0,(a0)+
		dbf	d1,.loop
		rts

		*/ Autorise les lutins */
sprites_on:	or.w	#$0020,(dmacon-t,a5)
		move.w	#$8020,(DMACON,a6)
		rts
