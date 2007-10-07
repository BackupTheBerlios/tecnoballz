*»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»*
*******************> Décompacte un fichier Powerpacké <********************
*»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»«»*
* Entrée	=> a0.l	Adr. fin fichier compacté 
*		=> a1.l	Adr. décompactage
*		=> a2.l	Adr. début fichier compacté 
* Sortie	<= d7.l	#-28 Fichier non powerpacké
decompacte_pp:
		cmp.l	#'PP20',(a2)
		bne.s	.error
		movem.l	d0-d6/a0-a6,-(a7)
		bsr.s	.decompacte
		movem.l	(a7)+,d0-d6/a0-a6
		moveq	#0,d7
.exit		rts
.error		moveq	#-28,d7			¤Fichier non powerpacké
		bra.	.exit
.decompacte	INCBIN	"Mentat/Include/Raw/powerpack.code"
