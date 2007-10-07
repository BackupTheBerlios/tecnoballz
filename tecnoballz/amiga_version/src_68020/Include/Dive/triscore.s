*=========================================================================*
*���������������������������� Trie les scores ����������������������������*
*=========================================================================*
* Entr�e	=> a5.l	adr. table 't'
*		=> a0.l	adr. liste joueur
* Sortie	<= d7.l	0 non class� / -1 class�	
triscore:
		tst.w	(sh.code-t,a5)
		bne.s	.triche
		cmp.l	#ETB,(triche-t,a5)
		beq.s	.triche
		bsr	meme_nom		|-> Un score par nom !
		beq.s	.triche			-> Joueur � un score sup�rieur
		move.l	(pt_score-t,a5),a1
		move.w	(hard-t,a5),d0
		mulu	#OFF.SCORE,d0
		lea	(IS+KS+NS+4,a1,d0),a1
		moveq	#TS-1,d1		nbre de scores � trier
.remus		lea	(J.SCORE,a0),a2		adr. score du joueur
		lea	(7,a1),a3		adr. score table score
		moveq	#5,d2			6 caract�res par score
.romulus	move.b	(a2)+,d0
		add.b	#'\'-65,d0
		cmp.b	(a3)+,d0
		bhi.s	.cesar			-> joueur � plus de points
		bne.s	.brutus
		dbf	d2,.romulus
.brutus		lea	(NS,a1),a1
		dbf	d1,.remus
.triche		moveq	#0,d7
		bra	.triscore_exit		-> sort

****NAME01-888888--i0----00-
****NAME02-888888--i0----00-
****NAME01-888888--i0----00-


		*/ d�cale les autres scores */
.cesar		move.l	(pt_score-t,a5),a3
		move.w	(hard-t,a5),d0
		mulu	#OFF.SCORE,d0
		lea	(IS+KS+NS+4,a3,d0),a3
		lea	(09*NS,a3),a2
		lea	(10*NS,a3),a3
		move.w	d1,d0			d0=position du joueur
		subq.w	#1,d0
		bmi.s	.rome
.egypte		moveq	#NS-6,d2
		subq.l	#5,a2
		subq.l	#5,a3
.cleopatre	move.b	-(a2),-(a3)
		dbf	d2,.cleopatre
		dbf	d0,.egypte

		*/ recopie nom dans table score */
.rome		moveq	#6-1,d2
.legion		move.b	(a0)+,(a1)+		lit code caract�re normal
		dbf	d2,.legion

		*/ recopie score */
		addq.l	#1,a1			saute l'espace s�pare score
		moveq	#5,d2
.legion_ere	move.b	(a0)+,d0		lit code caract�re normal
		add.b	#'\'-65,d0		code de "J"
		move.b	d0,(a1)+		sauve code de "J"
		dbf	d2,.legion_ere

		*/ recopie n� area */
		addq.l	#3,a1			saute 3 espaces s�parent n� area
		move.b	(a0)+,d0		lit code caract�re normal
		add.b	#'\'-65,d0		code de "J"
		move.b	d0,(a1)+		sauve code de "J"

		*/ recopie n� tableau */
		addq.l	#4,a1			saute 4 espaces s�parent n� tableau
		moveq	#2-1,d2
.legion_ere7	move.b	(a0)+,d0		lit code caract�re normal
		add.b	#'\'-65,d0		code de "J"
		move.b	d0,(a1)+		sauve code de "J"
		dbf	d2,.legion_ere7
		bsr.l	megadrive
		moveq	#-1,d7
.triscore_exit	rts

*-------------------------------------------------------------------------*
*-------------------* Cherche si le joueur exite d�j� *-------------------*
*-------------------------------------------------------------------------*
* Entr�e	=> a5.l	Adr. 't'
* Sortie 	<= Z=1/d7=0 alors le joueur est d�j� class� avec 
*			un meilleur score ne surtout pas le reclasser
meme_nom:
		*/ TEST SI LE NOM EST DEJA DANS LA TABLE */
		move.l	(pt_score-t,a5),a1
		move.w	(hard-t,a5),d0
		mulu	#OFF.SCORE,d0
		lea	(IS+KS+NS+4,a1,d0),a1
		moveq	#TS-1,d1		nbre de scores 
.remus		move.l	a0,a2			adr. nom du joueur
		move.l	a1,a3			adr. nom table score
		moveq	#5,d2			6 caract�res par nom
.romulus	move.b	(a2)+,d0
		cmp.b	(a3)+,d0		nom identique ?
		bne.s	.brutus			-> non
		dbf	d2,.romulus
		bra.s	.cestbon
.brutus		lea	(NS,a1),a1
		dbf	d1,.remus
		moveq	#1,d7			Le nom n'existe pas on peut...
		bra.s	.exit			...peut-�tre classer le joueur 

	*/ LE NOM EXISTE DANS LA TABLE, TEST SI LE SCORE EST SUPERIEUR */
.cestbon	lea	(J.SCORE,a0),a2		adr. score du joueur
		lea	(7,a1),a3		adr. score table score
		moveq	#5,d2			6 caract�res par score
.romulus2	move.b	(a2)+,d0
		add.b	#'\'-65,d0
		cmp.b	(a3)+,d0
		bhi.s	.cesar			-> joueur � plus de points
		bne	.itak			-> Score inf�rieur
		dbf	d2,.romulus2
.itak		moveq	#0,d7			Le joueur existe mais... 
		bra.s	.exit			...avec un score inf�rieur

	*/ ECRASE LE NOM DE LA TABLE (POUR LE RECLASSER) */
.cesar		move.l	a1,a2
		lea	(NS,a2),a3
		move.w	d1,d0			d0=position du joueur
		subq.w	#1,d0
		bmi.s	.rome
.egypte		moveq	#NS-6,d2
.cleopatre	move.b	(a3)+,(a2)+
		dbf	d2,.cleopatre
		addq.l	#5,a2
		addq.l	#5,a3
		dbf	d0,.egypte
.rome		moveq	#1,d7			On reclasse le joueur
.exit		rts
