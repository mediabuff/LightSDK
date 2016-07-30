/*!
* @file FilePropertyDescription.h
* 
* @brief This file define functions to get shell property description.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/03/29
*/

#ifdef __cplusplus
#ifndef _PROPERTYDESCRIPTION_H_
#define _PROPERTYDESCRIPTION_H_

#include "Common.h"
#include "CommonMacro.h"
#include "FilePropertyKey.h"
#include "FilePropertyKeyHelper.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief The definition of property key to property description map.
*/
typedef map<PROPERTYKEYNAME, IPropertyDescription*> PROPKEYTOPROPDESCMAP;

/*!
* @brief The FilePropertyDescription class is used to get property descriptions.
*/
class CLASS_DECLSPEC FilePropertyDescription
{
public:

    /*!
    * @brief The constructor function.
    */
    FilePropertyDescription();

    /*!
    * @brief The destructor function.
    */
    ~FilePropertyDescription();

    /*!
    * @brief Get the property description .
    *
    * @param lpszPropDesc       [ /O] The property description.
    * @param uDescSize          [I/ ] The buffer size.
    *
    * @return TRUE if succeed to get value, otherwise return FALSE.
    */
    BOOL GetPropDescription(IN PROPERTYKEYNAME keyName, OUT LPWSTR lpszPropDesc, IN UINT32 uDescSize = MAX_PATH);

private:

    /*!
    * @brief Get the property description and property value.
    *
    * @param pPropDesc          [I/ ] The IPropertyDescription instance.
    * @param lpszPropDesc       [ /O] The property description.
    * @param uDescSize          [I/ ] The buffer size.
    *
    * @return TRUE if succeed to get value, otherwise return FALSE.
    */
    BOOL GetPropDesc(IN IPropertyDescription *pPropDesc, OUT LPWSTR lpszDesc, UINT32 uDescSize);

private:

    PROPKEYTOPROPDESCMAP m_mapPropKeyToPropDesc;        // The property key to property description map.
};

END_NAMESPACE

#endif // _PROPERTYDESCRIPTION_H_
#endif // __cplusplus
