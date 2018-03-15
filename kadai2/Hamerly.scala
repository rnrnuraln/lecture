package kmeans
import scala.io.Source
//implementation of k-means clustering by Hamerly et.al, 2010

object Hamerly {
  def main(args: Array[String]) {
    val H = new Hamerly
    H.clustering()
  }
}

class Hamerly extends both {
  def clustering() {
    println(DimNum)
    val start = System.currentTimeMillis
    val Cs = Array.fill(ClusterNum)(new Cluster(DataNum))
    val a = Array.fill(DataNum)(0) //index of the center x(i) is assigned
    val u = Array.fill(DataNum)(0.0) //upper bound
    val l = Array.fill(DataNum)(0.0) //lower bound
    initialize(Cs, a, u, l)
    mainloop(Cs, a, u, l)
    println(System.currentTimeMillis - start)
  }
  
  def initialize(Cs: Array[Cluster], a: Array[Int], u: Array[Double], l: Array[Double]) {
    for (i <- 0 until ClusterNum) {
      Cs(i).Center = x(i).map(y => y)
    }
    for (i <- 0 until DataNum) {
      PointAllCenter(Cs, a, u, l, i)
      Cs(a(i)).length = Cs(a(i)).length + 1
      Cs(a(i)).Sum = Cs(a(i)).Sum.zip(x(i)).map(x => x._1 + x._2)
    }
  }
  def mainloop(Cs: Array[Cluster], a: Array[Int], u: Array[Double], l: Array[Double]) {
    for (C <- Cs) {
      var min = 100000000.0
      for (c <- Cs
          if c != C) {
        val distance = Distance(C.Center, c.Center)
        if (min > distance) {
          min = distance
        }
      }
      C.nearest = min
    }
    //Cs.foreach(x => println(x.mvdis))
    for (i <- 0 until DataNum) {
      //println(u(i) + ", " + l(i) + ", " + i)
      val m = 
        if (Cs(a(i)).nearest/2 > l(i)) Cs(a(i)).nearest/2
        else l(i)
      if (u(i) > m) {
        u(i) = Distance(x(i), Cs(a(i)).Center)
        if (u(i) > m) {
          val a2 = a(i)
          PointAllCenter(Cs, a, u, l, i)
          if (a2 != a(i)) {
            Cs(a(i)).length = Cs(a(i)).length + 1
            Cs(a(i)).Sum = Cs(a(i)).Sum.zip(x(i)).map(x => x._1 + x._2)
            Cs(a2).length = Cs(a2).length - 1
            Cs(a2).Sum = Cs(a2).Sum.zip(x(i)).map(x => x._1 - x._2)
          }
        }
      }
    }
    Cs.foreach(x => x.MoveCenter())
    Update(Cs, a, u, l)
    var i = 0
    for (c <- Cs
         if c.mvdis != 0.0) {
      i = i+1; 
    }
    if (i > 0) {println(i); mainloop(Cs, a, u, l);}
  }
  def PointAllCenter(Cs: Array[Cluster], a: Array[Int], u: Array[Double], l: Array[Double], i: Int) {
    var min = -1;
    var minVal = 100000000000.0
    val distance = Array.fill(ClusterNum)(0.0)
    for (j <- 0 until ClusterNum) {
      distance(j) = Distance(x(i), Cs(j).Center)
        if (minVal > distance(j)) {
          minVal = distance(j)
          min = j
        }
    }
    a(i) = min
    u(i) = minVal
    minVal = 100000000.0
    for (j <- 0 until ClusterNum
        if j != a(i)) {
      if (minVal > distance(j)) minVal = distance(j)
    }
    l(i) = minVal
  }
  def Update(Cs: Array[Cluster], a: Array[Int], u: Array[Double], l: Array[Double]) {
    var max = -1;
    var maxVal = -0.6
    val pArray = Cs.map(x => x.mvdis)
    for (i <- 0 until ClusterNum) {
        if (maxVal < pArray(i)) {
          maxVal = pArray(i)
          max = i
        }
    }
    val r = max
    maxVal = -0.5
    for (i <- 0 until ClusterNum
        if i != r) {
      if (maxVal < pArray(i)) {
        maxVal = pArray(i)
        max = i
      }
    }
    val r2 = max
    for (i <- 0 until DataNum) {
      u(i) = u(i) + pArray(a(i))
      l(i) = 
        if (r == a(i)) l(i)-pArray(r2)
        else l(i)-pArray(r)
    }
  }
}

class Cluster(DimNum: Int){
  var Center = Array.fill(DimNum)(0.0)
  var Sum = Array.fill(DimNum)(0.0)
  var length = 0
  var mvdis = 0.0
  var nearest = 0.0
  def MoveCenter() {
    val c2 = Center
    Center = Sum.map(x =>  x / length)
    mvdis = Distance(c2, Center)
  }
  def Distance(a: Array[Double], b: Array[Double]) =  {
    Math.sqrt(a.zip(b).map(x => (x._1 - x._2)*(x._1 - x._2)).sum)
  }
}
