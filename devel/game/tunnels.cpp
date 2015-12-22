
#include "tunnels.h"
#include "logger.h"

#include "physicsmanager.h"

#include "light.h"

CTunnels::CTunnels() {

    int x,y,z;

    mPosition=Ogre::Vector3( 0,0,0 );

    mMaterials.push_back("wall/1");
    mMaterials.push_back("wall/2");
    mMaterials.push_back("wall/3");
    mMaterials.push_back("wall/4");
    mMaterials.push_back("wall/5");
    mMaterials.push_back("wall/6");
    mMaterials.push_back("wall/7");
    mMaterials.push_back("wall/8");
    mMaterials.push_back("wall/9");
    mMaterials.push_back("wall/10");
    mMaterials.push_back("wall/11");

    // float xstart,ystart,zstart=1000;
    float scale=10;

    for( int i=0; i<TUNNEL_MAX_WIDTH+1; i++ ) {
        for( int j=0; j<TUNNEL_MAX_HEIGHT+1; j++ ) {
            for( int k=0; k<TUNNEL_MAX_DEPTH+1; k++ ) {
                grid[i][j][k]=0;
            }

        }

    }

    bool ok;
    int pass;
    int dx,dy,dz;


    x=0;
    y=1;
    z=1;
    grid[x][y][z]=1;
    ok = false;

    while( !ok ) {


        dx=dy=dz=0;
        switch( rand()%6 ) {
            case 0: dx=1; break;
            case 1: dx=-1; break;
            case 2: dz=1; break;
            case 3: dz=-1; break;
            case 4: dy=1; break;
            case 5: dy=-1; break;

        }


        for( int i=0; i < ((rand()%2+1)*2); i++ ) {

            if( grid[x+dx][y+dy][z+dz] ) break;

            if( x+dx<1 || y+dy<1 || z+dz<1 ) break;

            if( (x+dx)<(TUNNEL_MAX_WIDTH-1) && (y+dy)<(TUNNEL_MAX_HEIGHT-1) && (z+dz)<(TUNNEL_MAX_DEPTH-1) ) {
               x+=dx;
               y+=dy;
               z+=dz;
               grid[x][y][z]=1;

            }
            else {

                // if( y+dy>=TUNNEL_MAX_HEIGHT ) break;

                grid[x+dx][y+dy][z+dz]=1;
                ok=true;
                break;
            }


        }


    }



    /* ===================================================


    for( int i=0; i<100; i++ ) {
        grid[x][y][z]=1;

        pass=0;
        ok=false;
        dx=dy=dz=0;

        while( !ok ) {

            if( pass != 1 ) {

                switch( rand()%6 ) {
                    case 0: dx=1; break;
                    case 1: dx=-1; break;
                    case 2: dz=1; break;
                    case 3: dz=-1; break;
                    case 4: dy=1; break;
                    case 5: dy=-1; break;

                }

            }

            if( x+dx >0 && x+dx<=99 && y+dy>0 && y+dy<=99 && z+dz>0 && z+dz<=99 ) {
                ok=true;
                pass++;
                x+=dx;
                y+=dy;
                z+=dz;

            }

//            if( pass > 10 ) break;

        }

        if( !ok ) break;


    }

    =================================================== */

    for( int i=0; i<24; i++ ) mIndices[i]=i;


    mSceneManager=CSceneManager::getInstance()->getOgreSceneManager();
    Ogre::ManualObject* manual;
    Ogre::SceneNode* node;
    Ogre::String name;
    Ogre::Vector3 pos;
    OgreOde::Geometry* geom;
    CPointLight* light;

    int nb=0;

    for( int i=0; i<TUNNEL_MAX_WIDTH; i++ ) {
        for( int j=0; j<TUNNEL_MAX_HEIGHT; j++ ) {
            for( int k=0; k<TUNNEL_MAX_DEPTH; k++ ) {

                if( grid[i][j][k] ) {

                    name="tunnel" + Ogre::StringConverter::toString( nb++);

                    Ogre::ColourValue c( 128+rand()%127/255, 128+rand()%127/255, 128+rand()%127/255 );

                    manual =  mSceneManager->createManualObject( name );

                    // #define OP Ogre::RenderOperation::OT_POINT_LIST
                    #define OP Ogre::RenderOperation::OT_TRIANGLE_LIST
                    // #define OP Ogre::RenderOperation::OT_LINE_LIST

                    //manual->begin( mMaterials[ rand()%11 ], OP );
                    manual->begin( "wall/1", OP  );
                    #undef OP

                    x=y=z=0;

                //    manual->index(0);
                    mPoints[0].x=(x-1)*scale;
                    mPoints[0].y=(y+1)*scale;
                    mPoints[0].z=(z+1)*scale;

                    mPoints[1].x=(x-1)*scale;
                    mPoints[1].y=(y-1)*scale;
                    mPoints[1].z=(z+1)*scale;

                    mPoints[2].x=(x+1)*scale;
                    mPoints[2].y=(y-1)*scale;
                    mPoints[2].z=(z+1)*scale;

                    mPoints[3].x=(x+1)*scale;
                    mPoints[3].y=(y+1)*scale;
                    mPoints[3].z=(z+1)*scale;

                    mPoints[4].x=(x-1)*scale;
                    mPoints[4].y=(y+1)*scale;
                    mPoints[4].z=(z-1)*scale;

                    mPoints[5].x=(x-1)*scale;
                    mPoints[5].y=(y-1)*scale;
                    mPoints[5].z=(z-1)*scale;

                    mPoints[6].x=(x+1)*scale;
                    mPoints[6].y=(y-1)*scale;
                    mPoints[6].z=(z-1)*scale;

                    mPoints[7].x=(x+1)*scale;
                    mPoints[7].y=(y+1)*scale;
                    mPoints[7].z=(z-1)*scale;


                    manual->position( mPoints[0] );
                    pos=-mPoints[0].normalisedCopy();
                    manual->normal( pos );
                    manual->textureCoord( 0,0 );
                    manual->colour(c);

                //    manual->index(1);
                    manual->position( mPoints[1] );
                    pos=-mPoints[1].normalisedCopy();
                    manual->normal( pos );
                    manual->textureCoord( 0,1 );
                    manual->colour(c);

                //    manual->index(2);
                    manual->position( mPoints[2] );
                    pos=-mPoints[2].normalisedCopy();
                    manual->normal( pos );
                    manual->textureCoord( 1,1 );
                    manual->colour(c);

                 //   manual->index(3);
                    manual->position( mPoints[3] );
                    pos=-mPoints[3].normalisedCopy();
                    manual->normal( pos );
                    manual->textureCoord( 1,0 );
                    manual->colour(c);

                 //   manual->index(4);
                    manual->position( mPoints[4] );
                    pos=-mPoints[4].normalisedCopy();
                    manual->normal( pos );
                    manual->textureCoord( 1,0 );
                    manual->colour(c);

                 //   manual->index(5);
                    manual->position( mPoints[5] );
                    pos=-mPoints[5].normalisedCopy();
                    manual->normal( pos );
                    manual->textureCoord( 1,1 );
                    manual->colour(c);

                 //   manual->index(6);
                    manual->position( mPoints[6] );
                    pos=-mPoints[6].normalisedCopy();
                    manual->normal( pos );
                    manual->textureCoord( 0,1 );
                    manual->colour(c);

                 //   manual->index(7);
                    manual->position( mPoints[7] );
                    pos=-mPoints[7].normalisedCopy();
                    manual->normal( pos );
                    manual->textureCoord( 0,0 );
                    manual->colour(c);


                    mNumVertices=0;
                    mNumIndices=0;

                    if( ((i+1)<TUNNEL_MAX_WIDTH ) && !grid[i+1][j][k] ) {
                         manual->quad( 3,7,6,2 );

                        geom=new OgreOde::BoxGeometry(
                            Ogre::Vector3( 1, 2*scale, 2*scale),
                            CPhysicsManager::getInstance()->getPhysicsWorld(),
                            CPhysicsManager::getInstance()->getPhysicsSpace()  );
                        geom->setPosition( Ogre::Vector3( i*2*scale, j*2*scale, k*2*scale )
                            + (mPoints[3]+mPoints[6])/2
                            + Ogre::Vector3( 0.5,0,0 )
                            + mPosition );
                        mGeomList.push_back( geom );
                    }

                    if( ((i-1)>=0) && !grid[i-1][j][k] ) {
                         manual->quad( 4,0,1,5 );

                        geom=new OgreOde::BoxGeometry(
                            Ogre::Vector3( 1, 2*scale, 2*scale),
                            CPhysicsManager::getInstance()->getPhysicsWorld(),
                            CPhysicsManager::getInstance()->getPhysicsSpace()  );
                        geom->setPosition( Ogre::Vector3( i*2*scale, j*2*scale, k*2*scale )
                            + (mPoints[4]+mPoints[1])/2
                            + Ogre::Vector3( -0.5,0,0 )
                            + mPosition );
                        mGeomList.push_back( geom );

                    }

                    if( ((j+1)<TUNNEL_MAX_HEIGHT) && !grid[i][j+1][k] ) {
                        manual->quad( 0,4,7,3 );

                        geom=new OgreOde::BoxGeometry(
                            Ogre::Vector3( 2*scale, 1 , 2*scale),
                            CPhysicsManager::getInstance()->getPhysicsWorld(),
                            CPhysicsManager::getInstance()->getPhysicsSpace()  );
                        geom->setPosition( Ogre::Vector3( i*2*scale, j*2*scale, k*2*scale )
                            + (mPoints[0]+mPoints[7])/2
                            + Ogre::Vector3( 0, 0.5, 0)
                            + mPosition );
                        mGeomList.push_back( geom );


                    }

                    if( ((j-1)>=0) && !grid[i][j-1][k] ) {
                        manual->quad( 1,2,6,5 );

                        geom=new OgreOde::BoxGeometry(
                            Ogre::Vector3( 2*scale, 1 , 2*scale),
                            CPhysicsManager::getInstance()->getPhysicsWorld(),
                            CPhysicsManager::getInstance()->getPhysicsSpace()  );
                        geom->setPosition( Ogre::Vector3( i*2*scale, j*2*scale, k*2*scale )
                            + (mPoints[1]+mPoints[6])/2
                            + Ogre::Vector3( 0, -0.5, 0)
                            + mPosition );
                        mGeomList.push_back( geom );


                    }

                    if( ((k+1)<TUNNEL_MAX_DEPTH) && !grid[i][j][k+1] ) {
                        manual->quad( 0,3,2,1 );

                        geom=new OgreOde::BoxGeometry(
                            Ogre::Vector3( 2*scale, 2*scale, 1 ),
                            CPhysicsManager::getInstance()->getPhysicsWorld(),
                            CPhysicsManager::getInstance()->getPhysicsSpace()  );
                        geom->setPosition( Ogre::Vector3( i*2*scale, j*2*scale, k*2*scale )
                            + (mPoints[0]+mPoints[2])/2
                            + Ogre::Vector3( 0, 0, 0.5 )
                            + mPosition );

                        mGeomList.push_back( geom );


                    }

                    if( ((k-1)>=0) && !grid[i][j][k-1] ) {
                        manual->quad( 4,5,6,7 );
                        geom=new OgreOde::BoxGeometry(
                            Ogre::Vector3( 2*scale, 2*scale, 1 ),
                            CPhysicsManager::getInstance()->getPhysicsWorld(),
                            CPhysicsManager::getInstance()->getPhysicsSpace()  );
                        geom->setPosition( Ogre::Vector3( i*2*scale, j*2*scale, k*2*scale )
                            + (mPoints[4]+mPoints[6])/2
                            + Ogre::Vector3( 0, 0, -0.5 )
                            + mPosition );
                        // geom->setPosition( Ogre::Vector3( mPosition.x + i*2*scale, mPosition.y + j*2*scale, mPosition.z + k*2*scale );
                        mGeomList.push_back( geom );
                    }


                    manual->end();

                    mManualObjList.push_back( manual );


                    /*
                    OgreOde::EntityInformer ei;
                    ei.addMesh( manual->convertToMesh( name + "_mesh" ));
                    OgreOde::Geometry* geom = ei.createStaticTriangleMesh(
                        CPhysicsManager::getInstance()->getPhysicsWorld(),
                        CPhysicsManager::getInstance()->getPhysicsSpace() );

                    geom->disable();
                    mGeomList.push_back( geom );


                    Ogre::MeshManager::getSingleton().unload( name + "_mesh" );
                    Ogre::MeshManager::getSingleton().remove( name + "_mesh" );

                    */

                    /*
                    mGeomList.push_back(new OgreOde::TriangleMeshGeometry(
                        mVertices, mNumVertices, mIndices, mNumIndices,
                         CPhysicsManager::getInstance()->getPhysicsWorld(),
                         CPhysicsManager::getInstance()->getPhysicsSpace()  ));
                    */

                    node = mSceneManager->getRootSceneNode()->createChildSceneNode( name+"_node");
                    node->attachObject(manual);
                    node->setPosition( mPosition.x + i*2*scale, mPosition.y + j*2*scale, mPosition.z + k*2*scale );
                    // node->setPosition( i*2*scale, j*2*scale, k*2*scale );
                    // node->setScale( scale, scale, scale );

                    mNodeList.push_back( node );



                    if( rand()%100 < 10 ) {

                            light = new CPointLight( name + "light", node );
                            light->setColor(
                                (float)(rand()%256/256.f),
                                (float)(rand()%256/256.f),
                                (float)(rand()%256/256.f ));
                            mLightList.push_back( light );


                    }


                }

            }

        }

    }



}

CTunnels::~CTunnels() {

    // LOG("\n\n-----------------------------------------\n");

    LOG("\nCTunnels::~CTunnels() : destroying tunnel lights...");

    for( std::vector<CPointLight*>::iterator i=mLightList.begin(); i!=mLightList.end(); ++i ) {
        delete( (*i) );
    }
    LOG("done.");

    LOG("\nCTunnels::~CTunnels() : destroying tunnel ode geoms...");
    for( std::vector<OgreOde::Geometry*>::iterator i=mGeomList.begin(); i!=mGeomList.end(); ++i ) {
        delete( (*i) );
    }
    LOG("done.");

    LOG("\nCTunnels::~CTunnels() : destroying tunnel nodes...");

    for( std::vector<Ogre::SceneNode*>::iterator i=mNodeList.begin(); i!=mNodeList.end(); ++i ) {
        // LOG("\ndestroying %s", (*i)->getName().c_str() );
        mSceneManager->getRootSceneNode()->removeAndDestroyChild( (*i)->getName() );
//        mSceneManager->destroySceneNode( (*i)->getName() );
    }
    LOG("done.");

    LOG("\nCTunnels::~CTunnels() : destroying manual objects...");
    for( std::vector<Ogre::ManualObject*>::iterator i=mManualObjList.begin(); i!=mManualObjList.end(); ++i ) {
        mSceneManager->destroyManualObject( (*i) );
    }
    LOG("done.");

}


void CTunnels::update( Ogre::Vector3 pos ) {

    for( std::vector<OgreOde::Geometry*>::iterator i=mGeomList.begin(); i!= mGeomList.end(); ++i ) {
        if( ( (*i)->getPosition()-pos ).length() < 60 ) {
             if( !(*i)->isEnabled() ) {
                  (*i)->enable();
                  (*i)->setDebug(true);
             }
        }
        else {
            if( (*i)->isEnabled() ) {
                (*i)->disable();
                (*i)->setDebug(false);
            }
        }

    }

}


