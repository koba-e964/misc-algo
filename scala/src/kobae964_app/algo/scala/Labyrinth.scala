package kobae964_app.algo.scala

import scala.collection.mutable.Queue

/**
 * Code from Ari-hon pp.37-39
 */
object Labyrinth {
  /**
   * @param dat the map of labyrinth. S designates the starting position, G the goal, # a wall, and . a passage.
   */
  def solve(dat : Array[String]) : Int= {
    val INF = 0x3fffffff
    val n = dat.length
    val m = dat(0).length
    var sx, sy, gx, gy = 0 : Int
    val dp = Array.fill(n, m)(INF)
    for (i <- 0 until n; j <- 0 until m) {
      dat(i)(j) match {
        case 'S' => sx = i; sy = j
        case 'G' => gx = i; gy = j
        case _   =>
      }
    }
    dp(sx)(sy) = 0
    val que = Queue((sx, sy))
    while (que.nonEmpty) {
      val p = que.dequeue
      if (p == (gx, gy)) {
        que.clear
      } else {
        for ((dx, dy) <- List((1,0), (0,1), (-1,0), (0,-1))) {
          val nx = p._1 + dx
          val ny = p._2 + dy
          if ((0 until n contains nx) 
              && (0 until m contains ny) 
              && dat(nx)(ny) != '#' 
              && dp(nx)(ny) == INF) {
            que enqueue ((nx, ny))
            dp(nx)(ny) = dp(p._1)(p._2) + 1
          }
        }
      }
      
    }
    dp(gx)(gy)
  }
  def main(args : Array[String]) {
    val map = Array(
        "#S######.#",
        "......#..#",
        ".#.##.##.#",
        ".#........",
        "##.##.####",
        "....#....#",
        ".#######.#",
        "....#.....",
        ".####.###.",
        "....#...G#"
      )
    println(solve(map) + " = 22")  
  }
}