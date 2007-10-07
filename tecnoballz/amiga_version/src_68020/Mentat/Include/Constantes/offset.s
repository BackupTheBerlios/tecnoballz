*-------------------------> Constantes diverses <-------------------------*
GFXB_AA_ALICE	=$02
gb_ChipRevBits0	=$EC
pr_CLI		=$AC
pr_MsgPort	=$5C
MEMF_CHIP	=$02
MEMF_FAST	=$04
MEMF_CLEAR	=$10000
*--------------------> Offset d'appel librairie exec <--------------------*
Forbid		=-132
Permit		=-138
SuperState	=-150
UserState	=-156
AllocMem	=-198
AllocAbs	=-204
FreeMem		=-210
AvailMem	=-216
FindName	=-276
FindTask	=-294
AddPort		=-354
RemPort		=-360
GetMsg 		=-372
ReplyMsg  	=-378
WaitPort	=-384
CloseLibrary	=-414
OpenDevice	=-444
CloseDevice	=-450
DoIo		=-456
OpenLibrary	=-552
*--------------------> Offset d'appel librairie dos <---------------------*
Open		=-030
Close		=-036
Read		=-042
Write		=-048
Lock		=-084
UnLock		=-090
Examine		=-102
Delay		=-198
*------------------> Offset d'appel librairie intuition <-----------------*
CloseWorkBench	=-078
DisplayAlert	=-090
OpenWorkBench	=-210
WBenchToFront	=-342
WBenchToBack	=-336
*------------------> Offset d'appel librairie graphique <-----------------*
WaitBlit	=-228
OwnBlitter	=-456
DisownBlitter	=-462

*------------------------* Structure InfoBlock *--------------------------*
		RSRESET
INFOBLOCK:	rs	0		(AmigaDos/load_file.s)
DISKKEY		rs.l	1
DIRENTRY	rs.l	1
FILENAME	rs.b	108
PROTECTION	rs.l	1
ENTRYTYPE	rs.l	1
SIZE		rs.l	1		taille du fichier en octet
NUMBLOCKS	rs.l	1
DAYS		rs.l	1
MINUTES		rs.l	1
TICKS		rs.l	1
COMMENT		rs.b	116
IB.SIZE		rs	0		(260 octets)
