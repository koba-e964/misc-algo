package kobae964_app.algo.scala

import scala.collection.mutable.ListBuffer


object Knapsack {
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
    val n = appropriateWithIndices.length
    val appropriate = appropriateWithIndices.map(_._1)
    val totalW = appropriate.map(_._1).reduce(_ + _)
    val dp = Array.ofDim[Int](n + 1, totalW + 1)
    for (i <- 0 until n) {
      for (j <- 0 to totalW) {
        val weight = appropriate(i)._1
        val value = appropriate(i)._2
        if (j < weight) {
          dp(i + 1)(j) = dp(i)(j)
        } else {
          dp(i + 1)(j) = dp(i)(j) max (dp(i)(j - weight) + value)
        }
      }
    }
    // backtrace
    var l = new ListBuffer[Int]
    val ans = dp(n).slice(0, maxWeight + 1)
    val maxi = ans.zipWithIndex.maxBy(_._1)._2
    var cur = maxi
    for (i <- n - 1 to 0 by -1) {
      if (dp(i)(cur) != dp(i + 1)(cur)) { // uses goods[i]
        cur -= appropriate(i)._1
        appropriateWithIndices(i)._2 +=: l // converts to original index
      }
    }
    (l.toList, ans(maxi))
  }
  def main(args : Array[String]) {
    println(Knapsack(Array((2,3), (1,2), (3,4), (2,2)), 5) + " == (List(0, 2),7)")
    println(Knapsack(Array((1,2),(3,1),(1000,4)), 4) + " == (List(0, 1),3)")
    println(Knapsack(Array((10000,1999),(1,2),(3,1),(1000,4)), 4) + " == (List(1, 2),3)")
  }
}