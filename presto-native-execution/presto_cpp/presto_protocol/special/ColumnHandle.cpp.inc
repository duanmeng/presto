/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
namespace facebook::presto::protocol {
void to_json(json& j, const std::shared_ptr<ColumnHandle>& p) {
  if (p == nullptr) {
    return;
  }
  String type = p->_type;

  if (getConnectorKey(type) == "hive") {
    j = *std::static_pointer_cast<HiveColumnHandle>(p);
    return;
  }

  if (getConnectorKey(type) == "hive-iceberg") {
    j = *std::static_pointer_cast<IcebergColumnHandle>(p);
    return;
  }

  if (getConnectorKey(type) == "tpch") {
    j = *std::static_pointer_cast<TpchColumnHandle>(p);
    return;
  }

  throw TypeError(type + " no abstract type ColumnHandle ");
}

void from_json(const json& j, std::shared_ptr<ColumnHandle>& p) {
  String type;
  try {
    type = p->getSubclassKey(j);
  } catch (json::parse_error& e) {
    throw ParseError(std::string(e.what()) + " ColumnHandle  ColumnHandle");
  }

  if (getConnectorKey(type) == "hive") {
    std::shared_ptr<HiveColumnHandle> k = std::make_shared<HiveColumnHandle>();
    j.get_to(*k);
    p = std::static_pointer_cast<ColumnHandle>(k);
    return;
  }

  if (getConnectorKey(type) == "hive-iceberg") {
    std::shared_ptr<IcebergColumnHandle> k =
        std::make_shared<IcebergColumnHandle>();
    j.get_to(*k);
    p = std::static_pointer_cast<ColumnHandle>(k);
    return;
  }

  if (getConnectorKey(type) == "tpch") {
    std::shared_ptr<TpchColumnHandle> k = std::make_shared<TpchColumnHandle>();
    j.get_to(*k);
    p = std::static_pointer_cast<ColumnHandle>(k);
    return;
  }

  throw TypeError(type + " no abstract type ColumnHandle ");
}
} // namespace facebook::presto::protocol
