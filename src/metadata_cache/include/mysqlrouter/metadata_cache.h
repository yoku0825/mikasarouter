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

#ifndef MYSQLROUTER_METADATA_CACHE_INCLUDED
#define MYSQLROUTER_METADATA_CACHE_INCLUDED

#include <stdexcept>
#include <exception>
#include <vector>
#include <map>
#include <string>

#include "mysqlrouter/utils.h"
#include "mysqlrouter/datatypes.h"

namespace metadata_cache {

extern const uint16_t kDefaultMetadataPort;
extern const std::string kDefaultMetadataAddress;
extern const std::string kDefaultMetadataUser;
extern const std::string kDefaultMetadataPassword;
extern const unsigned int kDefaultMetadataTTL;
extern const std::string kDefaultMetadataReplicaset;

/** @class ManagedInstance
 *
 * Class ManagedInstance represents a server managed by the topology.
 */
class ManagedInstance {
public:
  /** @brief The replicaset ID of the replicaset to which the server belongs */
  std::string replicaset_name;
  /** @brief The name of the server */
  std::string instance_name;
  /** @brief The role of the server */
  std::string role;
  /** @brief The mode of the server */
  std::string mode;
  /** @brief The server weight */
  float weight;
  /** @brief The version token of the server */
  unsigned int version_token;
  /** @brief The location of the server */
  std::string location;
  /** @brief The host name on which the server is running */
  std::string host;
  /** The port number in which the server is running */
  unsigned int port;
};

/** @class connection_error
 *
 * Class that represents all the exceptions thrown while trying to
 * connect with a node managed by the topology.
 *
 */
class connection_error : public std::runtime_error {
public:
  explicit connection_error(const std::string &what_arg) : std::runtime_error(
    what_arg) { }
};

/** @class metadata_error
 * Class that represents all the exceptions that are thrown while fetching the
 * metadata.
 *
 */
class metadata_error : public std::runtime_error {
public:
  explicit metadata_error(const std::string &what_arg) : std::runtime_error(
    what_arg) { }
};

/** @class LookupResult
 *
 * Class holding result after looking up data in the cache.
 */
class LookupResult {
public:
  /** @brief Constructor */
  LookupResult(const std::vector<ManagedInstance> &instance_vector_) :
  instance_vector(instance_vector_) { }

  /** @brief List of ManagedInstance objects */
  const std::vector<metadata_cache::ManagedInstance> instance_vector;
};

/** @brief Initialize a MetadataCache object and start caching
 *
 * The metadata_cache::cache_init function will initialize a MetadataCache object
 * using the given arguments and store it globally using the given cache_name.
 *
 * Parameters host, port, user, password are used to setup the connection with
 * the metadata server.
 *
 * Cache name given by cache_name can be empty, but must be unique.
 *
 * The parameters connection_timeout and connection_attempts are used when
 * connected to the metadata server.
 *
 * Throws a std::runtime_error when the cache object was already
 * initialized.
 *
 * @param bootstrap_servers The list of metadata servers from.
 * @param user MySQL Metadata username
 * @param password MySQL Metadata password
 * @param ttl The time to live for the cached data
 * @param metadata_replicaset The replicaset that is used to maintain the
 *                            metadata.
 */
void cache_init(const std::vector<mysqlrouter::TCPAddress> &bootstrap_servers,
                const std::string &user, const std::string &password,
                unsigned int ttl, const std::string &metadata_replicaset);

/** @brief Returns list of managed server in a HA replicaset
 *
 * Returns a list of MySQL servers managed by the topology for the given
 * HA replicaset.
 *
 * @param replicaset_name ID of the HA replicaset
 * @return List of ManagedInstance objects
 */
LookupResult lookup_replicaset(const std::string &replicaset_name);

} // namespace metadata_cache

#endif // MYSQLROUTER_METADATA_CACHE_INCLUDED