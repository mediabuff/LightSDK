/*!
* @file SdkGridView.cpp
* 
* @brief This file defines the grid view 
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/06
*/

#include "stdafx.h"
#include "SdkGridView.h"

USING_NAMESPACE_VIEWS

SdkGridView::SdkGridView() : m_fChildWidth(200),
                       m_fChildHeight(200),
                       m_fLMargin(20),
                       m_fRMargin(20),
                       m_uRowCount(1),
                       m_uColCount(1),
                       m_isAutoSize(FALSE)
{
    SetSlideDirection(SLIDEDIRECTIOIN_VERTICAL);
    SetSlideAnimationEnable(FALSE);
}

//////////////////////////////////////////////////////////////////////////

SdkGridView::~SdkGridView()
{
}

//////////////////////////////////////////////////////////////////////////

void SdkGridView::SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
    CalcGridViewSize(&width, &height);
    SdkSlideLayout::SetLayoutInfo(x, y, width, height);
}

//////////////////////////////////////////////////////////////////////////

void SdkGridView::SetAutoSize(BOOL isAutoSize)
{
    if (m_isAutoSize != isAutoSize)
    {
        m_isAutoSize = isAutoSize;
        SdkSlideLayout::ResetOffset(0);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkGridView::SetChildSize(FLOAT fWidth, FLOAT fHeight)
{
    m_fChildWidth  = fWidth;
    m_fChildHeight = fHeight;
}

//////////////////////////////////////////////////////////////////////////

void SdkGridView::GetChildSize(FLOAT *pfWidth, FLOAT *pfHeight)
{
    if (NULL != pfWidth)
    {
        *pfWidth = m_fChildWidth;
    }

    if (NULL != pfHeight)
    {
        *pfHeight = m_fChildHeight;
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkGridView::SetRowCount(UINT32 uRowCount)
{
    if (m_uRowCount != uRowCount)
    {
        m_uRowCount = uRowCount;
        SdkSlideLayout::ResetOffset(0);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkGridView::SetColCount(UINT32 uColCount)
{
    if (m_uRowCount != uColCount)
    {
        m_uColCount = uColCount;
        SdkSlideLayout::ResetOffset(0);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkGridView::CalcGridViewSize(FLOAT *pWidth, FLOAT *pHeight)
{
    if (m_isAutoSize)
    {
        FLOAT width  = m_uColCount * m_fChildWidth + m_fLMargin + m_fRMargin + 20;
        FLOAT height = m_uRowCount * m_fChildHeight + 20;

        if (NULL != pWidth)
        {
            *pWidth = width;
        }

        if (NULL != pHeight)
        {
            *pHeight = height;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkGridView::CalcChildViewIndex(OUT INT32 *pStartIndex, OUT INT32 *pEndIndex)
{
    SdkBaseAdapter *pAdapter = GetAdapter();
    if ((NULL == pAdapter) || (0 == pAdapter->GetCount()))
    {
        return;
    }

    FLOAT offset = GetSlideOffset();
    FLOAT mostL = 0.0f;
    FLOAT fRowHeight = (GetHeight() / m_uRowCount);
    FLOAT fColWidth  = (GetWidth()  / m_uColCount);
    INT32 nStartIndex = 0;
    INT32 nEndIndex   = 0;
    INT32 nChildCount = 0;
    INT32 nCount = pAdapter->GetCount();
    INT32 nStartRow = 0;
    INT32 nStartCol = 0;

    switch (GetSlideDirection())
    {
    case SLIDEDIRECTIOIN_VERTICAL:
        mostL = max((offset), (0));
        nStartRow = (INT32)((mostL - offset) / (fRowHeight)) - 1;
        nStartIndex = nStartRow * m_uColCount;
        nChildCount = (m_uRowCount + 3) * m_uColCount;
        break;

    case SLIDEDIRECTIOIN_HORIZONTAL:
        mostL = max((offset), (0));
        nStartCol = (INT32)((mostL - offset) / (fColWidth));
        nStartIndex = (nStartCol - 2) * m_uRowCount;
        nChildCount = (m_uColCount + 5) * m_uRowCount;
        break;
    }

    //nStartIndex -= m_uColCount;
    nStartIndex = (nStartIndex < 0) ? 0 : ((nStartIndex >= nCount) ? (nCount - 1) : nStartIndex);
    nEndIndex   = nStartIndex + nChildCount;
    nEndIndex = ((nEndIndex > nCount) ? nCount : nEndIndex);

    if (NULL != pStartIndex)
    {
        (*pStartIndex) = nStartIndex;
    }

    if (NULL != pEndIndex)
    {
        (*pEndIndex)   = nEndIndex;
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkGridView::OnLayout(BOOL bChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    INT32 nChildCount = GetChildCount();
    INT32 nColWidth  = (INT32)(width - m_fLMargin - m_fRMargin) / (INT32)m_uColCount;
    INT32 nRowHeight = (INT32)(height) / (INT32)m_uRowCount;
    INT32 nCurRow  = 0;
    INT32 nCurCol  = 0;
    FLOAT fLMargin = (nColWidth  - m_fChildWidth)  / 2;
    FLOAT fTMargin = (nRowHeight - m_fChildHeight) / 2;
    FLOAT fCurLeft = fLMargin + m_fLMargin;
    FLOAT fCurTop  = fTMargin;

    SdkViewElement *pChild = NULL;
    for (int i = 0; i < nChildCount; ++i)
    {
        if (GetChildAt(i, &pChild) && (NULL != pChild))
        {
            nCurRow = i / m_uColCount;
            nCurCol = i % m_uColCount;
            fCurLeft = nCurCol * nColWidth  + m_fLMargin + fLMargin;
            fCurTop  = nCurRow * nRowHeight + fTMargin;
            pChild->SetLayoutInfo(fCurLeft, fCurTop, m_fChildWidth, m_fChildHeight);
        }
    }

    // Set the slide range.
    SdkBaseAdapter *pAdapter = GetAdapter();
    if (NULL != pAdapter)
    {
        nChildCount = pAdapter->GetCount();
    }

    INT32 nTotalRowNum = (INT32)(nChildCount / m_uColCount) + 1;
    FLOAT fSlideHeight = (FLOAT)(nTotalRowNum * nRowHeight);
    SetSlideRange((UINT)width, (UINT)fSlideHeight);
    SetSlideStep(height);

    SdkSlideLayout::OnLayout(bChanged, left, top, width, height);
}
