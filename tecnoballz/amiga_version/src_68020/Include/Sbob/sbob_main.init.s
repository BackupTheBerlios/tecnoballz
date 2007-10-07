;>>>>>>>>>>>>>>>>>>>>>>				    <<<<<<<<<<<<<<<<<<<<<<;
;»»»»»»»»»»»»»»»»»»»»»»» INITIALISE AFFICHAGE SBOBS ««««««««««««««««««««««;
;>>>>>>>>>>>>>>>>>>>>>>				    <<<<<<<<<<<<<<<<<<<<<<;
* Entrée	=>	d0.l	page graphique SBOB
*		=>	a0.l	adresse table SBOB
*		=>	a5.l	adresse 't'
* Sortie	<=	d7.l=0	pas d'erreur
*		<=	Z=1	pas d'erreur
sbob_main.init:
		move.l	d0,(sbob_pagegfx-t,a5)
		move.l	a0,(sbob_liste-t,a5)
		bsr	sbob_calcul_msk
		bne.s	.exit
		bsr	sbob_init_liste
		bne.s	.exit
		bsr	sbob_initbob
.exit		rts

*»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-*
*--------------------->Libère la mémoire des SBOBs<-----------------------*
*»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-««»»-*
* Entrée	=>	a5.l	adresse 't'
sbob.freemem:
		lea	(sbob_pagemsk-t,a5),a0
		bsr	libere_mem
sbob.freemem2:	lea	(sbob_struct-t,a5),a0
		bsr	libere_mem
		lea	(sbob_xtable-t,a5),a0
		bsr	libere_mem
		lea	(sbob_ytable-t,a5),a0
		bsr	libere_mem

		*/ Libère les liste d'affichage des Sbobs */
		lea	(sbob_width16-t,a5),a0
		bsr	libere_mem
		lea	(sbob_width32-t,a5),a0
		bsr	libere_mem
		lea	(sbob_width48-t,a5),a0
		bsr	libere_mem
		lea	(sbob_width64-t,a5),a0
		bsr	libere_mem
		lea	(sbob_width80-t,a5),a0
		bsr	libere_mem
		lea	(sbob_width256-t,a5),a0
		bsr	libere_mem

		*/ Libère les liste de restaurations */
		lea	(sbob_listes16-t,a5),a0
		bsr	libere_mem
		lea	(sbob_listes32-t,a5),a0
		bsr	libere_mem
		lea	(sbob_listes48-t,a5),a0
		bsr	libere_mem
		lea	(sbob_listes64-t,a5),a0
		bsr	libere_mem
		lea	(sbob_listes80-t,a5),a0
		bsr	libere_mem
		lea	(sbob_listes256-t,a5),a0
		bsr	libere_mem
		lea	(sbob_listes16+4-t,a5),a0
		bsr	libere_mem
		lea	(sbob_listes32+4-t,a5),a0
		bsr	libere_mem
		lea	(sbob_listes48+4-t,a5),a0
		bsr	libere_mem
		lea	(sbob_listes64+4-t,a5),a0
		bsr	libere_mem
		lea	(sbob_listes80+4-t,a5),a0
		bsr	libere_mem
		lea	(sbob_listes256+4-t,a5),a0
		bsr	libere_mem

		*/ Libère les listes de pointeurs sur les liste d'affichage */
		lea	(4+sbob_pt16-t,a5),a0
		bsr	libere_mem
		lea	(4+sbob_pt32-t,a5),a0
		bsr	libere_mem
		lea	(4+sbob_pt48-t,a5),a0
		bsr	libere_mem
		lea	(4+sbob_pt64-t,a5),a0
		bsr	libere_mem
		lea	(4+sbob_pt80-t,a5),a0
		bsr	libere_mem
		lea	(4+sbob_pt256-t,a5),a0
		bsr	libere_mem
		rts

