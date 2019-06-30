#pragma once 

#include "IWindowPluginSystem.h"
#include "IUnknownHelper.h"

namespace P3D
{
    /** @addtogroup windows */ /** @{ */

    /**
    * Base implementation of the IWindowPluginV400 which defines
    * default implementations of all IWindowPluginV400 functions and
    * a default IUnknown implementation.
    */
    class WindowPlugin : public IWindowPluginV400
    {
    public:
        WindowPlugin() : m_RefCount(1)
        {

        }

        virtual ~WindowPlugin()
        {

        }

        virtual void OnAdd(IWindowV400* pWindow, ICameraSystemV400* pCamera) override {}
        virtual void OnRemove(IWindowV400* pWindow, ICameraSystemV400* pCamera) override {}
        virtual void OnPreCameraUpdate(IWindowV400* pWindow, ICameraSystemV400* pCamera)  override {}
        virtual void OnPostCameraUpdate(IWindowV400* pWindow, ICameraSystemV400* pCamera)  override {}
        virtual void OnViewChange(IWindowV400* pWindow, ICameraSystemV400* pCamera)  override {}
        virtual void OnClose(IWindowV400* pWindow, ICameraSystemV400* pCamera)  override {}
        virtual bool OnUserInput(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam) override { return false; };

        DEFAULT_REFCOUNT_INLINE_IMPL()
        DEFAULT_IUNKNOWN_QI_INLINE_IMPL(IWindowPluginV400, IID_IWindowPluginV400)
    };
    /** @} */
}