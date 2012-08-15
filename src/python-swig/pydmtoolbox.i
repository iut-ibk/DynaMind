%module(directors="1", allprotected="1") pydmtoolbox
%feature("director");
%{
    #include <dmcomponent.h>

    #include <dmsystem.h>
    #include <dmattribute.h>
    #include <dmedge.h>
    #include <dmnode.h>
    #include <dmview.h>
    #include <tbvectordata.h>
    #include <rasterdatahelper.h>
    using namespace std;
    using namespace DM;
%}

%include std_vector.i
%include std_string.i
%include std_map.i


%include "../core/dmcomponent.h"
%include "../core/dmsystem.h"
%include "../core/dmattribute.h"
%include "../core/dmedge.h"
%include "../core/dmnode.h"
%include "../core/dmview.h"
%include "../DMToolboxes/tbvectordata.h"
%include "../DMToolboxes/rasterdatahelper.h"

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



