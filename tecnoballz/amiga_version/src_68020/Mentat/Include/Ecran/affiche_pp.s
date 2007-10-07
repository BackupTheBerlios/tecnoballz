*»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»*
****************> AFFICHE UNE IMAGE IFF-ILBM POWERPACKEE <*****************
*»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»»««»*
* Entrée	=> d0.l	Adr. du fichier powerpacké
*		=> d1.l Taille du fichier powerpacké
*		=> a0.l	pt/structure "SCREEN"
*		=> a5.l	Adr. 't'
* Sortie	<= a0.l	Structure écran
*		<= d7.l = #0 / Z=1 Pas d'erreur
affiche_pp:
		lea	(ai_hardscroll_f,pc),a1
		move.w	(SHARDSCROLL.F,a0),(a1)
		lea	(ai_nnbuffer,pc),a1
		move.w	(SPBUFFER,a0),(a1)
		lea	(.screen,pc),a1
		move.l	a0,(a1)
		lea	(.ppfile,pc),a0
		move.l	d0,(a0)
		lea	(.ppsize,pc),a0
		move.l	d1,(a0)


		*/ Décompacte l'image PowerPacker /*
		move.l	(.ppfile,pc),d0
		move.l	(.ppsize,pc),d1
		moveq	#0,d2			fast/chip
		bsr	decrunch_pp
		bne.s	.exit
		lea	(.ilbmfile,pc),a0
		move.l	d0,(a0)

		*/ Décompacte l'image ILBM-IFF /*
		move.l	(.ilbmfile,pc),d0	adresse du fichier IFF-ILBM
		move.l	(.screen,pc),a0		adresse structure "SCREEN"
		bsr	display.ilbm		|-> affiche l'image
		bne.s	.exit

		*/ Libère la mémoire de l'image IFF-ILBM /*
		move.l	(.ilbmfile,pc),d0
		bsr	libere_memoire		
		move.l	(.screen,pc),a0
		moveq	#0,d7
.exit		rts

		*/ Adresses utilisées /* 
		CNOP	0,4
.ppfile:	dc.l	0	pt/fichier PP
.ppsize:	dc.l	0	taille du fichier PP
.ilbmfile:	dc.l	0	pt/fichier IFF-ILBM
.screen		dc.l	0	pt/strcuture "SCREEN"
