#include "graphview.moc"

// ------------------------------------------------------------------------
GraphView::GraphView()
  : autoMin(true)
  , autoMax(true)
  , autoRedraw(true)
  , currentMeasurement(NULL)
{
  clear();
}

// ------------------------------------------------------------------------
GraphView::~GraphView() {
  setMeasurement(NULL);
}

// ------------------------------------------------------------------------
void GraphView::clear() {
  startValue = 0;
  historyLength = 0;

  if (autoMin) {
    min = 0;
  }

  if (autoMax) {
    max = 0;
  }

  update();
}

// ------------------------------------------------------------------------
void GraphView::paintEvent(QPaintEvent *event) {
  QPainter painter(this);

  painter.fillRect(event->rect(), QColor(255, 255, 255, 255));

  int32_t localMin = min - 10;
  int32_t localMax = max + 10;

  int32_t w = width();
  int32_t top = 10;
  int32_t bottom = height() - 10;
  int32_t diff = localMax - localMin;

  if (diff <= 0){
    diff = 1;
  }
  if (bottom <= top) {
    bottom = top + 1;
  }

  float pixelPerPointY = (float)(bottom - top) / (float)diff;
  float center = (float)top + ((float)localMax * pixelPerPointY);

  painter.setPen(QColor(192, 192, 192, 255));
  painter.drawLine(0, top, w, top);
  painter.drawLine(0, bottom, w, bottom);

  painter.setPen(QColor(224, 224, 224, 255));
  painter.drawLine(0, center, w, center);

  float pixelPerPointX = (float)w / (float)GRAPH_HISTORY_SIZE;
  float x = 0;

  uint32_t index = startValue;
  bool first = true;
  QPainterPath path;
  for (uint32_t i=0; i<historyLength; i++) {
    float y = center - (values[index] * pixelPerPointY);

    if (first) {
      first = false;
      path.moveTo(x, y);
    } else {
      path.lineTo(x, y);
    }

    if (++index == historyLength) {
      index = 0;
    }

    x += pixelPerPointX;
  }

  painter.setPen(QColor(255, 0, 0, 255));
  painter.drawPath(path);
}

// ------------------------------------------------------------------------
void GraphView::setMeasurement(Measurement* measurement) {
  if (currentMeasurement) {
    disconnect(currentMeasurement, SIGNAL(triggered(int32_t)), this, SLOT(add(int32_t)));
  }

  currentMeasurement = measurement;
  clear();

  if (currentMeasurement) {
    connect(currentMeasurement, SIGNAL(triggered(int32_t)), this, SLOT(add(int32_t)));
  }
}

// ------------------------------------------------------------------------
void GraphView::add(int32_t value) {
  uint32_t offset;

  if (historyLength == GRAPH_HISTORY_SIZE) {
    offset = startValue;
    startValue = (startValue + 1) % GRAPH_HISTORY_SIZE;
  } else {
    offset = (startValue + historyLength) % GRAPH_HISTORY_SIZE;
    historyLength++;
  }

  if (autoMin && value < min) {
    min = value;
  }

  if (autoMax && value > max) {
    max = value;
  }

  values[offset] = value;

  if (autoRedraw) {
    update();
  }
}

// ------------------------------------------------------------------------
