%module(directors="1", allprotected="1") pydynamind
%ignore "DM_HELPER_DLL_EXPORT";

%feature("autodoc", "1");

%feature("director");
%{
    #include <dmsimulation.h>
    #include <dmcomponent.h>
    #include <dmsystem.h>
    #include <dmrasterdata.h>
    #include <dmattribute.h>
    #include <dmedge.h>
    #include <dmnode.h>
    #include <dmface.h>
    #include <dmmodule.h>
    #include <dmview.h>
    #include <dmnodefactory.h>
    #include <dmmoduleregistry.h>
    #include <dmlog.h>
    #include <dmlogger.h>
    #include <dmlogsink.h>
    #include <dmsimulation.h>
    #include <iostream>    
    using namespace std;
    using namespace DM;
%}

%include std_vector.i
%include std_string.i
%include std_map.i
%include cpointer.i

%include "../core/dmcomponent.h"
%include "../core/dmsystem.h"
%include "../core/dmattribute.h"
%include "../core/dmedge.h"
%include "../core/dmface.h"
%include "../core/dmrasterdata.h"
%include "../core/dmnode.h"
%include "../core/dmview.h"
%include "../core/dmlog.h"
%include "../core/dmlogger.h"
%include "../core/dmlogsink.h"
%include "../core/dmsimulation.h"
namespace std {
    %template(stringvector) vector<string>;
    %template(doublevector) vector<double>;
    %template(systemvector) vector<DM::System* >;
    %template(systemmap) map<string, DM::System* >;
    %template(edgevector) vector<DM::Edge* >;
    %template(nodevector) vector<DM::Node* >;
    %template(facevector) vector<DM::Face* >;
    %template(viewvector) vector<DM::View >;
    %template(linkvector) vector<DM::LinkAttribute >;
    %template(viewmap) map<string, vector<DM::View* > >;
    %template(componentvector) vector<DM::Component* >;
    %template(attributevector) vector<DM::Attribute* >;
    %template(attributemap) map<string, DM::Attribute* >;
    %template(componentmap) map<string, DM::Component* >;
    %template(nodemap) map<string, DM::Node* >;
    %template(edgemap) map<string, DM::Edge* >;
    %template(facemap) map<string, DM::Face* >;
    %template(stringmap) map<string, string >;
}

%pointer_class(std::string,p_string)
%pointer_class(int,p_int)
%pointer_class(long,p_long)
%pointer_class(double,p_double)

%feature("director:except") {
    if ($error != NULL) {
                PyErr_Print();
    }
}


    enum  DATATYPES {
        INT,
        LONG,
        DOUBLE,
        STRING,
        FILENAME,
        STRING_MAP,
        BOOL,
        LASTPRIMITIVETYPE,
        //Port Types

        USER_DEFINED_INPUT,

        SYSTEM_OUT,
        SYSTEM_IN,
        SYSTEM
    };

    enum PORTTYPES {
        OUTSYSTEM,
        OUTTUPLESYSTEM,
        OUTPORTS,

        INSYSTEM,
        INTUPLESYSTEM,

        INPORTS
    };

    enum CORINE {
        ContUrbanFabric = 2,
        DisContUrbanFabric = 3,
        RoadRailNetwork = 4,
        AgriculturalAreas = 5,
        ForestsSemiNatural = 6,
        WaterBodies = 7
    };



class Module {

public:
    Module();
    virtual ~Module();
    virtual bool createInputDialog();
    virtual void run() = 0;
    virtual void init();
    virtual std::string getHelpUrl();

    void addData(std::string name, std::vector<DM::View> view);
    std::map<std::string, std::vector<DM::View> >  getViews();
    DM::System * getData(std::string dataname);
    DM::RasterData * getRasterData(std::string dataname, const DM::View & view);



    std::vector<std::string> getParameterListAsVector();    
    virtual std::string getParameterAsString(std::string Name);
    virtual void updateParameter();


    void addParameter(std::string name, int type, void * ref, std::string description);
    virtual void setParameterValue(std::string name, std::string value);

    virtual std::string getHelpUrl();


    virtual const char *getClassName();

    virtual const char *getFileName();
    virtual void setStatus(int s);
};

%extend Module {
    %pythoncode %{
    _data = {'d':'Module'}
    def getClassName(self):
            return self.__class__.__name__

    def getFileName(self):
            return self.__module__.split(".")[0]

    def __getattr__(self, name):
            if name in self._data:
                return self._data[name].value()

    def __setattr__(self, name, value):
            if name in self._data:
                return self._data[name].assign(value)

            return super(Module, self).__setattr__(name, value)

    def createParameter(self,name, DN_type, description):
            if 'd' in self._data:
                if self._data['d'] == 'Module':
                    self._data = {}

            if DN_type == STRING:
                self._data[name] = p_string()
            if DN_type == FILENAME:
                self._data[name] = p_string()
            if DN_type == DOUBLE:
                self._data[name] = p_double()
            if DN_type == LONG:
                self._data[name] = p_long()
            if DN_type == INT:
                self._data[name] = p_int()
            if DN_type == BOOL:
                self._data[name] = p_int()

            self.addParameter(name,DN_type,self._data[name],description)

    %}
    };

%inline %{

void log(std::string s, DM::LogLevel l) {
    DM::Logger(l) << s;
}

void initlog(){
//Init Logger
ostream *out = &cout;
DM::Log::init(new OStreamLogSink(*out), DM::Debug);
DM::Logger(DM::Debug) << "Start";
}

%}

class INodeFactory
{
    public:
        virtual ~INodeFactory(){}
        virtual Module *createNode() const = 0;
        virtual std::string getNodeName() = 0;
        virtual std::string getFileName() = 0;
};

class ModuleRegistry
{
public:
    ModuleRegistry();
    bool addNodeFactory(INodeFactory *factory);
    void addNativePlugin(const std::string &plugin_path);
    Module * createModule(const std::string & name) const;
    std::list<std::string> getRegisteredModules() const;
    bool contains(const std::string &name) const;
};

%pythoncode %{
def my_del(self):
    #print "Force no delete of python garbage collector"
    self.__disown__()

Component.__del__ = my_del
Node.__del__ = my_del
Edge.__del__ = my_del
Face.__del__ = my_del


class NodeFactory(INodeFactory):
    def __init__(self, klass):
        INodeFactory.__init__(self)
        self.klass = klass

    def createNode(self):
        return self.klass().__disown__()

    def getNodeName(self):
        return self.klass().getClassName()

    def getFileName(self):
        if(self.klass().getFileName() == ""):
            return self.klass.__module__.split(".")[0]
        else:
            return self.klass().getFileName()

def registerNodes(registry):
    for klass in Module.__subclasses__():
        registry.addNodeFactory(NodeFactory(klass).__disown__())

%}
