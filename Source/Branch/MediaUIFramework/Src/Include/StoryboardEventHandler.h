/*!
* @file StoryboardEventHandler.h 
* 
* @brief This file defines the event handler of storyboard.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _STORYBOARDEVENTHANDLER_H_
#define _STORYBOARDEVENTHANDLER_H_

#include "Common.h"
#include <UIAnimation.h>
#include "IAnimationListener.h"
#include "Animation.h"

/*!
* @brief This class defines the event handler of storyboard.
*/
class CLASS_DECLSPEC StoryboardEventHandler : public IUIAnimationStoryboardEventHandler
{
public:

    /*!
    * @brief The constructor.
    */
    StoryboardEventHandler();

    /*!
    * @brief Add the animation listener.
    *
    * @param pListener     [I/ ] The listener.
    * @param pAnimation    [I/ ] The animation.
    */
    void AddAnimationListener(IAnimationListener *pListener, Animation *pAnimation);

    /*!
    * @brief Remove the animation listener.
    *
    * @param pListener     [I/ ] The listener.
    */
    void RemoveAnimationListener(IAnimationListener *pListener);

    /*!
    * @brief Called when the status of storyboard changed.
    *
    * @param storyboard          [I/ ] The storyboard.
    * @param newStatus           [I/ ] The new status of the storyboard.
    * @param previousStatus      [I/ ] The previous status of the storyboard.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT __stdcall OnStoryboardStatusChanged(IUIAnimationStoryboard *storyboard,
                                                UI_ANIMATION_STORYBOARD_STATUS newStatus,
                                                UI_ANIMATION_STORYBOARD_STATUS previousStatus);

    /*!
    * @brief Called to update the storyboard.
    *
    * @param storyboard          [I/ ] The storyboard.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT __stdcall OnStoryboardUpdated(IUIAnimationStoryboard *storyboard);

    /*!
    * @brief Called to decide to query the interface or not.
    *
    * @param riid          [I/ ] Identifier of the interface being queried.
    * @param ppvObject     [I/O] Pointer to a buffer that receives a pointer to the object whose 
    *                            interface is queried or NULL when an interface is not supported.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT __stdcall QueryInterface( 
        REFIID riid,
        __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);

    /*!
    * @brief Called to add one to the animation count.
    *
    * @return The count of existing animation.
    */
    virtual ULONG __stdcall AddRef();

    /*!
    * @brief Called to release the event handler of storyboard.
    *
    * @return The count of existing animation.
    */
    virtual ULONG __stdcall Release();

private:

    /*!
    * @brief The default destructor.
    */
    virtual ~StoryboardEventHandler();

private:

    Animation         *m_pAnimation;            // The animation.
    LONG               m_lRefCount;             // The animation count.
    vector<IAnimationListener*> m_vecListener;  // The animation listener.
};

#endif // _STORYBOARDEVENTHANDLER_H_
#endif // __cplusplus
