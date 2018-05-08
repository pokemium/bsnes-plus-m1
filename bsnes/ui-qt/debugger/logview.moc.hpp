// TODO: Based on some #define, create either QPlainTextEdit or QWebEngineView for the console

#include <list>

struct LogMessage {
  string message;
  string color;

  LogMessage(const string& _message, const string& _color = "") {
    color = _color;
    message = string() << _message;
    message.replace("\n", "\\n");
  }
};

class LogView : public QWidget {
  Q_OBJECT

public:
  LogView(QWidget *parent = 0);

  void update();
  void clear();
  void echo(const char *message, const char *color = "");

private:
  QWebEngineView *webview;
  std::list<LogMessage> messageBuffer;
};
