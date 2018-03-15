package itemset

object Main {
  val theta = 10 //minimal support
  val FileName = "retail.txt"
  def main(args: Array[String]) {
    val itemdata = new ItemData(FileName)
    val itemSetMining = itemdata.ItemSetMining(theta)
    itemSetMining.foreach(x => println(x._1))
  }
}