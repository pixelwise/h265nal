/*
 *  Copyright (c) Facebook, Inc. and its affiliates.
 */

// This file was auto-generated using fuzz/converter.py from
// h265_vps_parser_unittest.cc.
// Do not edit directly.

#include "h265_vps_parser.h"
#include "h265_common.h"
#include "rtc_common.h"


// libfuzzer infra to test the fuzz target
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  {
  auto vps = h265nal::H265VpsParser::ParseVps(data, size);
  }
  return 0;
}
