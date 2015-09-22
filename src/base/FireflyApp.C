#include "FireflyApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"

template<>
InputParameters validParams<FireflyApp>()
{
  InputParameters params = validParams<MooseApp>();

  params.set<bool>("use_legacy_uo_initialization") = false;
  params.set<bool>("use_legacy_uo_aux_computation") = false;
  params.set<bool>("use_legacy_output_syntax") = false;

  return params;
}

FireflyApp::FireflyApp(InputParameters parameters) :
    MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  FireflyApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  FireflyApp::associateSyntax(_syntax, _action_factory);
}

FireflyApp::~FireflyApp()
{
}

// External entry point for dynamic application loading
extern "C" void FireflyApp__registerApps() { FireflyApp::registerApps(); }
void
FireflyApp::registerApps()
{
  registerApp(FireflyApp);
}

// External entry point for dynamic object registration
extern "C" void FireflyApp__registerObjects(Factory & factory) { FireflyApp::registerObjects(factory); }
void
FireflyApp::registerObjects(Factory & factory)
{
}

// External entry point for dynamic syntax association
extern "C" void FireflyApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory) { FireflyApp::associateSyntax(syntax, action_factory); }
void
FireflyApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}
