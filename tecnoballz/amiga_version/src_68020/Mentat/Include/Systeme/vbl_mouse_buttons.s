;===============»					  «===============;
;»»»»»»»»»»»»»»»»» VBL : test les 3 boutons des 2 souris «««««««««««««««««;
;===============»					  «===============;
* Entrée	=> a5.l	Adr. 't'
*		=> a6.l	CUSTOM
vbl_mouse_buttons:
		* test boutons : bouton gauche souris port 0 *
		lea	(1+mouse0_left-t,a5),a0
		move.b	CIAA_PRA,d2
		btst	#6,d2			CIA test bit de port 6
		seq	-(a0)

		* test boutons : bouton gauche souris port 1 *
		btst	#7,d2			CIA test bit de port 7
		seq	-(a0)

		* test boutons : bouton droit souris port 0 *
		move.w	(POTGOR,a6),d2
		btst	#10,d2			Test bit DATLY
		seq	-(a0)

		* test boutons : bouton droit souris port 1 *
		btst	#14,d2			Test bit DATRY
		seq	-(a0)

		* test boutons : bouton trois souris port 0 *
		btst	#8,d2			Test bit DATLX
		seq	-(a0)

		* test boutons : bouton trois souris port 1 *
		btst	#12,d2			Test bit DATRX
		seq	-(a0)
		move.w	#%1111111100000000,(POTGO,a6)
		rts
