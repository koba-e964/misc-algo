class NFA
  attr_accessor :n, :tbl, :fin
  attr_accessor :state
  # n is the number of vertices. Vertices are labeled with 0 .. n-1.
  # tbl[i] is a hashmap, tbl[i][ch] returns an integer representing a set of vertices that i can reach.
  # The initial state is 0. The final states are given by fin (an integer representing a set).
  def initialize(n, tbl, fin)
    @n = n
    @tbl = tbl.clone
    @fin = fin
    @state = 1
  end
  def run_one(ch)
    next_state = 0
    for i in 0 ... n
      if @state[i] != 0
        next_state |= @tbl[i][ch] || 0
      end
    end
    @state = next_state
    self
  end
  def run(str)
    for ch in str.split('')
      run_one(ch)
    end
    self
  end
  def accept?(str)
    run(str)
    (@state & @fin) != 0
  end
end

class DFA
  attr_accessor :n, :tbl, :fin
  attr_accessor :state
  # n is the number of vertices. Vertices are labeled with 0 .. n-1.
  # tbl[i] is a hashmap, tbl[i][ch] returns an integer representing the next vertex.
  # The initial state is 0 default. The final states are given by fin (an integer representing a set).
  def initialize(n, tbl, fin, init = 0)
    @n = n
    @tbl = tbl.clone
    @fin = fin
    @state = init
  end
  def run_one(ch)
    next_state = 0
    for i in 0 ... n
      if @state[i] != 0
        next_state |= @tbl[i][ch] || 0
      end
    end
    @state = next_state
    self
  end
  def run(str)
    for ch in str.split('')
      run_one(ch)
    end
    self
  end
  def accept?(str)
    run(str)
    (@state & @fin) != 0
  end
end

def nfa_to_dfa(nfa, alpha = [])
  nv = nfa.n
  dv = 1 << nv
  empty = Hash[alpha.map{|a| [a,0]}]
  trans_map = (0 ... dv).map{|set| (0 ... nv).select {|v| set[v] != 0}.map {|v| nfa.tbl[v]}.reduce(empty) {|a, b| a.merge(b) {|k, sv, ov| sv | ov}}
  }
  dfin = (0 ... dv).select {|v| (v & nfa.fin) != 0}
  DFA.new(dv, trans_map, dfin, 1)
end

