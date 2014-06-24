module QMInternal
	def bit_count(bits,mask)
		bits&=mask
		cnt=0
		for i in 0...bits.size*8
			cnt+=1 if (bits&(1<<i))!=0
		end
		return cnt
	end
end

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
	include QMInternal
	extend QMInternal
	def solve()
		rem=[] #rem=[[v1,m1], [v2,m2],...], v1:bitmask for true, ~v2:bitmask for don't care
		on_pat=[]
		mt=[] #Žå€, prime implicant
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
			nxtbuf=[] #next rem
			paired=Array.new(rem.size(),false)
			for x in 0...rem.size()-1
				for i in x+1...rem.size()
					next unless rem[x][1]==rem[i][1]
					mask=rem[x][1]
					bc=bit_count(rem[x][0]^rem[i][0], mask)
					if(bc==1)
						paired[x]=paired[i]=true
						new_pair=[rem[x][0]&rem[i][0],mask^rem[x][0]^rem[i][0]]
						nxtbuf<<new_pair if (nxtbuf.index(new_pair)).nil?
					end
				end
			end
			for i in 0...rem.size()
				mt+=[rem[i]] if !paired[i]
			end
			puts "mt="+mt.inspect
			puts "nxtbuf="+nxtbuf.inspect #debug
			rem=nxtbuf
		end
		# on_pat.size() * mt.size()
		tbl=Array.new(on_pat.size()).map{|v|Array.new(mt.size(),0)}
		for i in 0...on_pat.size()
			pat=on_pat[i]
			for j in 0...mt.size()
				set=mt[j]
				if set[0]==(pat&set[1]) # set has an element pat
					tbl[i][j]=1
				end
			end
		end
		puts "tbl="+tbl.inspect
		# patrick's algorithm
		ts = tbl[0].size
		dp = Array.new(1 << ts)
		dp.map!{|v| false}
		dp[0] = true
		for i in 0...tbl.size
			newdp = Array.new(1 << ts, false)
			for j in 0...tbl[i].size
				if tbl[i][j] == 1
					for t in 0 ... 1 << ts
						newdp[t | (1 << j)] |= dp[t]
					end
				end
			end
			dp = newdp
		end
		puts "dp=" + dp.inspect
		p (0...dp.size).map{|k| [k,dp[k]]}.select {|v| v[1]}
		minset = -1
		mask = (1 << ts) - 1
		for i in 0 ... 1 << ts
			if dp[i] && (minset == -1 || (bit_count(i, mask) < bit_count(minset, mask)))
				minset = i
			end
		end
		if minset == -1 then raise "Error: minset == -1" end
		return (0...ts).select{|v|(minset & (1 << v)) != 0}.map{|v| mt[v]}
	end
end



def QMtest()
	data=(0..15).map{|v|[v,0]}
	[4,8,10,11,12,15].each{|v|data[v]=[v,1]}
	data[9]=[9,2]
	data[14]=[14,2]
	return QM.new(4,data)
end
