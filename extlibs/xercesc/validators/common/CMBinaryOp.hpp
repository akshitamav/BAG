/*
 * Copyright 1999-2001,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Log: CMBinaryOp.hpp,v $
 * Revision 1.1.1.1  2005/07/29 01:44:55  openns
 * Checkin of library and support after second ONSWG meeting
 *
 * Revision 1.6  2004/09/08 13:56:51  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2004/01/29 11:51:21  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.4  2003/05/16 21:43:20  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.3  2003/05/15 18:48:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 14:54:58  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:37  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/05/11 13:27:16  tng
 * Copyright update.
 *
 * Revision 1.2  2001/02/16 14:58:57  tng
 * Schema: Update Makefile, configure files, project files, and include path in
 * certain cpp files because of the move of the common Content Model files.  By Pei Yong Zhang.
 *
 * Revision 1.1  2001/02/16 14:17:29  tng
 * Schema: Move the common Content Model files that are shared by DTD
 * and schema from 'DTD' folder to 'common' folder.  By Pei Yong Zhang.
 *
 * Revision 1.4  2000/03/02 19:55:37  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/24 20:16:47  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/09 21:42:36  abagchi
 * Copyright swat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:02  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:35  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(CMBINARYOP_HPP)
#define CMBINARYOP_HPP

#include <xercesc/validators/common/CMNode.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class CMStateSet;

class CMBinaryOp : public CMNode
{
public :
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
    CMBinaryOp
    (
        const ContentSpecNode::NodeTypes type
        ,     CMNode* const              leftToAdopt
        ,     CMNode* const              rightToAdopt
        ,     MemoryManager* const       manager = XMLPlatformUtils::fgMemoryManager
    );
    ~CMBinaryOp();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const CMNode* getLeft() const;
    CMNode* getLeft();
    const CMNode* getRight() const;
    CMNode* getRight();


    // -----------------------------------------------------------------------
    //  Implementation of the public CMNode virtual interface
    // -----------------------------------------------------------------------
    bool isNullable() const;


protected :
    // -----------------------------------------------------------------------
    //  Implementation of the protected CMNode virtual interface
    // -----------------------------------------------------------------------
    void calcFirstPos(CMStateSet& toSet) const;
    void calcLastPos(CMStateSet& toSet) const;


private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fLeftChild
    //  fRightChild
    //      These are the references to the two nodes that are on either side
    //      of this binary operation. We own them both.
    // -----------------------------------------------------------------------
    CMNode* fLeftChild;
    CMNode* fRightChild;

    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    CMBinaryOp(const CMBinaryOp&);
    CMBinaryOp& operator=(const CMBinaryOp&);
};

XERCES_CPP_NAMESPACE_END

#endif