import breeze.linalg._
import breeze.numerics._

object Solver {
  def main(args: Array[String]) {
    val s = new Solver(10)
    s.runCG()
  }
}

class Solver(N: Int) {
  //解いていくだけの機械
  //Nは分割の格子の個数。全Dirichletとすると、実際の未決定の値の数はN-2となる。
  val n = N-2 //未決定数
  val ipsilon = 1E-12
  val h = 1.0/(N-1) //刻み幅
  val f = createF
  var solution = Array.fill(n*n)(0.0)
  val strictResult = {
    //厳密解
    for {k <- 0 until n*n
      i = (k % n)+1
      j = (k / n)+1
    } yield u(i*h, j*h) 
  }.toArray

  def u(x: Double, y: Double) = Math.pow(x, 4) + Math.pow(y, 4)
  def createF() = {
    val F = Array.fill(n*n)(0.0)
    for (i <- 0 until n) {
      for (j <- 0 until n) { 
        //iとjによって場合分け
	val k = i*n + j
	F(k) = -12*(Math.pow((i+1)*h, 2) + Math.pow((j+1)*h, 2))
	if (j == 0) {
	  F(k) += u((i+1)*h, 0)/(h*h)
	}
	if (j == n-1) {
          F(k) += u((i+1)*h, 1)/(h*h)
	}
        if (i == 0) {
	  F(k) += u(0, (j+1)*h)/(h*h)
	}
	if (i == n-1) {
	  F(k) += u(1, (j+1)*h)/(h*h)
	}
      }
    }
    F
  }
  def multiplyA(b: Array[Double]): Array[Double] = {
    val result = Array.fill(n*n)(0.0)
    for (i <- 0 until n) {
      for (j <- 0 until n) { 
        //iとjによって場合分け
	val k = i*n + j
	result(k) = 4/(h*h)*b(k)
        if (j != 0) {
	  result(k) -= b(k-1)/(h*h)
	}
	if (j != n-1) {
	  result(k) -= b(k+1)/(h*h)
	}
	if (i != 0) {
	  result(k) -= b(k-n)/(h*h)
	}
	if (i != n-1) {
	  result(k) -= b(k+n)/(h*h)
	}
      }
    }
    result
  }

  def runCG() = {
    //CG法を使って、solutionに値を格納する。
    //それと同時に、その反復回数を返す。
    var r = calcResidual 
    var p = r.map(x => x)
    val fNorm = calcNorm(f)
    def runCGloop(i: Int): Int = {
      val alpha = r.map(x=>x*x).sum / calcANorm(p)
      solution = solution.zip(p.map(x => alpha*x)).map(x => x._1 + x._2)
      val prevr = r
      r = r.zip(multiplyA(p).map(x => alpha*x)).map(x => x._1 - x._2)
      val rel_residual = calcNorm(calcResidual)/fNorm
      if (rel_residual < ipsilon) i else {
        val beta = r.map(x => x*x).sum / prevr.map(x => x*x).sum
	p = r.zip(p.map(x => beta*x)).map(x => x._1 + x._2)
	//println("残差: " + rel_residual)
	//println("反復回数: " + i)
        runCGloop(i+1)
      }
    }
    runCGloop(0)
  }

  def adaptation() = calcNorm(f.zip(multiplyA(strictResult)).map(x => x._1 - x._2))
  def convergency() = calcNorm(strictResult.zip(solution).map(x => x._1 - x._2))
  
  def adares() = f.zip(multiplyA(strictResult)).map(x => x._1 - x._2)
  def calcResidual = f.zip(multiplyA(solution)).map(x => x._1 - x._2)
  def calcNorm(x: Array[Double]) = Math.sqrt(x.map(y => y*y).sum)
  def calcANorm(x: Array[Double]) = x.zip(multiplyA(x)).map(y => y._1 * y._2).sum
  def calcErrorNorm = {
    val m = DenseMatrix.zeros[Double](n*n, n*n)
    for (i <- 0 until n) {
      for (j <- 0 until n) {
	val k = i*n + j
	m(k, k) = 4/(h*h)
        if (j != 0) {
	  m(k, k-1) = -1/(h*h)
	}
	if (j != n-1) {
	  m(k, k+1) = -1/(h*h)
	}
	if (i != 0) {
	  m(k, k-n) = -1/(h*h)
	}
	if (i != n-1) {
	  m(k, k+n) = -1/(h*h)
	}
      }
    }
    val f2 = DenseVector.zeros[Double](n*n)
    for (i <- 0 until n*n) f2(i) = f(i)
    val realSol = (m \ f2).toArray
    calcNorm(realSol.zip(solution).map(x => x._1 - x._2))
  }
}
