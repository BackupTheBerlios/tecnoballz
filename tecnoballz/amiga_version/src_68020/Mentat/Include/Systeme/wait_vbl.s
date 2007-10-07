*------->							  <-------*
*»»»»»»»» Attend une nouvelle interruption du temps mort vertical ««««««««*
*------->							  <-------*
* Entrée	=> a5.l	Adr. 't'
wait_vbl:
		move.w	(vbl_counter-t,a5),d0	d0=n° dernière VBL
.wait_vbl	cmp.w	(vbl_counter-t,a5),d0	Nouvelle VBL passée ?
		beq.s	.wait_vbl		-> Non
		rts
