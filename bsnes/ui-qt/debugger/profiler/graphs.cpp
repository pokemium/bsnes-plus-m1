#include "graphs.moc"
Graphs *graphs;

// ------------------------------------------------------------------------
void GraphValue::trigger() {
  int32_t value = 0;

  if (measurement) {
    value = measurement->currentValue;
  }

  if (size < MAX_GRAPH_BUFFER) {
    buffer[size++] = value;
  } else {
    buffer[offset++] = value;
  }

  if (offset == MAX_GRAPH_BUFFER) {
    offset = 0;
  }
}

// ------------------------------------------------------------------------
void GraphValue::clear() {
  size = 0;
}

// ------------------------------------------------------------------------
Graph::Graph() {
  connect(measurements, SIGNAL(removed(Measurement*)), this, SLOT(onRemovedMeasurement(Measurement*)));
}

// ------------------------------------------------------------------------
Graph::~Graph() {
  disconnect(measurements, SIGNAL(removed(Measurement*)), this, SLOT(onRemovedMeasurement(Measurement*)));
}

// ------------------------------------------------------------------------
void Graph::clear() {
  for (uint32_t i=0; i<values.size(); i++) {
    values[i].clear();
  }
}

// ------------------------------------------------------------------------
void Graph::reset() {
  lines.reset();
  values.reset();
}

// ------------------------------------------------------------------------
void Graph::addLine(const QString &name, int offset) {
  lines.append(GraphLine(name, offset));
}

// ------------------------------------------------------------------------
void Graph::addValue(const GraphValue &value) {
  values.append(value);
}

// ------------------------------------------------------------------------
void Graph::onRemovedMeasurement(Measurement *measurement) {
  for (uint32_t i=0; i<values.size(); i++) {
    if (values[i].measurement == measurement) {
      values[i].measurement = NULL;
    }
  }
}

// ------------------------------------------------------------------------
void Graph::trigger() {
  for (uint32_t i=0; i<values.size(); i++) {
    values[i].trigger();
  }
}


// ------------------------------------------------------------------------
Graphs::Graphs()
  : numGraphs(0)
  , numGraphsAllocated(0)
  , graphs(NULL)
{

}

// ------------------------------------------------------------------------
Graph *Graphs::create() {
  Graph *graph = new Graph();

  if (numGraphsAllocated == 0) {
    numGraphsAllocated = 10;
    numGraphs = 0;
    graphs = (Graph**)malloc(sizeof(Graph*) * numGraphsAllocated);
  } else if (numGraphs == numGraphsAllocated) {
    numGraphsAllocated += 10;
    graphs = (Graph**)realloc(graphs, sizeof(Graph*) * numGraphsAllocated);
  }

  graphs[numGraphs++] = graph;
  emit created(graph);

  return graph;
}

// ------------------------------------------------------------------------
Graph* Graphs::find(const QString &name) {
  for (int32_t i=0; i<numGraphs; i++) {
    if (graphs[i]->name == name) {
      return graphs[i];
    }
  }

  return NULL;
}

// ------------------------------------------------------------------------
