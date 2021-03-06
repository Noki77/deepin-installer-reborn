// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/simple_partition_button.h"

#include <QLabel>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "ui/delegates/partition_util.h"
#include "ui/widgets/rounded_progress_bar.h"

namespace installer {

namespace {

const int kButtonWidth = 220;
const int kButtonHeight = 220;
const int kOsIconWidth = 120;
const int kOsIconHeight = 120;

}  // namespace

SimplePartitionButton::SimplePartitionButton(const Partition& partition,
                                             QWidget* parent)
    : PointerButton(parent),
      partition_(partition),
      selected_(false) {
  this->setObjectName("simple_partition_button");
  this->setFixedSize(kButtonWidth, kButtonHeight);
  this->setCheckable(true);
  this->initUI();
}

void SimplePartitionButton::setSelected(bool selected) {
  selected_ = selected;
  if (selected) {
    const QPixmap pixmap(":/images/driver_install_128.png");
    os_label_->setPixmap(pixmap);
  } else {
    QPixmap pixmap(GetOsTypeLargeIcon(partition_.os));
    os_label_->setPixmap(pixmap);
  }
}

void SimplePartitionButton::initUI() {
  os_label_ = new QLabel();
  os_label_->setObjectName("fs_label");
  os_label_->setFixedSize(kOsIconWidth, kOsIconHeight);
  os_label_->setPixmap(QPixmap(GetOsTypeLargeIcon(partition_.os)));

  QLabel* path_label = new QLabel(GetPartitionLabelAndPath(partition_));
  path_label->setObjectName("path_label");
  path_label->setFixedHeight(20);

  QLabel* usage_label = new QLabel();
  usage_label->setFixedHeight(16);
  usage_label->setText(GetPartitionUsage(partition_));
  usage_label->setObjectName("usage_label");

  QProgressBar* usage_bar = new RoundedProgressBar();
  usage_bar->setFixedSize(80, 6);
  usage_bar->setValue(GetPartitionUsageValue(partition_));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addStretch();
  layout->addWidget(os_label_, 0, Qt::AlignHCenter);
  layout->addSpacing(6);
  layout->addWidget(path_label, 0, Qt::AlignHCenter);
  layout->addWidget(usage_label, 0, Qt::AlignHCenter);
  layout->addSpacing(6);
  layout->addWidget(usage_bar, 0, Qt::AlignHCenter);
  layout->addStretch();

  this->setLayout(layout);

  this->setStyleSheet(ReadFile(":/styles/simple_partition_button.css"));
  this->setCheckable(true);
  this->setFixedSize(kButtonWidth, kButtonHeight);
}

}  // namespace installer
