*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*
*/»»»»»»»»»»»»»»»»»»»»»»» INITIALISE LE GIGABLITZ ««««««««««««««««««««««««/*
*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*
* Entrée	=> a5.l	Adr. 't'
init_blitz:
		bsr	cree_lutinblitz
		bne.s	.exit
		move.w	#NLINE.BLITZ-1,(blitz.count-t,a5)
		move.w	#TEMPO.BLITZ,(blitz.tempo-t,a5)
		clr.w	(blitz.haut-t,a5)
		moveq	#0,d7
.exit		rts

*/--------------------/ Crée les 7 lutins GigaBlitz /--------------------/*
* Entrée	=> a5.l	Adr. 't'
cree_lutinblitz:
		*/ Décompacte la page graphique des 7 GigaBlitz */
		sub.l	a2,a2
		lea	(blitz.pp-t,a5),a3
		lea	(blitz.pagegfx-t,a5),a4
		bsr	decode.pagepp
		bne	.exit

		*/ Réserve la mémoire des 7 Lutins */
		lea	(blitz.lutin-t,a5),a4
		bsr	reserve.mem
		bne	.exit

		*/ Recopie les 7 Lutins au format Sprite */
		move.l	(blitz.pagegfx-t,a5),a2
		move.w	(PG_SPLAN,a2),d1
		move.w	(PG_SLINE,a2),d2
		lea	(PG_DATA,a2),a2
		lea	(blitz.type-t,a5),a4
		move.l	(blitz.lutin-t,a5),a0
		lea	(blitz.list-t,a5),a3
		moveq	#6,d4			7 GigaBlitz
.looplaloop	move.l	a0,a1			
		move.l	(a4)+,d3		d3=taille liste lutin
		add.l	d3,a1
		move.l	(a4),d0			d0=hauteur du lutin
		subq.w	#1,d0
		move.l	a0,(a3)+		sauve adresse lutin paire
		move.l	a1,(a3)+		sauve adresse lutin impaire
		bsr	copytolutin16.3
		lea	(16,a0),a0
		lea	(16,a1),a1
		add.l	d3,a0
		move.l	(a4)+,d0		d0=hauteur du lutin
		mulu	d2,d0			*taille ligne page graphiqe
		add.l	d0,a2			a2=adresse prochain GigaBlitz
		dbf	d4,.looplaloop

		*/ Calcul coordonnées */
		lea	(blitz.height-t,a5),a2
		lea	(blitz.list-t,a5),a3
		move.l	(coordx-t,a5),d6
		move.l	(coordy-t,a5),d7
		moveq	#6,d4			7 GigaBlitz
.hope		moveq	#-64,d0
		moveq	#0,d1
		move.w	(a2)+,d2
		bsr	calcul_mot_ctrl_aga
		move.l	(a3)+,a0
		move.l	(a3)+,a1
		move.w	d3,(8,a0)
		or.w	#$80,d3
		move.w	d3,(8,a1)
		swap	d3
		move.w	d3,(a0)	
		move.w	d3,(a1)	
		dbf	d4,.hope

		lea	(blitz.list-t,a5),a3
		lea	(24+table_lutins-t,a5),a0	a0=Adr. table des 8 adresses
		move.l	(a3)+,(a0)+
		move.l	(a3)+,(a0)+

		*/ Libère la page graphique */
		lea	(blitz.pagegfx-t,a5),a0
		bsr	libere_mem
.exit		rts
