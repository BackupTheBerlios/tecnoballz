*=========================================================================*
*********************** Charge un fichier AmigaDos ************************
*=========================================================================*
* Entrée	=> d0.l	adr. de chargement ou 0 si pas de chargement
*		=> d1.l	nombre d'octet à charger
*		=> a0.l	pt/le nom du fichier AmigaDos
*		=> a5.l	adr. table 't'
* Sortie	<= d0.l	taille du fichier
*		<= d1.l	nombre d'octet effectivement chargé
*		<= d7.l	¤ 0 / Z=1 pas d'erreur
*			¤-7 impossible d'ouvrir fichier
*			¤-8 erreur de lecture
*		<= a0.l	pt/le nom du fichier AmigaDos
load_file:
		movem.l	a5/a6,-(a7)
		lea	(infoblock.pt-t,a5),a4
		bsr	reserve.mem
		bne.s	.exit
		bsr	.sauve_valeurs	
		bsr	.cherche_dosbase
		bsr	.sys_restore
		bsr	.lock_file
		bne.s	.exit			-> impossible d'ouvrir fichier
		bsr	.examine_file
		bne.s	.exit			-> erreur lecture
		bsr	.open_file
		bne.s	.exit			-> impossible d'ouvrir fichier
		bsr	.read_file
		bne.s	.exit			-> erreur de lecture
		bsr	.close_file
		bsr	.unlock_file
		moveq	#0,d7
.exit		move.l	d7,-(a7)		sauve message d'erreur
		bsr	.sys_gele
		move.l	(a7)+,d7		restore message d'erreur
		move.l	(infoblock.pt-t,a5),a0
		move.l	(SIZE,a0),d0		d0 = taille fichier
		move.l	(.size_read,pc),d1	d1 = nbre d'octet chargé
		lea	(infoblock.pt-t,a5),a0
		move.l	d7,-(a7)		sauve message d'erreur
		bsr	libere_mem
		move.l	(a7)+,d7
		move.l	(.pt_name,pc),a0	a0 = pt/nom du fichier
		movem.l	(a7)+,a5/a6
		tst.l	d7
		rts

*----------------> LOAD FILE : sauve valeurs transmisent <----------------*
.sauve_valeurs:
		lea	(.size_read,pc),a1
		clr.l	(a1)			nombre d'octet chargé = 0
		lea	(.size_load,pc),a1
		move.l	d1,(a1)			nombre d'octet à charger	
		lea	(.pt_name,pc),a1
		move.l	a0,(a1)			sauve pt/le nom
		lea	(.adr_load,pc),a1
		move.l	d0,(a1)			sauve adr. de chargement
		rts
		
*---------------------> LOAD FILE : cherche dosbase <---------------------*
.cherche_dosbase:
		lea	dosbase-t,a6
		add.l	a5,a6
		move.l	(a6),a6			adr. dosbase
		lea	(.dosbase,pc),a0
		move.l	a6,(a0)
		rts

*-----------------> LOAD FILE : retour sous le système <------------------*
.sys_restore:
		move.w	(flag_sys_gele-t,a5),d0
		lea	(.flag_sys_gele,pc),a0
		move.w	d0,(a0)			sauve flag
		beq.s	.sys_non_gele		-> système non gelé
		bsr	sys_restore		|-> restore le système
.sys_non_gele	rts

*---------------------> LOAD FILE : Lock le fichier <---------------------*
.lock_file:
		move.l	(.pt_name,pc),d1
		moveq	#-2,d2			-2 = lecture
		move.l	(.dosbase,pc),a6
		jsr	(Lock,a6)
		lea	(.lock,pc),a0
		move.l	d0,(a0)
		beq.s	.lock_error
		moveq	#0,d7
		rts
.lock_error	moveq	#-7,d7			¤ -7 = ouverture impossible
		rts

*-------------> LOAD FILE : Complète la structure InfoBlock <-------------*
.examine_file:
		move.l	(.lock,pc),d1
		move.l	(infoblock.pt-t,a5),d2
		move.l	(.dosbase,pc),a6
		jsr	(Examine,a6)
		tst.l	d0			aucune erreur ?
		beq.s	.examine_erreur		-> si
		moveq	#0,d7
		rts
.examine_erreur	moveq	#-8,d7			¤ -8 = erreur de lecture
		rts

*--------------------> LOAD FILE : ouvre le fichier <---------------------*
.open_file:
 		move.l	(.pt_name,pc),d1
		move.l	#1005,d2		1005 = "Mode_old"
		move.l	(.dosbase,pc),a6
		jsr	(Open,a6)
		lea	(.handle,pc),a0
		move.l	d0,(a0)			sauve valeur du handle
		beq.s	.open_erreur		-> si nul, alors erreur
		moveq	#0,d7
		rts
.open_erreur	moveq	#-7,d7			¤ -7 = ouverture impossible
		rts

*--------------> LOAD FILE : charge le fichier en mémoire <---------------*
.read_file:
		moveq	#0,d1
		move.l	(.adr_load,pc),d2	d2 = adr. chargement
		beq.s	.read_file_exit
		move.l	(.handle,pc),d1
		move.l	(.size_load,pc),d3	d3 = nombre d'octet à charger
		move.l	(.dosbase,pc),a6
		jsr	(Read,a6)
		lea	(.size_read,pc),a0
		move.l	d1,(a0)
		tst.l	d1			erreur de lecture ?
		bmi.s	.read_fileerror		-> oui
.read_file_exit	moveq	#0,d7
		rts
.read_fileerror	moveq	#-8,d7			¤ -8 erreur de lecture
		rts

*---------------> LOAD FILE : Supprime la structure Lock <----------------*
.unlock_file:
		move.l	(.lock,pc),d1
		move.l	(.dosbase,pc),a6
		jsr	(UnLock,a6)
		rts

*--------------------> LOAD FILE : ferme le fichier <---------------------*
.close_file:	
		move.l	(.handle,pc),d1
		move.l	(.dosbase,pc),a6
		jsr	(Close,a6)
		rts

*----------------> LOAD FILE : gèle le système si besoin <----------------*
.sys_gele:
		lea	(.flag_sys_gele,pc),a0
		tst.w	(a0)			système gelé lors de l'appel ?
		beq.s	.non_gele		-> non
		bsr	sys_save
		bsr	first_init
.non_gele	rts

*------------> LOAD FILE : adresse utilisées par la routine <-------------*
		CNOP	0,4
.dosbase	dc.l	0
.pt_name	dc.l	0		pt/le nom
.adr_load	dc.l	0		adr. de chargement
.size_load	dc.l	0		nombre d'octet à charger
.size_read	dc.l	0		nombre d'octet réélement chargé
.handle		dc.l	0		handle du fichier
.lock		dc.l	0
.flag_sys_gele	dc.w	0		si = 0 alors système non gelé lors de l'appel
