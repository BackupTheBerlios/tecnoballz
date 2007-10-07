*=========================================================================*
*========================> Décode page graphique <========================*
*=========================================================================*
* Décompacte la page graphique ILBM et libère la mémoire du fichier ILBM
* Entrée	=> d0.l	adr. page gfx ILBM
*		=> a2.l	adr. table couleur
* Sortie	<= d0.l	adr. page graphique
*		<= a2.l	adr. table couleur ou 0 si pas de table couleur
*		<= d7.l	=0 ou Z=1 si pas d'erreur
decode_page:
		lea	(.adr_color,pc),a1
		move.l	a2,(a1)
		lea	(.adr_fichier,pc),a0	
		move.l	d0,(a0)
		bsr	.calcul_taille
		bne.s	.exit			-> erreur
		movem.l	d0-d4,-(a7)
		bsr	.reserve_memoire
		beq.s	.out_of_memory		-> plus de mémoire disponible
		movem.l	(a7)+,d0-d4
		bsr	.decompacte_page
		bne.s	.exit			-> erreur
		bsr	.libere_memory
		bne.s	.exit			-> erreur
		move.l	(.adr_color,pc),a2
		move.l	(.adr_page_gfx,pc),d0
		moveq	#0,d7
.exit		rts
.out_of_memory	movem.l	(a7)+,d0-d4
		moveq	#-9,d7			¤chargement dépass. mémoire
		rts

*---------------| Calcul la taille de l'image décompacté |----------------*
* entrée 	=> d0.l	adr. fichier IFF
* sortie	<= d0.w	largeur en octets de la page
*		<= d1.w	hauteur en lignes de la page
*		<= d2.w	nombre de plan(s) de la page
*		<= d3.l	taille en octets de la page
*		<= d4.w	taille d'une ligne en octets
.calcul_taille
		move.l	d0,a4	
		sub.l	a2,a2
		sub.l	a3,a3
		bsr	decompacte_ilbm
		lsr.w	#3,d0
		move.w	d0,d4
		mulu.w	d2,d4
		tst.l	d7			erreur ?
		rts

*---------------| Reserve mémoire pour la page graphique |----------------*
* entrée	=> d3.l	taille page graphique
.reserve_memoire
		add.l	#PG_DATA,d3		quelques octets en plus
		move.l	d3,d0			d0 = taille image
		moveq	#MEMF_CHIP,d1		mémoire CHIP
		bsr	reserve_memoire
		lea	(.adr_page_gfx,pc),a0
		move.l	d0,(a0)
		tst.l	d0			erreur ?
		rts

*--------------------| Décompacte la page graphique |---------------------*
* entrée	=> d0.w	largeur en octets de la page
*		=> d1.w	hauteur en lignes de la page
*		=> d2.w	nombre de plan(s) de la page
*		=> d3.l	taille en octets de la page
*		=> d4.w	taille d'une ligne en octets
.decompacte_page
		move.l	(.adr_page_gfx,pc),a3
		move.l	#'PAGE',(PG_ID,a3)
		move.w	d0,(PG_SPLAN,a3)
		move.w	d1,(PG_HEIGHT,a3)
		move.w	d4,(PG_SLINE,a3)
		move.w	d2,(PG_NPLAN,a3)
		move.l	d3,(PG_SIZE,a3)
		lea	(PG_DATA,a3),a3
		move.l	(.adr_color,pc),a2
		move.l	(.adr_fichier,pc),a4
		bsr	decompacte_ilbm
		tst.l	d7			erreur ?
		rts
*----------------------| Libère mémoire chargement |----------------------*
.libere_memory
		move.l	(.adr_fichier,pc),d0
		bsr	libere_memoire
		tst.l	d7
		rts
*-------------------------| adresses utilisées |--------------------------*
		CNOP	0,4
.adr_page_gfx	dc.l	0		adr. page graphique RAW
.adr_fichier	dc.l	0		adr. fichier IFF-ILBM
.adr_color	dc.l	0		Adr. table couleur


*=========================================================================*
*========================> Décode page graphique <========================*
*=========================================================================*
* Décompacte la page graphique ILBM et libère la mémoire du fichier ILBM
* Entrée	=> 'load.colo'	Pt/table couleur
*		=> 'load.temp'  Structure RESA fichier IFF-ILBM
*		=> 'load.resa'	Pt/structure RESA
* Sortie	<= d0.l	adr. page graphique
*		<= a2.l	adr. table couleur ou 0 si pas de table couleur
*		<= d7.l	=0 ou Z=1 si pas d'erreur
decode.page:
		bsr	.calcul_taille
		bne.s	.exit			-> erreur
		movem.l	d0-d4,-(a7)
		bsr	.reserve_memoire
		movem.l	(a7)+,d0-d4
		tst.l	d7
		bne.s	.exit
		bsr	.decompacte_page
		bne.s	.exit			-> erreur
		lea	(load.temp-t,a5),a0
		bsr	libere_mem
.exit		rts

*---------------| Calcul la taille de l'image décompacté |----------------*
* entrée 	=> d0.l	adr. fichier IFF
* sortie	<= d0.w	largeur en octets de la page
*		<= d1.w	hauteur en lignes de la page
*		<= d2.w	nombre de plan(s) de la page
*		<= d3.l	taille en octets de la page
*		<= d4.w	taille d'une ligne en octets
.calcul_taille
		lea	(load.temp-t,a5),a4
		move.l	(RESA.ADR,a4),a4	a4=pt/fichier IFF
		sub.l	a2,a2
		sub.l	a3,a3
		bsr	decompacte_ilbm
		lsr.w	#3,d0
		move.w	d0,d4
		mulu.w	d2,d4
		tst.l	d7			erreur ?
		rts
*---------------| Reserve mémoire pour la page graphique |----------------*
* entrée	=> d3.l	taille page graphique
.reserve_memoire
		move.l	(load.resa-t,a5),a4
		cmp.l	#0,a4
		beq.s	.noresa
		add.l	#PG_DATA,d3		quelques octets en plus
		move.l	d3,(RESA.SIZ,a4)
		*/move.l	#MEMF_CHIP,(RESA.TYP,a4)
		bsr	reserve.mem
.noresa		rts
*--------------------| Décompacte la page graphique |---------------------*
* entrée	=> d0.w	largeur en octets de la page
*		=> d1.w	hauteur en lignes de la page
*		=> d2.w	nombre de plan(s) de la page
*		=> d3.l	taille en octets de la page
*		=> d4.w	taille d'une ligne en octets
.decompacte_page
		move.l	(load.resa-t,a5),a4
		move.l	(RESA.ADR,a4),a3
		cmp.l	#0,a3
		beq.s	.noresa2
		move.l	#'PAGE',(PG_ID,a3)
		move.w	d0,(PG_SPLAN,a3)
		move.w	d1,(PG_HEIGHT,a3)
		move.w	d4,(PG_SLINE,a3)
		move.w	d2,(PG_NPLAN,a3)
		move.l	d3,(PG_SIZE,a3)
		lea	(PG_DATA,a3),a3
.noresa2	move.l	(load.colo-t,a5),a2
		lea	(load.temp-t,a5),a4
		move.l	(RESA.ADR,a4),a4	a4=pt/fichier IFF
		bsr	decompacte_ilbm
		rts
