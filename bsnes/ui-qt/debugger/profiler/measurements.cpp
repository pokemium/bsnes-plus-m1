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
  , variableSource("cpu")
  , specialVariable(SPECIAL_VCOUNTER)
{
}


// ------------------------------------------------------------------------
Measurement::Measurement()
  : name("New measurement")
  , triggerOnExecute(false)
  , triggerAddress(0)
  , triggerOnCalculation(false)
  , triggerMeasurement(NULL)
  , op(OP_ADD)
{
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
Measurements::Measurements()
  : numMeasurements(0)
  , numMeasurementsAllocated(0)
  , measurements(NULL)
{
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
