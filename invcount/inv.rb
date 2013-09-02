def inv_bit(ary) #ary[i]=0 or 1
	n=ary.size
	a0=Array.new(n) # a0[j]=|{i|ary[i]=1,i<j}|
	a0[0]=0
	for i in 1...n
		a0[i]=a0[i-1]+ary[i-1]
	end
	sum=0
	for i in 0...n
		if(ary[i]==0)
			sum+=a0[i]
		end
	end
	return sum
end

include Math
def inv_naive(ary,max) # [0,max)
	n=ary.size
	m=1<<log2(max+1).ceil # guarantees that cnt[1..n] is valid
	ans=0
	cnt=Array.new(m,0) # cnt[...01110000]:ary[0]~ary[i]‚ÅA[....01110000, ....10000000)‚É‘¶İ‚·‚é‚à‚Ì‚ÌŒÂ”
	for i in 0...n
		if i%10000==0
			puts i.to_s+'/'+n.to_s
		end
		v=ary[i] # v is in [1,n]
		c=0
		w=v+1
		while(w<m)
			least=w&(-w)
			c+=cnt[w]
			w+=least
		end
		ans+=c
		if v==0
			cnt[0]+=1
		else
			w=v
			while(w>0)
				least=w&(-w)
				cnt[w]+=1
				w-=least
			end
		end
	end
	return ans
end

def inv_halving(ary,l) #l: size[bit] 0<=ary[i]<2^l
	if(l==0)
		return 0
	end
	if(l<0)
		raise Exception
	end
	least=ary.map{|v|if v<1<<(l-1) then 0 else 1 end}
	sum=inv_bit(least)
	sub0=[]
	sub1=[]
	for v in ary
		if v<1<<(l-1)
			sub0+=[v]
		else
			sub1+=[v-(1<<(l-1))]
		end
	end
	sum+=inv_halving(sub0,l-1)
	sub0=nil
	sum+=inv_halving(sub1,l-1)
	sub1=nil
	return sum
end

def inv_sqrtlog(ary,l)
	n=ary.size
	m=1<<log2(n+1).ceil
	if(l>m)
		big=ary.map{|v|v/(1<<(l-m))}
		sub=Array.new(1<<m).map{|v|[]}
		for v in ary
			sub[v/(1<<(l-m))]=v%(1<<(l-m))
		end
		res+=inv_halving(big,m)
		for s in sub
			res=inv_sqrtlog(s,l-m)
		end
		return res
	end
	return inv_halving(ary,l)
end

def inv_comp(ary,l)
	st0=Time.now
	inv_naive(ary,1<<l)
	en0=Time.now
	print "naive:"+(en0-st0).to_s+'sec'
	st1=Time.now
	inv_halving(ary,l)
	en1=Time.now
	print "halving:"+(en1-st1).to_s+'sec'
	st2=Time.now
	inv_sqrtlog(ary,l)
	en2=Time.now
	print "sqrtlog:"+(en2-st2).to_s+'sec'
end

