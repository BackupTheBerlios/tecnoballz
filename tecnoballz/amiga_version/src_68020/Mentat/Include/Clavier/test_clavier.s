* ROUTINE A EXECUTEE UNIQUEMENT PENDANT LA VBL *
* Cette routine modifie les "keys-flags" de la table clavier. Si cette
* routine fonctionne hors VBL, l'interruption clavier peut l'interrompre
* en plein travail. Une instruction teste le "key-flag" � 1, mais ce m�me
* "key-flag" (modifi� par l'interruption clavier) peut se retouver � 0
* l'instruction suivante. Cons�quence : La touche est consid�r�e pr�ss�e alors
* qu'en r�alit� elle est relach�e.
*-------------------------------------------------------------------------*
****************> Test si une touche est prise en compte <*****************
*-------------------------------------------------------------------------*
* Entr�e	=> a1.l	pt/key flag
* Sortie	<= d0.l	0=non pris en compte / 1=pris en compte
*		<= Z=1 non pris en compte / Z=0 pris en compte
* Utilise	(d0/a1)
* Modifie	(d0)
TC1		=25				key repeat delay
TC2		=TC1+2				key repeat speed
test_clavier:	
		tst.b	(a1)			touche pr�ss�e ?
		beq.s	.no_flag		-> non
		moveq	#1,d0			d0=1=touche prise en compte
		cmp.b	(a1),d0			1ere pression ?
		beq.s	.ok_press		-> oui, pris en compte
		cmp.b	#TC1,(a1)		infer. delai de r�p�tition ?
		blt.s	.no_press		-> oui, pas pris en compte
		beq.s	.ok_press		-> �gal, pris en compte
		cmp.b	#TC2,(a1)		fin compteur de r�p�tion ?
		bne.s	.no_press		-> non
		move.b	#TC1-1,(a1)
		* Touche non prise en compte *
.no_press	add.b	d0,(a1)			inc. compteur
.no_flag	moveq	#0,d0			d0=0=touche non prise en compte
		rts				<-| sort routine
		* Touche prise en compte *
.ok_press	add.b	d0,(a1)			inc. compteur
		rts				<-| sort routine
