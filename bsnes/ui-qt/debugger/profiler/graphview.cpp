#include "graphview.moc"

// ------------------------------------------------------------------------
GraphView::GraphView()
  : graph(NULL)
  , currentTrigger(NULL)
{
  connect(measurements, SIGNAL(removed(Measurement*)), this, SLOT(onRemovedMeasurement(Measurement*)));

  clear();
}

// ------------------------------------------------------------------------
GraphView::~GraphView() {
  setGraph(NULL);
  setTrigger(NULL);
}

// ------------------------------------------------------------------------
void GraphView::clear() {
  update();
}

// ------------------------------------------------------------------------
void GraphView::onRemovedMeasurement(Measurement *measurement) {
  if (measurement == currentTrigger) {
    setTrigger(NULL);
  }
}

// ------------------------------------------------------------------------
void GraphView::setTrigger(Measurement *measurement) {
  if (currentTrigger != NULL) {
    disconnect(currentTrigger, SIGNAL(triggered(int32_t)), this, SLOT(trigger()));
  }

  currentTrigger = measurement;

  if (currentTrigger != NULL) {
    connect(currentTrigger, SIGNAL(triggered(int32_t)), this, SLOT(trigger()));
  }
}

// ------------------------------------------------------------------------
void GraphView::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.fillRect(event->rect(), QColor(255, 255, 255, 255));

  if (graph == NULL) {
    return;
  }

  int32_t w = width();
  int32_t top = 0;
  int32_t bottom = height();
  int32_t diff = max - min;
  if (diff <= 0){
    diff = 1;
  }
  if (bottom <= top) {
    bottom = top + 1;
  }

  float pixelPerPointY = (float)(bottom - top) / (float)diff;
  float center = (float)top + ((float)max * pixelPerPointY);

  painter.setPen(QColor(192, 192, 192, 255));
  for (int32_t i=graph->lines.size() - 1; i>=0; i--) {
    int32_t y = center - (float)graph->lines[i].offset * pixelPerPointY;
    painter.drawLine(0, y, w, y);
    painter.drawText(2, y - 2, graph->lines[i].name);
  }

  float pixelPerPointX = (float)w / (float)MAX_GRAPH_BUFFER;
  float x = 0;

  for (int32_t j=graph->values.size()-1; j>=0; j--) {
    const GraphValue &gv = graph->values[j];

    uint32_t index = gv.offset;
    bool first = true;
    QPainterPath path;
    for (uint32_t i=0; i<gv.size; i++) {
      float y = center - (gv.buffer[index] * pixelPerPointY);

      if (first) {
        first = false;
        path.moveTo(x, y);
      } else {
        path.lineTo(x, y);
      }

      if (++index == gv.size) {
        index = 0;
      }

      x += pixelPerPointX;
    }

    painter.setPen(QColor(255, 0, 0, 255));
    painter.drawPath(path);
  }
}

// ------------------------------------------------------------------------
void GraphView::setGraph(Graph* graph) {
  this->graph = graph;

  bool first = true;
  min = 0;
  max = 0;
  for (int32_t i=graph->lines.size() - 1; i>=0; i--) {
    if (first) {
      max = min = graph->lines[i].offset;
      first = false;
    } else {
      if (min > graph->lines[i].offset) {
        min = graph->lines[i].offset;
      }
      if (max < graph->lines[i].offset) {
        max = graph->lines[i].offset;
      }
    }
  }

  min = min - 10;
  max = max + 10;

  clear();
}

// ------------------------------------------------------------------------
void GraphView::trigger() {
  if (graph) {
    graph->trigger();
  }

  update();
}

// ------------------------------------------------------------------------
