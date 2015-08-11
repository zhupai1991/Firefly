#ifndef GOPHERAPP_H
#define GOPHERAPP_H

#include "MooseApp.h"

class GopherApp;

template<>
InputParameters validParams<GopherApp>();

class GopherApp : public MooseApp
{
public:
  GopherApp(InputParameters parameters);
  virtual ~GopherApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* GOPHERAPP_H */
