package kobae964_app.algo.scala

object PartitionNumber {
  /**
   * An m-partition of n is a multiset of nonnegative integers whose sum is n and has no more than m elements.
   * This function returns the number of m-partitions of n.
   * Note that permutations are considered equal. For example, 4 = 2+1+1 = 1+2+1 are the same 3-partition of 4.
   * @param n a nonnegative integer\
   * @param m a nonnegative integer
   * @return the number of combinations
   */
  def partition(n : Int, m : Int) : BigInt = {
    if (n < 0 || m < 0) {
      throw new IllegalArgumentException("n < 0 || m < 0")
    }
    val dp = Array.ofDim[BigInt](m + 1, n + 1) // dp(i)(j) : the number of i-partition of j
    dp(0)(0) = 1
    for (i <- 1 to n) dp(0)(i) = 0
    for (i <- 1 to m;
         j <- 0 to n) {
      dp(i)(j) = dp(i - 1)(j) + (if (j >= i) dp(i)(j - i) else 0)
    }
    dp(m)(n)
  }
  private def check(n : Int, m : Int, expected : Int) {
    println("partition(" + n + ", " + m + ") = " + partition(n,m) + ", expected:" + expected)
  }  
  def main(args : Array[String]) {
    check(4, 3, 4)
    check(8, 3, 10)
  }
}