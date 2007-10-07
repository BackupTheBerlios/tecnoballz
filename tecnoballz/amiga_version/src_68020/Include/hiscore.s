
*/ TECNOBALLZ AGA (pour la V 0.85)
*/ Création d'un fichier contenant 4 tables de scores pour chaque
*/ niveau de difficulté (Easy, Hard, Mad, et Dead)
*/ 1) Corrige la valeurs des caractères
*/ 2) Calcul somme auto-contrôle (évite les tricheurs)
*/ -> Lancer avec MonAm 
*/ -> Sauver le fichier "TECNOBALLZ:TecnoballZ/Data/CosmicBaby
*/ -> Entre les adresses d,f-1

		OPT	P=68020

start:
		*/ Corrige les caractères */
		lea	(d+12,pc),a0
.loopx		move.b	(a0),d0
		cmp.b	#$FD,d0			Fin du fichier ?
		beq.s	.end			-> oui
		cmp.b	#$FF,d0
		beq.s	.endline
		cmp.b	#$FE,d0
		beq.s	.endline
		sub.b	#65,d0
		move.b	d0,(a0)+
		bra.s	.loopx
.endline	addq.l	#5,a0			Saute code fin & Pos. Y,X
		bra.s	.loopx
.end		move.b	#$FE,(a0)

		*/ Calcul la somme d'auto-contrôle */
		lea	(d+4,pc),a0		Début fichier
		lea	(f,pc),a1		Fin fichier
		move.w	#f-d-5,d7		Taille du fichier
		move.l	#'LOVE',d6		Valeur de base
.loop		moveq	#0,d0
		moveq	#0,d1
		move.b	-(a1),d1		Commence par la fin
		move.b	(a0)+,d0		Commence par le début
		mulu	d1,d0
		mulu	d7,d0
		eor.l	d0,d6
		dbf	d7,.loop
		lea	(d,pc),a0
		move.l	d6,(a0)			Sauve somme d'auto-ctrl
li:		rts

d		dc.l	0		Somme d'auto-contrôle
		dc.b	"ETB8"		Identificateur

s1		dc.w	22,8	Position Y,X (lignes,octets)
		dc.b	"EASY[MODE[TOP[TEN",$FF		EASY
		dc.w	75,4
		dc.b	'NAME[[SCORE[[AREA[LEVEL',$FF	$FF=fin de la ligne
		dc.w	090,4
		dc.b	'BRUNO[[\\a\\\[[i`[[[[\]',$FF
		dc.w	100,4
		dc.b	'JEROME[\\``\^[[i`[[[[\]',$FF
		dc.w	110,4
		dc.b	'JMMDS[[\\`^\\[[i^[[[[\^',$FF
		dc.w	120,4
		dc.b	'ZIBIBI[\\`\\\[[i^[[[[\^',$FF
		dc.w	130,4
		dc.b	'REGIS[[\\^^\\[[i][[[[\a',$FF
		dc.w	140,4
		dc.b	'PIERRE[\\^]\\[[i][[[[\b',$FF
		dc.w	150,4
		dc.b	'SOPHIE[\\^\\\[[i][[[[\]',$FF
		dc.w	160,4
		dc.b	'ISA[[[[\\]\\\[[i][[[[\]',$FF
		dc.w	170,4
		dc.b	'DARK[[[\\\]]\[[i][[[[\b',$FF
		dc.w	180,4
		dc.b	'TLK[[[[\\\]\\[[i][[[[\]',$FE	$FE=fin de la table

s2		dc.w	22,8
		dc.b	"HARD[MODE[TOP[TEN",$FF		HARD
		dc.w	75,4
		dc.b	'NAME[[SCORE[[AREA[LEVEL',$FF
		dc.w	090,4
		dc.b	'LETO[[[\\c\c\[[i_[[[[\]',$FF
		dc.w	100,4
		dc.b	'DUNCAN[\\bb\\[[i_[[[[\]',$FF
		dc.w	110,4
		dc.b	'PAUL[[[\\b\\b[[i_[[[[\]',$FF
		dc.w	120,4
		dc.b	'ATREID[\\b\\a[[i_[[[[\]',$FF
		dc.w	130,4
		dc.b	'IDAHO[[\\b\\][[i^[[[[\]',$FF
		dc.w	140,4
		dc.b	'TYRAN[[\\aa\\[[i^[[[[\]',$FF
		dc.w	150,4
		dc.b	'DUNE[[[\\a\\a[[i^[[[[\^',$FF
		dc.w	160,4
		dc.b	'THUFIR[\\a\\\[[i^[[[[\]',$FF
		dc.w	170,4
		dc.b	'MENTAT[\\\aa\[[i][[[[\]',$FF
		dc.w	180,4
		dc.b	'GURNEY[\\\a\\[[i][[[[\a',$FE

s3		dc.w	22,8
		dc.b	"[MAD[MODE[TOP[TEN",$FF		MAD
		dc.w	75,4
		dc.b	'NAME[[SCORE[[AREA[LEVEL',$FF
		dc.w	090,4
		dc.b	'SHAZZ[[\\a\\\[[i_[[[[\a',$FF
		dc.w	100,4
		dc.b	'ASYLUM[\\`\\\[[i^[[[[\a',$FF
		dc.w	110,4
		dc.b	'PACMAN[\\_\\\[[i^[[[[\b',$FF
		dc.w	120,4
		dc.b	'BONZAI[\\^`\\[[i^[[[[\b',$FF
		dc.w	130,4
		dc.b	'KORTEX[\\^^\\[[i^[[[[\a',$FF
		dc.w	140,4
		dc.b	'SCAN[X[\\^]\\[[i^[[[[\^',$FF
		dc.w	150,4
		dc.b	'ATKINS[\\^\\\[[i^[[[[\_',$FF
		dc.w	160,4
		dc.b	'STORM[[\\]\\\[[i^[[[[\`',$FF
		dc.w	170,4
		dc.b	'JFK[[[[\\\]\\[[i^[[[[\`',$FF
		dc.w	180,4
		dc.b	'GLAM[[[\\\\]\[[i^[[[[\]',$FE

s4		dc.w	22,8
		dc.b	"DEAD[MODE[TOP[TEN",$FF		DEAD
		dc.w	75,4
		dc.b	'NAME[[SCORE[[AREA[LEVEL',$FF
		dc.w	090,4
		dc.b	'LEON[[[\\beda[[i^[[[[\]',$FF
		dc.w	100,4
		dc.b	'RENE[[[\\b\\\[[i^[[[[\]',$FF
		dc.w	110,4
		dc.b	'JEAN[[[\\aeee[[i^[[[[\]',$FF
		dc.w	120,4
		dc.b	'MARCEL[\\a\b\[[i^[[[[\`',$FF
		dc.w	130,4
		dc.b	'ROGER[[\\a\a\[[i^[[[[\^',$FF
		dc.w	140,4
		dc.b	'RAOUL[[\\a\\\[[i^[[[[\`',$FF
		dc.w	150,4
		dc.b	'ANDRE[[\\_ccc[[i^[[[[\a',$FF
		dc.w	160,4
		dc.b	'GEORGE[\\_\\\[[i][[[[\_',$FF
		dc.w	170,4	
		dc.b	'ALBERT[\\^\\\[[i][[[[\e',$FF
		dc.w	180,4
		dc.b	'FIRMIN[\\]\\\[[i][[[[\]',$FD
f:
		END

*	ASCII  CODE J
*	'A' => 'A'
*	'B' => 'B'
*	'C' => 'C'
*	'D' => 'D'
*	'E' => 'E'
*	'F' => 'F'
*	'G' => 'G'
*	'H' => 'H'
*	'I' => 'I'
*	'J' => 'J'
*	'K' => 'K'
*	'L' => 'L'
*	'M' => 'M'
*	'N' => 'N'
*	'O' => 'O'
*	'P' => 'P'
*	'Q' => 'Q'
*	'R' => 'R'
*	'S' => 'S'
*	'T' => 'T'
*	'U' => 'U'
*	'V' => 'V'
*	'W' => 'W'
*	'X' => 'X'
*	'Y' => 'Y'
*	'Z' => 'Z'
*	' ' => '['
*	'0' => '\'
*	'1' => ']'
*	'2' => '^'
*	'3' => '_'
*	'4' => '`'
*	'5' => 'a'
*	'6' => 'b'
*	'7' => 'c'
*	'8' => 'd'
*	'9' => 'e'
*	'!' => 'f'
*	'.' => 'g'
*	"'" => 'h'
*	':' => 'i'
*	',' => 'j'
*	'-' => 'k'
