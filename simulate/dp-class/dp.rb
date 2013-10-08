def dp(time,p1,p2,n=1000)
	ary=Array.new(n).map!{|v|rand(2)}
	for i in 0...time
		ary2=Array.new(n)
		for j in 0...n
			tmp=ary[j]+ary[(j+1)%n]
			ary2[j]=case tmp
			when 2
				parity(p2)
			when 1
				parity(p1)
			else
				0
			end
		end
		ary=ary2
	end
	return ary
end

def parity(pro,v=1000000)
	val=rand(v)
	if(val>=pro * v) then return 0 else return 1 end
end



def test_statistics(time=100,m=20)
	for i in 0..m
		print (i*1.0/m).to_s+': '
		for j in 0..m
			print dp(time,i*1.0/m,j*1.0/m).reduce(:+).to_s+' '
		end
		puts ''
	end
	return nil
end

