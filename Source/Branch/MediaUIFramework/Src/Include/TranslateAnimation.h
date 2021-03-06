/*!
* @file TranslateAnimation.h 
* 
* @brief This file defines the functions of translate animation.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _TRANSLATEANIMATION_H_
#define _TRANSLATEANIMATION_H_

#include "Animation.h"

/*!
* @brief This class implements the function of the translate animation.
*/
class CLASS_DECLSPEC TranslateAnimation : public Animation
{
public:

    /*!
    * @brief The constructor.
    */
    TranslateAnimation();

    /*!
    * @brief The default destructor.
    */
    virtual ~TranslateAnimation();

    /*!
    * @brief Set the information of the translate animation in X axis.
    *
    * @param dFromX             [I/ ] The start position at the X direction.
    * @param dToX               [I/ ] The end position at the X direction.
    * @param dDuration          [I/ ] the duration of the translate animation.
    * @param accelerationRatio  [I/ ] The accelerate ratio of the translate animation.
    * @param decelerationRatio  [I/ ] The decelerate ratio of the translate animation.
    */
    VOID SetTranslateXInfo(DOUBLE dFormX,
                           DOUBLE dToX,
                           DOUBLE dDuration = 1.0,
                           DOUBLE accelerationRatio = 0.5,
                           DOUBLE decelerationRatio = 0.5);

    /*!
    * @brief Set the information of the translate animation in Y axis.
    *
    * @param dFromY             [I/ ] The start position at the Y direction.
    * @param dToY               [I/ ] The end position at the Y direction.
    * @param dDuration          [I/ ] the duration of the translate animation.
    * @param accelerationRatio  [I/ ] The accelerate ratio of the translate animation.
    * @param decelerationRatio  [I/ ] The decelerate ratio of the translate animation.
    */
    VOID SetTranslateYInfo(DOUBLE dFormY,
                           DOUBLE dToY,
                           DOUBLE dDuration = 1.0,
                           DOUBLE accelerationRatio = 0.5,
                           DOUBLE decelerationRatio = 0.5);

     /*!
    * @brief Get the value of the variable.
    *
    * @param pValueX     [ /O] The value of X variable.
    * @param pValueY     [ /O] The value of Y variable.
    * @param bUpdate     [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT GetValue(OUT DOUBLE  *pValueX,
                     OUT DOUBLE  *pValueY,
                     BOOL        bUpdate = TRUE);

    /*!
    * @brief Get the duration of the translate animation.
    *
    * @param pDuration      [ /O] The duration
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetDuration(OUT DOUBLE *pDuration);

    /*!
    * @brief Get the transform of the translate animation.
    *
    * @param pTransform     [ /O] The transform information.
    * @param bUpdate        [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetTransform(OUT LPTRANSFORMINFO pTransform,
                                 IN  BOOL bUpdate = TRUE);

    /*!
    * @brief Get the information of the translate animation.
    *
    * @param pppVarible             [ /O] The variable information of the translate animation.
    * @param pppTransition          [ /O] The transition information of the translate animation.
    * @param pTransitionCount       [ /O] The count of the variable.
    * @param bUpdate                [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetAnimationInfo(OUT  IUIAnimationVariable ***pppVarible,
                                     OUT  IUIAnimationTransition ***pppTransition,
                                     OUT  UINT32 *pTransitionCount,
                                     BOOL bUpdate = TRUE) ;

protected:

    /*!
    * @brief Initialize the storyboard of the translate animation.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT InitStoryboard();

private:

    LPTRANSITIONINFO                m_pTranslateInfoX;      // The information of the X-translate animation.
    LPTRANSITIONINFO                m_pTranslateInfoY;      // The information of the Y-translate animation.

    IUIAnimationVariable            *m_pVariableX;           // The variable of the translate animation at the X direction.
    IUIAnimationTransition          *m_pTransitionX;        // The transition of the translate animation at the X direction.
    IUIAnimationVariable            *m_pVariableY;           // The variable of the translate animation at the Y direction.
    IUIAnimationTransition          *m_pTransitionY;        // The transition of the translate animation at the Y direction.
};

#endif // _TRANSLATEANIMATION_H_
#endif // __cplusplus
