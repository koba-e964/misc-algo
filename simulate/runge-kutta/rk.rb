$ck1=0.2

def accel(t,xxp) #[x,x']->[x',x'']
	return [xxp[1],-$ck1*xxp[1]]
end
def vecadd(a,b) #V^2-> V^2->V^2
	return [a[0]+b[0],a[1]+b[1]]
end
def vecmul(a,b) #V->V^2->V^2
	return [a*b[0],a*b[1]]
end

def solve()
	dat=[]
	xxp=[0.0,3.0]
	width=0.01
	time=(100/width).to_i
	i=0.0
	while i<100
		dat[(i/width).to_i]=xxp
		tk1=accel(i,xxp)
		tk2=accel(i+width/2.0,vecadd(xxp,vecmul(width/2.0,tk1)))
		tk3=accel(i+width/2.0,vecadd(xxp,vecmul(width/2.0,tk2)))
		tk4=accel(i+width,vecadd(xxp,vecmul(width,tk3)))
		slope=vecadd(tk1,vecmul(2.0,tk2))
		slope=vecadd(slope,vecmul(2.0,tk3))
		slope=vecadd(slope,tk4)
		slope=vecmul(1/6.0,slope)
		xxp=vecadd(xxp,vecmul(width,slope))
		i+=width
	end
	return dat
end


