#define GRAPH_HISTORY_SIZE 500

class Graph;
class Trigger;

class GraphView : public QWidget {
  Q_OBJECT

public:
  GraphView();
  ~GraphView();

  void setGraph(Graph*);
  void setTrigger(Measurement*);
  void paintEvent(QPaintEvent *event);

  int32_t min;
  int32_t max;

public slots:
  void clear();
  void trigger();
  void onRemovedMeasurement(Measurement*);

protected:
  Graph *graph;
  Measurement *currentTrigger;
};
