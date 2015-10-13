[Mesh]
  file = circle-quads.e
[]

[Variables]
  [./u]
  [../]
[]

[UserObjects]
  [./montecarlo]
    type = MonteCarloUserObject
    boundary = 1
    max_reflect_count = 10
    particle_count=100000
    absorptivity=1.0
    diffuse_reflectivity=0.5
    mirrors_reflectivity=0.5
  [../]

[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = u
  [../]
[]

[BCs]
  [./out]
    type = DirichletBC
    variable = u
    boundary = 1
    value = 0
  [../]
[]

[Executioner]
  type = Steady
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
