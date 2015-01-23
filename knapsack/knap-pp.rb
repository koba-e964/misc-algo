# dat: list of [cost, benefit]
# dp with benefit
# return: [result, set]
def knapsack_dp(dat, bagsize)
  n = dat.size()
  total_w = 0
  inf = 1 # inf > dat[0].cost + ... L dat[n-1].cost
  for c, v in dat
    total_w += v
    inf += c
  end
  dp = Array.new(n + 1).map{|v| Array.new(total_w + 1, 0)}
  dp[0][0] = 0
  for j in 1..total_w
    dp[0][j] = inf # inf represents infinity
  end
  for i in 1..n
    for j in 0..total_w
      dp[i][j] = dp[i - 1][j]
      w = dat[i - 1][1]
      s = dat[i - 1][0]
      if w <= j
        dp[i][j] = [dp[i][j], s + dp[i - 1][j - w]].min
      else
        dp[i][j] = [dp[i][j], s].min
      end
    end
  end
  q = 0
  for i in 0..total_w
    if dp[n][i] <= bagsize
      q = i
    end
  end
  set = []
  i = n
  rem = q
  while i >= 0
    w = dat[i - 1][1]
    s = dat[i - 1][0]
    if dp[i][rem] - s == dp[i - 1][rem - w]
      set << (i - 1)
      rem -= w
    end
    i -= 1
  end
  return [q, set.reverse]
end


# (1 + 1/k)-approximation of knapsack
def knapsack_fptas(dat, bagsize, k)
  n = dat.size()
  max_w = dat.map{|v| v[1]}.max
  scale = [1.0, max_w.to_f / (k + 1) / n].max
  dat_new = dat.map{|v, w| [v, (w / scale).to_i]}
  sub = knapsack_dp(dat_new, bagsize)
  [sub[1].map{|v| dat[v][1]}.reduce(:+), sub[1]]
end
