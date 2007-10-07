*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
*--------------------------> INITIALISE LE SON <--------------------------*
*«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«*
* Entrée	=> a5.l	Adr. 't'
np_firstinit:
		move.w	#1,(music.on-t,a5)	Interdit PlayRoutine

		*/ Initialise le CIA-B */
		lea	$BFD000,a1
		move.b	#$7F,($D00,a1)		ICR, vire demandes d'irq
		move.b	#$08,($E00,a1)		CRA, one-shot-mode activé (arrête minuterie après chaque décompte et met le bit START à 0)
		move.b	#$80,($400,a1)		TALO
		move.b	#$01,($500,a1)		TAHI
		lea	($D00,a1),a0
		moveq	#0,d0
		moveq	#-1,d1
np_ini3		btst	d0,(a0)
		dbf	d1,np_ini3
		move.b	#$81,(a0)		ICR, autorise irq minuterie A

		*/ Calcul temps d'émission des Samples */
		move.l	(soundtable-t,a5),d4
		lea	(xt-t,a5),a0
		move.w	#(t_end-xt)/16-1,d3	nbre de son
		lea	(sample_rate-2-t,a5),a1
		lea	(np_per1-2,pc),a3
.ct_loop	add.l	d4,(a0)			adresse du sample
		move.w	(4,a0),d2
		add.w	d2,d2			taille sample en octets
		mulu	#60,d2
		move.w	(6,a0),d0		pt/la table des périodes
		move.w	(a3,d0),(6,a0)		valeur de la période
		move.w	(a1,d0),d1		lit sample rate
		divu	d1,d2			d2=taille*60/sample_rate
		move.w	d2,d1
		swap	d1
		tst.w	d1
		beq.s	.ct_ok
		addq.w	#1,d2
.ct_ok		move.w	d2,(8,a0)		Nbre de VBL
		lea	(t2-xt,a0),a0
		dbf	d3,.ct_loop

		*/ Change vecteur d'int. de niveau 6 */
		lea	(np_oldirq-t,a5),a1
		lea	(razure_nous,pc),a0
		move.l	a0,(a1)
		move.l	a0,$78.w

		*/ RAZ Structures */
		lea	(np0-t,a5),a0
		move.l	#NP_SIZE*4,d0
		bsr.l	clear.memory
		move.w	#$8200,(np_dmacon-t,a5)
		lea	(sound-t,a5),a0
		move.l	#KOFFER,d0
		bsr.l	clear.memory

		*/ RAZ volume, interdit dma audio, autorise irq */
		moveq	#0,d1
		move.w	d1,($0A8,a6)
		move.w	d1,($0B8,a6)
		move.w	d1,($0C8,a6)
		move.w	d1,($0D8,a6)
		move.w	#$f,(DMACON,a6)
		move.w	#$E000,($09A,a6)	autorise irq CIA-B
		rts
