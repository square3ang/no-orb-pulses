#include <Geode/Geode.hpp>
#include <Geode/loader/ModEvent.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

namespace {
    constexpr char const* kSizeSetting = "size";
    constexpr float kDefaultPulseSize = 0.7f;

    float s_pulseSize = kDefaultPulseSize;
    FMODAudioEngine* s_audioEngine = nullptr;

    void applyPulseSize(AudioEffectsLayer* layer) {
        if (!s_audioEngine) {
            s_audioEngine = FMODAudioEngine::get();
        }

        if (s_audioEngine) {
            s_audioEngine->m_pulse1 = s_pulseSize;
        }
        if (layer) {
            layer->m_audioScale = s_pulseSize;
        }
    }
}

$on_mod(Loaded) {
    s_pulseSize = static_cast<float>(Mod::get()->getSettingValue<double>(kSizeSetting));

    listenForSettingChanges<double>(kSizeSetting, +[](double size) {
        s_pulseSize = static_cast<float>(size);
    });
}

class $modify(NoOrbPulsesPlayLayer, PlayLayer) {
    void updateVisibility(float dt) {
        applyPulseSize(m_audioEffectsLayer);
        PlayLayer::updateVisibility(dt);
    }
};
