[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 1
  ny = 1
  nz = 1
[]

[Variables]
  [./u]
  [../]
[]

[UserObjects]
  [./montecarlo]
    type = MonteCarloUserObject
    boundary = '0 1 2 3 4 5'
    max_reflect_count = 10
    particle_count=10000000
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
  [./left]
    type = DirichletBC
    variable = u
    boundary = left
    value = 0
  [../]
  [./right]
    type = DirichletBC
    variable = u
    boundary = right
    value = 1
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
