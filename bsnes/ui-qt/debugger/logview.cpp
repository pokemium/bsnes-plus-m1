#include "logview.moc"

#if defined(USE_WEBENGINE)
const char logViewHtmlContent[] =
  "<html>"
  "<head>"
  "<meta charset='utf-8'>"
  "<style type = 'text/css'>"
    "body { margin:2px; }"
    "p,span { margin:0; white-space:pre-wrap; font-family:'SF Mono','Menlo','Liberation Mono','Lucida Console','Courier New'; font-size:10px; line-height:13px; }"
  "</style>"
  "<script type='text/javascript'>"
    "window.pgend = function() { window.scrollTo(0,document.body.scrollHeight); };"
    "window.pgclr = function() { document.body.textContent = ''; };"
    "window.putp = function(s,c) {"
      "const p = document.createElement('p');"
      "p.textContent = ''+s;"
      "if (!!c) p.style.color = ''+c;"
      "document.body.appendChild(p);"
    "};"
  "</script>"
  "</head>"
  "<body></body>"
  "</html>";
#endif

LogView::LogView(QWidget *parent) : QWidget(parent) {
#if defined(USE_WEBENGINE)
  console = new QWebEngineView;
  console->setHtml(QString(logViewHtmlContent));
#else
  console = new QPlainTextEdit;
  console->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  console->setReadOnly(true);
  console->setUndoRedoEnabled(false);
  console->setFont(QFont(Style::Monospace, Style::MonospaceSize));
  console->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  console->setMinimumWidth((98 + 4) * console->fontMetrics().width(' '));
  console->setMinimumHeight((25 + 1) * console->fontMetrics().height());
#endif

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setMargin(0);
  layout->setSpacing(Style::TightWidgetSpacing);
  layout->addWidget(console);
  setLayout(layout);
}

void LogView::update() {
#if defined(USE_WEBENGINE)
  if (messageBuffer.size()) {
    for (LogMessage &m : messageBuffer) {
      console->page()->runJavaScript(string() << "putp('" << m.message << "', '" << m.color << "')");
    }
    console->page()->runJavaScript("pgend()");
    messageBuffer.clear();
  }
#else
  if (messageBuffer.size()) {
    QTextCursor cursor = QTextCursor(console->document());
    cursor.movePosition(QTextCursor::End);
    for (LogMessage &m : messageBuffer) {
      string s = m.message;
      s.replace(" ", "&nbsp;");
      cursor.insertHtml(string() << "<font color='" << m.color << "'>" << s << "</font><br>");
    }
    messageBuffer.clear();
  }
#endif
}


void LogView::clear() {
  messageBuffer.clear();
#if defined(USE_WEBENGINE)
  console->page()->runJavaScript("pgclr()");
#else
  console->clear();
#endif
}

void LogView::echo(const char *message, const char *color) {
  messageBuffer.emplace_back(LogMessage(message, color));
}
