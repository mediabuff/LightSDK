/*!
* @file Animation.cpp 
* 
* @brief This file defines the base functions of animation.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/

#include "Animation.h"
#include "StoryboardEventHandler.h"

Animation::Animation():
           m_pStoryBoard(NULL),
           m_pStoryboardEventHandler(NULL),
           m_pAnimationCom(NULL),
           m_nAnimationType(TRANSFORM_TYPE_UNKNOWN),
           m_bStart(FALSE)
{
    m_pAnimationCom = new AnimationCommon();
}

//////////////////////////////////////////////////////////////////////////

Animation::~Animation()
{
    SAFE_DELETE(m_pAnimationCom);
    SAFE_RELEASE(m_pStoryBoard);
    SAFE_RELEASE(m_pStoryboardEventHandler);
}

//////////////////////////////////////////////////////////////////////////

BOOL Animation::IsStart()
{
    return m_bStart;
}

//////////////////////////////////////////////////////////////////////////

INT32 Animation::GetAnimationType()
{
    return m_nAnimationType;
}

//////////////////////////////////////////////////////////////////////////

HRESULT Animation::Stop()
{
    return this->Finish();
    /*HRESULT hr = E_FAIL;

    if(NULL != m_pStoryBoard)
    {
        hr = m_pStoryBoard->Conclude();
    }

    return hr;*/
}

//////////////////////////////////////////////////////////////////////////

HRESULT Animation::Finish()
{
    HRESULT hr = E_FAIL;

    if(NULL != m_pStoryBoard)
    {
        hr = m_pStoryBoard->Finish(0);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT Animation::Start(DOUBLE dDelaySeconds /*= 0*/)
{
    UI_ANIMATION_SECONDS timeNow;
    HRESULT hr = m_pAnimationCom->GetTime(&timeNow);
    if(SUCCEEDED(hr))
    {
        m_bStart = TRUE;
        hr = this->StartAnimation(timeNow + dDelaySeconds);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT Animation::GetElapsedTime(OUT DOUBLE &dElapsedTime)
{
    HRESULT hr = E_FAIL;

    if(NULL != m_pStoryBoard)
    {
        hr = m_pStoryBoard->GetElapsedTime(&dElapsedTime);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT Animation::StartAnimation(IN UI_ANIMATION_SECONDS timeNow)
{
    HRESULT hr = E_FAIL;

    SAFE_RELEASE(m_pStoryBoard);
    if(SUCCEEDED(InitStoryboard()))
    {
        this->SetStoryBoardListener(NULL);
        hr = m_pStoryBoard->Schedule(timeNow, NULL);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT Animation::Update()
{
    return m_pAnimationCom->Update();
}

//////////////////////////////////////////////////////////////////////////

HRESULT Animation::IsFinish(OUT BOOL *bFinish)
{
    HRESULT hr = E_FAIL;
    if(NULL != bFinish)
    {
        *bFinish = TRUE;
    }

    if(NULL != m_pStoryBoard && NULL != bFinish)
    {
        UI_ANIMATION_STORYBOARD_STATUS status;
        hr = m_pStoryBoard->GetStatus(&status);
        if(SUCCEEDED(hr))
        {
            if((UI_ANIMATION_STORYBOARD_READY == status)
                || ((UI_ANIMATION_STORYBOARD_BUILDING == status)))
            {
                *bFinish = TRUE;
            }
            else
            {
                *bFinish = FALSE;
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT Animation::AddAnimationListener(IAnimationListener *pListener)
{
    return this->SetStoryBoardListener(pListener);
}

//////////////////////////////////////////////////////////////////////////

HRESULT Animation::RemoveAnimationListener(IAnimationListener *pListener)
{
    HRESULT hr = S_OK;

    if(NULL != pListener && (NULL != m_pStoryboardEventHandler))
    {
        m_pStoryboardEventHandler->RemoveAnimationListener(pListener);
    }
    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT Animation::AddAnimationTimerListener(IAnimationTimerListener *pListener)
{
    return m_pAnimationCom->AddTimerListener(pListener, this);
}

//////////////////////////////////////////////////////////////////////////

HRESULT Animation::RemoveAnimationTimerListener(IAnimationTimerListener *pListener)
{
    return m_pAnimationCom->RemoveTimerListener(pListener);
}

//////////////////////////////////////////////////////////////////////////

HRESULT Animation::SetStoryBoardListener(IAnimationListener *pListener)
{
    HRESULT hr = S_OK;

    if(NULL == m_pStoryboardEventHandler)
    {
        m_pStoryboardEventHandler = new StoryboardEventHandler();
    }

    m_pStoryboardEventHandler->AddAnimationListener(pListener, this);
    if(NULL != m_pStoryBoard)
    {
        hr = m_pStoryBoard->SetStoryboardEventHandler(m_pStoryboardEventHandler);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT Animation::CreateVariable(LPTRANSITIONINFO      pTransitionInfo,
                                  IUIAnimationVariable  **pTransitionVarible,
                                  BOOL                  bUpdate)
{
    HRESULT hr = E_FAIL;

    if(NULL != pTransitionVarible)
    {
        if(bUpdate)
        {
            SAFE_RELEASE(*pTransitionVarible);
        }

        if(NULL == *pTransitionVarible)
        {
            hr = m_pAnimationCom->CreateVariable(pTransitionInfo->dFrom, pTransitionVarible);
        }
        else
        {
            hr = S_OK;
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT Animation::CreateTransition(LPTRANSITIONINFO          pTransitionInfo,
                                    IUIAnimationTransition    **pTransition,
                                    BOOL                      bUpdate)
{
    HRESULT hr = E_FAIL;

    if(NULL != pTransition)
    {
        if(bUpdate)
        {
            SAFE_RELEASE(*pTransition);
        }

        if(NULL == *pTransition)
        {
            hr = m_pAnimationCom->CreateAccelerateDecelerateTransition(pTransitionInfo->dDuration,
                pTransitionInfo->dTo,
                pTransitionInfo->dAccelerationRatio,
                pTransitionInfo->dDecelerationRatio,
                pTransition);
        }
        else
        {
            hr = S_OK;
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT Animation::GetVariableValue(OUT DOUBLE            *pVariableValue,
                                    IUIAnimationVariable  *pTransitionVarible,
                                    BOOL                  bUpdate)
{
    HRESULT hr = S_OK;

    if(NULL != pVariableValue && NULL != pTransitionVarible)
    {
        if(bUpdate)
        {
            hr = this->Update();
        }

        if(SUCCEEDED(hr))
        {
            if(NULL != pTransitionVarible)
            {
                hr = pTransitionVarible->GetValue(pVariableValue);
            }
            else
            {
                hr = E_FAIL;
            }
        }
    }
    else
    {
        hr = E_FAIL;
    }

    return hr;
}
