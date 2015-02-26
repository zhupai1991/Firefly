#include "GopherApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"

template<>
InputParameters validParams<GopherApp>()
{
  InputParameters params = validParams<MooseApp>();

  params.set<bool>("use_legacy_uo_initialization") = false;
  params.set<bool>("use_legacy_uo_aux_computation") = false;
  return params;
}

GopherApp::GopherApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(processor_id());

  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  GopherApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  GopherApp::associateSyntax(_syntax, _action_factory);
}

GopherApp::~GopherApp()
{
}

extern "C" void GopherApp__registerApps() { GopherApp::registerApps(); }
void
GopherApp::registerApps()
{
  registerApp(GopherApp);
}

void
GopherApp::registerObjects(Factory & factory)
{
}

void
GopherApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}
