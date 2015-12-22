//
// Ogre CSG Lib ported by C.O.Park (copjoker@naver.com)
//
// Licence : Free to use, as-it-is,
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.
//
// Ogre CSGLib: http://blog.naver.com/copjoker/120063626465
// CSG Base Code:
// Java   Code: http://www.geocities.com/danbalby/
// C++ Version: http://code.google.com/p/csgtestworld/
//

#ifndef FF_CV_CSGOGREWRAPPER_H
#define FF_CV_CSGOGREWRAPPER_H

#include "Ogre.h"
#include "string"

class Ogre::Entity;

enum evCSG_OPER
{
	CSG_UNION,
	CSG_INTERSECTION,
	CSG_DIFFERENCE
};

void cvEntityBoolean(const std::string& MeshName, Ogre::Entity *Ent1, Ogre::Entity *Ent2, evCSG_OPER Op);
void cvEntityUnion(const std::string& MeshName, Ogre::Entity *Ent1, Ogre::Entity *Ent2);
void cvEntityIntersection(const std::string& MeshName, Ogre::Entity *Ent1, Ogre::Entity *Ent2);
void cvEntityDifference(const std::string& MeshName, Ogre::Entity *Ent1, Ogre::Entity *Ent2);

#endif	// FF_CV_CSGOGREWRAPPER_H
