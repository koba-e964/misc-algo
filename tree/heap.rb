class Heap
	attr_reader :ary, :size
	def initialize(k)
		raise unless k>=0
		#|ary|=1<<k
		@ary=Array.new((1 << k)-1)
		@size=0
	end
	def insert(val)
		@ary[2*@size]=nil if(@size>=@ary.size) #expand
		@ary[@size]=val
		@size+=1
		heap_up(@size-1)
		self
	end
	def insert_all(col)
		for i in col
			insert(i)
		end
		self
	end
	def swap(i1,i2)
		tmp=@ary[i2]
		@ary[i2]=@ary[i1]
		@ary[i1]=tmp
	end
	def heap_up(ind)
		return if(ind==0)
		child=(ind-1)/2
		if(@ary[child]>@ary[ind])
			swap(child,ind)
			heap_up(child)
		end
	end
	def first()
		raise 'no such element' if(@size<=0)
		result=@ary[0]
		swap(0,@size-1)
		@size-=1
		@ary[@size]=nil #not necessary
		heap_down(0)
		return result
	end
	def heap_down(ind)
		p1=2*ind+1
		p2=2*ind+2
		mini=ind
		mini=p1 if(p1<@size && @ary[mini]>@ary[p1])
		mini=p2 if(p2<@size && @ary[mini]>@ary[p2])
		return if(mini==ind)
		swap(ind,mini)
		heap_down(mini)
	end
	def inspect
		return 'Heap:'+@ary[0...@size].inspect
	end
end
