#ifndef FIREFLYAPP_H
#define FIREFLYAPP_H

#include "MooseApp.h"

class FireflyApp;

template<>
InputParameters validParams<FireflyApp>();

class FireflyApp : public MooseApp
{
public:
  FireflyApp(InputParameters parameters);
  virtual ~FireflyApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* FIREFLYAPP_H */
