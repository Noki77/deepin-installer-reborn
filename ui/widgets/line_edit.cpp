// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/line_edit.h"

#include <QFocusEvent>
#include <QLabel>
#include <QResizeEvent>

#include "base/file_util.h"

namespace installer {

LineEdit::LineEdit(const QString& icon, QWidget* parent) : QLineEdit(parent) {
  this->setObjectName("line_edit");

  this->setStyleSheet(ReadFile(":/styles/line_edit.css"));
  // Same width as in table combobox.
  this->setFixedSize(310, 36);

  // Disable context menu.
  this->setContextMenuPolicy(Qt::NoContextMenu);

  image_label_ = new QLabel(this);
  image_label_->setPixmap(QPixmap(icon));
  image_label_->setFixedSize(12, 12);
  image_label_->move(12, 12);
}

void LineEdit::focusInEvent(QFocusEvent* event) {
  QLineEdit::focusInEvent(event);
  emit this->gotFocus();
}

void LineEdit::resizeEvent(QResizeEvent* event) {
  image_label_->move(12, 12);
  QLineEdit::resizeEvent(event);
}

}  // namespace installer