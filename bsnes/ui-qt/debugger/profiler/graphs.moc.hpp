#ifndef __GRAPHS_H__
#define __GRAPHS_H__

#define MAX_GRAPH_BUFFER 256

struct GraphLine {
  GraphLine() {}
  GraphLine(const QString &name, int offset) : name(name), offset(offset) {}

  QString name;
  int offset;
};

struct GraphValue {
  GraphValue() : offset(0), size(0) {}
  GraphValue(const QString &name, Measurement *measurement)
    : name(name)
    , measurement(measurement)
    , offset(0)
    , size(0)
    , color(QColor(255, 0, 0, 255))
  {}

  QString name;
  class Measurement *measurement;
  QColor color;

  uint32_t offset;
  uint32_t size;
  int32_t buffer[MAX_GRAPH_BUFFER];

  void clear();
  void trigger();
};

class Graph : public QObject {
  Q_OBJECT

public:
  Graph();
  ~Graph();

  QString name;
  linear_vector<GraphLine> lines;
  linear_vector<GraphValue> values;

  void addLine(const QString &name, int offset);
  void addValue(const GraphValue &value);
  void clear();
  void reset();

public slots:
  void onRemovedMeasurement(Measurement *measurement);
  void trigger();
};


class Graphs : public QObject {
  Q_OBJECT

public:
  Graphs();

  Graph *create();

  Graph **graphs;
  uint32_t numGraphs;
  uint32_t numGraphsAllocated;

  inline uint32_t size() const {
    return numGraphs;
  }

  inline Graph* get(uint32_t index) {
    return graphs[index];
  }

  Graph* find(const QString &name);

signals:
  void created(Graph*);
  void removed(Graph*);
};

extern Graphs *graphs;

#endif
