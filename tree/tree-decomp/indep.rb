n = 16
rnd = Random.new(0x314159265359)
for test in 0 ... 10
  mat = Array.new(n).map{|v| Array.new(n, false)}
  for i in 0 ... n
    for j in 0 ... n
      if i == j
        next
      end
      mat[i][j] = (rnd.rand(n) <= 1)
      mat[j][i] = mat[i][j]
    end
  end
  for i in 0 ... n
    mat[i] = (0 ... n).select{|v| mat[i][v]}
  end
  filename = "indep/test#{test}.txt"
  fp = open(filename, "w")
  fp.puts(n)
  e = 0
  for i in 0 ... n
    fp.write("#{mat[i].size} ")
    e += mat[i].size
    for v in mat[i]
      fp.write("#{v} ")
    end
    fp.puts("")
  end
  fp.puts("")
  for i in 0 ... n
    fp.puts(rnd.rand(100))
  end
  fp.puts("")
  fp.puts("|V| = #{n}, |E| = #{e}")
  fp.close
end

