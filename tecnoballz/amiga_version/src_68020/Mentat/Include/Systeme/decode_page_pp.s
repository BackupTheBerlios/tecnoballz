*-------------------------------------------------------------------------*
*****************> Décompacte fichier PP & Fichier ILBM <******************
*-------------------------------------------------------------------------*
* Entrée	=> d0.l	adresse du fichier ILBM powerpacké
*		=> d1.l	taille du fichier ILBM powerpacké
* 		=> a2.l	Adr. table couleur ou 0 si pas de table couleur
* Sortie	<= d0.l	adr. page graphique
*		<= a2.l	adr. table couleur ou 0 si pas de table couleur
*		<= d7.l	=0 ou Z=1 si pas d'erreur
decode_page_pp:
		*/ Décompacte le fichier powerpacké */
		lea	(.adr_color,pc),a0
		move.l	a2,(a0)
		moveq	#0,d2
		bsr	decrunch_pp
		bne.s	.exit
	
		*/ Décompacte le fichier ILBM */
		move.l	(.adr_color,pc),a2
		bsr	decode_page
		bne.s	.exit
		moveq	#0,d7
.exit		rts
		CNOP	0,4
.adr_color	dc.l	0		Adr. table couleur

*-------------------------------------------------------------------------*
*****************> Décompacte fichier PP & Fichier ILBM <******************
*-------------------------------------------------------------------------*
* Entrée	=> a2.l	Adr. table couleur ou 0 si pas de table couleur
*		=> a3.l pt/RESA fichier PP
*		=> a4.l pt/RESA destination
* Sortie	<= d7.l	=0 ou Z=1 si pas d'erreur
decode.pagepp:
		move.l	a4,(load.resa-t,a5)
		move.l	a2,(load.colo-t,a5)
		move.l	(RESA.ADR,a3),d0
		move.l	(RESA.SIZ,a3),d1
		lea	(load.temp-t,a5),a4
		bsr	decrunch.pp
		bne.s	.exit
		bsr	decode.page
.exit		rts
