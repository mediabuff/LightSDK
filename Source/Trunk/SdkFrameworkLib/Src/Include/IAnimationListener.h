/*!
* @file IAnimationListener.h 
* 
* @brief This file defines the event interface of animation.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _IANIMATIONLISTENER_H_
#define _IANIMATIONLISTENER_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_CALLBACK

/*!
* @brief The interface defines the common event handler for animation.
*/
class IAnimationListener
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~IAnimationListener() {}

    /*!
    * @brief This method can be called when the animation is about to start.
    */
    virtual void OnAnimationStart(SdkAnimation *pAnimation) = 0;

    /*!
    * @brief This method can be called when the animation is about to play.
    */
    virtual void OnAnimationPlaying(SdkAnimation *pAnimation) = 0;

    /*!
    * @brief This method can be called when the animation has ended.
    */
    virtual void OnAnimationEnd(SdkAnimation *pAnimation) = 0;
};

END_NAMESPACE_CALLBACK

#endif // _IANIMATIONLISTENER_H_
#endif //__cplusplus
