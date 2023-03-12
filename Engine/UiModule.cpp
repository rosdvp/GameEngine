#include "pch.h"
#include "UiModule.h"

#include "Logger.h"
#include "TransformComp.h"
#include "UiTextComp.h"

using namespace BlahEngine;

UiModule::UiModule():
	_ecs(nullptr),
	_textsFilter(nullptr) {}

UiModule::~UiModule()
{
    Logger::Debug("ui", "destroyed");
}

void UiModule::Init(IDXGISwapChain* swapChain, EcsCore* ecs)
{
    _ecs = ecs;
    _textsFilter = _ecs->GetFilter(FilterMask().Inc<TransformComp>().Inc<UiTextComp>());

    HRESULT result = S_OK;

    auto d2FactoryOptions = D2D1_FACTORY_OPTIONS();
    d2FactoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
    result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2FactoryOptions, _d2Factory.GetAddressOf());
    if (FAILED(result))
        throw std::exception("failed to create d2Factory");

    result = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(_dwFactory.GetAddressOf()));
    if (FAILED(result))
        throw std::exception("failed to create dwFactory");

    // Create a Direct2D render target.

    IDXGISurface* backBuffer = nullptr;
    result = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    if (FAILED(result))
        throw std::exception("failed to get buffer");

    auto d2RenderTargetProps = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT, 
        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
        0,
        0);
    result = _d2Factory->CreateDxgiSurfaceRenderTarget(
        backBuffer,
        &d2RenderTargetProps,
        _renderTarget.GetAddressOf());
    if (FAILED(result))
        throw std::exception("failed to create render target");
    
    result = _renderTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::White),
        _brushBlack.GetAddressOf());
    if (FAILED(result))
        throw std::exception("failed to create brush");

    Logger::Debug("ui", "initialized");
}

void UiModule::DrawFrame()
{
    _renderTarget->BeginDraw();

    _renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

    //_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

    D2D1_SIZE_F renderTargetSize = _renderTarget->GetSize();

    for (auto entity : *_textsFilter)
    {
        auto& textComp = _ecs->GetComp<UiTextComp>(entity);

        if (textComp.TextFormat.Get() == nullptr)
            SetupText(textComp);

        auto& tf = _ecs->GetComp<TransformComp>(entity);

        

        auto rect = D2D1::RectF(
            renderTargetSize.width / 2 + tf.Pos.X - tf.Scale.X / 2,
            renderTargetSize.height / 2 + tf.Pos.Y + tf.Scale.Y / 2,
            renderTargetSize.width / 2 + tf.Pos.X + tf.Scale.X / 2,
            renderTargetSize.height / 2 + tf.Pos.Y - tf.Scale.Y / 2
        );

        _renderTarget->DrawText(
            textComp.Text.data(),
            textComp.Text.length(),
            textComp.TextFormat.Get(),
            rect,
            _brushBlack.Get());
    }

    _renderTarget->EndDraw();
}

void UiModule::SetupText(UiTextComp& textComp)
{
    HRESULT result = _dwFactory->CreateTextFormat(
        textComp.Font.data(),
        nullptr,
        textComp.IsBold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        textComp.FontSize,
        L"", //locale
        textComp.TextFormat.GetAddressOf()
    );
    if (FAILED(result))
        throw std::exception("failed to create text layout");

    textComp.TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    textComp.TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}
