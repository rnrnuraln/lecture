import breeze.plot._
import breeze.linalg._

object Kadai {
  def main(args: Array[String]) {
    //精度は、1/10刻みから1/100刻みまで考える。
    val N = 50 //反復の回数
    var kadais = for {i <- 10 to N 
      val s = new Solver(i)
      val rNum = s.runCG
      val adap = s.adaptation
      val conv = s.convergency
      val errorNorm = s.calcErrorNorm
    } yield new Kadai(i, rNum, adap, conv, errorNorm)
    kadai1(kadais)
    kadai2_1(kadais)
    kadai2_2(kadais)
    kadai3(kadais)
  }
  def kadai1(k: Seq[Kadai]) {
    val x = DenseVector.zeros[Double](k.length)
    for (i <- 0 until k.length) x(i) = k(i).n.toDouble
    val y = DenseVector.zeros[Double](k.length)
    for (i <- 0 until k.length) y(i) = k(i).retryNum.toDouble
    plotting(x, y, "正方格子の一辺の数", "反復回数", "kadai1.png")
  }
  def kadai2_1(k: Seq[Kadai]) {
    val x = DenseVector.zeros[Double](k.length)
    for (i <- 0 until k.length) x(i) = k(i).n.toDouble
    val y = DenseVector.zeros[Double](k.length)
    for (i <- 0 until k.length) y(i) = Math.log(k(i).adaptation)
    plotting(x, y, "正方格子の一辺の数", "厳密解との残差ノルム", "kadai2.png")
  }
  def kadai2_2(k: Seq[Kadai]) {
    val x = DenseVector.zeros[Double](k.length)
    for (i <- 0 until k.length) x(i) = k(i).n.toDouble
    val y = DenseVector.zeros[Double](k.length)
    for (i <- 0 until k.length) y(i) = Math.log(k(i).convergency)
    plotting(x, y, "正方格子の一辺の数", "厳密解との誤差ノルム", "kadai3.png")
  }
  def kadai3(k: Seq[Kadai]) {
    val x = DenseVector.zeros[Double](k.length)
    for (i <- 0 until k.length) x(i) = k(i).n.toDouble
    val y = DenseVector.zeros[Double](k.length)
    for (i <- 0 until k.length) y(i) = Math.log(k(i).errorNorm)
    plotting(x, y, "正方格子の一辺の数", "CG法自体の誤差ノルム", "kadai4.png")
  }
  def plotting(x: DenseVector[Double], y: DenseVector[Double], xlabel: String, ylabel: String, name: String) {
    val f = Figure()
    val p = f.subplot(0)
    p += plot(x, y, '-')
    p.xlabel = xlabel
    p.ylabel = ylabel
    f.saveas(name)
  }
}

class Kadai(val n: Int, val retryNum: Int, val adaptation: Double, val convergency: Double, val errorNorm: Double) {
  //各精度における検査項目をここに記す
  //Solverはメモリを取るからな……
}
