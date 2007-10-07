*=========================================================================*
******************** INITIALISE AFFICHAGE DES MESSAGES ********************
*=========================================================================*
* Entrée	=> a5.l	Adr. 't'
init_message:
		bsr	mistigris_lili		£=> Reserve fond
		bne.s	.exit
		bsr	init_rikiki
		bsr	init_lover
		bsr	messe_de_minuit
		bsr	aurora_boralis
		moveq	#0,d1
		move.w	d1,(mess.req-t,a5)
		move.w	d1,(mess.pause-t,a5)
		move.w	d1,(mess.pt-t,a5)

		*/ Efface la table de demande des messages */
		lea	(message-t,a5),a0
		moveq	#mg2-message-1,d0
.loopx77	move.b	d1,(a0)+
		dbf	d0,.loopx77
		GETMESSAGE	01
		moveq	#0,d7
.exit		rts


********************RESERVE MEMOIRE DU FOND DES OCEANS*********************
* Entrée	=> a5.l	Adr. 't'
mistigris_lili:
		lea	(mess.fond-t,a5),a4
		moveq	#8*6,d0
		mulu	(SPPLAN,a0),d0
		move.l	d0,(RESA.SIZ,a4)
		bsr.l	reserve.mem
		rts

*********************initialise la table "mes_rikiki"**********************
* Entrée	=> a5.l	Adr. 't'
init_rikiki:
		move.l	(pt_screen-t,a5),a0
		moveq	#0,d1
		move.w	(SP_SLINE,a0),d1
		moveq	#45,d0
		mulu	d1,d0
		add.w	#12,d0
		move.l	d0,(mess.pos-t,a5)
		move.l	(mess.fond-t,a5),d2
		moveq	#8,d3			Largeur message
		mulu	(SPPLAN,a0),d3
		lea	(mes_rikiki-t,a5),a0
		moveq	#5,d6
.loop		move.l	d0,(a0)+
		add.l	d1,d0
		move.l	d2,(a0)+
		add.l	d3,d2
		dbf	d6,.loop
		rts

**********************INITIALISE LA TABLE 'mes_love'***********************
* Entrée	=> a5.l adr. 't'
init_lover:
		move.l	(mess.pos-t,a5),d0	d0=Adresse écran relative
		lea	(mes_love-t,a5),a0
		moveq	#3,d3			Largeur message : 4 mots (64 pixels)
.loopzz		moveq	#0,d1
		moveq	#3,d2			4 caractères  par mot de 16 bits
.loopxx		move.w	d0,(a0)+		Adresse relative
		move.w	d1,(a0)+		Décalage
		addq.w	#4,d1
		dbf	d2,.loopxx
		addq.w	#2,d0
		dbf	d3,.loopzz
		rts

**************************CALCUL VALEURS BLITTERS**************************
* Entrée	=> a5.l Adr. 't'
messe_de_minuit:
		*/ Calcul fenêtre Blitter */
		move.l	(pt_screen-t,a5),a0
		move.w	(SPPLAN,a0),d0
		lsl.w	#6,d0
		addq.w	#4,d0
		move.w	d0,(mess.win-t,a5)
		*/ Calcul modulos A & D */
		moveq	#0,d0
		move.w	(SP_SPLAN,a0),d0
		subq.w	#8,d0
		move.l	d0,(mess.mod-t,a5)
		rts

*****************Sauve partie décor où s'affiche messages******************
* Entrée	=> a5.l Adr. 't'
*		=> a6.l CUSTOM
aurora_boralis:
		move.l	(pt_screen-t,a5),a0
		moveq	#6,d2
		mulu	(SPPLAN,a0),d2
		lsl.w	#6,d2
		addq.w	#4,d2
		moveq	#-1,d1
		move.l	#$09F00000,d3
		move.l	(mess.mod-t,a5),d4
		swap	d4
		move.l	(mess.fond-t,a5),d5
		move.l	(SPECRAN1,a0),d7
		add.l	(mess.pos-t,a5),d7
		WAITBLITTER
		move.l	d1,(BLTAFWM,a6)
		move.l	d3,(BLTCON0,a6)
		move.l	d4,(BLTAMOD,a6)
		move.l	d7,(BLTAPTH,a6)
		move.l	d5,(BLTDPTH,a6)
		move.w	d2,(BLTSIZE,a6)
		rts
