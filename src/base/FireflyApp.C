#include "FireflyApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MonteCarloUserObject.h"
#include "MooseSyntax.h"
//#include "ModulesApp.h"

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
//  ModulesApp::registerObjects(_factory);
  FireflyApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
//  ModulesApp::associateSyntax(_syntax, _action_factory);
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

#undef  registerApp
#define registerApp(name) AppFactory::instance().reg<name>(#name)
  registerApp(FireflyApp);
#undef  registerApp
#define registerApp(name) AppFactory::instance().regLegacy<name>(#name)
}

// External entry point for dynamic object registration
extern "C" void FireflyApp__registerObjects(Factory & factory) { FireflyApp::registerObjects(factory); }
void
FireflyApp::registerObjects(Factory & factory)
{
#undef registerObject
#define registerObject(name) factory.reg<name>(stringifyName(name))

	registerUserObject(MonteCarloUserObject);
#undef registerObject
#define registerObject(name) factory.regLegacy<name>(stringifyName(name))
}

// External entry point for dynamic syntax association
extern "C" void FireflyApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory) { FireflyApp::associateSyntax(syntax, action_factory); }
void
FireflyApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{

#undef registerAction
#define registerAction(tplt, action) action_factory.reg<tplt>(stringifyName(tplt), action)

#undef registerAction
#define registerAction(tplt, action) action_factory.regLegacy<tplt>(stringifyName(tplt), action)
}
