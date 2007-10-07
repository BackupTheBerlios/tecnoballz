****************************************************************************
*=========================* Amiga Customs Chips *==========================*
****************************************************************************

*------------------* CIA (Complexe Interface Adapters) *-------------------*

		* CIA-A *
CIAA_PRA	=$bfe001	/FIR1 /FIR0 /RDY /TK0 /WPR0 /CHNG /LED /OVL
CIAA_SDR	=$bfec01	CIAA serial data register (connected keyboard)
CIAA_ICR	=$bfed01	CIAA interrupt control register
CIAA_CRA	=$bfee01	CIAA control register A
CIAA_CRB	=$bfef01	CIAA control register B

		* CIA-B *
CIAB_PRA	=$bfd000	/DTR /RTS /CD /CTS /DSR SEL POUT BUSY
CIAB_PRB	=$bfd100	/MTR /SEL3 /SEL2 /SEL1 /SEL0 /SIDE DIR /STEP
CIAB_TALO	=$bfd400	CIAB timer A low byte
CIAB_TAHI	=$bfd500	CIAB timer A high byte
CIAB_TBLO	=$bfd600	CIAB timer B low byte
CIAB_TBHI	=$bfd700	CIAB timer B high byte
CIAB_ICR	=$bfdd00	CIAB interrupt control register
CIAB_CRA	=$bfde00	CIAB control register A
CIAB_CRB	=$bfdf00	CIAB control register B

*=======================* DENISE, AGNES et PAULA *========================*
CUSTOM		=$DFF000
BLTDDAT		=$000
DMACONR		=$002	R
VPOSR		=$004	R	Position verticale bit de poids fort (bit 0)
VHPOSR		=$006	R	Pos. horiz. (octet bas) et vert. (octet haut)
DSKDATR		=$008
JOY0DAT		=$00A	W
JOY1DAT		=$00C	W
CLXDAT		=$00E
ADKCONR		=$010	R
POT0DAT		=$012
POT1DAT		=$014
POTGOR		=$016	R
SERDATR		=$018
DSKBYTR		=$01A
INTENAR		=$01C	R
INTREQR		=$01E	R
DSKPTH		=$020	W
DSKLEN		=$024	W
DSKDAT		=$026
REFPTR		=$028
VPOSW		=$02A	W
VHPOSW		=$02C
COPCON		=$02E
SERDAT		=$030
SERPER		=$032
POTGO		=$034	W
JOYTEST		=$036
STREQU		=$038
STRVBL		=$03A
STRHOR		=$03C
STRLONG		=$03E

*-------------------------* contrôle du blitter *-------------------------*
BLTCON0		=$040	W
BLTCON1		=$042	W
BLTAFWM		=$044	W
BLTALWM		=$046	W
BLTCPTH		=$048	W
BLTCPTL		=$04A	W
BLTBPTH		=$04C	W
BLTBPTL		=$04E	W
BLTAPTH		=$050	W
BLTAPTL		=$052	W
BLTDPTH		=$054	W
BLTDPTL		=$056	W
BLTSIZE		=$058	W
*		=$05A	
BLTCON0L	=$05B		???	note : byte access only
BLTSIZV		=$05C	W	ECS	largeur fenêtre blitter en mots
BLTSIZH		=$05E	W	ECS	hauteur fenêtre blitter en lignes
BLTCMOD		=$060	W
BLTBMOD		=$062	W
BLTAMOD		=$064	W
BLTDMOD		=$066	W
*		=$068
BLTCDAT		=$070	W
BLTBDAT		=$072	W
BLTADAT		=$074	W
*		=$076
*		=$078
*		=$07A
DENISEID	=$07C	R	???
DSKSYNC		=$07E


*---------------------------* contrôle écran *----------------------------*
COP1LCH		=$080	W
COP2LCH		=$084
COPJMP1		=$088	W
COPJMP2		=$08A	W
COPINS		=$08C
DIWSTRT		=$08e	W
DIWSTOP		=$090	W
DDFSTRT		=$092	W
DDFSTOP		=$094	W
DMACON		=$096	W
CLXCON		=$098
INTENA		=$09A	W
INTREQ		=$09C	W
ADKCON		=$09E	W

*-------------------------* contrôle de l'audio *-------------------------*
AUD0LCH		=$0a0	W
AUD0LCL		=$0a2	W
AUD0LEN		=$0a4	W
AUD0PER		=$0a6	W
AUD0VOL		=$0a8	W
AUD0DAT		=$0aa	W
AUD1LCH		=$0b0	W
AUD1LCL		=$0b2	W
AUD1LEN		=$0b4	W
AUD1PER		=$0b6	W
AUD1VOL		=$0b8	W
AUD1DAT		=$0ba	W
AUD2LCH		=$0c0	W
AUD2LCL		=$0c2	W
AUD2LEN		=$0c4	W
AUD2PER		=$0c6	W
AUD2VOL		=$0c8	W
AUD2DAT		=$0ca	W
AUD3LCH		=$0d0	W
AUD3LCL		=$0d2	W
AUD3LEN		=$0d4	W
AUD3PER		=$0d6	W
AUD3VOL		=$0d8	W
AUD3DAT		=$0da	W

*-----------------------* contrôle des bitplanes *------------------------*
BPL1PTH		=$0e0	W
BPL1PTL		=$0e2	W
BPL2PTH		=$0e4	W
BPL2PTL		=$0e6	W
BPL3PTH		=$0e8	W
BPL3PTL		=$0ea	W
BPL4PTH		=$0ec	W
BPL4PTL		=$0ee	W
BPL5PTH		=$0f0	W
BPL5PTL		=$0f2	W
BPL6PTH		=$0f4	W
BPL6PTL		=$0f6	W
BPL7PTH		=$0f8	W	AGA	pointeur bas 7ieme plan
BPL7PTL		=$0fa	W	AGA	pointeur haut 7ieme plan
BPL8PTH		=$0fc	W	AGA	pointeur bas 8ieme plan
BPL8PTL		=$0fe	W	AGA	pointeur 8ieme plan
BPLCON0		=$100	W
BPLCON1		=$102	W
BPLCON2		=$104	W
BPLCON3		=$106	W	AGA	accès reg. couleurs + taille lutins
BPL1MOD		=$108	W
BPL2MOD		=$10a	W
BPLCON4		=$10C	W	AGA	base palette lutins
CLXCON2		=$10E		???
BPLDAT1		=$110
BPLDAT2		=$112
BPLDAT3		=$114
BPLDAT4		=$116
BPLDAT5		=$118
BPLDAT6		=$11A
BPLDAT7		=$11C		AGA
BPLDAT8		=$11E		AGA

*-------------------------* contrôle des lutins *-------------------------*
SPR0PTH		=$120	W
SPR0PTL		=$122	W
SPR1PTH		=$124	W
SPR1PTL		=$126	W
SPR2PTH		=$128	W
SPR2PTL		=$12a	W
SPR3PTH		=$12c	W
SPR3PTL		=$12e	W
SPR4PTH		=$130	W
SPR4PTL		=$132	W
SPR5PTH		=$134	W
SPR5PTL		=$136	W
SPR6PTH		=$138	W
SPR6PTL		=$13a	W
SPR7PTH		=$13c	W
SPR7PTL		=$13e	W
SPR0POS		=$140	W
SPR0CTL		=$142	W
SPR0DATA	=$144	W
SPR0DATB	=$146	W
SPR1POS		=$148	W
SPR1CTL		=$14a	W
SPR1DATA	=$14c	W
SPR1DATB	=$14e	W
SPR2POS		=$150	W
SPR2CTL		=$152	W
SPR2DATA	=$154	W
SPR2DATB	=$156	W
SPR3POS		=$158	W
SPR3CTL		=$15a	W
SPR3DATA	=$15c	W
SPR3DATB	=$15e	W
SPR4POS		=$160	W
SPR4CTL		=$162	W
SPR4DATA	=$164	W
SPR4DATB	=$166	W
SPR5POS		=$168	W
SPR5CTL		=$16a	W
SPR5DATA	=$16c	W
SPR5DATB	=$16e	W
SPR6POS		=$170	W
SPR6CTL		=$172	W
SPR6DATA	=$174	W
SPR6DATB	=$176	W
SPR7POS		=$178	W
SPR7CTL		=$17a	W
SPR7DATA	=$17c	W
SPR7DATB	=$17e	W

*----------------------* les 32 registres couleurs *----------------------*
COLOR00		=$180	W
COLOR01		=$182	W
COLOR02		=$184	W
COLOR03		=$186	W
COLOR04		=$188	W
COLOR05		=$18a	W
COLOR06		=$18c	W
COLOR07		=$18e	W
COLOR08		=$190	W
COLOR09		=$192	W
COLOR10		=$194	W
COLOR11		=$196	W
COLOR12		=$198	W
COLOR13		=$19a	W
COLOR14		=$19c	W
COLOR15		=$19e	W
COLOR16		=$1a0	W
COLOR17		=$1a2	W
COLOR18		=$1a4	W
COLOR19		=$1a6	W
COLOR20		=$1a8	W
COLOR21		=$1aa	W
COLOR22		=$1ac	W
COLOR23		=$1ae	W
COLOR24		=$1b0	W
COLOR25		=$1b2	W
COLOR26		=$1b4	W
COLOR27		=$1b6	W
COLOR28		=$1b8	W
COLOR29		=$1ba	W
COLOR30		=$1bc	W
COLOR31		=$1be	W

*-------------------------* nouveaux registres *--------------------------*
HTOTAL		=$1C0	W
HSSTOP		=$1C2	W
HBSTRT		=$1C4	W
HBSTOP		=$1C6	W
VTOTAL		=$1C8
VSSTOP		=$1CA	W
VBSTRT		=$1CC
VBSTOP		=$1CE
SPRHSTRT	=$1D0
SPRHSTOP	=$1D2
BPLHSTRT	=$1D4
BPLHSTOP	=$1D6
HHPOSW		=$1D8
HHPOSR		=$1DA
BEAMCON0	=$1DC	W	ECS	$0200 = 50Hz / $0000 = 60Hz
HSSTRT		=$1DE	W
VSSTRT		=$1E0	W
HCENTER		=$1E2	W
DIWHIGH		=$1E4	W		#$2101 dans liste Copper ??
*		=$1E6
*		=$1E8
*		=$1EA
*		=$1EC
*		=$1EE

*		=$1F0
*		=$1F2
*		=$1F4
*		=$1F6
*		=$1F8
*		=$1FA
FMODE		=$1FC	W	AGA	burst $0=1x / $2=2x / $3=4x
*		=$1FE


* AudChannel
AC_PTR		=$00			pointeur sur le début du sample
AC_LEN		=$04			longeur du sample en mots
AC_PER		=$06			période du sample
AC_VOL	    	=$08			volume
AC_DAT	    	=$0A			sample
AC_SIZEOF	=$10


* TRACE DE DROITES * 
OCTANT8		=24
OCTANT7		=4
OCTANT6		=12
OCTANT5		=28
OCTANT4		=20
OCTANT3		=8
OCTANT2		=0
OCTANT1		=16
LINEMODE	=1

