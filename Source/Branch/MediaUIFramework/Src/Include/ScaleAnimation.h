/*!
* @file ScaleAnimation.h 
* 
* @brief This file defines the functions of scale animation.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/


#ifdef __cplusplus
#ifndef _SCALEANIMATION_H_
#define _SCALEANIMATION_H_

#include "Animation.h"

/*!
* @brief This class implements the function of the scale animation.
*/
class CLASS_DECLSPEC ScaleAnimation : public Animation
{
public:

    /*!
    * @brief The constructor.
    *
    * @param pointCenter             [I/ ] The pointer of the center point.
    */
    ScaleAnimation(const D2D1_POINT_2F *pointCenter);

    /*!
    * @brief The default destructor.
    */
    ~ScaleAnimation();

    /*!
    * @brief Set the information of the scale animation in X axis.
    *
    * @param dFromX             [I/ ] The start position at the X direction.
    * @param dToX               [I/ ] The end position at the X direction.
    * @param dDuration          [I/ ] The duration of the scale animation.
    * @param accelerationRatio  [I/ ] The accelerate ratio of the scale animation.
    * @param decelerationRatio  [I/ ] The decelerate ratio of the scale animation.
    */
    VOID SetScaleXInfo(DOUBLE dFromX,
                       DOUBLE dToX,
                       DOUBLE dDuration = 1.0,
                       DOUBLE accelerationRatio = 0.5,
                       DOUBLE decelerationRatio = 0.5);

    /*!
    * @brief Set the information of the scale animation in Y axis.
    *
    * @param dFromY             [I/ ] The start position at the Y direction.
    * @param dToY               [I/ ] The end position at the Y direction.
    * @param dDuration          [I/ ] the duration of the scale animation.
    * @param accelerationRatio  [I/ ] The accelerate ratio of the scale animation.
    * @param decelerationRatio  [I/ ] The decelerate ratio of the scale animation.
    */
    VOID SetScaleYInfo(DOUBLE dFromY,
                       DOUBLE dToY,
                       DOUBLE dDuration = 1.0,
                       DOUBLE accelerationRatio = 0.5,
                       DOUBLE decelerationRatio = 0.5);

    /*!
    * @brief Set the pivot of the scale animation.
    *
    * @param pointCenter    [I/ ] The pivot position.
    */
    VOID SetPivotPoint(D2D1_POINT_2F *pointCenter);

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
    * @brief Get the duration of the scale animation.
    *
    * @param pDuration  [ /O] The duration
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetDuration(OUT DOUBLE *pDuration);

    /*!
    * @brief Get the transform of the scale animation.
    *
    * @param pTransform     [ /O] The transform information.
    * @param bUpdate        [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetTransform(OUT LPTRANSFORMINFO pTransform,
                                 IN  BOOL bUpdate = TRUE);

    /*!
    * @brief Get the information of the scale animation.
    *
    * @param pppVarible             [ /O] The variable information of the scale animation.
    * @param pppTransition          [ /O] The transition information of the scale animation.
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
    * @brief Initialize the storyboard of the scale animation.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT InitStoryboard();

private:

    D2D1_POINT_2F                   m_centerPoint;        // The center point.
    LPTRANSITIONINFO                m_pScaleInfoX;        // The information of the X-scale animation.
    LPTRANSITIONINFO                m_pScaleInfoY;        // The information of the Y-scale animation.

    IUIAnimationVariable            *m_pVariableX;       // The variable of the scale animation at the X direction.
    IUIAnimationTransition          *m_pTransitionX;     // The transition of the scale animation at the X direction.
    IUIAnimationVariable            *m_pVariableY;       // The variable of the scale animation at the Y direction.
    IUIAnimationTransition          *m_pTransitionY;     // The transition of the scale animation at the Y direction.
};

#endif // _SCALEANIMATION_H_
#endif // __cplusplus