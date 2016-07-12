import scala.io.Source
import scala.collection.immutable

class Automaton(fileName: String) {
  val nodes = readFile
  val terminals = nodes.filter(x => x._2.ifTerminal)
  val starts = nodes.filter(x => x._2.ifStart)
  val reachableTerminals = leaveTerminalReachable

  def readFile(): Map[String, Anode] = {
    //here the input file is assumed to be written by dot format;
    //however, we don't consider subgraph or no-direction graph
    var nodes: Map[String, Anode] = Map.empty[String, Anode]
    val source = Source.fromFile(fileName)
    val lines = source.getLines
    var descriptionStarted = false
    for (l <- lines) {
      if (descriptionStarted) {
        if ((l.trim.startsWith("node")) && (l.contains("doublecircle"))) {
          //then there will be the information on the terminal
          val s = l.split(";")
          if (s.length > 1) {
            val aho = s(1).trim.split(" ")
            for (nodeName <- aho) {
              nodes = nodes.updated(nodeName, new Anode(nodeName))
              nodes(nodeName).ifTerminal = true
            }
          }
        } else if (l.contains("->") && (l.trim.split("->")(0).startsWith("start"))) {
          //then this edge indicates the start point
          val edge = l.split("->")
          if (edge.length > 1) {
            val nodeName = edge(1).split("\\[")(0).trim
            nodes.get(nodeName) match {
              case Some( f ) => 
              case None => nodes = nodes.updated(nodeName, new Anode(nodeName))
            }
            nodes(nodeName).ifStart = true
          }
        } else if (l.contains("->")) {
          val edges = l.split("->")
          val nodeName = edges(0).trim 
          nodes.get(nodeName) match {
            case Some( f ) => 
            case None => nodes = nodes.updated(nodeName, new Anode(nodeName))
          }
          if (edges.length != 1) {
	    //行き先のノードについてと、エッジとについての処理
            val lnext = edges(1).split("\\[")
            val nextAnodeName = lnext(0).trim
            val edgeName = lnext(1).split("\"")(1)
            nodes.get(nextAnodeName) match {
              case Some( f ) => 
              case None => nodes = nodes.updated(nextAnodeName, new Anode(nextAnodeName))
            }
            val nextAnode = nodes(nextAnodeName)
            nodes(nodeName).nextAnodes = nodes(nodeName).nextAnodes.updated(edgeName, nextAnode)
          }
        }
      } else if (l.startsWith("digraph")) {
        descriptionStarted = true
      }
    }
    nodes
  }
  def leaveTerminalReachable() = {
    for (nodeName <- starts) {
      nodes(nodeName._1).checkIfReachable
    }
    terminals.filter(x => x._2.ifReachable)
  }

  def emptinessCheck(): Boolean = {
    def emptinessCheckSub(it :Iterator[(String, Anode)]): Boolean = {
      val node = it.next._2
      if (node.findSelfPath) {
        true
      } else if (it.hasNext) {
        nodes.foreach(x => x._2.ifSearched = true)
        emptinessCheckSub(it)
      } else false
    }
    if (reachableTerminals == Map.empty[String, Anode]) false else {
      val iter = reachableTerminals.iterator
      emptinessCheckSub(iter)
    }
  }
}
