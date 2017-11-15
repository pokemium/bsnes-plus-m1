#ifndef __MEASUREMENTS_H__
#define __MEASUREMENTS_H__

class Measurement;

struct MeasurementValue {
  enum MeasurementType {
    MEASURE_CONSTANT,
    MEASURE_LAST,
    MEASURE_FROM_OTHER,
    MEASURE_VARIABLE,
    MEASURE_SPECIAL
  };

  enum SpecialVariable {
    SPECIAL_VCOUNTER
  };

  MeasurementValue();

  MeasurementType type;
  int32_t constant;
  Measurement *other;

  uint32_t variableAddress;
  uint8_t variableSize;
  bool variableSigned;
  QString variableSource;

  SpecialVariable specialVariable;
};

class Measurement : public QObject {
  Q_OBJECT

public:
  enum Operator {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_AND,
    OP_OR,
    OP_SHIFT_LEFT,
    OP_SHIFT_RIGHT,
    OP_EQUAL,
    OP_NOT_EQUAL
  };

  Measurement();

  QString name;

  bool triggerOnExecute;
  uint32_t triggerAddress;

  bool triggerOnCalculation;
  Measurement *triggerMeasurement;

  MeasurementValue left;
  Operator op;
  MeasurementValue right;

public slots:
  void onRemovedMeasurement(Measurement*);
};


class Measurements : public QObject {
  Q_OBJECT

public:
  Measurements();
  ~Measurements();

  Measurement *create();
  int32_t indexOf(Measurement *);
  void remove(Measurement *);

  Measurement **measurements;
  uint32_t numMeasurements;
  uint32_t numMeasurementsAllocated;

  inline uint32_t size() const {
    return numMeasurements;
  }

  inline Measurement* get(uint32_t index) {
    return measurements[index];
  }

signals:
  void created(Measurement*);
  void removed(Measurement*);
};

extern Measurements *measurements;

#endif
