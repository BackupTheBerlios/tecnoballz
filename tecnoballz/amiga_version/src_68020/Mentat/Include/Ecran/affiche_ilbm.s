*<<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><*
*--------------------* Affiche une image "IFF-ILBM" *---------------------*
*<<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><<>><*
* Entrée	=>	d0.l	adresse image IFF-ILBM
* 		=>	a0.l	pt/structure "SCREEN"
display.ilbm:
		lea	(ai_mem_screen,pc),a1
		move.l	a0,(a1)
		lea	(ai_adr_fichier,pc),a1
		move.l	d0,(a1)
		move.w	(ai_hardscroll_f,pc),(SHARDSCROLL.F,a0)
		move.w	(ai_nnbuffer,pc),(SPBUFFER,a0)
		clr.w	(flag_fade_off-t,a5)
		clr.w	(flag_fade_on-t,a5)
		*bra	sys_fin
		bra	afficheilbm

*-------------------------------------------------------------------------*
******************** AFFICHE UNE IMAGE ILBM A L'ECRAN *********************
*-------------------------------------------------------------------------*
* Entrée	=>	d0.l	Adresse image IFF-ILBM
*		=>	a0.l	structure écran
*		=>	a5.l	Adr. table 't'
*		=>	a6.l	CUSTOM
* Sortie	<=	d0.l	Adresse image IFF-ILBM
*		<=	a0.l	Adresse structure écran
*		<=	d7.l=0
*			ou Z=1	Alors pas d'erreur
affiche_image_ilbm:
		lea	(ai_adr_fichier,pc),a1
		move.l	d0,(a1)
		lea	(ai_mem_screen,pc),a1
		move.l	a0,(a1)
		lea	(ai_hardscroll_f,pc),a1
		move.w	(SHARDSCROLL.F,a0),(a1)
		lea	(ai_nnbuffer,pc),a1
		move.w	(SPBUFFER,a0),(a1)
		bra.s	afficheilbm

*-------------------------------------------------------------------------*
******************** AFFICHE UNE IMAGE ILBM A L'ECRAN *********************
*-------------------------------------------------------------------------*
* Entrée	=>	d0.l	Adresse image IFF-ILBM
*		=>	d1.w	Nombre de buffer(s) 1 2 ou 3
*		=>	d2.w	#1 = Défilement possible horizontal
*		=>	a5.l	Adr. table 't'
*		=>	a6.l	CUSTOM
* Sortie	<=	d0.l	Adresse image IFF-ILBM
*		<=	a0.l	Adresse structure écran
*		<=	d7.l=0
*			ou Z=1	Alors pas d'erreur
affiche_ilbm:
		clr.w	(flag_fade_off-t,a5)
		clr.w	(flag_fade_on-t,a5)
		lea	(ai_hardscroll_f,pc),a0
		move	d2,(a0)
		lea	(ai_nnbuffer,pc),a0
		move	d1,(a0)			sauve nombre de buffer(s)
		lea	(ai_adr_fichier,pc),a0
		move.l	d0,(a0)			sauve adr. du fichier ILBM
		bsr	ai_reserve_mem		|-> réserve structure écran
		bne.s	ai_exit
afficheilbm:	bsr	ai_init_screen
		bne.s	ai_exit
		bsr	init_screen
		bne.s	ai_exit
		bsr	ai_decompacte_ilbm
		bne.s	ai_exit
		bsr	ai_recopy
		bsr	go_fade_on
		move.l	(ai_adr_fichier,pc),d0
		move.l	(ai_mem_screen,pc),a0
		moveq	#0,d7
ai_exit		rts

*---------------* Réserve mémoire pour la structure écran *---------------*
ai_reserve_mem
		bsr	screen_reserve
		lea	(ai_mem_screen,pc),a0
		move.l	d0,(a0)
		tst.l	d7
		rts

*-------------------------- Initialise un écran --------------------------*
* entrée	=> a5.l	adr. table 't'
*		=> a6.l	CUSTOM
ai_init_screen:
		sub.l	a2,a2			a2=0=pas de table couleur
		sub.l	a3,a3			a3=0=pas de décompactage
		move.l	ai_adr_fichier(pc),a4	a4=adr. du fichier ILBM
		bsr	decompacte_ilbm		|-> routine décompactage
		tst.l	d7			erreur ?
		bne	.i_exit			-> oui

		* vérifie que taille écran n'est pas > à celle de la page *
		swap	d5			d5=largeur écran
		cmp	d5,d0			largeur page > largeur écran ?
		bhi.s	.good_width		-> oui
		move	d0,d5			largeur écran = largeur page
.good_width	swap	d5			d5=hauteur écran
		cmp	d5,d1			hauteur page > hauteur écran ?
		bhi.s	.good_high		-> oui
		move	d1,d5			hauteur écran = hauteur page
.good_high	
		* initialise structure écran *
		move.l	(ai_mem_screen,pc),a0	a0=adresse structure écran
		move.w	(ai_hardscroll_f,pc),(SHARDSCROLL.F,a0)
		move.w	(ai_nnbuffer,pc),(SPBUFFER,a0)
		move.w	#3,(SFMODE_PLANS,a0)	mode X4
		move.w	#3,(SFADEOFFSPEED,a0)
		move.w	#3,(SFADEONSPEED,a0)
		move.w	d0,(SPLARGE,a0)		d0=largeur page écran en pixels
		move.w	d1,(SPHAUTEUR,a0)	d1=hauteur page écran en lignes
		move.b	d2,(SNPLAN,a0)		d2=nombre de plans
		swap	d2
		move.b	d2,(SHOMOD,a0)		d2=flag "HAM"
		move.w	d5,(SNLIGNE,a0)		d5=hauteur de l'écran en lignes
		swap	d5
		move.w	d5,(SNPIXEL,a0)		d5=largeur de l'écran en pixels

		* test si écran haute-resolution *
		cmp.w	#MAX_LORES,d5		largeur écran > 368 pixels ?
		bls.s	.nohires		-> non
		move.b	#1,(SHIRES,a0)		oui, écran haute résolution
.nohires
		* test si écran super-haute-resolution *
		cmp.w	#MAX_HIRES,d5		largeur écran > 736 pixels ?
		bls.s	.nosuperhires		-> non
		clr.b	SHIRES(a0)
		move.b	#1,(SUPERHIRES,a0)	oui, écran super haute résolution
.nosuperhires
		* test si écran interlace *
		swap	d5
		cmp.w	#MAX_NOLACE,d5		hauteur écran > 290 lignes ?
		bls.s	.no_interlace		-> non
		move.b	#1,(SLACE,a0)		oui, écran lignes entrelacées
.no_interlace
		* sort de la routine *
.i_exit		tst.l	d7
		rts

*---------------------* Décompacte le fichier ILBM *----------------------*
ai_decompacte_ilbm:
		move.l	(ai_mem_screen,pc),a0
		lea	(SCOLOR_TABLE,a0),a2	a2=adresse table couleur
		move.l	(SPECRAN1,a0),a3	a3=adr. de décompactage
		move.l	(ai_adr_fichier,pc),a4	a4=adr. du fichier ILBM
		bsr	decompacte_ilbm		|-> routine de décompactage
		tst.l	d7
		rts

*-------------------* Recopie dans les autres buffers *-------------------*
ai_recopy:	
		move.w	(ai_nnbuffer,pc),d1
		cmp.w	#3,d1
		beq.s	.to_s2s3
		cmp.w	#2,d1
		beq.s	.to_s2
		rts
.to_s2s3	bra	copy_s1_to_s2s3
.to_s2		bra	copy_s1_to_s2

*-------------------------* Adresses utilisées *--------------------------*
		CNOP	0,4
ai_adr_fichier	dc.l	0			adresse du fichier ILBM
ai_mem_screen	dc.l	0			adresse structure écrab
ai_nnbuffer	dc.w	0			nbre de buffer 1, 2 ou 3
ai_hardscroll_f	dc.w	0
