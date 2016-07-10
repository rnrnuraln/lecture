
class Node(val name: String) {
  var ifTerminal = false
  var ifStart = false
  var nextNodes = Map[String, Node]() //the map that includes the all of the nodes that can be reached from this node by one transition.
  //The key is the words expressing the state of LTL formula, and the value is the distination of the transition.
  var ifSearched = false //the variable that is used for searching algorithm
  var ifReachable = false //if the value is reachable from the start point
  var traceback: Node = new Node("")
  def checkIfReachable {
    if (!ifReachable) {
      ifReachable = true
      for (node <- nextNodes) node._2.checkIfReachable
    }
  }
  def findSelfPath: Boolean = findPathTo(this)
  def findPathTo(node: Node): Boolean = findPathToSub(node, nextNodes.iterator)
  def findPathToSub(aimNode: Node, mapIterator: Iterator[(String, Node)]): Boolean = {
    ifSearched = true
    if (mapIterator.hasNext) {
      val node = mapIterator.next._2
      if (node eq aimNode) true else {
        if (node.ifSearched) findPathToSub(aimNode, mapIterator) else {
	  node.traceback = this
          node.findPathToSub(aimNode, node.nextNodes.iterator)
	}
      }
    } else {
      if (traceback.name == "") false else {
        traceback.findPathToSub(aimNode, traceback.nextNodes.iterator)
      }
    }
  }
}
