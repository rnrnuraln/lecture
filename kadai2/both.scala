package kmeans
import scala.io.Source

trait both {
  //val DataNum = 1000;
  //val DimNum = 2;
  //val ClusterNum = 10;
  def ifheader(s: => String) = {
    if (s.charAt(0) != '!') true else false
  }
  val filename = "GSE28547_series_matrix.txt"
  //val x = Array.fill[Array[Double]](DataNum)(Array.fill[Double](DimNum)(Math.random())) //the random data for clustering  
  val source = Source.fromFile(filename)
  val sf = source.getLines().toSeq
  val prevLabels = source.getLines().find(s => s != "" && ifheader(s))
  assert(prevLabels != None)
  val Labels = prevLabels.get.split(" |\t", 0)
  val DimNum = Labels.length - 1
  val prevx = 
    for {line <- sf
         if line != ""
         if line.charAt(0) != '!'
         splitted = line.split(" |\t")
         if splitted.length > 1
         if splitted(1).length > 0
         if splitted(1).charAt(0) != '"'
         tmp = for {i <- 1 until splitted.length
                 split = splitted(i)
                 afe = if (split == "") 0 else split.toDouble } yield afe
  } yield tmp.toArray
  val DataNum = prevx.length
  val x = Array.ofDim[Double](DataNum, DimNum)
  var i = 0
  val wef = prevx.toArray
  println(wef(0)(0))
  for (l <- prevx) {
    for (j <- 0 until l.length) {
      x(i)(j) = l(j)
    }
    i = i + 1
  }
  println(DataNum)
  println(x(0)(0))
  println(x(1)(1))
  val ClusterNum = 10
  def Distance(a: Array[Double], b: Array[Double]): Double =  {
    Math.sqrt(a.zip(b).map(x => (x._1 - x._2)*(x._1 - x._2)).sum)
  }
}