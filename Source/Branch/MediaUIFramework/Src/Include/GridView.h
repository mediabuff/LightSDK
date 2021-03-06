/*!
* @file GridView.h 
* 
* @brief This file defines the grid view 
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/06
*/

#ifdef __cplusplus
#ifndef _GRIDVIEW_H_
#define _GRIDVIEW_H_

#include "SlideLayout.h"

/*!
* @brief This class defines the GridView.
*/
class CLASS_DECLSPEC GridView : public SlideLayout
{
public:

    /*!
    * @brief The constructor function.
    */
    GridView();

    /*!
    * @brief The destructor function.
    */
    virtual ~GridView();

    /*!
    * @brief Set the layout information of the view.
    *
    * @param x          [I/ ] The x coordinate of left.
    * @param y          [I/ ] The y coordinate of left.
    * @param width      [I/ ] The width coordinate of left.
    * @param height     [I/ ] The height coordinate of left.
    */
    virtual void SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height);

    /*!
    * @brief Set the flag to indicate whether the grid view updates size automatically according
    *        to specified row and column.
    *
    * @param isAutoSize [I/ ] TRUE if auto size, otherwise FALSE.
    *
    * @remark You must call RequestLayout function to make change take effect.
    */
    virtual void SetAutoSize(BOOL isAutoSize = FALSE);

    /*!
    * @brief Set the elements' size.
    *
    * @param fWidth     [I/ ] The width.
    * @param fHeight    [I/ ] The height.
    *
    * @remark You must call RequestLayout function to make change take effect.
    */
    virtual void SetChildSize(FLOAT fWidth, FLOAT fHeight);

    /*!
    * @brief Get the elements' size.
    *
    * @param pfWidth   [ /O] The width.
    * @param pfHeight  [ /O] The height.
    *
    * @remark You must call RequestLayout function to make change take effect.
    */
    virtual void GetChildSize(FLOAT *pfWidth, FLOAT *pfHeight);

    /*!
    * @brief Set the row count of grid.
    *
    * @param uRowCount  [I/ ] The row count.
    *
    * @remark You must call RequestLayout function to make change take effect.
    */
    virtual void SetRowCount(UINT32 uRowCount);

    /*!
    * @brief Set the column count of grid.
    *
    * @param uColCount  [I/ ] The column count.
    *
    * @remark You must call RequestLayout function to make change take effect.
    */
    virtual void SetColCount(UINT32 uColCount);

protected:

    /*!
    * @brief Calculate the size of grid view itself.
    *
    * @param pWidth     [ /O] The fit width.
    * @param pHeight    [ /O] The fit height.
    */
    virtual void CalcGridViewSize(FLOAT *pWidth, FLOAT *pHeight);

    /*!
    * @brief Calculate child views' start and end index.
    *        Derived class should implement this function to tell rendering views' index.
    *
    * @param pStartIndex    [ /O] The start index of first rendering view.
    * @param pEndIndex      [ /O] The end index of last rendering view.
    */
    virtual void CalcChildViewIndex(OUT INT32 *pStartIndex, OUT INT32 *pEndIndex);

    /*!
    * @brief Called when the layout of view is changed.
    *
    * @param bChanged           [I/ ] Indicates whether is changed.
    * @param left               [I/ ] The left value.
    * @param top                [I/ ] The top value.
    * @param width              [I/ ] The width of view.
    * @param height             [I/ ] The height of view.
    */
    virtual void OnLayout(BOOL bChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height);

protected:

    BOOL        m_isAutoSize;       // Flag to indicates auto size.
    UINT32      m_uRowCount;        // The row count.
    UINT32      m_uColCount;        // The column count.
    FLOAT       m_fLMargin;         // The left margin.
    FLOAT       m_fRMargin;         // The right margin.
    FLOAT       m_fChildWidth;      // The width of children.
    FLOAT       m_fChildHeight;     // The height of children.
};

#endif // _GRIDVIEW_H_
#endif // __cplusplus
