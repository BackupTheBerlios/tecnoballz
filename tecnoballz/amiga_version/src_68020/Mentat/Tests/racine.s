






		move	#800*128,d0

		moveq	#0,d4
		move	d0,d4			d4=a
		lsl.l	#7,d4

		lsr	#1,d0
		add	#64,d0			+ 0.5*128
		
		moveq	#4-1,d5

loop		move	d0,d2			d2=xn
		add	d2,d2			d2=2*xn
		move.l	d4,d3			d3=a
		divs	d2,d3			d3=a/2*xn
		lsr	#1,d0			d1=xn/2
		add	d3,d0			d0=xn+1= xn/2 + a/(2*xn)

		dbf	d5,loop

		rts
















