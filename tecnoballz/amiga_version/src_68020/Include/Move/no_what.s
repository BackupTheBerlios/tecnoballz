;=========================================================================;
******************* Test si envoie capsules de monnaie ********************
;=========================================================================;
* Entrée	=> a2.l pt/liste "BRI"
*		=> a5.l	adr. table 't'
no_what:
		lea	(cap.frek-t,a5),a0
		addq.w	#1,(a0)
		move.w	(monai.frek-t,a5),d0	Fréquence laché de monnaies
		cmp.w	(a0),d0			Lache monnaie ?
		bge	.etb_end		-> non
		clr.w	(a0)
		move.l	(pt_capsule-t,a5),a0
		moveq	#NMONNAIE-1,d7
.capsloop	tst.w	(CAP.ACTIF,a0)		Capsule Monnaie libre ?
		beq.s	.etb_yoyo		-> Oui
		lea	(CAP.SIZE,a0),a0
		dbf	d7,.capsloop
		bra.s	.etb_end		-> oui
.etb_yoyo	move.w	(BRI.X,a2),(CAP.X,a0)	X monnaie
		move.w	(BRI.Y,a2),(CAP.Y,a0)	Y monnaie
		move.w	#1,(CAP.ACTIF,a0)	Flag Monnaie active
		move.l	(BRI.RAK,a2),a1		Raquette où va la monnaie.
		move.l	(BUMP.MONNAIE,a1),(CAP.DEP,a0)
		move.w	(hasard-t,a5),d0
		add.w	d7,d0
		and.w	#$3,d0
		lea	(cap.val-t,a5,d0*4),a1
		move.w	(a1)+,(CAP.VAL,a0)
		move.w	(a1),(CAP.SPEED,a0)
.etb_end	rts
