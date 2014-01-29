def graph(n,uvs) #Int->[(Int,Int)]->Array (Int,Int) Bool
	ary=Array.new(6)
	for i in 0...n
		ary[i]=Array.new(n)
	end
	for a,b in uvs
		ary[a][b]=true
	end
	return [n,ary]
end
def graph0() # Array (Int,Int) Bool
	return graph(6,[[0,1],[1,2],[0,2],[1,5],[3,0],[3,4],[4,5],[5,0],[5,2]])
end
def graph1() # DAG
	return graph(6,[[0,1],[1,2],[2,5],[3,1],[0,4],[3,4],[4,5],[4,2]])
end
