*------->							  <-------*
*�������� Attend une nouvelle interruption du temps mort vertical ��������*
*------->							  <-------*
* Entr�e	=> a5.l	Adr. 't'
wait_vbl:
		move.w	(vbl_counter-t,a5),d0	d0=n� derni�re VBL
.wait_vbl	cmp.w	(vbl_counter-t,a5),d0	Nouvelle VBL pass�e ?
		beq.s	.wait_vbl		-> Non
		rts
