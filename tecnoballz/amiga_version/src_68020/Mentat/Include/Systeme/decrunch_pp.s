*�������������������������������������������������������������������������*
*******************> DECOMPACTE UN FICHIER POWERPACKE <********************
*�������������������������������������������������������������������������*
* Entr�e	=> d0.l	adresse du fichier powerpack�
*		=> d1.l	taille du fichier powerpack�
*		=> d2.l	exigence m�moire (chip ou fast)
* Sortie	<= d0.l	Adr. fichier d�compact�
*		<= d1.l	Taille fichier d�compact�
* 		<= d7.l=0 / Z=1 Pas d'erreur
decrunch_pp:
		move.l	d0,a0
		cmp.l	#'PP20',(a0)
		bne.s	.error
		lea	(.adr_fichierpp,pc),a0
		move.l	d0,(a0)
		lea	(.size_pp,pc),a0
		move.l	d1,(a0)
		lea	(.mem_requester,pc),a0
		move.l	d2,(a0)
		bsr	.calcul_taille
		bsr	.reserve_memory
		bne.s	.exit
		bsr	.decompacte
		bne.s	.exit
		move.l	(.adr_fichier,pc),d0	Adr. fichier d�compact�
		move.l	(.taille_fichier,pc),d1	Taille fichier d�compact�
		moveq	#0,d7
.exit		rts
.error		moveq	#-28,d7
		rts

*---------------> Calcul la taille de l'image d�compact� <----------------*
.calcul_taille
		move.l	(.adr_fichierpp,pc),a2	a2=adr. d�but fichier pp
		move.l	a2,a0
		add.l	(.size_pp,pc),a0	a0=Adr. fin fichier pp
		lea	(.adr_endpp,pc),a1
		move.l	a0,(a1)
		moveq	#0,d3
		move.w	(-4,a0),d3
		lsl.l	#8,d3
		or.b	(-2,a0),d3		d3=Taille fichier d�compact�
		lea	(.taille_fichier,pc),a1
		move.l	d3,(a1)
		rts

*--------------> Reserve m�moire pour fichier d�compact� -----------------*
* entr�e	=> a5.l	adr. 't'
.reserve_memory
		move.l	(.taille_fichier,pc),d0	d0=taille m�moire � reserver
		move.l	(.mem_requester,pc),d1	d1=m�moire chip/fast/public
		bsr	reserve_memoire
		lea	(.adr_fichier,pc),a0
		move.l	d0,(a0)
		beq.s	.out_of_memory
		moveq	#0,d7
		rts
.out_of_memory	moveq	#-12,d7			�d�pass. m�moire
		rts

*------------------> D�compacte le fichier powerpack� <-------------------*
.decompacte
		move.l	(.adr_fichierpp,pc),a2	a2=adr. d�but fichier pp 
		move.l	(.adr_endpp,pc),a0	a0=adr. fin fichier pp
		move.l	(.adr_fichier,pc),a1	a1=adr. d�compactage
		bsr	decompacte_pp
		tst.l	d7
		rts

*-------------------------> adresses utilis�es <--------------------------*
		CNOP	0,4
.adr_fichierpp	dc.l	0		Adr. fichier powerpack�
.adr_endpp	dc.l	0		Adr. fin fichier powerpack�
.size_pp	dc.l	0		Taille fichier powerpack�
.adr_fichier	dc.l	0		Adr. fichier d�compact�
.taille_fichier	dc.l	0		Taille fichier d�compact�
.mem_requester	dc.l	0		Type m�moire Chip/Fast/Public

*�������������������������������������������������������������������������*
*******************> DECOMPACTE UN FICHIER POWERPACKE <********************
*�������������������������������������������������������������������������*
* Entr�e	=> d0.l	adresse du fichier powerpack�
*		=> d1.l	taille du fichier powerpack�
*		=> a4.l	structure RESA
* Sortie	<= d0.l	Adr. fichier d�compact�
*		<= d1.l	Taille fichier d�compact�
* 		<= d7.l=0 / Z=1 Pas d'erreur
decrunch.pp:
		move.l	d0,a2
		cmp.l	#'PP20',(a2)
		bne.s	.no_pp20
		move.l	a2,a0
		add.l	d1,a0			a0=Adr. fin fichier pp
		moveq	#0,d0
		move.w	(-4,a0),d0
		lsl.l	#8,d0
		or.b	(-2.w,a0),d0		d0=Taille fichier d�compact�
		move.l	d0,(4.w,a4)

		*/ R�serve la m�moire */
		bsr	reserve.mem
		bne.s	.exit

		*/ D�compacte le fichier */
		move.l	(a4),a1			a1=adr. d�compactage
		bsr	decompacte_pp

		*/ Retour des valeurs */
		move.l	(a4),d0		Adr. fichier d�compact�
		move.l	(4.w,a4),d1	Taille fichier d�compact�
.ok		moveq	#0,d7
.exit		rts

		*/ Fichier non powerpack� simple copie */
.no_pp20	move.l	d1,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne.s	.exit
		move.l	d0,a0
		move.l	(a4),a1
		move.l	d1,d0
		bsr	copy.memory
		move.l	(a4),d0		Adr. fichier d�compact�
		move.l	(4.w,a4),d1	Taille fichier d�compact�
		moveq	#0,d7
		rts
