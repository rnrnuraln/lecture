package itemset
import scala.io.Source
import scala.collection.mutable.Map

class ItemData(fileName: String) {
  def constructItemSet(): Set[Int] = {
    var prevSet: Set[Int] = Set()
    for (T <- Transactions) {
      prevSet = prevSet ++ T
    }
    prevSet
  }
  def Denotation(pattern: Set[Int]) = {
    Transactions.filter(T => T.subsetOf(pattern))
  }
  def Frequency(pattern: Set[Int]) = {
    Denotation(pattern).size
  }
  val Transactions = Source.fromFile(fileName).getLines.filter(l => l != "").map({
    line => line.split(" ", 0).map({
      word => word.toInt
    }).toSet
  })
  val Itemset = constructItemSet()
  def this() = {
    this("")
  }
  def ItemSetMining(minimalSupport: Int): Map[Set[Int], Int] =  {
    //return the map that contains the pairs of all pattern and their denotation
    assert(minimalSupport > 0)
    def is_ppc(closure1Iterator: Iterator[Int], closure2Iterator: Iterator[Int], i: Int): Boolean = {
      def is_ppcsub(k: Int): Boolean = {
        val a = closure1Iterator.next()
        val b = closure2Iterator.next()
        if (i <= b) true
        else if (a != b) false
        else if (closure2Iterator.hasNext) {
          is_ppcsub(k+1)
        } else true
      }
      is_ppcsub(1)
    }
    def ComputeClosure(pattern: Set[Int], transactions: Iterator[Set[Int]]): Set[Int] = {
      (Set[Int]() /: transactions.filter(t => pattern.subsetOf(t))) ({
        (xs, x) => (if (xs == Set[Int]()) x else x.intersect(xs))
      })
    }
    def BackTrack(closure: Set[Int], closureTail: Int, transactions: Iterator[Set[Int]]): Map[Set[Int], Int] = {
      val patterns = Map[Set[Int], Int]()
      if (closure.size != 0) patterns += closure -> Frequency(closure)
      for (i <- Itemset.filter(i => i > closureTail);
        if ! (closure.apply(i));
        pattern = closure + i;
        if (Frequency(pattern) >= minimalSupport);
        closure2 = ComputeClosure(pattern, transactions);
        if is_ppc(closure.toIterator, closure2.toIterator, i)) {
          patterns ++ BackTrack(closure2, i, Denotation(pattern))
      }
      patterns
    }
    if (Transactions.size > 0) Map[Set[Int], Int]();
    else {
      val C = ComputeClosure(Set[Int](), Transactions)
      BackTrack(C, 0, Transactions)
    }
  }
}