%module(directors="1", allprotected="1") pydmtoolbox
%feature("director");
%{
    #include <DMcomponent.h>
    #include <DMsystem.h>
    #include <DMattribute.h>
    #include <DMedge.h>
    #include <DMnode.h>
    #include <DMview.h>
    #include <tbvectordata.h>
    using namespace std;
    using namespace DM;

%}

%include std_vector.i
%include std_string.i
%include std_map.i


%include "../core/DMcomponent.h"
%include "../core/DMsystem.h"
%include "../core/DMattribute.h"
%include "../core/DMedge.h"
%include "../core/DMnode.h"
%include "../core/DMview.h"
%include "../DMToolboxes/tbvectordata.h"

namespace std {
    %template(stringvector) vector<string>;
    %template(doublevector) vector<double>;
    %template(systemvector) vector<DM::System* >;
    %template(systemmap) map<string, DM::System* >;
    %template(edgevector) vector<DM::Edge* >;
    %template(nodevector) vector<DM::Node* >;
    %template(viewvector) vector<DM::View >;
    %template(componentvector) vector<DM::Component* >;
    %template(attributevector) vector<DM::Attribute* >;
    %template(attributemap) map<string, DM::Attribute* >;
    %template(componentmap) map<string, DM::Component* >;
    %template(nodemap) map<string, DM::Node* >;
    %template(edgemap) map<string, DM::Edge* >;
    %template(stringmap) map<string, string >;
}

%feature("director:except") {
    if ($error != NULL) {
                PyErr_Print();
    }
}


