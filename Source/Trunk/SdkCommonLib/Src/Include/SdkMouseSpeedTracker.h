/*! 
* @file SdkMouseSpeedTracker.h 
* 
* @brief This file defines the functions to track mouse speed.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _SDKMOUSESPEEDTRACKER_H_
#define _SDKMOUSESPEEDTRACKER_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_UTILITIES

/*!
* @brief The class defines the functions to track mouse speed.
*/
class CLASS_DECLSPEC SdkMouseSpeedTracker
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkMouseSpeedTracker();

    /*
    * @brief The default destructor function
    */
    virtual ~SdkMouseSpeedTracker();

    /*!
    * @brief Get the X-speed.
    */
    virtual DOUBLE GetSpeedX();

    /*!
    * @brief Get the Y-speed.
    */
    virtual DOUBLE GetSpeedY();

    /*!
    * @brief Get the speed sample time.
    */
    virtual DWORD GetSpeedSampleTime();

    /*
    * @brief The mouse event procedure.
    *
    * @param hWnd       [I/ ] The handle to the window which occurs the message.
    * @param message    [I/ ] The message.
    * @param lParam     [I/ ] The high bit of parameter.
    * @param wParam     [I/ ] The low bit of parameter.
    */
    virtual void OnMouseEvent(HWND hWnd, UINT32 message, WPARAM wParam, LPARAM lParam);

private:

    DWORD       m_dwElapseTime;     // The elapse timer.
    POINT       m_deltaDist;        // The delta between first point and second point.
    POINT       m_firstPT;          // The first point of mouse.
    POINT       m_secondPT;         // The second point of mouse.
    BOOL        m_isLBDown;         // Indicates whether left button pressed down.
    DOUBLE      m_dSpeedX;          // The X-Speed.
    DOUBLE      m_dSpeedY;          // The Y-Speed.
};

END_NAMESPACE_UTILITIES

#endif // _SDKMOUSESPEEDTRACKER_H_
#endif // __cplusplus
