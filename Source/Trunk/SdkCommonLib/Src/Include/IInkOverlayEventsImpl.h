/*!
* @file IInkOverlayEventsImpl.h
* 
* @brief The template is derived from the ATL's IDispEventSimpleImpl to implement a sink for the 
*        IInkCollectorEvents, fired by the InkCollector object Since the IDispEventSimpleImpl doesn't require 
*        to supply implementation code for every event, this template has a handler for the Gesture event only.
*
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2009/10/19 12:00
* @version 1.0.0
*/

#ifdef __cplusplus
#ifndef _IINKCOLLECTOREVENTSIMPLE_H_
#define _IINKCOLLECTOREVENTSIMPLE_H_

#include "SdkInkCommon.h"

BEGIN_NAMESPACE_COMMON

#define SINK_ID                 1
#define COUNT_OVERLAY_EVENTS    7

template <class T>
class ATL_NO_VTABLE IInkOverlayEventsImpl :
    public IDispEventSimpleImpl<SINK_ID, IInkOverlayEventsImpl<T>, &(__uuidof(_IInkOverlayEvents))>
{
public:
    // ATL structures with the type information for each event, 
    // handled in this template.(Initialized in the AdvReco.cpp)
    static const _ATL_FUNC_INFO mc_AtlFuncInfo[COUNT_OVERLAY_EVENTS];

    BEGIN_SINK_MAP(IInkOverlayEventsImpl)
        SINK_ENTRY_INFO(SINK_ID, 
        __uuidof(_IInkOverlayEvents), 
        DISPID_ICEStroke, 
        Stroke, 
        const_cast<_ATL_FUNC_INFO*>(&mc_AtlFuncInfo[0]))

        SINK_ENTRY_INFO(SINK_ID, 
        __uuidof(_IInkOverlayEvents), 
        DISPID_ICEGesture, 
        Gesture,
        const_cast<_ATL_FUNC_INFO*>(&mc_AtlFuncInfo[1]))

        SINK_ENTRY_INFO(SINK_ID, 
        __uuidof(_IInkOverlayEvents), 
        DISPID_IOESelectionMoved, 
        SelectionMoved,
        const_cast<_ATL_FUNC_INFO*>(&mc_AtlFuncInfo[2]))

        SINK_ENTRY_INFO(SINK_ID, 
        __uuidof(_IInkOverlayEvents), 
        DISPID_IOESelectionResized, 
        SelectionResized,
        const_cast<_ATL_FUNC_INFO*>(&mc_AtlFuncInfo[3]))

        SINK_ENTRY_INFO(SINK_ID, 
        __uuidof(_IInkOverlayEvents), 
        DISPID_ICENewPackets, 
        NewPackets,
        const_cast<_ATL_FUNC_INFO*>(&mc_AtlFuncInfo[4]))

        SINK_ENTRY_INFO(SINK_ID, 
        __uuidof(_IInkOverlayEvents), 
        DISPID_IOESelectionChanged, 
        SelectionChanged, 
        const_cast<_ATL_FUNC_INFO*>(&mc_AtlFuncInfo[5]))

        SINK_ENTRY_INFO(SINK_ID, 
        __uuidof(_IInkOverlayEvents), 
        DISPID_IOEStrokesDeleting, 
        StrokesDeleting, 
        const_cast<_ATL_FUNC_INFO*>(&mc_AtlFuncInfo[6]))
    END_SINK_MAP()

    /*!
    * @brief Occurs when the user draws a new stroke on any tablet.
    */
    void __stdcall Stroke(IInkCursor* pIInkCursor, IInkStrokeDisp* pInkStroke, VARIANT_BOOL* pbCancel)
    {
        T* pT = static_cast<T*>(this);
        pT->OnStroke(pIInkCursor, pInkStroke, pbCancel);
    }

    /*!
    * @brief Occurs when an application-specific gesture is recognized.
    */
    void __stdcall Gesture(IInkCursor* pIInkCursor, IInkStrokes* pInkStrokes, VARIANT vGestures, VARIANT_BOOL* pbCancel)
    {
        T* pT = static_cast<T*>(this);
        pT->OnGesture(pIInkCursor, pInkStrokes, vGestures, pbCancel);
    }

    /*!
    * @brief Occurs when an application-specific gesture is recognized.
    */
    void __stdcall SelectionMoved(IInkRectangle *OldSelectionRect)
    {
        T* pT = static_cast<T*>(this);
        pT->OnSelectionMoved(OldSelectionRect);
    }

    /*!
    * @brief Occurs when the size of the current selection has changed, for example through
    *        alterations to the user interface, cut-and-paste procedures, or the Selection property.
    */
    void __stdcall SelectionResized(IInkRectangle *OldSelectionRect)
    {
        T* pT = static_cast<T*>(this);
        pT->OnSelectionResized(OldSelectionRect);
    }

    /*!
    * @brief Occurs when the ink collector receives packet.
    */
    void __stdcall NewPackets(IInkCursor *Cursor, IInkStrokeDisp *Stroke, long PacketCount, VARIANT *PacketData)
    {
        T* pT = static_cast<T*>(this);
        pT->OnNewPackets(Cursor, Stroke, PacketCount, PacketData);
    }

    /*!
    * @brief This function can be called when the selection of ink within the control has changed.
    */
    void __stdcall SelectionChanged()
    {
        T* pT = static_cast<T*>(this);
        pT->OnSelectionChanged();
    }

    /*!
    * @brief This function can be called before strokes are deleted from the Ink property.
    */
    void __stdcall StrokesDeleting(IInkStrokes *Strokes)
    {
        T* pT = static_cast<T*>(this);
        pT->OnStrokesDeleting(Strokes);
    }
};

END_NAMESPACE_COMMON

#endif // IINKCOLLECTOREVENTSIMPLE__H
#endif // __cplusplus