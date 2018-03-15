package kmeans

object Lloyd {
    def main(args: Array[String]) {
    val start = System.currentTimeMillis
    val L = new Lloyd
    val S = L.Clustering(L.InitialT)//S(j), the list, includes all the element in cluster C(j)
    val end = System.currentTimeMillis()
    println(end - start)
  }
}

class Lloyd extends both {
  val InitialT = Array.range(0, ClusterNum).map(t => x(t))
  def Clustering(prevT: Array[Array[Double]]): Array[List[Int]] = {
    val S = Recluster(prevT)
    val T = CalcRepresent(S)
    var i = 0
    for (t <- prevT.zip(T);
         ts <- t._1.zip(t._2)
         if ts._1 != ts._2) {
      i = i+1; 
    }
    if (i > 0) {println(i/DimNum); Clustering(T);} else S
  }
  def Recluster(T: Array[Array[Double]]): Array[List[Int]] = {
    val S = Array.fill[List[Int]](ClusterNum)(List()); //initialize S
    for (i <- 0 until DataNum) {
      var min = -1;
      var minVal = 10000000000000000000.0
      for (j <- 0 until ClusterNum) {
        val distance = Distance(x(i), T(j))
        //println(distance)
        if (minVal > distance) {
          minVal = distance
          min = j
        }
      }
      S(min) = i :: S(min)
    }
    S
  }
  def CalcRepresent(S: Array[List[Int]]): Array[Array[Double]] = {
    val T = for {
      s <- S
      val tmp = (Array.fill(DimNum)(0.0) /: s.map(y => x(y))) ((a, b) => a.zip(b).map(c => c._1 + c._2))
    } yield tmp.map(a => a/s.length)
    T
  }
}