// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_CONFIRM_QUIT_FRAME_H
#define INSTALLER_UI_FRAMES_CONFIRM_QUIT_FRAME_H

#include <QFrame>

namespace installer {

class CommentLabel;
class NavButton;
class TitleLabel;

// This frame is displayed when close-button is clicked.
class ConfirmQuitFrame : public QFrame {
  Q_OBJECT

 public:
  explicit ConfirmQuitFrame(QWidget* parent = nullptr);

 signals:
  // Emitted when cancel-button is clicked.
  void quitCancelled();

  // Emitted when users confirms to quit this program.
  void quitConfirmed();

 protected:
  void changeEvent(QEvent* event) override;

 private:
  void initConnections();
  void initUI();

  TitleLabel* title_label_ = nullptr;
  CommentLabel* comment_label_ = nullptr;
  NavButton* continue_button_ = nullptr;
  NavButton* abort_button_ = nullptr;
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_CONFIRM_QUIT_FRAME_H
