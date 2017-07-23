class CgramWidget : public QWidget {
  Q_OBJECT

public:
  CgramWidget();
  ~CgramWidget();
  void paintEvent(QPaintEvent*);
  void mousePressEvent(QMouseEvent*);

  void setScale(unsigned);

  void setPaletteBpp(unsigned);

  bool hasSelected() const;
  unsigned selectedPalette() const;

  unsigned selectedColor() const;

  void selectNone();
  void setSelected(int);

public slots:
  void refresh();

signals:
  void selectedChanged();

private:
  QImage *image;
  uint32_t *imageBuffer;
  int selected;

  unsigned scale;
  unsigned selectedMask;
  unsigned selectedWidth;
  unsigned selectedHeight;
};

uint32_t rgbFromCgram(unsigned);
