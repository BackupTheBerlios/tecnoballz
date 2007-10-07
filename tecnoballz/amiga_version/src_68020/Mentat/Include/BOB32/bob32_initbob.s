***************************************************************************
*=X=*=X=*=X=*=X=*=X=* Initialise l'affichage de BOBs =X=*=X=*=X=*=X=*=X=*=*
***************************************************************************
* Entrée	=> a5.l	adr. 't'
* ATTENTION UN ECRAN DOIT ETRE DEJA INITIALISE
bob32_initbob:
		tst.l	(pt_screen-t,a5)
		beq.s	.noscreen
		tst.l	(bob32_pagegfx-t,a5)
		beq.s	.nopage
		bsr	bob32_tab.mem
		bne.s	.exit
		bsr	bob32_liste.init
		bne.s	.exit
		bsr.s	bob32_memory
		bne.s	.exit
		bsr	vbob32_tab.mem
		bne.s	.exit
		bsr	vbob32_liste.init
		bne.s	.exit
		bsr	b32_init_xoctet
		bsr	b32_init_ymulu
		moveq	#0,d7
.exit		rts
.noscreen	moveq	#-32,d7			¤ pas de "pt_screen"
		rts
.nopage		moveq	#-30,d7			¤ pas de "bob32_pagegfx"
		rts

*-------------------* réserve mémoire pour les tables *-------------------*
* Entrée	=> a5.l	adr. 't'
* Sortie	<= d7.l=0 / Z=1 pas d'erreur
bob32_memory:
		*» Réserve mémoire pour les structures BOBs «*
		lea	(bob32_struct-t,a5),a4
		bsr	reserve.mem
		bne.s	.exit
		
		*» Réserve la table 'xbyte' «*
		moveq	#0,d0
		move.w	([pt_screen-t,a5],SPLARGE),d0
		add.l	d0,d0			2 octets par pixel
		lea	(bob32_xbyte-t,a5),a4
		move.l	d0,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne.s	.exit

		*» Réserve la table 'bltcon' «*
		moveq	#0,d0
		move.w	([pt_screen-t,a5],SPLARGE),d0
		lsl.l	#2,d0			4 octets par pixel
		lea	(bob32_bltcon-t,a5),a4
		move.l	d0,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne.s	.exit

		*» Réserve la table 'ymulu' «*
		moveq	#0,d0
		move.w	([pt_screen-t,a5],SPHAUTEUR),d0
		lsl.l	#2,d0			4 octets par ligne
		lea	(bob32_ymulu-t,a5),a4
		move.l	d0,(RESA.SIZ,a4)
		bsr	reserve.mem
		bne.s	.exit
		moveq	#0,d7
.exit		rts

*--------------* initialise les tables 'xbyte' & 'bltcon' *---------------*
* Entrée	=> a5.l	adr. 't'
b32_init_xoctet	move.l	(bob32_xbyte-t,a5),a1
		move.l	(bob32_bltcon-t,a5),a2
		move.w	([pt_screen-t,a5],SPLARGE),d0
		subq.w	#1,d0
		moveq	#0,d1			d1=pos. x bob en pixels
		move.w	#$FFFE,d4
		moveq	#$F,d5
		move.w	#$0FCA,d6		liaison logique
.loop		move.w	d1,d2
		move.w	d1,d3
		lsr.w	#3,d2			convertit pixels -> octets
		and.w	d4,d2			tjrs un octet paire (&$FFFE)
		move.w	d2,(a1)+		sauve pos. x en octet
		and.w	d5,d3			d3=décalage blitter (&$000F)
		ror.w	#4,d3
		move.w	d3,(a2)			décalage masque
		or.w	d6,(a2)+		+liaison logique (!$0FCA)
		move.w	d3,(a2)+		décalage gfx
		addq.w	#1,d1			x=x+1
		dbf	d0,.loop
		rts

*-------------------------* initialise 'ymulu' *--------------------------*
* Entrée	=> a5.l	adr. 't'
b32_init_ymulu	move.l	(bob32_ymulu-t,a5),a1
		move.w	([pt_screen-t,a5],SPHAUTEUR),d0
		subq.w	#1,d0
		moveq	#0,d1
		move.w	([pt_screen-t,a5],SP_SLINE),d1
		moveq	#0,d2			d0=pt/première ligne
.loop		move.l	d2,(a1)+		sauve pt
		add.l	d1,d2			adr. prochaine ligne
		dbf	d0,.loop
		rts

*------------* réserve listes d'affichage et de retauration *-------------*
* Entrée	=> a5.l	adr. 't'
* Sortie	<= d7.l=0 / Z=1 pas d'erreur
bob32_liste.init:
		lea	(bob32_listes16-t,a5),a1
		lea	(bob32_width16-t,a5),a0
		lea	(bob32_pt16-t,a5),a2
		move.w	#BOB32_MAXI16,d7
		moveq	#4,d6			largeur 2+2 octets
		bsr	.init
		bne	.exit

		lea	(bob32_listes32-t,a5),a1
		lea	(bob32_width32-t,a5),a0
		lea	(bob32_pt32-t,a5),a2
		move.w	#BOB32_MAXI32,d7
		moveq	#6,d6			largeur 4+2 octets
		bsr	.init
		bne	.exit

		lea	(bob32_listes48-t,a5),a1
		lea	(bob32_width48-t,a5),a0
		lea	(bob32_pt48-t,a5),a2
		move.w	#BOB32_MAXI48,d7
		moveq	#8,d6			largeur 6+2 octets
		bsr	.init
		bne.s	.exit

		lea	(bob32_listes64-t,a5),a1
		lea	(bob32_width64-t,a5),a0
		lea	(bob32_pt64-t,a5),a2
		move.w	#BOB32_MAXI64,d7
		moveq	#10,d6			largeur 8+2 octets
		bsr	.init
		bne.s	.exit

		lea	(bob32_listes80-t,a5),a1
		lea	(bob32_width80-t,a5),a0
		lea	(bob32_pt80-t,a5),a2
		move.w	#BOB32_MAXI80,d7
		moveq	#12,d6			largeur 10+2 octets
		bsr	.init
		bne.s	.exit

		lea	(bob32_listes96-t,a5),a1
		lea	(bob32_width96-t,a5),a0
		lea	(bob32_pt96-t,a5),a2
		move.w	#BOB32_MAXI96,d7
		moveq	#14,d6			largeur 12+2 octets
		bsr	.init

		lea	(bob32_listes192-t,a5),a1
		lea	(bob32_width192-t,a5),a0
		lea	(bob32_pt192-t,a5),a2
		move.w	#BOB32_MAXI192,d7
		moveq	#26,d6			largeur 24+2 octets
		bsr	.init

		lea	(bob32_listes256-t,a5),a1
		lea	(bob32_width256-t,a5),a0
		lea	(bob32_pt256-t,a5),a2
		move.w	#BOB32_MAXI256,d7
		moveq	#34,d6			largeur 32+2 octets
		bsr	.init
.exit		rts

		 * initialisation des listes d'affichage *
* entrée	=> d6.w	largeur du bob + décalage
*		=> d7.w	nbre de bobs maxi
*		=> a0.l	pt/liste d'affichage 'bob32_width??'
*		=> a1.l	pt/les 2 listes de restauration 'bob32_listes??'
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

		* liste d'affichage "bob32_widthXX" *
		move.l	d2,d0
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(a0)
		beq	.outmem

		* liste de retauration écran 1 "bob32_listeXX" *
		move.l	d2,d0
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,(a1)
		beq.s	.outmem

		* liste de retauration écran 2 "bob32_listeXX" *
		move.l	d2,d0
		moveq	#0,d1
		bsr	reserve_memoire
		move.l	d0,4(a1)
		beq.s	.outmem

		* efface liste mémoire *
.nores		subq.w	#1,d7
		moveq	#0,d0
		move.l	(a0),a2			"bob32_widthXX"
		move.l	(a1),a3			"bob32_listeXX" écran 1
		move.l	(4.w,a1),a4		"bob32_listeXX" écran 2
.loop		move.l	d0,(a2)+
		move.l	d0,(a2)+
		move.l	d0,(a3)+
		move.l	d0,(a3)+
		move.l	d0,(a4)+
		move.l	d0,(a4)+
		dbf	d7,.loop

		* écrit adr. des listes de retaur. dans liste d'aff. *
		move.l	(a0),a2			"bob32_widthXX"
		move.l	(a1),(8,a2)		liste rest. écran 1
		move.l	(4,a1),(12,a2)		liste rest. écran 2
		addq.l	#4,(8,a2)		saute les modulos blitter A & D
		addq.l	#4,(12,a2)		saute les modulos blitter A & D

		* calcul modulos blitter *
		move.l	(pt_screen-t,a5),a2
		move.l	(bob32_pagegfx-t,a5),a3
		move.w	(PG_SPLAN,a3),d1	d1=largeur page bob en octets
		move.w	(SP_SPLAN,a2),d0	d0=largeur écran en octets
		sub.w	d6,d0			d0=modulo écran
		sub.w	d6,d1			d1=modulo page bob
		move.l	(a0),a2			"bob32_widthXX"
		move.l	(a1),a3			"bob32_listeXX" écran 1
		move.l	(4,a1),a4		"bob32_listeXX" écran 2
		move.w	d0,(a2)+		modulo C affichage
		move.w	d1,(a2)+		modulo B affichage
		move.w	d1,(a2)+		modulo A affichage
		move.w	d0,(a2)+		modulo D affichage
		addq.l	#8,a2			saute adr. listes écran 1 & 2
		move.l	d5,a1			a1="bob32_ptXX"
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
bob32_tab.mem:
		lea	(4+bob32_pt16-t,a5),a2
		move.w	#BOB32_MAXI16,d7
		bsr	.init
		bne.s	.exit
		lea	(4+bob32_pt32-t,a5),a2
		move.w	#BOB32_MAXI32,d7
		bsr	.init
		bne.s	.exit
		lea	(4+bob32_pt48-t,a5),a2
		move.w	#BOB32_MAXI48,d7
		bsr	.init
		bne.s	.exit
		lea	(4+bob32_pt64-t,a5),a2
		move.w	#BOB32_MAXI64,d7
		bsr	.init
		bne.s	.exit
		lea	(4+bob32_pt80-t,a5),a2
		move.w	#BOB32_MAXI80,d7
		bsr	.init
		bne.s	.exit
		lea	(4+bob32_pt96-t,a5),a2
		move.w	#BOB32_MAXI96,d7
		bsr	.init
		lea	(4+bob32_pt192-t,a5),a2
		move.w	#BOB32_MAXI192,d7
		bsr	.init
		lea	(4+bob32_pt256-t,a5),a2
		move.w	#BOB32_MAXI256,d7
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


***************************************************************************
* INITIALISATION DES BOBS VERTICAUX
***************************************************************************
*------------* réserve listes d'affichage et de retauration *-------------*
* Entrée	=> a5.l	adr. 't'
* Sortie	<= d7.l=0 / Z=1 pas d'erreur
vbob32_liste.init:
		lea	(vbob32_listes16-t,a5),a1
		lea	(vbob32_width16-t,a5),a0
		lea	(vbob32_pt16-t,a5),a2
		move.w	#VBOB32_MAXI16,d7
		moveq	#2,d6			largeur 2 octets
		bsr	.init
		bne.s	.exit

		lea	(vbob32_listes32-t,a5),a1
		lea	(vbob32_width32-t,a5),a0
		lea	(vbob32_pt32-t,a5),a2
		move.w	#VBOB32_MAXI32,d7
		moveq	#4,d6			largeur 4 octets
		bsr	.init
		bne.s	.exit

		lea	(vbob32_listes48-t,a5),a1
		lea	(vbob32_width48-t,a5),a0
		lea	(vbob32_pt48-t,a5),a2
		move.w	#VBOB32_MAXI48,d7
		moveq	#6,d6			largeur 6 octets
		bsr	.init
		bne.s	.exit

		lea	(vbob32_listes64-t,a5),a1
		lea	(vbob32_width64-t,a5),a0
		lea	(vbob32_pt64-t,a5),a2
		move.w	#VBOB32_MAXI64,d7
		moveq	#8,d6			largeur 8 octets
		bsr	.init
		bne.s	.exit

		lea	(vbob32_listes80-t,a5),a1
		lea	(vbob32_width80-t,a5),a0
		lea	(vbob32_pt80-t,a5),a2
		move.w	#VBOB32_MAXI80,d7
		moveq	#10,d6			largeur 10 octets
		bsr	.init
		bne.s	.exit

		lea	(vbob32_listes96-t,a5),a1
		lea	(vbob32_width96-t,a5),a0
		lea	(vbob32_pt96-t,a5),a2
		move.w	#VBOB32_MAXI96,d7
		moveq	#12,d6			largeur 12 octets
		bsr	.init
.exit		rts

		 * initialisation des listes d'affichage *
* entrée	=> d6.w	largeur du bob
*		=> d7.w	nbre de bobs maxi
*		=> a0.l	pt/liste d'affichage 'vbob32_width??'
*		=> a1.l	pt/les 2 listes de restauration 'vbob32_listes??'
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
		move.l	(bob32_pagegfx-t,a5),a3
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
vbob32_tab.mem:
		lea	(4+vbob32_pt16-t,a5),a2
		move.w	#VBOB32_MAXI16,d7
		bsr	.init
		bne.s	.exit
		lea	(4+vbob32_pt32-t,a5),a2
		move.w	#VBOB32_MAXI32,d7
		bsr	.init
		bne.s	.exit
		lea	(4+vbob32_pt48-t,a5),a2
		move.w	#VBOB32_MAXI48,d7
		bsr	.init
		bne.s	.exit
		lea	(4+vbob32_pt64-t,a5),a2
		move.w	#VBOB32_MAXI64,d7
		bsr	.init
		bne.s	.exit
		lea	(4+vbob32_pt80-t,a5),a2
		move.w	#VBOB32_MAXI80,d7
		bsr	.init
		bne.s	.exit
		lea	(4+vbob32_pt96-t,a5),a2
		move.w	#VBOB32_MAXI96,d7
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
