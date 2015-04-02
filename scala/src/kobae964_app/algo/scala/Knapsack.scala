package kobae964_app.algo.scala

import scala.collection.mutable.LinkedList
import scala.collection.mutable.ListBuffer


object Knapsack {
  /**
   * Solver of 0-1 knapsack problem
   */
  def apply(goods : Array[(Int, Int)], maxWeight : Int) : (Array[Int], Int) = {
    if (maxWeight < 0) {
      throw new IllegalArgumentException("maxWeight < 0")
    }
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
        appropriateWithIndices(i)._2 +=: l
      }
    }
    (l.toArray, dp(n).slice(0, maxWeight + 1).reduce(_ max _))
  }
}