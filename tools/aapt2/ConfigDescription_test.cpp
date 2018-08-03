/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ConfigDescription.h"

#include <string>

#include "androidfw/StringPiece.h"

#include "SdkConstants.h"
#include "test/Test.h"

using android::StringPiece;

namespace aapt {

static ::testing::AssertionResult TestParse(
    const StringPiece& input, ConfigDescription* config = nullptr) {
  if (ConfigDescription::Parse(input, config)) {
    return ::testing::AssertionSuccess() << input << " was successfully parsed";
  }
  return ::testing::AssertionFailure() << input << " could not be parsed";
}

TEST(ConfigDescriptionTest, ParseFailWhenQualifiersAreOutOfOrder) {
  EXPECT_FALSE(TestParse("en-sw600dp-ldrtl"));
  EXPECT_FALSE(TestParse("land-en"));
  EXPECT_FALSE(TestParse("hdpi-320dpi"));
}

TEST(ConfigDescriptionTest, ParseFailWhenQualifiersAreNotMatched) {
  EXPECT_FALSE(TestParse("en-sw600dp-ILLEGAL"));
}

TEST(ConfigDescriptionTest, ParseFailWhenQualifiersHaveTrailingDash) {
  EXPECT_FALSE(TestParse("en-sw600dp-land-"));
}

TEST(ConfigDescriptionTest, ParseBasicQualifiers) {
  ConfigDescription config;
  EXPECT_TRUE(TestParse("", &config));
  EXPECT_EQ(std::string(""), config.toString().string());

  EXPECT_TRUE(TestParse("fr-land", &config));
  EXPECT_EQ(std::string("fr-land"), config.toString().string());

  EXPECT_TRUE(
      TestParse("mcc310-pl-sw720dp-normal-long-port-night-"
                "xhdpi-keyssoft-qwerty-navexposed-nonav",
                &config));
  EXPECT_EQ(std::string("mcc310-pl-sw720dp-normal-long-port-night-"
                        "xhdpi-keyssoft-qwerty-navexposed-nonav-v13"),
            config.toString().string());
}

TEST(ConfigDescriptionTest, ParseLocales) {
  ConfigDescription config;
  EXPECT_TRUE(TestParse("en-rUS", &config));
  EXPECT_EQ(std::string("en-rUS"), config.toString().string());
}

TEST(ConfigDescriptionTest, ParseQualifierAddedInApi13) {
  ConfigDescription config;
  EXPECT_TRUE(TestParse("sw600dp", &config));
  EXPECT_EQ(std::string("sw600dp-v13"), config.toString().string());

  EXPECT_TRUE(TestParse("sw600dp-v8", &config));
  EXPECT_EQ(std::string("sw600dp-v13"), config.toString().string());
}

TEST(ConfigDescriptionTest, ParseCarAttribute) {
  ConfigDescription config;
  EXPECT_TRUE(TestParse("car", &config));
  EXPECT_EQ(android::ResTable_config::UI_MODE_TYPE_CAR, config.uiMode);
}

TEST(ConfigDescriptionTest, TestParsingRoundQualifier) {
  ConfigDescription config;
  EXPECT_TRUE(TestParse("round", &config));
  EXPECT_EQ(android::ResTable_config::SCREENROUND_YES,
            config.screenLayout2 & android::ResTable_config::MASK_SCREENROUND);
  EXPECT_EQ(SDK_MARSHMALLOW, config.sdkVersion);
  EXPECT_EQ(std::string("round-v23"), config.toString().string());

  EXPECT_TRUE(TestParse("notround", &config));
  EXPECT_EQ(android::ResTable_config::SCREENROUND_NO,
            config.screenLayout2 & android::ResTable_config::MASK_SCREENROUND);
  EXPECT_EQ(SDK_MARSHMALLOW, config.sdkVersion);
  EXPECT_EQ(std::string("notround-v23"), config.toString().string());
}

TEST(ConfigDescriptionTest, TestWideColorGamutQualifier) {
  ConfigDescription config;
  EXPECT_TRUE(TestParse("widecg", &config));
  EXPECT_EQ(android::ResTable_config::WIDE_COLOR_GAMUT_YES,
            config.colorMode & android::ResTable_config::MASK_WIDE_COLOR_GAMUT);
  EXPECT_EQ(SDK_O, config.sdkVersion);
  EXPECT_EQ(std::string("widecg-v26"), config.toString().string());

  EXPECT_TRUE(TestParse("nowidecg", &config));
  EXPECT_EQ(android::ResTable_config::WIDE_COLOR_GAMUT_NO,
            config.colorMode & android::ResTable_config::MASK_WIDE_COLOR_GAMUT);
  EXPECT_EQ(SDK_O, config.sdkVersion);
  EXPECT_EQ(std::string("nowidecg-v26"), config.toString().string());
}

TEST(ConfigDescriptionTest, TestHdrQualifier) {
  ConfigDescription config;
  EXPECT_TRUE(TestParse("highdr", &config));
  EXPECT_EQ(android::ResTable_config::HDR_YES,
            config.colorMode & android::ResTable_config::MASK_HDR);
  EXPECT_EQ(SDK_O, config.sdkVersion);
  EXPECT_EQ(std::string("highdr-v26"), config.toString().string());

  EXPECT_TRUE(TestParse("lowdr", &config));
  EXPECT_EQ(android::ResTable_config::HDR_NO,
            config.colorMode & android::ResTable_config::MASK_HDR);
  EXPECT_EQ(SDK_O, config.sdkVersion);
  EXPECT_EQ(std::string("lowdr-v26"), config.toString().string());
}

TEST(ConfigDescriptionTest, ParseVrAttribute) {
  ConfigDescription config;
  EXPECT_TRUE(TestParse("vrheadset", &config));
  EXPECT_EQ(android::ResTable_config::UI_MODE_TYPE_VR_HEADSET, config.uiMode);
  EXPECT_EQ(SDK_O, config.sdkVersion);
  EXPECT_EQ(std::string("vrheadset-v26"), config.toString().string());
}

}  // namespace aapt