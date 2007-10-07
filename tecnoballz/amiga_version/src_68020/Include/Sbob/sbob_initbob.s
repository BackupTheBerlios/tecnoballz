***************************************************************************
*=*=*=*=*=*=*=*=*=*=* INITIALISE L'AFFICHAGE DE SBOBS *=*=*=*=*=*=*=*=*=*=*
***************************************************************************
* Entrée	=> a5.l	adr. 't'
sbob_initbob:
		tst.l	(pt_screen-t,a5)
		beq.s	.noscreen
		tst.l	(sbob_pagegfx-t,a5)
		beq.s	.nopage
		bsr	sbob_tab.mem		-> réserve liste "last_table"
		bne.s	.exit
		bsr	sbob_liste.init
		bne.s	.exit
		bsr.s	sbob_memory
		bne.s	.exit
		bsr	vsbob_tab.mem
		bne.s	.exit
		bsr	vsbob_liste.init
		bne.s	.exit
		bsr	sb_init_xtable
		bsr	sb_init_ytable
		moveq	#0,d7
.exit		rts
.noscreen	moveq	#-32,d7			¤ pas de "pt_screen"
		rts
.nopage		moveq	#-30,d7			¤ pas de "sbob_pagegfx"
		rts

*-------------------* réserve mémoire pour les tables *--------------------*
* Entrée	=> a5.l	adr. 't'
* Sortie	<= d7.l=0 / Z=1 pas d'erreur
sbob_memory:
		* Réserve mémoire structures SBOBs *
		lea	(sbob_struct-t,a5),a4
		move.l	#SBOB.SIZE*SBOB_MAXI,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne.s	.exit	
	
		* Réserve la  'sbob_xtable'*
		move.l	(pt_screen-t,a5),a0
		lea	(sbob_xtable-t,a5),a4
		moveq	#0,d0
		move.w	(SPLARGE,a0),d0		d0=largeur écran en pixels
		lsl.l	#3,d0			8 octets par pixel
		move.l	d0,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne.s	.exit

		* Réserve la 'sbob_ytable' *
		lea	(sbob_ytable-t,a5),a4
		moveq	#0,d0
		move.w	(SPHAUTEUR,a0),d0	d0=largeur écran en pixels
		lsl.l	#3,d0			4 octets pour 2 X nbre lignes
		move.l	d0,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne.s	.exit
		moveq	#0,d7
.exit		rts

*---------------------* initialise la 'sbob_xtable' *---------------------*
* Entrée	=> a5.l	adr. 't'
sb_init_xtable	move.l	(pt_screen-t,a5),a0
		move.l	(sbob_xtable-t,a5),a1
		move.w	(SPLARGE,a0),d0
		subq.w	#1,d0
		moveq	#0,d1			d1=pos. x bob en pixels
		move.w	#$FFFE,d4
		moveq	#$F,d5
		move.w	#$0FCA,d6		liaison logique
.loop		moveq	#0,d2
		move.w	d1,d2
		move.w	d1,d3
		lsr.w	#3,d2			convertit pixels -> octets
		and.w	d4,d2			tjrs un octet paire (&$FFFE)
		move.l	d2,(a1)+		sauve pos. x en octet
		and.w	d5,d3			d3=décalage blitter (&$000F)
		ror.w	#4,d3
		move.w	d3,(a1)			décalage masque
		or.w	d6,(a1)+		+liaison logique (!$0FCA)
		move.w	d3,(a1)+		décalage gfx
		addq.w	#1,d1			x=x+1
		dbf	d0,.loop
		rts

*-------------------------* initialise 'ytable' *-------------------------*
* Entrée	=> a5.l	adr. 't'
sb_init_ytable:	move.l	(pt_screen-t,a5),a0
		move.l	(sbob_ytable-t,a5),a1
		move.w	(SPHAUTEUR,a0),d0	d0=nbre de lignes de la page
		sub.w	(SPDOWN,a0),d0
		subq.w	#1,d0
		moveq	#0,d1
		move.w	(SP_SLINE,a0),d1	d1=taille ligne en octets
		moveq	#0,d2			d0=pt/première ligne
.loop		move.l	d2,(a1)+		sauve pt
		add.l	d1,d2			adr. prochaine ligne
		dbf	d0,.loop
		move.w	(SPHAUTEUR,a0),d0	d0=nbre de lignes de la page
		subq.w	#1,d0
		move.w	(SPUP,a0),d2
		mulu.w	d1,d2
.loop2		move.l	d2,(a1)+		sauve pt
		add.l	d1,d2			adr. prochaine ligne
		dbf	d0,.loop2
		rts

*------------* réserve listes d'affichage et de retauration *-------------*
* Entrée	=> a5.l	adr. 't'
* Sortie	<= d7.l=0 / Z=1 pas d'erreur
sbob_liste.init:
		lea	(sbob_listes16-t,a5),a1
		lea	(sbob_width16-t,a5),a0
		lea	(sbob_pt16-t,a5),a2
		move.w	#SBOB_MAXI16,d7
		moveq	#4,d6			largeur 2+2 octets
		bsr	.init
		bne.s	.exit

		lea	(sbob_listes32-t,a5),a1
		lea	(sbob_width32-t,a5),a0
		lea	(sbob_pt32-t,a5),a2
		move.w	#SBOB_MAXI32,d7
		moveq	#6,d6			largeur 4+2 octets
		bsr	.init
		bne.s	.exit

		lea	(sbob_listes48-t,a5),a1
		lea	(sbob_width48-t,a5),a0
		lea	(sbob_pt48-t,a5),a2
		move.w	#SBOB_MAXI48,d7
		moveq	#8,d6			largeur 6+2 octets
		bsr	.init
		bne.s	.exit

		lea	(sbob_listes64-t,a5),a1
		lea	(sbob_width64-t,a5),a0
		lea	(sbob_pt64-t,a5),a2
		move.w	#SBOB_MAXI64,d7
		moveq	#10,d6			largeur 8+2 octets
		bsr	.init
		bne.s	.exit

		lea	(sbob_listes80-t,a5),a1
		lea	(sbob_width80-t,a5),a0
		lea	(sbob_pt80-t,a5),a2
		move.w	#SBOB_MAXI80,d7
		moveq	#12,d6			largeur 10+2 octets
		bsr	.init
		bne.s	.exit

		lea	(sbob_listes256-t,a5),a1
		lea	(sbob_width256-t,a5),a0
		lea	(sbob_pt256-t,a5),a2
		move.w	#SBOB_MAXI256,d7
		moveq	#34,d6			largeur 32+2 octets
		bsr	.init
.exit		rts

		 * initialisation des listes d'affichage *
* entrée	=> d6.w	largeur du bob + décalage
*		=> d7.w	nbre de bobs maxi
*		=> a0.l	pt/liste d'affichage 'sbob_width??'
*		=> a1.l	pt/les 2 listes de restauration 'sbob_listes??'
*		=> a2.l	pt/pt fin de la liste d'affichage 'sbob_pt??'
* sortie	<= d7.l=0 / Z=1 pas d'erreur
.init		move.l	a2,d5			sauve a2 dans d5
		tst.w	d7			des bobs prévus
		beq	.stop			-> non
		tst.l	(a0)			mémoire déjà réservée ?
		bne.s	.nores			-> oui
		moveq	#0,d2
		move.w	d7,d2
		lsl.l	#4,d2

		* liste d'affichage "sbob_widthXX" *
		move.l	d2,d0
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(a0)
		beq	.outmem

		* liste de retauration écran 1 "sbob_listeXX" *
		move.l	d2,d0
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(a1)
		beq	.outmem

		* liste de retauration écran 2 "sbob_listeXX" *
		move.l	d2,d0
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(4,a1)
		beq.s	.outmem

		* efface liste mémoire *
.nores		subq.w	#1,d7
		moveq	#0,d0
		move.l	(a0),a2			"sbob_widthXX"
		move.l	(a1),a3			"sbob_listeXX" écran 1
		move.l	(4,a1),a4		"sbob_listeXX" écran 2
.loop		move.l	d0,(a2)+
		move.l	d0,(a2)+
		move.l	d0,(a3)+
		move.l	d0,(a3)+
		move.l	d0,(a4)+
		move.l	d0,(a4)+
		dbf	d7,.loop

		* écrit adr. des listes de retaur. dans liste d'aff. *
		move.l	(a0),a2			"sbob_widthXX"
		move.l	(a1),(8,a2)		liste rest. écran 1
		move.l	(4,a1),(12,a2)		liste rest. écran 2
		addq.l	#4,(8,a2)		saute les modulos blitter A & D
		addq.l	#4,(12,a2)		saute les modulos blitter A & D

		* calcul modulos blitter *
		move.l	(pt_screen-t,a5),a2
		move.l	(sbob_pagegfx-t,a5),a3
		move.w	(PG_SPLAN,a3),d1	d1=largeur page bob en octets
		move.w	(SP_SPLAN,a2),d0	d0=largeur écran en octets
		sub.w	d6,d0			d0=modulo écran
		sub.w	d6,d1			d1=modulo page bob
		move.l	(a0),a2			"sbob_widthXX"
		move.l	(a1),a3			"sbob_listeXX" écran 1
		move.l	(4,a1),a4		"sbob_listeXX" écran 2
		move.w	d0,(a2)+		modulo C affichage
		move.w	d1,(a2)+		modulo B affichage
		move.w	d1,(a2)+		modulo A affichage
		move.w	d0,(a2)+		modulo D affichage
		addq.l	#8,a2			saute adr. listes écran 1 & 2
		move.l	d5,a1			a1="sbob_ptXX"
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
sbob_tab.mem:
		lea	(4+sbob_pt16-t,a5),a2
		move.w	#SBOB_MAXI16,d7
		bsr	.init
		bne.s	.exit
		lea	(4+sbob_pt32-t,a5),a2
		move.w	#SBOB_MAXI32,d7
		bsr	.init
		bne.s	.exit
		lea	(4+sbob_pt48-t,a5),a2
		move.w	#SBOB_MAXI48,d7
		bsr	.init
		bne.s	.exit
		lea	(4+sbob_pt64-t,a5),a2
		move.w	#SBOB_MAXI64,d7
		bsr	.init
		bne.s	.exit
		lea	(4+sbob_pt80-t,a5),a2
		move.w	#SBOB_MAXI80,d7
		bsr	.init
		bne.s	.exit
		lea	(4+sbob_pt256-t,a5),a2
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


***************************************************************************
* INITIALISATION DES BOBS VERTICAUX
***************************************************************************
*------------* réserve listes d'affichage et de retauration *-------------*
* Entrée	=> a5.l	adr. 't'
* Sortie	<= d7.l=0 / Z=1 pas d'erreur
vsbob_liste.init:
		lea	(vsbob_listes16-t,a5),a1
		lea	(vsbob_width16-t,a5),a0
		lea	(vsbob_pt16-t,a5),a2
		move.w	#VSBOB_MAXI16,d7
		moveq	#2,d6			largeur 2 octets
		bsr	.init
		bne.s	.exit

		lea	(vsbob_listes32-t,a5),a1
		lea	(vsbob_width32-t,a5),a0
		lea	(vsbob_pt32-t,a5),a2
		move.w	#VSBOB_MAXI32,d7
		moveq	#4,d6			largeur 4 octets
		bsr	.init
		bne.s	.exit

		lea	(vsbob_listes48-t,a5),a1
		lea	(vsbob_width48-t,a5),a0
		lea	(vsbob_pt48-t,a5),a2
		move.w	#VSBOB_MAXI48,d7
		moveq	#6,d6			largeur 6 octets
		bsr	.init
		bne.s	.exit

		lea	(vsbob_listes64-t,a5),a1
		lea	(vsbob_width64-t,a5),a0
		lea	(vsbob_pt64-t,a5),a2
		move.w	#VSBOB_MAXI64,d7
		moveq	#8,d6			largeur 8 octets
		bsr	.init
		bne.s	.exit

		lea	(vsbob_listes80-t,a5),a1
		lea	(vsbob_width80-t,a5),a0
		lea	(vsbob_pt80-t,a5),a2
		move.w	#VSBOB_MAXI80,d7
		moveq	#10,d6			largeur 10 octets
		bsr	.init
		bne.s	.exit

		lea	(vsbob_listes96-t,a5),a1
		lea	(vsbob_width96-t,a5),a0
		lea	(vsbob_pt96-t,a5),a2
		move.w	#VSBOB_MAXI96,d7
		moveq	#12,d6			largeur 12 octets
		bsr	.init
.exit		rts

		 * initialisation des listes d'affichage *
* entrée	=> d6.w	largeur du bob
*		=> d7.w	nbre de bobs maxi
*		=> a0.l	pt/liste d'affichage 'vsbob_width??'
*		=> a1.l	pt/les 2 listes de restauration 'vsbob_listes??'
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

		* liste d'affichage *
		move.l	d2,d0
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(a0)
		beq	.outmem

		* liste de retauration écran 1 *
		move.l	d2,d0
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(a1)
		beq.s	.outmem

		* liste de retauration écran 2 *
		move.l	d2,d0
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(4.w,a1)
		beq.s	.outmem

		* efface liste mémoire *
.nores		subq.w	#1,d7
		moveq	#0,d0
		move.l	(a0),a2
		move.l	(a1),a3
		move.l	(4.w,a1),a4
.loop		move.l	d0,(a2)+
		move.l	d0,(a2)+
		move.l	d0,(a3)+
		move.l	d0,(a3)+
		move.l	d0,(a4)+
		move.l	d0,(a4)+
		dbf	d7,.loop

		* écrit adr. des 2 listes de retauration dans liste d'aff. *
		move.l	(a0),a2
		move.l	(a1),(8.w,a2)		liste rest. écran 1
		move.l	(4.w,a1),(12.w,a2)	liste rest. écran 2
		addq.l	#4,(8.w,a2)		saute les modulos blitter A & D
		addq.l	#4,(12.w,a2)		saute les modulos blitter A & D

		* calcul modulos blitter *
		move.l	(pt_screen-t,a5),a2
		move.l	(sbob_pagegfx-t,a5),a3
		move.w	(PG_SPLAN,a3),d1	d1=largeur page bob en octets
		move	(SP_SPLAN,a2),d0	d0=largeur écran en octets
		sub.w	d6,d0			d0=modulo écran
		sub.w	d6,d1			d1=modulo page bob
		move.l	(a0),a2
		move.l	(a1),a3
		move.l	(4.w,a1),a4
		move.w	d0,(a2)+		modulo C affichage
		move.w	d1,(a2)+		modulo B affichage
		move.w	d1,(a2)+		modulo A affichage
		move.w	d0,(a2)+		modulo D affichage
		addq.l	#8,a2			saute adr. listes écran 1 & 2
		move.l	d5,a1
		move.l	a2,(a1)			pt/fin de la liste d'affichage
		move.w	d0,(a3)+		modulo A restauration
		move.w	d0,(a4)+		modulo A restauration
		move.w	d0,(a3)+		modulo D restauration
		move.w	d0,(a4)+		modulo D restauration
.stop		moveq	#0,d7
		rts
.outmem		moveq	#-12,d7			¤
		rts

*--------------------* réserve table fenêtre blitter *--------------------*
* Entrée	=> a5.l	Adr. 't'
* Sortie	<= d7.l=0 / Z=1 pas d'erreur
vsbob_tab.mem:
		lea	(4+vsbob_pt16-t,a5),a2
		move.w	#VSBOB_MAXI16,d7
		bsr	.init
		bne.s	.exit
		lea	(4+vsbob_pt32-t,a5),a2
		move.w	#VSBOB_MAXI32,d7
		bsr	.init
		bne.s	.exit
		lea	(4+vsbob_pt48-t,a5),a2
		move.w	#VSBOB_MAXI48,d7
		bsr	.init
		bne.s	.exit
		lea	(4+vsbob_pt64-t,a5),a2
		move.w	#VSBOB_MAXI64,d7
		bsr	.init
		bne.s	.exit
		lea	(4+vsbob_pt80-t,a5),a2
		move.w	#VSBOB_MAXI80,d7
		bsr	.init
		bne.s	.exit
		lea	(4+vsbob_pt96-t,a5),a2
		move.w	#VSBOB_MAXI96,d7
		bsr	.init
.exit		rts

		 * initialisation des tables blitter  *
* Entrée	=> d7.w	nbre de bobs maxi
*		=> a2.l	pt/pt fin de la liste d'affichage
* Sortie	<= d7.l=0 / Z=1 pas d'erreur
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
