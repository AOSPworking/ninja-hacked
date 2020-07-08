// Copyright 2017 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef NINJA_PROTO_H_
#define NINJA_PROTO_H_

#include <iostream>

// This file and proto.cc implement a minimal write-only protobuf runtime to be
// used with headers generated by misc/generate_proto_header.py.

// Based on https://github.com/google/protobuf/blob/3.0.x/src/google/protobuf/wire_format_lite.h
static inline uint32_t ZigZagEncode32(int32_t n) {
  // Note:  the right-shift must be arithmetic
  return (static_cast<uint32_t>(n) << 1) ^ (n >> 31);
}

static inline uint64_t ZigZagEncode64(int64_t n) {
  // Note:  the right-shift must be arithmetic
  return (static_cast<uint64_t>(n) << 1) ^ (n >> 63);
}

// Based on https://github.com/google/protobuf/blob/3.0.x/src/google/protobuf/io/coded_stream.h
static inline size_t VarintSize32(uint32_t value) {
  if (value < (1 << 7)) {
    return 1;
  } else if (value < (1 << 14)) {
    return 2;
  } else if (value < (1 << 21)) {
    return 3;
  } else if (value < (1 << 28)) {
    return 4;
  } else {
    return 5;
  }
}

static inline size_t VarintSize32SignExtended(int32_t value) {
  if (value < 0) {
    return 10;     // TODO(kenton):  Make this a symbolic constant.
  } else {
    return VarintSize32(static_cast<uint32_t>(value));
  }
}

static inline size_t VarintSize64(uint64_t value) {
  if (value < (1ull << 35)) {
    if (value < (1ull << 7)) {
      return 1;
    } else if (value < (1ull << 14)) {
      return 2;
    } else if (value < (1ull << 21)) {
      return 3;
    } else if (value < (1ull << 28)) {
      return 4;
    } else {
      return 5;
    }
  } else {
    if (value < (1ull << 42)) {
      return 6;
    } else if (value < (1ull << 49)) {
      return 7;
    } else if (value < (1ull << 56)) {
      return 8;
    } else if (value < (1ull << 63)) {
      return 9;
    } else {
      return 10;
    }
  }
}

static inline size_t VarintSizeBool(bool /*value*/) {
  return 1;
}

static inline size_t FixedSize32(uint32_t /*value*/) {
  return 4;
}

static inline size_t FixedSize64(uint64_t /*value*/) {
  return 8;
}

static inline size_t StringSize(const std::string& value) {
  return VarintSize32(value.size()) + value.size();
}

void WriteVarint32(std::ostream* output, int number, uint32_t value);
void WriteVarint32NoTag(std::ostream* output, uint32_t value);
void WriteVarint32SignExtended(std::ostream* output, int number, int32_t value);
void WriteVarint64(std::ostream* output, int number, uint64_t value);
void WriteFixed32(std::ostream* output, int number, uint32_t value);
void WriteFixed64(std::ostream* output, int number, uint64_t value);
void WriteString(std::ostream* output, int number, const std::string& value);
void WriteLengthDelimited(std::ostream* output, int number, size_t size);

#endif