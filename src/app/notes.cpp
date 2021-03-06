#include <iostream>

#include <notes.h>

DruidNotes::DruidNotes() { setupUi(this); }

QString DruidNotes::text() const { return _notes->document()->toPlainText(); }

void DruidNotes::setText(const QString &text) {
  _notes->document()->setPlainText(text);
}
