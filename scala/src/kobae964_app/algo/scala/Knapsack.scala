package kobae964_app.algo.scala

import scala.collection.mutable.ListBuffer


object Knapsack {
  /*
   * Precondition: the weight of each of goods is in [0, maxWeight]
   * */
  private def knapsackSub(goods : Array[(Int, Int)], maxWeight : Int) : (List[Int], Int) = {
    val n = goods.length
    val totalW = goods.map(_._1).reduce(_ + _)
    // dp(i)(j) holds the max{ sum of values in S | S is a subset of [0..i] && sum of weights in S <= j }.
    // In particular, dp(0)(j) = 0 for all j.
    val dp = Array.ofDim[Int](n + 1, totalW + 1)
    for (i <- 0 until n) {
      val weight = goods(i)._1
      val value = goods(i)._2
      for (j <- 0 to totalW) {
        if (j < weight) {
          dp(i + 1)(j) = dp(i)(j)
        } else {
          dp(i + 1)(j) = dp(i)(j) max (dp(i)(j - weight) + value)
        }
      }
    }
    // backtrace
    val l = new ListBuffer[Int]
    val ans = dp(n).slice(0, maxWeight + 1)
    val maxi = ans.zipWithIndex.maxBy(_._1)._2 // the index of maximum elements in ans
    var cur = maxi
    for (i <- n - 1 to 0 by -1) { // loop invariant: forall i. the total weight of goods in optimal set `union` [0..i] is cur
      if (dp(i)(cur) != dp(i + 1)(cur)) { // uses goods(i)
        cur -= goods(i)._1
        i +=: l
      }
    }
    (l.toList, ans(maxi))
  }
  /**
   * Solver of 0-1 knapsack problem
   * @param goods pairs of weights and values
   * @param maxWeight limit of weight of goods in knapsack
   * @return Indices of goods that are used and total value
   */
  def apply(goods : Array[(Int, Int)], maxWeight : Int) : (List[Int], Int) = {
    if (maxWeight < 0) {
      throw new IllegalArgumentException("maxWeight < 0")
    }
    // Excludes goods that are too heavy or have negative weight
    val appropriateWithIndices = goods.zipWithIndex.filter(x => 0 <= x._1._1 && x._1._1 <= maxWeight)
    val appropriate = appropriateWithIndices.map(_._1)
    val (appIndices, sol) = knapsackSub(appropriate, maxWeight) // asks knapsackSub to solve
    (appIndices.map(appropriateWithIndices(_)._2), sol)
  }
  def main(args : Array[String]) {
    println(Knapsack(Array((2,3), (1,2), (3,4), (2,2)), 5) + " == (List(0, 2),7)")
    println(Knapsack(Array((1,2),(3,1),(1000,4)), 4) + " == (List(0, 1),3)")
    println(Knapsack(Array((10000,1999),(1,2),(3,1),(1000,4)), 4) + " == (List(1, 2),3)")
    val ary = Array.tabulate(100)(x => (x, x + 1))
    println(Knapsack(ary, 2500))
  }
}