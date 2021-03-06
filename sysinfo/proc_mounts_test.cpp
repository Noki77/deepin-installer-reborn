// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/proc_mounts.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(ParseMountItemsTest, ParseMountItems) {
  const MountItemList items = ParseMountItems();
  EXPECT_GT(items.length(), 0);
}

}  // namespace
}  // namespace installer