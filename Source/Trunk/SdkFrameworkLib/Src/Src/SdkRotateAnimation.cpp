/*!
* @file SdkRotateAnimation.cpp 
* 
* @brief This file defines the functions of rotate animation.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Qiao LiYun, Cui ShuYan
* @date 2010/12/28
*/

#include "stdafx.h"
#include "SdkRotateAnimation.h"
#include "SdkAnimationCom.h"

#include "D2D1.h"
using namespace D2D1;

USING_NAMESPACE_ANIMATION

//////////////////////////////////////////////////////////////////////////

SdkRotateAnimation::SdkRotateAnimation(
    const D2D1_POINT_2F *pointCenter,
    DOUBLE dFromAngle,
    DOUBLE dToAngle,
    DOUBLE dDuration,
    DOUBLE dAccelerationRatio,
    DOUBLE dDecelerationRatio)
{
    m_pAngleVariable = NULL;
    m_pTransition = NULL;
    m_nAnimationType = TRANSFORM_TYPE_ROTATE;

    m_pRotateInfo = new TRANSITIONINFO();
    ZeroMemory(m_pRotateInfo, sizeof(TRANSITIONINFO));

    m_pRotateInfo->dFrom = dFromAngle;
    m_pRotateInfo->dTo = dToAngle;
    m_pRotateInfo->dDuration = dDuration;
    m_pRotateInfo->dAccelerationRatio = dAccelerationRatio;
    m_pRotateInfo->dDecelerationRatio = dDecelerationRatio;

    if (NULL != pointCenter)
    {
        m_centerPoint = D2D1::Point2F(pointCenter->x, pointCenter->y);
    }
    else
    {
        m_centerPoint = D2D1::Point2F(0, 0);
    }
}

//////////////////////////////////////////////////////////////////////////

SdkRotateAnimation::~SdkRotateAnimation(void)
{
    SAFE_DELETE(m_pRotateInfo);

    SAFE_RELEASE(m_pAngleVariable);
    SAFE_RELEASE(m_pTransition);
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkRotateAnimation::GetValue(OUT DOUBLE *pValue, BOOL bUpdate)
{
    if (NULL != pValue)
    {
        *pValue = 0.0;
    }

    return GetVariableValue(pValue, m_pAngleVariable, bUpdate);
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkRotateAnimation::InitStoryboard()
{
    HRESULT hr = CreateVariable(m_pRotateInfo, &m_pAngleVariable, TRUE);
    if (SUCCEEDED(hr))
    {
        hr = CreateTransition(m_pRotateInfo, &m_pTransition, TRUE);
        if (SUCCEEDED(hr))
        {
            SAFE_RELEASE(m_pStoryBoard);
            hr = m_pAnimationCom->CreateStoryboard(&m_pStoryBoard);
            if (SUCCEEDED(hr))
            {
                hr = m_pStoryBoard->AddTransition(m_pAngleVariable, m_pTransition);
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkRotateAnimation::GetDuration(OUT DOUBLE *pDuration)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pTransition)
    {
        hr = m_pTransition->GetDuration(pDuration);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkRotateAnimation::GetTransform(OUT LPTRANSFORMINFO pTransform, IN BOOL bUpdate) 
{
    HRESULT hr = S_OK;
    if (NULL != pTransform)
    {
        if (bUpdate)
        {
            hr = this->Update();
        }

        if (SUCCEEDED(hr))
        {
            if (NULL != m_pAngleVariable && NULL != m_pRotateInfo)
            {
                DOUBLE dValue = 0.0; 
                hr = m_pAngleVariable->GetValue(&dValue);
                if (SUCCEEDED(hr))
                {
                    pTransform->typeTransform = m_nAnimationType;
                    pTransform->matrixTransform = D2D1::Matrix3x2F::Rotation((FLOAT)dValue, m_centerPoint);
                }
            }
            else
            {
                hr = E_FAIL;
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkRotateAnimation::GetAnimationInfo(
    OUT  IUIAnimationVariable ***pppVarible,
    OUT  IUIAnimationTransition ***pppTransition,
    OUT  UINT32 *pTransitionCount,
    BOOL bUpdate)
{
    HRESULT hr = E_FAIL;

    if ( (NULL != m_pRotateInfo) && 
         (NULL != pppVarible) &&
         (NULL != pppTransition) &&
         (NULL != pTransitionCount) )
    {
        (*pTransitionCount) = 1;
        (*pppVarible) = new IUIAnimationVariable*[1];
        (*pppTransition) = new IUIAnimationTransition*[1];

        hr = CreateVariable(m_pRotateInfo, &m_pAngleVariable, bUpdate);
        if (SUCCEEDED(hr))
        {
            hr = CreateTransition(m_pRotateInfo, &m_pTransition, bUpdate);
            if (SUCCEEDED(hr))
            {
                (*pppVarible)[0] = m_pAngleVariable;
                (*pppTransition)[0] = m_pTransition;

                SAFE_ADDREF(m_pAngleVariable);
                SAFE_ADDREF(m_pTransition);
            }
        }
    }

    return hr;
}
