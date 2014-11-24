/*
 * Copyright 2014 Google Inc. All rights reserved.
 *
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

#ifndef FRUIT_BINDING_DATA_DEFN_H
#define FRUIT_BINDING_DATA_DEFN_H

#include "fruit_internal_forward_decls.h"

#include <cassert>

namespace fruit {
namespace impl {

inline BindingData::BindingData(create_t create, const BindingDeps* deps)
: deps(deps), p(reinterpret_cast<void*>(create)) {
}
  
inline BindingData::BindingData(object_t object) 
: deps(nullptr), p(reinterpret_cast<void*>(object)) {
}

inline bool BindingData::isCreated() const {
  return deps == nullptr;
}

inline const BindingDeps* BindingData::getDeps() const {
  assert(deps != nullptr);
  return deps;
}

inline BindingData::create_t BindingData::getCreate() const {
  assert(!isCreated());
  return reinterpret_cast<create_t>(p);
}

inline BindingData::object_t BindingData::getObject() const {
  assert(isCreated());
  return reinterpret_cast<object_t>(p);
}

inline bool BindingData::operator==(const BindingData& other) const {
  return std::tie(deps, p)
      == std::tie(other.deps, other.p);
}

inline bool BindingData::operator<(const BindingData& other) const {
  // `destroy' is intentionally not compared.
  // If the others are equal it should also be equal. If it isn't, the two BindingData structs
  // are still equivalent because they produce the same injected object.
  return std::tie(deps, p)
       < std::tie(other.deps, other.p);
}

inline NormalizedBindingData::NormalizedBindingData(BindingData bindingData) {
  if (bindingData.isCreated()) {
    *this = NormalizedBindingData{bindingData.getObject()};
  } else {
    *this = NormalizedBindingData{bindingData.getCreate()};
  }
}

inline NormalizedBindingData::NormalizedBindingData(BindingData::create_t create)
: p(reinterpret_cast<void*>(create)) {
}
  
inline NormalizedBindingData::NormalizedBindingData(BindingData::object_t object) 
: p(reinterpret_cast<void*>(object)) {
}

inline BindingData::create_t NormalizedBindingData::getCreate() const {
  return reinterpret_cast<BindingData::create_t>(p);
}

inline BindingData::object_t NormalizedBindingData::getObject() const {
  return reinterpret_cast<BindingData::object_t>(p);
}

inline void NormalizedBindingData::create(InjectorStorage& storage,
                                          SemistaticGraph<TypeId, NormalizedBindingData>::edge_iterator depsBegin) {
  BindingData::object_t obj = getCreate()(storage, depsBegin);
  p = reinterpret_cast<void*>(obj);
}

inline bool NormalizedBindingData::operator==(const NormalizedBindingData& other) const {
  return std::tie(p)
      == std::tie(other.p);
}

inline bool NormalizedBindingData::operator<(const NormalizedBindingData& other) const {
  // `destroy' is intentionally not compared.
  // If the others are equal it should also be equal. If it isn't, the two NormalizedBindingData structs
  // are still equivalent because they produce the same injected object.
  return std::tie(p)
       < std::tie(other.p);
}

inline MultibindingData::MultibindingData(create_t create, const BindingDeps* deps,
                                          getObjectVector_t getObjectVector)
  : create(create), deps(deps), getObjectVector(getObjectVector) {
}
  
inline MultibindingData::MultibindingData(object_t object, getObjectVector_t getObjectVector)
  : object(object), getObjectVector(getObjectVector) {
}


} // namespace impl
} // namespace fruit


#endif // FRUIT_BINDING_DATA_DEFN_H
