require "./knap-pp.rb"

def test_knapsack(trial = 4, max=1024)
  for i in 0...trial
    dat = (0...128).map{|v| [rand(max), rand(max)]}
    limit = dat.map{|v| v[0]}.reduce(:+)
    st = Time.now
    opt = knapsack_dp(dat, limit)
    en = Time.now
    p opt[0]
    puts "#{en - st}sec"
    for k in 2..5
      st = Time.now
      app = knapsack_fptas(dat, limit, k)
      en = Time.now
      p [k, app[0], en - st]
    end
  end
end
