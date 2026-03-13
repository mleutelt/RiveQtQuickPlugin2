#pragma once

#include <QList>
#include <QObject>
#include <QUrl>

#include "rive/file_asset_loader.hpp"

class RiveBackendBridge;

class RiveFileAssetLoader : public rive::FileAssetLoader {
  public:
  RiveFileAssetLoader(QUrl baseUrl,
    RiveBackendBridge* bridge);

  bool loadContents(rive::FileAsset& asset,
    rive::Span<const uint8_t> inBandBytes,
    rive::Factory* factory) override;

  private:
  QList<QUrl> resolveUrls(const rive::FileAsset& asset) const;
  bool isLocalLikeBaseUrl() const;

  QUrl m_baseUrl;
  RiveBackendBridge* m_bridge { nullptr };
};
