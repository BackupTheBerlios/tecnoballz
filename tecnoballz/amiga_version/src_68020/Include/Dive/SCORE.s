;*//*//*//*//*//*//*//*//*// //*//*//*//*//*// //*//*//*//*//*//*//*//*//*;
;//*//*//*//*//*//*//*//*// GESTION/DES SCORES//*//*//*//*//*//*//*//*//*/;
;/*//*//*//*//*//*//*//*// //*//*//*//*//*// //*//*//*//*//*//*//*//*//*/*;
*								 	  *
*--------------------------* Charge les scores *--------------------------*
* Entrée	=> a5.l	Adr. 't'
charge_score:
		lea	(pt_score-t,a5),a4
		bsr.l	reserve.mem
		bne.s	.exit
		move.l	(pt_score-t,a5),d0
		move.l	#SIZE_FSCORE,d1
		lea	(score_name,pc),a0
		bsr.l	load_file
		bne	lamescore
		cmp.l	#SIZE_FSCORE,d1
		bne	lamescore
		bsr.s	autosomme
		move.l	(pt_score-t,a5),a0
		cmp.l	(a0),d6
		bne	lamescore
		bsr	megadrive
.exit		rts

*--------------------------* Sauve les scores *---------------------------*
* Entrée	=> a5.l	Adr. 't'
sauve_score:
		bsr.s	autosomme
		move.l	(pt_score-t,a5),a0
		move.l	d6,(a0)
		move.l	a0,d0
		move.l	#SIZE_FSCORE,d1
		lea	(score_name,pc),a0
		bsr.l	save_file
		rts

*---------------* Calcul somme d'auto-contrôle des scores *---------------*
* Entrée	=> a5.l	Adr. 't'
* Sortie	<= d6.l	Somme d'auto-contôle 
autosomme:
		move.l	(pt_score-t,a5),a0
		lea	(SIZE_FSCORE,a0),a1
		addq.l	#4,a0
		move.w	#SIZE_FSCORE-5,d7
		move.l	#'LOVE',d6
.loop		moveq	#0,d0
		moveq	#0,d1
		move.b	-(a1),d1
		move.b	(a0)+,d0
		mulu	d1,d0
		mulu	d7,d0
		eor.l	d0,d6
		dbf	d7,.loop
		rts

*/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/*
*--------------------* Les Scores ont été lamés !!! *---------------------*
*/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/|\/*
* Entrée	=> a5.l	Adr. 't'
lamescore:
		move.l	(pt_score-t,a5),a0
		clr.l	(a0)+			somme d'auto-ctrl
		move.l	#'ETB8',(a0)+		identificateur
		moveq	#3,d4			4 tableaux (EASY/HARD/MAD/DEAD)
.loop78		move.w	#']'-65,d3
		move.w	#22,(a0)+
		move.w	#8,(a0)+
		lea	(xxgo_te-t,a5),a1
		moveq	#17,d7
.loop		move.b	(a1)+,(a0)+		*/ TECNOBALL TOP TEN */
		dbf	d7,.loop
		move.w	#75,(a0)+
		move.w	#4,(a0)+
		moveq	#23,d7
.loop2		move.b	(a1)+,(a0)+		*/ NAME SCORE AREA LEV */
		dbf	d7,.loop2
		move.l	a1,a2
		moveq	#9,d6
		moveq	#90,d5
.abcdefghijklop	move.w	d5,(a0)+
		move.w	#4,(a0)+
		move.l	a2,a1
		moveq	#5,d7
.loop3789	move.b	(a1)+,(a0)+		*/ LAME[ */
		dbf	d7,.loop3789
		move.b	d3,(-1,a0)
		addq.w	#1,d3
		moveq	#17,d7
.loop3		move.b	(a1)+,(a0)+		*/ 000000 :0 00 */
		dbf	d7,.loop3
		add.w	#10,d5
		dbf	d6,.abcdefghijklop
		move.b	#$FE,(-1,a0)		dernière  ligne
		dbf	d4,.loop78
		rts
score_name	dc.b	"TecnoballZ:TecnoballZ/Raw/cosmicbaby.list",0
		EVEN

		*/ Recopie le meilleur score */
megadrive	move.l	(pt_score-t,a5),a0
		move.w	(hard-t,a5),d0
		mulu	#OFF.SCORE,d0
		lea	(IS+KS+NS+4,a0,d0),a0
		lea	(joueur0-t,a5),a1
		moveq	#6-1,d1
.bestcopy1.loop	move.b	(a0)+,(a1)+
		dbf	d1,.bestcopy1.loop
		addq	#1,a0			saute espace entre nom & score
		moveq	#6-1,d1
.bestcopy2.loop	move.b	(a0)+,(a1)+
		dbf	d1,.bestcopy2.loop
		rts
