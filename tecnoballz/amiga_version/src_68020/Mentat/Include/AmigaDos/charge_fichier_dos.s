*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
************************** CHARGE UN FICHIER DOS **************************
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
* Entrée	=> a0.l	pt/nom du fichier
*		=> d0.l	exigence mémoire #0 FAST-CHIP, #2 CHIP, #4 FAST
* Sortie	<= d0.l	adresse chargement
*		<= d1.l	taille fichier
*		<= d7.l	#0 / Z=1 si aucune erreur
*		<= a0.l	pt/nom du fichier
charge_fichier_dos:
		lea	(.memory_req,pc),a1
		move.l	d0,(a1)
		lea	(.file_name,pc),a1
		move.l	a0,(a1)
		moveq	#0,d0
		moveq	#0,d1
		bsr	load_file		|-> calcul taille fichier
		tst.l	d7			erreur ?
		bne.s	.error			-> oui
		move.l	(.memory_req,pc),d1
		move.l	d0,-(a7)		sauve taille
		bsr	reserve_memoire
		move.l	(a7)+,d1		d1 = taille fichier
		tst.l	d0			adresse = 0 ?
		beq.s	.out_of_memory		-> oui, erreur
		move.l	(.file_name,pc),a0
		movem.l	d0/d1,-(a7)
		bsr	load_file		(d0 = adresse, d1 = taille)
		movem.l	(a7)+,d0/d1
		tst.l	d7			erreur ?
		bne.s	.error			-> oui
		move.l	(.file_name,pc),a0
		moveq	#0,d7
		rts
.out_of_memory	moveq	#-9,d7			¤ -9 = dépassement mémoire
.error		rts
		CNOP	0,4
.file_name	dc.l	0		pt/le nom du fichier
.memory_req	dc.l	0		exigence mémoire

*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
************************** CHARGE UN FICHIER DOS **************************
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
* Entrée	=> a0.l	pt/nom du fichier
*		=> a4.l pt/RESA
* Sortie	<= d0.l	adresse chargement
*		<= d1.l	taille fichier
*		<= a4.l structure RESA
*		<= d7.l	#0 / Z=1 si aucune erreur
*		<= a0.l	pt/nom du fichier
charge.fichier:
		lea	(.resa,pc),a1
		move.l	a4,(a1)
		lea	(.file_name,pc),a1
		move.l	a0,(a1)
		moveq	#0,d0
		moveq	#0,d1
		bsr	load_file		|-> calcul taille fichier
		bne.s	.error			-> oui
		move.l	(.resa,pc),a4
		move.l	d0,(4.w,a4)
		bsr	reserve.mem
		bne.s	.error
		move.l	(.file_name,pc),a0
		move.l	(.resa,pc),a4
		move.l	(a4),d0			d0=adresse chargement
		move.l	(4.w,a4),d1		d1=taille du fichier
		bsr	load_file		(d0 = adresse, d1 = taille)
		bne.s	.error			-> oui
		move.l	(.file_name,pc),a0
		move.l	(.resa,pc),a4
		move.l	(a4),d0
		move.l	(4.w,a4),d1
		moveq	#0,d7
.error		rts
		CNOP	0,4
.file_name	dc.l	0		pt/le nom du fichier
.resa		dc.l	0		pt/structure RESA
