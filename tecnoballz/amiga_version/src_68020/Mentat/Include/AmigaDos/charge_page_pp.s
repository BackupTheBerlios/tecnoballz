*-------------------------------------------------------------------------*
*==================> Charge page graphique powerpackée <==================*
*-------------------------------------------------------------------------*
* Charge page graphique compacté PP & ILBM
* Entrée	=> a0.l	Adr. nom fichier AmigaDos ILBM powerpackée \
* 		=> a2.l	Adr. table couleur ou 0 si pas de table couleur
*		=> a5.l	Adr. 't'
* Sortie	<= d0.l	adr. page graphique raw-blitter
*		<= a0.l	Adr. nom fichier AmigaDos ILBM
*		<= a2.l	Adr. table couleur ou 0 si pas de table couleur
*		<= d7.l =0 / Z=1 pas d'erreur
charge_page_pp:
		lea	(.adr_color,pc),a1
		move.l	a2,(a1)
		bsr	.load_file_pp
		bne.s	.exit			-> erreur
		move.l	(.adr_color,pc),a2
		bsr	decode_page
		tst.l	d7
		bne.s	.exit
		move.l	(.nom_page_gfx,pc),a0
		moveq	#0,d7
.exit		rts

*--------------> Charge la page graphique IFF powerpackée <---------------*
.load_file_pp
		lea	(.nom_page_gfx,pc),a1
		move.l	a0,(a1)
		moveq	#0,d0			mémoire CHIP ou FAST
		bsr	charge_pp
		tst.l	d7			erreur ?
		rts

*-------------------------> adresses utilisées <--------------------------*
		CNOP	0,4
.nom_page_gfx	dc.l	0		Adr. chaine ASCII du nom de fichier
.adr_color	dc.l	0		Adr. table couleur

*-------------------------------------------------------------------------*
*==================> Charge page graphique powerpackée <==================*
*-------------------------------------------------------------------------*
* Charge page graphique compacté PP & ILBM
* Entrée	=> a0.l	Adr. nom fichier AmigaDos ILBM powerpackée \
* 		=> a2.l	Adr. table couleur ou 0 si pas de table couleur
*		=> a5.l	Adr. 't'
* Sortie	<= d0.l	adr. page graphique raw-blitter
*		<= a0.l	Adr. nom fichier AmigaDos ILBM
*		<= a2.l	Adr. table couleur ou 0 si pas de table couleur
*		<= d7.l =0 / Z=1 pas d'erreur
charge.pagepp:
		move.l	a0,(load.name-t,a5)
		move.l	a2,(load.colo-t,a5)
		move.l	a4,(load.resa-t,a5)
		lea	(load.temp-t,a5),a4
		bsr	charge.pp
		bne.s	.exit
		bsr	decode.page
.exit		rts
