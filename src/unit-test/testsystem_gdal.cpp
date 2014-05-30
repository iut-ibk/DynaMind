/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of DynaMind
 *
 * Copyright (C) 2014  Christian Urich

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "testsystem_gdal.h"


#include <dmsimulation.h>
#include <dmsystem.h>
#include <dmlogger.h>
#include <dmlogsink.h>
#include <dmcomponent.h>
#include <dmnode.h>

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <QTimer>


namespace DM {

//TEST_F(TestSystem_GDAL, addNode) {

//	ostream *out = &cout;
//	DM::Log::init(new DM::OStreamLogSink(*out), DM::Debug);

//	DM::Logger(DM::Standard) << "Test GDAL System";
//	DM::System sys;
//    OGRFieldDefn oField( "test", OFTReal );
//    sys.getComponentLayer()->CreateField(&oField);

//	DM::Component cmp;
//	sys.addComponent(&cmp);

//    cmp.addAttribute("test", 100.);


//    Logger(Debug) << cmp.getAttribute("test")->getDouble();

//    /* initialize random seed: */
//    srand (time(NULL));

//    QTime myTimer;
//    myTimer.start();

//    int counter = 0;
//    sys.getNodeLayer()->StartTransaction();
//    for (long i = 0; i < 1000000 ; i++){
//        DM::Node node;
//        sys.addNode(&node);
//        node.set(rand() % 10000 + 1,rand() % 10000 + 1,rand() % 10000 + 1);
//        counter++;
//        if (counter==10000) {
//            sys.getNodeLayer()->CommitTransaction();
//            sys.getNodeLayer()->StartTransaction();
//            counter = 0;
//            //Logger(Debug) << i;
//        }
//    }
//    sys.getNodeLayer()->CommitTransaction();

//    Logger(Debug) << myTimer.elapsed();
//	Logger(Debug) << "Done";
//}

//10.000.000 Memory  54508 ms 1.91 GB
//10.000.000 SQLite 312392 ms 634.7 MB
//10.000.000 SQLite 254157 ms  17.1 MB //Without indizes

//1.000.000 SQLite  28057 ms 79.6 MB
//1.000.000 SQLite  25663 ms 17.1 MB //Without indizes
//1.000.000 SQLite 437206 ms 151.1 MB //Without indizes (uuid as string)
//1.000.000 SQLite  36360 ms 76.8 MB //Without indizes (uuid as Quuid)



TEST_F(TestSystem_GDAL, addNodeView) {

    ostream *out = &cout;
    DM::Log::init(new DM::OStreamLogSink(*out), DM::Debug);

    DM::Logger(DM::Standard) << "Test GDAL System";
    DM::System sys;
    OGRFieldDefn oField( "test", OFTReal );
    sys.getComponentLayer()->CreateField(&oField);

    DM::Component cmp;
    sys.addComponent(&cmp);

    cmp.addAttribute("test", 100.);


    Logger(Debug) << cmp.getAttribute("test")->getDouble();

    /* initialize random seed: */
    srand (time(NULL));

    QTime myTimer;
    myTimer.start();

    int counter = 0;
    sys.getNodeLayer()->StartTransaction();
    DM::View n_view("test", DM::NODE, DM::WRITE);
    for (long i = 0; i < 10000000 ; i++){
        DM::Node node;
        DM::Node * n = sys.addNode(node, n_view);
        n->set(rand() % 10000 + 1,rand() % 10000 + 1,rand() % 10000 + 1);
        counter++;
        delete n;
        if (counter==10000) {
            sys.getNodeLayer()->CommitTransaction();
            sys.getNodeLayer()->StartTransaction();
            counter = 0;
            //Logger(Debug) << i;
        }
    }
    sys.getNodeLayer()->CommitTransaction();

    Logger(Debug) << myTimer.elapsed();

    myTimer.start();
    Logger(Debug) << "Reading";
    DM::System::ViewCache * vc = sys.getViewCache(n_view);
    Logger(Debug) << "in view" << (int) vc->getNumberOfElements();
    long elements = vc->getNumberOfElements();
    long totalL = 0;
    for (long i = 0; i < elements; i++) {
        DM::Component * cmp = vc->getElement(i);
        totalL+=cmp->getAttribute("dynamind_id")->getString().length();
        delete cmp;
    }
    Logger(Debug) << totalL;
    Logger(Debug) << myTimer.elapsed();
    Logger(Debug) << "Done";



}
}

//1.000.000 SQLite  42101 ms 121.6 MB //no acceess

//10.000.000 SQLite write 417.047 ms 900 MB read 464.160 //no acceess
