def test()
	delta=1e-3
	n=3000
	cur=0.0
	ary=[]
	for i in 0...n
		if i%100==0
			p i
		end
		if(cur>=1.0)
			break
		end
		x=i.to_f*delta
		ary+=[[x,cur]]
		dy=delta*((1-cur**4)**0.5)
		cur+=dy
	end
	ary
end
