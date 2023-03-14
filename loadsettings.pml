
set depth_cue=1
set ray_trace_fog=1
set cartoon_discrete_colors, 1
set dot_solvent, 1
set dot_density, 3
set dot_color, white
space cmyk
set sphere_scale, 0.25
set transparency, 0.65
set surface_color, yellow

load receptor.pdb, my_protein


from pymol import cmd

hide, my_protein
show surface, my_protein
color blue, e. n
color red, e. o
color forest, e. s
color orange, e. p

