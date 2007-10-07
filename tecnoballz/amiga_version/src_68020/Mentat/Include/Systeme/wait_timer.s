*�������������������������������������������������������������������������*
**************� Attente temporis�e avec le Timer B du CIA-B �**************
*�������������������������������������������������������������������������*
* Entr�e	=> d0.l	Valeur de l'attente
* Pour attendre une seconde d0.l = 709379
wait_timer:
		move.l	#$FFFF,d1		d1=Valeur maximum accept�e
		cmp.l	d1,d0			d0 d�passe valeur maximum ?
		bhi.s	.go			-> Oui
		move.l	d0,d1
.go		sub.l	d1,d0
		move.b	d1,CIAB_TBLO		Initialise octet haut
		lsr.w	#8,d1
		move.b	d1,CIAB_TBHI		Initialise octet bas
		move.b	#$19,CIAB_CRB		D�marre le Timer
.wait		btst	#0,CIAB_CRB		D�compte termin� ?
		bne.s	.wait			-> Non
		tst.w	d0			Encore au moins un passage ?
		bne.s	wait_timer		-> Oui
		rts
