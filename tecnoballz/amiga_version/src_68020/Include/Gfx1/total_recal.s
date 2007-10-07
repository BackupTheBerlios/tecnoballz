*=========================================================================*
*-------------------------->Gestion de la tête<---------------------------*
*=========================================================================*
* entrée	=> a5.l	adr. table 't'
total_recal:

		* Affiche la tête au blitter *
		move.l	(head.adr-t,a5),a0
		add.w	(head.run-t,a5),a0		
		moveq	#-1,d1
		move.l	(ecran_travail-t,a5),d6
		add.l	(head.ecr-t,a5),d6
		WAITBLITTER
		move.l	#$09F00000,(BLTCON0,a6)
		move.l	(head.mod-t,a5),(BLTAMOD,a6)
		move.l	d1,(BLTAFWM,a6)
		move.l	a0,(BLTAPTH,a6)
		move.l	d6,(BLTDPTH,a6)
		move.w	(head.win-t,a5),(BLTSIZE,a6)

		* tete : test si le temps avant prochaine anim. est écoulé *
		lea	(.head_end,pc),a3
		lea	(head.count-t,a5),a0	pt/compt. temps
		tst.w	(a0)			tempo écoulée ?
		beq.s	.head_go		-> oui
		subq.w	#1,(a0)			déc. le compt. temps
		jmp	(a3)			-> sort de la routine

		* tete : prépare anim. démandée *
.head_go	lea	(head.step-t,a5),a1	pt/inc. pt. anim.
		lea	(head.run-t,a5),a4
		move.w	(head.req-t,a5),d0	n° anim demandée
		clr.w	(head.req-t,a5)
		tst.w	(a4)			anim en cours ?
		bne.s	.head_oqp		-> oui
		move.w	d0,(a4)			1er pt image de l'anim
		move.w	#4,(a1)			valeur incrément
		move.w	#TETE_SPEED,(a0)
		jmp	(a3)			-> sort, de la routine

		* tete : test si dernère image de l'anim. démandée *
.head_oqp	move.w	(a4),d0			lit pt. d'animation actuel
		tst.w	(a1)			anim inversée ?
		bmi.s	.head_back		-> oui
		moveq	#TETE_SPEED2,d2
		moveq	#-4,d1
		cmp.w	#TETE_PARASITE2,d0	dernière image parasites ?
		bne.s	.head_oqp2		-> non
		move.w	d1,(a1)
		bra.s	.head_anim
.head_oqp2	cmp.w	#TETE_BAILLE2,d0	dernière image du baillement ?
		bne.s	.head_oqp3		-> non
		move.w	d1,(a1)
		move.w	d2,(a0)
		jmp	(a3)			-> sort, routine anim tête
.head_oqp3	cmp.w	#TETE_RIRE2,d0		dernière image du rire ?
		bne.s	.head_anim		-> non
		move.w	d1,(a1)
		move.w	d2,(a0)
		jmp	(a3)			-> sort, routine anim tête

		* tete : test si revenu 1er image de l'anim. démandée *
.head_back	moveq	#0,d1
		cmp.w	#TETE_PARASITE1,d0	1er image parasites ?
		bne.s	.head_oqp4		-> non
		move.w	d1,(a4)
		jmp	(a3)			-> sort, routine anim tête
.head_oqp4	cmp.w	#TETE_BAILLE1,d0	1er image du baillement ?
		bne.s	.head_oqp5		-> non
		move.w	d1,(a4)
		jmp	(a3)			-> sort, routine anim tête
.head_oqp5	cmp.w	#TETE_RIRE1,d0		1er image du rire ?
		bne.s	.head_anim		-> non
		move.w	d1,(a4)
		jmp	(a3)			-> sort, routine anim tête

		* tete : inc. pointeur d'animation de la tête *
.head_anim	move.w	(a1),d0
		add.w	d0,(a4)			inc. pt. d'animation
		move.w	#TETE_SPEED,(a0)
.head_end	rts
