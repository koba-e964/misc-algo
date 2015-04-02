package kobae964_app.algo.scala

object Fibonacci {
  def fibNaive(n : BigInt) : BigInt = {
    if (n.signum < 0) {
      -1
    } else {
      var cur = BigInt(0)
      var prev = BigInt(1)
      for (_ <- BigInt(0) until n) {
        val tmp = cur + prev
        prev = cur
        cur = tmp
      }
      cur
    }
  }
  def fibBinary(n : BigInt) : BigInt =
  {
    if (n.signum < 0) {
      -1
    } else {
      val size = n.bitLength
      var even = BigInt(0)
      var odd = BigInt(1) //F_(n>>i),F_((n>>i)+1)
      for(i <- size - 1 to 0 by -1)
      {
        val even_sq = even pow 2
        val even_odd = even * odd
        even = (even_odd << 1) - even_sq
        odd = even_sq + (odd pow 2)
        if (n testBit i) {
          val tmp = even + odd
          even = odd
          odd = tmp
        }
      }
      even
    }
  }
}