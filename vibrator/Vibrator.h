/*
 * Copyright (C) 2019 The Android Open Source Project
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

#pragma once

#include <aidl/android/hardware/vibrator/BnVibrator.h>
#include <map>

namespace aidl {
namespace android {
namespace hardware {
namespace vibrator {

const std::string kVibratorLoop       = "/sys/class/leds/vibrator/loop";
const std::string kVibratorIndex       = "/sys/class/leds/vibrator/index";
const std::string kVibratorDuration    = "/sys/class/leds/vibrator/duration";
const std::string kVibratorActivate    = "/sys/class/leds/vibrator/activate";
const std::string kVibratorActivateMode = "/sys/class/leds/vibrator/activate_mode";
const std::string kVibratorStrength    = "/sys/class/leds/vibrator/gain";

// From haptic_hv.h
enum aw_haptic_work_mode {
        AW_STANDBY_MODE = 0,
        AW_RAM_MODE = 1,
        AW_RTP_MODE = 2,
        AW_TRIG_MODE = 3,
        AW_CONT_MODE = 4,
        AW_RAM_LOOP_MODE = 5,
};

static std::map<Effect, std::pair<int32_t, int32_t>> vibEffects = {
    { Effect::CLICK, { 3, 0 } },
    { Effect::DOUBLE_CLICK, { 3, 0 } },
    { Effect::TICK, { 2, 0 } },
    { Effect::TEXTURE_TICK, { 4, 15 } },
    { Effect::HEAVY_CLICK, { 5, 0 } }
};

static std::map<EffectStrength, int32_t> vibStrengths = {
    { EffectStrength::LIGHT, 64 },
    { EffectStrength::MEDIUM, 96 },
    { EffectStrength::STRONG, 128 }
};

class Vibrator : public BnVibrator {
public:
    ndk::ScopedAStatus getCapabilities(int32_t* _aidl_return) override;
    ndk::ScopedAStatus off() override;
    ndk::ScopedAStatus on(int32_t timeoutMs,
                          const std::shared_ptr<IVibratorCallback>& callback) override;
    ndk::ScopedAStatus perform(Effect effect, EffectStrength strength,
                               const std::shared_ptr<IVibratorCallback>& callback,
                               int32_t* _aidl_return) override;
    ndk::ScopedAStatus getSupportedEffects(std::vector<Effect>* _aidl_return) override;
    ndk::ScopedAStatus setAmplitude(float amplitude) override;
    ndk::ScopedAStatus setExternalControl(bool enabled) override;
    ndk::ScopedAStatus getCompositionDelayMax(int32_t* maxDelayMs);
    ndk::ScopedAStatus getCompositionSizeMax(int32_t* maxSize);
    ndk::ScopedAStatus getSupportedPrimitives(std::vector<CompositePrimitive>* supported) override;
    ndk::ScopedAStatus getPrimitiveDuration(CompositePrimitive primitive,
                                            int32_t* durationMs) override;
    ndk::ScopedAStatus compose(const std::vector<CompositeEffect>& composite,
                               const std::shared_ptr<IVibratorCallback>& callback) override;
    ndk::ScopedAStatus getSupportedAlwaysOnEffects(std::vector<Effect>* _aidl_return) override;
    ndk::ScopedAStatus alwaysOnEnable(int32_t id, Effect effect, EffectStrength strength) override;
    ndk::ScopedAStatus alwaysOnDisable(int32_t id) override;
    ndk::ScopedAStatus getResonantFrequency(float *resonantFreqHz) override;
    ndk::ScopedAStatus getQFactor(float *qFactor) override;
    ndk::ScopedAStatus getFrequencyResolution(float *freqResolutionHz) override;
    ndk::ScopedAStatus getFrequencyMinimum(float *freqMinimumHz) override;
    ndk::ScopedAStatus getBandwidthAmplitudeMap(std::vector<float> *_aidl_return) override;
    ndk::ScopedAStatus getPwlePrimitiveDurationMax(int32_t *durationMs) override;
    ndk::ScopedAStatus getPwleCompositionSizeMax(int32_t *maxSize) override;
    ndk::ScopedAStatus getSupportedBraking(std::vector<Braking>* supported) override;
    ndk::ScopedAStatus composePwle(const std::vector<PrimitivePwle> &composite,
                                   const std::shared_ptr<IVibratorCallback> &callback) override;
private:
    static ndk::ScopedAStatus setNode(const std::string path, const int32_t value);
    static ndk::ScopedAStatus setNode(const std::string path, const std::string value);
    static ndk::ScopedAStatus setMode(const int32_t mode);
    static ndk::ScopedAStatus setIndex(const int32_t index);
    static ndk::ScopedAStatus setLoop(const int32_t times);
    static ndk::ScopedAStatus setStrength(const EffectStrength strength);
    static bool exists(const std::string path);
    static int getNode(const std::string path, const int fallback);
    bool mVibratorStrengthSupported;
    int mVibratorStrengthMax;
    ndk::ScopedAStatus activate(const int32_t timeoutMs);
    void msleep(const int32_t msec);
};

}  // namespace vibrator
}  // namespace hardware
}  // namespace android
}  // namespace aidl
