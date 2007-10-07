*=========================================================================*
*===================* SYSTEME : lib�re de la m�moire *====================*
*=========================================================================*
* Entr�e	=> d0.l	adresse m�moire � lib�rer
* 		=> a5.l	adr. table 't'
* Sortie	<= d7.l	� 0  lib�ration effectu�e
*			�-10 lib�ration �chou�e
libere_memoire:
		* Cherche la zone concern�e *
		moveq	#0,d7
		tst.l	d0			Adresse m�moire nulle ?
		beq	.exit			-> oui, sort
		movem.l	d0-d6/a0-a6,-(a7)
		bsr	cherche.zone
		bne	.error

		* r�oganise la liste m�moire *
		move.l	d0,a1			adresse m�moire � lib�rer
		move.l	(4,a0),d0		taille m�moire � lib�rer
		moveq	#0,d2
		move.l	d2,(a0)			raz adresse
		move.l	d2,(4,a0)		raz taille
		move.l	d2,(8,a0)		raz le pointeur
		move.l	d2,(12,a0)		raz l'identificateur
		subq.w	#1,d1
		bmi.s	.freememory		-> dernier de la liste
		lea	(16,a0),a2
.loop_organise	move.l	(a2),(a0)+		remonte adresse
		move.l	d2,(a2)+		efface adresse
		move.l	(a2),(a0)+		remonte taille
		move.l	d2,(a2)+		efface taille
		move.l	(a2),(a0)+		remonte pointeur
		move.l	d2,(a2)+		efface pointeur
		move.l	(a2),(a0)+		remonte identificateur
		move.l	d2,(a2)+		efface identificateur
		dbf	d1,.loop_organise

		* lib�re la m�moire *
.freememory	cmp.l	#'MENT',(a1)
		bne.s	.error38
		cmp.l	#'AT*M',(4,a1)
		bne.s	.error38
		cmp.l	#'EM*S',(8,a1)
		bne.s	.error38
		cmp.l	#'TART',(12,a1)
		bne.s	.error38
		cmp.l	#'STOP',(-16,a1,d0.l)
		bne.s	.error37
		cmp.l	#'*MEN',(-12,a1,d0.l)
		bne.s	.error37
		cmp.l	#'TAT*',(-08,a1,d0.l)
		bne.s	.error37
		cmp.l	#'MEM*',(-04,a1,d0.l)
		bne.s	.error37
		move.l	4.w,a6
		jsr	(FreeMem,a6)		|-> lib�re m�moire
		move.l	(mem_liste.adr-t,a5),a3
		subq.l	#1,(a3)			nbre de zone - 1
		moveq	#0,d7			� lib�ration effectu�e
.sortie		movem.l	(a7)+,d0-d6/a0-a6
.exit		tst.l	d7
		rts
.error		moveq	#-10,d7			� lib�ration �chou�e
		bra.s	.sortie
.error37	moveq	#-37,d7			d�bordement m�moire apr�s
		bra.s	.ici
.error38	moveq	#-38,d7			d�bordement m�moire avant
.ici		lea	(sys.adr,pc),a0
		move.l	a1,(a0)
		lea	(sys.siz,pc),a0
		move.l	d0,(a0)
		moveq	#8-1,d1
		lea	(alert.info+7,pc),a0
		move.l	(sys.adr,pc),d3
		add.l	#4*4,d3
		bsr	conv_number_ascii
		move.l	(sys.siz,pc),d2
		sub.l	#4*8,d2
		bsr	convertit_decimal
		moveq	#8-1,d1
		lea	(alert.info+25,pc),a0
		bsr	conv_number_ascii
		bra	.sortie

*--===---===---===---===---===---===---===---===---===---===---===---===--*
*=*=*=*=*=*=*=*=*=*=*=*=*=*=* Lib�re m�moire =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
*--===---===---===---===---===---===---===---===---===---===---===---===--*
* Entr�e	=> a0.l	pt/m�moire � lib�rer
* 		=> a5.l	adr. table 't'
libere_mem:	
		moveq	#0,d7
		move.l	d0,-(a7)
		move.l	(a0),d0
		beq.s	.exit
		bsr	libere_memoire
		clr.l	(a0)
.exit		move.l	(a7)+,d0
		tst.l	d7
		rts

*--===---===---===---===---===---===---===---===---===---===---===---===--*
*-*-*-*-*-*-*-*-*-*-*-*- Cherche une zone m�moire -*-*-*-*-*-*-*-*-*-*-*-*-
*--===---===---===---===---===---===---===---===---===---===---===---===--*
* Entr�e	=> d0.l	adresse m�moire
*		=> a5.l	adr. table 't'
* Sortie	<= d0.l	adresse r�elle m�moire
*		<= d1.l	nombre de zones m�moire -1 apr�s celle ci
*		<= Z=1 / d7=0 zone m�moire trouv�e
*		<= a0.l	pointeur sur la liste m�moire
cherche.zone:
		move.l	a3,-(a7)
		sub.l	#4*4,d0
		move.l	(mem_liste.adr-t,a5),a3
		move.l	(a3),d1			nombre zone(s) m�moire
		beq	.error			-> aucune zone r�serv�e
		subq.w	#1,d1
		lea	(4,a3),a0
.loop		cmp.l	(a0),d0			zone concern�e ?
		beq.s	.found_zone		-> oui
		lea	(16,a0),a0		16 octets par zone m�moire
		dbf	d1,.loop

		* Cherche si c'est un buffer d�compactage powerpaker *
		sub.l	#PP_SIZE,d0
		move.l	(a3),d1			nombre zone(s) m�moire
		beq	.error			-> aucune zone r�serv�e
		subq.w	#1,d1
		lea	(4,a3),a0
.loop_pp	cmp.l	(a0),d0			zone concern�e ?
		beq.s	.found_zone		-> oui
		lea	(16,a0),a0
		dbf	d1,.loop_pp
		bra.s	.error			-> zone non trouv�e
.found_zone	moveq	#0,d7
.sortie		move.l	(a7)+,a3
		tst.l	d7
		rts
.error		moveq	#-10,d7			� Lib�ration �choue
		bra.s	.sortie

