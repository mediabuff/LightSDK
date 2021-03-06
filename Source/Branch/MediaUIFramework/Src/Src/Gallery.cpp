/*!
* @file Gallery.cpp
* 
* @brief This file defines class Gallery, which supports drag to slide and sliding animation after dragging.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/28
*/

#include "Gallery.h"

Gallery::Gallery() : m_fMargin(10),
                     m_fChildWidth(200),
                     m_fChildHeight(150)
{
    SetSlideDirection(SLIDEDIRECTIOIN_HORIZONTAL);
}

//////////////////////////////////////////////////////////////////////////

Gallery::~Gallery()
{
}

//////////////////////////////////////////////////////////////////////////

void Gallery::SetChildMargin(FLOAT fMargin)
{
    m_fMargin = fMargin;
}

//////////////////////////////////////////////////////////////////////////

void Gallery::SetChildSize(FLOAT fWidth, FLOAT fHeight)
{
    m_fChildWidth  = fWidth;
    m_fChildHeight = fHeight;
}

//////////////////////////////////////////////////////////////////////////

void Gallery::CalcChildViewIndex(OUT INT32 *pStartIndex, OUT INT32 *pEndIndex)
{
    BaseAdapter *pAdapter = GetAdapter();
    if ((NULL == pAdapter) || (0 == pAdapter->GetCount()))
    {
        return;
    }

    FLOAT offset = GetSlideOffset();
    FLOAT mostL = 0.0f;
    FLOAT mostR = 0.0f;
    int nStartIndex = 0;
    int nChildCount = 0;
    int nCount = pAdapter->GetCount();

    switch (GetSlideDirection())
    {
    case SLIDEDIRECTIOIN_HORIZONTAL:
        mostL = max((offset), (0));
        mostR = GetWidth();
        nStartIndex = (int)((mostL - offset) / (m_fMargin + m_fChildWidth));
        nChildCount = (int)((mostR - mostL) / (m_fMargin + m_fChildWidth));
        break;

    case SLIDEDIRECTIOIN_VERTICAL:
        mostL = max((offset), (0));
        mostR = GetHeight();
        nStartIndex = (int)((mostL - offset) / (m_fMargin + m_fChildHeight));
        nChildCount = (int)((mostR - mostL) / (m_fMargin + m_fChildHeight));
        break;
    }

    nStartIndex -= nChildCount;
    nChildCount *= 4;
    nStartIndex = (nStartIndex < 0) ? 0 : ((nStartIndex >= nCount) ? (nCount - 1) : nStartIndex);
    nChildCount = ((nChildCount > nCount) ? nCount : nChildCount);

    if (NULL != pStartIndex)
    {
        (*pStartIndex) = nStartIndex;
    }

    if (NULL != pEndIndex)
    {
        (*pEndIndex)   = nStartIndex + nChildCount;
    }
}

//////////////////////////////////////////////////////////////////////////

void Gallery::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    FLOAT childLeft = m_fMargin;
    FLOAT childTop  = 0;
    SLIDEDIRECTIOIN slideDir = GetSlideDirection();

    int nChildCount = GetChildCount();
    for (int i = 0; i < nChildCount; ++i)
    {
        ViewElement *pChild = NULL;
        GetChildAt(i, &pChild);
        if (NULL != pChild)
        {
            switch (slideDir)
            {
            case SLIDEDIRECTIOIN_HORIZONTAL:
                childLeft = i * (m_fMargin + m_fChildWidth) + m_fMargin;
                pChild->SetLayoutInfo(childLeft, childTop, m_fChildWidth, m_fChildHeight);
                break;

            case SLIDEDIRECTIOIN_VERTICAL:
                childTop = i * (m_fMargin + m_fChildHeight) + m_fMargin;
                pChild->SetLayoutInfo(childLeft, childTop, m_fChildWidth, m_fChildHeight);
                break;
            }
        }
    }

    BaseAdapter *pAdapter = GetAdapter();
    if (NULL != pAdapter)
    {
        nChildCount = pAdapter->GetCount();
    }

    switch (slideDir)
    {
    case SLIDEDIRECTIOIN_HORIZONTAL:
        {
            FLOAT fSlideWidth = nChildCount * (m_fMargin + m_fChildWidth) + m_fMargin;
            SetSlideRange((UINT)fSlideWidth, (UINT)height);
            SetSlideStep(m_fMargin + m_fChildWidth);
        }
        break;

    case SLIDEDIRECTIOIN_VERTICAL:
        {
            FLOAT fSlideHeight = nChildCount * (m_fMargin + m_fChildHeight) + m_fMargin;
            SetSlideRange((UINT)width, (UINT)fSlideHeight);
            SetSlideStep(m_fMargin + m_fChildHeight);
        }
        break;
    }

    SlideLayout::OnLayout(fChanged, left, top, width, height);
}
