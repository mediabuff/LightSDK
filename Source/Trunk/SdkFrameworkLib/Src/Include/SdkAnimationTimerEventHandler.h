/*!
* @file SdkAnimationTimerEventHandler.h 
* 
* @brief This file defines the event handler of animation timer.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Qiao LiYun
* @date 2011/02/15
*/

#ifdef __cplusplus
#ifndef _SDKANIMATIONTIMEREVENTHANDLER_H_
#define _SDKANIMATIONTIMEREVENTHANDLER_H_

#include "SdkCommonInclude.h"
#include "SdkAnimation.h"

BEGIN_NAMESPACE_ANIMATION

/*!
* @brief This class defines the event handler of animation timer.
*/
class CLASS_DECLSPEC SdkAnimationTimerEventHandler : public IUIAnimationTimerEventHandler
{
public:

    /*!
    * @brief The constructor.
    */
    SdkAnimationTimerEventHandler();

    /*!
    * @brief Add the listener of animation timer.
    *
    * @param pListener     [I/ ] The listener.
    * @param pAnimation    [I/ ] The animation.
    */
    void AddListener(IAnimationTimerListener *pListener, SdkAnimation *pAnimation);

    /*!
    * @brief Remove the listener of animation timer.
    *
    * @param pListener     [I/ ] The listener.
    */
    void RemoveListener(IAnimationTimerListener *pListener);

    /*!
    * @brief Called when the timer is about to update.
    * 
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual IFACEMETHODIMP OnPreUpdate();

    /*!
    * @brief Called when the timer update.
    * 
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual IFACEMETHODIMP OnPostUpdate();

    /*!
    * @brief Called when the rendering slow.
    * 
    * @param fps            [I/ ] The fps value.
    * 
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual IFACEMETHODIMP OnRenderingTooSlow(UINT32 fps);

    /*!
    * @brief Called to decide to query the interface or not.
    *
    * @param riid          [I/ ] Identifier of the interface being queried.
    * @param ppvObject     [I/O] Pointer to a buffer that receives a pointer to the object whose 
    *                            interface is queried or NULL when an interface is not supported.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual IFACEMETHODIMP QueryInterface( 
        REFIID riid,
        __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);

    /*!
    * @brief Called to add one to the animation count.
    *
    * @return The count of existing animation.
    */
    virtual IFACEMETHODIMP_(ULONG) AddRef();

    /*!
    * @brief Called to release the event handler of storyboard.
    *
    * @return The count of existing animation.
    */
    virtual IFACEMETHODIMP_(ULONG) Release();

private:

    /*!
    * @brief The default destructor.
    */
    virtual ~SdkAnimationTimerEventHandler();

private:

    SdkAnimation                    *m_pAnimation;                // The animation.
    LONG                             m_lRefCount;                 // The animation count.
    vector<IAnimationTimerListener*> m_vecAnimationTimerListener; // The animation timer listener.
};

END_NAMESPACE_ANIMATION

#endif // _SDKANIMATIONTIMEREVENTHANDLER_H_
#endif // __cplusplus
