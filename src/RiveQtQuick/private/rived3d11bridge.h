#pragma once

#include "rivebackendbridge.h"

#ifdef Q_OS_WIN

#include <memory>

#include <d3d11.h>
#include <wrl/client.h>

#include "rive/renderer/d3d11/render_context_d3d_impl.hpp"
#include "rive/renderer/render_context.hpp"

namespace rive::gpu {
class RenderContext;
} // namespace rive::gpu

class RiveD3D11Bridge : public RiveBackendBridge {
  public:
  QSGRendererInterface::GraphicsApi api() const override;
  bool syncPresentation(QQuickWindow* window,
    const QSize& pixelSize) override;
  QRhiTexture* outputTexture() const override;
  bool prepareFrame(QQuickWindow* window,
    QRhiCommandBuffer* commandBuffer) override;
  rive::Factory* factory() const override;
  rive::rcp<rive::RenderImage> createRenderImage(const QImage& image) override;
  bool render(rive::ArtboardInstance* artboard,
    rive::Scene* scene,
    const rive::Mat2D& transform) override;
  void release() override;

  private:
  struct SharedContext;

  bool ensureContext(QQuickWindow* window);
  bool updateRenderTarget();

  QQuickWindow* m_window { nullptr };
  QRhiTexture* m_outputTexture { nullptr };
  QSize m_outputPixelSize;
  std::shared_ptr<SharedContext> m_sharedContext;
  rive::rcp<rive::gpu::RenderTargetD3D> m_renderTarget;
  uint64_t m_frameNumber { 0 };
};

#endif
