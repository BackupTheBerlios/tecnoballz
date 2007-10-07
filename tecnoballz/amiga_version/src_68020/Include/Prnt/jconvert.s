*/////////////////////////////////////////////////////////////////////////*
******************** CONVERTIT FORMAT ASCII EN CODE J *********************
*/////////////////////////////////////////////////////////////////////////*
* Entrée	=> d0.w Nombre de caractères - 1
*		=> a0.l Text ASCII
*		=> a1.l Test J
jconvert:	
		movem.l	d0-d5/a0-a2,-(a7)
		lea	(convert-t,a5),a2
		move.l	a2,d2
		moveq	#(cend-convert)/2-1,d3	d3=Nombre de code caratère J
		moveq	#'['-65,d5		Code espace
.main_loop	move.b	(a0)+,d1		d1=code ASCII
		move.w	d3,d4			d4=Nbre caractère code J
		move.l	d2,a2			adr. table convertion
.convert_loop	cmp.b	(a2)+,d1
		beq.s	.found_code_j
		addq.l	#1,a2
		dbf	d4,.convert_loop
		move.b	d5,(a0)+		code non trouvé = un espace
		bra.s	.end_loop
.found_code_j	move.b	(a2)+,(a1)+		convertit le code ASCII
.end_loop	dbf	d0,.main_loop
		movem.l	(a7)+,d0-d5/a0-a2
		rts

*/////////////////////////////////////////////////////////////////////////*
******************** CONVERTIT FORMAT CODE J EN ASCII *********************
*/////////////////////////////////////////////////////////////////////////*
* Entrée	=> d0.w Nombre de caractères - 1
*		=> a0.l Text J
*		=> a1.l Test ASCII
convertj:	
		movem.l	d0-d5/a0-a2,-(a7)
		lea	(1+convert-t,a5),a2
		move.l	a2,d2
		moveq	#(cend-convert)/2-1,d3	d3=Nombre de code caratère J
		moveq	#' '-65,d5		Code espace
.main_loop	move.b	(a0)+,d1		d1=code J
		move.w	d3,d4			d4=Nbre caractère code J
		move.l	d2,a2			adr. table convertion
.convert_loop	cmp.b	(a2),d1
		beq.s	.found_code_j
		addq.l	#2,a2
		dbf	d4,.convert_loop
		move.b	d5,(a0)+		code non trouvé = un espace
		bra.s	.end_loop
.found_code_j	move.b	-(a2),(a1)+		convertit le code J
.end_loop	dbf	d0,.main_loop
		movem.l	(a7)+,d0-d5/a0-a2
		rts
