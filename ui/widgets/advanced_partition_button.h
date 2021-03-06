// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_ADVANCED_PARTITION_ITEM_H
#define INSTALLER_UI_WIDGETS_ADVANCED_PARTITION_ITEM_H

#include "ui/widgets/pointer_button.h"

#include "partman/partition.h"

namespace installer {

// Display partition information in AdvancedPartitionFrame.
class AdvancedPartitionButton : public PointerButton {
  Q_OBJECT

  Q_PROPERTY(double alpha READ alpha WRITE setAlpha)

 public:
  AdvancedPartitionButton(const Partition& partition,
                          QWidget* parent = nullptr);

  double alpha() const { return alpha_; }

  // Get partition struct binding to current button.
  const Partition& partition() const;

 signals:
  // Emitted when delete partition button is clicked.
  void deletePartitionTriggered(const Partition& partition);

  // Emitted when edit-partition button is clicked.
  void editPartitionTriggered(const Partition& partition);

  // Emitted when edit-partition button is clicked and type of current
  // partition is Unallocated.
  void newPartitionTriggered(const Partition& partition);

 public slots:
  // Reset background alpha value of button to default.
  void resetAlpha();

  // Update background alpha value of button.
  void setAlpha(double alpha);

  void setEditable(bool editable);

 private:
  void initConnections();
  void initUI();

  // Update stylesheet to set background alpha value.
  void updateStyle();

  const Partition& partition_;

  // Switch status between new-partition, edit-partition and delete-partition.
  PointerButton* control_button_ = nullptr;
  enum class ControlStatus {
    Delete,
    Edit,
    Hide,
    New,
  };
  ControlStatus control_status_;
  bool editable_;
  void updateStatus();

  double alpha_;

 private slots:
  void onControlButtonClicked();

  // Handle toggled signal from self.
  void onToggled();
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_ADVANCED_PARTITION_ITEM_H
