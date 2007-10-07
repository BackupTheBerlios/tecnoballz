*-------------------------------------------------------------------------*

*-------------------------------------------------------------------------*
		SECTION	ETB,CODE
		OPT	C20+		labels et symboles 20 caractères maxi.,  majuscules/minuscules respectées
		OPT	p=68020		assemble les instructions MC68020
		INCDIR	DH0:Data/

*>>>>>>>>>>>>>>>>>>>>>>>>>>* CONSTANTES MENTAT *<<<<<<<<<<<<<<<<<<<<<<<<<<*
NReservation	=50	50 adresses mémoires réservées maximums
FLAG_AMIGADOS	=0	1=assemble routines chargement AmigaDOS
FLAG_BOB32	=0	1=assemble routines BOBs (aff. écran 32 bits)
FLAG_BOBSTRUCT	=0	1=Assemble la structure BOB
FLAG_DMACON.MSK	=0	1=ferme dma bitplane lors d'un accès disque-système
FLAG_EXIT	=0	0=sortie sous système possible avec la touche "ESC"
FLAG_FREQ	=0	1=assemble routine demande fréquence balayage
FLAG_ILBM	=0	1=assemble routines décompactage IFF-ILBM
FLAG_KEY	=1	1=assemble routines de test du clavier
FLAG_LUTIN	=0	1=assemble routines gestion des lutins
FLAG_MEMORY	=0	1=information sur la mémoire possible
FLAG_MOUSE0	=0	1=assemble la routine déplacement souris port 0
FLAG_PP		=0	1=assemble routines décompactage PowerPacker

*>>>>>>>>>>>>>>>>>>>>>>>>>* Debut code MC68020 *<<<<<<<<<<<<<<<<<<<<<<<<<*
debut_prg:	INCLUDE	"Mentat/Include/mentat.s"
msg.pers	dc.b	"                                           "	¤-128
		EVEN

************* routine appelée avant de quitter le multitâche **************
sys_extra:	
		moveq	#0,d7
		rts
sys_extra2:	rts

*>>>>>>>>>>>>>>>>>>>>>>>>>* début du programme *<<<<<<<<<<<<<<<<<<<<<<<<<<*
* Entrée	a5.l	=> adr. 't'
*		a6.l	=> CUSTOM
run:		bsr	first_init
		bsr	sprites_off
		bsr	install_ecran
		bne	sys_erreur

*/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*
*¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤* BOUCLE PRINCIPALE HORS INTERRUPTION *¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤*
*/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*
main_loop:	bsr	wait_vbl
		bsr	desas1
		tst.b	(mouse0_left-t,a5)	Bouton gauche souris ?
		beq.s	main_loop		-> Non
		bra	sys_fin

*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*
******** Routine appelée par l'interruption du temps mort vertical ********
*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*)*(*
* Entrée	a5.l	=> Adr. 't'
*		a6.l	=> CUSTOM
extra_vbl:
		rts

*«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*
*««««««««««««««««««««««| Initialisation d'un écran |»»»»»»»»»»»»»»»»»»»»»»*
*«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*»«*
* Entrée	=> a5.l Adr. table 't'
* Sortie	<= d7.l	#0 Pas d'erreur
install_ecran:
		bsr	screen_reserve		|-> Réserve structure mémoire
		bne.s	.exit			-> erreur
		move.w	#3,(SFMODE_PLANS,a0)	Mode AGA Plans BurstX4
		move.w	#3,(SFMODE_LUTINS,a0)	Mode AGA Lutins BurstX4
		move.w	#640,(SNPIXEL,a0)	Résolution horizontale écran
		move.w	#256,(SNLIGNE,a0)	Résolution verticale écran
		move.b	#1,(SNPLAN,a0)		Nombre de plans binaires (max. 8)
		move.w	#1,(SPBUFFER,a0)	Nombre d'écran (maximum 3)
		move.w	#640,(SPLARGE,a0)	Largeur de la page écran
		move.w	#256,(SPHAUTEUR,a0)	Hauteur de la page écran
		move.b	#1,(SHIRES,a0)		Haute résolution
		lea	(SCOLOR_TABLE,a0),a1
		clr.l	(a1)+			Valeur couleur 0
		move.l	#$CC0201,(a1)+		Valeur couleur 1
		bsr	init_screen		|-> Initialise un écran
		bne.s	.exit			-> erreur
		bsr	convert_aga_color	Format couleur 32 bits -> AGA 
		bsr	colors_on		Init. registres couleurs
		moveq	#0,d7			¤ 0=pas d'erreur
.exit		rts


*------------------------> DESASSEMBLEUR SIMPLE <-------------------------*

desas1		move.l	a6,-(a7)
		lea	(codepc,pc),a6
		lea	(256,a6),a6		Saute le PSP
		moveq	#19,d7

.loop		bsr	read
		moveq	#19,d1
		sub.w	d7,d1
		lsl.w	#3,d1
		moveq	#8,d0
		bsr	printf
		dbf	d7,.loop
	

		move.l	(a7)+,a6
		rts


*/////////////////////////////////////////////////////////////////////////*
*---------------------> Désasemble une instruction <----------------------*
*/////////////////////////////////////////////////////////////////////////*
* Entrée	=> a6.l	pt/le code
read		lea	(.chaine,pc),a0
		move.w	(a6)+,d0		Octet 1
		move.b	d0,d1
		and.w	#$0F,d1			Colonne=Second-chiffre
		and.w	#$F0,d0			Ligne=Premier chiffre
		lsr.w	#2,d0
		move.l	(.list,pc,d0),a1
		move.l	(a1,d1*4),a1
		jsr	(a1)
		lea	(.chaine,pc),a0
		rts
.list		dc.l	.list0
		dc.l	.list1
		dc.l	.list2
		dc.l	.list3
		dc.l	.list4
		dc.l	.list5
		dc.l	.list6
		dc.l	.list7
		dc.l	.list8
		dc.l	.list9
		dc.l	.listA
		dc.l	.listB
		dc.l	.listC
		dc.l	.listD
		dc.l	.listE
		dc.l	.listF
*««««««««««««««««««««««««««««««* LIGNE <0> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.list0		dc.l	.add0
		dc.l	.add1
		dc.l	.add2
		dc.l	.add3
		dc.l	.add4
		dc.l	.add5
		dc.l	.push_es
		dc.l	.pos_es
		dc.l	.or8
		dc.l	.or9
		dc.l	.orA
		dc.l	.orB
		dc.l	.orC
		dc.l	.orD
		dc.l	.push_cs
		dc.l	.twobytes
*««««««««««««««««««««««««««««««* LIGNE <1> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.list1		dc.l	.adc0
		dc.l	.adc1
		dc.l	.adc2
		dc.l	.adc3
		dc.l	.adc4
		dc.l	.adc5
		dc.l	.push_ss
		dc.l	.pos_ss
		dc.l	.sbb8
		dc.l	.sbb9
		dc.l	.sbbA
		dc.l	.sbbB
		dc.l	.sbbC
		dc.l	.sbbD
		dc.l	.push_ds
		dc.l	.pos_ds
*««««««««««««««««««««««««««««««* LIGNE <2> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.list2		dc.l	.and0
		dc.l	.and1
		dc.l	.and2
		dc.l	.and3
		dc.l	.and4
		dc.l	.and5
		dc.l	.seg_es
		dc.l	.daa
		dc.l	.sub8
		dc.l	.sub9
		dc.l	.subA
		dc.l	.subB
		dc.l	.subC
		dc.l	.subD
		dc.l	.seg_cs
		dc.l	.das
*««««««««««««««««««««««««««««««* LIGNE <3> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.list3		dc.l	.xor0
		dc.l	.xor1
		dc.l	.xor2
		dc.l	.xor3
		dc.l	.xor4
		dc.l	.xor5
		dc.l	.seg_ss
		dc.l	.aaa
		dc.l	.cmp8
		dc.l	.cmp9
		dc.l	.cmpA
		dc.l	.cmpB
		dc.l	.cmpC
		dc.l	.cmpD
		dc.l	.seg_ds
		dc.l	.aas
*««««««««««««««««««««««««««««««* LIGNE <4> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.list4		dc.l	.inc0
		dc.l	.inc1
		dc.l	.inc2
		dc.l	.inc3
		dc.l	.inc4
		dc.l	.inc5
		dc.l	.inc6
		dc.l	.inc7
		dc.l	.dec0
		dc.l	.dec1
		dc.l	.dec2
		dc.l	.dec3
		dc.l	.dec4
		dc.l	.dec5
		dc.l	.dec6
		dc.l	.dec7
*««««««««««««««««««««««««««««««* LIGNE <5> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.list5		dc.l	.push0
		dc.l	.push1
		dc.l	.push2
		dc.l	.push3
		dc.l	.push4
		dc.l	.push5
		dc.l	.push6
		dc.l	.push7
		dc.l	.pop0
		dc.l	.pop1
		dc.l	.pop2
		dc.l	.pop3
		dc.l	.pop4
		dc.l	.pop5
		dc.l	.pop6
		dc.l	.pop7
*««««««««««««««««««««««««««««««* LIGNE <6> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.list6		dc.l	.pusha2
		dc.l	.popa2
		dc.l	.bound2
		dc.l	.arpl2
		dc.l	.seg_fs3
		dc.l	.seg_gs3
		dc.l	.taille_operand3
		dc.l	.taille_adresse3
		dc.l	.push
		dc.l	.imul2
		dc.l	.push22
		dc.l	.imul22
		dc.l	.insb2
		dc.l	.insd3		(INSW2)
		dc.l	.outsb2
		dc.l	.outsd3		(OUTSW2)
*««««««««««««««««««««««««««««««* LIGNE <7> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.list7		dc.l	.jo
		dc.l	.jno
		dc.l	.jb		(JNE/JC)
		dc.l	.jnb		(JAE/JNC)
		dc.l	.jz		(JE)
		dc.l	.jnz		(JNE)
		dc.l	.jbe		(JNA)
		dc.l	.jnbe		(JA)
		dc.l	.js
		dc.l	.jns
		dc.l	.jp		(JPE)
		dc.l	.jnp		(JPO)
		dc.l	.jl		(JNGE)
		dc.l	.jnl		(JGE)
		dc.l	.jle		(JNG)
		dc.l	.jnle		(JG)
*««««««««««««««««««««««««««««««* LIGNE <8> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.list8		dc.l	.groupea1
		dc.l	.groupea2
		dc.l	.groupea3
		dc.l	.groupea4
		dc.l	.test1
		dc.l	.test2
		dc.l	.xchg1
		dc.l	.xchg2
		dc.l	.mov1
		dc.l	.mov2
		dc.l	.mov3
		dc.l	.mov4
		dc.l	.mov5
		dc.l	.lea
		dc.l	.mov6
		dc.l	.pop
*««««««««««««««««««««««««««««««* LIGNE <9> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.list9		dc.l	.nop
		dc.l	.xchg_cx
		dc.l	.xchg_dx
		dc.l	.xchg_bx
		dc.l	.xchg_sp
		dc.l	.xchg_bp
		dc.l	.xchg_si
		dc.l	.xchg_di
		dc.l	.cbw_cwde
		dc.l	.cwd_cdq
		dc.l	.call_far
		dc.l	.wait
		dc.l	.pushf
		dc.l	.popf
		dc.l	.sahf
		dc.l	.lahf
*««««««««««««««««««««««««««««««* LIGNE <A> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.listA		dc.l	.movax1
		dc.l	.movax2
		dc.l	.movax3
		dc.l	.movax4
		dc.l	.movsb
		dc.l	.movsw_movsd
		dc.l	.cmpsb
		dc.l	.cmpsw_smpsd
		dc.l	.testal
		dc.l	.testax
		dc.l	.stosb
		dc.l	.stosw_stosd
		dc.l	.loadsb
		dc.l	.loadsw_loadsd
		dc.l	.scasb
		dc.l	.scasw_scasd
*««««««««««««««««««««««««««««««* LIGNE <B> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.listB		dc.l	.moval
		dc.l	.movcl
		dc.l	.movdl
		dc.l	.movbl
		dc.l	.movah
		dc.l	.movch
		dc.l	.movdh
		dc.l	.movbh
		dc.l	.movax
		dc.l	.movcx
		dc.l	.movdx
		dc.l	.movbx
		dc.l	.movsp
		dc.l	.movbp
		dc.l	.movsi
		dc.l	.movdi
*««««««««««««««««««««««««««««««* LIGNE <C> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.listC		dc.l	.groupeb1
		dc.l	.groupeb2
		dc.l	.retn1
		dc.l	.retn2
		dc.l	.les
		dc.l	.lds
		dc.l	.mov.mem8
		dc.l	.mov.mem16
		dc.l	.enter
		dc.l	.leave
		dc.l	.retf1
		dc.l	.retf2
		dc.l	.int3
		dc.l	.int
		dc.l	.into
		dc.l	.iret
*««««««««««««««««««««««««««««««* LIGNE <D> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.listD		dc.l	.groupeb3
		dc.l	.groupeb4
		dc.l	.groupeb5
		dc.l	.groupeb6
		dc.l	.aam
		dc.l	.aad
		dc.l	.stealc
		dc.l	.xlat
		dc.l	.esc1
		dc.l	.esc2
		dc.l	.esc3
		dc.l	.esc4
		dc.l	.esc5
		dc.l	.esc6
		dc.l	.esc7
		dc.l	.esc8
*««««««««««««««««««««««««««««««* LIGNE <E> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.listE		dc.l	.loopne
		dc.l	.loope
		dc.l	.loop
		dc.l	.jcxz
		dc.l	.in1
		dc.l	.in2
		dc.l	.out1
		dc.l	.out2
		dc.l	.call_near
		dc.l	.jmp_near
		dc.l	.jmp_fear
		dc.l	.jmp_short
		dc.l	.in3
		dc.l	.in4
		dc.l	.out3
		dc.l	.out4
*««««««««««««««««««««««««««««««* LIGNE <F> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.listF		dc.l	.lock1
		dc.l	.lock2
		dc.l	.repne
		dc.l	.rep
		dc.l	.hlt
		dc.l	.cmc
		dc.l	.groupec1
		dc.l	.groupec2
		dc.l	.clc
		dc.l	.stc
		dc.l	.cli
		dc.l	.sti
		dc.l	.cld		*
		dc.l	.std
		dc.l	.grouped
		dc.l	.groupee

*««««««««««««««««««««««««««««««* LIGNE <0> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.add0
.add1
.add2
.add3
.add4
.add5
.push_es
.pos_es
.or8
.or9
.orA
.orB
.orC
.orD
.push_cs
.twobytes
*««««««««««««««««««««««««««««««* LIGNE <1> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.adc0
.adc1
.adc2
.adc3
.adc4
.adc5
.push_ss
.pos_ss
.sbb8
.sbb9
.sbbA
.sbbB
.sbbC
.sbbD
.push_ds
.pos_ds
*««««««««««««««««««««««««««««««* LIGNE <2> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.and0
.and1
.and2
.and3
.and4
.and5
.seg_es
.daa
.sub8
.sub9
.subA
.subB
.subC
.subD
.seg_cs
.das
*««««««««««««««««««««««««««««««* LIGNE <3> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.xor0
.xor1
.xor2
.xor3
.xor4
.xor5
.seg_ss
.aaa
.cmp8
.cmp9
.cmpA
.cmpB
.cmpC
.cmpD
.seg_ds
.aas
*««««««««««««««««««««««««««««««* LIGNE <4> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.inc0
.inc1
.inc2
.inc3
.inc4
.inc5
.inc6
.inc7
.dec0
.dec1
.dec2
.dec3
.dec4
.dec5
.dec6
.dec7
*««««««««««««««««««««««««««««««* LIGNE <5> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.push0
.push1
.push2
.push3
.push4
.push5
.push6
.push7
.pop0
.pop1
.pop2
.pop3
.pop4
.pop5
.pop6
.pop7
*««««««««««««««««««««««««««««««* LIGNE <6> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.pusha2
.popa2
.bound2
.arpl2
.seg_fs3
.seg_gs3
.taille_operand3
.taille_adresse3
.push
.imul2
.push22
.imul22
.insb2
.insd3
.outsb2
.outsd3
*««««««««««««««««««««««««««««««* LIGNE <7> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.jo
.jno
.jb
.jnb
.jz
.jnz
.jbe
.jnbe
.js
.jns
.jp
.jnp
.jl
.jnl
.jle
.jnle
*««««««««««««««««««««««««««««««* LIGNE <8> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.groupea1
.groupea2
.groupea3
.groupea4
.test1
.test2
.xchg1
.xchg2		move.l	#$3F3F0000,(a0)
		rts
.mov1		move.l	#'MOV ',(a0)+
		clr.w	(a0)+
		rts
.mov2
.mov3
.mov4
.mov5
.lea
.mov6
.pop
*««««««««««««««««««««««««««««««* LIGNE <9> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.nop
.xchg_cx
.xchg_dx
.xchg_bx
.xchg_sp
.xchg_bp
.xchg_si
.xchg_di
.cbw_cwde
.cwd_cdq
.call_far
.wait
.pushf
.popf
.sahf
.lahf
*««««««««««««««««««««««««««««««* LIGNE <A> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.movax1
.movax2
.movax3
.movax4
.movsb
.movsw_movsd
.cmpsb
.cmpsw_smpsd
.testal
.testax
.stosb
.stosw_stosd
.loadsb
.loadsw_loadsd
.scasb
.scasw_scasd
*««««««««««««««««««««««««««««««* LIGNE <B> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.moval
.movcl
.movdl
.movbl
.movah
.movch
.movdh
.movbh
.movax
.movcx
.movdx
.movbx
.movsp
.movbp
.movsi
.movdi
*««««««««««««««««««««««««««««««* LIGNE <C> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.groupeb1
.groupeb2
.retn1
.retn2
.les
.lds
.mov.mem8
.mov.mem16
.enter
.leave
.retf1
.retf2
.int3
.int
.into
.iret
*««««««««««««««««««««««««««««««* LIGNE <D> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.groupeb3
.groupeb4
.groupeb5
.groupeb6
.aam
.aad
.stealc
.xlat
.esc1
.esc2
.esc3
.esc4
.esc5
.esc6
.esc7
.esc8
*««««««««««««««««««««««««««««««* LIGNE <E> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.loopne
.loope
.loop
.jcxz
.in1
.in2
.out1
.out2
.call_near
.jmp_near
.jmp_fear
.jmp_short
.in3
.in4
.out3
.out4
*««««««««««««««««««««««««««««««* LIGNE <F> *»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»*
.lock1
.lock2
.repne
.rep
.hlt
.cmc
.groupec1
.groupec2
.clc
.stc
.cli
.sti
.cld
.std
.grouped
.groupee	move.l	#$3F3F0000,(a0)
		rts
.chaine		dcb.l	10



*--------------------------*  r/m si mod=00b  *---------------------------*
.rm		dc.l	rm0,rm1,rm2,rm3,rm4,rm5,rm6,rm7
.rm0		dc.b	"DS:[BX+SI]",0
.rm1		dc.b	"DS:[BX+DI]",0
.rm2		dc.b	"SS:[BP+SI]",0
.rm3		dc.b	"SS:[BP+DI]",0
.rm4		dc.b	"DS:[SI]",0
.rm5		dc.b	"DS:[DI]",0
.rm6		dc.b	"DS:[",0,"]",0	(Déplacement 16 Bits)
.rm7		dc.b	"DS:[BX]",0
*-------* r/m si mod=01b (dépl. 8 bits) & mod=10b (dépl. 16 bits) *-------*
.rmb		dc.l	rm0b,rm1b,rm2b,rm3b,rm4b,rm5b,rm6b,rm7b
.rm0b		dc.b	"DS:[BX+SI+",0,"]",0
.rm1b		dc.b	"DS:[BX+DI+",0,"]",0
.rm2b		dc.b	"SS:[BP+SI+",0,"]",0
.rm3b		dc.b	"SS:[BP+DI+",0,"]",0
.rm4b		dc.b	"DS:[SI+",0,"]",0
.rm5b		dc.b	"DS:[DI+",0,"]",0
.rm6b		dc.b	"SS:[BP+",0,"]",0
.rm7b		dc.b	"DS:[BX+",0,"]",0
*-------------------------* rm si mod=11b & w= 0 *------------------------*
.rm_mode11w0	dc.b	"AL"
		dc.b	"CL"
		dc.b	"DL"
		dc.b	"BL"
		dc.b	"AH"
		dc.b	"CH"
		dc.b	"DH"
		dc.b	"BH"
*-------------------------* rm si mod=11b & w= 1 *------------------------*
.rm_mode11w1	dc.b	"AX"
		dc.b	"CX"
		dc.b	"DX"
		dc.b	"BX"
		dc.b	"SP"
		dc.b	"BP"
		dc.b	"SI"
		dc.b	"DI"

		



*>>>>>>>>>>>>>>>>>* Adresses utilisées par le programme *<<<<<<<<<<<<<<<<<*
		INCLUDE	"Mentat/Include/Systeme/t.s"
fonteraw	INCBIN	"Mentat/Include/Raw/fontes.raw"
version:	dc.b	"$VER: PCRESOURCE (05.06.95)",0,0
codepc:		INCBIN	"DH1:Data/Heaven.com"
		CNOP	0,4
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*
fin_prg:	
		END
*<><><><><><><><><><><><><><><><><><>*<><><><><><><><><><><><><><><><><><>*

	9288 1292
	2201 3D56
	1C92 7401
	4001 E90C
	028B E83D


 .------.	.---.---.---.   .----.  .-----.
 |opcode|	|mod|reg|r/m|	|dépl|	|Imméd|
 °------°	°---°---°---°   °----°  °-----°

* Un opcode sur un octet
* Un octet falcultatif de mode d'adressage
* Un champ de déplacement de un ou deux octets
* Un champ d'opérande immédiate falculatif sur un ou deux octets






