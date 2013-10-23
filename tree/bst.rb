class BST
	class Entry
		attr_accessor :left,:right,:val
		def self.node()
			o=new(0,nil,nil)
			o.set_node()
			return o
		end
		def initialize(val,left,right)
			raise 'val == nil' if val.nil?
			@val=val
			@left=left
			@right=right
		end
		def set_val(val)
			if node?
				@left=Entry::node
				@right=Entry::node
			end
			@val=val
		end
		def set_node()
			@val=nil
			@left=nil
			@right=nil
			nil
		end
		def node? #if self is leaf
			return @val.nil?
		end
		def inspect()
			if node?
				return '()'
			end
			return '('+val.to_s+' '+left.inspect+' '+right.inspect+')'
		end
	end
	attr_reader :ent, :size
	def initialize()
		@ent=Entry.node()
		@size=0
	end
	def insert(val)
		insert_on(val,@ent)
		return self
	end
	def insert_all(col)
		col.each{|v|insert(v)}
		return self
	end
	def insert_on(val,pt)
		if(pt.node?)
			@size+=1
			pt.set_val(val)
			return
		end
		if(val<=pt.val)
			insert_on(val,pt.left)
		else
			insert_on(val,pt.right)
		end
	end
	def place(val)
		cur=@ent
		while 1
			if(cur.node?)
				return nil
			end
			if(val==cur.val)
				return cur
			end
			if(val<=cur.val)
				cur=cur.left
			else
				cur=cur.right
			end
		end
		raise 'unreachable'
	end
	def place_min(pt)
		if(pt.node?)
			raise 'no such node'
		end
		cur=pt
		while 1
			if(cur.left.node?)
				return cur
			end
			cur=cur.left
		end
		raise 'unreachable 76'
	end
	def contains?(val)
		return place(val)!=nil
	end
	def delete(val)
		pt=place(val)
		if(pt.nil?)
			return nil
		end
		delete_on(pt)
	end
	def poll_min()
		return place_min(@ent).val
	end
	def delete_min()
		return delete_on(place_min(@ent))
	end
	def delete_on(pt)
		val=pt.val
		sum=0
		sum+=1 unless(pt.left.node?)
		sum+=1 unless(pt.right.node?)
		@size-=1
		if(sum==0)
			pt.set_node
			return val
		end
		if(sum==1)
			asgn=if pt.left.node? then pt.right else pt.left end
			pt.val=asgn.val
			pt.left=asgn.left
			pt.right=asgn.right
			asgn=nil #let gc works
			return val
		end
		#sum==2
		right_min=place_min(pt.right)
		pt.val=right_min.val
		right_min.set_node
		return val
	end
	def delete_all_sorted()
		ary=Array.new(@size)
		i=0
		for i in 0...ary.size
			ary[i]=delete_min()
		end
		return ary
	end
	def clear()
		@ent=Entry::node()
		@size=0
		return nil
	end
end
module BSTTest
	module_function
	def sort_test(s=1000000, maxv=s)
		t0=Time.now
		ary=(0...s).map{|v|rand(maxv)}
		t1=Time.now
		puts "array(size="+s.to_s+") generation:"+(t1-t0).to_s+"sec"
		t2=Time.now
		bst=BST.new()
		for i in ary
			bst.insert(i)
		end
		t3=Time.now
		puts "insertion:"+(t3-t2).to_s+"sec"
		t4=Time.now
		result=bst.delete_all_sorted()
		t5=Time.now
		puts "deletion:"+(t5-t4).to_s+"sec"
		return nil
	end
end
