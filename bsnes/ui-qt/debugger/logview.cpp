#include "logview.moc"

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

LogView::LogView(QWidget *parent) : QWidget(parent) {
  webview = new QWebEngineView;
  webview->setHtml(QString(logViewHtmlContent));

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(webview);
  setLayout(layout);
}

void LogView::update() {
  if (messageBuffer.size()) {
    for (LogMessage &m : messageBuffer) {
      webview->page()->runJavaScript(string() << "putp('" << m.message << "', '" << m.color << "')");
    }
    webview->page()->runJavaScript("pgend()");
    messageBuffer.clear();
  }
}

void LogView::clear() {
  messageBuffer.clear();
  webview->page()->runJavaScript("pgclr()");
}

void LogView::echo(const char *message, const char *color) {
  messageBuffer.emplace_back(LogMessage(message, color));
}
