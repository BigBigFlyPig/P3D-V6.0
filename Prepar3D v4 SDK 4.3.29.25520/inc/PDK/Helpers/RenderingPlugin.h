#pragma once

#include "IRenderingPluginSystem.h"
#include "IUnknownHelper.h"
namespace P3D
{
    /** @addtogroup rendering */ /** @{ */

   /**
    * Base implementation of the IRenderingPluginV400 which defines
    * default implementations of all IRenderingPluginV400 functions and
    * a default IUnknown implementation.
    */
    class RenderingPlugin : public IRenderingPluginV400
    {
    public:
        RenderingPlugin() : m_RefCount(1) {  }

        virtual ~RenderingPlugin() {  }

        /**
        *  Enables rendering if the current render pass is RenderPassDefault
        */ 
        virtual void PreRender(IRenderDataV400* pRenderData)
        {
            // only want Render() called during the default render pass
            mRenderFlags.RenderingIsEnabled = (pRenderData->GetRenderPass() == RenderPassDefault);
            pRenderData->SetRenderFlags(mRenderFlags);
        }

        virtual void Render(IRenderDataV400* pRenderData) abstract;


        DEFAULT_REFCOUNT_INLINE_IMPL()
        DEFAULT_IUNKNOWN_QI_INLINE_IMPL(IRenderingPluginV400, IID_IRenderingPluginV400)

    protected:
        PdkRenderFlags mRenderFlags;
    };
    /** @} */ // end of group
}