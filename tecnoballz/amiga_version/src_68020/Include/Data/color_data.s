		CNOP	0,4
degrade_copper	dc.l	0,0			pt/liste Copper
copper_blitz	dc.l	0,0
copper.tir	dc.l	0,0
*/ Table des 27 couleurs barre t�moin du gigablitz */
blitz_color:	dc.l	$FDFFF0,$FDFEE0,$FCFDD0,$FCFCC0,$FBFBB0,$FBFAA0
		dc.l	$FAF990,$FAF880,$F9F770,$F9F660,$F8F550,$F8F440
		dc.l	$F7F330,$F7F220,$F6F11A,$F6F018,$F5E010,$F5D20A
		dc.l	$F4C308,$F4B407,$F3A506,$F39605,$F28704,$F27803
		dc.l	$F16902,$F15A00,$F04B01
blitz_color.aga	ds.l	NLINE.BLITZ

*/ 10 tables d�grad� de 17 couleurs des briques */
gaga		ds.l	TBC
degas:		dc.l	$400180,$420290,$440392,$500494,$600596,$700698 <1>
		dc.l	$800795,$900893,$A00990,$A20A80,$A40B70,$A60C60
		dc.l	$A80D50,$AA0E40,$AC0F30,$AE1020,$B01110,$B21200

degas2:		dc.l	$014080,$024590,$034A92,$045094,$056096,$067097 <2>
		dc.l	$078096,$089093,$09A090,$0AA180,$0BA270,$0CA360
		dc.l	$0DA450,$0EA540,$0FA630,$10A720,$11A810,$12A900

		dc.l	$408001,$459002,$4A9203,$509404,$609605,$709806 <3>
		dc.l	$809507,$909308,$A19009,$A2800A,$A3700B,$A4600C
		dc.l	$A5500D,$A6400E,$A7300F,$A82010,$A91011,$AA0012

		dc.l	$002080,$102190,$202292,$302394,$402496,$502598 <4>
		dc.l	$602695,$702793,$802890,$782980,$702A90,$602B92
		dc.l	$502C94,$402D96,$302E98,$202F95,$102A93,$002B92

		dc.l	$104080,$204590,$304A92,$405094,$506096,$607098 <5>
		dc.l	$708095,$809093,$90A090,$A0A280,$B0A470,$C0A660
		dc.l	$B0A850,$A0AA40,$90AC30,$80AE20,$70B010,$60B210

		dc.l	$4080B0,$4590A0,$4A9290,$509480,$609670,$709860 <6>
		dc.l	$809550,$909340,$A09030,$A18020,$A27010,$A36020
		dc.l	$A45030,$A54040,$A63050,$A72060,$A81070,$A90080

		dc.l	$101080,$181290,$201492,$281694,$301896,$381A98 <7>
		dc.l	$401C95,$451E93,$4A2080,$502270,$582460,$602650
		dc.l	$702840,$782B30,$802E20,$853010,$8A3205,$903600

		dc.l	$404080,$484890,$504A91,$605092,$706093,$807094 <8>
		dc.l	$908093,$889092,$80A090,$81A280,$82A670,$83A860
		dc.l	$84AA50,$85AC40,$86AE30,$87B020,$88B210,$89B405

		dc.l	$458040,$4A9045,$40924A,$509450,$609655,$70985A <9>
		dc.l	$809562,$909364,$A09066,$A1806A,$A2706C,$A36070
		dc.l	$A45073,$A54076,$A6307A,$A72080,$A81085,$A90590

		dc.l	$407080,$457890,$4A8091,$508892,$609093,$709894 <10>
		dc.l	$80A093,$90A892,$A0B090,$A1B880,$A2C070,$A3B860
		dc.l	$A4B050,$A5A840,$A6A030,$A79820,$A89010,$A98805

*/ Table cycling de couleur des tirs */
		dc.w	0
cycl1:		dc.w	$ff0,$fe0,$ee0,$ed0,$dd0,$dc0,$cc0,$cb0,$bb0,$cb0,$cc0
		dc.w	$dc0,$dd0,$ed0,$ee0,$fe0
cycl2		dc.w	$0f0,$0e0,$0e0,$0d0,$0d0,$0c0,$0c0,$0b0,$0b0,$0b0,$0c0
		dc.w	$0c0,$0d0,$0d0,$0e0,$0e0
