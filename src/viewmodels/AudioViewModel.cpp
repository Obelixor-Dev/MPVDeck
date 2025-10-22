#include "AudioViewModel.h"
#include "Constants.h"
#include <algorithm>

using namespace MPVDeck;
using namespace MPVDeck::Constants;

AudioViewModel::AudioViewModel(QObject *parent) : QObject(parent) {}

auto AudioViewModel::mute() const -> bool { return m_mute; }

void AudioViewModel::setMute(bool mute) {
  if (m_mute != mute) {
    m_mute = mute;
    emit muteChanged(m_mute);
  }
}

auto AudioViewModel::volume() const -> int { return m_volume; }

void AudioViewModel::setVolume(int volume) {
  volume = std::clamp(volume, 0, MPVDeck::Constants::MAX_VOLUME);
  if (m_volume != volume) {
    m_volume = volume;
    emit volumeChanged(m_volume);
  }
}

auto AudioViewModel::audioDevice() const -> QString { return m_audioDevice; }

void AudioViewModel::setAudioDevice(const QString &audioDevice) {
  if (m_audioDevice != audioDevice) {
    m_audioDevice = audioDevice;
    emit audioDeviceChanged(m_audioDevice);
  }
}

auto AudioViewModel::audioChannels() const -> QString {
  return m_audioChannels;
}

void AudioViewModel::setAudioChannels(const QString &audioChannels) {
  if (m_audioChannels != audioChannels) {
    m_audioChannels = audioChannels;
    emit audioChannelsChanged(m_audioChannels);
  }
}

auto AudioViewModel::audioDelay() const -> double { return m_audioDelay; }

void AudioViewModel::setAudioDelay(double audioDelay) {
  audioDelay = std::clamp(audioDelay, MIN_AUDIO_DELAY, MAX_AUDIO_DELAY);
  if (m_audioDelay != audioDelay) {
    m_audioDelay = audioDelay;
    emit audioDelayChanged(m_audioDelay);
  }
}

auto AudioViewModel::audioNormalizeDownmix() const -> bool {
  return m_audioNormalizeDownmix;
}

void AudioViewModel::setAudioNormalizeDownmix(bool audioNormalizeDownmix) {
  if (m_audioNormalizeDownmix != audioNormalizeDownmix) {
    m_audioNormalizeDownmix = audioNormalizeDownmix;
    emit audioNormalizeDownmixChanged(audioNormalizeDownmix);
  }
}
