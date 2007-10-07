*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*
*                         pointe la liste joueur 			  *
*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*
* Sortie	<= a0	pt/liste joeur
pointeplayer:	
		move.w	(player-t,a5),d0
		move.l	(liste_joueur-t,a5,d0*4),a0
		rts
