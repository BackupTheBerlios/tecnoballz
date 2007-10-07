*»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»*
*********************** SAUVE UN FICHIER AMIGAdOS *************************
*»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»*
* Entrée	=> d0.l	adr. du buffer à sauver
*		=> d1.l	nombre d'octet à sauver
*		=> a0.l	pt/le nom du fichier AmigaDos
*		=> a5.l	adr. table 't'
* Sortie	<= d0.l	adr. du buffer à sauver
*		<= d1.l	nombre d'octets effectivement sauvés
*		<= d7.l	¤ 0 / Z=1 pas d'erreur
*			¤-27 impossible d'ouvrir fichier
*			¤-26 erreur d'écriture
*		<= a0.l	pt/le nom du fichier AmigaDos
save_file:
		movem.l	a5/a6,-(a7)
		bsr	.sauve_valeurs	
		bsr	.cherche_dosbase
		bsr	.sys_restore
		bsr	.open_file
		bne.s	.exit			-> impossible d'ouvrir fichier
		bsr	.save_file
		bne.s	.exit			-> erreur de lecture
		bsr	.close_file
		move.l	(.dosbase,pc),a6
		move.l	#50*5,d1
		jsr	(Delay,a6)		|-> Attend 5 secondes
		moveq	#0,d7
.exit		move.l	d7,-(a7)		sauve message d'erreur
		bsr	.sys_gele
		move.l	(a7)+,d7		restore message d'erreur
		move.l	(.adr_save,pc),d0	d0=taille fichier
		move.l	(.size_write,pc),d1	d1=nbre d'octets sauvés
		move.l	(.pt_name,pc),a0		a0=pt/nom du fichier
		movem.l	(a7)+,a5/a6
		tst.l	d7
		rts

*----------------> SAVE FILE : sauve valeurs transmisent <----------------*
.sauve_valeurs:
		lea	(.size_write,pc),a1
		clr.l	(a1)			nombre d'octets sauvés = 0
		lea	(.size_save,pc),a1
		move.l	d1,(a1)			d1=nombre d'octets à sauver
		lea	(.pt_name,pc),a1
		move.l	a0,(a1)			a0=pt/le nom
		lea	(.adr_save,pc),a1
		move.l	d0,(a1)			d0=adr. du buffer
		rts
		
*---------------------> SAVE FILE : cherche dosbase <---------------------*
.cherche_dosbase:
		lea	dosbase-t,a6
		add.l	a5,a6
		move.l	(a6),a6			adr. dosbase
		lea	(.dosbase,pc),a0
		move.l	a6,(a0)
		rts

*-----------------> SAVE FILE : retour sous le système <------------------*
.sys_restore:
		move.w	(flag_sys_gele-t,a5),d0
		lea	(.flag_sys_gele,pc),a0
		move.w	d0,(a0)			sauve flag
		beq.s	.sys_non_gele		-> système non gelé
		bsr	sys_restore		|-> restore le système
.sys_non_gele	rts

*--------------------> SAVE FILE : ouvre le fichier <---------------------*
.open_file:
 		move.l	(.pt_name,pc),d1
		move.l	#1006,d2		1006 = "Mode_new"
		move.l	(.dosbase,pc),a6
		jsr	(Open,a6)
		lea	(.handle,pc),a0
		move.l	d0,(a0)			sauve valeur du handle
		beq.s	.open_erreur		-> si nul, alors erreur
		moveq	#0,d7
		rts
.open_erreur	moveq	#-27,d7			¤ -27=ouverture impossible
		rts

*--------------> SAVE FILE : sauve le fichier en mémoire <---------------*
.save_file:
		moveq	#0,d0
		move.l	(.adr_save,pc),d2	d2=adr. tampon
		beq.s	.write_filexit
		move.l	(.handle,pc),d1
		move.l	(.size_save,pc),d3	d3=nombre d'octets à sauver
		move.l	(.dosbase,pc),a6
		jsr	(Write,a6)
		lea	(.size_write,pc),a0
		move.l	d0,(a0)			d0=nbre d'octets sauvés
		tst.l	d0			erreur de d'écriture ?
		bmi.s	.write_filerror		-> oui
.write_filexit	moveq	#0,d7
		rts
.write_filerror	moveq	#-26,d7			¤ -26 erreur d'écriture
		rts

*--------------------> SAVE FILE : ferme le fichier <---------------------*
.close_file:	
		move.l	(.handle,pc),d1
		move.l	(.dosbase,pc),a6
		jsr	(Close,a6)
		rts

*----------------> SAVE FILE : gèle le système si besoin <----------------*
.sys_gele:
		lea	(.flag_sys_gele,pc),a0
		tst.w	(a0)			système gelé lors de l'appel ?
		beq.s	.non_gele		-> non
		bsr	sys_save
		bsr	first_init
.non_gele	rts

*------------> SAVE FILE : adresse utilisées par la routine <-------------*
		CNOP	0,4
.dosbase	dc.l	0
.pt_name	dc.l	0		pt/le nom
.adr_save	dc.l	0		adr. tampon à sauver
.size_save	dc.l	0		nombre d'octet à sauver
.size_write	dc.l	0		nombre d'octet réélement sauvé
.handle		dc.l	0		handle du fichier
.flag_sys_gele	dc.w	0
