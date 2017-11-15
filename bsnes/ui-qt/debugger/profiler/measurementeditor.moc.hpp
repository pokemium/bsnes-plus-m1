class MeasurementEditorConfig : public QObject {
  Q_OBJECT

public:
  MeasurementEditorConfig();

  QGroupBox *layout;

  QRadioButton *radioConstant;
  QLineEdit *constantValue;

  QRadioButton *radioLast;
  QRadioButton *radioOther;
  QComboBox *otherValue;

  QRadioButton *radioVariable;
  QLineEdit *variableAddress;
  QComboBox *variableSource;
  QComboBox *variableType;

  QRadioButton *radioSpecial;
  QComboBox *specialValue;

  MeasurementValue *currentMeasurement;

  void update(MeasurementValue *measurement);
  void build(const QString &name);

public slots:
  void onUpdate();
};

class MeasurementEditor : public Window {
  Q_OBJECT

public:
  MeasurementEditor();

  QHBoxLayout *layout;

  //left
  QVBoxLayout *leftLayout;
  QTreeWidget *measurementList;
  QPushButton *createMeasurementButton;
  QPushButton *removeMeasurementButton;

  //right
  QVBoxLayout *rightLayout;
  QHBoxLayout *trigOptLayout;
  QHBoxLayout *calculateLayout;

  // general
  QVBoxLayout *generalLayout;
  QLineEdit *name;

  // trigger
  QVBoxLayout *triggerLayout;
  QCheckBox *triggerAddress;
  QLineEdit *addressValue;
  QCheckBox *triggerOther;
  QComboBox *otherValue;

  // options
  QVBoxLayout *optionsLayout;

  // measure
  QHBoxLayout *measureLayout;
  MeasurementEditorConfig left;
  MeasurementEditorConfig right;

  // calculate
  QRadioButton *opAdd;
  QRadioButton *opSub;
  QRadioButton *opMul;
  QRadioButton *opDiv;
  QRadioButton *opMod;
  QRadioButton *opAnd;
  QRadioButton *opOr;
  QRadioButton *opShLeft;
  QRadioButton *opShRight;
  QRadioButton *opEq;
  QRadioButton *opNeq;

public slots:
  void createMeasurement();
  void removeMeasurement();
  void updateList();
  void onMeasurementSelected(QTreeWidgetItem *);
  void onUpdateName();
  void onUpdateCalculate();
  void onUpdateTrigger();

protected:
  Measurement *currentMeasurement;
  void select(Measurement *measurement);

  QGroupBox *setupMeasureOperator();
};

extern MeasurementEditor *measurementEditor;
