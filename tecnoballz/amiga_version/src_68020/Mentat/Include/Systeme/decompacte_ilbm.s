* /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  *
*/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/*
********************>* Décompacte une image IFF ILBM *<********************
* /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  /\  *
*/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/  \/*
* Entrée	=> a2.l	adresse table couleur / 0 si pas de table couleur
*		=> a3.l	adresse décompacte    / 0 si pas de décompactage
*		=> a4.l	adresse fichier IFF
*		=> a5.l	adresse de la table 't'
* Sortie	<= d0.w	largeur de l'image en pixels
*		<= d1.w	hauteur de l'image en lignes
*		<= d2.l	mot bas nombre de plan de l'image/mot haut flag HAM
*		<= d3.l	taille de l'image en octet
*		<= d4.w	format de compactage
*		<= d5.l	mot faible hauteur écran/mort fort largeur écran pixel
* 		<= d7.l	¤00	pas d'erreur
*			¤-2	fichier non IFF
*			¤-3	fichier non ILBM
*			¤-4	chunk non trouvé
*			¤-5	format compression inconnue
decompacte_ilbm:
		bsr	.test_FORM
		bne.s	.exit
		bsr.s	.save_sizeIFF
		bsr.s	.test_ILBM
		bne.s	.exit
		bsr	.decode_CMAP
		bne.s	.exit
		bsr	.decode_BMHD
		bne.s	.exit
		movem.l	d0-d5,-(a7)
		bsr	.decode_BODY
		movem.l	(a7)+,d0-d5
.exit		tst.l	d7
		rts

*----------------* DEC IFF : test si le fichier est IFF *-----------------*
* entrée	=> a4.l	adr. fichier IFF
.test_FORM:	
		cmp.l	#'FORM',(a4)+
		bne.s	.no_IFF
		moveq	#0,d7
		rts
.no_IFF		moveq	#-2,d7			¤fichier non IFF
		rts

*----------------* DEC IFF : test si le fichier est ILBM *----------------*
* entrée	=> a4.l	adr. fichier IFF
.test_ILBM:	
		cmp.l	#'ILBM',(a4)+		fichier graphique ? 
		bne.s	.no_ILBM		-> non
		moveq	#0,d7
		rts
.no_ILBM	moveq	#-3,d7			¤fichier non ILBM
		rts

*--------------* DEC IFF : sauve la taille du fichier IFF *---------------*
* entrée	=> a4.l	adr. fichier IFF
.save_sizeIFF:	
		move.l	(a4)+,d6
		rts

*---------------------* DEC IFF : cherche un chunk *----------------------*
* entrée	=> d1.l	nom du chunk
*		=> d6.l	taille fichier IFF
*		=> a4.l	adr. fichier IFF
* sortie	<= a0.l	pt/nom du chunk
* utilise	d1/d2/d6/d7/a0/a1/a4
.cherche_chunk:
		move.l	a4,a0			début du fichier
		lea	(a4,d6.l),a1		fin du fichier
.chunk_loop	cmp.l	(a0),d1			identificateur ?
		beq.s	.chunk_found		oui, ok
		addq.l	#2,a0			prochain long mot
		cmp.l	a0,a1			fin du fichier ?
		bge.s	.chunk_loop		-> non
		moveq	#-4,d7			¤chunk non trouvé
		rts
.chunk_found	moveq	#0,d7
		rts

*-------------------* DEC IFF : décode le chunk CMAP *--------------------*
* entrée	d6.l	taille fichier IFF
*		a2.l	adr. table couleur
*		a4.l	adr. fichier IFF
* sortie	<= d0.l	taille du chunk 'CMAP'
.decode_CMAP:
		move.l	#'CMAP',d1
		bsr.s	.cherche_chunk		|-> cherche chunk CMAP
		bne	.CMAP_exit
		addq	#4,a0			saute le nom du chunk
		move.l	(a0)+,d0		taille chunk 'CMAP'
		move.l	a2,d1
		tst.l	d1			table couleur ?
		beq.s	.CMAP_exit		-> non
		move.l	d0,-(a7)		sauve taille chunk 'CMAP' (pour le test HAM8)
		moveq	#3,d5			couleur sur 24 bits (3 octets)
.CMAP_loop	clr.b	(a2)+			4 bits inutilisés
		move.b	(a0)+,(a2)+		valeur du rouge
		move.b	(a0)+,(a2)+	 	valeur du vert
		move.b	(a0)+,(a2)+ 		valeur du bleu
		movem.l	d0-d2,-(a7)
		move.l	(-4,a2),d2
		bsr	aga_color
		move.w	d0,(-4,a2)
		move.w	d1,(-2,a2)
		movem.l	(a7)+,d0-d2
		sub.l	d5,d0
		bne.s	.CMAP_loop
		move.l	(a7)+,d0		restitue taille chunk 'CMAP' (pour le test HAM8)
		moveq	#0,d7
.CMAP_exit:	rts

*-------------------* DEC IFF : analyse le chunk BMHD *-------------------*
* entrée	=> d0.l	taille chunk 'CMAP'
*		=> d6.l	taille fichier IFF
*		=> a4.l	adr. fichier IFF
* sortie	<= d0.w	largeur de l'image en pixel
*		<= d1.w	hauteur de l'image en pixel
*		<= d2.l	mot bas nombre de plan de l'image/mot haut flag HAM
*		<= d3.l	taille de l'image en octet
*		<= d4.w	format de compactage
*		<= d5.l	mot faible hauteur écran/mort fort largeur écran pixel
*		<= d7.l	0 = pas d'erreur
.decode_BMHD:
		move.l	#'BMHD',d1
		bsr.s	.cherche_chunk		|-> cherche chunk BMHD
		bne	.BMHD_exit
		addq.l	#8,a0			saute nom & taille chunk
		moveq	#0,d1
		moveq	#0,d2
		moveq	#0,d3
		move.l	d0,d4			d4=taille chunk 'CMAP' (pour le test HAM8)
		moveq	#0,d0
		move.w	(a0)+,d0		d0=largeur image en pixels
		move.w	(a0)+,d1		d1=hauteur image en pixels
		addq.l	#4,a0			saute la position écran
		move.b	(a0)+,d2		d2=nombre de plans
		bsr	.test_si_HAM8
		addq.l	#1,a0			saute type de masque
		moveq	#0,d4
		move.b	(a0)+,d4		d4=type de compression
		addq.l	#5,a0			saute certaines informations
		move.w	(a0)+,d5		d5=largeur fenêtre écran en pixels
		swap	d5
		move.w	(a0)+,d5		d5=hauteur fenêtre écran en pixels
		move.w	d0,d3			d3=largeur image en pixels
		lsr.w	#3,d0			d0=largeur image en octets
		and.w	#$FFFE,d0		octet paire
		and.w	#$000F,d3		largeur multiple de 16 ?
		beq.s	.paire			-> oui		
		addq.w	#2,d0			non, rajoute un mot de 16 bits
.paire		move.w	d0,d3			d3=largeur image en octets
		lsl.w	#3,d0			d0=Largeur image en pixels
		mulu.w	d2,d3			d3=taille image decompactée
		mulu.l	d1,d3			d3=taille plan en raw normal
		cmp.w	#1,d4
		bhi.s	.type_compress
		moveq	#0,d7
.BMHD_exit:	rts
.type_compress	moveq	#-5,d7			¤format compression inconnue
		rts

*---------------------* DEC IFF : Test si mode HAM *----------------------*
* entrée	=> d2.w	Nombre de plan de 1 à 6
*		=> d4.l	Taille du chunk 'CMAP'
* sortie	<= d2.l	Mot bas nbre de plan/mot haut flag HAM
.test_si_HAM8
		cmp.w	#8,d2			8 plans ?
		bne.s	.pas_HAM8		-> Non
		cmp.l	#$300,d4		256 Registres couleurs ?
		beq.s	.pas_HAM8		-> Oui
		swap	d2
		move.w	#1,d2			d2=Flag mode HAM
		swap	d2
.pas_HAM8	rts

*-----------------* DEC IFF : décompacte le chunk BODY *------------------*
* entrée	=> d4.w	format de compactage
*		=> d3.l	taille image décompacté
*		=> d6.l	taille fichier IFF
*		=> a3.l	adr. décompactage / 0 si pas de décompactage
*		=> a4.l	adr. fichier IFF
.decode_BODY:
		move.l	#'BODY',d1
		bsr	.cherche_chunk		|-> cherche chunk BODY
		bne	.BODY_exit
		addq.l	#4,a0			saute nom du chunk
		move.l	a3,d0
		tst.l	d0			décompactage ?
		beq.s	.BODY_exit		-> non
		move.l	a3,a1			a1=adr. page graphique
		addq.l	#4,a0
		tst	d4			fichier compacté ByteRun1 ?
		beq	.nocrunch		-> non

		* routine testant la fin du chunk BODY *
* entrée	=> d3.l	taille page graphique
*		=> a0.l	adr. premier octet compacté
*		=> a1.l	adr. premier octet page graphique
.decomp_loop	moveq	#0,d1
.read		move.b	(a0)+,d1		octet de commande
		cmp.b	#$80,d1			code nul ?
		beq.s	.read			-> oui
		tst.b	d1
		bmi.s	.duplication
.copie_loop	move.b	(a0)+,(a1)+
		subq.l	#1,d3
		beq.s	.stop
		dbf	d1,.copie_loop
		bra.s	.decomp_loop
.duplication	neg.b	d1
		move.b	(a0)+,d6		d6=octet à dupliquer
.dupli_loop	move.b	d6,(a1)+
		subq.l	#1,d3
		beq.s	.stop
		dbf	d1,.dupli_loop
		bra.s	.decomp_loop
.stop		bra.s	.BODY_exit		-> fin décompactage

		* fichier non compacté *
.nocrunch	move.b	(a0)+,(a1)+
		subq.l	#1,d3
		bne.s	.nocrunch		-> non
.BODY_exit	rts


* ANCIENNE ROUTINE DECOMPACTAGE PLUS RAPIDE MAIS MOINS FIABLE
*		- boucle de décompactage byterun 1 -
*		move.l	(a0)+,d0		d0 = taille du chunk BODY
*		moveq	#1,d3
*		moveq	#2,d2
*.decomp_loop	moveq	#0,d1
*		move.b	(a0)+,d1		octet de commande
*		bmi.s	.duplication
*		sub.l	d1,d0			d0-(nbre d'octets à copier-1)
*.copie_loop	move.b	(a0)+,(a1)+
*		dbf	d1,.copie_loop
*		sub.l	d2,d0			d0-2 octets
*		bne.s	.decomp_loop
*		bra.s	.BODY_exit		-> fin décompactage
*.duplication	neg.b	d1
*		move.b	(a0)+,d6		d6=octet à dupliquer
*.dupli_loop	move.b	d6,(a1)+
*		dbf	d1,.dupli_loop
*		sub.l	d2,d0			d0-2 octets
*		bne.s	.decomp_loop
*.stop_la	bra.s	.BODY_exit		-> fin décompactage
*
*		- fichier non compacté -
*.nocrunch	moveq	#1,d3
*		move.l	(a0)+,d0		d0=taille du chunk BODY
*.lnocrunch	move.b	(a0)+,(a1)+
*		sub.l	d3,d0
*		bne.s	.lnocrunch
*.BODY_exit	rts
