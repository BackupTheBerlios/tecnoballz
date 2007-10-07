*=========================================================================*
*«««««««««««««««««««««««* RESERVE LISTES JOUEURS *»»»»»»»»»»»»»»»»»»»»»»»»*
*=========================================================================*
* Entrée	=> a5.l	Adr. 't'
init_players:
		*/ Reserve mémoire */
		lea	(pt_player-t,a5),a4
		bsr.l	reserve.mem
		bne.s	.exit

		*/ Sauve Adresse  */
		lea	(pt_player-t,a5),a4
		move.l	(RESA.ADR,a4),a4
		moveq	#NJOUEUR-1,d1
		lea	(4+liste_joueur-t,a5),a2
.loop		move.l	a4,(a2)+
		lea	(J.SIZE,a4),a4
		dbf	d1,.loop
.exit		rts
