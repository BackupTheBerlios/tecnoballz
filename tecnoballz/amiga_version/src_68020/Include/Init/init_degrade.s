*-------------------------------------------------------------------------*
*--------------------* Initialise le dégradé Copper *---------------------*
*-------------------------------------------------------------------------*
* Entrée	=> a5.l Adr. 't'
*		=> a6.l CUSTOM
init_degrade:
		*/ Sélectionne la liste Couleur */
		move.w	(avu-t,a5),d0		n° des couluers à utiliser
		tst.w	(rout-t,a5)		Editeur ?
		bne.s	.nomax10		-> oui
		move.w	(hasard-t,a5),d0
		add.w	(VHPOSR,a6),d0
		add.w	(VPOSR,a6),d0
		and.w	#$F,d0
		cmp.w	#10,d0
		blt.s	.nomax10
		subq.w	#8,d0
.nomax10	mulu	#degas2-degas,d0
		lea	(degas-t,a5,d0),a2	a2 pt/liste couleur 32 bits
		lea	(gaga-t,a5),a3
		move.l	(pt_screen-t,a5),a0
		move.w	(SBPLCON3,a0),d6	sélection bits 4 à 7 RVB
		move.w	(SBPLCON3,a0),d7	
		or.w	#$0200,d7		sélection bits 0 à 3 RVB
		move.l	(degrade_copper-t,a5),a1
		move.l	(4+degrade_copper-t,a5),a4

		*/ Couleur cycle des Tirs */
		move.w	#BPLCON3,(a1)
		move.w	d6,(2,a1)
		move.l	(a1)+,(a4)+
		move.w	#COLOR30,(a1)
		move.w	(a1)+,(a4)+
		move.l	a1,(copper.tir-t,a5)
		move.l	a4,(4+copper.tir-t,a5)
		clr.w	(a1)+
		clr.w	(a4)+
		move.w	#BPLCON3,(a1)
		move.w	d7,(2,a1)
		move.l	(a1)+,(a4)+
		move.w	#COLOR30,(a1)
		clr.w	(2,a1)
		move.l	(a1)+,(a4)+

* - - - - - - Dégradé couleur des 18 lignes pour 17 briques - - - - - - - *
* entrée	=> d6.w valeur BPLCON3 bits couleurs 4 à 7
*		=> d7.w valeur BPLCON3 bits couleurs 0 à 3
		move.w	(SCOORDY,a0),d5
		add.w	#BRIY-BK.HEIGHT-1,d5
		moveq	#TBC-1,d4		nbre de  couleurs
.cj1.loop	move.b	d5,(a1)			coordonnée Y
		move.b	#$01,(1,a1)		coordonnée X
		move.w	#$FFFE,(2,a1)
		move.l	(a1)+,(a4)+
		* RGB 32 bits -> format AGA *
		move.l	(a2)+,d2		d2=$xxRRGGBB
		bsr	aga_color		(sortie d0 & d1)
		move.w	d0,(a3)+
		move.w	d1,(a3)+
		* bits 4 à 7 *
		move.w	#BPLCON3,(a1)
		move.w	d6,(2,a1)
		move.l	(a1)+,(a4)+
		move.w	#COLOR16,(a1)
		clr.w	(2,a1)
		move.l	(a1)+,(a4)+
		* bits 0 à 3 *
		move.w	#BPLCON3,(a1)
		move.w	d7,(2,a1)
		move.l	(a1)+,(a4)+
		move.w	#COLOR16,(a1)
		clr.w	(2,a1)
		move.l	(a1)+,(a4)+
		addq.w	#BK.HEIGHT+1,d5		Brique suivante
		dbf	d4,.cj1.loop

		*/ 27 dégradés couleur 16 du GigaBlitz */
		lea	(blitz_color-t,a5),a2
		lea	(blitz_color.aga-t,a5),a3
		move.l	#$FFE1FFFE,(a1)+	wait($ff,$e1) Blitz > 255
		move.l	#$FFE1FFFE,(a4)+	wait($ff,$e1) Blitz > 255
		move.l	a1,(copper_blitz-t,a5)
		move.l	a4,(4+copper_blitz-t,a5)
		move.w	(SCOORDY,a0),d5
		add.w	#POS.GIGABLITZ,d5
		moveq	#NLINE.BLITZ-1,d4
.cj2.loop	* WAIT (x,y) *	
		move.b	d5,(a1)
		move.b	#$01,(1,a1)
		move.w	#$FFFE,(2,a1)
		move.l	(a1)+,(a4)+
		* RGB 32 bits -> format AGA *
		move.l	(a2)+,d2		d2=$xxRRGGBB
		bsr	aga_color		(sortie d0 & d1)
		move.w	d0,(a3)+
		move.w	d1,(a3)+
		* bits 4 à 7 *
		move.w	#BPLCON3,(a1)
		move.w	d6,(2,a1)
		move.l	(a1)+,(a4)+
		move.w	#COLOR16,(a1)
		clr.w	(2,a1)
		move.l	(a1)+,(a4)+
		* bits 0 à 3 *
		move.w	#BPLCON3,(a1)
		move.w	d7,(2,a1)
		move.l	(a1)+,(a4)+
		move.w	#COLOR16,(a1)
		clr.w	(2,a1)
		move.l	(a1)+,(a4)+
		addq.w	#1,d5
		dbf	d4,.cj2.loop
		move.l	#$FFFFFFFE,(a1)		Fin de la liste Copper 1
		move.l	#$FFFFFFFE,(a4)		Fin de la liste Copper 2
		rts
		rts


	
* $01060000,$01BE0000,$01060000,$01BE0000
* $xxE1FFFE,$01060000,$01A0xxxx,$01060200,$01A0xxxx	20 octets * TBC
* $FFE1FFFE						4 octets 
* $xxE1FFFE,$01060000,$01A0xxxx,$01060200,$01A0xxxx	20 octets * 27


