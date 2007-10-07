;=========================================================================;
*---------------------* routine qui enlève une vie *----------------------*
;=========================================================================;
; entrée	=> a5.l	adr. table 't'
lune:
		*- perte d'une vie : demande animation -*
		IFEQ	A_TAB
		move.w	#TETE_PARASITE1,(head.req-t,a5)
		move.b	#1,(sound+S_ENLEVE.VIE-t,a5)
		ENDC

		* perte d'une vie : test si dernière vie *
		tst.l	(life0-t,a5)		vies à zéro ?
		beq.s	.soph3			-> oui
		cmp.w	#1,(life0-t,a5)		unité = 1 ?
		bne.s	.lezzon2		-> non
		tst.w	(life1-t,a5)		dizaine = 0 ; dernière vie ?
		bne.s	.lezzon2		-> non

		*- perte d'une vie : plus de vies => GameOver -*
.soph2		moveq	#0,d0
		move.w	d0,(life0-t,a5)
		move.w	d0,(life1-t,a5)
.soph3		move.w	#1,(gameover-t,a5)	flag joueur game-over
		bra.s	.lezzon4

		*- perte d'une vie : enlève une vie  -*
.lezzon2	cmp.w	#9,(life0-t,a5)		unité > 9 ?
		bhi.s	.soph2			-> oui, ya eu triche
		tst.w	(life0-t,a5)		unité = 0  ?
		bne.s	.lezzon3		-> non
		move.w	#10,(life0-t,a5)	unité = 10
		cmp.w	#9,(life1-t,a5)		dizaine > 9 ? 
		bhi.s	.soph2			-> oui, ya eu triche
		subq.w	#1,(life1-t,a5)		dizaine = dizaine - 1
.lezzon3	subq.w	#1,(life0-t,a5)		unité = unité - 1
.lezzon4	rts
