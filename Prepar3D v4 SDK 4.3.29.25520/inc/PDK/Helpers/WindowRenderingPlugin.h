#include "IWindowPluginSystem.h"
#include "IRenderingPluginSystem.h"
#include "IUnknownHelper.h"

namespace P3D
{
    class WindowRenderingPlugin : public IWindowPluginV400, public IRenderingPluginV400
    {
    public:
        WindowRenderingPlugin() : m_RefCount(1)
        {

        }

        virtual ~WindowRenderingPlugin()
        {

        }

        // Window Plugin Interface
        virtual void OnAdd(IWindowV400* pWindow, ICameraSystemV400* pCamera) override {}
        virtual void OnRemove(IWindowV400* pWindow, ICameraSystemV400* pCamera) override {}
        virtual void OnPreCameraUpdate(IWindowV400* pWindow, ICameraSystemV400* pCamera)  override {}
        virtual void OnPostCameraUpdate(IWindowV400* pWindow, ICameraSystemV400* pCamera)  override {}
        virtual void OnViewChange(IWindowV400* pWindow, ICameraSystemV400* pCamera)  override {}
        virtual void OnClose(IWindowV400* pWindow, ICameraSystemV400* pCamera)  override {}
        virtual bool OnUserInput(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam) override { return false; };
        
        // Rendering plugin interface
        virtual void PreRender(IRenderDataV400* pRenderData)
        {
            // only want Render() called during the default render pass
            mRenderFlags.RenderingIsEnabled = (pRenderData->GetRenderPass() == RenderPassDefault);
            pRenderData->SetRenderFlags(mRenderFlags);
        }

        virtual void Render(IRenderDataV400* pRenderData) abstract;
        virtual bool HasUpdate() { return true; }
        virtual bool RequiredDoubleBuffer() { return false; }
        virtual bool IsPreVc() { return false; }
        virtual void OnAdd(IWindowV400* pWindow, ICameraSystemV400* pCamera) override {}
        virtual void OnRemove(IWindowV400* pWindow, ICameraSystemV400* pCamera) override {}

    protected:
        PdkRenderFlags mRenderFlags;
        DEFAULT_REFCOUNT_INLINE_IMPL()
        DEFAULT_IUNKNOWN_QI_INLINE_IMPL(IWindowPluginV400, IID_IWindowPluginV400)
    };
}