class QM
	@n=0
	@stat=[]
	# n: the number of terms
	# data: an array of map:
	# data[i]=0 ->false 1->true 2->don't care
	def initialize(n,data)
		if(n>=16)
			raise Exception
		end
		@n=n
		@stat=Array.new(1<<n)
		for e in data
			@stat[e[0]]=e[1]
		end
	end
	def solve()
		rem=[]
		on_pat=[]
		mt=[] #å€
		for i in 0...@stat.size()
			v=@stat[i]
			if(v==1)
				rem+=[[i,(1<<@n)-1]]
				on_pat+=[i]
			end
			if(v==2)
				rem+=[[i,(1<<@n)-1]]
			end
		end
		print "rem="
		p rem
		while(rem.size()>0)
			break
		end
	end
end



def QMtest()
	data=(0..15).map{|v|[v,0]}
	[4,8,10,11,12,15].each{|v|data[v]=[v,1]}
	data[9]=[9,2]
	data[14]=[14,2]
	return QM.new(4,data)
end
