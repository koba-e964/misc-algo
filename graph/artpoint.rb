def art_point(graph)
	n=graph.size
	ary=Array.new(n)
	low=Array.new(n)
	child=Array.new(n)
	for i in 0...n
		child[i]=[]
	end
	$id=0
	dfs(0,graph,ary,low,[],child)
	res=[]
	p ["ary",ary]
	p ["low",low]
	p ["child",child]
	if child[0].size>=2
		res=[0]
		puts "root is an articulation point"
	end
	for i in 1...n
		m=child[i].map{|v|low[v]}.max
		if m!=nil && m>=ary[i]
			res << i
		end
	end
	res
end

def min(x,y)
	if x<y then x else y end
end

def dfs(k,graph,ary,low,path,child)
	n=ary.size
	ary[k]=$id
	$id+=1
	m=ary[k]
	last=child[child.size-1]
	for i in 0...n
		if i==k || i==last
			next
		end
		if graph[k][i]
			if ary[i].nil? # i is not visited yet
				dfs(i,graph,ary,low,path+[k],child)
				child[k]+=[i]
				m=min(m,low[i])
			elsif path.index i #i is in path
				m=min(m,ary[i])
			end
		end
	end
	low[k]=m
	nil
end

def test_graph()
	e=[[1,2],[2,3],[3,6],[1,4],[2,4],[2,5],[4,5],[2,6]]
	ary=Array.new(6)
	for i in 0...6
		ary[i]=Array.new(6,nil)
	end
	for i,j in e
		ary[i-1][j-1]=1
		ary[j-1][i-1]=1
	end
	return ary
end



