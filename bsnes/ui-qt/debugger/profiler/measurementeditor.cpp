#include "measurementeditor.moc"

class Measurement;

MeasurementEditor *measurementEditor;

// ------------------------------------------------------------------------
MeasurementEditorConfig::MeasurementEditorConfig()
  : currentMeasurement(NULL)
{

}

// ------------------------------------------------------------------------
void MeasurementEditorConfig::update(MeasurementValue *measurement) {
  switch (measurement->type) {
  case MeasurementValue::MEASURE_CONSTANT: radioConstant->setChecked(true); break;
  case MeasurementValue::MEASURE_LAST: radioLast->setChecked(true); break;
  case MeasurementValue::MEASURE_FROM_OTHER: radioOther->setChecked(true); break;
  case MeasurementValue::MEASURE_VARIABLE: radioVariable->setChecked(true); break;
  case MeasurementValue::MEASURE_SPECIAL: radioSpecial->setChecked(true); break;
  }

  int32_t other = measurements->indexOf(measurement->other);
  if (other == -1) {
    otherValue->setCurrentIndex(0);
  } else {
    otherValue->setCurrentIndex(otherValue->findData(QVariant(other)));
  }

  constantValue->setText(integer(measurement->constant));
  variableAddress->setText(hex<6,'0'>(measurement->variableAddress));
  uint32_t type = measurement->variableSize | (measurement->variableSigned ? 0x100 : 0);
  variableType->setCurrentIndex(variableType->findData(QVariant(type)));
  variableSource->setCurrentIndex(variableSource->findData(QVariant((uint32_t)measurement->variableSource)));
  specialValue->setCurrentIndex(specialValue->findData(QVariant(measurement->specialVariable)));
}

// ------------------------------------------------------------------------
void MeasurementEditorConfig::onUpdate() {
  if (!currentMeasurement) {
    return;
  }

  if (radioConstant->isChecked()) {
    currentMeasurement->type = MeasurementValue::MEASURE_CONSTANT;
  } else if (radioLast->isChecked()) {
    currentMeasurement->type = MeasurementValue::MEASURE_LAST;
  } else if (radioOther->isChecked()) {
    currentMeasurement->type = MeasurementValue::MEASURE_FROM_OTHER;
  } else if (radioVariable->isChecked()) {
    currentMeasurement->type = MeasurementValue::MEASURE_VARIABLE;
  } else if (radioSpecial->isChecked()) {
    currentMeasurement->type = MeasurementValue::MEASURE_SPECIAL;
  }

  currentMeasurement->constant = integer(constantValue->text().toUtf8().data());
  currentMeasurement->variableAddress = hex(variableAddress->text().toUtf8().data());

  uint32_t type = variableType->itemData(variableType->currentIndex()).toUInt();
  currentMeasurement->variableSize = type & 0xFF;
  currentMeasurement->variableSigned = type & 0x100;

  currentMeasurement->variableSource = (SNES::Debugger::MemorySource)variableSource->itemData(variableSource->currentIndex()).toUInt();
  currentMeasurement->specialVariable = (MeasurementValue::SpecialVariable)specialValue->itemData(specialValue->currentIndex()).toUInt();

  int32_t otherIndex = otherValue->itemData(otherValue->currentIndex()).toInt();
  currentMeasurement->other = NULL;
  if (otherIndex >= 0 && otherIndex < measurements->size()) {
    currentMeasurement->other = measurements->get(otherIndex);
  }
}

// ------------------------------------------------------------------------
void MeasurementEditorConfig::build(const QString &name) {
  QGroupBox *box = new QGroupBox(name);
  QVBoxLayout *contentLayout = new QVBoxLayout;

  radioConstant = new QRadioButton("Constant");
  contentLayout->addWidget(radioConstant);

  constantValue = new QLineEdit();
  contentLayout->addWidget(constantValue);

  radioLast = new QRadioButton("Last result");
  contentLayout->addWidget(radioLast);

  radioOther = new QRadioButton("Result from measurement");
  contentLayout->addWidget(radioOther);

  otherValue = new QComboBox();
  contentLayout->addWidget(otherValue);

  radioVariable = new QRadioButton("Variable from memory");
  contentLayout->addWidget(radioVariable);

  QHBoxLayout *variableLayout = new QHBoxLayout();
  variableAddress = new QLineEdit();
  variableLayout->addWidget(variableAddress);

  variableSource = new QComboBox();
  variableLayout->addWidget(variableSource);
  variableSource->addItem("S-CPU bus", QVariant((uint32_t)SNES::Debugger::MemorySource::CPUBus));
  variableSource->addItem("S-SMP bus", QVariant((uint32_t)SNES::Debugger::MemorySource::APUBus));
  variableSource->addItem("S-PPU VRAM", QVariant((uint32_t)SNES::Debugger::MemorySource::VRAM));
  variableSource->addItem("S-PPU OAM", QVariant((uint32_t)SNES::Debugger::MemorySource::OAM));
  variableSource->addItem("S-PPU CGRAM", QVariant((uint32_t)SNES::Debugger::MemorySource::CGRAM));
  variableSource->addItem("SA-1 bus", QVariant((uint32_t)SNES::Debugger::MemorySource::SA1Bus));
  variableSource->addItem("GSU bus", QVariant((uint32_t)SNES::Debugger::MemorySource::SFXBus));
  contentLayout->addLayout(variableLayout);

  variableType = new QComboBox();
  //variableType->addItem("int8", QVariant(0x108));
  //variableType->addItem("int16", QVariant(0x110));
  //variableType->addItem("int24", QVariant(0x118));
  //variableType->addItem("int32", QVariant(0x120));
  variableType->addItem("uint8", QVariant(0x008));
  variableType->addItem("uint16", QVariant(0x010));
  variableType->addItem("uint24", QVariant(0x018));
  variableType->addItem("uint32", QVariant(0x020));
  contentLayout->addWidget(variableType);

  radioSpecial = new QRadioButton("Special value");
  contentLayout->addWidget(radioSpecial);

  specialValue = new QComboBox();
  specialValue->addItem("V-Counter", QVariant(MeasurementValue::SPECIAL_VCOUNTER));
  specialValue->addItem("H-Counter", QVariant(MeasurementValue::SPECIAL_HCOUNTER));
  specialValue->addItem("Frame-Counter", QVariant(MeasurementValue::SPECIAL_FRAME));
  contentLayout->addWidget(specialValue);

  contentLayout->addStretch(1);

  layout = box;
  box->setLayout(contentLayout);
  box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

  connect(radioConstant, SIGNAL(toggled(bool)), this, SLOT(onUpdate()));
  connect(radioLast, SIGNAL(toggled(bool)), this, SLOT(onUpdate()));
  connect(radioOther, SIGNAL(toggled(bool)), this, SLOT(onUpdate()));
  connect(radioVariable, SIGNAL(toggled(bool)), this, SLOT(onUpdate()));
  connect(radioSpecial, SIGNAL(toggled(bool)), this, SLOT(onUpdate()));

  connect(constantValue, SIGNAL(textEdited(const QString &)), this, SLOT(onUpdate()));
  connect(variableAddress, SIGNAL(textEdited(const QString &)), this, SLOT(onUpdate()));

  connect(otherValue, SIGNAL(currentIndexChanged(int)), this, SLOT(onUpdate()));
  connect(variableSource, SIGNAL(currentIndexChanged(int)), this, SLOT(onUpdate()));
  connect(variableType, SIGNAL(currentIndexChanged(int)), this, SLOT(onUpdate()));
  connect(specialValue, SIGNAL(currentIndexChanged(int)), this, SLOT(onUpdate()));
}


// ------------------------------------------------------------------------
MeasurementEditor::MeasurementEditor()
  : currentMeasurement(NULL)
{
  setObjectName("measurements");
  setWindowTitle("Configure measurements");

  layout = new QHBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  setLayout(layout);

  // LAYOUT

  // LAYOUT: LEFT
  QGroupBox *listBox = new QGroupBox("Measurements");
  leftLayout = new QVBoxLayout;
  listBox->setLayout(leftLayout);
  layout->addWidget(listBox);

  // LAYOUT: RIGHT
  rightLayout = new QVBoxLayout;
  layout->addLayout(rightLayout);

  generalLayout = new QVBoxLayout;
  rightLayout->addLayout(generalLayout);
  trigOptLayout = new QHBoxLayout;
  rightLayout->addLayout(trigOptLayout);

  QGroupBox *triggerBox = new QGroupBox("Trigger");
  triggerLayout = new QVBoxLayout;
  triggerBox->setLayout(triggerLayout);
  trigOptLayout->addWidget(triggerBox);

  QGroupBox *optionsBox = new QGroupBox("Options");
  optionsLayout = new QVBoxLayout;
  optionsBox->setLayout(optionsLayout);
  trigOptLayout->addWidget(optionsBox);

  calculateLayout = new QHBoxLayout;
  rightLayout->addLayout(calculateLayout);
  measureLayout = new QHBoxLayout;
  rightLayout->addLayout(measureLayout);

  // LEFT
  measurementList = new QTreeWidget;
  measurementList->header()->close();
  measurementList->setColumnCount(1);
  measurementList->setAllColumnsShowFocus(true);
  measurementList->sortByColumn(0, Qt::AscendingOrder);
  measurementList->setRootIsDecorated(false);
  measurementList->setMinimumWidth(130);
  measurementList->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  leftLayout->addWidget(measurementList);

  QHBoxLayout *leftButtons = new QHBoxLayout();
  leftLayout->addLayout(leftButtons);

  createMeasurementButton = new QPushButton("New");
  leftButtons->addWidget(createMeasurementButton);

  removeMeasurementButton = new QPushButton("Delete");
  leftButtons->addWidget(removeMeasurementButton);

  // TRIGGER
  triggerAddress = new QCheckBox("Trigger on execute:");
  triggerLayout->addWidget(triggerAddress);
  addressValue = new QLineEdit();
  triggerLayout->addWidget(addressValue);

  triggerOther = new QCheckBox("Trigger after calculation of:");
  triggerLayout->addWidget(triggerOther);
  otherValue = new QComboBox();
  triggerLayout->addWidget(otherValue);

  triggerNow = new QPushButton("Trigger manually");
  triggerLayout->addWidget(triggerNow);

  // GENERAL
  QGroupBox *nameBox = new QGroupBox("Name");
  QVBoxLayout *nameBoxLayout = new QVBoxLayout();
  nameBox->setLayout(nameBoxLayout);
  name = new QLineEdit();
  nameBoxLayout->addWidget(name);
  generalLayout->addWidget(nameBox);

  // MEASURE
  left.build("Left value");
  right.build("Right value");

  measureLayout->addWidget(left.layout);
  measureLayout->addWidget(setupMeasureOperator());
  measureLayout->addWidget(right.layout);

  // PREVIEW
  QGroupBox *previewBox = new QGroupBox("Preview");
  QVBoxLayout *previewBoxLayout = new QVBoxLayout();
  preview = new GraphView();
  graph = new Graph();
  preview->setMinimumHeight(150);
  preview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
  preview->setGraph(graph);
  previewBoxLayout->addWidget(preview);
  previewBox->setLayout(previewBoxLayout);
  rightLayout->addWidget(previewBox);

  connect(createMeasurementButton, SIGNAL(released()), this, SLOT(createMeasurement()));
  connect(removeMeasurementButton, SIGNAL(released()), this, SLOT(removeMeasurement()));
  connect(triggerNow, SIGNAL(released()), this, SLOT(trigger()));
  connect(measurements, SIGNAL(created(Measurement*)), this, SLOT(updateList()));
  connect(measurements, SIGNAL(removed(Measurement*)), this, SLOT(updateList()));
  connect(measurements, SIGNAL(removed(Measurement*)), this, SLOT(updateList()));
  connect(name, SIGNAL(textEdited(const QString&)), this, SLOT(onUpdateName()));

  connect(triggerAddress, SIGNAL(toggled(bool)), this, SLOT(onUpdateTrigger()));
  connect(triggerOther, SIGNAL(toggled(bool)), this, SLOT(onUpdateTrigger()));
  connect(addressValue, SIGNAL(textEdited(const QString &)), this, SLOT(onUpdateTrigger()));
  connect(otherValue, SIGNAL(currentIndexChanged(int)), this, SLOT(onUpdateTrigger()));

  connect(measurementList, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(onMeasurementSelected(QTreeWidgetItem *)));

  updateList();
}

// ------------------------------------------------------------------------
QGroupBox *MeasurementEditor::setupMeasureOperator() {
  QGroupBox *box = new QGroupBox("Calculate");
  QVBoxLayout *layout = new QVBoxLayout;

  opAdd = new QRadioButton("+");
  layout->addWidget(opAdd);

  opSub = new QRadioButton("-");
  layout->addWidget(opSub);

  opMul = new QRadioButton("*");
  layout->addWidget(opMul);

  opDiv = new QRadioButton("/");
  layout->addWidget(opDiv);

  opMod = new QRadioButton("%");
  layout->addWidget(opMod);

  opAnd = new QRadioButton("&&");
  layout->addWidget(opAnd);

  opOr = new QRadioButton("|");
  layout->addWidget(opOr);

  opShLeft = new QRadioButton("<<");
  layout->addWidget(opShLeft);

  opShRight = new QRadioButton(">>");
  layout->addWidget(opShRight);

  opEq = new QRadioButton("==");
  layout->addWidget(opEq);

  opNeq = new QRadioButton("!=");
  layout->addWidget(opNeq);

  layout->addStretch(1);

  box->setLayout(layout);
  box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  connect(opAdd, SIGNAL(toggled(bool)), this, SLOT(onUpdateCalculate()));
  connect(opSub, SIGNAL(toggled(bool)), this, SLOT(onUpdateCalculate()));
  connect(opMul, SIGNAL(toggled(bool)), this, SLOT(onUpdateCalculate()));
  connect(opDiv, SIGNAL(toggled(bool)), this, SLOT(onUpdateCalculate()));
  connect(opMod, SIGNAL(toggled(bool)), this, SLOT(onUpdateCalculate()));
  connect(opAnd, SIGNAL(toggled(bool)), this, SLOT(onUpdateCalculate()));
  connect(opOr, SIGNAL(toggled(bool)), this, SLOT(onUpdateCalculate()));
  connect(opShLeft, SIGNAL(toggled(bool)), this, SLOT(onUpdateCalculate()));
  connect(opShRight, SIGNAL(toggled(bool)), this, SLOT(onUpdateCalculate()));
  connect(opEq, SIGNAL(toggled(bool)), this, SLOT(onUpdateCalculate()));
  connect(opNeq, SIGNAL(toggled(bool)), this, SLOT(onUpdateCalculate()));

  return box;
}

// ------------------------------------------------------------------------
void MeasurementEditor::onMeasurementSelected(QTreeWidgetItem *item) {
  if (item == NULL) {
    return;
  }

  uint32_t index = item->data(0, Qt::UserRole).toUInt();

  if (index < measurements->size()) {
    select(measurements->get(index));
  }
}

// ------------------------------------------------------------------------
void MeasurementEditor::createMeasurement() {
  Measurement *measurement = measurements->create();

  int32_t index = measurements->indexOf(measurement);
  for (uint32_t i=0; i<measurementList->topLevelItemCount(); i++) {
    QTreeWidgetItem *item = measurementList->topLevelItem(i);

    uint32_t match = item->data(0, Qt::UserRole).toUInt();
    if (match == index) {
      measurementList->setCurrentItem(item);
      break;
    }
  }
}

// ------------------------------------------------------------------------
void MeasurementEditor::removeMeasurement() {
  Measurement *current = NULL;
  QTreeWidgetItem *currentItem = measurementList->currentItem();
  if (currentItem) {
    uint32_t index = currentItem->data(0, Qt::UserRole).toUInt();
    if (index < measurements->size()) {
      current = measurements->get(index);
    }
  }

  if (current) {
    measurements->remove(current);
  }
}

// ------------------------------------------------------------------------
void MeasurementEditor::updateList() {
  currentMeasurement = NULL;
  left.currentMeasurement = NULL;
  right.currentMeasurement = NULL;

  Measurement *current = NULL;
  Measurement *selectedMeasurement = NULL;
  QTreeWidgetItem *currentItem = measurementList->currentItem();
  if (currentItem) {
    uint32_t index = currentItem->data(0, Qt::UserRole).toUInt();
    if (index < measurements->size()) {
      current = measurements->get(index);
    }
  }

  measurementList->clear();
  measurementList->setSortingEnabled(false);

  otherValue->clear();
  otherValue->addItem("-", QVariant(-1));
  left.otherValue->clear();
  left.otherValue->addItem("-", QVariant(-1));
  right.otherValue->clear();
  right.otherValue->addItem("-", QVariant(-1));

  currentItem = NULL;
  uint32_t items = measurements->size();
  for (uint32_t i=0; i<items; i++) {
    Measurement *measurement = measurements->get(i);

    auto item = new QTreeWidgetItem(measurementList);
    item->setData(0, Qt::UserRole, QVariant(i));
    item->setText(0, measurement->name);

    otherValue->addItem(measurement->name, QVariant(i));
    left.otherValue->addItem(measurement->name, QVariant(i));
    right.otherValue->addItem(measurement->name, QVariant(i));

    if (measurement == current) {
      currentItem = item;
      selectedMeasurement = measurement;
    } else if (currentItem == NULL) {
      currentItem = item;
      selectedMeasurement = measurement;
    }
  }

  measurementList->setSortingEnabled(true);

  if (currentItem) {
    measurementList->setCurrentItem(currentItem);
  }
}

// ------------------------------------------------------------------------
void MeasurementEditor::select(Measurement *measurement) {
  currentMeasurement = NULL;
  left.currentMeasurement = NULL;
  right.currentMeasurement = NULL;

  name->setText(measurement->name);

  triggerAddress->setCheckState(measurement->triggerOnExecute ? Qt::Checked : Qt::Unchecked);
  addressValue->setText(hex<6,'0'>(measurement->triggerAddress));
  triggerOther->setCheckState(measurement->triggerOnCalculation ? Qt::Checked : Qt::Unchecked);

  int32_t other = measurements->indexOf(measurement->triggerMeasurement);
  if (other == -1) {
    otherValue->setCurrentIndex(0);
  } else {
    otherValue->setCurrentIndex(otherValue->findData(QVariant(other)));
  }

  switch(measurement->op) {
    case Measurement::OP_ADD: opAdd->setChecked(true); break;
    case Measurement::OP_SUB: opSub->setChecked(true); break;
    case Measurement::OP_MUL: opMul->setChecked(true); break;
    case Measurement::OP_DIV: opDiv->setChecked(true); break;
    case Measurement::OP_MOD: opMod->setChecked(true); break;
    case Measurement::OP_AND: opAnd->setChecked(true); break;
    case Measurement::OP_OR: opOr->setChecked(true); break;
    case Measurement::OP_SHIFT_LEFT: opShLeft->setChecked(true); break;
    case Measurement::OP_SHIFT_RIGHT: opShRight->setChecked(true); break;
    case Measurement::OP_EQUAL: opEq->setChecked(true); break;
    case Measurement::OP_NOT_EQUAL: opNeq->setChecked(true); break;
  }

  left.update(&measurement->left);
  right.update(&measurement->right);

  currentMeasurement = measurement;
  left.currentMeasurement = &measurement->left;
  right.currentMeasurement = &measurement->right;

  graph->reset();
  graph->addLine("0", 0);
  graph->addLine("64", 64);
  graph->addLine("128", 128);
  graph->addLine("192", 192);
  graph->addLine("256", 256);
  graph->addValue(GraphValue(measurement->name, measurement));

  preview->setGraph(graph);
  preview->setTrigger(currentMeasurement);
  preview->clear();
}

// ------------------------------------------------------------------------
void MeasurementEditor::onUpdateName() {
  if (!currentMeasurement) {
    return;
  }

  currentMeasurement->name = name->text();
  updateList();
}

// ------------------------------------------------------------------------
void MeasurementEditor::onUpdateTrigger() {
  if (!currentMeasurement) {
    return;
  }

  Measurement *other = NULL;
  int32_t otherIndex = otherValue->itemData(otherValue->currentIndex()).toInt();
  if (otherIndex >= 0 && otherIndex < measurements->size()) {
    other = measurements->get(otherIndex);
  }

  currentMeasurement->setTriggerAddress(
    triggerAddress->isChecked(),
    hex(addressValue->text().toUtf8().data())
  );

  currentMeasurement->setTriggerMeasurement(
    triggerOther->isChecked(),
    other
  );
}

// ------------------------------------------------------------------------
void MeasurementEditor::onUpdateCalculate() {
  if (!currentMeasurement) {
    return;
  }

  if (opAdd->isChecked()) {
    currentMeasurement->op = Measurement::OP_ADD;
  }
  if (opSub->isChecked()) {
    currentMeasurement->op = Measurement::OP_SUB;
  }
  if (opMul->isChecked()) {
    currentMeasurement->op = Measurement::OP_MUL;
  }
  if (opDiv->isChecked()) {
    currentMeasurement->op = Measurement::OP_DIV;
  }
  if (opMod->isChecked()) {
    currentMeasurement->op = Measurement::OP_MOD;
  }
  if (opAnd->isChecked()) {
    currentMeasurement->op = Measurement::OP_AND;
  }
  if (opOr->isChecked()) {
    currentMeasurement->op = Measurement::OP_OR;
  }
  if (opShLeft->isChecked()) {
    currentMeasurement->op = Measurement::OP_SHIFT_LEFT;
  }
  if (opShRight->isChecked()) {
    currentMeasurement->op = Measurement::OP_SHIFT_RIGHT;
  }
  if (opEq->isChecked()) {
    currentMeasurement->op = Measurement::OP_EQUAL;
  }
  if (opNeq->isChecked()) {
    currentMeasurement->op = Measurement::OP_NOT_EQUAL;
  }
}

// ------------------------------------------------------------------------
void MeasurementEditor::trigger() {
  if (!currentMeasurement) {
    return;
  }

  currentMeasurement->trigger();
}

// ------------------------------------------------------------------------
