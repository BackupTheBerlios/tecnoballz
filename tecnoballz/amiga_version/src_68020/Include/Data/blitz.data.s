*=========================================================================*
*--------------------------> DONNES GIGABLITZ <---------------------------*
*=========================================================================*
		CNOP	0,4
blitz.motctrl	dc.l	0
blitz.pp	dc.l	0,0,"BLZ©",0
blitz.pagegfx	dc.l	0,0,"BLZ!",0
blitz.lutin	dc.l	0,TGB,'BLZs',MEMF_CHIP
blitz.list	ds.l	14
blitz.type	dc.l	TGB1,HGB1
		dc.l	TGB2,HGB2
		dc.l	TGB3,HGB3
		dc.l	TGB4,HGB4
		dc.l	TGB5,HGB5
		dc.l	TGB6,HGB6
		dc.l	TGB7,HGB7
blitz.height	dc.w	HGB1,HGB2,HGB3,HGB4,HGB5,HGB6,HGB7
blitz.x		dc.w	0		 X GigaBlitz
blitz.y		dc.w	0		 Y GigaBlitz	
blitz.count	dc.w	0		 compteur GigaBlitz 26 à 0
blitz.haut	dc.w	0		 hauteur GigaBlitz
blitz.tempo	dc.w	0		 tempo compt GigaBlitz
blitz.out	dc.w	0		 flag Blitz sort de la raquette
blitz.settab	dc.w	0		 pt horz. sur set_table (collisions blizt)
blitz.nbrick	dc.w	0		 nbre de briks à tester /\
blitz.restore	dc.w	0		 pt/ligne lutin à restaurer	
		CNOP	0,4
blitz.sprline	dc.l	0,0,0,0		 ligne gfx du lutin
		dc.l	0,0,0,0
blitz.lutin0	dc.l	0
blitz.lutin1	dc.l	0

