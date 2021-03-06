/*!
* @file SdkInkManager.cpp
* 
* @brief Implementation of the CInkManager class.
*
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2011/05/19 12:00
* @version 1.0.0
*/

#include "stdafx.h"
#include "SdkInkManager.h"

USING_NAMESPACE_COMMON

#define MAXIMUMALTERNATES   5       // The maximum alternates

//////////////////////////////////////////////////////////////////////////

SdkInkManager::SdkInkManager()
{
    m_langID            = 0;
    m_pInkRecogContext  = NULL;
    m_pInkOverlay       = NULL;
    m_pInkDisp          = NULL;
    m_hClientWnd        = NULL;
    m_pInkStrokes       = NULL;
    m_hAttachedWnd      = NULL;
    m_pInkDrawingAttr   = NULL;
    m_pInkRender        = NULL;
    m_pInkRecognizers   = NULL;
    m_pInkRecogGuide    = NULL;
    m_pInkRecognizer    = NULL;
    m_pInkRecogResult   = NULL;
    m_pGetRecoglistCallback     = NULL;
    m_pHandWrittenTextInsertion = NULL;

    m_vtRecognizerList = new vector<TCHAR*>();
    m_vtRecogResult = new vector<BSTR>();
}

//////////////////////////////////////////////////////////////////////////

SdkInkManager::~SdkInkManager()
{
    // Detach the Window and release resources
    if (m_pInkOverlay != NULL)
    {
        m_pInkOverlay->put_Enabled(VARIANT_FALSE);
        SdkInkEvents::UnadviseInkEvent(m_pInkOverlay);
    }

    if (m_pInkRecogContext != NULL)
    {
        m_pInkRecogContext->EndInkInput();
        SdkInkEvents::UnadviseInkEvent(m_pInkRecogContext);
    }

    typedef vector<TCHAR*>::iterator ITEM;
    for (ITEM itor = m_vtRecognizerList->begin(); itor != m_vtRecognizerList->end(); itor++)
    {
        SAFE_DELETE(*itor);
    }
    m_vtRecognizerList->clear();
    SAFE_DELETE(m_vtRecognizerList);

    SAFE_RELEASE(m_pInkOverlay);
    SAFE_RELEASE(m_pInkRecogContext);
    SAFE_RELEASE(m_pInkDisp);
    SAFE_RELEASE(m_pInkStrokes);
    SAFE_RELEASE(m_pInkDrawingAttr);
    SAFE_RELEASE(m_pInkRender);
    SAFE_RELEASE(m_pInkRecognizers);
    SAFE_RELEASE(m_pInkRecogGuide);
    SAFE_RELEASE(m_pInkRecognizer);
    SAFE_RELEASE(m_pHandWrittenTextInsertion);

    Free(&m_vtRecogResult);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::Initialize(BOOL bRegisterRecognizeEvent, BOOL bRegisterOverlayEvent)
{
    HRESULT hr = E_FAIL;
    // Create instance of IInkOverlay interface
    hr = CoCreateInstance(
        CLSID_InkOverlay,
        NULL,
        CLSCTX_ALL,
        IID_IInkOverlay,
        (LPVOID*)&m_pInkOverlay);

    if ( FAILED(hr) )
    {
        return FALSE;
    }

    // Create instance of IInkRecognizerContext interface
    hr = CoCreateInstance(
        CLSID_InkRecognizerContext,
        NULL, CLSCTX_ALL,
        IID_IInkRecognizerContext,
        (LPVOID*)&m_pInkRecogContext);

    if ( FAILED(hr) )
    {
        return FALSE;
    }

    // Get the object of IInkRecognizers
    hr = CoCreateInstance(
        CLSID_InkRecognizers,
        NULL, CLSCTX_ALL,
        IID_IInkRecognizers,
        (LPVOID*)&m_pInkRecognizers);

    if ( FAILED(hr) )
    {
        return FALSE;
    }

    // Get the default recognizer
    if (FAILED(m_pInkRecognizers->GetDefaultRecognizer(0, &m_pInkRecognizer)))
    {
        return FALSE;
    }

    // Initialize the m_langID member.
    GetLanguageID(&m_langID);

    // Get the object of IInkRecognizerGuide
    hr = CoCreateInstance(
        CLSID_InkRecognizerGuide,
        NULL,
        CLSCTX_ALL,
        IID_IInkRecognizerGuide,
        (LPVOID*)&m_pInkRecogGuide);

    if ( FAILED(hr) )
    {
        return FALSE;
    }

    // Get the object of IInkDrawingAttributes
    hr = CoCreateInstance(
        CLSID_InkDrawingAttributes,
        NULL, CLSCTX_ALL,
        IID_IInkDrawingAttributes,
        (LPVOID*)&m_pInkDrawingAttr);

    if ( SUCCEEDED(hr) )
    {
        m_pInkOverlay->putref_DefaultDrawingAttributes(m_pInkDrawingAttr);
        m_pInkOverlay->get_Renderer(&m_pInkRender);
    }

    // Get the object of IInkDisp
    if ( FAILED(m_pInkOverlay->get_Ink(&m_pInkDisp)) )
    {
        return FALSE;
    }

    // Get the object of IInkStrokes
    if ( FAILED(m_pInkDisp->get_Strokes(&m_pInkStrokes)) )
    {
        return FALSE;
    }

    if (bRegisterOverlayEvent)
    {
        hr = SdkInkEvents::AdviseInkEvent(m_pInkOverlay);
        // Advise the event of _IInkOverlayEvents
    }

    if (bRegisterRecognizeEvent)
    {
        // Advise the event of _IInkRecognitionEvents
        hr = SdkInkEvents::AdviseInkEvent(m_pInkRecogContext);
    }

    return (SUCCEEDED(hr) ? TRUE : FALSE);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::AttachHWND(HWND hWnd)
{
    if ( !IsWindow(hWnd) || (NULL == m_pInkOverlay) )
    {
        return FALSE;
    }

    HRESULT hr = m_pInkOverlay->put_hWnd((LONG_PTR)hWnd);
    if (FAILED(hr))
    {
        return FALSE;
    }

    m_hAttachedWnd = hWnd;

    hr = m_pInkOverlay->put_Enabled(VARIANT_TRUE);

    return (SUCCEEDED(hr) ? TRUE : FALSE);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::Recognize(IN IInkRecognitionResult* pIInkRecoResult)
{
    if (NULL == pIInkRecoResult)
    {
        return FALSE;
    }

    HRESULT hr = E_FAIL;
    IInkRecognitionAlternates* pIInkRecoAlternates = NULL;
    hr = pIInkRecoResult->AlternatesFromSelection(
        0,
        -1,
        MAXIMUMALTERNATES,
        &pIInkRecoAlternates);

    // Free the item memory in the vector and clear its items.
    typedef vector<BSTR>::iterator vector_item;
    for (vector_item item = m_vtRecogResult->begin(); item != m_vtRecogResult->end(); item++)
    {
        SysFreeString(*item);
    }
    m_vtRecogResult->clear();

    // Count the returned alternates, it may be less then we asked for
    LONG lCount = 0;
    if ( SUCCEEDED(hr) && SUCCEEDED(pIInkRecoAlternates->get_Count(&lCount)) )
    {
        // Get the alternate strings
        IInkRecognitionAlternate* pIInkRecoAlternate = NULL;
        for (LONG iItem = 0; (iItem < lCount) && (iItem < MAXIMUMALTERNATES); iItem++)
        {
            // Get the alternate string if there is one
            if ( SUCCEEDED(pIInkRecoAlternates->Item(iItem, &pIInkRecoAlternate)) )
            {
                BSTR bstr = NULL;
                if ( SUCCEEDED(pIInkRecoAlternate->get_String(&bstr)) )
                {
                    // Add the string to list.
                    m_vtRecogResult->push_back(bstr);
                }
                SAFE_RELEASE(pIInkRecoAlternate);
            }
        }

        SAFE_RELEASE(pIInkRecoAlternates);
    }

    return ( SUCCEEDED(hr) ? TRUE : FALSE );
}

//////////////////////////////////////////////////////////////////////////

void SdkInkManager::DetachHWND()
{
    if (m_pInkOverlay != NULL)
    {
        m_pInkOverlay->put_Enabled(VARIANT_FALSE);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::ClearStrokes(BOOL bClearAllOrSelected)
{
    if (NULL == m_pInkDisp)
    {
        return FALSE;
    }

    if ( TRUE == bClearAllOrSelected )
    {
        // Delete the strokes in collection
        m_pInkDisp->DeleteStrokes(NULL);
        SAFE_RELEASE(m_pInkStrokes);

        CComVariant vt(0);
        if ( SUCCEEDED(m_pInkDisp->CreateStrokes(vt, &m_pInkStrokes)) )
        {
            // Attach the new stroke collection to the recognition context.
            // If get_Strokes has failed, m_spIInkStrokes is NULL, so no
            // stroke collection will be attached to the context
            if (m_pInkRecogContext != NULL)
            {
                if (SUCCEEDED(m_pInkRecogContext->putref_Strokes(m_pInkStrokes)))
                {
                    CancelSelection();
                    InvalidateRect(m_hAttachedWnd, NULL, TRUE);
                    return TRUE;
                }
            }
        }
    }
    else
    {
        // Not implement
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::GetBoundingBox(OUT LPRECT lpRect)
{
    if ( (NULL == lpRect) || (NULL == m_pInkDisp) )
    {
        return FALSE;
    }

    IInkRectangle *pInkRectangle = NULL;

    HRESULT hr = m_pInkDisp->GetBoundingBox(IBBM_Default, &pInkRectangle);
    if (SUCCEEDED(hr))
    {
        hr = pInkRectangle->GetRectangle(
            &lpRect->top, &lpRect->left,
            &lpRect->bottom, &lpRect->right);

        if ( SUCCEEDED(hr) )
        {
            HDC hDC = ::GetDC(m_hAttachedWnd);
            if ( (NULL != hDC) && (NULL != m_pInkRender) )
            {
                hr = m_pInkRender->InkSpaceToPixel(
                    (LONG_PTR)hDC, &lpRect->left, &lpRect->top);
                hr = m_pInkRender->InkSpaceToPixel(
                    (LONG_PTR)hDC, &lpRect->right, &lpRect->bottom);
            }
        }
    }

    SAFE_RELEASE(pInkRectangle);

    return (SUCCEEDED(hr) ? TRUE : FALSE);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::GetSelectionRect(OUT LPRECT lpRect)
{
    if ( (NULL == lpRect) || (NULL == m_pInkDisp) || (NULL == m_pInkOverlay) )
    {
        return FALSE;
    }

    HRESULT hr = E_FAIL;
    
    IInkStrokes *pSelection = NULL;
    IInkRectangle *pInkRectangle = NULL;

    hr = m_pInkOverlay->get_Selection(&pSelection);
    if (SUCCEEDED(hr))
    {
        hr = pSelection->GetBoundingBox(IBBM_Default, &pInkRectangle);
        if (SUCCEEDED(hr))
        {
            hr = pInkRectangle->GetRectangle(
                &lpRect->top, &lpRect->left,
                &lpRect->bottom, &lpRect->right);

            if (SUCCEEDED(hr))
            {
                HDC hDC = ::GetDC(m_hAttachedWnd);
                if ( (NULL != hDC) && (NULL != m_pInkRender) )
                {
                    hr = m_pInkRender->InkSpaceToPixel(
                        (LONG_PTR)hDC, &lpRect->left, &lpRect->top);
                    hr = m_pInkRender->InkSpaceToPixel(
                        (LONG_PTR)hDC, &lpRect->right, &lpRect->bottom);
                }
            }
        }
    }

    SAFE_RELEASE(pInkRectangle);
    SAFE_RELEASE(pSelection);

    return (SUCCEEDED(hr) ? TRUE : FALSE);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::AddInkStrokes(IN const SdkInkManager* pSrcInkManager)
{
    HRESULT hr = E_FAIL;

    SdkInkManager *tempObj = const_cast<SdkInkManager*>(pSrcInkManager);
    if (NULL != tempObj)
    {
        IInkRectangle *pInkRectangle = NULL;
        hr = tempObj->m_pInkDisp->GetBoundingBox(IBBM_Default, &pInkRectangle);
        if (SUCCEEDED(hr))
        {
            ClearStrokes();
            IInkStrokes *pstrokes = NULL;
            tempObj->m_pInkDisp->get_Strokes(&pstrokes);
            hr = m_pInkDisp->AddStrokesAtRectangle(pstrokes, pInkRectangle);
        }

        SAFE_RELEASE(pInkRectangle);
    }

    return (SUCCEEDED(hr) ? TRUE : FALSE);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::AddInkStrokes(IN const SdkInkManager* pSrcInkManager, IN const RECT& rect)
{
    HRESULT hr = E_FAIL;

    if ( (NULL == pSrcInkManager) || (NULL == m_pInkDisp) )
    {
        return FALSE;
    }

    RECT tempRect = {rect.left, rect.top, rect.right, rect.bottom};

    HDC hDC = ::GetDC(m_hAttachedWnd);
    if ( (NULL != hDC) && (NULL != m_pInkRender) ) 
    {
        m_pInkRender->PixelToInkSpace((LONG)hDC, &tempRect.left, &tempRect.top);
        m_pInkRender->PixelToInkSpace((LONG)hDC, &tempRect.right, &tempRect.bottom);
    }
    ReleaseDC(m_hAttachedWnd, hDC);

    IInkRectangle *pInkRect = NULL;
    hr = m_pInkDisp->GetBoundingBox(IBBM_Default, &pInkRect);
    if (SUCCEEDED(hr))
    {
        hr = pInkRect->SetRectangle(
            tempRect.top, tempRect.left, tempRect.bottom, tempRect.right);
        if (SUCCEEDED(hr))
        {
            ClearStrokes();
            hr = m_pInkDisp->AddStrokesAtRectangle(
                pSrcInkManager->m_pInkStrokes, pInkRect);
        }
        SAFE_RELEASE(pInkRect);
    }

    return (SUCCEEDED(hr) ? TRUE : FALSE);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::MoveTo(IN IInkRectangle* pInkRectangle, IN BOOL bMoveAllOrSelected)
{
    if ( NULL == pInkRectangle )
    {
        return FALSE;
    }

    HRESULT hr = E_FAIL;
    IInkStrokes *pStrokes = NULL;

    // Move all strokes
    if (bMoveAllOrSelected)
    {
        hr = m_pInkDisp->get_Strokes(&pStrokes);
    }
    else
    {
        // Move the selected strokes.
        hr = m_pInkOverlay->get_Selection(&pStrokes);
    }

    if (SUCCEEDED(pStrokes->ScaleToRectangle(pInkRectangle)))
    {
        m_pInkOverlay->put_Selection(pStrokes);
    }

    SAFE_RELEASE(pStrokes);

    return (SUCCEEDED(hr) ? TRUE : FALSE);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::MoveSelection(IN FLOAT offsetX, IN FLOAT offsetY)
{
    HRESULT hr = E_FAIL;
    IInkStrokes *pStrokes = NULL;

    if (NULL == m_pInkOverlay)
    {
        return FALSE;
    }

    if ( SUCCEEDED(m_pInkOverlay->get_Selection(&pStrokes)) )
    {
        LONG lCount = 0;
        hr = pStrokes->get_Count(&lCount);
        HDC hDC = ::GetDC(m_hAttachedWnd);
        if ( (lCount > 0) && (NULL != m_pInkRender) && (NULL != hDC) )
        {
            LONG X = (LONG)offsetX;
            LONG Y = (LONG)offsetY;

            RECT rect = {0};
            GetSelectionRect(&rect);
            OffsetRect(&rect, (int)X, (int)Y);
            if ( IsSelectedRectInClientRect(rect) )
            {
                hr = m_pInkRender->PixelToInkSpace((LONG_PTR)hDC, &X, &Y);
                if ( SUCCEEDED(hr) && SUCCEEDED(pStrokes->Move((FLOAT)X, (FLOAT)Y)) )
                {
                    m_pInkOverlay->put_Selection(pStrokes);
                }
            }
        }
    }

    SAFE_RELEASE(pStrokes);

    return (SUCCEEDED(hr) ? TRUE : FALSE);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::SaveToFile(IN LPCWSTR lpfilePath)
{
    if (NULL == m_pInkDisp)
    {
        return FALSE;
    }

    // Create a file specified by lpfilePath
    HANDLE hFile = ::CreateFile(lpfilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if ( INVALID_HANDLE_VALUE == hFile )
    {
        return FALSE;
    }

    CComVariant vData;
    HRESULT hr = m_pInkDisp->Save(IPF_InkSerializedFormat, IPCM_Default, &vData);
    if ( FAILED(hr) )
    {
        CloseHandle(hFile);
        return FALSE;
    }

    BOOL bSuccess = FALSE;
    BYTE *pbData = NULL;
    hr = ::SafeArrayAccessData(vData.parray, (LPVOID*)&pbData);
    if ( SUCCEEDED(hr) )
    {
        DWORD dwWritten = 0;
        // The size of the vData.parray
        ULONG dwSize = vData.parray->rgsabound[0].cElements;
        // Write data to file
        bSuccess = ::WriteFile(hFile, pbData, dwSize, &dwWritten, NULL);
        // Close the handle associated with the file
        CloseHandle(hFile);
    }

    ::SafeArrayUnaccessData(vData.parray);

    return bSuccess;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::LoadFromFile(IN LPCWSTR lpfilePath)
{
    if ( (NULL == m_pInkDisp) || (NULL == m_pInkOverlay) )
    {
        return FALSE;
    }

    BOOL bSuccess = FALSE;
    HRESULT hr = E_FAIL;

    // Open the file specified by lpfilePath
    HANDLE hFile = ::CreateFile(lpfilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if ( INVALID_HANDLE_VALUE == hFile )
    {
        return FALSE;
    }

    m_pInkOverlay->put_Enabled(VARIANT_FALSE);

    SAFE_RELEASE(m_pInkDisp);
    // Create a new instance of IInkDisp interface
    hr = CoCreateInstance(CLSID_InkDisp, NULL, CLSCTX_ALL, IID_IInkDisp, (LPVOID*)&m_pInkDisp);
    if ( FAILED(hr) )
    {
        CloseHandle(hFile);
        return FALSE;
    }

    DWORD dwSize = 0, dwRead = 0;
    dwSize = GetFileSize(hFile, NULL);
    if ( INVALID_FILE_SIZE == dwSize )
    {
        CloseHandle(hFile);
        return FALSE;
    }

    VARIANT vtData;
    VariantInit(&vtData);
    vtData.vt = VT_ARRAY | VT_UI1;
    vtData.parray = ::SafeArrayCreateVector(VT_UI1, 0, dwSize);

    if ( NULL != vtData.parray )
    {
        BYTE *pbData;
        if ( SUCCEEDED(::SafeArrayAccessData(vtData.parray, (LPVOID*)&pbData)) )
        {
            bSuccess = ReadFile(hFile, pbData, dwSize, &dwRead, NULL);
            if ( (TRUE == bSuccess) && SUCCEEDED(m_pInkDisp->Load(vtData)) )
            {
                hr = m_pInkOverlay->putref_Ink(m_pInkDisp);
            }

            bSuccess = (SUCCEEDED(hr) ? TRUE : FALSE);
            ::SafeArrayUnaccessData(vtData.parray);
        }
        ::SafeArrayDestroy(vtData.parray);
    }

    CloseHandle(hFile);
    m_pInkOverlay->put_Enabled(VARIANT_TRUE);

    return bSuccess;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::DrawInks(IN HDC hDC)
{
    HRESULT hr = E_FAIL;

    IInkStrokes *pStrokes = NULL;
    IInkRenderer *pInkRender = NULL;

    if ( (NULL != m_pInkDisp) && (NULL != m_pInkOverlay) )
    {
        hr = m_pInkDisp->get_Strokes(&pStrokes);
        if ( SUCCEEDED(hr) )
        {
            hr = m_pInkOverlay->get_Renderer(&pInkRender);
            if ( SUCCEEDED(hr) )
            {
                hr = pInkRender->Draw((LONG_PTR)hDC, pStrokes);
            }
        }
    }

    SAFE_RELEASE(pStrokes);
    SAFE_RELEASE(pInkRender);
    
    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::GetRecognizerNames(IN vector<BSTR>** pvtNames)
{
    if ( (NULL == pvtNames) || (NULL == m_pInkRecognizers))
    {
        return FALSE;
    }

    LONG lCount = 0;
    m_pInkRecognizers->get_Count(&lCount);
    if (0 == lCount)
    {
        *pvtNames = NULL;
        return FALSE;
    }

    *pvtNames = new vector<TCHAR*>();
    IInkRecognizer *pInkRecognizer = NULL;

    for (int i = 0; i < lCount; i++)
    {
        if (FAILED(m_pInkRecognizers->Item(i, &pInkRecognizer)))
        {
            continue;
        }

        CComVariant vLanguages;
        if (SUCCEEDED(pInkRecognizer->get_Languages(&vLanguages)))
        {
            if ( (VT_ARRAY == (VT_ARRAY & vLanguages.vt)) &&
                 (NULL != vLanguages.parray) &&
                 (0 < vLanguages.parray->rgsabound[0].cElements) )
            {
                BSTR bstrName = NULL;
                if (SUCCEEDED(pInkRecognizer->get_Name(&bstrName)))
                {
                    (*pvtNames)->push_back(bstrName);
                }
            }
        }
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::SetInkDrawAttribute(IN float fNewWidth, IN long newColor)
{
    if (NULL == m_pInkDrawingAttr)
    {
        return FALSE;
    }

    HRESULT hr1 = m_pInkDrawingAttr->put_Color(newColor);
    HRESULT hr2 = m_pInkDrawingAttr->put_Width(fNewWidth);

    return ((SUCCEEDED(hr1) && SUCCEEDED(hr2)) ? TRUE : FALSE);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::SetEditingMode(InkOverlayEditingMode editingMode)
{
    if (NULL == m_pInkOverlay)
    {
        return FALSE;
    }

    HRESULT hr = m_pInkOverlay->put_EditingMode(editingMode);

    if (IOEM_Delete == editingMode)
    {
        m_pInkOverlay->put_EraserMode(IOERM_StrokeErase);
    }

    if (IOEM_Select != editingMode)
    {
        CancelSelection();
    }

    return (SUCCEEDED(hr) ? TRUE : FALSE);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::CancelSelection()
{
    HRESULT hr = E_FAIL;
    // Cancel the selected ink
    CComVariant vt(0);
    IInkStrokes *pInkStroke = NULL;
    if ( NULL != m_pInkDisp )
    {
        hr = m_pInkDisp->CreateStrokes(vt, &pInkStroke);
        if (SUCCEEDED(hr))
        {
            hr = m_pInkOverlay->put_Selection(pInkStroke);
            SAFE_RELEASE(pInkStroke);
        }
    }

    return (SUCCEEDED(hr) ? TRUE : FALSE);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::IsSelectedRectInClientRect(IN const RECT& rect)
{
    RECT clientRect = {0, 0, 0, 0};
    // Here should get the rectangle of attached window.
    // The rectangle maybe changes. The rectangle is got at AttachedHWND function
    // different the value got here. At the present time,
    // I don't know the reason, because the HWND(m_hAttachedWnd) is not changed.
    GetClientRect(m_hAttachedWnd, &clientRect);

    // The selected rectangle has one border is outer the client rectangle.
    BOOL isOuter = ( (rect.left < clientRect.left) || (rect.top < clientRect.top) ||
                     (rect.right > clientRect.right) || (rect.bottom > clientRect.bottom) );

    return !isOuter;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::CreateNewRecogContext(IN IInkRecognizer* pInkRecognizer)
{
    if (NULL == pInkRecognizer)
    {
        return FALSE;
    }

    if (m_pInkRecognizer != pInkRecognizer)
    {
        SAFE_RELEASE(m_pInkRecognizer);
        m_pInkRecognizer = pInkRecognizer;
    }

    HRESULT hr = E_FAIL;
    if (NULL != m_pInkRecogContext)
    {
        SdkInkEvents::AdviseInkEvent(m_pInkRecogContext);
        //IInkRecognitionEventsImpl<SdkInkManager>::DispEventUnadvise(m_pInkRecogContext);
        m_pInkRecogContext->putref_Strokes(NULL);
        SAFE_RELEASE(m_pInkRecogContext);
    }

    hr = pInkRecognizer->CreateRecognizerContext(&m_pInkRecogContext);
    if (FAILED(hr))
    {
        return FALSE;
    }

    SdkInkEvents::AdviseInkEvent(m_pInkRecogContext);

    if (NULL != m_pInkRecogGuide)
    {
        if ( FAILED(m_pInkRecogContext->putref_Guide(m_pInkRecogGuide)) ||
             FAILED(m_pInkRecogContext->put_Factoid(FACTOID_DEFAULT)) ||
             FAILED(m_pInkRecogContext->putref_Strokes(m_pInkStrokes)) )
        {
            return FALSE;
        }
    }
    
    return (SUCCEEDED(hr) ? TRUE : FALSE);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::GetLanguageID(OUT LANGID* langID)
{
    if ( NULL == langID )
    {
        return FALSE;
    }

    *langID = ::GetUserDefaultLangID();
    if (NULL == m_pInkRecognizer)
    {
        return FALSE;
    }

    CComVariant vLangIDs;
    if ( SUCCEEDED(m_pInkRecognizer->get_Languages(&vLangIDs)) &&
         (NULL != vLangIDs.parray) )
    {
        WORD *pwLIDs = NULL;
        if (SUCCEEDED(::SafeArrayAccessData(vLangIDs.parray, (void HUGEP**)&pwLIDs)))
        {
            *langID = pwLIDs[0];
            ::SafeArrayUnaccessData(vLangIDs.parray);
        }
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::SetNewRecognizer(LONG nIndex)
{
    if (NULL == m_pInkRecognizers)
    {
        return FALSE;
    }

    LONG lCount = 0;
    m_pInkRecognizers->get_Count(&lCount);
    if ( (0 == lCount) || (nIndex >= lCount) || (nIndex < 0) )
    {
        return FALSE;
    }

    SAFE_RELEASE(m_pInkRecognizer);
    HRESULT hr = m_pInkRecognizers->Item(nIndex, &m_pInkRecognizer);
    BOOL bOK = CreateNewRecogContext(m_pInkRecognizer);

    GetLanguageID(&m_langID);

    return ((SUCCEEDED(hr) && bOK) ? TRUE : FALSE);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::InsertRecognitionResult()
{
    if ( NULL == m_pHandWrittenTextInsertion )
    {
        return FALSE;
    }

    IInkRecognitionResult *pResult = NULL;
    HRESULT hr = m_pInkStrokes->get_RecognitionResult(&pResult);
    if (SUCCEEDED(hr) && (NULL != pResult))
    {
        hr = m_pHandWrittenTextInsertion->InsertInkRecognitionResult(pResult, m_langID, TRUE);
    }

    return (SUCCEEDED(hr) ? TRUE : FALSE);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::RecogResultItem(LONG nIndex, OUT LPTSTR szRecogResult, DWORD dwSize)
{
    if ( (nIndex < 0) || (nIndex >= (LONG)m_vtRecogResult->size()) )
    {
        return FALSE;
    }

    BSTR strTemp = m_vtRecogResult->at(nIndex);
    wcscpy_s(szRecogResult, dwSize, strTemp);
    // StringCchCopy(szRecogResult, dwSize, strTemp);

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkInkManager::RecognizerItem(LONG nIndex, OUT LPTSTR szRecognizer, DWORD dwSize)
{
    if ( (nIndex < 0) || (nIndex >= (LONG)m_vtRecognizerList->size()) )
    {
        return FALSE;
    }

    TCHAR *pRecognizer = m_vtRecognizerList->at(nIndex);
    wcscpy_s(szRecognizer, dwSize, pRecognizer);

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

LONG SdkInkManager::GetRecogResultCount()
{
    LONG nCount = 0;
    if (m_vtRecogResult != NULL)
    {
        nCount = (LONG)m_vtRecogResult->size();
    }

    return nCount;
}

//////////////////////////////////////////////////////////////////////////

LONG SdkInkManager::GetRecognizerCount()
{
    LONG nCount = 0;
    if (m_vtRecognizerList != NULL)
    {
        nCount = (LONG)m_vtRecognizerList->size();
    }

    return nCount;
}

//////////////////////////////////////////////////////////////////////////

void SdkInkManager::SetRecogNotification(GETRECOGLIST_CALLBACK pGetRecogListFunction, IN HWND hClientWnd)
{
    m_hClientWnd = hClientWnd;
    m_pGetRecoglistCallback = pGetRecogListFunction;
}

//////////////////////////////////////////////////////////////////////////

void SdkInkManager::SetRecognizeGuide(IN InkRecoGuide inkGuide)
{
    if ( (NULL == m_pInkRecogGuide) || (NULL == m_pInkRecogContext) ||
        (NULL == m_hAttachedWnd) || (NULL == m_pInkRender) )
    {
        return;
    }

    HDC hDC = ::GetDC(m_hAttachedWnd);
    m_pInkRender->PixelToInkSpace(
        (LONG)hDC, &inkGuide.rectWritingBox.left, &inkGuide.rectWritingBox.top);

    m_pInkRender->PixelToInkSpace(
        (LONG)hDC, &inkGuide.rectWritingBox.right, &inkGuide.rectWritingBox.bottom);

    m_pInkRender->PixelToInkSpace(
        (LONG)hDC, &inkGuide.rectDrawnBox.left, &inkGuide.rectDrawnBox.top);

    m_pInkRender->PixelToInkSpace(
        (LONG)hDC, &inkGuide.rectDrawnBox.right, &inkGuide.rectDrawnBox.bottom);

    m_pInkRecogGuide->put_GuideData(inkGuide);
    ::ReleaseDC(m_hAttachedWnd, hDC);

    m_pInkRecogGuide->put_Columns(inkGuide.cColumns);
    m_pInkRecogGuide->put_Rows(inkGuide.cRows);
    m_pInkRecogContext->putref_Strokes(NULL);
    m_pInkRecogContext->putref_Guide(m_pInkRecogGuide);
    m_pInkRecogContext->putref_Strokes(m_pInkStrokes);
}

//////////////////////////////////////////////////////////////////////////

void SdkInkManager::Free(IN vector<BSTR>** pvtNames)
{
    if (NULL == pvtNames)
    {
        return;
    }

    vector<BSTR> *pTempVector = *pvtNames;
    if (NULL == pTempVector)
    {
        return;
    }

    typedef vector<BSTR>::iterator vector_item;
    for (vector_item item = pTempVector->begin(); item != pTempVector->end(); item++)
    {
        SysFreeString(*item);
    }

    pTempVector->clear();
    pTempVector = NULL;
    SAFE_DELETE(*pvtNames);
}

//////////////////////////////////////////////////////////////////////////

void SdkInkManager::OnRecognitionWithAlternates(
    IInkRecognitionResult* pIInkRecoResult,
    VARIANT vCustomParam,
    InkRecognitionStatus RecognitionStatus)
{
    UNREFERENCED_PARAMETER(vCustomParam);
    UNREFERENCED_PARAMETER(RecognitionStatus);

    Recognize(pIInkRecoResult);
    // Invoke the call back of the window, notify window to get the recognize result list.
    if (m_pGetRecoglistCallback != NULL)
    {
        m_pGetRecoglistCallback(this, m_hClientWnd);
    }

    HRESULT hr = S_OK;
    if (NULL == m_pHandWrittenTextInsertion)
    {
        hr = CoCreateInstance(
            CLSID_HandwrittenTextInsertion,
            NULL,
            CLSCTX_ALL,
            IID_IHandwrittenTextInsertion,
            (LPVOID*)&m_pHandWrittenTextInsertion);
    }

    hr = pIInkRecoResult->SetResultOnStrokes();
}

//////////////////////////////////////////////////////////////////////////

void SdkInkManager::OnStroke(IInkCursor* pIInkCursor, IInkStrokeDisp* pInkStroke, VARIANT_BOOL* pbCancel)
{
    UNREFERENCED_PARAMETER(pIInkCursor);
    UNREFERENCED_PARAMETER(pbCancel);

    if ( (NULL == m_pInkRecogContext) || (NULL == m_pInkStrokes) || (NULL == pInkStroke) )
    {
        return;
    }

    BOOL bSuccess = FALSE;
    InkOverlayEditingMode editingMode;
    m_pInkOverlay->get_EditingMode(&editingMode);

    switch (editingMode)
    {
    case IOEM_Ink:
        bSuccess = SUCCEEDED(m_pInkStrokes->Add(pInkStroke));
        break;

    case IOEM_Delete:
        bSuccess = TRUE;
        break;
    }

    m_pInkRecogContext->StopBackgroundRecognition();
    if ( (TRUE == bSuccess) && SUCCEEDED(m_pInkRecogContext->putref_Strokes(m_pInkStrokes)) )
    {
        CComVariant vCustomData;
        m_pInkRecogContext->BackgroundRecognizeWithAlternates(vCustomData);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkInkManager::OnSelectionMoved(IInkRectangle *OldSelectionRect)
{
    RECT rect = {0};
    GetSelectionRect(&rect);
    if ( !IsSelectedRectInClientRect(rect) )
    {
        // Move selected strokes to old position.
        MoveTo(OldSelectionRect, FALSE);
    }
}
