// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_DELEGATES_OPERATION_RESIZE_H
#define DEEPIN_INSTALLER_REBORN_UI_DELEGATES_OPERATION_RESIZE_H

#include "ui/delegates/operation.h"

namespace ui {

// To mark extended partition is resized.
class OperationResize : public Operation {
 public:
  OperationResize(const partman::Partition& partition_orig,
                  const partman::Partition& partition_new);

  void applyToVisual(partman::PartitionList& partitions) const override;
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_DELEGATES_OPERATION_RESIZE_H