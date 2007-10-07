*=========================================================================*
*======================* SYSTEME : RESERVE MEMOIRE *======================*
*=========================================================================*
* Entr�e	=> a4.l	pt/chaine RESA (adresse,taille,identificateur,type)
*		=> a5.l	adr. table 't'
* Sortie	<= Z=1 / d7.l=0 pas d'erreur
reserve.mem:
		movem.l	d0/d1/a0,-(a7)
		move.l	(a4),d0			d0=adresse zone m�moire
		beq.s	.new			-> nul pas encore r�serv�
		bsr	cherche.zone		|-> cherche la zone dans la liste
		bne.s	.new			-> zone pas trouv�
		move.l	(a4),d1
		sub.l	(a0),d1
		move.l	(4.w,a0),d0		taille m�moire d�j� r�serv�e
		sub.l	d1,d0
		sub.l	#4*4,d0
		cmp.l	(4.w,a4),d0		identique � nouvelle taille ?
		beq.s	.clear			-> oui
		move.l	a4,a0
		bsr	libere_mem
		bra.s	.new

		*/ Efface la m�moire */
.clear		move.l	(a4),a0			pointeur m�moire � effacer
		move.l	(4.w,a4),d0		taille m�moire
		bsr	clear.memory
		bra.s	.stop

		*/ Reserve une nouvelle zone m�moire */
.new		move.l	(4,a4),d0		d0=taille � r�server
		move.l	(12,a4),d1		d1=type de m�moire
		bsr	reserve_memoire
		move.l	d0,(a4)			sauve adresse m�moire
		beq.s	.outmem			-> nul, erreur d'allocation
		move.l	(mem_liste.adr-t,a5),a0
		move.l	(a0)+,d0		d0=nombre de zones m�moire
		subq.l	#1,d0
		lsl.l	#4,d0			*16 (4 longs mots)		
		add.l	d0,a0			pointeur sur la derni�re zone
		move.l	a4,(8.w,a0)		sauve pointeur
		move.l	(8.w,a4),(12.w,a0)	sauve identificateur

		*/ Sort de la routine */
.stop		moveq	#0,d7
.exit		movem.l	(a7)+,d0/d1/a0
		tst.l	d7
		rts

		*/ D�bordement m�moire */
.outmem		moveq	#-12,d7			�D�passement m�moire
		bra.s	.exit
