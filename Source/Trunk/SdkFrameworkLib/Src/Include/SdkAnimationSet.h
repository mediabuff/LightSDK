/*!
* @file SdkAnimationSet.h 
* 
* @brief This file defines the functions of animation set.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _SDKANIMATIONSET_H_
#define _SDKANIMATIONSET_H_

#include <list>
#include "SdkAnimation.h"

BEGIN_NAMESPACE_ANIMATION

/*!
* @brief This class defines the functions of animation set.
*/
class CLASS_DECLSPEC SdkAnimationSet : public SdkAnimation
{
public:

    /*!
    * @brief The constructor.
    */
    SdkAnimationSet();

    /*!
    * @brief The default destructor.
    */
    virtual ~SdkAnimationSet();

    /*!
    * @brief Add a animation to animation-set.
    *
    * @param pAnimation [I/ ]   The added animation.
    */
    virtual void AddAnimation(SdkAnimation *pAnimation);

    /*!
    * @brief Get the duration of the animation.
    *
    * @param pDuration      [ /O] The duration.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetDuration(OUT DOUBLE *pDuration);

    /*!
    * @brief Get the transform of the animation.
    *
    * @param pTransform     [ /O] The transform information.
    * @pram  bUpdate        [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetTransform(OUT LPTRANSFORMINFO pTransform,
                                 IN  BOOL bUpdate = TRUE);

    /*!
    * @brief Get the information of the scale animation.
    *
    * @param pppVarible             [ /O] The variable information of the animation.
    * @param pppTransition          [ /O] The transition information of the animation.
    * @param pTransitionCount       [ /O] The count of the variable.
    * @param bUpdate                [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetAnimationInfo(OUT  IUIAnimationVariable ***pppVarible,
                                     OUT  IUIAnimationTransition ***pppTransition,
                                     OUT  UINT32 *pTransitionCount,
                                     BOOL bUpdate = TRUE);

protected:

    /*!
    * @brief Initialize the storyboard of animation.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT InitStoryboard();

    /*!
    * @brief Clear the animation.
    */
    void ClearAnimation();

    /*!
    * @brief Clear animation-info.
    */
    void ClearAnimationInfo();

    /*!
    * @brief Add animation-info to list.
    *
    * @param pInfo  [I/ ]   The animation information.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT AddAnimation(const LPANIMATIONINFO2 pInfo);

private:

    list<SdkAnimation*>         m_listAnimation;        // The animation list.
    list<LPANIMATIONINFO2>      m_listAniamtionInfo;    // The animation-info list.
};

END_NAMESPACE_ANIMATION

#endif // _SDKANIMATIONSET_H_
#endif // __cplusplus
