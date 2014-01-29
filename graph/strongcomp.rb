#Strong Components of a given graph
require "./graph.rb"

def dfs(n,edges,nums,vis,c,cntref)
	p [c,cntref[0]]
	if(vis[c]) #visited
		return nil
	end
	vis[c]=true
	for i in 0...n
		if edges[c][i]
			dfs(n,edges,nums,vis,i,cntref)
		end
	end
	nums[c]=cntref[0]
	cntref[0]+=1
	return nil
end

def strongComp(n,edges)
	nums=Array.new(n,-1)
	vis=Array.new(n,false)
	cnt=[0]
	for i in 0...n
		if !vis[i]
			dfs(n,edges,nums,vis,i,cnt)
		end
	end
	p nums
	vis=Array.new(n,false)
	res=[] # [Int]
	rem=n
	_,gop=graph(n,[]) #opposite
	for i in 0...n
		for j in 0...n
			gop[j][i]=edges[i][j]
		end
	end
	while true
		maxi=-1
		for i in 0...n
			if (maxi==-1 || nums[maxi]<nums[i]) && !vis[i]
				maxi=i
			end
		end
		if maxi==-1
			break
		end
		p ["maxi",maxi]
		dummy=Array.new(n) # :: Array Int Int
		dfs(n,gop,dummy,vis,maxi,[0])
		set=0
		for i in 0...n
			if dummy[i]
				set|=1<<i
			end
		end
		res+=[set]
	end
	return res
end

