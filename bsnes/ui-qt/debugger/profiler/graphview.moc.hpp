#define GRAPH_HISTORY_SIZE 500

class Measurement;

class GraphView : public QWidget {
  Q_OBJECT

public:
  GraphView();
  ~GraphView();

  void setMeasurement(Measurement*);
  void paintEvent(QPaintEvent *event);
  
  int32_t values[GRAPH_HISTORY_SIZE];
  uint32_t startValue;
  uint32_t historyLength;

  bool autoRedraw;

  bool autoMin;
  int32_t min;

  bool autoMax;
  int32_t max;

public slots:
  void clear();
  void add(int32_t);

protected:
  Measurement *currentMeasurement;
};
