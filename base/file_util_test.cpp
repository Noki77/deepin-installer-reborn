// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "base/file_util.h"

#include "third_party/googletest/include/gtest/gtest.h"

namespace installer {
namespace {

TEST(FileUtil, CopyFodlerTest) {
  EXPECT_TRUE(CopyFolder("/etc/systemd/system", "/tmp/systemd/system", false));
  EXPECT_TRUE(CopyFolder("/etc/pacman.d", "/tmp/pacman.d", false));
}

TEST(FileUtil, GetFileNameTest) {
  EXPECT_EQ(GetFileName("/etc/pacman.d/mirrorlist"), "mirrorlist");
}

}  // namespace
}  // namespace installer