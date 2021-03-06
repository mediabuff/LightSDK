/*!
* @file SdkAnimationSet.cpp 
* 
* @brief This file defines the functions of animation set.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/

#include "stdafx.h"
#include "SdkAnimationSet.h"
#include "SdkAnimationCom.h"

USING_NAMESPACE_ANIMATION

//////////////////////////////////////////////////////////////////////////

SdkAnimationSet::SdkAnimationSet()
{
}

//////////////////////////////////////////////////////////////////////////

SdkAnimationSet::~SdkAnimationSet()
{
    ClearAnimation();
    ClearAnimationInfo();
}

//////////////////////////////////////////////////////////////////////////

void SdkAnimationSet::AddAnimation(SdkAnimation *pAnimation)
{
    if (NULL != pAnimation)
    {
        m_listAnimation.push_back(pAnimation);
        // The m_nAnimationType is base class's member.
        m_nAnimationType |= pAnimation->GetAnimationType();
    }
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAnimationSet::GetDuration(OUT DOUBLE *pDuration)
{
    UNREFERENCED_PARAMETER(pDuration);

    return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAnimationSet::GetTransform(OUT LPTRANSFORMINFO pTransform, IN BOOL bUpdate)
{
    if (NULL == pTransform)
    {
        return E_INVALIDARG;
    }

    HRESULT hr = bUpdate ? this->Update() : S_OK;
    if ( SUCCEEDED(hr) )
    {
        pTransform->typeTransform = m_nAnimationType;
        pTransform->matrixTransform = D2D1::IdentityMatrix();
        static TRANSFORMINFO TempInfo = { 0 };
        ZeroMemory(&TempInfo, sizeof(TRANSFORMINFO));

        for( list<SdkAnimation*>::iterator iter = m_listAnimation.begin();
             iter != m_listAnimation.end();
             ++iter )
        {
            if (NULL != *iter && SUCCEEDED((*iter)->GetTransform(&TempInfo, bUpdate)))
            {
                pTransform->matrixTransform = pTransform->matrixTransform * TempInfo.matrixTransform;

                if (TRANSFORM_TYPE_ALPHA == TempInfo.typeTransform)
                {
                    pTransform->dAlpha = TempInfo.dAlpha;
                }
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAnimationSet::InitStoryboard()
{
    SAFE_RELEASE(m_pStoryBoard);

    ClearAnimationInfo();

    HRESULT hr = m_pAnimationCom->CreateStoryboard(&m_pStoryBoard);
    if ( SUCCEEDED(hr) )
    {
        for ( list<SdkAnimation*>::iterator iter = m_listAnimation.begin();
              iter != m_listAnimation.end();
              ++iter )
        {
            if (NULL == *iter)
            {
                continue;
            }

            LPANIMATIONINFO2 pInfo = new ANIMATIONINFO2();
            hr = (*iter)->GetAnimationInfo(
                &pInfo->ppVariable, 
                &pInfo->ppTransition,
                &pInfo->nTransitionCount);

            if (SUCCEEDED(hr))
            {
                hr = AddAnimation(pInfo);
                if ( SUCCEEDED(hr) )
                {
                    m_listAniamtionInfo.push_back(pInfo);
                }
            }

            if ( FAILED(hr) )
            {
                SAFE_DELETE(pInfo);
                break;
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAnimationSet::AddAnimation(const LPANIMATIONINFO2 pInfo)
{
    HRESULT hr = E_FAIL;

    if (NULL == pInfo || NULL == pInfo->ppTransition || NULL == pInfo->ppVariable)
    {
        hr = E_INVALIDARG;
    }
    else if (NULL != m_pStoryBoard)
    {
        for (UINT32 nIndex = 0; nIndex < pInfo->nTransitionCount; ++nIndex )
        {
            hr = m_pStoryBoard->AddTransition(
                (pInfo->ppVariable)[nIndex],
                (pInfo->ppTransition)[nIndex]);

            if ( FAILED(hr) )
            {
                break;
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

void SdkAnimationSet::ClearAnimation()
{
    for ( list<SdkAnimation*>::iterator iter = m_listAnimation.begin();
          iter != m_listAnimation.end();
          ++iter )
    {
        SAFE_DELETE(*iter);
    }

    m_listAnimation.clear();
}

//////////////////////////////////////////////////////////////////////////

void SdkAnimationSet::ClearAnimationInfo()
{
    for ( list<LPANIMATIONINFO2>::iterator iter = m_listAniamtionInfo.begin();
          iter != m_listAniamtionInfo.end();
          ++iter )
    {
        SAFE_DELETE(*iter);
    }

    m_listAniamtionInfo.clear();
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAnimationSet::GetAnimationInfo(
    OUT IUIAnimationVariable ***pppVarible,
    OUT IUIAnimationTransition ***pppTransition,
    OUT UINT32 *pTransitionCount,
    BOOL bUpdate)
{
    UNREFERENCED_PARAMETER(pppVarible);
    UNREFERENCED_PARAMETER(pppTransition);
    UNREFERENCED_PARAMETER(pTransitionCount);
    UNREFERENCED_PARAMETER(bUpdate);

    return E_NOTIMPL;
}
