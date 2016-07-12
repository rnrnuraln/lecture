
object Main {
  def main(args: Array[String]) {
    if (args.length == 1) {
      val automaton = new Automaton(args(0))
      val ifEmpty = automaton.emptinessCheck()
      println(ifEmpty)
    } else {
      println ("The number of arguments is wrong! There needs 1 argument, the name of dot file")
    }
  }
}
