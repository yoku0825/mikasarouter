/*
  Copyright (c) 2016, Oracle and/or its affiliates. All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/**
 * Test the fabric cache implementation.
 */

#include "fabric_cache.h"
#include "mock_fabric.h"

#include "gmock/gmock.h"

using fabric_cache::ManagedServer;

class FabricCacheTest : public ::testing::Test {
public:
  MockFabric mf;
  FabricCache cache;

  FabricCacheTest() : mf("localhost", 32275, "admin", "admin", 1, 1),
                      cache("localhost", 32275, "admin", "admin", 1, 1) {}
};

/**
 * Test that the list of servers that are part of a group is accurate.
 */
TEST_F(FabricCacheTest, ValidGroupTest_1) {
  list<ManagedServer> server_list_1;

  server_list_1 = cache.group_lookup("group-1");

  ManagedServer ms1_fetched = server_list_1.front();
  EXPECT_EQ(ms1_fetched, mf.ms1);
  server_list_1.pop_front();
  ManagedServer ms2_fetched = server_list_1.front();
  EXPECT_EQ(ms2_fetched, mf.ms2);
}

/**
 * Test that looking up an invalid group returns a empty list.
 */
TEST_F(FabricCacheTest, InvalidGroupTest) {
  list<ManagedServer> server_list;

  server_list = cache.group_lookup("InvalidGroupTest");

  EXPECT_TRUE(server_list.empty());
}
