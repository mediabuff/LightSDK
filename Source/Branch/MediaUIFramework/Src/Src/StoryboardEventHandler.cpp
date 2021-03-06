/*!
* @file StoryboardEventHandler.cpp 
* 
* @brief This file defines the event handler of storyboard.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/

#include "StoryboardEventHandler.h"

//////////////////////////////////////////////////////////////////////////

StoryboardEventHandler::StoryboardEventHandler():
                        m_pAnimation(NULL),
                        m_lRefCount(1)
{
}

//////////////////////////////////////////////////////////////////////////

StoryboardEventHandler::~StoryboardEventHandler()
{
}

//////////////////////////////////////////////////////////////////////////

void StoryboardEventHandler::AddAnimationListener(IAnimationListener *pListener, Animation *pAnimation)
{
    m_pAnimation = pAnimation;
    if(NULL != pListener)
    {
        BOOL bFind = FALSE;
        for(vector<IAnimationListener*>::iterator iter = m_vecListener.begin();
            iter != m_vecListener.end();
            ++iter)
        {
            if(*iter == pListener)
            {
                bFind = TRUE;
                break;
            }
        }

        if(!bFind)
        {
            m_vecListener.push_back(pListener);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void StoryboardEventHandler::RemoveAnimationListener(IAnimationListener *pListener)
{
    if(NULL != pListener)
    {
        for(vector<IAnimationListener*>::iterator iter = m_vecListener.begin();
            iter != m_vecListener.end();
            ++iter)
        {
            if(*iter == pListener)
            {
                m_vecListener.erase(iter);
                break;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

HRESULT StoryboardEventHandler::OnStoryboardStatusChanged(
    IUIAnimationStoryboard * /*storyboard*/,
    UI_ANIMATION_STORYBOARD_STATUS newStatus,
    UI_ANIMATION_STORYBOARD_STATUS previousStatus)
{
    IAnimationListener *pListener = NULL;
    for(vector<IAnimationListener*>::iterator iter = m_vecListener.begin();
        iter != m_vecListener.end();
        ++iter)
    {
        pListener = *iter;
        if(NULL != pListener)
        {
            if(previousStatus == UI_ANIMATION_STORYBOARD_BUILDING
                && newStatus == UI_ANIMATION_STORYBOARD_SCHEDULED)
            {
                pListener->OnAnimationStart(m_pAnimation);
            }
            else if(previousStatus == UI_ANIMATION_STORYBOARD_SCHEDULED
                && newStatus == UI_ANIMATION_STORYBOARD_PLAYING)
            {
                pListener->OnAnimationPlaying(m_pAnimation);
            }
            else if((previousStatus == UI_ANIMATION_STORYBOARD_PLAYING)
                && (newStatus == UI_ANIMATION_STORYBOARD_FINISHED))
            {
                pListener->OnAnimationEnd(m_pAnimation);
            }
        }
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

HRESULT StoryboardEventHandler::OnStoryboardUpdated(IUIAnimationStoryboard * /*storyboard*/)
{
    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

HRESULT StoryboardEventHandler::QueryInterface( REFIID riid,
                       __RPC__deref_out void __RPC_FAR *__RPC_FAR * ppvObject)
{
    if(NULL != ppvObject)
    {
        if(IID_IUIAnimationStoryboardEventHandler == riid)
        {
            *ppvObject = this;
        }
        else
        {
            *ppvObject = NULL;
        }

        return S_OK;
    }

    return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////

ULONG StoryboardEventHandler::AddRef()
{
    return InterlockedIncrement(&m_lRefCount);
}

//////////////////////////////////////////////////////////////////////////

ULONG StoryboardEventHandler::Release()
{
    LONG lRef = InterlockedDecrement(&m_lRefCount);
    if ( 0 == lRef )
    {
        delete this;
    }

    return lRef;
}
