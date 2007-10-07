*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*/=>>>>>>>>>>>>>>>>>>>>>>>> GESTION DU MESSAGE <<<<<<<<<<<<<<<<<<<<<<<<<<\*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
* Entrée	=> a5.l	Adr. 't'
no_dream1:
		tst.w	(mess.pause-t,a5)	message en cours ?
		bne.s	.no_get_mesx		-> non
		lea	(mes0-t,a5),a1
		lea	(message-t,a5),a0
		moveq	#mg2-message-1,d0
.loopx77	tst.b	(a0)+
		bne.s	.go
		lea	(16,a1),a1
		dbf	d0,.loopx77
		rts
.go		clr.b	-(a0)
		clr.w	(mess.pt-t,a5)
		move.w	#66,(mess.pause-t,a5)	Temps du message
		move.l	a1,(mess.req-t,a5)	Pt/le message
.no_get_mesx:	moveq	#0,d0
		move.w	(mess.pt-t,a5),d1
		cmp.w	#16,d1			affiche les caractères ?
		blt	mes_affichage		-> oui
		subq.w	#1,(mess.pause-t,a5)
		cmp.w	#5,(mess.pause-t,a5)	efface message ?
		bhi	mes_exit		-> non

*=========================================================================*
******************** Efface un message ligne par ligne ********************
*=========================================================================*
		move.w	(mess.pause-t,a5),d1
		lsl.w	#3,d1			*8
		lea	(mes_rikiki-t,a5),a0
		add.w	d1,a0
		move.l	(a0)+,d0		Position de la ligne à effacer
		move.l	(a0)+,a0		Adresse de la ligne à copier
		move.l	(ecran1-t,a5),a1
		add.l	d0,a1			adr. écr 1
		move.l	(ecran2-t,a5),a2
		add.l	d0,a2			adr. écr 2
		move.l	(ecran3-t,a5),a3
		add.l	d0,a3			adr. écr 3
		move.w	(mess.win-t,a5),d0
		move.l	#$09F00000,d1
		moveq	#-1,d2
		move.l	(mess.mod-t,a5),d3
		WAITBLITTER
		move.l	d1,(BLTCON0,a6)
		move.l	d2,(BLTAFWM,a6)
		move.l	d3,(BLTAMOD,a6)
		move.l	a0,(BLTAPTH,a6)
		move.l	a1,(BLTDPTH,a6)
		move.w	d0,(BLTSIZE,a6)
		WAITBLITTER
		move.l	a0,(BLTAPTH,a6)
		move.l	a2,(BLTDPTH,a6)
		move.w	d0,(BLTSIZE,a6)
		WAITBLITTER
		move.l	a0,(BLTAPTH,a6)
		move.l	a3,(BLTDPTH,a6)
		move.w	d0,(BLTSIZE,a6)

		;tst.w	(mess.pause-t,a5)
		;bne	mes_exit		
		;lea	(mess.req-t,a5),a0
		;cmp	#mes8-mes0,(a0)
		;bne	mes_exit
		;GET_MESSAGE	#70,#mes9-mes0
		;bra	mes_exit

*=========================================================================*
*                  Affiche un message lettre par lettre			  *
*=========================================================================*
* Entrée	=> a5.l	Adr. table 't'
mes_affichage:
		*- lit le caractère à afficher et calcul l'adresse -*
		move.l	(mess.req-t,a5),a0	a0=pt/le message
		add.w	d1,a0			pt/la lettre
		moveq	#0,d0
		move.b	(a0),d0
		sub.w	#'A',d0
		add.w	d0,d0
		move.l	(mini.pagegfx-t,a5),a0
		moveq	#0,d2
		move.w	(PG_SLINE,a0),d2
		lea	(PG_DATA,a0,d0),a0

		*/ lit le position + décalage */
		add.w	d1,d1 
		add.w	d1,d1 
		lea	(mes_love-t,a5),a1
		add.w	d1,a1
		move.w	(a1)+,d0		pos. d'affichage
		move.w	(a1)+,d1		décalage

		* calcul adr. dans écran 1, 2 et 3 *
		move.l	(pt_screen-t,a5),a1
		moveq	#0,d3
		move.w	(SP_SPLAN,a1),d3
		move.w	(SPPLAN,a1),d4
		subq.w	#1,d4
		move.l	(ecran1-t,a5),a1
		add.l	d0,a1
		move.l	(ecran2-t,a5),a2
		add.l	d0,a2
		move.l	(ecran3-t,a5),a3
		add.l	d0,a3

		*/ Boucle d'affichage */
		moveq	#5,d6
.loop		move.w	(a0),d0
		lsr.w	d1,d0
		move.w	d4,d5
.loopx		or.w	d0,(a1)
		add.l	d3,a1
		or.w	d0,(a2)
		add.l	d3,a2
		or.w	d0,(a3)
		add.l	d3,a3
		dbf	d5,.loopx
		add.l	d2,a0
		dbf	d6,.loop
		addq.w	#1,(mess.pt-t,a5)
mes_exit:	rts
