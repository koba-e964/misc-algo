#2-3 tree 

class TTT #Two-Three Tree
	class Node
		attr_accessor :data
		attr_accessor :label
		def leaf?()
			return @data!=nil && !@data.is_a?(Array)
		end
		def nonleaf?()
			return @data.is_a?(Array)
		end
		def initialize()
			@data=nil
			@label=[]
		end
		def self.leaf(val)
			raise 'array is not illegal:'+val.inspect if val.is_a?(Array)
			inst=Node.new()
			inst.data=val
			return inst
		end
		def self.nonleaf(children)
			inst=Node.new()
			n=children.size()
			inst.data=Array.new(n)
			for i in 0...n
				raise unless children[i].is_a? Node
				inst.data[i]=children[i]
			end
			inst.set_label
			return inst
		end
		def set_label()
			if(data.size()<=1 || data.size()>=4)
				return nil
			end
			n=data.size
			@label=Array.new(n-1)
			for i in 1...n
				@label[i-1]=data[i].get_min()
			end
			return nil
		end
		def get_min()
			raise if empty?
			return @data if leaf?
			min=data[0].get_min
			for i in 1...data.size
				l=data[i].get_min
				if(min>l)
					min=l
				end
			end
			return min
		end
		def self.empty()
			a=Node.new()
			a.data=nil
			return a
		end
		def empty?()
			@data.nil?
		end
		def insert_child(child)
			raise unless nonleaf?
		end
		def remove_child(child)
			raise unless nonleaf?
		end
		def children()
			raise unless nonleaf?
			ary=[]
			for i in 0...n
				raise unless children[i].is_a? Node
				ary[i]=data[i]
			end
			return ary
		end
		def inspect
			return "()" if empty?
			return data.to_s if leaf?
			str="("
			for i in @data
				str+=i.to_s+' '
			end
			return str+')'+@label.inspect
		end
	end
	def initialize(val1,val2)
		@entry=Node.nonleaf([Node.leaf(val1),Node.leaf(val2)])
	end
	def member?(val)
		return member_node(@entry,val)
	end
	def member_node(node,val)
		if(node.empty?)
			return false
		end
		if(node.leaf?)
			return node.data==val
		end
		for child in node.data
			res=member_node(child,val)
			return true if res
		end
		return false
	end
	def insert(val)
		insert_node(@entry,val)
	end
	def insert_node(node,val)
		
	end
end