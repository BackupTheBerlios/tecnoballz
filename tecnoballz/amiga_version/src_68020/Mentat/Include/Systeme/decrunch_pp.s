*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*******************> DECOMPACTE UN FICHIER POWERPACKE <********************
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
* Entrée	=> d0.l	adresse du fichier powerpacké
*		=> d1.l	taille du fichier powerpacké
*		=> d2.l	exigence mémoire (chip ou fast)
* Sortie	<= d0.l	Adr. fichier décompacté
*		<= d1.l	Taille fichier décompacté
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
		move.l	(.adr_fichier,pc),d0	Adr. fichier décompacté
		move.l	(.taille_fichier,pc),d1	Taille fichier décompacté
		moveq	#0,d7
.exit		rts
.error		moveq	#-28,d7
		rts

*---------------> Calcul la taille de l'image décompacté <----------------*
.calcul_taille
		move.l	(.adr_fichierpp,pc),a2	a2=adr. début fichier pp
		move.l	a2,a0
		add.l	(.size_pp,pc),a0	a0=Adr. fin fichier pp
		lea	(.adr_endpp,pc),a1
		move.l	a0,(a1)
		moveq	#0,d3
		move.w	(-4,a0),d3
		lsl.l	#8,d3
		or.b	(-2,a0),d3		d3=Taille fichier décompacté
		lea	(.taille_fichier,pc),a1
		move.l	d3,(a1)
		rts

*--------------> Reserve mémoire pour fichier décompacté -----------------*
* entrée	=> a5.l	adr. 't'
.reserve_memory
		move.l	(.taille_fichier,pc),d0	d0=taille mémoire à reserver
		move.l	(.mem_requester,pc),d1	d1=mémoire chip/fast/public
		bsr	reserve_memoire
		lea	(.adr_fichier,pc),a0
		move.l	d0,(a0)
		beq.s	.out_of_memory
		moveq	#0,d7
		rts
.out_of_memory	moveq	#-12,d7			¤dépass. mémoire
		rts

*------------------> Décompacte le fichier powerpacké <-------------------*
.decompacte
		move.l	(.adr_fichierpp,pc),a2	a2=adr. début fichier pp 
		move.l	(.adr_endpp,pc),a0	a0=adr. fin fichier pp
		move.l	(.adr_fichier,pc),a1	a1=adr. décompactage
		bsr	decompacte_pp
		tst.l	d7
		rts

*-------------------------> adresses utilisées <--------------------------*
		CNOP	0,4
.adr_fichierpp	dc.l	0		Adr. fichier powerpacké
.adr_endpp	dc.l	0		Adr. fin fichier powerpacké
.size_pp	dc.l	0		Taille fichier powerpacké
.adr_fichier	dc.l	0		Adr. fichier décompacté
.taille_fichier	dc.l	0		Taille fichier décompacté
.mem_requester	dc.l	0		Type mémoire Chip/Fast/Public

*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*******************> DECOMPACTE UN FICHIER POWERPACKE <********************
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
* Entrée	=> d0.l	adresse du fichier powerpacké
*		=> d1.l	taille du fichier powerpacké
*		=> a4.l	structure RESA
* Sortie	<= d0.l	Adr. fichier décompacté
*		<= d1.l	Taille fichier décompacté
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
		or.b	(-2.w,a0),d0		d0=Taille fichier décompacté
		move.l	d0,(4.w,a4)

		*/ Réserve la mémoire */
		bsr	reserve.mem
		bne.s	.exit

		*/ Décompacte le fichier */
		move.l	(a4),a1			a1=adr. décompactage
		bsr	decompacte_pp

		*/ Retour des valeurs */
		move.l	(a4),d0		Adr. fichier décompacté
		move.l	(4.w,a4),d1	Taille fichier décompacté
.ok		moveq	#0,d7
.exit		rts

		*/ Fichier non powerpacké simple copie */
.no_pp20	move.l	d1,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne.s	.exit
		move.l	d0,a0
		move.l	(a4),a1
		move.l	d1,d0
		bsr	copy.memory
		move.l	(a4),d0		Adr. fichier décompacté
		move.l	(4.w,a4),d1	Taille fichier décompacté
		moveq	#0,d7
		rts
