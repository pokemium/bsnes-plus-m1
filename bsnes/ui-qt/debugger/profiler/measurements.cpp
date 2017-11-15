#include "measurements.moc"
Measurements *measurements;

// ------------------------------------------------------------------------
MeasurementValue::MeasurementValue()
  : type(MEASURE_CONSTANT)
  , constant(0)
  , other(NULL)
  , variableAddress(0)
  , variableSize(8)
  , variableSigned(false)
  , variableSource(SNES::Debugger::MemorySource::CPUBus)
  , specialVariable(SPECIAL_VCOUNTER)
{
}

// ------------------------------------------------------------------------
int32_t MeasurementValue::calculateVariable() {
  if (!SNES::cartridge.loaded()) {
    return 0;
  }

  uint32_t value = 0;
  uint8_t size = 0;
  uint32_t addr = variableAddress;

  SNES::debugger.bus_access = true;
  while (size < variableSize) {
    value |= SNES::debugger.read(variableSource, addr++) << size;
    size += 8;
  }
  SNES::debugger.bus_access = false;

  return value;
}

// ------------------------------------------------------------------------
int32_t MeasurementValue::calculateSpecial() {
  switch (specialVariable) {
  case SPECIAL_VCOUNTER: return SNES::cpu.vcounter();
  case SPECIAL_HCOUNTER: return SNES::cpu.hdot();
  case SPECIAL_FRAME: return SNES::cpu.framecounter();

  default:
    return 0;
  }
}

// ------------------------------------------------------------------------
int32_t MeasurementValue::calculate(Measurement *base) {
  switch (type) {
  case MEASURE_CONSTANT:
    return constant;

  case MEASURE_LAST:
    return base->currentValue;

  case MEASURE_FROM_OTHER:
    if (!other) {
      return 0;
    }
    return other->currentValue;

  case MEASURE_VARIABLE:
    return calculateVariable();

  case MEASURE_SPECIAL:
    return calculateSpecial();

  default:
    return 0;
  }
}


// ------------------------------------------------------------------------
Measurement::Measurement()
  : name("New measurement")
  , triggerOnExecute(false)
  , triggerAddress(0)
  , triggerOnCalculation(false)
  , triggerMeasurement(NULL)
  , currentValue(0)
  , isInCalculation(false)
  , op(OP_ADD)
{
}

// ------------------------------------------------------------------------
Measurement::~Measurement() {
  setTriggerAddress(false, 0);
  setTriggerMeasurement(false, NULL);
}

// ------------------------------------------------------------------------
void Measurement::setTriggerAddress(bool enabled, uint32_t address) {
  if (triggerOnExecute) {
    SNES::debugger.remove_trigger(triggerAddress, this);
  }

  triggerOnExecute = enabled;
  triggerAddress = address;

  if (enabled) {
    SNES::debugger.add_trigger(triggerAddress, this);
  }
}

// ------------------------------------------------------------------------
void Measurement::setTriggerMeasurement(bool enabled, Measurement *measurement) {
  if (measurement == NULL || measurement == this) {
    enabled = false;
  }

  if (!enabled) {
    measurement = NULL;
  }

  if (triggerOnCalculation && triggerMeasurement) {
    disconnect(triggerMeasurement, SIGNAL(triggered(uint32_t)), this, SLOT(trigger()));
  }

  triggerOnCalculation = enabled;
  triggerMeasurement = measurement;

  if (triggerOnCalculation && triggerMeasurement) {
    connect(triggerMeasurement, SIGNAL(triggered(uint32_t)), this, SLOT(trigger()));
  }
}

// ------------------------------------------------------------------------
void Measurement::onRemovedMeasurement(Measurement* removed) {
  if (triggerMeasurement == removed) {
    triggerMeasurement = NULL;
  }

  if (left.other == removed) {
    left.other = NULL;
  }

  if (right.other == removed) {
    right.other = NULL;
  }
}

// ------------------------------------------------------------------------
void Measurement::trigger() {
  if (isInCalculation) {
    return;
  }

  isInCalculation = true;

  int32_t l = left.calculate(this);
  int32_t r = right.calculate(this);

  switch (op) {
  case OP_ADD: currentValue = l + r; break;
  case OP_SUB: currentValue = l - r; break;
  case OP_MUL: currentValue = l * r; break;
  case OP_DIV: currentValue = r == 0 ? 0x7FFFFFFF : l / r; break;
  case OP_MOD: currentValue = r == 0 ? 0 : l % r; break;
  case OP_AND: currentValue = l & r; break;
  case OP_OR: currentValue = l | r; break;
  case OP_SHIFT_LEFT: currentValue = l << r; break;
  case OP_SHIFT_RIGHT: currentValue = l >> r; break;
  case OP_EQUAL: currentValue = l == r ? 1 : 0; break;
  case OP_NOT_EQUAL: currentValue = l != r ? 1 : 0; break;
  }

  emit triggered(currentValue);

  isInCalculation = false;
}


void on_trigger_snes(void *data) {
  Measurement *measurement = (Measurement*)data;
  measurement->trigger();
}


// ------------------------------------------------------------------------
Measurements::Measurements()
  : numMeasurements(0)
  , numMeasurementsAllocated(0)
  , measurements(NULL)
{
  SNES::debugger.trigger_function = on_trigger_snes;

  create();
}

// ------------------------------------------------------------------------
Measurements::~Measurements() {
  for (uint32_t i=0; i<numMeasurements; i++) {
    delete measurements[i];
  }

  free(measurements);
}

// ------------------------------------------------------------------------
Measurement *Measurements::create() {
  Measurement *measurement = new Measurement();
  connect(this, SIGNAL(removed(Measurement*)), measurement, SLOT(onRemovedMeasurement(Measurement*)));

  if (numMeasurementsAllocated == 0) {
    numMeasurementsAllocated = 10;
    numMeasurements = 0;
    measurements = (Measurement**)malloc(sizeof(Measurements*) * numMeasurementsAllocated);
  } else if (numMeasurements == numMeasurementsAllocated) {
    numMeasurementsAllocated += 10;
    measurements = (Measurement**)realloc(measurements, sizeof(Measurements*) * numMeasurementsAllocated);
  }

  measurements[numMeasurements++] = measurement;
  emit created(measurement);

  return measurement;
}

// ------------------------------------------------------------------------
int32_t Measurements::indexOf(Measurement *measurement) {
  for (int32_t i=0; i<numMeasurements; i++) {
    if (measurements[i] == measurement) {
      return i;
    }
  }

  return -1;
}

// ------------------------------------------------------------------------
void Measurements::remove(Measurement *measurement) {
  int32_t index = indexOf(measurement);
  if (index < 0) {
    return;
  }

  while (index + 1 < numMeasurements) {
    measurements[index] = measurements[index + 1];
    index++;
  }

  numMeasurements--;

  emit removed(measurement);

  delete measurement;

  if (numMeasurements == 0) {
    create();
  }
}

// ------------------------------------------------------------------------
