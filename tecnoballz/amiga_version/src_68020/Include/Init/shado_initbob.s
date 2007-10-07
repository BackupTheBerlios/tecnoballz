***************************************************************************
*=*=*=*=*=*=*=*=*=*=* INITIALISE L'AFFICHAGE DE BOBS *=*=*=*=*=*=*=*=*=*=*=
***************************************************************************
* Entrée	=> a5.l	adr. 't'
* ATTENTION UN ECRAN DOIT ETRE DEJA INITIALISE
shado_initbob:
		tst.l	(pt_screen-t,a5)
		beq.s	.noscreen
		tst.l	(bob32_pagegfx-t,a5)
		beq.s	.nopage
		bsr	shado_tab.mem
		bne.s	.exit
		bsr	shado_liste.init
		bne.s	.exit
		bsr	shado_memory
		bne.s	.exit
		bsr.s	shado_comptute

		moveq	#0,d7
.exit		rts
.noscreen	moveq	#-32,d7			¤ pas de "pt_screen"
		rts
.nopage		moveq	#-30,d7			¤ pas de "bob32_pagegfx"
		rts

*--------------------* Calcul les tables d'affichage *--------------------*
* Entrée	=> a5.l	adr. 't'
shado_comptute:
		*/ TABLE XBYTE */
		lea	(bob32_xbyte-t,a5),a3
		lea	(shado_xbyte-t,a5),a4
		move.l	(RESA.SIZ,a3),d0
		lsr.w	#1,d0
		sub.w	#ODECA+1,d0
		move.l	(RESA.ADR,a3),a0
		move.l	(RESA.ADR,a4),a1
		lea	(ODECA*2,a1),a1
.loopxb		move.w	(a0)+,(a1)+
		dbf	d0,.loopxb
		moveq	#ODECA-1,d0
		move.l	(RESA.ADR,a4),a1
.loopxb2	move.w	(a0)+,(a1)+
		dbf	d0,.loopxb2

		*/ TABLE XMULU */
		lea	(bob32_ymulu-t,a5),a3
		lea	(shado_ymulu-t,a5),a4
		move.l	(RESA.SIZ,a3),d0
		lsr.w	#2,d0
		sub.w	#ODECA+1,d0
		move.l	(RESA.ADR,a3),a0
		move.l	(RESA.ADR,a4),a1
		lea	(ODECA*4,a0),a0
.loopym		move.l	(a0)+,(a1)+
		dbf	d0,.loopym
		moveq	#ODECA-1,d0
		move.l	(RESA.ADR,a3),a0
.loopym2	move.l	(a0)+,(a1)+
		dbf	d0,.loopym2

		*/ TABLE BLTCON */
		lea	(bob32_bltcon-t,a5),a3
		lea	(shado_bltcon-t,a5),a4
		move.l	(RESA.SIZ,a3),d0
		lsr.w	#2,d0
		sub.w	#ODECA+1,d0
		move.l	(RESA.ADR,a3),a0
		move.l	(RESA.ADR,a4),a1
		lea	(ODECA*4,a1),a1
.loopbt		move.l	(a0)+,(a1)
		and.w	#$F000,(a1)
		or.w	#$0D0C,(a1)+
		clr.w	(a1)+
		dbf	d0,.loopbt
		moveq	#ODECA-1,d0
		move.l	(RESA.ADR,a4),a1
.loopbt2	move.l	(a0)+,(a1)
		and.w	#$F000,(a1)
		or.w	#$0D0C,(a1)+
		clr.w	(a1)+
		dbf	d0,.loopbt2
		rts
*-------------------* réserve mémoire pour les tables *-------------------*
* Entrée	=> a5.l	adr. 't'
* Sortie	<= d7.l=0 / Z=1 pas d'erreur
shado_memory:
		*» Réserve la table 'xbyte' «*
		lea	(bob32_xbyte-t,a5),a3
		lea	(shado_xbyte-t,a5),a4
		move.l	(RESA.SIZ,a3),(RESA.SIZ,a4)
		bsr	reserve.mem
		bne.s	.exit
		*» Réserve la table 'bltcon' «*
		lea	(bob32_bltcon-t,a5),a3
		lea	(shado_bltcon-t,a5),a4
		move.l	(RESA.SIZ,a3),(RESA.SIZ,a4)
		bsr	reserve.mem
		bne.s	.exit
		*» Réserve la table 'ymulu' «*
		lea	(bob32_ymulu-t,a5),a3
		lea	(shado_ymulu-t,a5),a4
		move.l	(RESA.SIZ,a3),(RESA.SIZ,a4)
		bsr	reserve.mem
		bne.s	.exit
		moveq	#0,d7
.exit		rts

*------------* réserve listes d'affichage et de retauration *-------------*
* Entrée	=> a5.l	adr. 't'
* Sortie	<= d7.l=0 / Z=1 pas d'erreur
shado_liste.init:
		lea	(shado_listes16-t,a5),a1
		lea	(shado_width16-t,a5),a0
		lea	(shado_pt16-t,a5),a2
		move.w	#BOB32_MAXI16,d7
		moveq	#4,d6			largeur 2+2 octets
		bsr	.init
		bne	.exit

		lea	(shado_listes32-t,a5),a1
		lea	(shado_width32-t,a5),a0
		lea	(shado_pt32-t,a5),a2
		move.w	#BOB32_MAXI32,d7
		moveq	#6,d6			largeur 4+2 octets
		bsr	.init
		bne.s	.exit

		lea	(shado_listes48-t,a5),a1
		lea	(shado_width48-t,a5),a0
		lea	(shado_pt48-t,a5),a2
		move.w	#BOB32_MAXI48,d7
		moveq	#8,d6			largeur 6+2 octets
		bsr	.init
		bne.s	.exit

		lea	(shado_listes64-t,a5),a1
		lea	(shado_width64-t,a5),a0
		lea	(shado_pt64-t,a5),a2
		move.w	#BOB32_MAXI64,d7
		moveq	#10,d6			largeur 8+2 octets
		bsr	.init
		bne.s	.exit

		lea	(shado_listes80-t,a5),a1
		lea	(shado_width80-t,a5),a0
		lea	(shado_pt80-t,a5),a2
		move.w	#BOB32_MAXI80,d7
		moveq	#12,d6			largeur 10+2 octets
		bsr	.init
		bne.s	.exit

		lea	(shado_listes96-t,a5),a1
		lea	(shado_width96-t,a5),a0
		lea	(shado_pt96-t,a5),a2
		move.w	#BOB32_MAXI96,d7
		moveq	#14,d6			largeur 12+2 octets
		bsr	.init

		lea	(shado_listes192-t,a5),a1
		lea	(shado_width192-t,a5),a0
		lea	(shado_pt192-t,a5),a2
		move.w	#BOB32_MAXI192,d7
		moveq	#26,d6			largeur 24+2 octets
		bsr	.init
.exit		rts

		 * initialisation des listes d'affichage *
* entrée	=> d6.w	largeur du bob + décalage
*		=> d7.w	nbre de bobs maxi
*		=> a0.l	pt/liste d'affichage 'shado_width??'
*		=> a1.l	pt/les 2 listes de restauration 'shado_listes??'
*		=> a2.l	pt/pt fin de la liste d'affichage
* sortie	<= d7.l=0 / Z=1 pas d'erreur
.init		move.l	a2,d5			sauve a2 dans d5
		tst.w	d7			des bobs prévus
		beq	.stop			-> non
		tst.l	(a0)			mémoire déjà réservée ?
		bne.s	.nores			-> oui
		moveq	#0,d2
		move.w	d7,d2
		lsl.l	#4,d2

		* liste d'affichage "shado_widthXX" *
		move.l	d2,d0
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(a0)
		beq	.outmem

		* liste de retauration écran 1 "shado_listeXX" *
		move.l	d2,d0
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(a1)
		beq.s	.outmem

		* liste de retauration écran 2 "shado_listeXX" *
		move.l	d2,d0
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,4(a1)
		beq.s	.outmem

		* efface liste mémoire *
.nores		subq.w	#1,d7
		moveq	#0,d0
		move.l	(a0),a2			"shado_widthXX"
		move.l	(a1),a3			"shado_listeXX" écran 1
		move.l	(4.w,a1),a4		"shado_listeXX" écran 2
.loop		move.l	d0,(a2)+
		move.l	d0,(a2)+
		move.l	d0,(a3)+
		move.l	d0,(a3)+
		move.l	d0,(a4)+
		move.l	d0,(a4)+
		dbf	d7,.loop

		* écrit adr. des listes de retaur. dans liste d'aff. *
		move.l	(a0),a2			"shado_widthXX"
		move.l	(a1),(8,a2)		liste rest. écran 1
		move.l	(4,a1),(12,a2)		liste rest. écran 2
		addq.l	#4,(8,a2)		saute les modulos blitter A & D
		addq.l	#4,(12,a2)		saute les modulos blitter A & D

		* calcul modulos blitter *
		move.l	(pt_screen-t,a5),a2
		move.l	(bob32_pagegfx-t,a5),a3
		move.w	(PG_SLINE,a3),d1	d1=largeur page bob en octets
		move.w	(SP_SLINE,a2),d0	d0=largeur écran en octets
		sub.w	d6,d0			d0=modulo écran
		sub.w	d6,d1			d1=modulo page bob
		move.l	(a0),a2			"shado_widthXX"
		move.l	(a1),a3			"shado_listeXX" écran 1
		move.l	(4,a1),a4		"shado_listeXX" écran 2
		move.w	d0,(a2)+		modulo C affichage
		move.w	d0,(a2)+		modulo B affichage
		move.w	d1,(a2)+		modulo A affichage
		move.w	d0,(a2)+		modulo D affichage
		addq.l	#8,a2			saute adr. listes écran 1 & 2
		move.l	d5,a1			a1="shado_ptXX"
		move.l	a2,(a1)			pt/fin de la liste d'affichage
		move.w	d0,(a3)+		modulo A restauration
		move.w	d0,(a4)+		modulo A restauration
		move.w	d0,(a3)+		modulo D restauration
		move.w	d0,(a4)+		modulo D restauration
.stop		moveq	#0,d7
		rts
.outmem		moveq	#-12,d7
		rts

*--------------------* réserve table fenêtre blitter *--------------------*
* Entrée	=> a5.l	Adr. 't'
* Sortie	<= d7.l=0 / Z=1 pas d'erreur
shado_tab.mem:
		lea	(4+shado_pt16-t,a5),a2
		move.w	#BOB32_MAXI16,d7
		bsr	.init
		bne.s	.exit
		lea	(4+shado_pt32-t,a5),a2
		move.w	#BOB32_MAXI32,d7
		bsr	.init
		bne.s	.exit
		lea	(4+shado_pt48-t,a5),a2
		move.w	#BOB32_MAXI48,d7
		bsr	.init
		bne.s	.exit
		lea	(4+shado_pt64-t,a5),a2
		move.w	#BOB32_MAXI64,d7
		bsr	.init
		bne.s	.exit
		lea	(4+shado_pt80-t,a5),a2
		move.w	#BOB32_MAXI80,d7
		bsr	.init
		bne.s	.exit
		lea	(4+shado_pt96-t,a5),a2
		move.w	#BOB32_MAXI96,d7
		bsr	.init
		lea	(4+shado_pt192-t,a5),a2
		move.w	#BOB32_MAXI192,d7
		bsr	.init
.exit		rts

		 * initialisation des tables blitter  *
* entrée	=> d7.w	nbre de bobs maxi
*		=> a2.l	pt/pt fin de la liste d'affichage
* sortie	<= d7.l=0 / Z=1 pas d'erreur
.init		tst.w	d7			des bobs prévus
		beq.s	.stop			-> non

		* réserve mémoire *
		tst.l	(a2)			mémoire déjà réservée ?
		bne.s	.nores			-> oui
		moveq	#0,d0
		move.w	d7,d0
		lsl.l	#2,d0
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(a2)
		beq.s	.outmem
.nores
		* efface mémoire *
		move.l	(a2),a0
		subq.w	#1,d7
		moveq	#0,d0
.loop_clear	move.l	d0,(a0)+
		dbf	d7,.loop_clear
		moveq	#0,d7
.stop		rts
.outmem		moveq	#-12,d7
		rts

* Libère liste *
shado.freemem:
		lea	(shado_xbyte-t,a5),a0
		bsr	libere_mem
		lea	(shado_bltcon-t,a5),a0
		bsr	libere_mem
		lea	(shado_ymulu-t,a5),a0
		bsr	libere_mem

		lea	(shado_width16-t,a5),a0
		bsr	libere_mem
		lea	(shado_width32-t,a5),a0
		bsr	libere_mem
		lea	(shado_width48-t,a5),a0
		bsr	libere_mem
		lea	(shado_width64-t,a5),a0
		bsr	libere_mem
		lea	(shado_width80-t,a5),a0
		bsr	libere_mem
		lea	(shado_width96-t,a5),a0
		bsr	libere_mem
		lea	(shado_width192-t,a5),a0
		bsr	libere_mem

		lea	(shado_listes16-t,a5),a0
		bsr	libere_mem
		lea	(shado_listes32-t,a5),a0
		bsr	libere_mem
		lea	(shado_listes48-t,a5),a0
		bsr	libere_mem
		lea	(shado_listes64-t,a5),a0
		bsr	libere_mem
		lea	(shado_listes80-t,a5),a0
		bsr	libere_mem
		lea	(shado_listes96-t,a5),a0
		bsr	libere_mem
		lea	(shado_listes192-t,a5),a0
		bsr	libere_mem


		lea	(shado_listes16+4-t,a5),a0
		bsr	libere_mem
		lea	(shado_listes32+4-t,a5),a0
		bsr	libere_mem
		lea	(shado_listes48+4-t,a5),a0
		bsr	libere_mem
		lea	(shado_listes64+4-t,a5),a0
		bsr	libere_mem
		lea	(shado_listes80+4-t,a5),a0
		bsr	libere_mem
		lea	(shado_listes96+4-t,a5),a0
		bsr	libere_mem
		lea	(shado_listes192+4-t,a5),a0
		bsr	libere_mem

		lea	(4+shado_pt16-t,a5),a0
		bsr	libere_mem
		lea	(4+shado_pt32-t,a5),a0
		bsr	libere_mem
		lea	(4+shado_pt48-t,a5),a0
		bsr	libere_mem
		lea	(4+shado_pt64-t,a5),a0
		bsr	libere_mem
		lea	(4+shado_pt80-t,a5),a0
		bsr	libere_mem
		lea	(4+shado_pt96-t,a5),a0
		bsr	libere_mem
		lea	(4+shado_pt192-t,a5),a0
		bsr	libere_mem
		rts
