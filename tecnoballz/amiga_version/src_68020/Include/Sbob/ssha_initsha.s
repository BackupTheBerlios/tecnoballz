***************************************************************************
*=*=*=*=*=*=*=*=*=*= INITIALISE AFFICHAGE SHADOWS-SBOBS *=*=*=*=*=*=*=*=*=*
***************************************************************************
* Entrée	=> a5.l	adr. 't'
ssha_initsha:
		tst.l	(pt_screen-t,a5)
		beq.s	.noscreen
		tst.l	(sbob_pagegfx-t,a5)
		beq.s	.nopage
		bsr	ssha_tab.mem		-> réserve liste "last_table"
		bne.s	.exit
		bsr	ssha_liste.init
		bne.s	.exit
		bsr.s	ssha_memory
		bne.s	.exit
		bsr	ssh_init_xtable
		bsr	ssh_init_ytable
		moveq	#0,d7
.exit		rts
.noscreen	moveq	#-32,d7			¤ pas de "pt_screen"
		rts
.nopage		moveq	#-30,d7			¤ pas de "sbob_pagegfx"
		rts

*-------------------* Réserve mémoire pour les tables *-------------------*
* Entrée	=> a5.l	adr. 't'
* Sortie	<= d7.l=0 / Z=1 pas d'erreur
ssha_memory:
		* Réserve la  'ssha_xtable'*
		lea	(sbob_xtable-t,a5),a3
		lea	(ssha_xtable-t,a5),a4
		move.l	(RESA.SIZ,a3),(RESA.SIZ,a4)
		bsr	reserve.mem
		bne.s	.exit

		* Réserve la 'ssha_ytable' *
		lea	(sbob_ytable-t,a5),a3
		lea	(ssha_ytable-t,a5),a4
		move.l	(RESA.SIZ,a3),(RESA.SIZ,a4)
		bsr	reserve.mem
		bne.s	.exit
		moveq	#0,d7
.exit		rts

*---------------------* initialise la 'ssha_xtable' *---------------------*
* Entrée	=> a5.l	adr. 't'
ssh_init_xtable	
		lea	(sbob_xtable-t,a5),a3
		lea	(ssha_xtable-t,a5),a4
		move.l	(RESA.SIZ,a3),d0
		lsr.w	#3,d0
		sub.w	#ODECAFEINE+1,d0
		move.l	(RESA.ADR,a3),a0
		move.l	(RESA.ADR,a4),a1
		lea	(ODECAFEINE*8,a1),a1
		move.w	#$F000,d1
		move.w	#$0D0C,d2
		moveq	#0,d3
.loopbt		move.l	(a0)+,(a1)+		position en octets
		move.l	(a0)+,(a1)
		and.w	d1,(a1)
		or.w	d2,(a1)+
		move.w	d3,(a1)+
		dbf	d0,.loopbt
		moveq	#ODECAFEINE-1,d0
		move.l	(RESA.ADR,a4),a1
.loopbt2	move.l	(a0)+,(a1)+		position en octets
		move.l	(a0)+,(a1)
		and.w	d1,(a1)
		or.w	d2,(a1)+
		move.w	d3,(a1)+
		dbf	d0,.loopbt2
		rts
*-------------------------* initialise 'ytable' *-------------------------*
* Entrée	=> a5.l	adr. 't'
ssh_init_ytable:	
		lea	(sbob_ytable-t,a5),a3
		lea	(ssha_ytable-t,a5),a4
		move.l	(RESA.SIZ,a3),d0
		lsr.w	#2,d0
		sub.w	#ODECAFEINE+1,d0
		move.l	(RESA.ADR,a3),a0
		move.l	(RESA.ADR,a4),a1
		lea	(ODECAFEINE*4,a0),a0
.loopym		move.l	(a0)+,(a1)+
		dbf	d0,.loopym
		moveq	#ODECAFEINE-1,d0
		move.l	(RESA.ADR,a3),a0
.loopym2	move.l	(a0)+,(a1)+
		dbf	d0,.loopym2
		rts

*------------* réserve listes d'affichage et de retauration *-------------*
* Entrée	=> a5.l	adr. 't'
* Sortie	<= d7.l=0 / Z=1 pas d'erreur
ssha_liste.init:
		lea	(ssha_listes16-t,a5),a1
		lea	(ssha_width16-t,a5),a0
		lea	(ssha_pt16-t,a5),a2
		move.w	#SBOB_MAXI16,d7
		moveq	#4,d6			largeur 2+2 octets
		bsr	.init
		bne.s	.exit

		lea	(ssha_listes32-t,a5),a1
		lea	(ssha_width32-t,a5),a0
		lea	(ssha_pt32-t,a5),a2
		move.w	#SBOB_MAXI32,d7
		moveq	#6,d6			largeur 4+2 octets
		bsr	.init
		bne.s	.exit

		lea	(ssha_listes48-t,a5),a1
		lea	(ssha_width48-t,a5),a0
		lea	(ssha_pt48-t,a5),a2
		move.w	#SBOB_MAXI48,d7
		moveq	#8,d6			largeur 6+2 octets
		bsr	.init
		bne.s	.exit

		lea	(ssha_listes64-t,a5),a1
		lea	(ssha_width64-t,a5),a0
		lea	(ssha_pt64-t,a5),a2
		move.w	#SBOB_MAXI64,d7
		moveq	#10,d6			largeur 8+2 octets
		bsr	.init
		bne.s	.exit

		lea	(ssha_listes80-t,a5),a1
		lea	(ssha_width80-t,a5),a0
		lea	(ssha_pt80-t,a5),a2
		move.w	#SBOB_MAXI80,d7
		moveq	#12,d6			largeur 10+2 octets
		bsr	.init
		bne.s	.exit

		lea	(ssha_listes256-t,a5),a1
		lea	(ssha_width256-t,a5),a0
		lea	(ssha_pt256-t,a5),a2
		move.w	#SBOB_MAXI256,d7
		moveq	#34,d6			largeur 32+2 octets
		bsr	.init
.exit		rts

		 * initialisation des listes d'affichage *
* entrée	=> d6.w	largeur du bob + décalage
*		=> d7.w	nbre de bobs maxi
*		=> a0.l	pt/liste d'affichage 'ssha_width??'
*		=> a1.l	pt/les 2 listes de restauration 'ssha_listes??'
*		=> a2.l	pt/pt fin de la liste d'affichage 'ssha_pt??'
* sortie	<= d7.l=0 / Z=1 pas d'erreur
.init		move.l	a2,d5			sauve a2 dans d5
		tst.w	d7			des bobs prévus
		beq	.stop			-> non
		tst.l	(a0)			mémoire déjà réservée ?
		bne.s	.nores			-> oui
		moveq	#0,d2
		move.w	d7,d2
		lsl.l	#4,d2

		* liste d'affichage "ssha_widthXX" *
		move.l	d2,d0
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(a0)
		beq	.outmem

		* liste de retauration écran 1 "ssha_listeXX" *
		move.l	d2,d0
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(a1)
		beq	.outmem

		* liste de retauration écran 2 "ssha_listeXX" *
		move.l	d2,d0
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(4,a1)
		beq.s	.outmem

		* efface liste mémoire *
.nores		subq.w	#1,d7
		moveq	#0,d0
		move.l	(a0),a2			"ssha_widthXX"
		move.l	(a1),a3			"ssha_listeXX" écran 1
		move.l	(4,a1),a4		"ssha_listeXX" écran 2
.loop		move.l	d0,(a2)+
		move.l	d0,(a2)+
		move.l	d0,(a3)+
		move.l	d0,(a3)+
		move.l	d0,(a4)+
		move.l	d0,(a4)+
		dbf	d7,.loop

		* écrit adr. des listes de retaur. dans liste d'aff. *
		move.l	(a0),a2			"ssha_widthXX"
		move.l	(a1),(8,a2)		liste rest. écran 1
		move.l	(4,a1),(12,a2)		liste rest. écran 2
		addq.l	#4,(8,a2)		saute les modulos blitter A & D
		addq.l	#4,(12,a2)		saute les modulos blitter A & D

		* calcul modulos blitter *
		move.l	(pt_screen-t,a5),a2
		move.l	(sbob_pagegfx-t,a5),a3
		move.w	(PG_SLINE,a3),d1
		move.w	(SP_SLINE,a2),d0
		sub.w	d6,d0			d0=modulo écran
		sub.w	d6,d1			d1=modulo page bob
		move.l	(a0),a2			"ssha_widthXX"
		move.l	(a1),a3			"ssha_listeXX" écran 1
		move.l	(4,a1),a4		"ssha_listeXX" écran 2
		move.w	d0,(a2)+		modulo C *INUTILISE*
		move.w	d0,(a2)+		modulo B écran
		move.w	d1,(a2)+		modulo A BOB
		move.w	d0,(a2)+		modulo D écran
		addq.l	#8,a2			saute adr. listes écran 1 & 2
		move.l	d5,a1			a1="ssha_ptXX"
		move.l	a2,(a1)			pt/fin de la liste d'affichage
		move.w	d0,(a3)+		modulo A restauration
		move.w	d0,(a4)+		modulo A restauration
		move.w	d0,(a3)+		modulo D restauration
		move.w	d0,(a4)+		modulo D restauration
		move.l	#-1,(a3)		pas de SBOB à effacer
		move.l	#-1,(a4)		pas de SBOB à effacer
.stop		moveq	#0,d7
		rts
.outmem		moveq	#-12,d7
		rts

*--------------------* Réserve table des "last_table" *-------------------*
* Entrée	=> a5.l	Adr. 't'
* Sortie	<= d7.l=0 / Z=1 pas d'erreur
ssha_tab.mem:
		lea	(4+ssha_pt16-t,a5),a2
		move.w	#SBOB_MAXI16,d7
		bsr	.init
		bne.s	.exit
		lea	(4+ssha_pt32-t,a5),a2
		move.w	#SBOB_MAXI32,d7
		bsr	.init
		bne.s	.exit
		lea	(4+ssha_pt48-t,a5),a2
		move.w	#SBOB_MAXI48,d7
		bsr	.init
		bne.s	.exit
		lea	(4+ssha_pt64-t,a5),a2
		move.w	#SBOB_MAXI64,d7
		bsr	.init
		bne.s	.exit
		lea	(4+ssha_pt80-t,a5),a2
		move.w	#SBOB_MAXI80,d7
		bsr	.init
		bne.s	.exit
		lea	(4+ssha_pt256-t,a5),a2
		move.w	#SBOB_MAXI256,d7
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
		move.w	d7,d0			nbre de SBOBs maxi.
		lsl.l	#2,d0			*4
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(a2)
		beq.s	.outmem
.nores
		* Efface mémoire *
		move.l	(a2),a0
		subq.w	#1,d7
		moveq	#0,d0
.loop_clear	move.l	d0,(a0)+
		dbf	d7,.loop_clear
		moveq	#0,d7
.stop		rts
.outmem		moveq	#-12,d7
		rts

*»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-*
*---------------------->Libère la mémoire des SSHAs<----------------------*
*»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-*
ssha.freemem	lea	(ssha_xtable-t,a5),a0
		bsr	libere_mem
		lea	(ssha_ytable-t,a5),a0
		bsr	libere_mem

		*/ Libère les liste d'affichage des sshas */
		lea	(ssha_width16-t,a5),a0
		bsr	libere_mem
		lea	(ssha_width32-t,a5),a0
		bsr	libere_mem
		lea	(ssha_width48-t,a5),a0
		bsr	libere_mem
		lea	(ssha_width64-t,a5),a0
		bsr	libere_mem
		lea	(ssha_width80-t,a5),a0
		bsr	libere_mem
		lea	(ssha_width256-t,a5),a0
		bsr	libere_mem

		*/ Libère les liste de restaurations */
		lea	(ssha_listes16-t,a5),a0
		bsr	libere_mem
		lea	(ssha_listes32-t,a5),a0
		bsr	libere_mem
		lea	(ssha_listes48-t,a5),a0
		bsr	libere_mem
		lea	(ssha_listes64-t,a5),a0
		bsr	libere_mem
		lea	(ssha_listes80-t,a5),a0
		bsr	libere_mem
		lea	(ssha_listes256-t,a5),a0
		bsr	libere_mem
		lea	(ssha_listes16+4-t,a5),a0
		bsr	libere_mem
		lea	(ssha_listes32+4-t,a5),a0
		bsr	libere_mem
		lea	(ssha_listes48+4-t,a5),a0
		bsr	libere_mem
		lea	(ssha_listes64+4-t,a5),a0
		bsr	libere_mem
		lea	(ssha_listes80+4-t,a5),a0
		bsr	libere_mem
		lea	(ssha_listes256+4-t,a5),a0
		bsr	libere_mem

		*/ Libère les listes de pointeurs sur les liste d'affichage */
		lea	(4+ssha_pt16-t,a5),a0
		bsr	libere_mem
		lea	(4+ssha_pt32-t,a5),a0
		bsr	libere_mem
		lea	(4+ssha_pt48-t,a5),a0
		bsr	libere_mem
		lea	(4+ssha_pt64-t,a5),a0
		bsr	libere_mem
		lea	(4+ssha_pt80-t,a5),a0
		bsr	libere_mem
		lea	(4+ssha_pt256-t,a5),a0
		bsr	libere_mem
		rts
