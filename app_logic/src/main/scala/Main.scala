
object Main {
  def main(args: Array[String]) {
    val automaton = new Automaton("test1.dot")
    val ifEmpty = automaton.emptinessCheck()
    println(ifEmpty)
  }
}
