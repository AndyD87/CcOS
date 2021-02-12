# CodingGuide

Coding guidelines for CcOS and supported projects.

The guidlines was created after many lines of code. So there is many code in projects which is not fomated right.

Basic coding guidelines described as code, says more than 1000 words:

    CcByteArray ATTRIBUTES example(const char* pString, const CcString& sString)
    {
      CcByteArray oArray;
      if(pString)
      {
        sString = pString;
        oArray.set(CCMOVE(sString));
      }
      return oArray;
    }

## IDEs

For IDEs were templates generated which can be imported to apply the coding guides to their formaters:
 - https://coolcow.de/projects/CcCodeStyles.git

Avialable IDEs:
 - QtCreator
 - VisualStudio
 - Eclipse
 - ...

## classes

Example:

    class CcExample : public CcBaseExample
                      public CcBase
    {
      
    }

## new and delete

Makros are used for memory functions.

**new** is replaced by CCNEW  
**delete** is replaced by CCDELETE

### CCNEW

### CCDELETE

## Header guard

In earlier versions of CcOS the header guard was realized with **\#ifndef** and **\#define**.  
It has changed to use the more readable version from MSVC **\#pragma once**.  
No compiler will be supported which does not support this command.

New Header files have to use this preprocessor commands:

    /**
     * Header info
     */
    #pragma once

It has to be placed afer header informations for file.
