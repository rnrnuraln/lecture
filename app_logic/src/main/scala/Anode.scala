
class Anode(val name: String) {
  var ifTerminal = false
  var ifStart = false
  var nextAnodes = Map[String, Anode]() //the map that includes the all of the nodes that can be reached from this node by one transition.
  //The key is the words expressing the state of LTL formula, and the value is the distination of the transition.
  var ifSearched = false //the variable that is used for searching algorithm
  var ifReachable = false //if the value is reachable from the start point
  var traceback: Anode = null
  def checkIfReachable {
    if (!ifReachable) {
      ifReachable = true
      for (node <- nextAnodes) node._2.checkIfReachable
    }
  }
  def findSelfPath: Boolean = findPathTo(this)
  def findPathTo(node: Anode): Boolean = findPathToSub(node, nextAnodes.iterator)
  def findPathToSub(aimAnode: Anode, mapIterator: Iterator[(String, Anode)]): Boolean = {
    ifSearched = true
    if (mapIterator.hasNext) {
      val node = mapIterator.next._2
      if (node eq aimAnode) true else {
        if (node.ifSearched) findPathToSub(aimAnode, mapIterator) else {
          node.traceback = this
          node.findPathToSub(aimAnode, node.nextAnodes.iterator)
        }
      }
    } else {
      if (traceback == null) false else {
        traceback.findPathToSub(aimAnode, traceback.nextAnodes.iterator)
      }
    }
  }
}
