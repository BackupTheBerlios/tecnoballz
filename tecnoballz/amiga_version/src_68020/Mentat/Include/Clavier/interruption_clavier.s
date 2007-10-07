***************************************************************************
*>>>>>>>>>>>* INTERRUPTION DE NIVEAU 2 : CIA-A, Test clavier *<<<<<<<<<<<<*
***************************************************************************
interruption_clavier:
		btst	#3,CIAA_ICR		irq du port série ?
		beq.s	ki.exit			-> non
		movem.l	d0/a0,-(a7)

*---------------> INT CLAVIER : lit le code "RAW" clavier <---------------*
		clr.w	d0
		move.b	CIAA_SDR,d0
		ori.b	#$40,CIAA_CRA
		swap	d0
		lea	$DFF006,a0
		move.b	(a0),d0
.wait1		cmp.b	(a0),d0
		beq.s	.wait1
		move.b	(a0),d0
.wait2		cmp.b	(a0),d0
		beq.s	.wait2
		move.b	(a0),d0
.wait3		cmp.b	(a0),d0
		beq.s	.wait3
		swap	d0
		andi.b	#$BF,CIAA_CRA
		not.b	d0
		ror.b	#1,d0			d0=code raw clavier

*-----> INT CLAVIER : test si c'est une touche enfoncée ou relachée <-----*
ki.adr:		lea	0,a0		¤	(modifié dans "first.init.s")
		move.w	d0,(-4,a0)		sauve code clavier
		cmp.w	#128,d0			touche enfoncée ?
		blt.s	.touche_enfonce		-> oui

*-------------------> INIT CLAVIER : touche relachée <--------------------*
		sub.w	#128,d0		
		clr.b	(a0,d0)
		tst.w	(-2,a0)			nbre de touche enfoncée nul ?
		beq.s	.exit			-> oui
		subq.w	#1,(-2,a0)		nbre de touche enfoncée - 1
		bra.s	.exit

*-------------------> INIT CLAVIER : touche enfoncée <--------------------*
.touche_enfonce	move.b	#1,(a0,d0)
		IFEQ	FLAG_EXIT
		sub.w	#TOUCHE_ESC,d0		retour sous système ?
		beq.s	ki.superquit		-> oui
		ENDC
		addq.w	#1,(-2,a0)		nbre de touche enfoncée + 1

*----------------> INT CLAVIER : sort de l'interruption <-----------------*
.exit		movem.l	(a7)+,d0/a0
ki.exit		move.w	#$0008,CUSTOM+INTREQ
		rte

*------> INT CLAVIER : Touche "ESC" = demande à revenir au système <------*
		IFEQ	FLAG_EXIT
ki.superquit:	lea	(ki.superquit,pc),a0
		add.l	#superquit-ki.superquit,a0
		jmp	(a0)
		ENDC
