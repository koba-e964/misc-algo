# Calculates lemniscate constant
def calc_lemniscate_constant(delta)
	n=(3.0/delta).to_i
	cur=0.0
        x = 0.0
	for i in 0...n
		if(cur>=1.0)
			break
		end
		x = i.to_f * delta
		dy = delta*((1-cur**4)**0.5)
		cur += dy
	end
	x
end

def test
  step = [1.0, 0.5, 0.2]
  for i in 0..6
    for s in step
      delta = s * 0.1 ** i
      start_time = Time.now
      lem_const = calc_lemniscate_constant(delta)
      end_time = Time.now
      puts "delta = #{delta}, lemConst = #{lem_const}, time = #{end_time - start_time}s"
    end
  end
end
