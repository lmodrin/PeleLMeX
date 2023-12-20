get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(pele_physics_lib_name "PelePhysicsLib-${PELE_PHYSICS_EOS_MODEL}-${PELE_PHYSICS_CHEMISTRY_MODEL}-${PELE_PHYSICS_TRANSPORT_MODEL}-Spray${PELE_PHYSICS_ENABLE_SPRAY}-Soot${PELE_PHYSICS_ENABLE_SOOT}-Radiation${PELE_PHYSICS_ENABLE_RADIATION}")
set(pele_exe_name "${PROJECT_NAME}-${DIR_NAME}")
include(BuildPelePhysicsLib)
include(BuildPeleExe)
build_pele_physics_lib(${pele_physics_lib_name})
build_pele_exe(${pele_exe_name} ${pele_physics_lib_name})
