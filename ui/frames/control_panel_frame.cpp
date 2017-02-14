// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/control_panel_frame.h"

#include <QDebug>
#include <QPropertyAnimation>
#include <QScrollBar>
#include <QStackedWidget>
#include <QTabBar>
#include <QTextEdit>
#include <QTextFormat>
#include <QTimer>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "service/log_manager.h"
#include "ui/widgets/pointer_button.h"
#include "ui/widgets/table_combo_box.h"

namespace installer {

namespace {

// Read log file each 1000ms.
const int kReadLogInterval = 1000;

const int kWindowWidth = 860;
const int kWindowHeight = 480;

const int kSettingsPageId = 2;

// Absolute path to settings file.
// Same in service/settings_manager.cpp.
const char kInstallerConfigFile[] = "/etc/deepin-installer.conf";

}  // namespace

ControlPanelFrame::ControlPanelFrame(QWidget* parent)
    : QFrame(parent),
      timer_(new QTimer(this)),
      log_file_path_(GetLogFilepath()),
      log_content_(){
  this->setObjectName("control_panel_frame");

  timer_->setInterval(kReadLogInterval);
  this->initUI();
  this->initConnections();
}

void ControlPanelFrame::toggleVisible() {
  if (!this->isVisible()) {
    // Slide in
    const QRect geom = this->parentWidget()->geometry();
    const int x_orig = geom.x() - this->width();
    const int x = geom.x();
    const int y_orig = geom.y();
    const int y = geom.y();
    this->move(x_orig, y_orig);
    this->show();
    this->raise();
    QPropertyAnimation* animation = new QPropertyAnimation(this, "pos", this);
    animation->setStartValue(QPoint(x_orig, y_orig));
    animation->setEndValue(QPoint(x, y));
    animation->setDuration(300);
    connect(animation, &QPropertyAnimation::finished,
            animation, &QPropertyAnimation::deleteLater);
    animation->start();

    // Read log file immediately.
    this->onTimerTimeout();
    timer_->start();
  } else {
    // Slide out
    const int x_orig = this->x();
    const int x = x_orig - this->width();
    const int y_orig = this->y();
    const int y = y_orig;
    this->move(x_orig, y_orig);
    this->show();
    this->raise();
    QPropertyAnimation* animation = new QPropertyAnimation(this, "pos", this);
    animation->setStartValue(QPoint(x_orig, y_orig));
    animation->setEndValue(QPoint(x, y));
    animation->setDuration(100);
    connect(animation, &QPropertyAnimation::finished,
            this, &ControlPanelFrame::hide);
    connect(animation, &QPropertyAnimation::finished,
            animation, &QPropertyAnimation::deleteLater);
    animation->start();

    timer_->stop();
  }
}

void ControlPanelFrame::initConnections() {
  connect(page_combo_box_,
          static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &ControlPanelFrame::currentPageChanged);
  connect(tab_bar_, &QTabBar::currentChanged,
          this, &ControlPanelFrame::onTabBarChanged);
  connect(log_viewer_, &QTextEdit::cursorPositionChanged,
          this, &ControlPanelFrame::onLogViewerCursorPositionChanged);
  connect(timer_, &QTimer::timeout,
          this, &ControlPanelFrame::onTimerTimeout);

  connect(refresh_devices_button_, &QPushButton::clicked,
          this, &ControlPanelFrame::requestRefreshDevices);
  connect(refresh_qr_button_, &QPushButton::clicked,
          this, &ControlPanelFrame::requestRefreshQR);
  connect(simulate_slide_button_, &QPushButton::clicked,
          this, &ControlPanelFrame::requestSimulateSlide);
}

void ControlPanelFrame::initUI() {
  tab_bar_ = new QTabBar();
  tab_bar_->setShape(QTabBar::RoundedEast);
  tab_bar_->addTab("Log");
  tab_bar_->addTab("Pages");
  tab_bar_->addTab("Settings");

  log_viewer_ = new QTextEdit();
  log_viewer_->setObjectName("log_viewer");
  // Disable user modification.
  log_viewer_->setReadOnly(true);
  // Disable rich text support.
  log_viewer_->setAcceptRichText(false);
  // Disable context menu.
  log_viewer_->setContextMenuPolicy(Qt::NoContextMenu);
  log_viewer_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  log_viewer_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  page_combo_box_ = new TableComboBox();
  page_combo_box_->addItems({"ConfirmQuitFrame",
                             "DiskSpaceInsufficientFrame",
                             "InstallFailedFrame",
                             "InstallProgressFrame",
                             "InstallSuccessFrame",
                             "PackageListFrame",
                             "PartitionFrame",
                             "PartitionTableWarningFrame",
                             "SelectLanguageFrame",
                             "SystemInfoFrame",
                             "VirtualMachineFrame",
                            });

  refresh_devices_button_ = new PointerButton();
  refresh_devices_button_->setObjectName("refresh_devices");
  refresh_devices_button_->setText("Refresh Device List");
  refresh_devices_button_->setFixedHeight(40);

  refresh_qr_button_ = new PointerButton();
  refresh_qr_button_->setObjectName("refresh_qr");
  refresh_qr_button_->setText("Refresh QR");
  refresh_qr_button_->setFixedHeight(40);

  simulate_slide_button_ = new PointerButton();
  simulate_slide_button_->setObjectName("simulate_slide");
  simulate_slide_button_->setText("Start slide");
  simulate_slide_button_->setFixedHeight(40);

  QHBoxLayout* page_layout = new QHBoxLayout();
  page_layout->addWidget(page_combo_box_, 0, Qt::AlignLeft | Qt::AlignTop);
  page_layout->addWidget(refresh_devices_button_, 0,
                         Qt::AlignLeft | Qt::AlignTop);
  page_layout->addWidget(refresh_qr_button_, 0, Qt::AlignLeft | Qt::AlignTop);
  page_layout->addWidget(simulate_slide_button_, 0,
                         Qt::AlignLeft | Qt::AlignTop);
  page_layout->addStretch();

  QFrame* page_frame = new QFrame();
  page_frame->setContentsMargins(0, 0, 0, 0);
  page_frame->setLayout(page_layout);

  settings_viewer_ = new QTextEdit();
  settings_viewer_->setObjectName("settings_viewer");
  // Disable user modification.
  settings_viewer_->setReadOnly(true);
  // Disable rich text support.
  settings_viewer_->setAcceptRichText(false);
  // Disable context menu.
  settings_viewer_->setContextMenuPolicy(Qt::NoContextMenu);
  settings_viewer_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  settings_viewer_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  stacked_widget_ = new QStackedWidget();
  stacked_widget_->addWidget(log_viewer_);
  stacked_widget_->addWidget(page_frame);
  stacked_widget_->addWidget(settings_viewer_);

  QHBoxLayout* layout = new QHBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(stacked_widget_);
  layout->addWidget(tab_bar_);

  this->setLayout(layout);
  this->setStyleSheet(ReadFile(":/styles/control_panel_frame.css"));
  this->setFixedSize(kWindowWidth, kWindowHeight);
}

void ControlPanelFrame::onLogViewerCursorPositionChanged() {
  // Highlight current line.
  QTextEdit::ExtraSelection highlight;
  highlight.cursor = log_viewer_->textCursor();
  highlight.cursor.clearSelection();
  highlight.format.setProperty(QTextFormat::FullWidthSelection, true);
  highlight.format.setBackground(QBrush(QColor(65, 65, 65)));
  log_viewer_->setExtraSelections({highlight});
}

void ControlPanelFrame::onTabBarChanged(int index) {
  if (index == kSettingsPageId) {
    const QString content = ReadFile(kInstallerConfigFile);
    settings_viewer_->setText(content);
  }
  stacked_widget_->setCurrentIndex(index);
}

void ControlPanelFrame::onTimerTimeout() {
  const QString content(ReadFile(log_file_path_));
  if (content.length() == log_content_.length()) {
    return;
  }
  log_content_ = content;

  QTextCursor cursor = log_viewer_->textCursor();
  // Restore vertical position when log content is updated.
  const int pos = log_viewer_->verticalScrollBar()->value();
  log_viewer_->setPlainText(log_content_);
  log_viewer_->verticalScrollBar()->setValue(pos);
}

}  // namespace installer