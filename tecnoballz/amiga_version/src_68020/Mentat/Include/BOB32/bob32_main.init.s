;>>>>>>>>>>>>>>>>>>>>>>				    <<<<<<<<<<<<<<<<<<<<<<;
;»»»»»»»»»»»»»»»»»»»»»»» INITIALISE AFFICHAGE BOBS «««««««««««««««««««««««;
;>>>>>>>>>>>>>>>>>>>>>>				    <<<<<<<<<<<<<<<<<<<<<<;
* Entrée	=>	d0.l	page graphique BOB
*		=>	a0.l	adresse table BOB
*		=>	a5.l	adresse 't'
* Sortie	<=	d7.l=0	pas d'erreur
*		<=	Z=1	pas d'erreur
bob32_main.init:
		move.l	d0,(bob32_pagegfx-t,a5)
		move.l	a0,(bob32_liste-t,a5)
		bsr	bob32_calcul_msk
		bne.s	.exit
		bsr	bob32_init_liste
		bne.s	.exit
		bsr	bob32_initbob
.exit		rts

*»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-*
*---------------------->Libère la mémoire des BOBs<-----------------------*
*»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-*
* Entrée	=>	a5.l	adresse 't'
xx32		rts
bob32.freemem:
		lea	(bob32_pagemsk-t,a5),a0
		bsr	libere_mem
		bne.s	xx32
bob32.freemem2:	lea	(bob32_struct-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_xbyte-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_bltcon-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_ymulu-t,a5),a0
		bsr	libere_mem
		bne	.exit

		*/ Libère les liste d'affichage des BOBs */
		lea	(bob32_width16-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_width32-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_width48-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_width64-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_width80-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_width96-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_width192-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_width256-t,a5),a0
		bsr	libere_mem
		bne	.exit

		*/ Libère les liste de restaurations */
		lea	(bob32_listes16-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_listes32-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_listes48-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_listes64-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_listes80-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_listes96-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_listes192-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_listes256-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_listes16+4-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_listes32+4-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_listes48+4-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_listes64+4-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_listes80+4-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_listes96+4-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_listes192+4-t,a5),a0
		bsr	libere_mem
		bne	.exit
		lea	(bob32_listes256+4-t,a5),a0
		bsr	libere_mem
		bne.s	.exit

		*/ Libère les listes de pointeurs sur les liste d'affichage */
		lea	(4+bob32_pt16-t,a5),a0
		bsr	libere_mem
		bne.s	.exit
		lea	(4+bob32_pt32-t,a5),a0
		bsr	libere_mem
		bne.s	.exit
		lea	(4+bob32_pt48-t,a5),a0
		bsr	libere_mem
		bne.s	.exit
		lea	(4+bob32_pt64-t,a5),a0
		bsr	libere_mem
		bne.s	.exit
		lea	(4+bob32_pt80-t,a5),a0
		bsr	libere_mem
		bne.s	.exit
		lea	(4+bob32_pt96-t,a5),a0
		bsr	libere_mem
		bne.s	.exit
		lea	(4+bob32_pt192-t,a5),a0
		bsr	libere_mem
		bne.s	.exit
		lea	(4+bob32_pt256-t,a5),a0
		bsr	libere_mem
		bne.s	.exit
		moveq	#0,d7
.exit		rts
