*�������������������������������������������������������������������������*
*******************> D�compacte un fichier Powerpack� <********************
*�������������������������������������������������������������������������*
* Entr�e	=> a0.l	Adr. fin fichier compact� 
*		=> a1.l	Adr. d�compactage
*		=> a2.l	Adr. d�but fichier compact� 
* Sortie	<= d7.l	#-28 Fichier non powerpack�
decompacte_pp:
		cmp.l	#'PP20',(a2)
		bne.s	.error
		movem.l	d0-d6/a0-a6,-(a7)
		bsr.s	.decompacte
		movem.l	(a7)+,d0-d6/a0-a6
		moveq	#0,d7
.exit		rts
.error		moveq	#-28,d7			�Fichier non powerpack�
		bra.	.exit
.decompacte	INCBIN	"Mentat/Include/Raw/powerpack.code"
